#include <config.h>
#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif
#ifdef HAVE_LINUX_LIMITS_H
#include <linux/limits.h>
#endif

#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include "clink.h"

int listing = 0;
int silent = 0;
Clof_Type module_type = CLOF_UNKNOWN;
int current_segment = DEFAULT_SEGMENT;

static struct SegmentTable all_seg = {0, NULL};
static struct LabelTable all_sym = {0, NULL};

void component_error(const char *name, const char *msg, char *detail)
{
  if(!silent)
    fprintf(stderr, "clink: %s: %s: %s\n", name, msg, detail);
}

static int new_modules(int n_modules, char **name)
{
  modules = safe_malloc(sizeof(struct Module) * n_modules);
  for(int i = 0; i < n_modules; i++)
      init_module(&modules[i], name[i]);

  return 1;
}

static int unify_exe_segments(int n_modules)
{
  int loc = DEFAULT_SEGMENT + 1;

  /* Assemble explicitly defined segments */
  for(int i = 0; i < n_modules; i++) {
    modules[i].st.segments[DEFAULT_SEGMENT].in_use = 0;
    for(int j = DEFAULT_SEGMENT + 1; j < modules[i].st.size; j++) {
      struct Segment *seg = &modules[i].st.segments[j];
      if(!seg->in_use || !seg->defined)
	continue;
      
      /* Weed out duplicates */
      for(int k = DEFAULT_SEGMENT + 1; k < loc; k++) {
	if(!strcmp(seg->name, all_seg.segments[k].name)) {
	  component_error(modules[i].name, "duplicate segment", 
			   modules[i].st.segments[j].name);
	  return -1;
	}
      }
      /* Insert a segment into the compound table */
      seg->new_loc = loc;
      all_seg.segments[loc++] = modules[i].st.segments[j];
    }
  }

  /* Identify undefined segments */
  for(int i = 0; i < n_modules; i++) {
    for(int j = DEFAULT_SEGMENT + 1; j < modules[i].st.size; j++) {
      struct Segment *seg = &modules[i].st.segments[j];
      if(seg->in_use && !seg->defined) {
	int k;
	for(k = DEFAULT_SEGMENT + 1; k < loc; k++) {
	  if(! strcmp(all_seg.segments[k].name, seg->name)) {
	    seg->new_loc = k;
	    break;
	  }
	}
	if(k == loc) { /* not found! */
	  if(module_type == CLOF_BIN) {
	    component_error(modules[i].name,
			     "unresolved segment in a CLE file", 
			     seg->name);
	    return -1;
	  } else {
	    component_error(modules[i].name, "warning: unresolved segment", 
			     seg->name);
	    seg->new_loc = loc;
	    all_seg.segments[loc++] = modules[i].st.segments[j];
	  }
	}
      }
    }
  }

  return loc;
}

static int unify_bin_segments(int n_modules)
{
  struct Segment *dest_seg = &all_seg.segments[DEFAULT_SEGMENT];
  struct Segment *src_seg = &modules[0].st.segments[DEFAULT_SEGMENT];

  assert(src_seg->defined);
  assert(!src_seg->name);
  assert(src_seg->new_loc == DEFAULT_SEGMENT);

  src_seg->name = DEFAULT_SEGMENT_NAME;
  *dest_seg = *src_seg;

  int loc = DEFAULT_SEGMENT + 1;    
  int offset = src_seg->code_size;// src_seg->image_size - src_seg->escapes;

  /*  for(int i = 0; i < n_modules; i++) {
    printf(")))) %d %d %d\n", i,
	   modules[i].st.segments[DEFAULT_SEGMENT].image_size,
 	   modules[i].st.segments[DEFAULT_SEGMENT].code_size);
	   }*/
   
  // collect the pieces 
  for(int i = 1; i < n_modules; i++) {
    struct Segment *this_seg = &modules[i].st.segments[DEFAULT_SEGMENT];
    int new_nfiles;
    assert(modules[i].st.size == 1);

    this_seg->name = DEFAULT_SEGMENT_NAME;
    dest_seg->image_size += this_seg->image_size;
    dest_seg->code_size += this_seg->code_size;
	    
    for(int k = 0; k < modules[i].lt.size; k++) {
      struct Label *label = &modules[i].lt.labels[k];
      assert(label->segment == DEFAULT_SEGMENT);
      if(label->in_use && label->defined) {
	label->address += offset;
      }
    }
	    
    new_nfiles = src_seg->nfiles + this_seg->nfiles;
    src_seg->files = safe_realloc(src_seg->files,
				   sizeof(struct DebugFile) * new_nfiles);
    for(int k = 0, j = src_seg->nfiles; k < this_seg->nfiles; k++, j++) {
      int l;
      src_seg->files[j] = this_seg->files[k];
      for(l = 0; l < this_seg->files[k].nlines_inuse; l++)
	this_seg->files[k].flines[l].offset += offset;
    }
    src_seg->nfiles = new_nfiles;
    offset += this_seg->code_size; //this_seg->image_size - this_seg->escapes;
    //    src_seg->link_overhead += this_seg->link_overhead;
  }
  
  //  printf("Offset: %d\n", offset);
  dest_seg->code_size = offset;
  dest_seg->files = src_seg->files;
  dest_seg->nfiles = src_seg->nfiles;
  //  dest_seg->link_overhead = src_seg->link_overhead;

  return loc;
}

