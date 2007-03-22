#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "cas.h"

#define DEFAULT_OBJECT "a.out"
#define VERSION "CLOWN assembler version 1.00 ["__DATE__"]"

static void show_version (void)
{
    fputs (VERSION"\n", stderr);
}

static void show_copy (void)
{
    fputs ("Copyright 2004 D. Zinoviev\n"
	   "This program is free software; you may redistribute it under the terms of\n"
	   "the GNU General Public License.  This program has absolutely no warranty.\n", 
	   stderr);
}

static void show_usage (char *name)
{
    assert (name);

    fprintf (stderr, "USAGE: %s [option...] IFILE.s\n", name);
    fputs ("Options:\n", stderr);
    fputs ("  -h, --help               show this message and exit\n", stderr);
    fputs ("  -P, --nocpp              do not preprocess\n", stderr);
    fputs ("  --pass-to-cpp \"OPTIONS\"  pass OPTIONS to preprocessor\n", stderr);
    fputs ("      option -I($CLOWN/include/) is passed implicitly\n", stderr);
    fputs ("  -b, --bin                generate a BIN module (default)\n", stderr);
    fputs ("  -x, --exe                generate an EXE module\n", stderr);
    fprintf (stderr, "  -e ADDR, --entry ADDR    set start address (default %ld)\n", offset);
    fputs ("  -o OFILE                 set output file name (default IFILE.bin)\n", stderr);
    fputs ("  -v, --version            print assembler version number and exit\n", stderr);
    fputs ("  -l, --listing            produce listings\n", stderr);
    fputs ("  -V                       print assembler version number\n", stderr);
    fputs ("  --                       treat the next argument as a file name\n", stderr);
}

int get_options (int argc, char *argv[], char **object,
		 char source[], int *ecode)
{
    int i;

    assert (argv);
    assert (object);
    assert (ecode);

    if (argc < 2) {
	show_usage (argv[0]);
	*ecode = EXIT_SUCCESS;
	return 0;
    }
    
    for (i = 1; i < argc; i++) {
	if (!strcmp (argv[i], "-V")) {
	    show_version ();
	    continue;
	}

	if (!strcmp (argv[i], "--version")) {
	    show_version ();
	    show_copy ();
	    *ecode = EXIT_SUCCESS;
	    return 0;
	}

	if (   !strcmp (argv[i], "--help")
	    || !strcmp (argv[i], "-h")) {
	    show_usage (argv[0]);
	    *ecode = EXIT_SUCCESS;
	    return 0;
	}

	if (   !strcmp (argv[i], "--nocpp")
	    || !strcmp (argv[i], "-P")) {
	    cpp_options = NULL;
	    continue;
	}

	if (!strcmp (argv[i], "--pass-to-cpp")) {
	    if (i == argc - 1) {
		fprintf (stderr,
			 "%s: option requires an argument -- pass-to-cpp\n", 
			 argv[0]);
		*ecode = EXIT_FAILURE;
		return 0;
	    } 
	    i++;
	    cpp_options = argv[i];
	    continue;
	}

	if (!strcmp (argv[i], "--entry") ||
	    !strcmp (argv[i], "-e")) {
	    char *endptr;
	    if (i == argc - 1) {
		fprintf (stderr,
			 "%s: option requires an argument -- entry\n", 
			 argv[0]);
		*ecode = EXIT_FAILURE;
		return 0;
	    } 
	    i++;
	    offset = strtol (argv[i], &endptr, 0);
	    if (*endptr) {
		fprintf (stderr, 
			 "%s: bad characters in entry address: %s; "
			 "assuming entry = 0\n", argv[0], endptr);
		offset = 0;
	    }
	    continue;
	}

	if (   !strcmp (argv[i], "--bin")
	    || !strcmp (argv[i], "-b")) {
	    if (module_type != CLOF_UNKNOWN) {
		fprintf (stderr,
			 "%s: module type cannot be redefined\n",argv[0]);
	    } else
		module_type = CLOF_BIN;
	    continue;
	}

	if (   !strcmp (argv[i], "--exe")
	    || !strcmp (argv[i], "-x")) {
	    if (module_type != CLOF_UNKNOWN) {
		fprintf (stderr,
			 "%s: module type cannot be redefined\n",argv[0]);
	    } else
		module_type = CLOF_EXE;
	    continue;
	}

	if (   !strcmp (argv[i], "--listing")
	    || !strcmp (argv[i], "-l")) {
	    listing = 1;
	    continue;
	}

	if (!strcmp (argv[i], "-o")) {
	    if (*object) {
		fprintf (stderr,
			 "%s: option cannot be repeated -- o\n", argv[0]);
		*ecode = EXIT_FAILURE;
		return 0;
	    }
	    if (i == argc - 1) {
		fprintf (stderr,
			 "%s: option requires an argument -- o\n", argv[0]);
		*ecode = EXIT_FAILURE;
		return 0;
	    } 
	    i++;
	    *object = argv[i];
	    continue;
	}

	if (!strcmp (argv[i], "--"))
	    i++;

	if (i != argc) {
	  struct stat buf;
	  if (source[0]) {
	    fprintf (stderr,
		     "%s: only one source can be assembled at a time\n", 
		     argv[0]);
	    *ecode = EXIT_FAILURE;
	    return 0;
	  }
	  
	  strcpy (source, argv[i]);
	  if (-1 == stat (source, &buf))
	    strcat (source, ".s");
	  if (-1 == stat (source, &buf)) {
	    perror (source);
	    *ecode = EXIT_FAILURE;
	    return 0;
	  }
	}
    }

    if (!*object) {
      if (source[0]) {
	int len;
	char *filename = strrchr (source, '/');
	if (!filename)
	  filename = source;
	else
	  filename ++;

	len = strlen (filename);
	if (!strncmp (filename + len - 2, ".s", 2)) {
	  *object = malloc (len + 3);
	  strcpy (*object, filename);
	  strcpy (*object + len - 1, "bin");
	} else {
	  *object = malloc (len + 5);
	  strcpy (*object, filename);
	  strcat (*object, ".bin");
	}
      } else {
	*object = DEFAULT_OBJECT; 
      }
    }
    
    if (module_type == CLOF_UNKNOWN)
	module_type = CLOF_BIN;

    return 1;
}

