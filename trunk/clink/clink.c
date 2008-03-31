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

extern FILE *yyin;
int yyparse (void);
int listing = 0;
int silent = 0;
Clof_Type module_type = CLOF_UNKNOWN;
jmp_buf failure;
int current_segment = DEFAULT_SEGMENT;

static struct SegmentTable all_seg = {0, NULL};
static struct LabelTable all_sym = {0, NULL};
struct Module *modules = NULL;
int current_module;

void component_error (int modid, const char *msg, char *detail)
{
    if (!silent)
	fprintf (stderr, "clink: %s: %s: %s\n", modules[modid].name, msg, detail);
}

static int newModules (int n_modules, char **name)
{
  int i;
  modules = malloc (sizeof (struct Module) * n_modules);
  if (!modules) {
    perror ("realloc");
    return 0;
  }
  for (i = 0; i < n_modules; i++) {
    modules[i].name = name[i];
    modules[i].st.size = 0;
    modules[i].st.segments = NULL;
    modules[i].lt.size = 0;
    modules[i].lt.labels = NULL;
    modules[i].nsegments = 0;
    modules[i].segments = NULL;
  }
  return 1;
}

static int unify_segments (int n_modules)
{
  int i, j, k, index = 0, anon_segment = -1;
  Clof_Type mix_type = CLOF_UNKNOWN;

  all_seg.segments = malloc (all_seg.size * sizeof (struct Segment));
  if (!all_seg.segments) {
    perror ("malloc");
    longjmp (failure, 1);
  }

  /* What kind of mixture is this? */
  for (i = 0; i < n_modules; i++)
    for (j = 0; j < modules[i].st.size; j++) {
      if (modules[i].st.segments[j].in_use && modules[i].st.segments[j].defined) {
	if (modules[i].st.segments[j].name) {
	  if (mix_type != CLOF_BIN)
	    mix_type = CLOF_EXE;
	  else {
	    component_error (i, "named and anonymous segments mixed", modules[i].st.segments[j].name);
	    return EXIT_FAILURE;
	  }
	} else {
	  if (mix_type != CLOF_EXE) {
	    mix_type = CLOF_BIN;
	    if (anon_segment == -1)
	      anon_segment = i;
	  } else {
	    component_error (i, "named and anonymous segments mixed", "code*");
	    return EXIT_FAILURE;
	  }
	}
      }
    }

  if (mix_type == CLOF_BIN) {
    int offset = 0;
    assert (    modules[anon_segment].st.segments[DEFAULT_SEGMENT].defined 
	    && !modules[anon_segment].st.segments[DEFAULT_SEGMENT].name);

    all_seg.segments[DEFAULT_SEGMENT] = modules[anon_segment].st.segments[DEFAULT_SEGMENT];
    all_seg.segments[DEFAULT_SEGMENT].name = "code*";
    all_seg.segments[DEFAULT_SEGMENT].new_index = DEFAULT_SEGMENT;
    modules[anon_segment].st.segments[DEFAULT_SEGMENT].name = "code*";

    index = DEFAULT_SEGMENT + 1;
    
    offset = (modules[anon_segment].st.segments[DEFAULT_SEGMENT].image_size 
	      - modules[anon_segment].st.segments[DEFAULT_SEGMENT].escapes);

    /* collect the pieces */
    for (i = anon_segment + 1; i < n_modules; i++) {
      for (j = 0; j < modules[i].st.size; j++) {
	int new_size, k;

	all_seg.segments[DEFAULT_SEGMENT].file_size += modules[i].st.segments[DEFAULT_SEGMENT].file_size;
	modules[i].st.segments[DEFAULT_SEGMENT].name = "code*";
	
	new_size = all_seg.segments[DEFAULT_SEGMENT].image_size 
	  + modules[i].st.segments[DEFAULT_SEGMENT].image_size;	
	
	all_seg.segments[DEFAULT_SEGMENT].image_size = new_size;
	
	for (k = 0; k < modules[i].lt.size; k++) {
	  assert (modules[i].lt.labels[k].segment == j);
	  if (modules[i].lt.labels[k].in_use && modules[i].lt.labels[k].defined)
	      modules[i].lt.labels[k].address += offset;
	}
	offset += (modules[i].st.segments[DEFAULT_SEGMENT].image_size 
		   - modules[i].st.segments[DEFAULT_SEGMENT].escapes);
      }
    }
  } else {
    /* Assemble explicitly defined segments */
    for (i = 0; i < n_modules; i++)
      for (j = 0; j < modules[i].st.size; j++) {
	if (   !modules[i].st.segments[j].in_use 
	       || !modules[i].st.segments[j].defined)
	  continue;
	/* Weed out duplicates */
	for (k = 0; k < index; k++) {
	  if (!strcmp (modules[i].st.segments[j].name, all_seg.segments[k].name)) {
	    component_error (i, "duplicate segment", modules[i].st.segments[j].name);
	    return EXIT_FAILURE;
	  }
	}
	/* Insert a segment into the compound table */
	modules[i].st.segments[j].new_index = index;
	all_seg.segments[index] = modules[i].st.segments[j];
      
	index++;
      }

    /* Identify undefined segments */
    for (i = 0; i < n_modules; i++)
      for (j = 0; j < modules[i].st.size; j++) {
	if (   modules[i].st.segments[j].in_use 
	       && !modules[i].st.segments[j].defined) {
	  for (k = 0; k < index; k++) {
	    if (! strcmp (all_seg.segments[k].name, modules[i].st.segments[j].name)) {
	      modules[i].st.segments[j].new_index = k;
	      modules[i].st.segments[j].defined = 1;
	      break;
	    }
	  }
	}
      }

    /* Locate unresolved segments */
    for (i = 0; i < n_modules; i++)
      for (j = 0; j < modules[i].st.size; j++) {
	if     (modules[i].st.segments[j].in_use 
		&& !modules[i].st.segments[j].defined) {
	  if (module_type == CLOF_BIN) {
	    component_error (i, "unresolved segment in a CLE file", modules[i].st.segments[j].name);
	    return EXIT_FAILURE;
	  } else {
	    if (!silent)
	      component_error (i, "warning: unresolved segment", modules[i].st.segments[j].name);
	    modules[i].st.segments[j].new_index = index;
	    all_seg.segments[index] = modules[i].st.segments[j];
	    index++;
	  }
	}
      }
  }

  all_seg.size = index;
  all_seg.segments = realloc (all_seg.segments, 
			      all_seg.size * sizeof (struct Segment));
    
  return EXIT_SUCCESS;
}

