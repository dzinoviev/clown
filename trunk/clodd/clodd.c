#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#ifdef __APPLE__
#	include <limits.h>
#else
#	include <linux/limits.h>
#endif

#include "clodd.h"

Bit silent = 0;
int sector = 0;
int track = 0;
int size = 0;
int unpack = 0;
enum Clodd_Dir direction = UNDEF;
char *file = NULL;

int main (int argn, char *argv[])
{
  int infile, chunk, total;
  struct Clown_Hdd params;
  char disc_path[PATH_MAX] = "";
  char *path;
  Dword wbuffer[DISC_WORDS_PER_SECTOR];
  char cbuffer[DISC_WORDS_PER_SECTOR * sizeof (Dword)];

  read_options (argn, argv);
  if (!file) {
    fprintf (stderr, "%s: no file name given on the command line\n", argv[0]);
    exit (EXIT_FAILURE);      
  }
  if (direction == UNDEF) {
    fprintf (stderr, "%s: no copy direction on the command line\n", argv[0]);
    exit (EXIT_FAILURE);      
  }

  if ((path = getenv (DISC_HOME)))
    strcpy (disc_path, path);
  strcat (disc_path, DISC_IMAGE);
  disc_path[PATH_MAX - 1] = 0;

  if (!hdd_init (disc_path, &params, NULL)) {
    return EXIT_FAILURE;
  }

  if (track >= params.n_tracks) {
    fprintf (stderr, "%s: track too large: %d\n", argv[0], track);
    exit (EXIT_FAILURE);      
  }

  if (sector >= params.n_sectors) {
    fprintf (stderr, "%s: sector too large: %d\n", argv[0], sector);
    exit (EXIT_FAILURE);      
  }

  total = 0;
  if (direction == TODISK) {
    if (-1 == (infile = open (file, O_RDONLY))) {
      perror (file);
      exit (EXIT_FAILURE);
    }

    memset (wbuffer, 0, DISC_WORDS_PER_SECTOR * sizeof (Dword));
    while ((chunk = read (infile, cbuffer, DISC_WORDS_PER_SECTOR * sizeof (Dword)))) {
      if (-1 == chunk) {
	perror (file);
	close (infile);
	return EXIT_FAILURE;	
      }
      if (unpack) {
	int i;
	for (i = 0; i < DISC_WORDS_PER_SECTOR; i++) {
	  wbuffer[i] = (Dword)cbuffer[i];
	}
	hdd_write_sector (track, sector, wbuffer);      
      } else 
	hdd_write_sector (track, sector, (Dword*)cbuffer);      
      total += chunk;
      if (++sector == params.n_sectors) {
	sector = 0;
	if (++track == params.n_tracks) {
	  fprintf (stderr, "%s: file %s is too large: %d byte(s) copied\n", argv[0], file, total);
	  break;
	}
      }
    }

  } else {
    if (-1 == (infile = creat (file, S_IRUSR | S_IWUSR | S_IRGRP))) {
      perror (file);
      exit (EXIT_FAILURE);
    }

    while (!size || total < size) {
      if (unpack) {
	int i;
	hdd_read_sector (track, sector, wbuffer);
	for (i = 0; i < DISC_WORDS_PER_SECTOR; i++) {
	  cbuffer[i] = (char)wbuffer[i];
	}
      } else {
	hdd_read_sector (track, sector, (Dword*)cbuffer);
      }
      if (-1 == write (infile, cbuffer, DISC_WORDS_PER_SECTOR * sizeof (Dword))) {
	perror (file);
	close (infile);
	return EXIT_FAILURE;	
      }
      total++;
      if (++sector == params.n_sectors) {
	sector = 0;
	if (++track == params.n_tracks) {
	  break;
	}
      }
   }
    fprintf (stderr, "%s: %d block(s) copied into file %s\n", argv[0], total, file);
  }
  
  close (infile);
  hdd_close ();
  return EXIT_SUCCESS;
}
