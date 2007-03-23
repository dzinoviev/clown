
#include "readln.h"
#include <stdio.h>
#include <readline/readline.h>

/* A static variable for holding the line. */
static char *line_read = (char *)NULL;

/* Read a string, and return a pointer to it.
Returns NULL on EOF. */
char *rl_gets ()
{
	/* If the buffer has already been allocated,
	return the memory to the free pool. */
	if (line_read)
    {
		free (line_read);
		line_read = (char *)NULL;
    }
	
	/* Get a line from the user. */
	line_read = readline ("CLOWN> ");
	
	/* If the line has any text in it,
	save it on the history. */
	if (line_read && *line_read)
		add_history (line_read);
	
	return (line_read);
}

