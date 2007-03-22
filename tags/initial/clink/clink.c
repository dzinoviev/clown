#ifdef __APPLE__
#	include <limits.h>
#else
#	include <linux/limits.h>
#endif
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include "clink.h"
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include "clowndev.h"

int listing = 0;
int silent = 0;
int no_unresolved_segments = 1;

static struct Module {
    struct SegmentTable seg;
    struct LabelTable lab;
    off_t begin;
} *modules;

static struct SegmentTable all_seg = {0, NULL};
static struct LabelTable all_lab = {0, NULL};
static Bit has_unreferenced = 0;
jmp_buf failure;

void component_error (const char *msg, char *detail)
{
    fprintf (stderr, "clink:%s: %s\n", msg, detail);
}

static void secure_read (int file, void *addr, int size, char *name)
{
    if (size != read (file, addr, size)) {
	perror (name);
	longjmp (failure, 1);
    }
}

static void *secure_malloc (size_t size)
{
    void *p;
    if (!(p = malloc (size))) {
	perror ("malloc");
	longjmp (failure, 1);
    }
    return p;
}

static void read_string (int infile, char *addr, int len, char *fname)
{
    int j;
    for (j = 0; j < len; j++) {
	Dword character;
	secure_read (infile, &character, sizeof (Dword), fname);
	addr[j] = character;
    }
    addr[j] = 0;
}

static Bit unify_labels (int n_modules)
{
    int i, j, k, index = 0;
    all_lab.labels = secure_malloc (all_lab.size * sizeof (struct Label));

    /* Assemble explicitly defined symbols */
    for (i = 0; i < n_modules; i++)
	for (j = 0; j < modules[i].lab.size; j++) {
	    if (modules[i].lab.labels[j].defined) {
		int seg;
		/* Weed out duplicates */
		for (k = 0; k < index; k++) {
		    if (!strcmp (modules[i].lab.labels[j].name,
				 all_lab.labels[k].name)) {
			fprintf (stderr, 
				 "clink: duplicate symbol: %s\n",
				 modules[i].lab.labels[j].name);
			return EXIT_FAILURE;
		    }
		}
		/* Insert the symbol into the compound table */
		modules[i].lab.labels[j].new_index = index;
		all_lab.labels[index] = modules[i].lab.labels[j];

		/* Update segment reference */
		seg = all_lab.labels[index].segment;
		assert (seg != -1);
		all_lab.labels[index].segment = 
		    modules[i].seg.segments[seg].new_index;

		index++;
	    }
	}

    /* Identify undefined symbols */
    for (i = 0; i < n_modules; i++)
	for (j = 0; j < modules[i].lab.size; j++) {
	    if (!modules[i].lab.labels[j].defined) {
		for (k = 0; k < index; k++) {
		    if (! strcmp (all_lab.labels[k].name,
				  modules[i].lab.labels[j].name)) {
			modules[i].lab.labels[j].new_index = k;
			modules[i].lab.labels[j].defined = 1;
			break;
		    }
		}
	    }
	}


    /* Locate unresolved symbols */
    for (i = 0; i < n_modules; i++)
	for (j = 0; j < modules[i].lab.size; j++) {
	    if (!modules[i].lab.labels[j].defined) {
		if (no_unresolved_segments) {
		    fprintf (stderr, "clink: unresolved symbol: %s\n",
			     modules[i].lab.labels[j].name);
		    return EXIT_FAILURE;
		} else {
		    if (!silent)
			fprintf (stderr, "clink: warning: unresolved label: %s\n",
				 modules[i].lab.labels[j].name);
		    modules[i].lab.labels[j].new_index = index;
		    all_lab.labels[index] = modules[i].lab.labels[j];
		    index++;
		    has_unreferenced = 1;
		}
	    }
	}

    all_lab.size = index;
    all_lab.labels = realloc (all_lab.labels, 
			      all_lab.size * sizeof (struct Label));
    return EXIT_SUCCESS;
}

static Bit unify_segments (int n_modules)
{
    int i, j, k, index = 0;
    unsigned offset = 0;
    all_seg.segments = secure_malloc (all_seg.size * sizeof (struct Segment));

    /* Assemble explicitly defined segments */
    for (i = 0; i < n_modules; i++)
	for (j = 0; j < modules[i].seg.size; j++) {
	    if (modules[i].seg.segments[j].defined) {
		/* Weed out duplicates */
		for (k = 0; k < index; k++) {
		    if (!strcmp (modules[i].seg.segments[j].name,
				 all_seg.segments[k].name)) {
			fprintf (stderr, 
				 "clink: duplicate segment name: $%s\n",
				 modules[i].seg.segments[j].name);
			return EXIT_FAILURE;
		    }
		}
		/* Insert a segment into the compound table */
		modules[i].seg.segments[j].new_index = index;
		all_seg.segments[index] = modules[i].seg.segments[j];

		/* Update the offsets */
		all_seg.segments[index].file_offset = offset;
		offset += all_seg.segments[index].file_size;

		index++;
	    }
	}

    /* Identify undefined segments */
    for (i = 0; i < n_modules; i++)
	for (j = 0; j < modules[i].seg.size; j++) {
	    if (!modules[i].seg.segments[j].defined) {
		for (k = 0; k < index; k++) {
		    if (! strcmp (all_seg.segments[k].name,
				  modules[i].seg.segments[j].name)) {
			modules[i].seg.segments[j].new_index = k;
			modules[i].seg.segments[j].defined = 1;
			break;
		    }
		}
	    }
	}

    /* Locate unresolved segments */
    for (i = 0; i < n_modules; i++)
	for (j = 0; j < modules[i].seg.size; j++) {
	    if (!modules[i].seg.segments[j].defined) {
		if (no_unresolved_segments) {
		    fprintf (stderr, "clink: unresolved segment name: $%s\n",
			     modules[i].seg.segments[j].name);
		    return EXIT_FAILURE;
		} else {
		    if (!silent)
			fprintf (stderr, 
				 "clink: warning: unresolved segment name: $%s\n",
				 modules[i].seg.segments[j].name);
		    modules[i].seg.segments[j].new_index = index;
		    all_seg.segments[index] = modules[i].seg.segments[j];
		    index++;
		    has_unreferenced = 1;
		}
	    }
	}

    all_seg.size = index;
    all_seg.segments = realloc (all_seg.segments, 
				all_seg.size * sizeof (struct Segment));
    
    return EXIT_SUCCESS;
}

