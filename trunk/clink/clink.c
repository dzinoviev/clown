#ifdef __APPLE__
#	include <limits.h>
#else
#	include <linux/limits.h>
#endif
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include "clink.h"
#include "clowndev.h"

int listing = 0;
int silent = 0;
Clof_Type module_type = CLOF_UNKNOWN;
int current_segment = DEFAULT_SEGMENT;

static struct SegmentTable all_seg = {0, NULL};
static struct LabelTable all_sym = {0, NULL};

void component_error (const char *name, const char *msg, char *detail)
{
    if (!silent)
	fprintf (stderr, "clink: %s: %s: %s\n", name, msg, detail);
}

static int newModules (int n_modules, char **name)
{
  int i;
  modules = safe_malloc (sizeof (struct Module) * n_modules);
  for (i = 0; i < n_modules; i++)
      init_module (&modules[i], name[i]);

  return 1;
}

static int unify_exe_segments (int n_modules)
{
    int i, j, k, index;

    index = DEFAULT_SEGMENT + 1;

    /* Assemble explicitly defined segments */
    for (i = 0; i < n_modules; i++) {
	modules[i].st.segments[DEFAULT_SEGMENT].in_use = 0;
	for (j = DEFAULT_SEGMENT + 1; j < modules[i].st.size; j++) {
	    if (   !modules[i].st.segments[j].in_use 
		|| !modules[i].st.segments[j].defined)
		continue;
	    /* Weed out duplicates */
	    for (k = DEFAULT_SEGMENT + 1; k < index; k++) {
		if (!strcmp (modules[i].st.segments[j].name, all_seg.segments[k].name)) {
		    component_error (modules[i].name, "duplicate segment", 
				     modules[i].st.segments[j].name);
		    return -1;
		}
	    }
	    /* Insert a segment into the compound table */
	    modules[i].st.segments[j].new_index = index;
	    all_seg.segments[index++] = modules[i].st.segments[j];
	}
    }

    /* Identify undefined segments */
    for (i = 0; i < n_modules; i++) {
	for (j = DEFAULT_SEGMENT + 1; j < modules[i].st.size; j++) {
	    if (    modules[i].st.segments[j].in_use 
		&& !modules[i].st.segments[j].defined) {
		for (k = DEFAULT_SEGMENT + 1; k < index; k++) {
		    if (! strcmp (all_seg.segments[k].name, modules[i].st.segments[j].name)) {
			modules[i].st.segments[j].new_index = k;
			break;
		    }
		}
		if (k == index) { /* not found! */
		    if (module_type == CLOF_BIN) {
			component_error (modules[i].name, "unresolved segment in a CLE file", 
					 modules[i].st.segments[j].name);
			return -1;
		    } else {
			if (!silent)
			    component_error (modules[i].name, "warning: unresolved segment", 
					     modules[i].st.segments[j].name);
			modules[i].st.segments[j].new_index = index;
			all_seg.segments[index++] = modules[i].st.segments[j];
		    }
		}
	    }
	}
    }

    return index;
}

