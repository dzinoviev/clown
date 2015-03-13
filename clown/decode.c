#include <string.h>
#include <stdio.h>
#include "registers.h"
#include "exceptions.h"

static cycle_t do_move_to_segment (Selector s, Bit reg)
{
    if (reg == _CODE) {
	bark (SEG2CODE);
	raise_exception (PROTECTION_EX);
	return EFAIL;
    }
    
    /* Try to load the target segment */
    return clown_load_seg (s, reg);
}


static cycle_t do_move_to_regular (Dword datum, Bit reg)
{
    if (reg == _PAGE) {	
	/* Only the inner ring may change the page table */
	if (clown.flags.bitwise.cpl) {
	    bark (CPLVIOL);
	    raise_exception (PROTECTION_EX);
	    return EFAIL;
	} 
	/* page table reloaded - TLB must be purged */
	clown_clear_TLB ();
    }
    clown.gpr[reg] = datum;
    return 0;
}

static cycle_t do_return (Bit interrupt)
{
    Dword pc, tmp, flags;
    cycle_t cycles, cycles_all = 0;
    Selector cs;
    Bit old_cpl;

    if ((cycles = do_pop (&pc, PUSHPOP)) == EFAIL) /* pop PC */
	return EFAIL;
    cycles_all += cycles;

    if ((cycles = do_pop (&tmp, PUSHPOP)) == EFAIL) { /* pop CS */
	cycles_all += do_push (pc, PUSHPOP);
	return EFAIL;
    }
    cycles_all += cycles;

    if (interrupt) {
	if ((cycles = do_pop (&flags, PUSHPOP)) == EFAIL) { /* pop the flags */
	    cycles_all += do_push (tmp, PUSHPOP);
	    cycles_all += do_push (pc, PUSHPOP);
	    return EFAIL;
	}
	cycles_all += cycles;
    }


    cs = (Selector)tmp;
    old_cpl = clown.flags.bitwise.cpl; /* old CPL */

    if ((cycles = clown_load_seg (cs, _CODE)) == EFAIL) {
	/* restore the stack */
	cycles_all += do_push (flags, PUSHPOP);
	cycles_all += do_push (tmp, PUSHPOP);
	cycles_all += do_push (pc, PUSHPOP);
	return EFAIL;
    }
    cycles_all += cycles;

    /* Do we have an inter-ring return? */
    if (old_cpl              < clown.flags.bitwise.cpl &&
	 SF_DPL (clown.DATA) < clown.flags.bitwise.cpl)
	/* if so, invalidate the data segment */
	clown.DATA.limit = 0;

    if (interrupt)
	clown.flags.raw = flags;

    clown.pc = pc;
    return cycles;
}

cycle_t do_call (Selector new_seg, Dword new_pc, Dword save_pc)
{
    Dword datum;
    cycle_t cycles_all = 0, cycles;
    
    cycles = do_push (clown.segr[_CODE].selector, PUSHPOP);
    if (cycles != EFAIL) {
	cycles_all += cycles;
	cycles = do_push (save_pc, PUSHPOP);
	if (cycles != EFAIL) {
	    cycles_all += cycles;
	    cycles = clown_load_seg (new_seg, _CODE);
	    if (cycles != EFAIL) {
		cycles_all += cycles;
		clown.pc = new_pc;
	    } else {
		/* restore the stack - remove the PC */ 
		cycles_all += do_pop (&datum, PUSHPOP);
		/* restore the stack - remove the CS */ 
		cycles_all += do_pop (&datum, PUSHPOP); 
	    }
	} else
	    /* restore the stack - remove the CS */
	    cycles_all += do_pop (&datum, PUSHPOP); 
    }
    return cycles_all;
}

static void do_flags (DDword result)
{
    clown.flags.bitwise.sign =  (result <  0);
    clown.flags.bitwise.zero =  (result == 0);
    clown.flags.bitwise.over =  (result >  DWORD_MAX);
    clown.flags.bitwise.carry = (result >  DWORD_MAX) 
	                     || (result <  DWORD_MIN);
}

