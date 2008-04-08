#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <signal.h>
#include <ctype.h>
#include <term.h>
#include <sys/ioctl.h>
#ifdef __APPLE__
#	include <limits.h>
#else
#	include <linux/limits.h>
#endif
#include <readln.h>

#include "config.h"
#include "registers.h"
#include "clowndev.h"

extern int cmdparse ();

struct cmd_buffer_state;
extern struct cmd_buffer_state *cmd_scan_string(char *);
extern void cmd_delete_buffer(struct cmd_buffer_state *);

jmp_buf begin_or_abort;
Bit silent = 0;
Bit initial_cpl = 0;

#ifdef __APPLE__

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
	fprintf (stderr, "%+011ld", val);
	break;
    case 'h':
    case 'H':
	fprintf (stderr, "%08lXh", val);
	break;
    case 'o':
    case 'O':
	fprintf (stderr, "%012loo", val);
	break;
    case 'a':			/* linear address */
    case 'A':
	fprintf (stderr, "DIR=%08lXh", L_DIR(val));
	fprintf (stderr, " PAGE=%08lXh", L_PAGE(val));
	fprintf (stderr, " OFFSET=%08lXh", L_OFFSET(val));
	break;
    case 'p':			/* page descriptor */
    case 'P':
	fprintf (stderr, "FRAME=%08lXh", PD_FRAME(val));
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
    fprintf (stderr, " %%OP3 = ");
    show_val (clown.op3, format);
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

static void show_cmd_stats (cycle_t cycles)
{
  fprintf (stderr, "FETCH UNIT %s. ", fetch ? "on" : "off");
  fprintf (stderr, "BUS CYCLES: %u. ", cycles);
  fprintf (stderr, "TIME = %lld. ", clown_time);
  show_pc ('d');
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
	    normal_terminal ();
	    signal (SIGINT, SIG_IGN);
	    
	    char *line = rl_gets ();
	    if (line) {
		struct cmd_buffer_state *my_string_buffer = cmd_scan_string (line);
		cmdparse ();
		cmd_delete_buffer (my_string_buffer);
	    }
	    else {
		fprintf(stderr, "\n");
		clown_done = 1;
	    }
	} else {
	    fprintf (stderr, "STOPPED\n");
	    show_cmd_stats (0);
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
		fprintf (stderr, "STOPPED\n");
		show_cmd_stats (0);
	    }
	}
    } else {
	instant_terminal ();
	go_interactive ();
    }

    return EXIT_SUCCESS;
}
