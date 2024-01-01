#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include "clowndev.h"
#include "registers.h"

/*
So far, this function does not seem to work for multisegment files.
*/
static struct DebugFile *lookup_debug_info (Dword address, int *record)
{
  int j;
  /* Search bottom up */
  for (j = current_module - 1; j >= 0; j--) {
    int i;
    if (address < modules[j].offset)
	continue;
    for (i = modules[j].st.size - 1; i >= 0; i--) {
	struct Segment s;
	int k;
	Dword adjusted_address = address - modules[j].st.segments[i].base;
	if ((adjusted_address >= modules[j].st.segments[i].image_size) || (adjusted_address < 0))
	    continue;
	s = modules[j].st.segments[i];
	for (k = s.nfiles - 1; k >= 0; k--) {
	    int l;
	    struct DebugFile *df = &(s.files[k]);
	    for (l = df->nlines_inuse - 1; l >= 0; l--) {
		if (adjusted_address >= df->flines[l].offset) {
		    *record = l;
		    return df;
		}
	    }
	}
    }
    return NULL;		/* is this possible? */
  }
  return NULL;
}

void print_debug_info (Dword address)
{ 
    int location;
    struct DebugFile *df;
    if (PD_PRESENT (clown.PAGE))
      fprintf (stderr, " [paging is on: no debug info]");
    if ((df = lookup_debug_info (address, &location)))
      fprintf (stderr, " [%s:%d]", df->file, df->flines[location].line);
}

static void show_val (Dword val, int format)
{
  unsigned uval;
  int j;
  switch (format) {
  case 'b':
  case 'B':
    uval = (unsigned)val;
    for (j = sizeof (Dword) * 8 - 1; j >= 0; j--)
      fprintf (stderr, "%c", (val & (0x01 << j)) ? '1' : '0');
    fprintf (stderr, "b");
    break;
  case 'c':
  case 'C':
    fprintf (stderr, "'");
    for (j = sizeof (Dword) - 1; j >= 0; j--) {
      unsigned char c = ((char*)&val)[j];
      fprintf (stderr, "%c", isprint (c) ? c : ' ');
    }
    fprintf (stderr, "'");
    break;
  case 'd':
  case 'D':
    fprintf (stderr, "%+011"PRIi32, val);
    break;
  case 'h':
  case 'H':
    fprintf (stderr, "%08"PRIu32"Xh", val);
    break;
  case 'o':
  case 'O':
    fprintf (stderr, "%012"PRIi32"oo", val);
    break;
  case 'a':			/* linear address */
  case 'A':
    fprintf (stderr, "DIR=%08"PRIi32"Xh", L_DIR(val));
    fprintf (stderr, " PAGE=%08"PRIi32"Xh", L_PAGE(val));
    fprintf (stderr, " OFFSET=%08"PRIi32"Xh", L_OFFSET(val));
    break;
  case 'p':			/* page descriptor */
  case 'P':
    fprintf (stderr, "FRAME=%08"PRIi32"Xh", PD_FRAME(val));
    fprintf (stderr, " D=%d", PD_DIRTY(val) != 0);
    fprintf (stderr, " U=%d", PD_USED(val) != 0);
    fprintf (stderr, " W=%d", PD_WRITE(val) != 0);
    fprintf (stderr, " S=%d", PD_SUPER(val) != 0);
    fprintf (stderr, " V=%d", PD_VALID(val) != 0);
    fprintf (stderr, " P=%d", PD_PRESENT(val) != 0);
    break;
  default:
    assert (0);
    break;
  }
}

void show_nmb (Dword value, int format)
{
    show_val (value, format);
    fprintf (stderr, "\n");
}

void show_flags (int format)
{
    fprintf (stderr, "%%FLAGS:");
    fprintf (stderr, " CPL=%1d",  clown.flags.bitwise.cpl  );
    fprintf (stderr, " IOPL=%1d", clown.flags.bitwise.iopl );
    fprintf (stderr, " I=%1d",    clown.flags.bitwise.inter);
    fprintf (stderr, " O=%1d",    clown.flags.bitwise.over );
    fprintf (stderr, " S=%1d",    clown.flags.bitwise.sign );
    fprintf (stderr, " Z=%1d",    clown.flags.bitwise.zero );
    fprintf (stderr, " C=%1d",    clown.flags.bitwise.carry);
    fprintf (stderr, "\n");
}

