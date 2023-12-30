#include <config.h>
#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif
#ifdef HAVE_LINUX_LIMITS_H
#include <linux/limits.h>
#endif

#include <config.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "clodd.h"

int listing = 0;
Bit silent = 0;
int sector = 0;
int track = 0;
int size = 0;
int unpack = 0;
enum Clodd_Dir direction = UNDEF;
char *file = NULL;
Clof_Type module_type = CLOF_UNKNOWN;

void component_error (const char *name, const char *msg, char *detail)
{
    if (!silent)
        fprintf (stderr, "clodd: %s: %s: %s\n", name, msg, detail);
}

static void validate_options(int argn, char *argv[])
{
  read_options(argn, argv);
  if (!file) {
    fprintf(stderr, "%s: no file name given on the command line\n", argv[0]);
    exit(EXIT_FAILURE);      
  }
  if (direction == UNDEF) {
    fprintf(stderr, "%s: no copy direction on the command line\n", argv[0]);
    exit(EXIT_FAILURE);      
  }
}

static void construct_disc_path(char *disc_path, size_t max_length)
{
  char *path = getenv(DISC_HOME);
  if (path) {
    strncpy(disc_path, path, max_length - strlen(DISC_IMAGE) - 1);
  } else {
    disc_path[0] = '\0';
  }
  strncat(disc_path, DISC_IMAGE, max_length - strlen(disc_path) - 1);
}

static void validate_track_sector(int track, int max_tracks,
				  int max_sectors, char *prog_name) {
  if (track >= max_tracks) {
    fprintf(stderr, "%s: track too large: %d\n", prog_name, track);
    exit(EXIT_FAILURE);      
  }
  if (sector >= max_sectors) {
    fprintf(stderr, "%s: sector too large: %d\n", prog_name, sector);
    exit(EXIT_FAILURE);      
  }
}

int main (int argn, char *argv[])
{
  int infile, chunk;
  struct Clown_Hdd params;
  char disc_path[PATH_MAX] = "";
  char *path;
  Dword wbuffer[DISC_WORDS_PER_SECTOR];
  char cbuffer[DISC_WORDS_PER_SECTOR * sizeof (Dword)];

  validate_options(argn, argv);
  construct_disc_path(disc_path, PATH_MAX);
 
  if (!hdd_init (disc_path, &params, NULL)) {
    return EXIT_FAILURE;
  }
  
  validate_track_sector(track, params.n_tracks, params.n_sectors, argv[0]);

  int total = 0;
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
	for (int i = 0; i < DISC_WORDS_PER_SECTOR; i++) {
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
	hdd_read_sector (track, sector, wbuffer);
	for (int i = 0; i < DISC_WORDS_PER_SECTOR; i++) {
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
