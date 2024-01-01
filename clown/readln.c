#include <config.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifdef HAVE_LIBREADLINE
#include <readline/readline.h>
#include <readline/history.h>
#include "readln.h"
#endif

/* A static variable for holding the line. */
static char *line_read = (char *)NULL;

/* Read a string, and return a pointer to it.
Returns NULL on EOF. */

char *rl_gets ()
{
  /* If the buffer has already been allocated,
     return the memory to the free pool. */
  if (line_read) {
    free (line_read);
    line_read = NULL;
  }
  
  /* Get a line from the user. */
#ifdef HAVE_LIBREADLINE
  line_read = readline ("CLOWN> ");
#else
  line_read = malloc (1024);
  fputs ("CLOWN> ", stdout);
  line_read = fgets (line_read, 1024, stdin);
#endif 
  /* If the line has any text in it,
     save it on the history. */
  if (line_read) {
#ifdef HAVE_LIBREADLINE
    add_history (line_read);
#endif
    line_read = realloc(line_read, strlen(line_read)+2);
    strcat(line_read, "\n");
  }
	
  return (line_read);
}

