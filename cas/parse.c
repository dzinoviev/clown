#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <setjmp.h>
#include <sys/types.h>
#include "cas.h"
#include "generate.h"

#ifdef __STRICT_ANSI__
int fileno(FILE *stream);	/* for Ansi C  */
#endif

extern int success;
Clof_Type module_type = CLOF_UNKNOWN;
int listing = 0;
int global_offset = 0;
struct SegmentTable segments = {0, NULL};
struct LabelTable   labels   = {0, NULL};

void component_error(const char *name, const char *msg, char *detail)
{
  fprintf(stderr, "%s: %s: %s\n", name, msg, detail);
}

static int search_segment(char *name)
{
  for (int i = 0; i < segments.size; i++)
    if (! (strcmp(segments.segments[i].name, name)))
      return i;
  return NOT_FOUND;
}

static int create_segment(char *name)
{
  int seg_id = segments.size;
  segments.segments = safe_realloc(segments.segments, 
				   sizeof(struct Segment) 
				   * (segments.size + 1));

  struct Segment *seg = &segments.segments[seg_id];
  seg->name = safe_malloc(strlen(name) + 1);

  strcpy(seg->name, name);
  seg->defined = 0;
  seg->new_loc = NOT_FOUND;
  seg->code_size = 0;
  
  /* Support for debugging */
  seg->nfiles = 1;
  seg->files = safe_malloc(sizeof(struct DebugFile));
  seg->files[0].file = *source;
  seg->files[0].nlines = 0;
  seg->files[0].nlines_inuse = 0;
  seg->files[0].flines = NULL;

  seg->id = seg_id;
  seg->new_loc = seg_id;

  segments.size++;
 
  if (seg_id)
    segments.segments[DEFAULT_SEGMENT].defined = 0;

  return seg_id;
}

int lookup_segment(char *name)
{
  int seg = search_segment(name);
  if (seg == NOT_FOUND)
    return create_segment(name);
  else
    return seg;
}

int begin_segment(int type, char *name)
{
  int seg_id = search_segment(name);

  if (NOT_FOUND != seg_id) {
    if (segments.segments[seg_id].defined) {
      component_error(*source, "duplicate segment definition", name);
      return NOT_FOUND;
    }
  } else {
    seg_id = create_segment(name);
    if(NOT_FOUND == seg_id)
      return NOT_FOUND;
  }

  /* define segment */
  struct Segment *seg = &segments.segments[seg_id];
  seg->type = type;
  seg->defined = 1;
  seg->image = safe_malloc(sizeof(Dword) * IMAGE_CHUNK);
  seg->image_size = 0;
  seg->image_extent = IMAGE_CHUNK;
  //  seg->link_overhead = 0;

  return seg_id;
}

static int lookup_label(char *label_name)
{
  for (int i = 0; i < labels.size; i++)
    if (! (strcmp(labels.labels[i].name, label_name)))
      return i;
  return NOT_FOUND;
}

static int create_label(char *label_name)
{
  int i = labels.size;
  labels.labels = safe_realloc(labels.labels, 
			       sizeof(struct Label) * (labels.size + 1));

  struct Label *label = &labels.labels[i];
  label->name = safe_malloc(strlen(label_name) + 1);  

  strcpy(label->name, label_name);
  label->export = 0; /* can be exported? */
  label->defined = 0;
  label->segment = DEFAULT_SEGMENT;
  label->new_loc = i;

  labels.size++;
  return i;
}

int mark_export_label(char *label_name)
{
  int lab;
    
  if (NOT_FOUND != (lab = lookup_label(label_name)))
    component_error(*source, "warning: duplicate export definition", label_name);
  else 
    if (NOT_FOUND != (lab = create_label(label_name)))
      labels.labels[lab].export = 1;

  return lab;
}

static void define_label(int i, int segment, Uword label_offset)
{
  labels.labels[i].address = label_offset;
  labels.labels[i].segment = segment;
  labels.labels[i].defined = 1;
}

