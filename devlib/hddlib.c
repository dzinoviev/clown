#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include "hdd.h"
#include "registers.h"

#define BUFFER_SIZE (sizeof (Dword) * DISC_WORDS_PER_SECTOR)

static int clown_disk = -1;
static Uword n_words;

Bit hdd_init (char disc_path[], struct Clown_Hdd *params, Uword *track_len)
{
    if (-1 != clown_disk) {
/*	fputs ("Clown HDD already initialized\n", stderr);*/
	return 1;
    }

    clown_disk = open (disc_path, O_RDWR);
    if (-1 == clown_disk) {
	if (!silent) {
	    fputs ("\n\tI was not able to initialize the hard disk image:\n\t", 
		stderr);
	    perror (disc_path);
	    fprintf (stderr,
			"\tPerhaps you forgot to set the "DISC_HOME" shell variable\n"
			"\tto tell me the path to the disk image file?\n"
			"\tMeanwhile, CLOWN will work without a hard drive.\n"
			"\tThere is nothing wrong with that:\n"
			"\tyour iPod has no hard drive, either!\n\n");
	}
	return 0;
    }

    if (sizeof (struct Clown_Hdd) 
	!= read (clown_disk, params,  sizeof (struct Clown_Hdd))) {
      perror (disc_path);
	close (clown_disk);
	return 0;
    }

    if (track_len) {
      n_words = params->n_sectors * (DISC_WORDS_PER_SECTOR + DISC_WORDS_PER_GAP);
      *track_len = n_words;
    }
    return 1;
}

Bit hdd_read_sector (Dword track, Dword sector, Dword *hidden_buffer) 
{
  if (-1 == clown_disk) {
      if (!silent)
	  fputs ("Clown HDD not initialized\n", stderr);
    return 0;
  }

  if (-1 == lseek (clown_disk, (n_words * track 
				+ sector * (DISC_WORDS_PER_SECTOR 
					      + DISC_WORDS_PER_GAP)) 
		   * sizeof (Uword) + sizeof (struct Clown_Hdd), 
		   SEEK_SET)) {
    perror (DISC_IMAGE);
    return 0;
  }

  if (BUFFER_SIZE != read (clown_disk, hidden_buffer, BUFFER_SIZE)) {
    perror (DISC_IMAGE);
    return 0;
  }
  
  return 1;
}

Bit hdd_write_sector (Dword track, Dword sector, Dword *hidden_buffer) 
{
  if (-1 == clown_disk) {
      if (!silent)
	  fputs ("Clown HDD not initialized\n", stderr);
    return 0;
  }

  if (-1 == lseek (clown_disk, (n_words * track 
				+ sector * (DISC_WORDS_PER_SECTOR 
					      + DISC_WORDS_PER_GAP)) 
		   * sizeof (Uword) + sizeof (struct Clown_Hdd), 
		   SEEK_SET)) {
    perror (DISC_IMAGE);
    return 0;
  }

  if (BUFFER_SIZE != write (clown_disk, hidden_buffer, BUFFER_SIZE)) {
    perror (DISC_IMAGE);
    return 0;
  }
  
  return 1;
}

Bit hdd_close ()
{
    if (-1 == clown_disk) {
	if (!silent)
	    fputs ("Clown HDD not initialized\n", stderr);
	return 0;
    }
    close (clown_disk);
    clown_disk = -1;
    return 1;
}
