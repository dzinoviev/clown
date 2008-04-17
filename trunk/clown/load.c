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
struct Clown_Segment_Descriptor init_ldt_descr = {0,0,0};

void component_error (const char *name, const char *msg, char *detail)
{
    /* not really needed */
}

/* We are not ready to load more than one segment yet */
int load_memory (char *fname, Dword offset)
{
    int i, j;
    int size = 0, jstart = 0;
    int status = 0, module_offset = 0;
    struct Clown_Segment_Descriptor *my_ldt;

    modules = safe_realloc (modules, sizeof (struct Module) * (current_module + 1));
    init_module (&modules[current_module], fname);

    rdin = fopen (fname, "r");
    if (!rdin) {
	perror (fname);
	return 0; 
    }
    status = rdparse ();
    fclose (rdin);
  
    if (status)
	return 0;

    if (modules[current_module].type != CLOF_BIN) {
	fprintf (stderr, "--> %s is not a CLEF file\n", fname);
	return 0;
    }

    my_ldt = safe_malloc (modules[current_module].st.size * sizeof (struct Clown_Segment_Descriptor));
    my_ldt[0].base = 0;
    my_ldt[0].limit = CLOWN_MEMORY_SIZE;
    my_ldt[0].flags = 0;
    SF_SET_PRESENT (my_ldt[0]);
    SF_SET_PERM (my_ldt[0], (CAN_READ | CAN_WRITE | CAN_EXEC));

    /*    assert (modules[current_module].st.size == 1);*/
    if (modules[current_module].st.size > 1) {
	jstart++;
    }

    for (j = jstart; j < modules[current_module].st.size; j++) {
	if (!modules[current_module].st.segments[j].in_use)
	    continue;

	size = modules[current_module].st.segments[j].file_size;

	if (size > CLOWN_MEMORY_SIZE - offset - module_offset) {
	    size = CLOWN_MEMORY_SIZE - offset - module_offset;
	    fprintf (stderr, "--> Warning: image file %s is large than the size of Clown memory\n", 
		     fname);
	}
	memcpy (&CLOWN_MEMORY[offset + module_offset], modules[current_module].st.segments[j].image, 
		size * sizeof (Dword));
	
	if (!modules[current_module].st.segments[j].files) {
	    if (!silent)
		fprintf (stderr, "--> Segment %s in file %s has no debug info.\n", 
			 modules[current_module].st.segments[j].name, fname);
	} else
	    for (i = 0; i < modules[current_module].st.segments[j].nfiles; i++) {
		struct stat src, clef;
		struct DebugFile *df = &modules[current_module].st.segments[j].files[i];
	    
		
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

	if (j > DEFAULT_SEGMENT) {
	    my_ldt[j].flags = 0;
	    SF_SET_PRESENT (my_ldt[j]);
	    switch (modules[current_module].st.segments[j].type) {
	    case SEG_DEFAULT:
		SF_SET_PERM (my_ldt[j], (CAN_READ | CAN_WRITE | CAN_EXEC));
		break;
	    case SEG_CODE:
		SF_SET_PERM (my_ldt[j], (CAN_READ | CAN_EXEC));
		break;
	    case SEG_DATA:
		SF_SET_PERM (my_ldt[j], (CAN_READ | CAN_WRITE));
		break;
	    case SEG_CONST:
		SF_SET_PERM (my_ldt[j], CAN_READ);
		break;
	    }
	    /* Other descriptor flags can be set up here */
	    my_ldt[j].base = offset + module_offset;
	    my_ldt[j].limit = size;
	    /*	    printf ("Descriptor created: sbase=%d, ssize=%d sflags=%d\n",
	      my_ldt[j].base, my_ldt[j].limit, my_ldt[j].flags);	    */
	}

	module_offset += size;
    }

    /* Load the LDT into the RAM immediately after the last segment from the file */
    if (modules[current_module].st.size > 1) {
	struct Clown_Segment_Descriptor ldt_descr;
	int ldt_size = modules[current_module].st.size * sizeof (struct Clown_Segment_Descriptor);
	int new_ldt_address, ldt_address = offset + module_offset;
	new_ldt_address = ldt_address / LGDT_ENTRY_SIZE;
	new_ldt_address *= LGDT_ENTRY_SIZE;
	if (new_ldt_address < ldt_address) {
	    new_ldt_address += LGDT_ENTRY_SIZE;
	    size += LGDT_ENTRY_SIZE;
	}
	memcpy (&CLOWN_MEMORY[new_ldt_address], my_ldt, ldt_size);
	size += ldt_size;
	/*	printf ("LDT with %d entries loaded at address %d\n", modules[current_module].st.size,
	  new_ldt_address);*/

	ldt_descr.flags = 0;
	SF_SET_PRESENT (ldt_descr);
	ldt_descr.base = new_ldt_address;
	ldt_descr.limit = modules[current_module].st.size * LGDT_ENTRY_SIZE;

	if (SF_PRESENT (init_ldt_descr))
	    fprintf (stderr, "--> Warning: more than one LDT has been loaded.\n"
		     "--> Warning: only the most recent is saved.");
	init_ldt_descr = ldt_descr;
	/*	printf ("Descriptor created and loaded into %%LDT: sbase=%d, ssize=%d sflags=%d\n",
	  new_ldt_address, ldt_descr.limit, ldt_descr.flags);*/
    }

    current_module++;
    return size + module_offset;
}

/*
So far, this function does not work for multisegment files.
*/
static struct DebugFile *lookup_debug_info (Dword address, int *record)
{
  int j;
  /* Search bottom up */
  for (j = current_module - 1; j >= 0; j--) {
    int i;
    if (address < modules[j].offset)
	continue;
    for (i = modules[j].st.size - 1; i >= 0; i--) {
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
