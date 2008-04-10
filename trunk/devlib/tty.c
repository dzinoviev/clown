#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include "options.h"
#include "registers.h"

#ifdef TTY_PTHREAD
#ifdef TTY_POLL
#error Only one of TTY_POLL and TTY_PTHREAD can be defined
#endif
#include <pthread.h>
static pthread_t thread;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t poll = PTHREAD_MUTEX_INITIALIZER;

void my_lock (pthread_mutex_t *lock)
{
    int status = 1;
    if (pthread_mutex_lock (lock)) {
	perror ("pthread_mutex_lock() in tty");
	pthread_exit (&status);
    }
}

void my_unlock (pthread_mutex_t *lock) 
{
    int status = 1;
    if (pthread_mutex_unlock (lock)) {
	perror ("pthread_mutex_unlock() in tty");
	pthread_exit (&status);
    }
}
#endif

struct Clown_IODevice tty_device;
enum {POLLING, INTERRUPT} mode = POLLING;

/*
 * Simulation speed as a function of SAMPLE_RATE:
 * inf  4.84 sec/10M instr.
 * 1000 5.3
 *  200 5.3
 *  100 5.4
 *   50 5.4
 *   25 5.6
 *   10 5.9
 *    1 8.1
 */


#if (defined(TTY_POLL) || defined(TTY_PTHREAD))
#define BUFFER_SIZE 16
static int head = 0, tail = 0;
static char buffer[BUFFER_SIZE];
#endif

#ifdef TTY_POLL
#ifdef TTY_PTHREAD
#error Only one of TTY_POLL and TTY_PTHREAD can be defined
#endif
#define SAMPLE_RATE 50
static int counter = 0;
#endif

static void write_tty (Dword character)
{
    char c = (char)character;
    write (1, &c, 1);
}

static void mode_tty (Dword character)
{
    switch (character) {
    case 0:
	mode = POLLING;
	break;
    default:
#if (defined(TTY_POLL) || defined(TTY_PTHREAD))
	mode = INTERRUPT;
#ifdef TTY_PTHREAD
	my_unlock (&poll);
#else
	counter = 0;
#endif
#else
	fputs ("Support for interrupt mode is not enabled. "
	       "Staying in polling mode.\n", stderr);
#endif
	break;
    }
}

static Dword real_is_ready ()
{
    fd_set kbd;
    struct timeval zero = {0, 0};
    FD_ZERO (&kbd);
    FD_SET (0, &kbd);
    return 1 == select (1, &kbd, NULL, NULL, &zero);
}

static Dword isready_tty ()
{
    if (mode == POLLING)
	return real_is_ready ();
    else {
#if (defined(TTY_POLL) || defined(TTY_PTHREAD))
	return head != tail;
#else
	assert (0);
#endif
    }
}

static Dword read_tty ()
{
    if (isready_tty ()) {
	char c;
	if (mode == POLLING)
	    read (0, &c, 1);
	else {
#if (defined(TTY_POLL) || defined(TTY_PTHREAD))
#ifdef TTY_PTHREAD
	    my_lock (&lock);
#endif
	    c = buffer[tail];
	    tail = (tail + 1) % BUFFER_SIZE;
#ifdef TTY_PTHREAD
	    my_unlock (&lock);
#endif
#else
	    assert (0);
#endif
	}
	return (Dword)c;
    } else 
	return 0;
}

static Dword id_tty ()
{
    Dword d;
    char id[] = " tty";
    id[0] = DEVICE_ID;
    memcpy (&d, id, sizeof (Dword));
    return d;
}

#ifdef TTY_POLL
static void execute_tty (Bit dummy)
{
    if (mode == POLLING)
	return;
    if (counter == SAMPLE_RATE) {
	counter = 0;
	if (real_is_ready ()) {
	    read (0, &buffer[head], 1);
	    head = (head + 1) % BUFFER_SIZE;
	    if ((head + 1) % BUFFER_SIZE == tail)
		tail++;		/* buffer full */
	    raise_exception (tty_device.IRQ);
	}
    } else
	counter++;
}
#else
#define execute_tty NULL
#endif

#ifdef TTY_PTHREAD
static void *monitor (void* args)
{
    int fid;
    fd_set kbd;

    while (1) {
	FD_ZERO (&kbd);
	FD_SET (0, &kbd);
	fid = select (1, &kbd, NULL, NULL, NULL);

	if (mode == POLLING) {
	    my_lock (&poll);
	} else {
	    my_lock (&lock);
	    read (0, &buffer[head], 1);
	    head = (head + 1) % BUFFER_SIZE;
	    if ((head + 1) % BUFFER_SIZE == tail)
		tail++;		/* buffer full */
	    my_unlock (&lock);
	    
	    raise_exception (tty_device.IRQ);
	}
    }

    return NULL;
}

static Bit init_tty (void)
{
    if (pthread_create (&thread, NULL, monitor, NULL)) {
	perror ("pthread_create() in tty");
	return 0;
    }
    return 1;
}
#else
#define init_tty NULL
#endif

static struct Clown_IOPort ports[] =  {
    {     id_tty, write_tty}, 
    {   read_tty,  mode_tty}, 
    {isready_tty,      NULL}
};
struct Clown_IODevice tty_device = {3, ports, 
				    execute_tty, /* execute */
				    init_tty, /* initialize */
				    NULL /* reset */
};
