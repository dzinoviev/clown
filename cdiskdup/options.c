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
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "clodd.h"

#define CLODD_VERSION "CLOWN disc copy utility version "PACKAGE_VERSION" ["__DATE__"]"


static void show_version (void)
{
    fputs (CLODD_VERSION"\n", stderr);
}

static void show_copy (void)
{
    fputs (COPYRIGHT, stderr);
}

static void show_usage (char *me)
{
  fprintf (stderr, "Usage: %s [option ...] (-fromdisk|-todisk) FILE\n", me);
  fputs ("Options:\n", stderr);
  fputs ("  -h, --help\t\tshow this message and exit\n", stderr);
  fputs ("  -sector SEC\t\tread from or write to sector SEC (default 0)\n", stderr);
  fputs ("  -track TR\t\tread from or write to track TR (default 0)\n", stderr);
  fputs ("  -count CNT\t\tread CNT blocks (default: entire disk)\n", stderr);
  fputs ("  -unpack\t\trecord each 8-bit character as one CLOWN word\n", stderr);
  fputs ("\t\t\twhile writing to the disk,\n", stderr);
  fputs ("\t\t\trecord each CLOWN word as an 8-bit character\n", stderr);
  fputs ("\t\t\twhile reading from the disk\n", stderr);
  fputs ("  -fromdisk\t\tread from the disk into the FILE\n", stderr);
  fputs ("  -todisk\t\twrite from the FILE to the disk\n", stderr);
  fputs ("  -q, --silent\t\twork silently\n", stderr);
  fputs ("  -v, --version\t\tprint program version number and exit\n\n", stderr);
  fputs ("The disk image is in the file \""DISC_IMAGE"\".\n\n", stderr);
}


void read_options (int argc, char *argv[])
{
  int i, silent = 0;

  for (i = 1; i < argc; i++) {
    if (   ! (strcmp (argv[i], "-h"))
	   || ! (strcmp (argv[i], "--help"))) {
      show_usage (argv[0]);
      exit (EXIT_SUCCESS);
    }
      
    if (   !strcmp (argv[i], "-v")
	   || !strcmp (argv[i], "--version")) {
      show_version ();
      show_copy ();
      exit (EXIT_SUCCESS);
    }

    if (   !strcmp (argv[i], "-q")
	   || ! (strcmp (argv[i], "--silent"))) {
      silent = 1;
      continue;
    }
      
    if (!strcmp (argv[i], "-unpack")) {
      unpack = 1;
      continue;
    }
            
    if (!strcmp (argv[i], "-todisk")) {
      direction = TODISK;
      continue;
    }
      
    if (!strcmp (argv[i], "-fromdisk")) {
      direction = FROMDISK;
      continue;
    }
      
    if (  ! strcmp (argv[i], "-sector")) {
      char *endptr;
      i ++;
      if (i >= argc) {
	fprintf (stderr,
		 "%s: option requires one argument -- sector\n", 
		 argv[0]);
	exit (EXIT_FAILURE);
      }
      sector = strtoul (argv[i], &endptr, 0);
      if (*endptr) {
	fprintf (stderr, 
		 "%s: bad character(s) in sector number: %s\n", argv[0], endptr);
	exit (EXIT_FAILURE);
      }
      continue;
    }
      
    if (  ! strcmp (argv[i], "-track")) {
      char *endptr;
      i ++;
      if (i >= argc) {
	fprintf (stderr,
		 "%s: option requires one argument -- track\n", 
		 argv[0]);
	exit (EXIT_FAILURE);
      }
      track = strtoul(argv[i], &endptr, 0);
      if (*endptr) {
	fprintf (stderr, 
		 "%s: bad character(s) in sector number: %s\n", argv[0], endptr);
	exit (EXIT_FAILURE);
      }
      continue;
    }
      
    if (  ! strcmp (argv[i], "-count")) {
      char *endptr;
      i ++;
      if (i >= argc) {
	fprintf (stderr,
		 "%s: option requires one argument -- count\n", 
		 argv[0]);
	exit (EXIT_FAILURE);
      }
      size = strtol (argv[i], &endptr, 0);
      if (*endptr) {
	fprintf (stderr, 
		 "%s: bad character(s) in block count: %s\n", argv[0], endptr);
	exit (EXIT_FAILURE);
      }
      if (size <= 0) {
	fprintf (stderr, 
		 "%s: block count must be positive: %d\n", argv[0], size);
	exit (EXIT_FAILURE);
      }
      continue;
    }
      
    if (argv[i][0] == '-') {
      fprintf (stderr, "Unknown option: <%s>. Ignored.\n", argv[i]);
      continue;
    }

    if (!file) {
      file = argv[i];
    } else {
      fprintf (stderr, "%s: too many file names on the command line: %s\n", argv[0], argv[i]);
      exit (EXIT_FAILURE);
    }
  }

  if (!silent)
    show_version ();
}
