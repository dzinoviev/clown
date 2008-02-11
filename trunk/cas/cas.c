#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#ifdef __APPLE__
#	include <limits.h>
#else
#	include <linux/limits.h>
#endif
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

#ifdef __STRICT_ANSI__
FILE *popen(const char *command, const char *type); /* for ANSI compatibility */
#endif

#include "cas.h"
#define CPP "cpp"
#define INCLUDE "/include/"
char source[PATH_MAX] = "";
char *cpp_options = "";			/* should we preprocess the file? */
FILE *debugfile;

int main (int argc, char *argv[])
{
    int ecode;
    char *object = NULL, *dfile = NULL, *cmdline;
    FILE *infile;
    int outfile;
    char *clownpath;
    int cmdlen = PATH_MAX + 1;

    if (!get_options (argc, argv, &object, source, &ecode))
	return ecode;

    dfile = malloc (strlen (object) + 5);
    if (!dfile) {
	perror ("malloc");
	return EXIT_FAILURE;
    }
    strcpy (dfile, object);
    strcat (dfile, ".dbg");

    clownpath = getenv ("CLOWN");
    if (clownpath)
	cmdlen += strlen (clownpath) + sizeof INCLUDE + 2;
    if (cpp_options)
	cmdlen += sizeof CPP + strlen (cpp_options) + 3;
    cmdline = malloc (cmdlen);
    
    if (!cmdline) {
	perror ("malloc");
	return EXIT_FAILURE;
    }
    
    if (cpp_options) {		/* use CPP */
      strcpy (cmdline, CPP" ");
      if (clownpath) {		/* Get default CPP options */
	strcat (cmdline, "-I");
	strcat (cmdline, clownpath);
	strcat (cmdline, INCLUDE" ");
      }
      strcat (cmdline, cpp_options);
      strcat (cmdline, " ");
    } else
	cmdline[0] = 0;
    
    if (source[0]) {
	if (cpp_options) {
	  strcat (cmdline, source);
	  infile = popen (cmdline, "r");
	  if (!infile) {
	    perror (cmdline);
	    return EXIT_FAILURE;
	  } 
	  free (cmdline);
	} else {
	  infile = fopen (source, "r");
	  
	  if (!infile) {
	    perror (source);
	    return EXIT_FAILURE;
	  } 
	}
    } else {
      strcpy (source, "<stdin>");
      if (cpp_options) {
	infile = popen (cmdline, "r");
	if (!infile) {
	  perror (cmdline);
	  return EXIT_FAILURE;
	} 
      } else
	infile = stdin;
    }

    outfile = open (object, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
    if (outfile == -1) {
	perror (object);
	return EXIT_FAILURE;
    }

    debugfile = fopen (dfile, "w");
    if (!debugfile) {
	perror (dfile);
	return EXIT_FAILURE;
    }
    fprintf (debugfile, "1\n0 %s\n", source);

    ecode = parse_and_assembly (infile, outfile);
    if (ecode == EXIT_FAILURE)
	if (-1 == unlink (object))
	    perror (object);

    fclose (debugfile);
    return ecode;
}