static int read_headers (char *fname, int ord)
{
    int infile, i;
    Dword instr, header;
    
    infile = open (fname, O_RDONLY);
    if (-1 == infile) {
	perror (fname);
	return EXIT_FAILURE;
    }

    /* Read CLOF signature */
    secure_read (infile, &instr, sizeof (instr), fname);
    memcpy (&header, clof_header, 4);
    if (header != instr) {
	fprintf (stderr, "clink: %s is not a CLOF file\n", fname);
	return EXIT_FAILURE;	
    }

    /*Read file type and incomplete flag */
    secure_read (infile, &instr, sizeof (instr), fname);
    if ((instr & ~INCOMPLETE) != CLOF_EXE) {
	fprintf (stderr, "clink: %s is not an EXE module\n", fname);
	return EXIT_FAILURE;	
    }

    /* Read in the segment table */
    secure_read (infile, &modules[ord].seg.size, 
		 sizeof (modules[ord].seg.size), fname);
    assert (modules[ord].seg.size >= 0);

    if (modules[ord].seg.size == 0) {
	fprintf (stderr, "clink: anonymous segment in module %s\n",
		 fname);
	return EXIT_FAILURE;	
    }

    modules[ord].seg.segments = secure_malloc (modules[ord].seg.size 
					       * sizeof (struct Segment));
    all_seg.size += modules[ord].seg.size;

    for (i = 0; i < modules[ord].seg.size; i++) {
	Dword len, j;
	struct Segment s;
	secure_read (infile, &len, sizeof (len), fname);
	assert (len);

	s.name = secure_malloc (len);
	read_string (infile, s.name, len, fname);

	secure_read (infile, &j, sizeof (j), fname);
	s.defined = j;
	if (j) {
	    secure_read (infile, &s.type, sizeof (s.type), fname);
	    secure_read (infile, &s.file_offset, sizeof (s.file_offset), 
			 fname);
	    secure_read (infile, &s.file_size, sizeof (s.file_size), fname);
	}
	modules[ord].seg.segments[i] = s;
    }

    /* Read in the export/reference table */
    secure_read (infile, &modules[ord].lab.size, 
		 sizeof (modules[ord].lab.size), fname);
    assert (modules[ord].lab.size > 0);
    modules[ord].lab.labels = secure_malloc (modules[ord].lab.size
					     * sizeof (struct Label));

    all_lab.size += modules[ord].lab.size;

    for (i = 0; i < modules[ord].lab.size; i++) {
	Dword len, j;
	struct Label l;
	secure_read (infile, &len, sizeof (len), fname);
	assert (len);

	l.near = 0;
	l.name = secure_malloc (len);
	read_string (infile, l.name, len, fname);

	secure_read (infile, &j, sizeof (j), fname);
	l.defined = j;
	secure_read (infile, &j, sizeof (j), fname);
	l.export = j;
	if (j) {
	    secure_read (infile, &l.segment, sizeof (l.segment), fname);
	    secure_read (infile, &l.address, sizeof (l.address), fname);
	}

	modules[ord].lab.labels[i] = l;
    }

    /* Record the current position */
    modules[ord].begin = lseek (infile, 0, SEEK_CUR);
    close (infile);

    return EXIT_SUCCESS;
}

int main (int argc, char *argv[])
{
    char **source = NULL;
    int n_modules = 0;
    int ecode = EXIT_SUCCESS;
    char *object = NULL;
    int i, outfile;

    if (!get_options (argc, argv, &object, &source, &n_modules, &ecode))
	return ecode;

    if (setjmp (failure))
	return EXIT_FAILURE;

    /* Read the tables of all files */
    modules = secure_malloc (n_modules * sizeof (struct Module));
    for (i = 0; i < n_modules; i++)
	if (EXIT_FAILURE == read_headers (source[i], i))
	    return EXIT_FAILURE;

    if (EXIT_FAILURE == unify_segments (n_modules))
	return EXIT_FAILURE;

    if (EXIT_FAILURE == unify_labels (n_modules))
	return EXIT_FAILURE;

    if (listing) {
	int i;
	fprintf (stderr, "\nSegments:\n");
	for (i = 0; i < all_seg.size; i++)
	    list_segment (all_seg.segments[i]);
	fprintf (stderr, "\nSymbols:\n");
	for (i = 0; i < all_lab.size; i++)
	    list_label (all_lab.labels[i], all_seg.segments);
    }

    if (-1 == (outfile = open (object, O_RDWR | O_TRUNC | O_CREAT, S_IRWXU))) {
	perror (object);
	return EXIT_FAILURE;
    }

    write_header (outfile, has_unreferenced, &all_seg, all_lab.size, &all_lab);

    for (i = 0; i < n_modules; i++) {
	int infile = open (source[i], O_RDONLY);
	if (-1 == infile || -1 == lseek (infile, modules[i].begin, SEEK_SET)) {
	    perror (source[i]);
	    return EXIT_FAILURE;
	}
	copy_code (infile, outfile, &all_seg, &all_lab);
	close (infile);
    }

    close (outfile);
    return ecode;
}
