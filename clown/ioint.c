#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>
#ifdef __APPLE__
#	include <limits.h>
#else
#	include <linux/limits.h>
#endif
#include <assert.h>
#include "registers.h"

struct Clown_ActivePeriphery clown_periphery[CLOWN_NPORTS];
Bit clown_active_count = 0;

struct Clown_IOPort clown_port[CLOWN_NPORTS];
Bit    Clown_IRQ[MAX_TRAP];

Dword read_from_port (Bit port_no)
{
    if (clown_port[port_no].used && clown_port[port_no].read_hook) {
	return clown_port[port_no].read_hook ();
    } else
	return 0;
}

void write_to_port (Bit port_no, Dword datum)
{
    if (clown_port[port_no].used && clown_port[port_no].write_hook)
	clown_port[port_no].write_hook (datum);
}

int load_device (struct Clown_IODevice *device, Bit first_port, Bit IRQ)
{
    Bit code;
    detach_device (device);
    code = attach_device (device, first_port, IRQ);

    if (!silent) {
	if (code) {
	    fprintf (stderr, "%3d registers, I/O base = %2d, IRQ = %2d.\n",
		     device->n_registers, device->first_port, device->IRQ);
	} else {
	    fprintf (stderr, "\tFailed.\n");
	}
    }
    return code;
}

int detach_device (struct Clown_IODevice *device)
{
    int i;

    if (!device)
	return 0;

    if (!device->attached)
	return 1;

    /* Check if the requested range of ports is still available */
    for (i = 0; i < device->n_registers; i++) {
	clown_port[device->first_port + i].used = 0;
	clown_port[device->first_port + i].write_hook = NULL;
	clown_port[device->first_port + i].read_hook = NULL;
    }

    Clown_IRQ[device->IRQ] = 0;

    if (device->reset || device->execute) {
	int i;
	for (i = 0; i < clown_active_count; i++)
	    if (clown_periphery[i].device == device) {
		int j;
		for (j = i + 1; j < clown_active_count; j++)
		    clown_periphery[j - 1] = clown_periphery[j];
	    }
		
	clown_active_count--;
    }

    device->attached = 0;
    return 1;
}

int attach_device (struct Clown_IODevice *device, Bit first_port, Bit IRQ)
{
    int i;

    if (!device)
	return 0;

    if (device->attached)
	return device->first_port == first_port;;

    /* Check if the requested range of ports is still available */
    for (i = 0; i < device->n_registers; i++)
	if (clown_port[first_port + i].used) 
	    return 0;
    
    if (Clown_IRQ[IRQ])
	return 0;
    else {
	Clown_IRQ[IRQ] = 1;
	device->IRQ = IRQ;
    }

    /* assign ports to registers */
    for (i = 0; i < device->n_registers; i++) {
	if (device->registers[i].read_hook) {
	    clown_port[first_port + i].read_hook 
		= device->registers[i].read_hook;
	    clown_port[first_port + i].used = 1;
	}		
	if (device->registers[i].write_hook) {
	    clown_port[first_port + i].write_hook 
		= device->registers[i].write_hook;
	    clown_port[first_port + i].used = 1;
	}		
    }

    device->first_port = first_port;

    if (device->reset || device->execute) {
	clown_periphery[clown_active_count].device = device;
	clown_active_count++;
    }

    /* Pass this function to the module to enable interrupts */
/*    device->raise = raise_exception;*/
    /* Pass these functions to the module to enable direct bus control */
/*    device->read = read_from_port;
    device->write = write_to_port;*/

    if (device->init && !device->init ()) {
	detach_device (device);	/* failed to initialize */
	return 0;
    } else {
	device->attached = 1;
	return 1;
    }
}

