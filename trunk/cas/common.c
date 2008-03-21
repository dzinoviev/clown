#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include "registers.h"
#include "cas.h"

static int output_offset;

void store (Dword item)
{
  struct Segment *s = &segments.segments[current_segment];

  if (s->image_size >= s->image_extent) {
    s->image_extent += IMAGE_CHUNK;
    s->image = realloc (s->image, s->image_extent * sizeof (Dword));
    if (!s->image) {
      perror ("realloc");
      longjmp (failure, 1);
    }
  }
  s->image[s->image_size++] = item;
}

static void list_word (Dword instr)
{
  char character[] = "' '";
  if (!listing)
    return;
    
  if (instr == (char)instr && isprint ((char)instr))
    character[1] = (char)instr;
  else
    character[0] = character[2] = ' ';
  fprintf (stderr, "0x%08x: 0x%08lX %s\n", output_offset, instr, character);
  output_offset++;
}

void secure_write (int file, void *addr, int size)
{
  if (size != write (file, addr, size)) {
    perror ("Output file");
    longjmp (failure, 1);
  }    
}

void secure_string (int file, char *string)
{
  size_t len = strlen (string);
  secure_write (file, string, len);
}


void write_header (int outfile, Bit has_unreferenced,
		   struct SegmentTable *segments,
		   int interface,
		   struct LabelTable *labels)
{
  int i;
  static char params[64];

  secure_string (outfile, "<clof_exe");
  if (has_unreferenced)
    secure_string (outfile, " incomplete");
  secure_string (outfile, ">\n");
  
  /* Segment table */
  secure_string (outfile, "  <segments>\n");
  for (i = 0; i < segments->size; i++) {
    if (segments->segments[i].file_size) {
      struct Segment s  = segments->segments[i];
      secure_string (outfile, "    <segment");
      sprintf (params, " name=%s", s.name);
      secure_string (outfile, params);
      if (s.defined) {
	sprintf (params, " defined type=%d size=%d", s.type, s.file_size);
	secure_string (outfile, params);
      }
      secure_string (outfile, " />\n");
    }
  }
  secure_string (outfile, "  </segments>\n  <symbols>\n");

  /* Import/reference table */
  for (i = 0; i < labels->size; i++) {
    struct Label l = labels->labels[i];

    sprintf (params, "    <symbol name=%s", l.name);
    secure_string (outfile, params);

    if (l.export) {
      assert (l.defined);
    }

    if (l.defined) {
      sprintf (params, " segment=%d offset=%ld", l.segment, l.address);
      secure_string (outfile, params);
    }

    sprintf (params, " id=%d />\n", i);
    secure_string (outfile, params);
  }
  secure_string (outfile, "  </symbols>\n");

}

static void write_expression (int outfile, Expression *e)
{
  if (!e) {
    e = NULL;
    secure_write (outfile, &e->type, sizeof (Dword));
    list_word (e->type);
    return;
  }
  secure_write (outfile, &e->type, sizeof (Dword));
  list_word (e->type);
  switch (e->type) {
  case CONSTANT:
    secure_write (outfile, &e->detail.constant, sizeof (Dword));
    list_word (e->detail.constant);
    break;
  case LABEL:
    secure_write (outfile, &e->detail.label, sizeof (Dword));
    list_word (e->detail.label);
    break;
  case EXPRESSION:
    secure_write (outfile, &e->detail.expression.operation, sizeof (Dword));
    list_word (e->detail.expression.operation);
    write_expression (outfile, e->detail.expression.left);
    write_expression (outfile, e->detail.expression.right);
    break;
  }
}

static int save_escape (int pointer, Dword state, Dword instr, int outfile,
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
    if (seg->defined && seg->new_index != DEFAULT_SEGMENT) {
      Dword newseg = I_SEG (instr);
      SET_NEWID (newseg, seg->new_index);
      UPDATE_SEGMENT (instr, newseg);
    } else {
      /* do nothing -- let the linker take care of this */
      secure_write (outfile, &state, sizeof (Dword));
      list_word (state);
    }
    secure_write (outfile, &instr, sizeof (Dword));
    list_word (instr);
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

    if (0 >= try_to_evaluate (e, labels, &target_offset, &segment)) {
      component_error ("relative jump", "undefined displacement");
      return -1;
    }
    my_offset = target_offset - my_offset;
    if (abs (my_offset) >= MAX_OFFSET) {
      component_error ("too long jump", "suppressed");
      return -1;
    }
    instr = BUILD_INSTRUCTION_B (I_OPC (instr), I_OP1 (instr), my_offset);
    secure_write (outfile, &instr, sizeof (Dword));
    list_word (instr);
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
      secure_write (outfile, &state, sizeof (Dword));
      list_word (state);
      secure_write (outfile, &instr, sizeof (Dword));
      list_word (instr);
      write_expression (outfile, e);
    } else {
      if (0 >= try_to_evaluate (e, labels, &target_offset, &segment)) {
	component_error ("immediate parameter", "cannot be computed");
	return -1;
      }
      instr = BUILD_INSTRUCTION_B (I_OPC (instr), I_OP1 (instr), target_offset);
      secure_write (outfile, &instr, sizeof (Dword));
      list_word (instr);
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
      secure_write (outfile, &state, sizeof (Dword));
      list_word (state);
      secure_write (outfile, &instr, sizeof (Dword));
      list_word (instr);
      write_expression (outfile, e);
    } else {
      if (0 >= try_to_evaluate (e, labels, &target_offset, &segment)) {
	component_error ("undefined symbols", "suppressed");
	return -1;
      }
      instr = target_offset;
      secure_write (outfile, &instr, sizeof (Dword));
      list_word (instr);
    }
    break;

  default:
    assert (0);
  }

  return pointer;
}

int save_segment (int outfile, struct Segment *seg, struct LabelTable *labels)
{
  Dword instr, escape = ~FIX_SEGMENT;
  int pointer = 0; 
  int status = 1, newpointer;
  static char tmp[1000];

  if (module_type == CLOF_EXE) {
    sprintf (tmp, "  <bin segment=%s>",  seg->name);
    secure_string (outfile, tmp);
  }

  if (listing)
    fprintf (stderr, "$%s\n", seg->name);

  output_offset = 0;
  while (pointer < seg->image_size) {
    instr = seg->image[pointer++];
     
    switch (instr) {
    case FIX_SEGMENT:
    case FIX_ADISPLACEMENT:
    case FIX_RDISPLACEMENT:
    case FIX_EXPRESSION:
      if        (escape == instr) {/* real escape! */
	secure_write (outfile, &instr, sizeof (Dword));
	list_word (instr);
	escape = ~FIX_SEGMENT;
      } else if (escape == ~FIX_SEGMENT) /* escape prefix */
	escape = instr;
      else {		/* special symbol */
	newpointer = save_escape (pointer, escape, instr, outfile, seg, labels);
	escape = ~FIX_SEGMENT;
	if (newpointer == -1)
	  status = 0;
	else
	  pointer = newpointer;
      }
      break;
    default:
      if (escape == ~FIX_SEGMENT) {	/* normal copy */
	secure_write (outfile, &instr, sizeof (Dword));
	list_word (instr);
      } else {		/* special symbol */
	newpointer = save_escape (pointer, escape, instr, outfile, seg, labels);
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
    component_error ("warning: incomplete escape sequence at EOF",
		     "ESC");
  free (seg->image);
  if (module_type == CLOF_EXE) {
    secure_string (outfile, "</bin>\n");
  }
  return status;
}