static int unify_bin_segments (int n_modules)
{
    int i, j, index;
    int offset;
    struct Segment *dest_seg = &all_seg.segments[DEFAULT_SEGMENT];
    struct Segment *src_seg = &modules[0].st.segments[DEFAULT_SEGMENT];

    assert (src_seg->defined);
    assert (!src_seg->name);
    assert (src_seg->new_index == DEFAULT_SEGMENT);

    src_seg->name = DEFAULT_SEGMENT_NAME;
    *dest_seg = *src_seg;

    index = DEFAULT_SEGMENT + 1;
    
    offset = src_seg->image_size - src_seg->escapes;

    /* collect the pieces */
    for (i = 1; i < n_modules; i++) {
	struct Segment *this_seg = &modules[i].st.segments[DEFAULT_SEGMENT];
	int new_nfiles, k;
	assert (modules[i].st.size == 1);

	this_seg->name = DEFAULT_SEGMENT_NAME;
	dest_seg->file_size += this_seg->file_size;
	dest_seg->image_size += this_seg->image_size;
	    
	for (k = 0; k < modules[i].lt.size; k++) {
	    assert (modules[i].lt.labels[k].segment == DEFAULT_SEGMENT);
	    if (modules[i].lt.labels[k].in_use && modules[i].lt.labels[k].defined)
		modules[i].lt.labels[k].address += offset;
	}
	    
	new_nfiles = src_seg->nfiles + this_seg->nfiles;
	src_seg->files = safe_realloc (src_seg->files, sizeof (struct DebugFile) * new_nfiles);
	for (k = 0, j = src_seg->nfiles; k < this_seg->nfiles; k++, j++) {
	    int l;
	    src_seg->files[j] = this_seg->files[k];
	    for (l = 0; l < this_seg->files[k].nlines_inuse; l++)
		this_seg->files[k].flines[l].offset += offset;
	}
	src_seg->nfiles = new_nfiles;
	offset += this_seg->image_size - this_seg->escapes;
	src_seg->link_overhead += this_seg->link_overhead;
    }
    dest_seg->files = src_seg->files;
    dest_seg->nfiles = src_seg->nfiles;
    dest_seg->link_overhead = src_seg->link_overhead;

    return index;
}

static Clof_Type mix_type = CLOF_UNKNOWN;
static int unify_segments (int n_modules)
{
    int i, j, index = 0;
    Clof_Type module_mix_type = CLOF_UNKNOWN;

    all_seg.segments = safe_malloc (all_seg.size * sizeof (struct Segment));

    /* What kind of mixture is this? */    
    for (i = 0; i < n_modules; i++) {
	if (module_mix_type == CLOF_UNKNOWN)
	    module_mix_type = modules[i].type;
	else if (modules[i].type != module_mix_type) {
	    component_error (modules[i].name, "CLOF and CLEF modules mixed", modules[i].name);
	    return EXIT_FAILURE;
	}

	for (j = 0; j < modules[i].st.size; j++) {
	    if (modules[i].st.segments[j].in_use && modules[i].st.segments[j].defined) {
		if (modules[i].st.segments[j].name) {
		    if (mix_type != CLOF_BIN)
			mix_type = CLOF_EXE;
		    else {
			component_error (modules[i].name, "named and anonymous segments mixed", 
					 modules[i].st.segments[j].name);
			return EXIT_FAILURE;
		    }
		} else {
		    if (mix_type != CLOF_EXE) {
			mix_type = CLOF_BIN;
		    } else {
			component_error (modules[i].name, "named and anonymous segments mixed",
					 DEFAULT_SEGMENT_NAME);
			return EXIT_FAILURE;
		    }
		}
	    }
	}
    }

    if (mix_type == CLOF_BIN) {
	index = unify_bin_segments (n_modules);
    } else {
	if (-1 == (index = unify_exe_segments (n_modules)))
	    return EXIT_FAILURE;
    }

    all_seg.size = index;
    all_seg.segments = realloc (all_seg.segments, 
				all_seg.size * sizeof (struct Segment));
    
    return EXIT_SUCCESS;
}

static int save_segments (int outfile, int n_modules) 
{
    int status = 1, i, j;
    /* The header */
    write_header (outfile, &all_seg, &all_sym);

    for (i = 0; i < n_modules; i++) {
	for (j = 0; j < modules[i].st.size; j++) { 
	    int fragment = 0; 
	    if (!modules[i].st.segments[j].defined || !modules[i].st.segments[j].in_use)
		continue;
	    if (j == DEFAULT_SEGMENT) {
		if (!i)
		    fragment |= FIRST_FRAGMENT;
		if (i == n_modules - 1)
		    fragment |= LAST_FRAGMENT;
	    } else {
		fragment = (FIRST_FRAGMENT | LAST_FRAGMENT);
	    }
	    status &= save_segment (outfile, j, &modules[i].st, &modules[i].lt, fragment);
	}
    }

    /* The trailer */
    write_trailer (outfile);
    return !status;
}