void show_ir (int format)
{
  fprintf (stderr, "%%IR = ");
  show_val (clown.ir, format);

  switch (I_OPC (clown.ir)) {
  case ROR:
  case RORI:
    fprintf (stderr, " {ROR}");
    break;
  case ROL:
  case ROLI:
    fprintf (stderr, " {ROL}");
    break;
  case SAR:
  case SARI:
    fprintf (stderr, " {SAR}");
    break;
  case SAL:
  case SALI:
    fprintf (stderr, " {SAL}");
    break;
  case GETB:
  case GETBI:
    fprintf (stderr, " {GETB}");
    break;
  case SETB:
  case SETBI:
    fprintf (stderr, " {SETB}");
    break;
  case CLRB:
    fprintf (stderr, " {CLRB}");
    break;
  case xLDI:
  case xLD:
  case xLDS:
  case LDX:
  case xST:
  case xSTS:
  case STX:
  case MOV:
  case MOVSI:
  case MOVTS:
  case MOVFS:
  case GETBY:
  case SETBY:
    fprintf (stderr, " {MOV}");
    break;
  case xXCHG:
    fprintf (stderr, " {XCHG}");
    break;
  case POP:
    fprintf (stderr, " {POP}");
    break;
  case PUSH:
  case xPUSHI:
    fprintf (stderr, " {PUSH}");
    break;
  case ADD:
  case xADDI:
    fprintf (stderr, " {ADD}");
    break;
  case SUB:
  case xSUBI:
    fprintf (stderr, " {SUB}");
    break;
  case MUL:
  case xMULI:
    fprintf (stderr, " {MUL}");
    break;
  case DIV:
  case xDIVI:
  case xDIVJ:
    fprintf (stderr, " {DIV}");
    break;
  case CMP:
  case xCMPI:
    fprintf (stderr, " {CMP}");
    break;
  case NEG:
    fprintf (stderr, " {NEG}");
    break;
  case INC:
    fprintf (stderr, " {INC}");
    break;
  case DEC:
    fprintf (stderr, " {DEC}");
    break;
  case AND:
  case xANDI:
    fprintf (stderr, " {AND}");
    break;
  case OR:
  case xORI:
    fprintf (stderr, " {OR}");
    break;
  case XOR:
  case xXORI:
    fprintf (stderr, " {XOR}");
    break;
  case TST:
  case xTSTI:
    fprintf (stderr, " {TST}");
    break;
  case NOT:
    fprintf (stderr, " {NOT}");
    break;
  case CLI:
    fprintf (stderr, " {CLI}");
    break;
  case STI:
    fprintf (stderr, " {STI}");
    break;
  case CLC:
    fprintf (stderr, " {CLC}");
    break;
  case STC:
    fprintf (stderr, " {STC}");
    break;
  case PUSHF:
    fprintf (stderr, " {PUSHF}");
    break;
  case POPF:
    fprintf (stderr, " {POPF}");
    break;
  case CHIO:
    fprintf (stderr, " {CHIO}");
    break;
  case xNCALL:
  case xFCALL:
  case NCALLX:
    fprintf (stderr, " {CALL}");
    break;
  case NRET:
  case FRET:
  case IFRET:
  case INRET:
    fprintf (stderr, " {RET}");
    break;
  case TRAP:
    fprintf (stderr, " {TRAP}");
    break;
  case JC:
    fprintf (stderr, " {JC}");
    break;
  case JNC:
    fprintf (stderr, " {JNC}");
    break;
  case JO:
    fprintf (stderr, " {JO}");
    break;
  case JNO:
    fprintf (stderr, " {JNO}");
    break;
  case JZ:
    fprintf (stderr, " {JZ}");
    break;
  case JNZ:
    fprintf (stderr, " {JNZ}");
    break;
  case JS:
    fprintf (stderr, " {JS}");
    break;
  case JNS:
    fprintf (stderr, " {JNS}");
    break;
  case xNJMP:
  case xFJMP:
    fprintf (stderr, " {JMP}");
    break;
  case IN:
    fprintf (stderr, " {IN}");
    break;
  case OUT:
  case xOUTI:
    fprintf (stderr, " {OUT}");
    break;
  case HLT:
    fprintf (stderr, " {HLT}");
    break;
  case NOP:
    fprintf (stderr, " {NOP}");
    break;
  case STOP:
    fprintf (stderr, " {STOP}");
    break;
  case CLRBI:
    fprintf (stderr, " {CLRB}");
    break;
  case PEEK:
    fprintf (stderr, " {PEEK}");
    break;
  case POKE:
    fprintf (stderr, " {POKE}");
    break;
  case REM:
  case xREMI:
  case xREMJ:
    fprintf (stderr, " {REM}");
    break;
  }

  if ((I_OPC (clown.ir)) & EXTENSION_BIT) {
    fprintf (stderr, " %%OP3 = ");
    show_val (clown.op3, format);
  }
  fprintf (stderr, "\n");
}

void show_pc (int format)
{
    fprintf (stderr, "%%PC = ");
    show_val (clown.pc, format);
    print_debug_info (clown.pc);
    fprintf (stderr, "\n");
}

void set_pc (Dword val)
{
    if (val < 0)
	fprintf (stderr, "The value of %%PC cannot be negative\n");
    else
	clown.pc = val;
}

