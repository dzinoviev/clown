#include <config.h>
#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif
#ifdef HAVE_LINUX_LIMITS_H
#include <linux/limits.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <term.h>
#include <sys/ioctl.h>
#include <readln.h>

#include "hardware.h"
#include "registers.h"
#include "clowndev.h"

extern int cmdparse ();

struct cmd_buffer_state;
extern struct cmd_buffer_state *cmd_scan_string(char *);
extern void cmd_delete_buffer(struct cmd_buffer_state *);

Bit silent = 0;
Bit initial_cpl = 0;

#ifdef HAVE_STRUCT_TERMIO
	
static struct termio gettty;
static void instant_terminal ()
{
    struct termio settty;
    ioctl(0, TCGETA, &gettty);
	
    settty = gettty;
    settty.c_lflag &= ~ICANON;	/* do not buffer stdin */
    settty.c_lflag &= ~ECHO;	/* do not echo the symbol  */
    if (-1 == ioctl(0, TCSETAF, &settty)) {
	perror ("terminal");
	fputs ("Terminal behavior may be unpredictable\n", stderr);
    }
}

static void normal_terminal ()
{
    if (-1 == ioctl(0, TCSETAF, &gettty)) {
	perror ("terminal");
	fputs ("Terminal behavior may be unpredictable\n", stderr);
    }
}

#else
#ifdef HAVE_STRUCT_TERMIOS
static struct termios gettty;
static void instant_terminal ()
{
    struct termios settty;
    tcgetattr(0, &gettty);
	
    settty = gettty;
    settty.c_lflag &= ~ICANON;	/* do not buffer stdin */
    settty.c_lflag &= ~ECHO;	/* do not echo the symbol  */
    if (-1 == tcsetattr(0, TCSANOW, &settty)) {
	perror ("terminal");
	fputs ("Terminal behavior may be unpredictable\n", stderr);
    }
}

static void normal_terminal ()
{
    if (-1 == tcsetattr(0, TCSANOW, &gettty)) {
	perror ("terminal");
	fputs ("Terminal behavior may be unpredictable\n", stderr);
    }
}

#else
#error struct termio or struct termios required
#endif
#endif

cycle_t do_one_step ()
{
    cycle_t cycles_all = 0, cycles;
    int i;

    if (fetch) {		/* fetch unit enabled  */
	clown.old_pc = clown.pc;
	
	cycles = clown_read_linear (clown.CODE.base + clown.pc, (Dword*)&clown.ir);
	if (cycles == EFAIL) {
	    cycles_all = EFAIL;
	    goto pending;
	}
	cycles_all += cycles;
	
	if (NEEDS_EXTENSION (I_OPC (clown.ir))) {
	    clown.pc++;
	    cycles = clown_read_linear (clown.CODE.base + clown.pc, 
					(Dword*)&clown.op3);
	    if (cycles == EFAIL) {
		cycles_all = EFAIL;
		goto pending;
	    }
	    cycles_all += cycles;
	} else
	    clown.op3 = 0;
	
	clown.pc++;
	cycles = clown_decode_execute (clown.ir, clown.op3);
	if (cycles != EFAIL)
	    cycles_all += cycles;
    }

    /* Periphery devices work regardless of the state of the fetch unit */
    for (i = 0; i < clown_active_count; i++)
	if (clown_periphery[i].device->execute)
	    clown_periphery[i].device->execute (cycles_all == 0);

    /* All exceptions are processed at the end of the cycle  */
    if (pending_exception) {
    pending:
	cycles = handle_exception ();
	if (cycles != EFAIL)
	    cycles_all += cycles;
    }
    clown_time++;

    /*
     * The value of variable cycles_all is the number of memory requests 
     * (both read and write) made during the fetch, decoding, and execution
     * of the current instruction. We assume that if it is equal to zero,
     * then the bus is available for DMA transfers.
     */

    return cycles_all;
}

int read_options (int argc, char *argv[]);
extern struct Clown_IODevice timer_device;
extern struct Clown_IODevice tty_device;
extern struct Clown_IODevice dma_device;
extern struct Clown_IODevice hdd_device;

static int suspend = 0;

static void suspend_clown (int dummy)
{
    suspend = 1;
    fputs ("Execution suspended. Type 'run' to resume\n", stderr);
}

void do_run (int count)
{
    int iteration = count ? count : -1;
    cycle_t cycles = 0;
    instant_terminal (); 
    suspend = 0;
    signal (SIGINT, suspend_clown);
    while (!suspend && iteration) {
	cycles = do_one_step ();
	if (count)
	    iteration--;
    }
    show_cmd_stats (cycles);
}

void do_step ()
{
  cycle_t cycles;
  instant_terminal (); 
  cycles = do_one_step ();
  show_cmd_stats (cycles);
}

void do_reset ()
{
    clown_init ();
    clown_init_TLB (CLOWN_TLB_SIZE);
    clown_init_cache (CLOWN_CACHE_BITS);

    if (!silent) fprintf (stderr, "timer:\t");
    load_device (&timer_device, IOBASE_TIMER, IRQ_TIMER);
    if (!silent) fprintf (stderr, "tty:\t");
    load_device (&tty_device, IOBASE_TTY, IRQ_TTY);
    if (!silent) fprintf (stderr, "dma:\t");
    load_device (&dma_device,   IOBASE_DMA, IRQ_DMA);
    if (!silent) fprintf (stderr, "hdd:\t");
    load_device (&hdd_device,   IOBASE_HDD, IRQ_HDD);
}

static int clown_done = 0;
void do_quit ()
{
  clown_done = 1;
}

static void go_interactive ()
{
    while (!clown_done) {
	if (!setjmp (begin_or_abort)) {
	    char *line;
	    normal_terminal ();
	    signal (SIGINT, SIG_IGN);
	    
	    if ((line = rl_gets ())) {
		struct cmd_buffer_state *my_string_buffer = cmd_scan_string (line);
		cmdparse ();
		cmd_delete_buffer (my_string_buffer);
	    }
	    else {
		fprintf(stderr, "\n");
		clown_done = 1;
	    }
	} else {
	    if (!silent) {
		fprintf (stderr, ":::::: STOPPED\n");
		show_cmd_stats (0);
	    }
	}
    }
}

int main (int argc, char *argv[])
{
    int mode;
/*    atexit (normal_terminal); */

    if (setjmp (failure))
      return EXIT_FAILURE;

    mode = read_options (argc, argv);
    if (!mode)
      return EXIT_FAILURE;

    atexit (normal_terminal);
    do_reset ();
	
    if (!silent)
	fprintf (stderr, "\n");

    if (mode == -1) {		/* non-interactive */
	if (!setjmp (begin_or_abort)) {
	    do_run (0);
	    /* This happens only if the RUN is interrupted with ^C */
	    go_interactive ();
	} else {
	    if (!silent) {
		fprintf (stderr, ":::::: STOPPED\n");
		show_cmd_stats (0);
	    }
	}
    } else {
	instant_terminal ();
	go_interactive ();
    }

    return EXIT_SUCCESS;
}
