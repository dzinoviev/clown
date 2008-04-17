#include <assert.h>
#include "registers.h"
#include "exceptions.h"
#include <stdio.h>		/* FIXME */

static const char *_errors[] = {
    "%cs access or usage violation.",
    "CPL violation.",
    "Segmentation violation.",
    "Invalid segment register.",
    "ISR segment too small.",
};

void bark (int err)
{
    if (!silent)
	fprintf (stderr, "--> %s\n", _errors[err]);
}


cycle_t clown_load_seg_descr (Selector seg_sel, 
			      struct Clown_Segment_Descriptor *seg_descr) 
{
    struct Clown_Segment_Descriptor table;
    cycle_t cycles_all = 0, cycles;
    Uword datum;
    Uword offset;

    /* Trivial sanity check */
    if (seg_sel == clown.segr[SEL_TABL (seg_sel)].selector)
	return 0;

    /* The segment will be either from the GDT or from the LDT */

    /* It is assumed that the table is valid, 
       because its validity was checked at the 
       time of table loading */
    table  = clown.segr[SEL_TABL (seg_sel)].descriptor;
    offset = SEL_ID (seg_sel) * LGDT_ENTRY_SIZE;

    if (offset >= table.limit) {
	bark (SEGVIOL);
	raise_exception (PROTECTION_EX);
	return EFAIL;
    }

    /*    fprintf (stderr, "Table.base=%d Table.limit=%d Offset=%d\n", table.base, table.limit, offset);*/

    /* Load the body of the descriptor from the respective table */
    cycles = clown_read_linear (table.base + offset + 2, (Dword*)&seg_descr->limit);
    if (cycles == EFAIL) 
	return EFAIL;
    cycles_all += cycles;
    cycles = clown_read_linear (table.base + offset + 1, (Dword*)&seg_descr->base);
    if (cycles == EFAIL) 
	return EFAIL;
    cycles_all += cycles;
    cycles = clown_read_linear (table.base + offset + 0, (Dword*)&datum);
    if (cycles == EFAIL) 
	return EFAIL;
    cycles_all += cycles;
    seg_descr->flags = datum;

    /*fprintf (stderr, "Limit=%d base=%d flags=%d\n", seg_descr->limit, seg_descr->base, datum);*/
    /* Is this segment present or not? If not, 
       signal an exception to the the VMS */
    if (!SF_PRESENT (*seg_descr)) {
	clown.FAR = seg_sel;
	raise_exception (SEGFAULT_EX);
	return EFAIL;
    }

    return cycles_all;
}

#define MAX(x,y) ((x)>(y)?(x):(y))
cycle_t clown_load_seg (Selector seg_sel, int reg, Dword *new_pc)
{
    Bit rpl, dpl;
    struct Clown_Segment_Descriptor seg_descr;
    cycle_t cycles_all = 0, cycles;

    /* Load the target descriptor */
    if ((cycles = clown_load_seg_descr (seg_sel, &seg_descr)) == EFAIL)
	return EFAIL;
    cycles_all += cycles;

    rpl = SEL_RPL (seg_sel);
    dpl =  SF_DPL (seg_descr);

    /* Check protections; they are different for different segments */
    /* _ISR limit must be large enough */
    if (reg == _ISR && seg_descr.limit < MAX_TRAP) {
	bark (SMALLISR);
	raise_exception (PROTECTION_EX);
	return EFAIL;
    }

    switch (reg) {
    case _ISR:			/* ISR table */
    case _GDT:			/* Global descriptors */
    case _LDT:			/* Local descriptors */
	/* Only the inner ring is allowed to change these tables */
	if (clown.flags.bitwise.cpl) {
	    bark (CPLVIOL);
	    raise_exception (PROTECTION_EX);
	    return EFAIL;
	}

	/* These tables must be readable */
	if (!READABLE (seg_descr)) {
	    bark (SEGVIOL);
	    raise_exception (PROTECTION_EX);
	    return EFAIL;
	}
	break;
                                                            
    case _CODE:			/* Code */
	/* Code must be executable */
	if (!EXECUTABLE (seg_descr)) {
	    bark (SEGVIOL);
	    raise_exception (PROTECTION_EX);
	    return EFAIL;
	}

	/* change the current protection level */
	clown.flags.bitwise.cpl = SF_DPL (seg_descr);
	break;

    case _STACK:		/* Stack */
	/* Stacks must be writable */
	if (!WRITABLE (seg_descr)) {
	    bark (SEGVIOL);
	    raise_exception (PROTECTION_EX);
	    return EFAIL;
	}
	/* one cannot access protected segments */
	if (dpl < MAX (clown.flags.bitwise.cpl, rpl)) {
	    bark (CPLVIOL);
	    raise_exception (PROTECTION_EX);
	    return EFAIL;
	}

	break;

    case _DATA:			/* Data */
    case _ES:			/* Data */
    case _FS:			/* Data */
	/* Data must be either readable or writable */
	if (!WRITABLE (seg_descr) && !READABLE (seg_descr)) {
	    bark (SEGVIOL);
	    raise_exception (PROTECTION_EX);
	    return EFAIL;
	}

	/* one cannot access protected segments */
	if (dpl < MAX (clown.flags.bitwise.cpl, rpl)) {
	    bark (CPLVIOL);
	    raise_exception (PROTECTION_EX);
	    return EFAIL;
	}
	break;

    default:
	bark (INVSEGR);
	raise_exception (PROTECTION_EX);
	return EFAIL;
    }

    /* We are clean and ready to go */
    clown.segr[reg].selector   = seg_sel;
    clown.segr[reg].descriptor = seg_descr;
    /* printf ("New selector in [%d; %d?]: %d\n", reg, _CODE, clown.segr[reg].selector);*/

    return cycles;
}

