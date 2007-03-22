#include <assert.h>
#include "registers.h"
#include "exceptions.h"

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
    offset = SEL_ID (seg_sel) * sizeof (seg_descr) / sizeof (Dword);

    if (offset >= table.limit) {
	raise_exception (PROTECTION_EX);
	return EFAIL;
    }

    /* Load the body of the descriptor from the respective table */
    cycles = clown_read_linear (table.base + offset + 0, &seg_descr->limit);
    if (cycles == EFAIL) 
	return EFAIL;
    cycles_all += cycles;
    cycles = clown_read_linear (table.base + offset + 1, &seg_descr->base);
    if (cycles == EFAIL) 
	return EFAIL;
    cycles_all += cycles;
    cycles = clown_read_linear (table.base + offset + 2, &datum);
    if (cycles == EFAIL) 
	return EFAIL;
    cycles_all += cycles;
    seg_descr->flags = datum;

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
    Bit rpl, dpl, is_door = 0;
    struct Clown_Segment_Descriptor seg_descr;
    cycle_t cycles_all = 0, cycles;
    Dword offset = 0;

    /* Load the target descriptor */
    if ((cycles = clown_load_seg_descr (seg_sel, &seg_descr)) == EFAIL)
	return EFAIL;
    cycles_all += cycles;

    rpl = SEL_RPL (seg_sel);
    dpl =  SF_DPL (seg_descr);

    /* Now, what kind of segment is this? Is it a DOOR? */
    if (SF_DOOR (seg_descr)) {
	Selector door;
	/* Only code segments are allowed to use doors */
	if (reg != _CODE) {
	    raise_exception (PROTECTION_EX);
	    return EFAIL;
	}

	door   = seg_descr.DOOR_SELECTOR;
	offset = seg_descr.DOOR_OFFSET;

	/* Go and load yet another segment! It's selector is the base 
	   of the door */
	if ((cycles = clown_load_seg_descr (door, &seg_descr)) == EFAIL)
	    return EFAIL;
	cycles_all += cycles;

	/* Too many doors! */
	if (SF_DOOR (seg_descr)) {
	    raise_exception (PROTECTION_EX);
	    return EFAIL;
	}

	is_door = 1;
    }


    /* Check protections; they are different for different segments */
    /* _ISR limit must be large enough */
    if (reg == _ISR && seg_descr.limit < MAX_TRAP) {
	raise_exception (PROTECTION_EX);
	return EFAIL;
    }

    switch (reg) {
    case _ISR:			/* ISR table */
    case _GDT:			/* Global descriptors */
    case _LDT:			/* Local descriptors */
	/* Only the inner ring is allowed to change these tables */
	if (clown.flags.bitwise.cpl) {
	    raise_exception (PROTECTION_EX);
	    return EFAIL;
	}

	/* These tables must be readable */
	if (!READABLE (seg_descr)) {
	    raise_exception (PROTECTION_EX);
	    return EFAIL;
	}
	break;
                                                            
    case _CODE:			/* Code */
	/* Code must be executable */
	if (!EXECUTABLE (seg_descr)) {
	    raise_exception (PROTECTION_EX);
	    return EFAIL;
	}

	/* one cannot execute segments from the inner rings */
	if ((is_door && (dpl <  MAX (rpl, clown.flags.bitwise.cpl))) 
	             || (dpl !=           clown.flags.bitwise.cpl)) {
	    raise_exception (PROTECTION_EX);
	    return EFAIL;
	}

	/* change the current protection level */
	clown.flags.bitwise.cpl = SF_DPL (seg_descr);
	break;

    case _STACK:		/* Stack */
	/* Stacks must be writable */
	if (!WRITABLE (seg_descr)) {
	    raise_exception (PROTECTION_EX);
	    return EFAIL;
	}
	/* one cannot access protected segments */
	if (dpl < MAX (clown.flags.bitwise.cpl, rpl)) {
	    raise_exception (PROTECTION_EX);
	    return EFAIL;
	}

	break;

    case _DATA:			/* Data */
	/* Data must be either readable or writable */
	if (!WRITABLE (seg_descr) && !READABLE (seg_descr)) {
	    raise_exception (PROTECTION_EX);
	    return EFAIL;
	}

	/* one cannot access protected segments */
	if (dpl < MAX (clown.flags.bitwise.cpl, rpl)) {
	    raise_exception (PROTECTION_EX);
	    return EFAIL;
	}
	break;

    default:
	raise_exception (PROTECTION_EX);
	return EFAIL;
    }

    /* We are clean and all sat */
    clown.segr[reg].selector   = seg_sel;
    clown.segr[reg].descriptor = seg_descr;

    /* If it was a door descriptor, we have to report the adjusted PC */
    if (is_door) {
	assert (new_pc);
	*new_pc = offset;
    }

    return cycles;
}

