#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <setjmp.h>
#include <ctype.h>
#include <sys/types.h>
#include "cas.h"

#ifdef __STRICT_ANSI__
int fileno (FILE *stream);	/* for Ansi C  */
#endif

       Clof_Type module_type      = CLOF_UNKNOWN;
static Bit       has_unreferenced = 0;
static int       interface        = 0;
static int       ofile;
jmp_buf   failure;

       int listing = 0;

struct SegmentTable    segments     = {0, NULL};
struct LabelTable      labels       = {0, NULL};

void component_error (const char *msg, char *detail)
{
    fprintf (stderr, "cas:%s: %s\n", msg, detail);
}

void end_segment (int segment, Dword size)
{
    segments.segments[segment].file_size = size;
}

static int search_segment (char *name)
{
    int i;
    for (i = 0; i < segments.size; i++)
	if (! (strcmp (segments.segments[i].name, name)))
	    return i;
    return -1;
}

static int create_segment (char *name)
{
    int seg = segments.size;

    if (seg == 1 && module_type == CLOF_BIN) {
	component_error ("too many segments in a BIN file: ", name);
	return -1;
    }

    segments.segments = realloc (segments.segments, 
				 sizeof (struct Segment) 
				 * (segments.size + 1));
    
    if (!segments.segments)
	return -1;
    
    segments.segments[seg].name = malloc (strlen (name) + 1);
    if (!segments.segments[seg].name)
	return -1;
    strcpy (segments.segments[seg].name, name);
    segments.segments[seg].defined = 0;
    segments.segments[seg].new_index = -1;

    segments.size++;

    return seg;
}

int lookup_segment (char *name)
{
    int seg = search_segment (name);
    if (seg == -1)
	return create_segment (name);
    else
	return seg;
}

int begin_segment (int type, char *name)
{
    int seg = search_segment (name);

    if (-1 != seg) {
	if (segments.segments[seg].defined) {
	    component_error ("duplicate segment definition: ", name);
	    return -1;
	}
    } else {
	seg = create_segment (name);
	if (-1 == seg)
	    return -1;
    }

    /* define segment */
    segments.segments[seg].type = type;
    segments.segments[seg].defined = 1;
    segments.segments[seg].file_offset = offset;

    if (listing)
	fprintf (stderr, "\n$%s\n", name);

    return seg;
}

static int lookup_label (char *label)
{
    int i;

    for (i = 0; i < labels.size; i++)
	if (! (strcmp (labels.labels[i].name, label)))
	    return i;
    return -1;
}

static int create_label (char *label)
{
    int i = labels.size;
    labels.labels = realloc (labels.labels, 
			     sizeof (struct Label) * (labels.size + 1));
    if (!labels.labels)
	return -1;

    labels.labels[i].name = malloc (strlen (label) + 1);
    if (!labels.labels[i].name)
	return -1;
    strcpy (labels.labels[i].name, label);
    labels.labels[i].near = 0;	/* must be near? */
    labels.labels[i].export = 0; /* can be exported? */
    labels.labels[i].interrupt = 0; /* must be aligned? */
    labels.labels[i].defined = 0;
    labels.labels[i].intersegment = 0;
    labels.labels[i].segment = -1;

    labels.size++;
    return i;
}

void mark_near_label (int i)
{
    labels.labels[i].near = 1;	/* must be near! */
}

int mark_export_label (char *label)
{
    int lab;
    
    if (-1 != (lab = lookup_label (label)))
	component_error ("warning: duplicate export definition: ",
		   label);
    else 
	if (-1 != (lab = create_label (label)))
	    labels.labels[lab].export = 1;

    return lab;
}

static void define_label (int i, int segment, Dword offset)
{
    labels.labels[i].address = offset;
    labels.labels[i].segment = segment;
    labels.labels[i].defined = 1;
}

