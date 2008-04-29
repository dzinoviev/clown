#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <ctype.h>
#ifdef __linux
#	include <linux/limits.h>
#else
#	include <limits.h>
#endif
#include "registers.h"
#include "clowndev.h"

static int output_offset;
static int bin_offset;
Dword offset;
int current_overhead;
int link_overhead;
int debug = 1;

void init_module (struct Module *m, char *fname)
{
    m->name = safe_malloc (strlen (fname) + 1);
    strcpy (m->name, fname);
    m->st.size = 0;
    m->st.segments = NULL;
    m->lt.size = 0;
    m->lt.labels = NULL;
}
  
static void list_segment (struct Segment s)
{
  int size = (module_type == CLOF_BIN) ? (s.image_size - s.link_overhead) : s.image_size;
  if (s.defined) {
    char type = 0;
    switch (s.type) {
    case SEG_DATA:
      type = 'D';
      break;
    case SEG_CODE:
      type = 'C';
      break;
    case SEG_CONST:
      type = 'R';
      break;
    case SEG_DEFAULT:
      type = 'M';
      break;
    }
    fprintf (stderr, "%3d. $%-15s %c DEF size=%4d\n", s.new_index, s.name, type, size);
  } else
    fprintf (stderr, "%3d. $%-17s UNDEF\n", s.new_index, s.name);
}

void list_segments (struct SegmentTable st)
{
  int i, found = 0;
  fputs ("\nSegments:\n--------------------------\n", stderr);

  for (i = 0; i < st.size; i++)
    if (st.segments[i].in_use || st.segments[i].defined) {
      list_segment (st.segments[i]);
      found = 1;
    }
  
  if (!found)
    fputs ("No named segments in this file\n", stderr);
}

static void list_label (struct Label l, struct Segment *seglist)
{
    fprintf (stderr, 
	     "%-16s %c ", 
	     l.name, 
	     l.export ? 'G' : ' ');
    if (l.defined) {
	fprintf (stderr, 
		 "DEF $%s:(0x%08lX)\n", 
		 (l.segment != DEFAULT_SEGMENT) ? seglist[l.segment].name : DEFAULT_SEGMENT_NAME,
		 l.address);
    } else {
	fputs ("UNDEF \n", stderr);
    }
}

static int compare_labels (const void *l1, const void *l2)
{
  struct Label *label1, *label2;
  label1 = (struct Label*)l1;
  label2 = (struct Label*)l2;
  return (label1->segment != label2->segment) ? 
    label1->segment > label2->segment :
    label1->address > label2->address;
}

void list_labels (struct LabelTable syms, struct SegmentTable segs)
{
    int i;
    fputs ("\nSymbols:\n--------------------------\n", stderr);
 
    if (syms.size) {
	qsort ((void *)syms.labels, syms.size, sizeof (struct Label), compare_labels);
	for (i = 0; i < syms.size; i++)
	    list_label (syms.labels[i], segs.segments);
    } else {
	fputs ("No symbols in this file\n", stderr);
    }
}

static char* ascii[] = {"NUL","SOH","STX","ETX","EOT","ENQ","ACK","\\a","\\b",
			"\\t","\\n","\\v","\\f","\\r","SO","SI",
			"DLE","DC1","DC2","DC3","DC4","NAK","SYN","ETB",
			"CAN","EM","SUB","ESC","FS","GS","RS","US", "SPACE"};

static void list_word (Dword instr, int withaddress)
{
  output_offset++;
  if (!listing)
    return;
    
  if (withaddress) {
      bin_offset++;
      fprintf (stderr, "0x%08X: ", bin_offset - 1);
  } else
      fprintf (stderr, "            ");
  fprintf (stderr, "0x%08lX ", instr);
  if (instr == (char)instr) {
      if (iscntrl ((char)instr) || isspace ((char)instr))
	  fprintf (stderr, ascii[instr]);
      else
	  if (isprint ((char)instr))
	      fprintf (stderr, "'%c'", (char)instr);
  }
  fprintf (stderr, "\n");
}

static void write_expression (int outfile, Expression *e, struct LabelTable *labels)
{
    if (!e) {
	safe_base64 (outfile, (Dword)e);
	list_word ((Dword)e, 0);
	return;
    }
    safe_base64 (outfile, e->type);
    list_word (e->type, 0);
    switch (e->type) {
    case CONSTANT:
    case SELECTOR:
	safe_base64 (outfile, e->detail.constant);
	list_word (e->detail.constant, 1);
	break;
    case LABEL:
	safe_base64 (outfile, labels->labels[e->detail.label].new_index);
	list_word (e->detail.label, 1);
	break;
    case EXPRESSION:
	safe_base64 (outfile, e->detail.expression.operation);
	list_word (e->detail.expression.operation, 0);
	write_expression (outfile, e->detail.expression.left, labels);
	write_expression (outfile, e->detail.expression.right, labels);
	break;
    case DUMMY:
	assert (e->type!=DUMMY);
	break;
    }
}

