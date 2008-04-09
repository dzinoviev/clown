#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "registers.h"

static Dword counter, interval = -1;
static enum {IDLE, READY, WORKING} state = IDLE;
struct Clown_IODevice timer_device;

void reset_timer (void)
{
    counter = 0;
    interval = -1;
    state = IDLE;
}

void write_timer (Dword datum)
{
    switch (datum) {
    case 0:			/* STOP and RESET */
	reset_timer ();
	break;
    case 1:			/* SET */
	state = READY;
	break;
    default:		   
	if (state == READY) {
	    if (datum > 0) {	/* START */
		counter = datum - 1;
		state = WORKING;
	    } else 		/* CHANGE INTERVAL */
		interval = -datum;
	}
	break;
    }
}

Dword read_timer (void)
{
    return counter;
}

Dword id_timer ()
{
    Dword d;
    char id[] = " timer";
    id[0] = DEVICE_ID;
    memcpy (&d, id, sizeof (Dword));
    return d;
}


Bit init_timer (void)
{
    reset_timer ();
    return 1;			/* it never fails */
}

void execute_timer (Bit dummy)
{
    if (state == WORKING && !(--counter)) {
	raise_exception (timer_device.IRQ);
	if (interval > 1)
	  counter = interval - 1;
	else
	  state = IDLE;
    }
}

static struct Clown_IOPort ports[] =  {
{id_timer, write_timer}, 
{read_timer, NULL}, 
};
struct Clown_IODevice timer_device = {2, ports, 
execute_timer, 
init_timer, 
reset_timer		    
};