int add_label (char *label, int segment, Dword offset, Bit global, Bit align8)
{
    int i;

    if (-1 == (i = lookup_label (label)) &&
	-1 == (i = create_label (label)))
      return -1;

    if (labels.labels[i].defined) {
      component_error ("duplicate symbol: ", label);
      return -1;
    }
	
    if (labels.labels[i].segment != -1 /* has been defined */
	&& labels.labels[i].segment != segment)
      labels.labels[i].intersegment = 1;
    
    if (align8) {
      if (offset % 8) {
	int i;
	Dword offset1 = (offset / 8 + 1) * 8;
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

    if (-1 == (i = lookup_label (label)))
	i = create_label (label);
    else
	if (   labels.labels[i].segment != -1 /* has been defined */
	    && labels.labels[i].segment != segment)
	    labels.labels[i].intersegment = 1;

    if (-1 != i)
	labels.labels[i].segment = segment;
   
    return i;    
}

static int find_undefined_labels ()
{
    int status = 1;
    int i;

    if (listing)
	fputs ("\nSymbols:\n", stderr);

    for (i = 0; i < labels.size; i++) {
	/* count interface symbols: global and undefined */
	interface += labels.labels[i].export + !labels.labels[i].defined;

	if (listing)
	    list_label (labels.labels[i], segments.segments);

	if (labels.labels[i].defined) {
	    if (labels.labels[i].export && module_type != CLOF_EXE) {
	      component_error ("warning: exported symbol ignored in a BIN file", 
			 labels.labels[i].name);
	    }
	} else {
	    if (labels.labels[i].export)
		component_error ("warning: exported symbol declared but "
			   "not defined", 
			   labels.labels[i].name);
	    else {
		if (module_type != CLOF_EXE) {
		    component_error ("undefined symbol in a BIN file", 
			       labels.labels[i].name);
		    status = 0;
		} else
		    has_unreferenced = 1;
	    }
	    
	}
	if (labels.labels[i].near) {
	    if  (!labels.labels[i].defined) {
		component_error ("undefined symbol: ", labels.labels[i].name);
		status = 0;
	    } 
	    if (labels.labels[i].intersegment) {
		component_error ("invalid cross-segment reference: ", 
			   labels.labels[i].name);
		status = 0;
	    }
	}
    }

    return status;
}

void emit (Dword instr)
{
    if (   instr == FIX_SEGMENT 
	|| instr == FIX_DISPLACEMENT
	|| instr == FIX_ADDRESS_LOC
	|| instr == FIX_ADDRESS_GLOB ) {
	if (listing)
	    fprintf (stderr, "            0x%08lX (ESC)\n", instr);
	secure_write (ofile, &instr, sizeof (Dword));
    } else
	offset++;
    secure_write (ofile, &instr, sizeof (Dword));

    if (listing) {
      char character[] = "' '";
      if (isprint ((char)instr))
	character[1] = (char)instr;
      else
	character[0] = character[2] = ' ';
      fprintf (stderr, "0x%08lX: 0x%08lX %s\n", offset - 1, instr, 
	       character);
    }
}

void emit_escape (Dword escape)
{
    if (listing)
	fprintf (stderr, "            0x%08lX *\n", escape);
    secure_write (ofile, &escape, sizeof (Dword));
}

static int generate (int scratch, int outfile)
{
    if ((off_t)-1 == lseek (scratch, 0, SEEK_SET)) {
	perror ("temporary file");
	return 0;
    }

    ofile = outfile;

    /* Update the signature */
    if (module_type == CLOF_EXE)
	write_header (outfile, has_unreferenced, 
		      &segments, interface, &labels);
    
    /* The code */
    copy_code (scratch, outfile, &segments, &labels);

    return 1;
}

int find_undefined_segments ()
{
    int i, status = 1;

    if (listing)
	fprintf (stderr, "\nSegments:\n");

    for (i = 0; i < segments.size; i++) {
	if (module_type != CLOF_EXE && !segments.segments[i].defined) {
	    component_error ("undefined segment in a BIN file: ", 
		       labels.labels[i].name);
	    status = 0;
	}
	if (listing)
	    list_segment (segments.segments[i]);
    }

    return status;
}

int parse_and_assembly (FILE *infile, int outfile)
{
    FILE *tf;

    assert (infile);
    assert (outfile);

    tf = tmpfile ();
    if (!tf) {
	perror ("temporary file");
	return EXIT_FAILURE;
    }
    ofile = fileno (tf);
    yyin = infile;

    if (!setjmp (failure) && yyparse ())
	return EXIT_FAILURE;

    if (!find_undefined_segments ())
	return EXIT_FAILURE;

    if (!find_undefined_labels ())
	return EXIT_FAILURE;

    if (!generate (ofile, outfile))
	return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
