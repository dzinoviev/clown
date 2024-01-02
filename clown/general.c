#include <config.h>

#include <stdio.h>

#include "registers.h"
#include "exceptions.h"

#define INVALID (Uword)(~(Uword)0)

/* Initialize the CPU */
void clown_init (void)
{
  if (!silent) {
    fprintf (stderr, "\t%3d general-purpose registers.\n", CLOWN_NGPR);
    fprintf (stderr, "\t%3d segment registers.\n", CLOWN_NSEGR);
  }
	
  /* Initialize the ports */
  if (!silent)
    fprintf (stderr, "\t%3d I/O ports.\n", CLOWN_NPORTS);
  for (int i = 0; i < CLOWN_NPORTS; i++)
    clown_port[i].used = 0;

  /* Initialize IRQs */
  if (!silent)
    fprintf (stderr, "\t%3d IRQs (%d available).\n", 
	     MAX_TRAP, MAX_FAULT - 1);
  for (int i = 0; i < MAX_FAULT; i++)
    Clown_IRQ[i] = 1;
  for (int i = MAX_FAULT + 1; i < MAX_TRAP; i++)
    Clown_IRQ[i] = 0;
  Clown_IRQ[TRAP_EX] = 1;		/* TRAP */

  clown_time = 0;

  clown_reset ();
  if (!silent)
    fprintf (stderr, "\n");

  clown.flags.bitwise.cpl = initial_cpl;
}

/* Restart the CPU */
void clown_reset (void)
{

  for (int i = 0; i < CLOWN_NGPR; i++)
    clown.gpr[i] = 0;

  for (int i = 0; i < CLOWN_NSEGR; i++) {
    clown.segr[i].descriptor.base = 0;
    clown.segr[i].descriptor.limit = CLOWN_MEMORY_SIZE - 1;
    clown.segr[i].descriptor.flags = 0;
    SF_SET_PRESENT (clown.segr[i].descriptor);
  }

  if (SF_PRESENT (init_ldt_descr))
    clown.segr[_LDT].descriptor = init_ldt_descr;
  if (-1 != firstcode)
    clown_load_seg (MK_SELECTOR (firstcode, 0, _LDT), _CODE);

  clown.flags.raw = 0;
  clown.flags.bitwise.traplevel = MAX_TRAP; /* not in an ISR */
  clown.flags.bitwise.inter = 1; /* enable interrupts */
  clown.pc = 0;

  /* enable the stack, code, and data segments */
  clown.SP = CLOWN_MEMORY_SIZE - 1;
  SF_SET_PERM (clown.STACK, (CAN_READ | CAN_WRITE));
  SF_SET_PERM (clown.DATA, (CAN_READ | CAN_WRITE));
  SF_SET_PERM (clown.CODE, (CAN_READ | CAN_EXEC));

  /* Clear the TLB and the cache */
  clown_clear_TLB ();
  clown_clear_cache ();

  /* Clean up the vector of pending exceptions */
  exception_status = 0;
  pending_exception = 0;

  /* enable fetch unit */
  fetch = 1;    
}

/* Handle the highest-priority interrupt */
cycle_t handle_exception (void) 
{
  cycle_t cycles, cycles_all = 0;
  /* Check if interrupts are enabled */
  int ilimit = clown.flags.bitwise.inter ? MAX_TRAP : MAX_FAULT;

  /* Are we already in an ISR? */
  if (ilimit > clown.flags.bitwise.traplevel)
    ilimit = clown.flags.bitwise.traplevel;

  for (int i = 0; i < ilimit; i++)
    if (exception_status & (0x01 << i)) {
      Dword save_pc;
      Uword isrh;

      pending_exception &= ~(0x01 << i);

      /* Is it a trap or a fault? */
      save_pc = (i >= MAX_FAULT) ? clown.pc : clown.old_pc;
	    
      if ((cycles = do_push (clown.flags.raw, PUSHPOP)) == EFAIL) 
	return EFAIL;
      cycles_all += cycles;

      if ((cycles = clown_read_linear (clown.ISR.base + i, (Dword*)&isrh) 
	   == EFAIL))
	return EFAIL;
      cycles_all += cycles;
	    
      if (!SEL_REAL (isrh)) {	
	Selector isr_segment = (Selector)(isrh & 0x0000FFFF);
	if ((cycles = do_call (isr_segment /* target selector */, 
			       0 /* the new PC*/, 
			       save_pc)) 
	    == EFAIL) {
	  Dword datum;
	  cycles_all += do_pop (&datum, PUSHPOP);	/* remove the flags */
	  return cycles_all;
	}
	cycles_all += cycles;
      } else {	/* direct address */
	Uword prot = (isrh >> 1) & 0x3;
	Uword dest = isrh & 0xFFFFFFF8;
		
	cycles = do_push (save_pc, PUSHPOP);
	if (cycles != EFAIL) {
	  cycles_all += cycles;
	  clown.pc = dest;
	  clown.flags.bitwise.cpl = prot;
	} else {	/* pop the flags */
	  Dword datum;
	  cycles_all += do_pop (&datum, PUSHPOP);	/* remove the flags */
	  return cycles_all;
	}
      }
	    
      /* disable interrupts */
      clown.flags.bitwise.traplevel = i + 1;
      clown.flags.bitwise.inter = 0; 

      break;
    }

  return cycles_all;
}


    