static int unify_symbols (int n_modules)
{
  int i, j, k, index = 0;
  all_sym.labels = malloc (all_sym.size * sizeof (struct Label));
  if (!all_sym.labels) {
    perror ("malloc");
    longjmp (failure, 1);
  }

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
	    component_error (i, "duplicate symbol", modules[i].lt.labels[j].name);
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
	  component_error (i, "unresolved symbol in a CLE module", modules[i].lt.labels[j].name);
	  return EXIT_FAILURE;
	} else {
	  if (!silent)
	    component_error (i, "warning: unresolved symbol", modules[i].lt.labels[j].name);
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

static int save_segments (int outfile, int n_modules) 
{
    int status = 1, i, j;
    /* The header */
    if (module_type == CLOF_EXE)
	write_header (outfile, &all_seg, &all_sym);

    for (i = 0; i < n_modules; i++) {
	for (j = 0; j < modules[i].st.size; j++) {
	    int fragment = 0; 
	    if (j == DEFAULT_SEGMENT) {
		if (!i)
		    fragment |= FIRST_FRAGMENT;
		if (i == n_modules - 1)
		    fragment |= LAST_FRAGMENT;
	    } else {
		fragment = (FIRST_FRAGMENT | LAST_FRAGMENT);
	    }
	    status &= save_segment (outfile, i, &modules[i].st.segments[j], 
				    &modules[i].lt, fragment);
	}
    }

    /* The trailer */
    if (module_type == CLOF_EXE)
	secure_string (outfile, "</clof>\n");

    return !status;
}

int main (int argc, char *argv[])
{
    char **source = NULL;
    int n_modules = 0;
    int ecode = EXIT_SUCCESS;
    char *object = NULL;
    int status;
    int outfile;

    if (!get_options (argc, argv, &object, &source, &n_modules, &ecode))
	return ecode;

    if (setjmp (failure))
	return EXIT_FAILURE;

    if (!newModules (n_modules, source))
      return EXIT_FAILURE;

    for (current_module = 0; current_module < n_modules; current_module++) {
      yyin = fopen (source[current_module], "r");
      if (!yyin) {
	perror (source[current_module]);
	return EXIT_FAILURE; 
      }
      status =  yyparse ();
      fclose (yyin);
      all_sym.size += modules[current_module].lt.size;
      all_seg.size += modules[current_module].st.size;
    }

    if (status != EXIT_SUCCESS)
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
