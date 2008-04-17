#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <setjmp.h>
#include <sys/types.h>
#include "cas.h"
#include "generate.h"

#ifdef __STRICT_ANSI__
int fileno (FILE *stream);	/* for Ansi C  */
#endif

       Clof_Type module_type      = CLOF_UNKNOWN;
jmp_buf   failure;

       int listing = 0;

int global_offset = 0;
struct SegmentTable    segments     = {0, NULL};
struct LabelTable      labels       = {0, NULL};

void component_error (const char *name, const char *msg, char *detail)
{
    fprintf (stderr, "%s: %s: %s\n", name, msg, detail);
}

void end_segment (int segment, Dword size)
{
    size -= global_offset;
    segments.segments[segment].file_size = (module_type == CLOF_BIN) ? size : (size + current_overhead);
}

static int search_segment (char *name)
{
    int i;
    for (i = 0; i < segments.size; i++)
	if (! (strcmp (segments.segments[i].name, name)))
	    return i;
    return NOT_FOUND;
}

static int create_segment (char *name)
{
    int seg = segments.size;
    /*
    if (seg == 1 && module_type == CLOF_BIN) {
	component_error (*source, "too many segments in a BIN file", name);
	return NOT_FOUND;
    }
    */
    segments.segments = safe_realloc (segments.segments, 
				      sizeof (struct Segment) 
				      * (segments.size + 1));
    
    segments.segments[seg].name = safe_malloc (strlen (name) + 1);

    strcpy (segments.segments[seg].name, name);
    segments.segments[seg].defined = 0;
    segments.segments[seg].new_index = NOT_FOUND;

    /* Support for debugging */
    segments.segments[seg].nfiles = 1;
    segments.segments[seg].files = safe_malloc (sizeof (struct DebugFile));
    segments.segments[seg].files[0].file = source[0];
    segments.segments[seg].files[0].nlines = 0;
    segments.segments[seg].files[0].nlines_inuse = 0;
    segments.segments[seg].files[0].flines = NULL;

    segments.segments[seg].id = seg;
    segments.segments[seg].new_index = seg;

    segments.size++;
 
    if (seg)
	segments.segments[DEFAULT_SEGMENT].defined = 0;

    return seg;
}

int lookup_segment (char *name)
{
    int seg = search_segment (name);
    if (seg == NOT_FOUND)
	return create_segment (name);
    else
	return seg;
}

int begin_segment (int type, char *name)
{
    int seg = search_segment (name);

    if (NOT_FOUND != seg) {
	if (segments.segments[seg].defined) {
	    component_error (*source, "duplicate segment definition", name);
	    return NOT_FOUND;
	}
    } else {
	seg = create_segment (name);
	if (NOT_FOUND == seg)
	    return NOT_FOUND;
    }

    /* define segment */
    segments.segments[seg].type = type;
    segments.segments[seg].defined = 1;

    segments.segments[seg].image = safe_malloc (sizeof (Dword) * IMAGE_CHUNK);

    segments.segments[seg].image_size = 0;
    segments.segments[seg].image_extent = IMAGE_CHUNK;

    current_overhead = 0;

    return seg;
}

static int lookup_label (char *label)
{
    int i;

    for (i = 0; i < labels.size; i++)
	if (! (strcmp (labels.labels[i].name, label)))
	    return i;
    return NOT_FOUND;
}

static int create_label (char *label)
{
    int i = labels.size;
    labels.labels = safe_realloc (labels.labels, 
				  sizeof (struct Label) * (labels.size + 1));

    labels.labels[i].name = safe_malloc (strlen (label) + 1);

    strcpy (labels.labels[i].name, label);
    labels.labels[i].export = 0; /* can be exported? */
    labels.labels[i].defined = 0;
    labels.labels[i].segment = DEFAULT_SEGMENT;
    labels.labels[i].new_index = i;

    labels.size++;
    return i;
}

int mark_export_label (char *label)
{
    int lab;
    
    if (NOT_FOUND != (lab = lookup_label (label)))
	component_error (*source, "warning: duplicate export definition", label);
    else 
	if (NOT_FOUND != (lab = create_label (label)))
	    labels.labels[lab].export = 1;

    return lab;
}

static void define_label (int i, int segment, Dword offset)
{
    labels.labels[i].address = offset;
    labels.labels[i].segment = segment;
    labels.labels[i].defined = 1;
}