static Clof_Type mix_type = CLOF_UNKNOWN;
static int unify_segments(int n_modules)
{
  int loc = 0;
  Clof_Type module_mix_type = CLOF_UNKNOWN;

  all_seg.segments = safe_malloc(all_seg.size * sizeof(struct Segment));

  /* What kind of mixture is this? */    
  for(int i = 0; i < n_modules; i++) {
    if(module_mix_type == CLOF_UNKNOWN)
      module_mix_type = modules[i].type;
    else if(modules[i].type != module_mix_type) {
      component_error(modules[i].name, "CLOF and CLEF modules mixed",
		       modules[i].name);
      return EXIT_FAILURE;
    }

    for(int j = 0; j < modules[i].st.size; j++) {
      struct Segment *seg = &modules[i].st.segments[j];
      if(seg->in_use && seg->defined) {
	if(seg->name) {
	  if(mix_type != CLOF_BIN)
	    mix_type = CLOF_EXE;
	  else {
	    component_error(modules[i].name,
			     "named and anonymous segments mixed", 
			     seg->name);
	    return EXIT_FAILURE;
	  }
	} else {
	  if(mix_type != CLOF_EXE)
	    mix_type = CLOF_BIN;
	  else {
	    component_error(modules[i].name,
			     "named and anonymous segments mixed",
			     DEFAULT_SEGMENT_NAME);
	    return EXIT_FAILURE;
	  }
	}
      }
    }
  }

  if(mix_type == CLOF_BIN) {
    loc = unify_bin_segments(n_modules);
  } else {
    if(-1 == (loc = unify_exe_segments(n_modules)))
      return EXIT_FAILURE;
  }

  all_seg.size = loc;
  all_seg.segments = realloc(all_seg.segments, 
			      all_seg.size * sizeof(struct Segment));
    
  return EXIT_SUCCESS;
}

static int save_segments(int outfile, int n_modules) 
{
  int status = 1;
  /* The header */
  write_header(outfile, &all_seg, &all_sym);

  for(int mod = 0; mod < n_modules; mod++) {
    for(int i = 0; i < modules[mod].st.size; i++) { 
      int fragment = 0;
      struct Segment *seg = &modules[mod].st.segments[i];
      if(!seg->defined || !seg->in_use)
	continue;
      if(i == DEFAULT_SEGMENT) {
	if(!mod)
	  fragment |= FIRST_FRAGMENT;
	if(mod == n_modules - 1)
	  fragment |= LAST_FRAGMENT;
      } else {
	fragment = (FIRST_FRAGMENT | LAST_FRAGMENT);
      }
      status &= save_segment(outfile, i, &modules[mod].st, &modules[mod].lt,
			     fragment);
    }
  }

  /* The trailer */
  write_trailer(outfile);
  return !status;
}

