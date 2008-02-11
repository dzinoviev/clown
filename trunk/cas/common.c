#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include "registers.h"
#include "cas.h"

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


void list_segment (struct Segment s)
{
    if (s.defined)
	fprintf (stderr, "$%-15s %c DEF offset=%4d size=%4d\n", 
		 s.name,
		 s.type == SEG_DATA ? 'D' : 'C',
		 s.file_offset,
		 s.file_size);
    else
	fprintf (stderr, "$%-15s UNDEF\n", s.name);
}

void list_label (struct Label l, struct Segment *seglist)
{
    fprintf (stderr, 
	     "%-16s %c %s ", 
	     l.name, 
	     l.export ? 'G' : ' ', 
	     l.near ? "NEAR" : "FAR ");
    if (l.defined) {
	fprintf (stderr, 
		 "DEF    %s $%s:(0x%08lx)\n", 
		 l.intersegment ? "XSEG" : "    ",
		 (l.segment != -1) ? seglist[l.segment].name : "code*",
		 l.address);
    } else {
	fputs ("UNDEF \n", stderr);
    }
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
    struct Segment s  = segments->segments[i];
    secure_string (outfile, "    <segment");
    sprintf (params, " name=\"%s\" id=%d", s.name, i);
    secure_string (outfile, params);
    if (s.defined) {
      sprintf (params, " defined type=%d offset=%d size=%d", s.type, s.file_offset, s.file_size);
      secure_string (outfile, params);
    }
    secure_string (outfile, " />\n");
  }
  secure_string (outfile, "  </segments>\n  <symbols>\n");

  /* Import/reference table */
  for (i = 0; i < labels->size; i++) {
    struct Label l = labels->labels[i];
    if (l.defined && !l.export)
      continue;
    assert (!l.near);

    sprintf (params, "    <symbol name=\"%s\"", l.name);
    secure_string (outfile, params);

    if (l.export) {
      assert (l.defined);
      sprintf (params, " segment=%d offset=%ld", l.segment, l.address);
      secure_string (outfile, params);
    }
    secure_string (outfile, " />\n");
  }
  secure_string (outfile, "  </symbols>\n");

}

static int copy_special (Dword state, Dword instr, int scratch, int outfile,
			 struct SegmentTable *segments,
			 struct LabelTable *labels)
{
    struct yyLabel l;
    Dword my_offset, segment;

    switch (state) {
    case FIX_SEGMENT:
      /*
       * emit (instr); -- this is "instr"
       */
      segment = SEL_ID (I_SEG (instr));
      if (segments->segments[segment].defined 
	  && segments->segments[segment].new_index != -1) {
	  Dword newseg = I_SEG (instr);
	  SET_NEWID (newseg, segments->segments[segment].new_index);
	  UPDATE_SEGMENT (instr, newseg);
      } else {
	  /* do nothing */
	  secure_write (outfile, &state, sizeof (Dword));
      }
      secure_write (outfile, &instr, sizeof (Dword));
      break;
    case FIX_DISPLACEMENT:
      /*
       * emit (BUILD_INSTRUCTION_B (opc, 0, 0)); -- this is "instr"
       * emit_escape (l.index); 
       * emit_escape (l.offset); 
       * emit_escape (offset);
       */
	if (   sizeof (Dword) != read (scratch, &l.index, sizeof (Dword))
	    || sizeof (Dword) != read (scratch, &l.offset, sizeof (Dword)) 
	    || sizeof (Dword) != read (scratch, &my_offset, sizeof (Dword))) {
		perror ("temporary file");
		return 0;
	    }
	
	assert (l.index < labels->size);
	assert (labels->labels[l.index].defined);

	my_offset = (labels->labels[l.index].address + l.offset) - my_offset;
	if (abs (my_offset) >= MAX_OFFSET) {
	    component_error ("too long jump: ", 
		       labels->labels[l.index].name);
	    return 0;
	}
	instr = BUILD_INSTRUCTION_B (I_OPC (instr), 0, my_offset);
	secure_write (outfile, &instr, sizeof (Dword));
	break;

    case FIX_ADDRESS_LOC:
    case FIX_ADDRESS_GLOB:
	assert (!instr);
	/*
	 * emit (0); -- this is "instr"
	 * emit_escape (l.index); 
	 * emit_escape (l.offset);
	 */

	if (   sizeof (Dword) != read (scratch, &l.index, sizeof (Dword))
	    || sizeof (Dword) != read (scratch, &l.offset, sizeof (Dword))) {
		perror ("temporary file");
		return 0;
	    }
	
	assert (l.index < labels->size);

	if (!labels->labels[l.index].defined) {
	    if (state == FIX_ADDRESS_LOC) {
		component_error ("undefined symbol: ",
			   labels->labels[l.index].name);
		return 0;
	    }
	    /* undefined label; do nothing */
	    secure_write (outfile, &state, sizeof (Dword));
	    secure_write (outfile, &instr, sizeof (Dword));
	    secure_write (outfile, &l.index, sizeof (Dword));
	    secure_write (outfile, &l.offset, sizeof (Dword));
	} else {
	    my_offset = labels->labels[l.index].address + l.offset;
	    secure_write (outfile, &my_offset, sizeof (Dword));
	}
	break;

    default:
	assert (0);
    }

    return 1;
}

int copy_code (int scratch, int outfile, struct SegmentTable *segments,
	       struct LabelTable *labels)
{
  Dword instr, escape = ~FIX_SEGMENT;

  while (sizeof (Dword) == read (scratch, &instr, sizeof (Dword))) {
    switch (instr) {
    case FIX_SEGMENT:
    case FIX_DISPLACEMENT:
    case FIX_ADDRESS_LOC:
    case FIX_ADDRESS_GLOB:
      if        (escape == instr) {/* real escape! */
	secure_write (outfile, &instr, sizeof (Dword));
	escape = ~FIX_SEGMENT;
      } else if (escape == ~FIX_SEGMENT) /* escape prefix */
	escape = instr;
      else {		/* special symbol */
	copy_special (escape, instr, scratch, outfile, 
		      segments, labels);
	escape = ~FIX_SEGMENT;
      }
      break;
    default:
      if (escape == ~FIX_SEGMENT)	/* normal copy */
	secure_write (outfile, &instr, sizeof (Dword));
      else {		/* special symbol */
	copy_special (escape, instr, scratch, outfile, 
		      segments, labels);
	escape = ~FIX_SEGMENT;
      }
      break;
    }
  }

  if (escape != ~FIX_SEGMENT)
    component_error ("warning: incomplete escape sequence at EOF: ",
		     "ESC");

  return 1;
}