int add_label (char *label, int segment, Dword offset, int global, int align8)
{
    int i;

    if (NOT_FOUND == (i = lookup_label (label)) &&
	NOT_FOUND == (i = create_label (label)))
      return NOT_FOUND;

    if (labels.labels[i].defined) {
	component_error (*source, "duplicate symbol", label);
      return NOT_FOUND;
    }
	
    if (align8) {
	int boundary;
	if (align8 == 1) {
	    boundary = 8;
	} else {
	    boundary = CLOWN_FRAME_SIZE;	/* PAGE ALIGNMENT */
	}
      if (offset % boundary) {
	int i;
	Dword offset1 = (offset / boundary + 1) * boundary;
	for (i = 0; i < offset1 - offset; i++)
	  emit (BUILD_INSTRUCTION_A (NOP, 0, 0));
	offset = offset1;
      }
    }
    
    if (global)
      labels.labels[i].export = 1;

    define_label (i, segment, offset);
    
    return i;
}

int use_label (char *label, int segment)
{
  int i;

  if (NOT_FOUND == (i = lookup_label (label))) {
    i = create_label (label);
    if (NOT_FOUND != i)
      labels.labels[i].segment = segment;
  } 
   
  return i;    
}

static int find_undefined_segments ()
{
    int status = 1;
    int i;

    for (i = 0; i < segments.size; i++) {
	if (!segments.segments[i].defined && i != DEFAULT_SEGMENT && module_type != CLOF_EXE) {
	    component_error (*source, "undefined segment", segments.segments[i].name);
	    status = 0;
	}
    }
    
    return status;
}

static int find_undefined_labels ()
{
    int status = 1;
    int i;

    for (i = 0; i < labels.size; i++) {
	if (labels.labels[i].defined) {
	    if (labels.labels[i].export && module_type != CLOF_EXE) {
		component_error (*source, "warning: exported symbol ignored", 
			 labels.labels[i].name);
	    }
	} else {
	  if (module_type != CLOF_EXE) {
	      component_error (*source, "undefined symbol", labels.labels[i].name);
	    status = 0;
	  }
	}
    }

    return status;
}

static void store (Dword item)
{
  struct Segment *s = &segments.segments[current_segment];

  if (s->image_size >= s->image_extent) {
    s->image_extent += IMAGE_CHUNK;
    s->image = safe_realloc (s->image, s->image_extent * sizeof (Dword));
  }
  s->image[s->image_size++] = item;
}

void emit (Dword instr)
{
  static int prev_line_no = -100;
  struct DebugFile *dbg_info = segments.segments[current_segment].files;
  if (line_no != prev_line_no) {
    prev_line_no = line_no;
    if (dbg_info->nlines_inuse >= dbg_info->nlines) {
      dbg_info->nlines += IMAGE_CHUNK;
      dbg_info->flines = safe_realloc (dbg_info->flines, dbg_info->nlines * sizeof (struct DebugInfo));
    }
    dbg_info->flines[dbg_info->nlines_inuse].offset = offset;
    dbg_info->flines[dbg_info->nlines_inuse].line = line_no - 1;
    dbg_info->nlines_inuse++;
  }

  if (   instr == FIX_SEGMENT 
      || instr == FIX_ADISPLACEMENT
      || instr == FIX_RDISPLACEMENT
      || instr == FIX_EXPRESSION ) {
    store (instr);
  } else
    offset++;
  store (instr);
}

void emit_escape (Dword escape)
{
    store (escape);
}

static int generate (int outfile)
{
  int status = 1, i;

  /* The header */
  write_header (outfile, &segments, &labels);

  /* The code */
  for (i = 0; i < segments.size; i++)
    if (segments.segments[i].file_size && segments.segments[i].defined)
	status &= save_segment (outfile, i, &segments, &labels, FIRST_FRAGMENT | LAST_FRAGMENT);

  write_trailer (outfile);
  return status;
}

extern int success;
int parse_and_assembly (FILE *infile, int outfile)
{
    assert (infile);
    assert (outfile);

    yyin = infile;

    link_overhead = 0;
    offset = global_offset;
    if (yyparse () || !success)
      return EXIT_FAILURE;

    if (!find_undefined_labels () || !find_undefined_segments ())
	return EXIT_FAILURE;

    offset -= global_offset;
    if (!generate (outfile))
	return EXIT_FAILURE;

    if (listing) {
      list_segments (segments);
      list_labels (labels, segments);
    }

    return EXIT_SUCCESS;
}