/* Assemble explicitly defined symbols */
static int assemble_labels(int n_modules)
{
  int loc = 0;

  for(int i = 0; i < n_modules; i++) {
    for(int j = 0; j < modules[i].lt.size; j++) {
      struct Label *label = &modules[i].lt.labels[j];
      if(!label->defined || !label->in_use)
	continue;
      
      if(modules[i].lt.labels[j].export) {
	// Weed out duplicates 
	for(int k = 0; k < loc; k++) {
	  if(!strcmp(label->name, all_sym.labels[k].name)) {
	    component_error(modules[i].name, "duplicate symbol", label->name);
	    return -1;
	  }
	}
      }
      
      if(label->segment != DEFAULT_SEGMENT) {
	// Update segment reference 
	label->segment = modules[i].st.segments[label->segment].new_loc;
      }
      
      // Insert the symbol into the compound table 
      label->new_loc = loc;
      all_sym.labels[loc] = *label;
      
      loc++;
    }
  }
  return loc;
}

// Identify undefined symbols 
static void id_undefined_symbols(int n_modules, int loc)
{
  for(int i = 0; i < n_modules; i++) {
    for(int j = 0; j < modules[i].lt.size; j++) {
      struct Label *label = &modules[i].lt.labels[j];
      if(label->in_use && !label->defined) {
	for(int k = 0; k < loc; k++) {
	  if(all_sym.labels[k].export
	      && !strcmp(all_sym.labels[k].name, label->name)) {
	    label->new_loc = k;
	    label->defined = 1;
	    label->address = all_sym.labels[k].address;
	    break;
	  }
	}
      }
    }
  }
}

// Locate unresolved symbols 
static int locate_unresolved_symbols(int n_modules, int loc)
{
  for(int i = 0; i < n_modules; i++) {
    for(int j = 0; j < modules[i].lt.size; j++) {
      struct Label *label = &modules[i].lt.labels[j];
      if(label->in_use && !label->defined) {
	if(module_type == CLOF_BIN) {
	  component_error(modules[i].name, "unresolved symbol in a CLE module",
			  label->name);
	  return -1;
	} else {
	  component_error(modules[i].name, "warning: unresolved symbol",
			  label->name);
	  label->new_loc = loc;
	  all_sym.labels[loc] = *label;
	  loc++;
	}
      }
    }
  }
  return loc;
}

static int unify_symbols(int n_modules)
{
  all_sym.labels = safe_malloc(all_sym.size * sizeof(struct Label));

  // Assemble explicitly defined symbols 
  int loc = assemble_labels(n_modules);
  if(-1 == loc)
    return EXIT_FAILURE;
  
  // Identify undefined symbols 
  id_undefined_symbols(n_modules, loc);

  // Locate unresolved symbols
  all_sym.size = locate_unresolved_symbols(n_modules, loc);
  if(-1 == all_sym.size)
    return EXIT_FAILURE;
  
  all_sym.labels = realloc(all_sym.labels, 
			    all_sym.size * sizeof(struct Label));

  return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
  int n_modules = 0;
  int ecode = EXIT_SUCCESS;
  char *object = NULL;
  int outfile;

  if(!get_options(argc, argv, &object, &source, &n_modules, &ecode))
    return ecode;

  if(setjmp(failure))
    return EXIT_FAILURE;

  if(!new_modules(n_modules, source))
    return EXIT_FAILURE;
    
  int status = 0;
  //  link_overhead = 0;
  for(current_module = 0; current_module < n_modules; current_module++) {
    yyin = fopen(source[current_module], "r");
    if(!yyin) {
      perror(source[current_module]);
      return EXIT_FAILURE; 
    }
    yyrestart(yyin);
    line_no = 1;
    status |= yyparse();          
    fclose(yyin);
    
    all_sym.size += modules[current_module].lt.size;
    all_seg.size += modules[current_module].st.size;
  }

  if(status)
    return EXIT_FAILURE;

  if(unify_segments(n_modules) != EXIT_SUCCESS ||
     unify_symbols(n_modules)  != EXIT_SUCCESS)
    return EXIT_FAILURE;

  outfile = open(object, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
  if(outfile == -1) {
    perror(object);
    return EXIT_FAILURE;
  }

  if(save_segments(outfile, n_modules) != EXIT_SUCCESS)
    return EXIT_FAILURE;

  close(outfile);

  if(listing) {
    list_segments(all_seg);
    list_labels(all_sym, all_seg);
  }

  return EXIT_SUCCESS;
}
