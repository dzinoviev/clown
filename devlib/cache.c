#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include "registers.h"

#define CACHE_LINE_BITS 4
#define CACHE_LINE_SIZE (1<<CACHE_LINE_BITS)
static int cache_size = 0;
Uword exception_status;
Uword pending_exception;
Bit fetch;
Dword CLOWN_MEMORY[CLOWN_MEMORY_SIZE];

/* 
 * Simple direct mapped cache:
 *
 * *------*-------*----*----*----*------*----*
 * | TAG0 | DIRTY | W0 | W1 | W2 | .... | WN |
 * *------*-------*----*----*----*------*----*
 * | TAG1 | DIRTY | W0 | W1 | W2 | .... | WN |
 * *------*-------*----*----*----*------*----*
 * ...................................
 * *------*-------*----*----*----*------*----*
 * | TAGM | DIRTY | W0 | W1 | W2 | .... | WN |
 * *------*-------*----*----*----*------*----*
 */

typedef struct {
  Dword tag; 
  short dirty;
  /* line contents are simulated */
  /* Dword address [CACHE_LINE_SIZE]; */
} CacheLine;

static CacheLine *CLOWN_CACHE = NULL;

void clown_clear_cache (void)
{
  int i;
  for (i = 0; i < cache_size; i++) {
    CLOWN_CACHE[i].tag = -1;
    CLOWN_CACHE[i].dirty = 0;
  }
}

int clown_init_cache (unsigned nbits)
{
  unsigned size;
  if (nbits > sizeof (Dword) * CHAR_BIT - CACHE_LINE_BITS)
    nbits = sizeof (Dword) * CHAR_BIT - CACHE_LINE_BITS;

  size = 1 << nbits;		/* compute the cache size */

  CLOWN_CACHE = realloc (CLOWN_CACHE, sizeof (CacheLine) * size);
  if (!CLOWN_CACHE) {
    perror ("TLB");
    return 0;
  }

  if (!silent)
    fprintf (stderr, 
	     "Direct mapped cache with %d lines, %d words per line.\n", 
	     size, CACHE_LINE_SIZE);
  cache_size = size;

  clown_clear_cache ();

  return 1;
}

void clown_invalidate_cache (Dword phys_addr)
{
  if (cache_size) {
    int line;
    line = (phys_addr / CACHE_LINE_SIZE) % cache_size;
    CLOWN_CACHE[line].tag = -1;
  }
}

static unsigned touch_cache (Dword phys_addr, Bit update)
{
  cycle_t cycles;
  if (cache_size) {
    int /* offset, */ line, tag;
    /* Offset is simulated */
    /* offset = phys_addr % CACHE_LINE_SIZE;*/ 
    line = (phys_addr / CACHE_LINE_SIZE) % cache_size;
    tag = (phys_addr / CACHE_LINE_SIZE) / cache_size;

    if (CLOWN_CACHE[line].tag != tag) { /* cache miss */
      cycles = CACHE_LINE_SIZE;
      if (CLOWN_CACHE[line].dirty) {
	/* simulation only! */
	/* memcpy (&CLOWN_MEMORY[phys_addr - offset], 
	   CLOWN_CACHE[line].address,
	   sizeof (Dword) * CACHE_LINE_SIZE);*/
	CLOWN_CACHE[line].dirty = 0;
	cycles += CACHE_LINE_SIZE;
      }
      /* memcpy (CLOWN_CACHE[line].address, 
	 &CLOWN_MEMORY[phys_addr - offset],
	 sizeof (Dword) * CACHE_LINE_SIZE);*/
      CLOWN_CACHE[line].tag = tag;
    } else {		/* cache hit */
      cycles = 0;
    }

    CLOWN_CACHE[line].dirty = update;
  } else
    cycles = 1;
  return cycles;
}

void clown_read_mem (Dword phys_addr, Dword *datum)
{
  *datum = CLOWN_MEMORY[phys_addr];
}

void clown_write_mem (Dword phys_addr, Dword datum)
{
    CLOWN_MEMORY[phys_addr] = datum;
}

cycle_t clown_read_cache (Dword phys_addr, Dword *datum)
{
    cycle_t cycles = touch_cache (phys_addr, 0);
    clown_read_mem (phys_addr, datum);
    return cycles;
}

cycle_t clown_write_cache (Dword phys_addr, Dword datum)
{
    cycle_t cycles = touch_cache (phys_addr, 1);
    clown_write_mem (phys_addr, datum);
    return cycles;
}

static const struct {char *code, *name;} exception_names[] = {
  {"INV", "Invalid Opcode"},
  {"PAG", "Page Fault"},
  {"DIV", "Divide by Zero"},
  {"STK", "Stack Overflow"},
  {"SEG", "Segmentation Fault"},
  {"GPF", "General Protection Fault"},
  {"BUS", "Bus Error"},
  {"---", "RESERVED"},
  {"INT", "External Interrupt"},
  {"INT", "External Interrupt"},
  {"INT", "External Interrupt"},
  {"INT", "External Interrupt"},
  {"INT", "External Interrupt"},
  {"INT", "External Interrupt"},
  {"INT", "External Interrupt"},
  {"TRP", "Trap"},
};

const char *exception_code (enum Clown_Exception exc_no)
{
  return exception_names[exc_no].code;
}

static void report_exception (enum Clown_Exception exc_no)
{
  Dword new_pc = clown.pc;
  fprintf (stderr, "\n:::::: INTERRUPT #%d {%s}\n", exc_no, 
	   exception_names[exc_no].name);
  clown.pc = clown.old_pc;
  show_pc ('d');
  clown.pc = new_pc;
  show_ir ('H');
  if (exc_no == PAGEFAULT_EX)
    fprintf (stderr, "\tFAR   = 0x%08"PRIu32"x.\n", 
	     clown.FAR); 
  fprintf (stderr, "\n"); 
}

/* Exceptions are raised any time, but handled only at the end of a cycle */
cycle_t raise_exception (enum Clown_Exception exc_no) 
{
  if (exc_no < MAX_FAULT && (exception_status & (0x01 << exc_no))) {
    fprintf (stderr, "Double fault. Execution aborted.\n");
    longjmp (begin_or_abort, 1);
  }
  exception_status |= (0x01 << exc_no);
  pending_exception |= (0x01 << exc_no);

  if (!silent)
    report_exception (exc_no);

  /* enable fetch unit */
  fetch = 1;    

  return 0;
}
