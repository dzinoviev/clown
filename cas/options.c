#include <config.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "cas.h"

#define DEFAULT_OBJECT "a.cle"
#define CAS_VERSION "CLOWN assembler version "VERSION" ["__DATE__"]"

static void show_version (void)
{
    fputs (CAS_VERSION"\n", stderr);
}

static void show_copy (void)
{
    fputs (COPYRIGHT, stderr);
}

static void show_usage (char *name)
{
    assert (name);

    fprintf (stderr, "USAGE: %s [option...] IFILE.s\n", name);
    fputs ("Options:\n", stderr);
    fputs ("  -h, --help               show this message and exit\n", stderr);
    fputs ("  -P, --nocpp              do not preprocess\n", stderr);
    fputs ("  --pass-to-cpp \"OPTIONS\"  pass OPTIONS to preprocessor\n", stderr);
    fputs ("      option -I"INCLUDEHOME" is passed implicitly\n", stderr);
    fputs ("  -b, --cle                generate a CLE module (default)\n", stderr);
    fputs ("  -s, --clo                generate a CLO module\n", stderr);
    fprintf (stderr, "  -e ADDR, --entry ADDR    set start address (default %"PRIi32")\n", offset);
    fputs ("  -o OFILE                 set output file name (default IFILE.cle/IFILE.clo)\n", stderr);
    fputs ("  -v, --version            print assembler version number and exit\n", stderr);
    fputs ("  -l, --listing            produce listings\n", stderr);
    fputs ("  -ng, --nodebug           do not generate debug info\n", stderr);
    fputs ("  -V                       print assembler version number\n", stderr);
    fputs ("  --                       treat the next argument as a file name\n", stderr);
}

int get_options (int argc, char *argv[], char **object,
		 char **source, int *ecode)
{
    int i;
    char *suffix = NULL;

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
	    global_offset = strtol (argv[i], &endptr, 0);
	    if (*endptr) {
		fprintf (stderr, 
			 "%s: bad characters in entry address: %s; "
			 "assuming entry = 0\n", argv[0], endptr);
		global_offset = 0;
	    }
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

	if (   !strcmp (argv[i], "--clo")
	    || !strcmp (argv[i], "-s")) {
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

	if (   !strcmp (argv[i], "--nodebug")
	    || !strcmp (argv[i], "-ng")) {
	    debug = 0;
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
	  if (*source) {
	    fprintf (stderr,
		     "%s: only one source can be assembled at a time\n", 
		     argv[0]);
	    *ecode = EXIT_FAILURE;
	    return 0;
	  }
	  
	  *source = safe_malloc (strlen (argv[i]) + 3);
	  strcpy (*source, argv[i]);
	  if (-1 == stat (*source, &buf))
	    strcat (*source, ".s");
	  if (-1 == stat (*source, &buf)) {
	    perror (*source);
	    *ecode = EXIT_FAILURE;
	    return 0;
	  }
	}
    }

    if (!*source) {
      fprintf (stderr,
	       "%s: no input files\n", 
	       argv[0]);
      *ecode = EXIT_FAILURE;
      return 0;
    }

    switch (module_type) {
    case CLOF_UNKNOWN:
      module_type = CLOF_BIN;
    case CLOF_BIN:
      suffix = "cle";
      break;
    case CLOF_EXE:
      suffix = "clo";
      break;
    }

    if (!*object) {
      if (*source[0]) {
	int len;
	char *filename = strrchr (*source, '/');
	if (!filename)
	  filename = *source;
	else
	  filename ++;

	len = strlen (filename);
	if (!strncmp (filename + len - 2, ".s", 2)) {
	  *object = safe_malloc (len + 3);
	  strcpy (*object, filename);
	  strcpy (*object + len - 1, suffix);
	} else {
	  *object = safe_malloc (len + 5);
	  strcpy (*object, filename);
	  strcat (*object, ".");
	  strcat (*object, suffix);
	}
      } else {
	*object = DEFAULT_OBJECT; 
      }
    }
    
    return 1;
}

