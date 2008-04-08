#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include "registers.h"
#include "clowndev.h"

int link_overhead = 0;/* not really needed */
int listing = 0;/* not really needed */
Clof_Type module_type = CLOF_UNKNOWN;/* not really needed */

void component_error (const char *name, const char *msg, char *detail)
{
    /* not really needed */
}

/* We are not ready to load more than one segment yet */
int load_memory (char *fname, Dword offset)
{
    int i;
    int size;
    int status = 0;

    modules = safe_realloc (modules, sizeof (struct Module) * (current_module + 1));
    init_module (&modules[current_module], fname);

    rdin = fopen (fname, "r");
    if (!rdin) {
	perror (fname);
	return EXIT_FAILURE; 
    }
    status = rdparse ();
    fclose (rdin);
  
    if (status)
	return 0;

    if (modules[current_module].type != CLOF_BIN) {
	fprintf (stderr, "%s is not a CLEF file\n", fname);
	return 0;
    }

    assert (modules[current_module].st.size == 1);
    size = modules[current_module].st.segments[0].file_size;

    if (size > CLOWN_MEMORY_SIZE - offset) {
	size = CLOWN_MEMORY_SIZE - offset;
	fprintf (stderr, "Warning: image file %s is large than the size of Clown memory\n", fname);
    }
    memcpy (&CLOWN_MEMORY[offset], modules[current_module].st.segments[0].image, 
	    size * sizeof (Dword));

    if (!modules[current_module].st.segments[0].files) {
	if (!silent)
	    fprintf (stderr, "--> File %s has no debug info.\n", fname);
    } else
	for (i = 0; i < modules[current_module].st.segments[0].nfiles; i++) {
	    struct stat src, clef;
	    struct DebugFile *df = &modules[current_module].st.segments[0].files[i];
	    
	    
	    if (-1 == stat (fname, &clef)) {
		perror (fname);
		return 0;
	    }
	    if (-1 == stat (df->file, &src)) {
		perror (df->file);
		return 0;
	    }
	    if (src.st_mtime >= clef.st_mtime && !silent)
		fprintf (stderr, 
			 "--> Source file %s is \n"
			 "--> newer than %s.\n",
			 df->file, fname);
	}

    current_module++;
    return size;
}

static struct DebugFile *lookup_debug_info (Dword address, int *record)
{
  int j;
  /* Search bottom up */
  for (j = current_module - 1; j >= 0; j--) {
    int i;
    if (address < modules[j].offset)
	continue;
    for (i = 0; i < modules[j].st.size; i++) {
	struct Segment s;
	int k;
	if (address >= modules[j].offset + modules[j].st.segments[i].image_size)
	    continue;
	s = modules[j].st.segments[i];
	address -= modules[j].offset;
	for (k = s.nfiles - 1; k >= 0; k--) {
	    int l;
	    struct DebugFile *df = &(s.files[k]);
	    for (l = df->nlines_inuse - 1; l >= 0; l--) {
		if (address >= df->flines[l].offset) {
		    *record = l;
		    return df;
		}
	    }
	}
    }
    return NULL;		/* is this possible? */
  }
  return NULL;
}

void print_debug_info (Dword address)
{ 
    int location;
    struct DebugFile *df = lookup_debug_info (address, &location);
    if (df)
	fprintf (stderr, " [%s:%d]", df->file, df->flines[location].line);
}
