#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "registers.h"
struct Clown_IODevice tty_device;
enum {POLLING, INTERRUPT} mode = POLLING;
#define SAMPLE_RATE 50

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


#ifdef SAMPLE_RATE
#define BUFFER_SIZE 16
static int counter = 0;
static int head = 0, tail = 0;
char buffer[BUFFER_SIZE];
#endif

void write_tty (Dword character)
{
    char c = (char)character;
    write (1, &c, 1);
}

void mode_tty (Dword character)
{
    switch (character) {
    case 0:
	mode = POLLING;
	break;
    default:
#ifdef SAMPLE_RATE
	counter = 0;
	mode = INTERRUPT;
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

Dword isready_tty ()
{
    if (mode == POLLING)
	return real_is_ready ();
    else {
#ifdef SAMPLE_RATE
	return head != tail;
#else
	assert (0);
#endif
    }
}

Dword read_tty ()
{
    if (isready_tty ()) {
	char c;
	if (mode == POLLING)
	    read (0, &c, 1);
	else {
#ifdef SAMPLE_RATE
	    c = buffer[tail];
	    tail = (tail + 1) % BUFFER_SIZE;
#else
	    assert (0);
#endif
	}
	return (Dword)c;
    } else 
	return 0;
}

Dword id_tty ()
{
    Dword d;
    char id[] = " tty";
    id[0] = DEVICE_ID;
    memcpy (&d, id, sizeof (Dword));
    return d;
}

#ifdef SAMPLE_RATE
void execute_tty (Bit dummy)
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
#endif

static struct Clown_IOPort ports[] =  {
{     id_tty, write_tty}, 
{   read_tty,  mode_tty}, 
{isready_tty,      NULL}
};
struct Clown_IODevice tty_device = {3, ports, 
#ifdef SAMPLE_RATE
execute_tty,
#else
NULL,
#endif
NULL,				/* initialize */
NULL				/* reset */
};
