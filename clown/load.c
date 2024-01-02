#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include "registers.h"
#include "clowndev.h"

int firstcode = -1;
int listing = 0;/* not really needed */
Clof_Type module_type = CLOF_UNKNOWN;/* not really needed */
struct Clown_Segment_Descriptor init_ldt_descr = {0, 0, 0};

void component_error(__attribute__((unused)) const char *name,
		     __attribute__((unused)) const char *msg,
		     __attribute__((unused)) char *detail)
{
  /* not really needed */
}

static int load_seg (char *fname, struct Segment *current_seg, int seg_no,
		     Dword offset, struct Clown_Segment_Descriptor *my_ldt)
{
  int size = current_seg->image_size;
  
  if (size > CLOWN_MEMORY_SIZE - offset) {
    size = CLOWN_MEMORY_SIZE - offset;
    fprintf(stderr, "--> Warning: image file %s is larger than Clown memory\n", 
	    fname);
  }
  memcpy(&CLOWN_MEMORY[offset], current_seg->image,
	 size * sizeof(Dword));
  
  if (!current_seg->files) {
    if (!silent)
      fprintf(stderr, "--> Segment %s in file %s has no debug info.\n", 
	      current_seg->name, fname);
  } else {
    for (int i = 0; i < current_seg->nfiles; i++) {
      struct stat src, clef;
      struct DebugFile *df = &current_seg->files[i];
      
      if (-1 == stat(fname, &clef)) {
	perror(fname);
	return 0;
      }
      if (-1 == stat(df->file, &src)) {
	perror(df->file);
	return 0;
      }
      if (!silent && src.st_mtime >= clef.st_mtime)
	fprintf(stderr, 
		"--> Source file %s is \n"
		"--> newer than %s.\n",
		df->file, fname);
    }
  }
  
  if (seg_no > DEFAULT_SEGMENT) {
    my_ldt[seg_no].flags = 0;
    SF_SET_PRESENT(my_ldt[seg_no]);
    
    int perm = CAN_READ ;
    switch (current_seg->type) {
    case SEG_DEFAULT:
      perm |= (CAN_WRITE | CAN_EXEC);
      break;
    case SEG_CODE:
      perm |= CAN_EXEC;
      if (-1 == firstcode)
	firstcode = seg_no;
      break;
    case SEG_DATA:
      perm |= CAN_WRITE;
      break;
    case SEG_CONST:
      break;
    }

    SF_SET_PERM(my_ldt[seg_no], perm);
    /* Other descriptor flags can be set up here */
    my_ldt[seg_no].base = offset;
    my_ldt[seg_no].limit = size;
    
    /* Record the actuall segment base */
    current_seg->base = my_ldt[seg_no].base;
  }

  return size;
}

static int parse_code_file(char *fname)
{
  yyin = fopen(fname, "r");
  if (!yyin) {
    perror(fname);
    return 0; 
  }
  yyrestart(yyin);
  line_no = 1;
  source = &fname;
  if (yyparse()) {
    fclose(yyin);
    return 0;
  }

  fclose(yyin);
  return 1;
}

int load_memory(char *fname, Dword offset)
{
  modules = safe_realloc(modules, sizeof(struct Module) * (current_module + 1));
  struct Module *mod = &modules[current_module];
  init_module(mod, fname);

  /* Parse the file */
  if (!parse_code_file(fname))
    return 0;

  if (mod->type != CLOF_BIN) {
    fprintf(stderr, "--> %s is not a CLEF file\n", fname);
    return 0;
  }

  struct Clown_Segment_Descriptor *my_ldt =
    safe_malloc(mod->st.size * sizeof(struct Clown_Segment_Descriptor));
  my_ldt[0].base = 0;
  my_ldt[0].limit = CLOWN_MEMORY_SIZE;
  my_ldt[0].flags = 0;
  SF_SET_PRESENT(my_ldt[0]);
  SF_SET_PERM(my_ldt[0], CAN_READ | CAN_WRITE | CAN_EXEC);

  int seg_count = 0;
  if (mod->st.size > 1)
    seg_count = 1;

  int module_offset = 0, size = 0;
  for (int seg_no = seg_count;  seg_no < mod->st.size;  seg_no++) {
    struct Segment *current_seg = &(mod->st.segments[seg_no]);
    if (!current_seg->in_use)
      continue;
    size = load_seg (fname, current_seg, seg_no, offset + module_offset,
		     my_ldt);
    module_offset += size;
  }

  // Load the LDT into the RAM immediately after the last segment from the file
  if (mod->st.size > 1) {
    struct Clown_Segment_Descriptor ldt_descr;
    int ldt_size = mod->st.size * sizeof(struct Clown_Segment_Descriptor);
    int new_ldt_address, ldt_address = offset + module_offset;

    new_ldt_address = (ldt_address / LGDT_ENTRY_SIZE) * LGDT_ENTRY_SIZE;
    if (new_ldt_address < ldt_address) {
      new_ldt_address += LGDT_ENTRY_SIZE;
      size += LGDT_ENTRY_SIZE;
    }
    memcpy(&CLOWN_MEMORY[new_ldt_address], my_ldt, ldt_size);
    size += ldt_size;
    ldt_descr.flags = 0;
    SF_SET_PRESENT(ldt_descr);
    ldt_descr.base = new_ldt_address;
    ldt_descr.limit = mod->st.size * LGDT_ENTRY_SIZE;

    if (SF_PRESENT(init_ldt_descr))
      fprintf(stderr,
	      "--> Warning: more than one LDT has been loaded.\n"
	      "--> Warning: only the most recent is saved.");
    init_ldt_descr = ldt_descr;
  }

  current_module++;
  return size + module_offset;
}
