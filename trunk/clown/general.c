#include <stdio.h>
#include "options.h"
#include "registers.h"

#ifdef TTY_PTHREAD
#include <pthread.h>
void my_lock (pthread_mutex_t *lock);
void my_unlock (pthread_mutex_t *lock);
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
#endif

#include "exceptions.h"


DDword clown_time;
Bit fetch;
Uword exception_status;
Uword pending_exception;

#define INVALID (Uword)(~(Uword)0)

/* Initialize the CPU */
void clown_init (void)
{
    int i;

    if (!silent) {
	fprintf (stderr, "\t%3d general-purpose registers.\n", CLOWN_NGPR);
	fprintf (stderr, "\t%3d segment registers.\n", CLOWN_NSEGR);
    }
	
    /* Initialize the ports */
    if (!silent)
	fprintf (stderr, "\t%3d I/O ports.\n", CLOWN_NPORTS);
    for (i = 0; i < CLOWN_NPORTS; i++)
	clown_port[i].used = 0;

    /* Initialize IRQs */
    if (!silent)
	fprintf (stderr, "\t%3d IRQs (%d available).\n", 
		 MAX_TRAP, MAX_FAULT - 1);
    for (i = 0; i < MAX_FAULT; i++)
	Clown_IRQ[i] = 1;
    for (i = MAX_FAULT + 1; i < MAX_TRAP; i++)
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
    int i;

    for (i = 0; i < CLOWN_NGPR; i++)
	clown.gpr[i] = 0;

    for (i = 0; i < CLOWN_NSEGR; i++) {
	clown.segr[i].descriptor.base = 0;
	clown.segr[i].descriptor.limit = CLOWN_MEMORY_SIZE - 1;
	clown.segr[i].descriptor.flags = 0;
    }
    clown.flags.raw = 0;
    clown.flags.bitwise.traplevel = MAX_TRAP; /* not in an ISR */
    clown.flags.bitwise.inter = 1; /* enable interrupts */
    clown.pc = 0;

    /* enable the stack, code, and data segments */
    /* IS THIS PART REALLY USEFUL? */
    clown.SP = CLOWN_MEMORY_SIZE - 1;
    clown.STACK.flags |= (CAN_READ | CAN_WRITE) << 4;
    clown.DATA.flags  |= (CAN_READ | CAN_WRITE) << 4;
    clown.CODE.flags  |= (CAN_READ | CAN_EXEC ) << 4;

    /* Clear the TLB and the cache */
    clown_clear_TLB ();
    clown_clear_cache ();

    /* Clean up the vector of pending exceptions */
    exception_status = 0;
    pending_exception = 0;

    /* enable fetch unit */
    fetch = 1;    
}

const static struct {char *code, *name;} exception_names[] = {
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
    fprintf (stderr, "Interrupt #%2d [%-24s]\n", exc_no, 
	     exception_names[exc_no].name);
    fprintf (stderr, "\tCS:PC = %012ld:%012ld.\n", 
	     clown.CODE.base, clown.old_pc); 
    fprintf (stderr, "\tIR    = 0x%08lx.\n", 
	     clown.ir); 
    if (exc_no == PAGEFAULT_EX)
	fprintf (stderr, "\tFAR   = 0x%08lx.\n", 
		 clown.FAR); 
}

/* Handle the highest-priority interrupt */
cycle_t handle_exception (void) 
{
    cycle_t cycles, cycles_all = 0;
    /* Check if interrupts are enabled */
    int i, ilimit = clown.flags.bitwise.inter ? MAX_TRAP : MAX_FAULT;

    /* Are we already in an ISR? */
    if (ilimit > clown.flags.bitwise.traplevel)
	ilimit = clown.flags.bitwise.traplevel;

    for (i = 0; i < ilimit; i++)
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

/* Exceptions are raised any time, but handled only at the end of a cycle */
cycle_t raise_exception (enum Clown_Exception exc_no) 
{
    if (exc_no < MAX_FAULT && (exception_status & (0x01 << exc_no))) {
	fprintf (stderr, "Double fault. Execution aborted.\n");
	longjmp (begin_or_abort, 1);
    }
#ifdef TTY_PTHREAD
    my_lock (&lock);
#endif
    exception_status |= (0x01 << exc_no);
    pending_exception |= (0x01 << exc_no);
#ifdef TTY_PTHREAD
    my_unlock (&lock);
#endif

    if (!silent)
	report_exception (exc_no);

    /* enable fetch unit */
    fetch = 1;    

    return 0;
}
    