static int unify_symbols (int n_modules)
{
  int i, j, k, index = 0;
  all_sym.labels = safe_malloc (all_sym.size * sizeof (struct Label));

  /* Assemble explicitly defined symbols */
  for (i = 0; i < n_modules; i++) {
    for (j = 0; j < modules[i].lt.size; j++) {
      int seg;
      if (!modules[i].lt.labels[j].defined || !modules[i].lt.labels[j].in_use)
	continue;

      if (modules[i].lt.labels[j].export) {
	/* Weed out duplicates */
	for (k = 0; k < index; k++) {
	  if (!strcmp (modules[i].lt.labels[j].name, all_sym.labels[k].name)) {
	    component_error (modules[i].name, "duplicate symbol", modules[i].lt.labels[j].name);
	    return EXIT_FAILURE;
	  }
	}
      }

      seg = modules[i].lt.labels[j].segment;
      if (seg != DEFAULT_SEGMENT) {
	/* Update segment reference */
	modules[i].lt.labels[j].segment = modules[i].st.segments[seg].new_index;
      }
      
      /* Insert the symbol into the compound table */
      modules[i].lt.labels[j].new_index = index;
      all_sym.labels[index] = modules[i].lt.labels[j];

      index++;
    }
  }


  /* Identify undefined symbols */
  for (i = 0; i < n_modules; i++)
    for (j = 0; j < modules[i].lt.size; j++) {
      if (modules[i].lt.labels[j].in_use && !modules[i].lt.labels[j].defined) {
	for (k = 0; k < index; k++) {
	  if (all_sym.labels[k].export && !strcmp (all_sym.labels[k].name, modules[i].lt.labels[j].name)) {
	    modules[i].lt.labels[j].new_index = k;
	    modules[i].lt.labels[j].defined = 1;
	    modules[i].lt.labels[j].address = all_sym.labels[k].address;
	    break;
	  }
	}
      }
    }


  /* Locate unresolved symbols */
  for (i = 0; i < n_modules; i++)
    for (j = 0; j < modules[i].lt.size; j++) {
      if (modules[i].lt.labels[j].in_use && !modules[i].lt.labels[j].defined) {
	if (module_type == CLOF_BIN) {
	  component_error (modules[i].name, "unresolved symbol in a CLE module", modules[i].lt.labels[j].name);
	  return EXIT_FAILURE;
	} else {
	  if (!silent)
	    component_error (modules[i].name, "warning: unresolved symbol", modules[i].lt.labels[j].name);
	  modules[i].lt.labels[j].new_index = index;
	  all_sym.labels[index] = modules[i].lt.labels[j];
	  index++;
	}
      }
    }

  all_sym.size = index;
  all_sym.labels = realloc (all_sym.labels, 
			    all_sym.size * sizeof (struct Label));

  return EXIT_SUCCESS;
}

int main (int argc, char *argv[])
{
    int n_modules = 0;
    int ecode = EXIT_SUCCESS;
    char *object = NULL;
    int status = 0;
    int outfile;

    if (!get_options (argc, argv, &object, &source, &n_modules, &ecode))
	return ecode;

    if (setjmp (failure))
	return EXIT_FAILURE;

    if (!newModules (n_modules, source))
      return EXIT_FAILURE;
    
    link_overhead = 0;
    for (current_module = 0; current_module < n_modules; current_module++) {
      rdin = fopen (source[current_module], "r");
      if (!rdin) {
	perror (source[current_module]);
	return EXIT_FAILURE; 
      }
      status |= rdparse ();
      fclose (rdin);
      all_sym.size += modules[current_module].lt.size;
      all_seg.size += modules[current_module].st.size;
    }

    if (status)
      return status;

    if ((status = unify_segments (n_modules)) != EXIT_SUCCESS)
      return status;
    if ((status = unify_symbols (n_modules)) != EXIT_SUCCESS)
      return status;

    outfile = open (object, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
    if (outfile == -1) {
	perror (object);
	return EXIT_FAILURE;
    }

    if ((status = save_segments (outfile, n_modules)) != EXIT_SUCCESS)
      return status;

    close (outfile);

    if (listing) {
      list_segments (all_seg);
      list_labels (all_sym, all_seg);
    }

    return status ? EXIT_FAILURE : EXIT_SUCCESS;
}
