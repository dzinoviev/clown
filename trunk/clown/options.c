#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "registers.h"
#include "version.h"
#include "clowndev.h"

static void show_version (void)
{
    fputs (VERSION"\n", stderr);
}

static void show_copy (void)
{
    fputs ("Copyright 2004-2005, 2008 D. Zinoviev\n"
	   "This program is free software; you may redistribute it under the terms of\n"
	   "the GNU General Public License.  This program has absolutely no warranty.\n", 
	   stderr);
}

static void show_usage (char *name)
{
    assert (name);

    fprintf (stderr, "USAGE: %s [option...] ROMIMAGE\n", name);
    fputs ("Options:\n", stderr);
    fputs ("  -h, --help                show this message and exit\n", stderr);
    fputs ("  -b, --bin ROMIMAGE ADDR   load ROMIMAGE into memory at address ADDR\n",
	   stderr);
    fputs ("  --cpl CPL                 pre-set initial CPL\n", stderr);
    fputs ("  -q, --silent              work silently\n", stderr);
    fputs ("  -r, --run                 run noninteractively\n", stderr);
    fputs ("  -v, --version             print simulator version number"
	                                " and exit\n", stderr);
    fputs ("\n", stderr);
    fputs ("Use variable CLOWN_DISC to specify the location of the disc "
	   "image file.\n", stderr);
}

int read_options (int argc, char *argv[])
{
    int i, interactive = 1;
    modules = NULL;
    current_module = 0;
/*    if (argc == 1) {
	show_usage (argv[0]);
	return 0;
    }*/

    for (i = 1; i < argc; i++) {
	if (   ! (strcmp (argv[i], "-h"))
	    || ! (strcmp (argv[i], "--help"))) {
	    show_usage (argv[0]);
	    return 0;
	}
      
	if (   !strcmp (argv[i], "-v")
	    || !strcmp (argv[i], "--version")) {
	    show_version ();
	    show_copy ();
	    return 0;
	}

	if (   !strcmp (argv[i], "-q")
	    || ! (strcmp (argv[i], "--silent"))) {
	    silent = 1;
	    continue;
	}
      
	if (   !strcmp (argv[i], "-r")
	    || ! (strcmp (argv[i], "--run"))) {
	    interactive = -1;
	    continue;
	}
      
	if (  ! (strcmp (argv[i], "-b"))
	    ||! (strcmp (argv[i], "--bin"))) {
	    char *endptr;
	    int offset;
	    i += 2;
	    if (i >= argc) {
		fprintf (stderr,
			 "%s: option requires two arguments -- bin\n", 
			 argv[0]);
		return 0;
	    }
	    offset = strtol (argv[i], &endptr, 0);
	    if (*endptr) {
		fprintf (stderr, 
			 "%s: bad characters in entry address: %s; "
			 "assuming entry = 0\n", argv[0], endptr);
		offset = 0;
	    }
	    if (!load_memory (argv[i - 1], offset))
		return 0;
	    continue;
	}
      
	if (! (strcmp (argv[i], "--cpl"))) {
	    i++;
	    if (i >= argc) {
		fprintf (stderr,
			 "%s: option requires an argument -- cpl\n", 
			 argv[0]);
		return 0;
	    }
	    initial_cpl = atoi (argv[i]);
	    continue;
	}	

	if (argv[i][0] == '-') {
	    fprintf (stderr, "Unknown option: <%s>. Ignored.\n", argv[i]);
	    continue;
	}

	/* default */
	if (!load_memory (argv[i], 0))
	    return 0;
    }

    if (!silent)
	show_version ();
    
    return interactive;
}

