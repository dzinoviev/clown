#include <stdlib.h>
#include <stdio.h>
#include "registers.h"

static struct TlbEntry {
    Dword lin_addr;
    Dword phys_addr;
    Bit   access  : 1;		/* 0 - read, 1 - write */
    Bit   present : 1;
} *CLOWN_TLB = NULL;

static Dword tlb_miss = 0;
static Dword tlb_hit  = 0;
static Dword tlb_size = 0;

void clown_clear_TLB (void)
{
    int i;
    for (i = 0; i < tlb_size; i++)
	CLOWN_TLB[i].present = 0;
}

int clown_init_TLB (int size)
{
    if (size < 0) {
	fprintf (stderr, "TLB size must be non-negative: %d\n", size);
	return -1;
    }

    CLOWN_TLB = realloc (CLOWN_TLB, sizeof (struct TlbEntry) * size);
    if (!CLOWN_TLB) {
	perror ("TLB");
	return 0;
    }

    if (!silent)
	fprintf (stderr, "TLB with %d entries.\n", size);
    tlb_size = size;

    clown_clear_TLB ();

    return 1;
}

void insert_TLB (Dword lin_addr, Dword *phys_addr, Bit update)
{
    static int pointer = 0;
    int i;

    if (!tlb_size)
	return;

    /* Try to find an unused entry */
    for (i = 0; i < tlb_size; i++)
	if (!CLOWN_TLB[i].present) {
	    CLOWN_TLB[i].present = 1;
	    CLOWN_TLB[i].access = update;
	    CLOWN_TLB[i].lin_addr = lin_addr;
	    CLOWN_TLB[i].phys_addr = *phys_addr;
	    return;
	}

    /* purge the next entry and use it */
    CLOWN_TLB[pointer].access = update;
    CLOWN_TLB[pointer].lin_addr = lin_addr;
    CLOWN_TLB[pointer].phys_addr = *phys_addr;

    pointer = (pointer + 1) % CLOWN_TLB_SIZE;

    return;
}

Bit lookup_TLB (Dword lin_addr, Dword *phys_addr, Bit update)
{
    int i;

    if (!tlb_size)
	return 0;

    for (i = 0; i < CLOWN_TLB_SIZE; i++)
	if (CLOWN_TLB[i].present 
	    && CLOWN_TLB[i].access == update
	    && CLOWN_TLB[i].lin_addr == lin_addr) {
	    *phys_addr = CLOWN_TLB[i].phys_addr;
	    tlb_hit++;
	    return 1;
	}

    tlb_miss++;
    return 0;
}

