#include <config.h>
#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif
#ifdef HAVE_LINUX_LIMITS_H
#include <linux/limits.h>
#endif

/*#define DEBUG */
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#include "hdd.h"

#define MIN(x,y) ((x)<(y)?(x):(y))
#define MAX(x,y) ((x)>(y)?(x):(y))

/* Disk image file handle and disk geometry */
static struct Clown_Hdd params;

/* The internal buffer of the controller */
static Bit running = 0;
static Dword        buffer[DISC_WORDS_PER_SECTOR];
static Dword hidden_buffer[DISC_WORDS_PER_SECTOR];
static Uword track, phi /* the current angle */, pointer, hidden_pointer;
static Uword dest_track, dest_phi;
static Dword direction;
static Uword counter, n_words, phi_counter, 
             delta_seek /* time to move from track to track */,
             delta_phi /* time to move from word to word */;

static enum {INTERRUPT, POLLING} mode = POLLING;
static enum {
    DSCOP_NONE = 0, 
    DSCOP_READ = 1, 
    DSCOP_READ_START = 2, 
    DSCOP_SEEK = 3, 
    DSCOP_SEEK_START = 4, 
    DSCOP_WRITE = 5,
    DSCOP_WRITE_START = 6,
    PREP_DSCOP_READ_START = 7, 
    PREP_DSCOP_SEEK_START = 8, 
    PREP_DSCOP_WRITE_START = 9
} operation = DSCOP_NONE;

struct Clown_IODevice hdd_device;


void reset_hdd (void)
{
    dest_track = 0;
    track = 0;
    dest_phi = 0;
    phi = 0;
    hidden_pointer = pointer = 0;
    phi_counter = 0;
    mode = POLLING;
    operation = DSCOP_NONE;
}

Bit init_hdd (void)
{
    char disc_path[PATH_MAX] = "";
    char *path = getenv (DISC_HOME);

    if (path)
      strcpy (disc_path, path);
    strcat (disc_path, DISC_IMAGE);
    disc_path[PATH_MAX - 1] = 0;

    if (!(running = hdd_init (disc_path, &params, &n_words)))
      return 0;
    
    reset_hdd ();
    delta_seek = (params.max_seek - params.t2t_seek) / (params.n_tracks - 2);
    delta_phi = params.rot_speed / n_words;
    if (delta_phi == 0)
	delta_phi = 1;		/* the disc must rotate, anyway! */
    return 1;
}

Dword read_hdd (void)
{
  if (!running)
    return 0;
  else {
    Dword datum = buffer[pointer++];
    if (pointer == DISC_WORDS_PER_SECTOR)
      pointer = 0;
    return datum;
  }
}

void write_hdd (Dword datum)
{
#ifdef DEBUG
  printf ("Write: %0x [%d]\n", datum, operation);
#endif
  if (running) {
    switch (operation) {
    case PREP_DSCOP_SEEK_START:	/* specify the destination track number */
      if (datum < 0 || (Uword)datum >= params.n_tracks || track == (Uword)datum) {
	operation = DSCOP_NONE;
	break;
      }
      dest_track = datum;
      direction = (dest_track > track) ? 1 : -1;
      counter = 0;
      operation = DSCOP_SEEK_START;
      break;
    case PREP_DSCOP_READ_START:
    case PREP_DSCOP_WRITE_START:
      operation = (operation == PREP_DSCOP_READ_START) ? DSCOP_READ_START
	: DSCOP_WRITE_START;
      datum = MAX (datum, 0);
      datum = datum % params.n_sectors;
      dest_phi = datum * (DISC_WORDS_PER_SECTOR + DISC_WORDS_PER_GAP);
      if (dest_phi == phi) {
	operation = (operation == DSCOP_READ_START) 
	  ? DSCOP_READ : DSCOP_WRITE;
	counter = 0;
	pointer = hidden_pointer = 0;
      }	
      break;
    default:			/* just write into the buffer */
      buffer[pointer++] = datum;
      if (pointer == DISC_WORDS_PER_SECTOR)
	pointer = 0;
      break;
    }
  }
}

Dword status_hdd (void)
{
    if (running) {
	Dword status;
	if (operation == DSCOP_NONE) {
	    if (pointer < hidden_pointer) {
		status = 1;		/* data available */
	    } else {
		status = 2;	/* idle */
	    }
	} else {
	    status = 0;		/* busy */
	}
#ifdef DEBUG
	printf ("Status: %d\n", status);
#endif
	return status;
    } else {
	return 2;
    }
}