void show_regs (Dword nmb, int format)
{
  int i;
  for (i = ((nmb < 0) ? 0 : nmb);
       i < ((nmb < 0) ? CLOWN_NGPR : nmb + 1);
       i++) {
      fprintf (stderr, "%%R%-2u = ", i);
      show_val (clown.gpr[i], format);
      if (i % 2 || nmb >= 0 || format == 'a' || format == 'p')
	  fprintf (stderr, "\n");
      else
	  fprintf (stderr, "\t");
  }
}

static const char *sregs[] = {
    "GDT",
    "LDT",
    "ISR",
    "CS",
    "SS",
    "DS",
    "ES",
    "FS",
};

static void show_selector (Selector s, int format)
{
    fprintf (stderr, "ID=%"PRIu32" RPL=%"PRIu32" %s",
	     SEL_ID (s), SEL_RPL (s), SEL_TABL(s) ? "GDT" : "LDT");
}

static void show_sflags (struct Clown_Segment_Descriptor sd)
{
    fprintf (stderr, "P=%d ", SF_PRESENT (sd) != 0);
    fputs ("PERM=", stderr);
    if (READABLE (sd))
	fputs ("R", stderr);
    else
	fputs ("-", stderr);
    if (WRITABLE (sd))
	fputs ("W", stderr);
    else
	fputs ("-", stderr);
    if (EXECUTABLE (sd))
	fputs ("X", stderr);
    else
	fputs ("-", stderr);
    fprintf (stderr, " DPL=%"PRIu32, SF_DPL (sd));
}

static void show_sreg (struct Clown_Segment_Register sreg, int format)
{
    fputs ("  Selector  : ", stderr);
    show_selector (sreg.selector, format);
    fputs ("\n  Descriptor: [", stderr);
    show_val (sreg.descriptor.base, format);
    fputs ("]:[", stderr);
    show_val (sreg.descriptor.base + sreg.descriptor.limit, format);
    fputs ("] ", stderr);
    show_sflags (sreg.descriptor);
}

void show_sregs (Dword nmb, int format)
{
    fprintf (stderr, "%%%s\n", sregs[nmb]);
    show_sreg (clown.segr[nmb], format);
    fprintf (stderr, "\n");
}

void set_regs (Dword nmb, Dword value)
{
  int i;
  for (i = ((nmb < 0) ? 0 : nmb);
       i < ((nmb < 0) ? CLOWN_NGPR : nmb + 1);
       i++) {
      clown.gpr[i] = value;
  }
}

void set_mem (Dword address, Dword value)
{
    if (address < CLOWN_MEMORY_SIZE && address >= 0) {
	clown_write_mem (address, value);
    } else {
	fprintf (stderr, "Address out of range: ");
	show_val (address, 'h');
	fprintf (stderr, "\n");
    }
}

void set_refregs (Dword nmb, Dword value)
{
  int i;
  for (i = ((nmb < 0) ? 0 : nmb);
       i < ((nmb < 0) ? CLOWN_NGPR : nmb + 1);
       i++) {
      Dword address;
      address = clown.gpr[i];
      set_mem (address, value);
  }
}

static void show_mem (Dword address, int format)
{
    if (address < CLOWN_MEMORY_SIZE && address >= 0) {
	Dword datum;
	clown_read_mem (address, &datum);
	fprintf (stderr, "RAM[0x%08X] = ", (unsigned int)address);
	show_val (datum, format);
    } else {
	fprintf (stderr, "Address out of range: 0x%08X", (unsigned int)address);
    }
    print_debug_info (address);
    fprintf (stderr, "\n");
}

void show_range (Dword address, Dword range, int format)
{
    int i;
    if (range >= 0) {
	for (i = 0; i < range; i++)
	    show_mem (address + i, format);
    } else {
	for (i = range; i < 0; i++)
	    show_mem (address + i, format);
    }
}

void show_refrange (Dword nmb, Dword range, int format)
{
  int i;
  if (range != 1 && nmb < 0) {
      fprintf (stderr, "Cannot combine [%%all] and 'range\n");
      return;
  }

  for (i = ((nmb < 0) ? 0 : nmb);
       i < ((nmb < 0) ? CLOWN_NGPR : nmb + 1);
       i++) {
      Dword address;
      address = clown.gpr[i];
      if (range == 1)
	  fprintf (stderr, "[%%R%-2u]: ", i);
      show_range (address, range, format);
  }
}

void show_cmd_stats (cycle_t cycles)
{
  fprintf (stderr, "Fetch unit %s. ", fetch ? "on" : "off");
  fprintf (stderr, "Bus cycles: %u. ", cycles);
  fprintf (stderr, "Time = %lld.\n", clown_time);
  show_pc ('d');
  show_ir ('H');
  if (pending_exception) {
    int i;
    fprintf (stderr, "PENDING EXCEPTIONS: ");
    for (i = 0; i < 16; i++)
      if (exception_status & (0x01 << i)) {
	fprintf (stderr, "[%s #%XH] ", exception_code (i), i);
      }
    fprintf (stderr, "\n");
  }
}
