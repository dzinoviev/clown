#include <config.h>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif
#ifdef HAVE_LINUX_LIMITS_H
#include <linux/limits.h>
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
static char *myfile = NULL;
char **source = { &myfile };
char *cpp_options = "";			/* should we preprocess the file? */
FILE *debugfile;

int main (int argc, char *argv[])
{
    int ecode;
    char *object = NULL, *cmdline;
    FILE *infile;
    int outfile;
    int cmdlen = PATH_MAX + 1;

    if (setjmp (failure))
      return EXIT_FAILURE;

    if (!get_options (argc, argv, &object, source, &ecode))
	return ecode;

    cmdlen += sizeof (INCLUDEHOME) + 2;
    if (cpp_options)
	cmdlen += sizeof CPP + strlen (cpp_options) + 3;
    cmdline = safe_malloc (cmdlen);
    
    if (cpp_options) {		/* use CPP */
      strcpy (cmdline, CPP" ");
      /* Get default CPP options */
      strcat (cmdline, "-I");
      strcat (cmdline, INCLUDEHOME" ");
      strcat (cmdline, cpp_options);
      strcat (cmdline, " ");
    } else
	cmdline[0] = 0;
    
    if (*source[0]) {
	if (cpp_options) {
	  strcat (cmdline, *source);
	  infile = popen (cmdline, "r");
	  if (!infile) {
	    perror (cmdline);
	    return EXIT_FAILURE;
	  } 
	  free (cmdline);
	} else {
	  infile = fopen (*source, "r");
	  
	  if (!infile) {
	    perror (*source);
	    return EXIT_FAILURE;
	  } 
	}
    } else {
      strcpy (*source, "<stdin>");
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

    ecode = parse_and_assembly (infile, outfile);
    if (ecode == EXIT_FAILURE)
	if (-1 == unlink (object))
	    perror (object);

    close (outfile);

    return ecode;
}