Dword id_hdd ()
{
    if (running) {
	Dword d;
	char id[] = " hdd";
	id[0] = DEVICE_ID;
	memcpy (&d, id, sizeof (Dword));
	return d;
    } else {
	return 0;
    }
}

void control_hdd (Dword datum)
{
    if (running) {
#ifdef DEBUG
	printf ("Control: %ld\n", datum);
#endif
	switch (datum) {
	case 0:			/* SEEK TRACK */
	    operation = PREP_DSCOP_SEEK_START;
	    break;
	case 1:			/* READ SECTOR */
	    operation = PREP_DSCOP_READ_START;
	    break;
	case 2:			/* WRITE SECTOR */
	    operation = PREP_DSCOP_WRITE_START;
	    break;
	case 3:			/* RESET POINTER */
	    pointer = 0;
	    break;
	case 4:			/* INTERRUPT MODE */
	    mode = INTERRUPT;
	    break;
	case 5:			/* POLLING MODE */
	    mode = POLLING;
	    break;
	default:			/* do nothing */
	    break;
	}
    }
}

void execute_hdd (__attribute__((unused)) Bit dummy)
{
    if (!running)
	return;

    counter++;
    phi_counter++;

    /* The disc keeps rolling */
    if (phi_counter == delta_phi) {
	phi++;
	phi_counter = 0;
	if (phi == n_words)
	  phi = 0;
    }

    switch (operation) {
    case DSCOP_READ_START:	/* we found the sector */
    case DSCOP_WRITE_START:
	if (phi == dest_phi) {
	    counter = 0;
	    hidden_pointer = 1;

	    if (operation == DSCOP_READ_START) {
		operation = DSCOP_READ;
		/* THINK HOW TO FIX THIS! */
		hdd_read_sector (track, 
				 dest_phi / (DISC_WORDS_PER_SECTOR 
					     + DISC_WORDS_PER_GAP), 
				 hidden_buffer);
		buffer[0] = hidden_buffer[0];
	    } else {
		operation = DSCOP_WRITE;
		hidden_buffer[0] = buffer[0];
		/*		hdd_write_sector (track, 
				 dest_phi / (DISC_WORDS_PER_SECTOR 
					     + DISC_WORDS_PER_GAP), 
					     hidden_buffer);*/
	    }
	}
	break;

    case DSCOP_READ:	/* reading the words */
	if (counter == delta_phi) {
	    if (hidden_pointer == DISC_WORDS_PER_SECTOR) {
		if (mode == INTERRUPT)
		    raise_exception (hdd_device.IRQ);
		operation = DSCOP_NONE;
		pointer = 0;
		break;
	    }
	    buffer[hidden_pointer] = hidden_buffer[hidden_pointer];
	    hidden_pointer++;
	    counter = 0;
	}
	break;

    case DSCOP_WRITE:	/* writing the words */
	if (counter == delta_phi) {
	    if (hidden_pointer == DISC_WORDS_PER_SECTOR) {
		hdd_write_sector (track, dest_phi 
				  / (DISC_WORDS_PER_SECTOR 
				     + DISC_WORDS_PER_GAP), hidden_buffer);
		if (mode == INTERRUPT)
		    raise_exception (hdd_device.IRQ);
		operation = DSCOP_NONE;
		pointer = 0;
		break;
	    }
	    hidden_buffer[hidden_pointer] = buffer[hidden_pointer];
	    hidden_pointer++;
	    counter = 0;
	}
	break;

    case DSCOP_SEEK_START:
	if (counter == params.t2t_seek) {
	    track += direction;
	    if (track != dest_track) {
		operation = DSCOP_SEEK;
		counter = 0;
	    } else {
		if (mode == INTERRUPT)
		    raise_exception (hdd_device.IRQ);
		operation = DSCOP_NONE;
	    }
	}
	break;

    case DSCOP_SEEK:
	if (counter == delta_seek) {
	    track += direction;
	    if (track == dest_track) {
		if (mode == INTERRUPT)
		    raise_exception (hdd_device.IRQ);
		operation = DSCOP_NONE;
	    }
	    counter = 0;
	}
	break;

    default:
	break;
    }
}


static struct Clown_IOPort ports[] = {
  {id_hdd, control_hdd, 0}, 
  {status_hdd, NULL, 0}, 
  {read_hdd, write_hdd, 0}, 
};
struct Clown_IODevice hdd_device = {3, ports, 
  execute_hdd,
  init_hdd,
  reset_hdd, 0,0,0
};
