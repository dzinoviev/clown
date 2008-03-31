#ifndef CLOWNDEV_H
#define CLOWNDEV_H 1

#include <setjmp.h>
#include "isa.h"
#include "symtab.h"

typedef enum {CLOF_BIN = 0, CLOF_EXE = ('X'<<24), CLOF_UNKNOWN} Clof_Type;

#define DEFAULT_SEGMENT 0
#define NOT_FOUND (-1)
#define IMAGE_CHUNK 1024

struct Segment {
    char *name;
    int  file_size;
    enum {SEG_DEFAULT, SEG_CODE, SEG_DATA} type;
    Bit   defined;

    Dword *image;
    int image_size;
    int image_extent;

    /* This field is only used by the linker */
    int  id;
    int  in_use;
    int  escapes;
    int  new_index;
    int  module;
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
    /* This field is only used by the linker */
    int  id;
    int  in_use;
    int  new_index;
};

struct LabelTable {
    Dword  size;
    struct Label *labels;
};

typedef enum {DUMMY = 0, CONSTANT, EXPRESSION, LABEL} EType;

typedef struct _Expression {
    EType type;
    union {
	int constant;
	int label;
	struct {
	    int operation;
	    struct _Expression *left, *right;
	} expression;
    } detail;
} Expression;

#define FIX_SEGMENT      ((Dword)0xFF000001)
#define FIX_ADISPLACEMENT ((Dword)0xFF000002)
#define FIX_RDISPLACEMENT ((Dword)0xFF000004)
#define FIX_EXPRESSION   ((Dword)0xFF000008)

static const char clof_header[4] = {'#', 'C', 'O', 'F'};
extern jmp_buf   failure;
void secure_write (int file, void *addr, int size);
void secure_string (int file, char *string);
void write_header (int outfile, struct SegmentTable *segments, struct LabelTable *labels);
#define FIRST_FRAGMENT 1
#define LAST_FRAGMENT 2
int save_segment (int outfile, int id, struct Segment *seg, struct LabelTable *labels, int fragment);

Expression *newConstant (int constant);
Expression *newLabel (int label);
Expression *newExpression (int op, Expression *left, Expression *right);
Expression *do_math (int op, Expression *left, Expression *right);
int try_to_evaluate (Expression *e, struct LabelTable *labels, Dword *value, Dword *segment);
int expression_overhead (Expression *e);
void list_segments (struct SegmentTable st);
void list_labels (struct LabelTable syms, struct SegmentTable segs);
#endif
