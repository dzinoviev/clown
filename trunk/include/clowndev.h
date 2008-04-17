#ifndef CLOWNDEV_H
#define CLOWNDEV_H 1


#include <setjmp.h>
#include "isa.h"
#include "symtab.h"

typedef enum {CLOF_BIN, CLOF_EXE, CLOF_UNKNOWN} Clof_Type;

#define DEFAULT_SEGMENT 0
#define DEFAULT_SEGMENT_NAME "code*"
#define NOT_FOUND (-1)
#define IMAGE_CHUNK 1024

struct MyDebug {
    int nfiles;
    struct DebugFile *files;
};

struct DebugInfo {
  Uword offset;
  int line;
};

struct DebugFile {
  char *file;
  int nlines;
  int nlines_inuse;
  struct DebugInfo *flines;
};

struct Segment {
    char *name;
    int  file_size;
    enum {SEG_DEFAULT, SEG_CODE, SEG_DATA, SEG_CONST} type;
    Bit   defined;
    Bit   global;

    Dword *image;
    int image_size;
    int image_extent;

    /* Debug info */
  int nfiles;
  struct DebugFile *files;

    /* This field is used only by the linker */
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

struct Module {
    char *name;
    Clof_Type type;
    int offset;			/* in RAM */
    struct SegmentTable st;
    struct LabelTable lt;
};

typedef enum {DUMMY = 0, CONSTANT, EXPRESSION, LABEL, SELECTOR} EType;

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

extern jmp_buf   failure;
extern int current_overhead;
extern int link_overhead;
extern struct Module *modules;
extern int current_module;
extern int line_no;
extern char **source;
extern int codelimit;
extern FILE *rdin;
extern int escapes;
extern int debug;
extern int listing;
extern Clof_Type module_type;

#define FIRST_FRAGMENT 1
#define LAST_FRAGMENT 2

enum { C_LL = 'A', C_GG = 'B', C_UNARY_MIN = 'C'};
void secure_write (int file, void *addr, int size);
void secure_string (int file, char *string);
void *safe_malloc (size_t size);
void *safe_realloc (void *ptr, size_t size);

void write_header (int outfile, struct SegmentTable *segments, struct LabelTable *labels);
void write_trailer (int outfile);
int save_segment (int outfile, int id, struct SegmentTable *st, struct LabelTable *labels, int fragment);

Expression *newConstant (int constant);
Expression *newLabel (int label);
Expression *newSelector (Selector s);
Expression *newExpression (int op, Expression *left, Expression *right);
Expression *do_math (int op, Expression *left, Expression *right);
int try_to_evaluate (Expression *e, struct LabelTable *labels,
		     struct SegmentTable *st, Dword *value, Dword *segment);
int expression_overhead (Expression *e);
void list_segments (struct SegmentTable st);
void list_labels (struct LabelTable syms, struct SegmentTable segs);
Dword *build_expressions (Dword* code, int codesize, int *truesize, int *escapes);
int rdparse (void);
void init_module (struct Module *m, char *fname);
void component_error (const char *name, const char *msg, char *detail);
#endif
