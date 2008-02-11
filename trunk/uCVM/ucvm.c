/*#define DEBUG*/
#include <stdio.h>

#include <assert.h>
#include <limits.h>
#ifndef __APPLE__
#	include <linux/limits.h>
#endif
#include "prototypes.h"
#include "ucvm.h"

void new_ucvm (struct UCVM *ucvm, const short *memory)
{
    int i;

    assert (memory);
    assert (ucvm);

    ucvm->pc = 0;
    ucvm->equal = 0;
    ucvm->memory = memory;
    for (i = 0; i < UCVM_NREGISTERS; i++)
	ucvm->r[i] = 0;
}

static Dword loadDword (const short *memory, short pc)
{
    return memory[pc + 1] + ((Uword)memory[pc] * USHRT_MAX);
}

/* is iorestr is set, I/O and memoiry instructions cannot be executed */
int ucvm_dostep (struct UCVM *ucvm, Bit iorestr)
{
    short ir;

    assert (ucvm);
    assert (ucvm->memory);

#ifdef DEBUG
    /*    usleep (100000);*/
#endif
    ir = ucvm->memory[ucvm->pc];
    
    switch (OPC (ir)) {
    case UCVM_END:
#ifdef DEBUG
	fprintf (stderr, "%d %s\n", ucvm->pc, "end");
#endif
	return 0;		/* all done! */

    case UCVM_IN:
	if (iorestr)
	    return 1;
#ifdef DEBUG
	fprintf (stderr, "%d %s\n", ucvm->pc,"in");
#endif
	ucvm->r[REG (ir)] = read_from_port (PORT (ir));
	break;

    case UCVM_JMP:
#ifdef DEBUG
	fprintf (stderr, "%d %s -> %d\n", ucvm->pc,"jmp", OFF (ir));
#endif
	ucvm->pc = OFF (ir);
	return 1;

    case UCVM_JEQ:  
#ifdef DEBUG
	fprintf (stderr, "%d %s -> %d\n", ucvm->pc,"jeq", OFF (ir));
#endif
	if (ucvm->equal) {
	    ucvm->pc = OFF (ir);
	    return 1;
	}
	break;

    case UCVM_LD:
	if (iorestr)
	    return 1;
#ifdef DEBUG
	fprintf (stderr, "%d %s\n", ucvm->pc,"ld");
#endif
	clown_read_mem (ucvm->r[MEM (ir)], &ucvm->r[REG (ir)]);
	break;

    case UCVM_ST:
	if (iorestr)
	    return 1;
#ifdef DEBUG
	fprintf (stderr, "%d %s\n", ucvm->pc,"st");
#endif
	clown_invalidate_cache (ucvm->r[MEM (ir)]);
	clown_write_mem (ucvm->r[MEM (ir)], ucvm->r[REG (ir)]);
	break;

    case UCVM_NOP:
#ifdef DEBUG
	fprintf (stderr, "%d %s\n", ucvm->pc,"nop");
#endif
	break;

    case UCVM_OUT:
	if (iorestr)
	    return 1;
#ifdef DEBUG
	fprintf (stderr, "%d %s %d->%d\n", ucvm->pc,"out",ucvm->r[REG (ir)],
		 PORT (ir));
#endif
	write_to_port (PORT (ir), ucvm->r[REG (ir)]);
	break;

    case UCVM_xADDI:
#ifdef DEBUG
	fprintf (stderr, "%d %s\n", ucvm->pc,"xaddi");
#endif
	ucvm->r[REG (ir)] += loadDword (ucvm->memory, ucvm->pc + 1);
	ucvm->equal = (ucvm->r[REG (ir)] == 0);
	ucvm->pc += 2;
	break;

    case UCVM_xCMPI:
#ifdef DEBUG
	fprintf (stderr, "%d %s\n", ucvm->pc,"xcmpi");
#endif
	ucvm->equal = (ucvm->r[REG (ir)] == loadDword (ucvm->memory, 
						       ucvm->pc + 1));
	ucvm->pc += 2;
	break;

    case UCVM_xMOVI:
#ifdef DEBUG
	fprintf (stderr, "%d %s\n", ucvm->pc,"xmovi");
#endif
	ucvm->r[REG (ir)] = loadDword (ucvm->memory, ucvm->pc + 1);
	ucvm->pc += 2;
	break;

    case UCVM_xOUTI:
	if (iorestr)
	    return 1;
#ifdef DEBUG
	fprintf (stderr, "%d %s %d->%d %d %d\n", ucvm->pc,"xouti",
		 loadDword (ucvm->memory, ucvm->pc + 1), PORT (ir),
		 ucvm->memory[ucvm->pc + 1], ucvm->memory[ucvm->pc + 2]);
#endif
	write_to_port (PORT (ir), loadDword (ucvm->memory, ucvm->pc + 1));
	ucvm->pc += 2;
	break;

    default:
	fprintf (stderr, "uCVM:invalid instruction: 0x%X\n", ir);
	break;
    }

    ucvm->pc++;
    return 1;
}
