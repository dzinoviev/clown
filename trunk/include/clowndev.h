#ifndef CLOWNDEV_H
#define CLOWNDEV_H 1

#include <setjmp.h>
#include "isa.h"
#include "symtab.h"

typedef enum {CLOF_BIN = 0, CLOF_EXE = ('X'<<24), CLOF_UNKNOWN} Clof_Type;
#define INCOMPLETE 'I'

struct Segment {
    char *name;
    int  file_offset;
    int  file_size;
    enum {SEG_CODE, SEG_DOOR, SEG_DATA} type;
    Bit   defined;
    /* This field is only used by the linker */
    int  new_index;
}; 

struct SegmentTable {
    Dword          size;
    struct Segment *segments;
};

struct Label {
    char  *name;
    int   segment;
    Dword address;
    Bit   export;
    Bit   defined;
    Bit   near;		/* must be defined here */
    Bit   intersegment;
    /* This field is only used by the linker */
    int  new_index;
    Bit   interrupt;
};

struct LabelTable {
    Dword  size;
    struct Label *labels;
};

#define FIX_DISPLACEMENT ((Dword)0xFF000000)
#define FIX_ADDRESS_LOC  ((Dword)0xFF000001)
#define FIX_SEGMENT      ((Dword)0xFF000002)
#define FIX_ADDRESS_GLOB ((Dword)0xFF000003)

static const char clof_header[4] = {'#', 'C', 'O', 'F'};
extern jmp_buf   failure;
void list_segment (struct Segment s);
void list_label (struct Label l, struct Segment *seglist);
void secure_write (int file, void *addr, int size);
void secure_string (int file, char *string);
void write_header (int outfile, Bit has_unreferenced,
		   struct SegmentTable *segments,
		   int interface,
		   struct LabelTable *labels);
void component_error (const char *msg, char *detail);
int copy_code (int scratch, int outfile, struct SegmentTable *segments,
	       struct LabelTable *labels);
#endif
