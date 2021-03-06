#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
/*#define REALLY_VERBOSE*/
#include "registers.h"
#include "clowndev.h"

int firstcode = -1;
int link_overhead = 0;/* not really needed */
int listing = 0;/* not really needed */
Clof_Type module_type = CLOF_UNKNOWN;/* not really needed */
struct Clown_Segment_Descriptor init_ldt_descr = {0,0,0};

void component_error (const char *name, const char *msg, char *detail)
{
    /* not really needed */
}

int load_memory (char *fname, Dword offset)
{
    int i, j;
    int size = 0, jstart = 0;
    int status = 0, module_offset = 0;
    struct Clown_Segment_Descriptor *my_ldt;

    modules = safe_realloc (modules, sizeof (struct Module) * (current_module + 1));
    init_module (&modules[current_module], fname);

    yyin = fopen (fname, "r");
    if (!yyin) {
	perror (fname);
	return 0; 
    }
    yyrestart (yyin);
    line_no = 1;
    source = &fname;
    status = yyparse ();
    fclose (yyin);
  
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

    if (modules[current_module].st.size > 1) {
	jstart++;
    }

    for (j = jstart; j < modules[current_module].st.size; j++) {
	if (!modules[current_module].st.segments[j].in_use)
	    continue;

	size = modules[current_module].st.segments[j].image_size;

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
		if (-1 == firstcode)
		    firstcode = j;
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
	    /* Record the actuall segment base */
	    modules[current_module].st.segments[j].base = my_ldt[j].base;
#ifdef REALLY_VERBOSE
	    printf ("Descriptor created: sbase=%d, ssize=%d sflags=%d\n",
		    my_ldt[j].base, my_ldt[j].limit, my_ldt[j].flags);	    
#endif
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
#ifdef REALLY_VERBOSE
	printf ("LDT with %d entries loaded at address %d\n", modules[current_module].st.size,
		new_ldt_address);
#endif

	ldt_descr.flags = 0;
	SF_SET_PRESENT (ldt_descr);
	ldt_descr.base = new_ldt_address;
	ldt_descr.limit = modules[current_module].st.size * LGDT_ENTRY_SIZE;

	if (SF_PRESENT (init_ldt_descr))
	    fprintf (stderr, "--> Warning: more than one LDT has been loaded.\n"
		     "--> Warning: only the most recent is saved.");
	init_ldt_descr = ldt_descr;
#ifdef REALLY_VERBOSE
	printf ("Descriptor created and loaded into %%LDT: sbase=%d, ssize=%d sflags=%d\n",
		new_ldt_address, ldt_descr.limit, ldt_descr.flags);
#endif
    }

    current_module++;
    return size + module_offset;
}