int add_label(char *label, int segment, Uword label_offset,
	      int global, int align8)
{
  int label_id;

  if (NOT_FOUND == (label_id = lookup_label(label)) &&
      NOT_FOUND == (label_id = create_label(label)))
    return NOT_FOUND;

  if (labels.labels[label_id].defined) {
    component_error(*source, "duplicate symbol", label);
    return NOT_FOUND;
  }
	
  if (align8) { // Page Alignment
    int boundary = (align8 == 1) ? 8 : CLOWN_FRAME_SIZE;
    if (label_offset % boundary) {
      Uword label_offset1 = (label_offset / boundary + 1) * boundary;
      for (unsigned i = 0; i < label_offset1 - label_offset; i++)
	emit(BUILD_INSTRUCTION_A(NOP, 0, 0));
      label_offset = label_offset1;
    }
  }
    
  if (global)
    labels.labels[label_id].export = 1;

  define_label(label_id, segment, label_offset);
    
  return label_id;
}

int use_label(char *label_name, int segment)
{
  int label;
  if (NOT_FOUND == (label = lookup_label(label_name))) {
    label = create_label(label_name);
    if (NOT_FOUND != label)
      labels.labels[label].segment = segment;
  } 
  return label;    
}

static int find_undefined_segments()
{
  int status = 1;

  for (int i = 0; i < segments.size; i++) {
    if (!segments.segments[i].defined && i != DEFAULT_SEGMENT &&
	module_type != CLOF_EXE) {
      component_error(*source, "undefined segment", segments.segments[i].name);
      status = 0;
    }
  }
    
  return status;
}

static int find_undefined_labels()
{
  int status = 1;

  for (int i = 0; i < labels.size; i++) {
    if (labels.labels[i].defined) {
      if (labels.labels[i].export && module_type != CLOF_EXE) {
	component_error(*source, "warning: exported symbol ignored", 
			labels.labels[i].name);
      }
    } else {
      if (module_type != CLOF_EXE) {
	component_error(*source, "undefined symbol", labels.labels[i].name);
	status = 0;
      }
    }
  }

  return status;
}

void store(Dword item)
{
  struct Segment *s = &segments.segments[current_segment];

  if (s->image_size >= s->image_extent) {
    s->image_extent += IMAGE_CHUNK;
    s->image = safe_realloc(s->image, s->image_extent * sizeof(Dword));
  }
  s->image[s->image_size] = item;
  s->image_size++;
  //  printf("STORE: %X\n", item);
}

void emit(Dword instr)
{
  static int prev_line_no = -100;
  struct Segment *s = &segments.segments[current_segment];
  struct DebugFile *dbg_info = s->files;

  // New line in the source file
  if (line_no != prev_line_no) {
    prev_line_no = line_no;
    if (dbg_info->nlines_inuse >= dbg_info->nlines) {
      dbg_info->nlines += IMAGE_CHUNK;
      dbg_info->flines = safe_realloc(dbg_info->flines,
				      dbg_info->nlines * sizeof(struct DebugInfo));
    }
    dbg_info->flines[dbg_info->nlines_inuse].offset = offset;
    dbg_info->flines[dbg_info->nlines_inuse].line = line_no;
    dbg_info->nlines_inuse++;
  }
  
  if (instr == FIX_SEGMENT || instr == FIX_ADISPLACEMENT ||
      instr == FIX_RDISPLACEMENT || instr == FIX_EXPRESSION) {
    store(instr); // Escape character
  } else {
    offset++;
  }
  store(instr);
  s->code_size++;
  //  printf("O=%d\n", offset);
}

static int generate(int outfile)
{
  int status = 1;

  /* The header */
  write_header(outfile, &segments, &labels);

  /* The code */
  for (int i = 0; i < segments.size; i++) {
    if (segments.segments[i].image_size && segments.segments[i].defined)
      status &= save_segment(outfile, i, &segments, &labels,
			     FIRST_FRAGMENT | LAST_FRAGMENT);
  }

  write_trailer(outfile);
  return status;
}

int parse_and_assembly(FILE *infile, int outfile)
{
  assert(infile);
  assert(outfile >= 0);

  // Parse
  yyin = infile;
  //  link_overhead = 0;
  offset = global_offset;
  if (yyparse() || !success)
    return EXIT_FAILURE;

  if (!find_undefined_labels() || !find_undefined_segments())
    return EXIT_FAILURE;

  // Assemble
  offset -= global_offset;
  if (!generate(outfile))
    return EXIT_FAILURE;

  if (listing) {
    list_segments(segments);
    list_labels(labels, segments);
  }

  return EXIT_SUCCESS;
}

