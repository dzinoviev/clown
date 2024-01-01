#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "clink.h"

#define DEFAULT_OBJECT "default.clo"
#define DEFAULT_EXE "default.cle"
#define CLINK_VERSION "CLOWN linker version "PACKAGE_VERSION" ["__DATE__"]"

static void show_version (void)
{
    fputs (CLINK_VERSION"\n", stderr);
}

static void show_copy (void)
{
    fputs (COPYRIGHT, stderr);
}

static void show_usage (char *name)
{
    assert (name);

    fprintf (stderr, "USAGE: %s [option...] binfile [binfile...]\n", name);
    fputs ("Options:\n", stderr);
    fputs ("  -h, --help                show this message and exit\n", stderr);
    fputs ("  -b, --cle                 generate a CLE module (default)\n", stderr);
    fputs ("  -s, --clo                 generate a CLO module\n", stderr);
    fputs ("  -o FILE                   set output file name ("DEFAULT_OBJECT"/"DEFAULT_EXE")\n", stderr);
    fputs ("  -v, --version             print linker version number and exit\n", stderr);
    fputs ("  -l, --listing             produce listings\n", stderr);
    fputs ("  -ng, --nodebug            do not generate debug info\n", stderr);
    fputs ("  -q, --silent              work silently\n", stderr);
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

	if (!strcmp (argv[i], "--version")
	    || !strcmp (argv[i], "-v")) {
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
	    if (silent)
		fprintf (stderr,
			 "%s: cannot produce listings in the silent mode; option -l ignored\n", argv[0]);
	    else
		listing = 1;
	    continue;
	}

	if (   !strcmp (argv[i], "--silent")
	    || !strcmp (argv[i], "-q")) {
	    if (listing)
		fprintf (stderr,
			 "%s: cannot work silently and produce listings; option -q ignored\n", argv[0]);
	    else
		silent = 1;
	    continue;
	}

	if (   !strcmp (argv[i], "--clo")
	    || !strcmp (argv[i], "-s")) {
	    if (module_type != CLOF_UNKNOWN) {
		fprintf (stderr,
			 "%s: module type cannot be redefined\n",argv[0]);
	    } else
		module_type = CLOF_EXE;
	    continue;
	}

	if (   !strcmp (argv[i], "--cle")
	    || !strcmp (argv[i], "-b")) {
	    if (module_type != CLOF_UNKNOWN) {
		fprintf (stderr,
			 "%s: module type cannot be redefined\n",argv[0]);
	    } else
		module_type = CLOF_BIN;
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


	if (   !strcmp (argv[i], "--nodebug")
	    || !strcmp (argv[i], "-ng")) {
	    debug = 0;
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
	  if (!*source) {
	      perror ("realloc");
	      longjmp (failure, 1);	      
	  }
	  (*source)[*nsources] = malloc (strlen (argv[i]) + 1);
	  if (!(*source)[*nsources]) {
	      perror ("malloc");
	      longjmp (failure, 1);	      
	  }
	  strcpy ((*source)[*nsources], argv[i]);
	  (*nsources)++;
	}
    }

    if (module_type == CLOF_UNKNOWN)
	module_type = CLOF_BIN;

    if (!*object)
	*object = (module_type == CLOF_BIN) ? DEFAULT_EXE : DEFAULT_OBJECT; 

    return 1;
}