cycle_t do_pop (Dword *datum, Dword offset) 
{
    cycle_t cycles = 0;
    Dword addr = clown.SP;
    if (offset != PUSHPOP)
	addr += offset;
    /* Do we have enough items on the stack? */
    if (addr >= clown.STACK.limit || !READABLE (clown.STACK)) {
	bark (SEGVIOL);
	raise_exception (PROTECTION_EX);
	return EFAIL;
    } else {
	/* Check segment boundaries here! */
	cycles = clown_read_linear (clown.STACK.base + addr, datum);
	if (cycles != EFAIL && offset == PUSHPOP)
	    clown.SP++;
    }
    /*    printf ("<< %d\n", *datum);*/
    return cycles;
}

cycle_t do_push (Dword datum, Dword offset) 
{
    cycle_t cycles = 0;
    Dword addr = clown.SP;
    if (offset != PUSHPOP)
	addr += offset + 1;
    /* Do we have enough stack space? */
    if (addr == 0) {
	raise_exception (STACKOVER_EX);
	return EFAIL;
    } else {
	addr--; /* new head of stack */
	/* Check segment boundaries here! */
	if (addr >= clown.STACK.limit || !WRITABLE (clown.STACK)) {
	    bark (SEGVIOL);
	    raise_exception (PROTECTION_EX);
	    return EFAIL;
	} else
	    if ((cycles
		 = clown_write_linear (clown.STACK.base + addr, datum)) 
		!= EFAIL && offset == PUSHPOP)
		clown.SP = addr;
    }
    /*    printf (">> %d\n", datum);*/
    return cycles;
}

#define RELATIVE 0
#define ABSOLUTE 1

static cycle_t jumpOnCondition (Bit condition, Bit absolute, Dword addr) 
{
  if (condition) {
    Dword dest;
    /* absolute or relative? */
    dest = (absolute ? 0 : clown.pc) + addr;
    
    /* Check segment boundaries here! */
    if (dest >= clown.CODE.limit) {
	bark (SEGVIOL);
	raise_exception (PROTECTION_EX);
	return EFAIL;
    }
    clown.pc = dest;
  }
  return 0;
}