void write_trailer (int outfile)
{
    safe_string (outfile, module_type == CLOF_EXE ? "</clof>\n" : "</clef>\n");
}

void write_header (int outfile, struct SegmentTable *segments, struct LabelTable *labels)
{
  int i;
  static char params[PATH_MAX+128];
  char cwd[PATH_MAX];
  if (!getcwd (cwd, PATH_MAX)) {
    perror ("getcwd");
    longjmp (failure, 1);
  }

  safe_string (outfile, module_type == CLOF_EXE ? "<clof>\n" : "<clef>\n");

  /* Segment table */
  safe_string (outfile, "  <segments>\n");
  for (i = 0; i < segments->size; i++) {
      if (segments->segments[i].image_size || segments->segments[i].id != DEFAULT_SEGMENT) {
      int j;
      struct Segment s  = segments->segments[i];
      safe_string (outfile, "    <segment");
      sprintf (params, " name=\"%s\" id=\"%d\" defined=\"%d\"", s.name, i, s.defined);
      safe_string (outfile, params);
      if (s.defined) {
	  sprintf (params, " type=\"%d\"", s.type);
	safe_string (outfile, params);
      }
      safe_string (outfile, ">\n");

      /* DEBUG INFO */
      if (debug && segments->segments[i].defined)
	  for (j = 0; j < s.nfiles; j++) {
	      int k;
	      if (s.files[j].file[0] == '/')
		  sprintf (params, "      <file name=\"%s\">\n", s.files[j].file);
	      else
		  sprintf (params, "      <file name=\"%s/%s\">\n", cwd, s.files[j].file);
	      safe_string (outfile, params);
	      for (k = 0; k < s.files[j].nlines_inuse; k++) {
		  sprintf (params, "        <line offset=\"%lu\" lineno=\"%d\" />\n", 
			   s.files[j].flines[k].offset,
			   s.files[j].flines[k].line);
		  safe_string (outfile, params);
	      }
	      safe_string (outfile, "      </file>\n");		
	  }

      safe_string (outfile, "    </segment>\n");
    }
  }
  safe_string (outfile, "  </segments>\n");

  if (module_type == CLOF_EXE) {
      safe_string (outfile, "  <symbols>\n");
      /* Symbol table */
      for (i = 0; i < labels->size; i++) {
	  struct Label l = labels->labels[i];
	  
	  sprintf (params, "    <symbol name=\"%s\" id=\"%d\"", l.name, i);
	  safe_string (outfile, params);
	  
	  if (l.export) {
	      assert (l.defined);
	      safe_string (outfile, " global=\"1\"");
	  } else {
	      safe_string (outfile, " global=\"0\"");
	  }
	  
	  if (l.defined) {
	      sprintf (params, " defined=\"1\" segment=\"%d\" offset=\"%ld\"", l.segment, l.address);
	      safe_string (outfile, params);
	  } else {
	      safe_string (outfile, " defined=\"0\"");
	  }
	  
	  safe_string (outfile, " />\n");
      }
      safe_string (outfile, "  </symbols>\n");
  }
}

static int save_escape (int pointer, Dword state, Dword instr, int outfile,
			struct SegmentTable *st, 
			struct Segment *seg,
			struct LabelTable *labels)
{
    Expression *e;
    Dword my_offset, target_offset, segment = NOT_FOUND;

