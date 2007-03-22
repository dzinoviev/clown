#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "clink.h"

#define DEFAULT_OBJECT "a.out"
#define VERSION "CLOWN linker version 1.00 ["__DATE__"]"

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

    fprintf (stderr, "USAGE: %s [option...] binfile [binfile...]\n", name);
    fputs ("Options:\n", stderr);
    fputs ("  -h, --help                show this message and exit\n", stderr);
    fputs ("  -o FILE                   set output file name (default "DEFAULT_OBJECT")\n", stderr);
    fputs ("  -v, --version             print linker version number and exit\n", stderr);
    fputs ("  -l, --listing             produce listings\n", stderr);
    fputs ("  -U, --allow-unresolved    allow unresolved segments and symbols\n", stderr);
    fputs ("  --silent                  work silently\n", stderr);
    fputs ("  -V                        print linker version number\n", stderr);
    fputs ("  --                        treat the next argument as a file name\n", stderr);
}

int get_options (int argc, char *argv[], char **object,
		 char ***source, int *nsources, int *ecode)
{
    int i;

    assert (argv);
    assert (object);
    assert (source);
    assert (!*source);
    assert (ecode);
    assert (nsources);
    assert (!*nsources);

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

	if (   !strcmp (argv[i], "--listing")
	    || !strcmp (argv[i], "-l")) {
	    listing = 1;
	    continue;
	}

	if (   !strcmp (argv[i], "--silent")
	    || !strcmp (argv[i], "-q")) {
	    silent = 1;
	    continue;
	}

	if (   !strcmp (argv[i], "--allow-unresolved")
	    || !strcmp (argv[i], "-U")) {
	    no_unresolved_segments = 0;
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

	  if (-1 == stat (argv[i], &buf)) {
	      perror (argv[i]);
	      *ecode = EXIT_FAILURE;
	      return 0;
	  }

	  *source = realloc (*source, sizeof (char **) * (*nsources + 1));
	  assert (*source);	/* BAD! */
	  (*source)[*nsources] = malloc (strlen (argv[i]) + 1);
	  assert ((*source)[*nsources]); /* BAD! */
	  strcpy ((*source)[*nsources], argv[i]);
	  (*nsources)++;
	}
    }

    if (!*object)
	*object = DEFAULT_OBJECT; 
    
    return 1;
}