cycle_t clown_decode_execute (Dword i, Dword op3)
{
    cycle_t cycles, cycles_all = 0;
    Dword op1, op2, addr;
    Selector seg1, seg2;
    Dword datum, datum1 = 0;
    DDword result = 0;
    Bit b, ilevel;
  
    switch (I_OPC (i)) {
    case ROR:
    case RORI:
	op1 = I_OP1 (i);
	if (I_OPC (i) == RORI)
	    op2 = I_DSPL (i) & DWORD_BITMASK;
	else
	    op2 = clown.gpr[I_OP2 (i)] & DWORD_BITMASK;
	datum = ((unsigned)clown.gpr[op1] >> op2) 
	    | ((unsigned)clown.gpr[op1] << (sizeof (Dword) * CHAR_BIT - op2));
	cycles_all = do_move_to_regular (datum, op1);
	if (cycles_all != EFAIL)
	    do_flags (result);
	break;
    case ROL:  
    case ROLI:  
	op1 = I_OP1 (i);
	if (I_OPC (i) == ROLI)
	    op2 = I_DSPL (i) & DWORD_BITMASK;
	else
	    op2 = clown.gpr[I_OP2 (i)] & DWORD_BITMASK;
	datum = ((unsigned)clown.gpr[op1] << op2) 
	    | ((unsigned)clown.gpr[op1] >> (sizeof (Dword) * CHAR_BIT - op2));
	cycles_all = do_move_to_regular (datum, op1);
	if (cycles_all != EFAIL)
	    do_flags (result);
	break;
    case SAR: 
    case SARI:
	op1 = I_OP1 (i);
	if (I_OPC (i) == SARI)
	    op2 = I_DSPL (i) & DWORD_BITMASK;
	else
	    op2 = clown.gpr[I_OP2 (i)] & DWORD_BITMASK;
	datum = clown.gpr[op1] >> op2;
	cycles_all = do_move_to_regular (datum, op1);
	if (cycles_all != EFAIL)
	    do_flags (result);
	break;
    case SAL: 
    case SALI: 
	op1 = I_OP1 (i);
	if (I_OPC (i) == SALI)
	    op2 = I_DSPL (i) & DWORD_BITMASK;
	else
	    op2 = clown.gpr[I_OP2 (i)] & DWORD_BITMASK;
	datum = clown.gpr[op1] << op2;
	cycles_all = do_move_to_regular (datum, op1);
	if (cycles_all != EFAIL)
	    do_flags (result);
	break;

    case GETB: 
	op1 = I_OP1 (i);
	op2 = I_OP2 (i);
	b = (clown.gpr[op1] & (0x1 << clown.gpr[op2])) != 0;
	clown.flags.bitwise.carry = b;
	break;

    case GETBI: 
	op1 = I_OP1 (i);
	op2 = I_DSPL (i);
	b = (clown.gpr[op1] & (0x1 << op2)) != 0;
	clown.flags.bitwise.carry = b;
	break;

    case SETB: 
	op1 = I_OP1 (i);
	op2 = I_OP2 (i);
	clown.gpr[op1] |= (0x1 << clown.gpr[op2]);
	break;

    case SETBI: 
	op1 = I_OP1 (i);
	op2 = I_DSPL (i);
	clown.gpr[op1] |= (0x1 << op2);
	break;

    case CLRBI: 
	op1 = I_OP1 (i);
	op2 = I_DSPL (i);
	datum = clown.gpr[op1] & ~(0x1 << op2);
	cycles_all = do_move_to_regular (datum, op1);
	break;

    case CLRB: 
	op1 = I_OP1 (i);
	op2 = I_OP2 (i);
	datum = clown.gpr[op1] & ~(0x1 << clown.gpr[op2]);
	cycles_all = do_move_to_regular (datum, op1);
	break;

    case GETBY: 
	op1  = I_OP1 (i);
	op2  = I_OP2 (i);
	addr = I_DSPL (i) & (sizeof (Dword) - 1);
	datum = ((char*)&clown.gpr[op2])[addr];
	cycles_all = do_move_to_regular (datum, op1);
	break;

    case SETBY: 
	op1  = I_OP1 (i);
	op2  = I_OP2 (i);
	if (op2 == _PAGE) {	
	    /* Only the inner ring may change the page table */
	    if (clown.flags.bitwise.cpl) {
		bark (CPLVIOL);
		raise_exception (PROTECTION_EX);
		return EFAIL;
	    } 
	    /* page table reloaded - TLB must be purged */
	    clown_clear_TLB ();
	}
	addr = I_DSPL (i) & (sizeof (Dword) - 1);
	((char*)&clown.gpr[op2])[addr] = (char)clown.gpr[op1];
	break;

    case xLDI: 
	op1 = I_OP1 (i);
	cycles_all = do_move_to_regular (op3, op1);
	break;

    case xLD: 
	op1 = I_OP1 (i);	/* where to load? */
	if (op3 >= clown.DATA.limit || !READABLE (clown.DATA)) {
	    bark (SEGVIOL);
	    raise_exception (PROTECTION_EX);
	    return EFAIL;
	}
	/* fetch the thing from the memory */
	cycles_all = clown_read_linear (clown.DATA.base + op3, &datum);
	if (cycles_all != EFAIL) {
	    cycles = do_move_to_regular (datum, op1);
	    if (cycles != EFAIL)
		cycles_all += cycles;
	    else
		return EFAIL;
	}
        break;

    case xLDS: 
	op1 = I_OP1 (i);	/* where to load? */
	op2 = I_OP2 (i); /* data segment to use */
	if (op2 >= CLOWN_NSEGR) {
	    bark (INVSEGR);
	    raise_exception (PROTECTION_EX);
	    return EFAIL;
	}
	if (   op3 >= clown.segr[op2].descriptor.limit
	    || !READABLE (clown.segr[op2].descriptor)) {
	    bark (SEGVIOL);
	    raise_exception (PROTECTION_EX);
	    return EFAIL;
	}
	/* fetch the thing from the memory */
	cycles_all = clown_read_linear (clown.segr[op2].descriptor.base + op3, 
					&datum);
	if (cycles_all != EFAIL) {
	    cycles = do_move_to_regular (datum, op1);
	    if (cycles != EFAIL)
		cycles_all += cycles;
	    else
		return EFAIL;
	}
        break;			/* WORKS */

    case LDX: 
	op1  = I_OP1 (i);	/* where to load? */
	op2  = I_OP2 (i); /* index register */
	addr = clown.gpr[op2];	/* memory address */
	if (addr >= clown.DATA.limit || !READABLE (clown.DATA)) {
	    bark (SEGVIOL);
	    raise_exception (PROTECTION_EX);
	    return EFAIL;
	}
	/* fetch the thing from the memory */
	cycles_all = clown_read_linear (clown.DATA.base + addr, &datum);
	if (cycles_all != EFAIL) {
	    cycles = do_move_to_regular (datum, op1);
	    if (cycles != EFAIL)
		cycles_all += cycles;
	    else
		return EFAIL;
	}
	break;

    case xST: 
	op1 = I_OP1 (i);	/* what to store? */
	if (op3 >= clown.DATA.limit || !WRITABLE (clown.DATA)) {
	    bark (SEGVIOL);
	    raise_exception (PROTECTION_EX);
	    return EFAIL;
	}
	/* fetch the thing from the memory */
	cycles_all = clown_write_linear (clown.DATA.base + op3, 
					 clown.gpr[op1]);
	break;

    case xSTS: 
	op1 = I_OP1 (i);	/* data segment to use */	
	op2 = I_OP2 (i);	/* what to store? */
	if (op1 >= CLOWN_NSEGR) {
	    bark (INVSEGR);
	    raise_exception (PROTECTION_EX);
	    return EFAIL;
	}
	if (   op3 >= clown.segr[op1].descriptor.limit
	    || !WRITABLE (clown.segr[op1].descriptor)) {
	    bark (SEGVIOL);
	    raise_exception (PROTECTION_EX);
	    return EFAIL;
	}
	/* fetch the thing from the memory */
	cycles_all = clown_write_linear (clown.segr[op1].descriptor.base + op3,
					 clown.gpr[op2]);
	break;

    case STX: 
	op1  = I_OP1 (i);	/* index register */
	op2  = I_OP2 (i);       /* what to store? */
	addr = clown.gpr[op1];	/* memory address */
	if (addr >= clown.DATA.limit 
	    || !WRITABLE (clown.DATA)) {
	    bark (SEGVIOL);
	    raise_exception (PROTECTION_EX);
	    return EFAIL;
	}
	/* fetch the thing from the memory */
	cycles_all = clown_write_linear (clown.DATA.base + addr, 
					 clown.gpr[op2]);
	break;

    case MOV: 
	op1 = I_OP1 (i);
	op2 = I_OP2 (i);
	cycles_all = do_move_to_regular (clown.gpr[op2], op1);
	break;

    case MOVFS:			/* move from segment register  */
	/* Trim the register # */
	op1 = I_OP1 (i);
	op2 = I_OP2 (i) & ~((~(Bit)0) << CLOWN_SEGR_BITS);
	cycles_all = do_move_to_regular (clown.segr[op2].selector, op1);
	break;

    case MOVSI:			/* move to segment register immediate */
	/* Trim the register # */
	addr = I_OP1 (i) & ~((~(Bit)0) << CLOWN_SEGR_BITS);
	seg2 = I_SEG (i);
	cycles_all = do_move_to_segment (seg2, addr);
	break;			/* WORKS */

    case MOVTS:			/* move to segment register  */
	/* Trim the register # */
	addr = I_OP1 (i) & ~((~(Bit)0) << CLOWN_SEGR_BITS);
	op2  = I_OP2 (i);
	cycles_all = do_move_to_segment (clown.gpr[op2], addr);
	break;

    case xXCHG:
	op1 = I_OP1 (i);	/* which register to move to MEM? */
	if (op3 >= clown.DATA.limit 
	    || !READABLE (clown.DATA) 
	    || !WRITABLE (clown.DATA)) {
	    bark (SEGVIOL);
	    raise_exception (PROTECTION_EX);
	    return EFAIL;
	}
	/* fetch the thing from the memory */
	cycles_all = clown_read_linear (clown.DATA.base + op3, &datum);
	if (cycles_all != EFAIL) {	    
	    cycles = clown_write_linear (clown.DATA.base + op3, 
					 clown.gpr[op1]);
	    if (cycles != EFAIL) {
		cycles_all += cycles;
		cycles = do_move_to_regular (datum, op1);
		if (cycles != EFAIL)
		    cycles_all += cycles;
		else
		    return EFAIL;
	    }
	}
        break;

    case POP: 
	op1  = I_OP1 (i);	/* into which register to pop */
	cycles_all = do_pop (&datum, PUSHPOP);
	if (cycles_all != EFAIL) {
	    cycles = do_move_to_regular (datum, op1);
	    if (cycles != EFAIL)
		cycles_all += cycles;
	    else
		return EFAIL;
	}
	break;

    case PEEK: 
	op1  = I_OP1 (i);	/* into which register to pop */
	op2  = I_DSPL (i);	/* distance from the top of the stack */
	cycles_all = do_pop (&datum, op2);
	if (cycles_all != EFAIL) {
	    cycles = do_move_to_regular (datum, op1);
	    if (cycles != EFAIL)
		cycles_all += cycles;
	    else
		return EFAIL;
	}
	break;

    case PUSH:
	op1  = I_OP1 (i);	/* which register to push */
	cycles_all = do_push (clown.gpr[op1], PUSHPOP);
	break;
      
    case POKE:
	op1  = I_OP1 (i);	/* which register to push */
	op2  = I_DSPL (i);	/* distance from the top of the stack */
	cycles_all = do_push (clown.gpr[op1], op2);
	break;
      
    case xPUSHI:
	cycles_all = do_push (op3, PUSHPOP);
	break;
      
    case ADD:
	op1 = I_OP1 (i);
	op2 = I_OP2 (i);
	result = (DDword)clown.gpr[op1] + (DDword)clown.gpr[op2];
	cycles_all = do_move_to_regular ((Dword)result, op1);
	if (cycles_all != EFAIL)
	    do_flags (result);
	break;

    case xADDI:
	op1 = I_OP1 (i);
	result = (DDword)clown.gpr[op1] + (DDword)op3;
	cycles_all = do_move_to_regular ((Dword)result, op1);
	if (cycles_all != EFAIL)
	    do_flags (result);
	break;

    case SUB: 
	op1 = I_OP1 (i);
	op2 = I_OP2 (i);
	result = (DDword)clown.gpr[op1] - (DDword)clown.gpr[op2];
	cycles_all = do_move_to_regular ((Dword)result, op1);
	if (cycles_all != EFAIL)
	    do_flags (result);
	break;
    case xSUBI: 
	op1 = I_OP1 (i);
	result = (DDword)clown.gpr[op1] - (DDword)op3;
	cycles_all = do_move_to_regular ((Dword)result, op1);
	if (cycles_all != EFAIL)
	    do_flags (result);
	break;
    case MUL: 
	op1 = I_OP1 (i);
	op2 = I_OP2 (i);
	result = (DDword)clown.gpr[op1] * (DDword)clown.gpr[op2];
	cycles_all = do_move_to_regular ((Dword)result, op1);
	if (cycles_all != EFAIL)
	    do_flags (result);
	break;
    case xMULI: 
	op1 = I_OP1 (i);
	result = (DDword)clown.gpr[op1] * (DDword)op3;
	cycles_all = do_move_to_regular ((Dword)result, op1);
	if (cycles_all != EFAIL)
	    do_flags (result);
	break;
    case xDIVI:
    case xDIVJ:
    case DIV: 
    case xREMI:
    case xREMJ:
    case REM:
	addr = I_OP1 (i);
	switch (I_OPC (i)) {
	case DIV:
	case REM:
	    datum = clown.gpr[addr];
	    datum1 = clown.gpr[I_OP2 (i)];
	    break;
	case xDIVI:
	case xREMI:
	    datum = clown.gpr[addr];
	    datum1 = op3;
	    break;
	case xDIVJ:
	case xREMJ:
	    datum = op3;
	    datum1 = clown.gpr[addr];
	    break;
	}

	if (!datum1) {
	    raise_exception (DIVIDEZERO_EX);
	    cycles_all = EFAIL;
	} else {
	    if (I_OPC (i) == REM || I_OPC (i) == xREMI || I_OPC (i) == xREMJ) {
		result = (DDword)datum % (DDword)datum1;
	    } else {
		result = (DDword)datum / (DDword)datum1;
	    }
	    cycles_all = do_move_to_regular ((Dword)result, addr);
	    if (cycles_all != EFAIL)
		do_flags (result);
	}

	break;
    case CMP: 
	op1 = I_OP1 (i);
	op2 = I_OP2 (i);
	result = (DDword)clown.gpr[op1] - (DDword)clown.gpr[op2];
	do_flags (result);
	break;
    case xCMPI: 
	op1 = I_OP1 (i);
	result = (DDword)clown.gpr[op1] - (DDword)op3;
	do_flags (result);
	break;
    case NEG: 
	op1 = I_OP1 (i);
	result = -(DDword)clown.gpr[op1];
	cycles_all = do_move_to_regular ((Dword)result, op1);
	if (cycles_all != EFAIL)
	    do_flags (result);
	break;
    case INC: 
	op1 = I_OP1 (i);
	result = (DDword)clown.gpr[op1] + 1;
	cycles_all = do_move_to_regular ((Dword)result, op1);
	if (cycles_all != EFAIL)
	    do_flags (result);
	break;
    case DEC: 
	op1 = I_OP1 (i);
	result = (DDword)clown.gpr[op1] - 1;
	cycles_all = do_move_to_regular ((Dword)result, op1);
	if (cycles_all != EFAIL)
	    do_flags (result);
	break;
    case AND: 
	op1 = I_OP1 (i);
	op2 = I_OP2 (i);
	result = (DDword)clown.gpr[op1] & (DDword)clown.gpr[op2];
	cycles_all = do_move_to_regular ((Dword)result, op1);
	if (cycles_all != EFAIL)
	    do_flags (result);
	break;
    case xANDI: 
	op1 = I_OP1 (i);
	result = (DDword)clown.gpr[op1] & (DDword)op3;
	cycles_all = do_move_to_regular ((Dword)result, op1);
	if (cycles_all != EFAIL)
	    do_flags (result);
	break;
    case OR : 
	op1 = I_OP1 (i);
	op2 = I_OP2 (i);
	result = (DDword)clown.gpr[op1] | (DDword)clown.gpr[op2];
	cycles_all = do_move_to_regular ((Dword)result, op1);
	if (cycles_all != EFAIL)
	    do_flags (result);
	break;
    case xORI: 
	op1 = I_OP1 (i);
	result = (DDword)clown.gpr[op1] | (DDword)op3;
	cycles_all = do_move_to_regular ((Dword)result, op1);
	if (cycles_all != EFAIL)
	    do_flags (result);
	break;
    case XOR: 
	op1 = I_OP1 (i);
	op2 = I_OP2 (i);
	result = (DDword)clown.gpr[op1] ^ (DDword)clown.gpr[op2];
	cycles_all = do_move_to_regular ((Dword)result, op1);
	if (cycles_all != EFAIL)
	    do_flags (result);
	break;
    case xXORI: 
	op1 = I_OP1 (i);
	result = (DDword)clown.gpr[op1] ^ (DDword)op3;
	cycles_all = do_move_to_regular ((Dword)result, op1);
	if (cycles_all != EFAIL)
	    do_flags (result);
	break;
    case TST: 
	op1 = I_OP1 (i);
	op2 = I_OP2 (i);
	result = (DDword)clown.gpr[op1] & (DDword)clown.gpr[op2];
	do_flags (result);
	break;
    case xTSTI: 
	op1 = I_OP1 (i);
	result = (DDword)clown.gpr[op1] & (DDword)op3;
	do_flags (result);
	break;
    case NOT: 
	op1 = I_OP1 (i);
	result = ~(DDword)clown.gpr[op1];
	cycles_all = do_move_to_regular ((Dword)result, op1);
	if (cycles_all != EFAIL)
	    do_flags (result);
	break;

    case CLI: 
	/* Only the inner ring may do this */
	if (clown.flags.bitwise.cpl > clown.flags.bitwise.iopl) {
	    bark (CPLVIOL);
	    raise_exception (PROTECTION_EX);
	    cycles_all = EFAIL;
	} else 
	    clown.flags.bitwise.inter = 0;
	break;

    case STI: 
	/* Only the inner ring may do this */
	if (clown.flags.bitwise.cpl > clown.flags.bitwise.iopl) {
	    bark (CPLVIOL);
	    raise_exception (PROTECTION_EX);
	    cycles_all = EFAIL;
	} else 
	    clown.flags.bitwise.inter = 1;
	break;

    case CHIO: 
	op1 = I_DSPL (i);
	/* Only the inner ring may do this */
	if (clown.flags.bitwise.cpl) {
	    bark (CPLVIOL);
	    raise_exception (PROTECTION_EX);
	    cycles_all = EFAIL;
	} else 
	    clown.flags.bitwise.iopl = op1;
	break;

    case CLC: 
	clown.flags.bitwise.carry = 0;
	break;
    case STC: 
	clown.flags.bitwise.carry = 1;
	break;

    case PUSHF:      
	cycles_all = do_push (clown.flags.raw, PUSHPOP);
	break;

    case POPF:
	cycles_all = do_pop (&datum, PUSHPOP);
	if (cycles_all != EFAIL) {
	    union Clown_Flags cf;
	    cf.raw = datum;
	    /* Only the inner ring may alter the CPL, IO, and INTER */
	    if (clown.flags.bitwise.cpl) {
		cf.bitwise.cpl   = clown.flags.bitwise.cpl;
		cf.bitwise.iopl  = clown.flags.bitwise.iopl;
		if (clown.flags.bitwise.cpl > clown.flags.bitwise.iopl)
		    cf.bitwise.inter = clown.flags.bitwise.inter;
	    }
	    clown.flags.raw = cf.raw;
	}
	break;

    case JC : 
	cycles_all = jumpOnCondition ( clown.flags.bitwise.carry, RELATIVE, I_DSPL (i));
	break;
    case JNC: 
	cycles_all = jumpOnCondition (!clown.flags.bitwise.carry, RELATIVE, I_DSPL (i));
	break;
    case JO : 
	cycles_all = jumpOnCondition ( clown.flags.bitwise.over,  RELATIVE, I_DSPL (i));
	break;
    case JNO: 
	cycles_all = jumpOnCondition (!clown.flags.bitwise.over,  RELATIVE, I_DSPL (i));
	break;
    case JZ : 
	cycles_all = jumpOnCondition ( clown.flags.bitwise.zero,  RELATIVE, I_DSPL (i));
	break;
    case JNZ: 
	cycles_all = jumpOnCondition (!clown.flags.bitwise.zero,  RELATIVE, I_DSPL (i));
	break;
    case JS : 
	cycles_all = jumpOnCondition ( clown.flags.bitwise.sign,  RELATIVE, I_DSPL (i));
	break;
    case JNS: 
	cycles_all = jumpOnCondition (!clown.flags.bitwise.sign,  RELATIVE, I_DSPL (i));
	break;

    case xNJMP:
	cycles_all = jumpOnCondition (1, ABSOLUTE, op3);
	break;

    case xFJMP:
	seg1 = clown.segr[_CODE].selector;
	seg2 = I_SEG (i);

	/* Try to load the target segment */
	cycles_all = clown_load_seg (seg2, _CODE);
	if (cycles_all != EFAIL) {
	    /* If success, try to jump */
	    cycles = jumpOnCondition (1, ABSOLUTE, op3);
	    if (cycles != EFAIL)
		cycles_all += cycles;
	    else 
		return EFAIL;
	} else
	    /* If not, restore the original code segment (must be VALID!) */
	    cycles_all += clown_load_seg (seg1, _CODE);
	break;

    case NCALLX:
	op1 = I_OP1 (i);
	op3 = clown.gpr[op1];
	/* That's right - no "break" here! */
    case xNCALL:
	cycles_all = do_push (clown.pc, PUSHPOP);
	if (cycles_all != EFAIL)
	    clown.pc = op3;
        break;

    case NRET:
	cycles_all = do_pop (&datum, PUSHPOP);
	if (cycles_all != EFAIL)
	    clown.pc = datum;
        break;

    case INRET:
	/* enable that kind of interrupts again */
	ilevel = clown.flags.bitwise.traplevel - 1;
	cycles_all = do_pop (&datum, PUSHPOP);
	if (cycles_all != EFAIL) {
	    Dword flags;
	    cycles = do_pop (&flags, PUSHPOP);
	    if (cycles != EFAIL) {
		cycles_all += cycles;
		clown.flags.raw = flags;
		clown.pc = datum;
		exception_status &= ~(0x01 << ilevel);
	    } else
		do_push (datum, PUSHPOP);
	}
        break;

    case xFCALL:
	seg2 = I_SEG (i);
	cycles_all = do_call (seg2, op3, clown.pc /* not a fault*/);
        break;

    case TRAP: 
	if (clown.flags.bitwise.inter)
	    cycles_all = raise_exception (TRAP_EX);
	break;

    case FRET:
	cycles_all = do_return (0);
        break;

    case IFRET:
	/* enable that kind of interrupts again */
	ilevel = clown.flags.bitwise.traplevel - 1;
	cycles_all = do_return (1);
	if (cycles_all != EFAIL)
	    exception_status &= ~(0x01 << ilevel);
        break;

    case IN :
	op1 = I_OP1 (i);
	op2 = I_DSPL (i);
	/* Only the inner rings may do this */
	if (clown.flags.bitwise.cpl > clown.flags.bitwise.iopl) {
	    bark (CPLVIOL);
	    raise_exception (PROTECTION_EX);
	    cycles_all = EFAIL;
	} else {
	    datum = read_from_port (op2);
	    cycles_all = do_move_to_regular (datum, op1);
	    if (cycles_all != EFAIL) {
		cycles_all++;	/* for IN */
		result = clown.gpr[op1];
		do_flags (result);
	    }
	}
	break;

    case OUT: 
	op1 = I_OP1 (i);
	op2 = I_DSPL (i);
	/* Only the inner rings may do this */
	if (clown.flags.bitwise.cpl > clown.flags.bitwise.iopl) {
	    bark (CPLVIOL);
	    raise_exception (PROTECTION_EX);
	    cycles_all = EFAIL;
	} else {
	    cycles_all = 1;	/* for OUT */
	    write_to_port (op2, clown.gpr[op1]);
	}
	break;

    case xOUTI: 
	op2 = I_DSPL (i);
	/* Only the inner rings may do this */
	if (clown.flags.bitwise.cpl > clown.flags.bitwise.iopl) {
	    bark (CPLVIOL);
	    raise_exception (PROTECTION_EX);
	    cycles_all = EFAIL;
	} else {
	    cycles_all = 1;	/* for OUT */
	    write_to_port (op2, op3);
	}
	break;

    case HLT:			/* tell the fetch unit to stop  */
	if (clown.flags.bitwise.cpl) {
	    bark (CPLVIOL);
	    raise_exception (PROTECTION_EX);
	    cycles_all = EFAIL;
	} else {
	    fetch = 0;			/* disable fetch unit */
	}
	break;

    case NOP: 			/* do not do anything */
	break;

    case STOP: 			/* stop */
	if (clown.flags.bitwise.cpl) {
	    bark (CPLVIOL);
	    raise_exception (PROTECTION_EX);
	    cycles_all = EFAIL;
	} else {
	    fetch = 0;
	    clown.pc--;
	    longjmp (begin_or_abort, 1);
	}
	break;

    default:
	raise_exception (INVALIDOPC_EX);
	cycles_all = EFAIL;
	break;
    }

    return cycles_all;
}