    switch (state) {
    case FIX_SEGMENT:
	/*
	 * emit_escape (instr); -- this is "instr"
	 */
	segment = SEL_ID (I_SEG (instr));
	if (seg->defined && seg->new_index != DEFAULT_SEGMENT && module_type == CLOF_BIN) {
	    Dword newseg = I_SEG (instr);
	    SET_NEWID (newseg, st->segments[segment].new_index);
	    UPDATE_SEGMENT (instr, newseg);
	} else {
	    /* do nothing -- let the linker take care of this */
	    safe_base64 (outfile, state);
	    list_word (state, 0);
	}
	safe_base64 (outfile, instr);
	list_word (instr, 1);
	break;

    case FIX_RDISPLACEMENT:
	/*
	 * emit_escape (BUILD_INSTRUCTION_B (opc, 0, 0)); -- this is "instr"
	 * emit_escape (target); 
	 * emit_escape (offset);
	 */
	e = (Expression*)(seg->image[pointer++]);
	assert (e);

	my_offset = seg->image[pointer++];

	if (0 >= try_to_evaluate (e, labels, st, &target_offset, &segment)) {
	    component_error (seg->name, "relative jump", "undefined displacement");
	    return -1;
	}
	my_offset = target_offset - my_offset;
	if (abs (my_offset) >= MAX_OFFSET) {
	    component_error (seg->name, "too long jump", "suppressed");
	    return -1;
	}
	instr = BUILD_INSTRUCTION_B (I_OPC (instr), I_OP1 (instr), my_offset);
	safe_base64 (outfile, instr);
	list_word (instr, 1);
	break;

    case FIX_ADISPLACEMENT:
	/*
	 * emit_escape (BUILD_INSTRUCTION_B (opc, 0, 0)); -- this is "instr"
	 * emit_escape (expression); 
	 */
	e = (Expression*)(seg->image[pointer++]);
	assert (e);

	if (module_type == CLOF_EXE) {
	    /* do nothing -- let the linker take care of this */
	    safe_base64 (outfile, state);
	    list_word (state, 0);
	    safe_base64 (outfile, instr);
	    list_word (instr, 0);
	    write_expression (outfile, e, labels);
	} else {
	    if (0 >= try_to_evaluate (e, labels, st, &target_offset, &segment)) {
		component_error (seg->name, "immediate parameter", "cannot be computed");
		return -1;
	    }
	    instr = BUILD_INSTRUCTION_B (I_OPC (instr), I_OP1 (instr), target_offset);
	    safe_base64 (outfile, instr);
	    list_word (instr, 1);
	}
	break;

    case FIX_EXPRESSION:
	/*
	 * emit_escape (0); -- this is "instr"
	 * emit_escape (expression); 
	 */
	e = (Expression*)(seg->image[pointer++]);
	assert (instr == 0 && e);

	if (module_type == CLOF_EXE) {
	    /* do nothing -- let the linker take care of this */
	    safe_base64 (outfile, state);
	    list_word (state, 0);
	    safe_base64 (outfile, instr);
	    list_word (instr, 0);
	    write_expression (outfile, e, labels);
	} else {
	    if (0 >= try_to_evaluate (e, labels, st, &target_offset, &segment)) {
		component_error (seg->name, "undefined symbols", "suppressed");
		return -1;
	    }
	    instr = target_offset;
	    safe_base64 (outfile, instr);
	    list_word (instr, 1);
	}
	break;

    default:
	assert (0);
    }

    return pointer;
}

int save_segment (int outfile, int id, struct SegmentTable *st, struct LabelTable *labels, int fragment)
{
  Dword instr, escape = ~FIX_SEGMENT;
  int pointer = 0; 
  int status = 1, newpointer;
  struct Segment *seg = &st->segments[id];
  static char tmp[1000];

  if (fragment & FIRST_FRAGMENT) {
    sprintf (tmp, "  <bin segment=\"%d\"><![CDATA[",  st->segments[id].new_index);
    safe_string (outfile, tmp);
  }

  if (listing)
    fprintf (stderr, "$%s\n", seg->name);

  if (fragment & FIRST_FRAGMENT) { 
      output_offset = 0;
      bin_offset = 0;
  }

  while (pointer < seg->image_size) {
    instr = seg->image[pointer++];
     
    switch (instr) {
    case FIX_SEGMENT:
    case FIX_ADISPLACEMENT:
    case FIX_RDISPLACEMENT:
    case FIX_EXPRESSION:
      if        (escape == instr) {/* real escape! */
	safe_base64 (outfile, instr);
	list_word (instr, 1);
	escape = ~FIX_SEGMENT;
      } else if (escape == ~FIX_SEGMENT) /* escape prefix */
	escape = instr;
      else {		/* special symbol */
	  newpointer = save_escape (pointer, escape, instr, outfile, st, seg, labels);
	escape = ~FIX_SEGMENT;
	if (newpointer == -1)
	  status = 0;
	else
	  pointer = newpointer;
      }
      break;
    default:
      if (escape == ~FIX_SEGMENT) {	/* normal copy */
	safe_base64 (outfile, instr);
	list_word (instr, 1);
      } else {		/* special symbol */
	newpointer = save_escape (pointer, escape, instr, outfile, st, seg, labels);
	escape = ~FIX_SEGMENT;
	if (newpointer == -1)
	  status = 0;
	else
	  pointer = newpointer;
      }
      break;
    }
  }

  if (escape != ~FIX_SEGMENT)
    component_error (seg->name, "warning: incomplete escape sequence at EOF",
		     "ESC");
  /*  free (seg->image);*/
  if (fragment & LAST_FRAGMENT) {
    safe_string (outfile, "]]></bin>\n");
  }
  /*
  assert ((module_type == CLOF_BIN) || (offset==0) || (output_offset == offset + current_overhead));
  */
  return status;
}

