#ifndef CLOWNDEV_H
#define CLOWNDEV_H 1

#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#ifdef __STRICT_ANSI__
char *strdup(const char *s);
#endif

#include "isa.h"

#define COPYRIGHT "Copyright © 2004-2023 D. Zinoviev\n"\
  "This program is free software; you may redistribute it under the terms of\n"\
  "the GNU General Public License.  This program has absolutely no warranty.\n"

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
  enum {SEG_DEFAULT, SEG_CODE, SEG_DATA, SEG_CONST} type;
  Bit   defined;
  Bit   global;

  Dword *image;
  unsigned code_size;
  unsigned image_size;
  unsigned image_extent;

  /* Debug info */
  int nfiles;
  struct DebugFile *files;

  /* These fields are used only by the linker */
  int  id;
  int  in_use;
  int  escapes;
  int  new_loc;
  int  module;

  /* This field is used only by the loader/clown */
  int  base;		
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
    int  new_loc;
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

#define FIX_SEGMENT       ((Dword)0xFF000001)
#define FIX_ADISPLACEMENT ((Dword)0xFF000002)
#define FIX_RDISPLACEMENT ((Dword)0xFF000004)
#define FIX_EXPRESSION    ((Dword)0xFF000008)
#define FIX_NOTHING       ((Dword)0xFF000010)

extern jmp_buf   failure;
extern int link_overhead;
extern struct Module *modules;
extern int current_module;
extern int line_no;
extern char **source;
extern int codelimit;
extern FILE *yyin;
extern int debug;
extern int listing;
extern Clof_Type module_type;

#define FIRST_FRAGMENT 1
#define LAST_FRAGMENT 2

enum { C_LL = 'A', C_GG = 'B', C_UNARY_MIN = 'C'};
void safe_base64 (int file, Dword word);
void safe_string (int file, char *string);
void *safe_malloc (size_t size);
void *safe_realloc (void *ptr, size_t size);

void write_header (int outfile, struct SegmentTable *segments, struct LabelTable *labels);
void write_trailer (int outfile);
int save_segment (int outfile, int id, struct SegmentTable *st, struct LabelTable *labels, int fragment);

Expression *newConstant (Dword constant);
Expression *newLabel (int label);
Expression *newSelector (Selector s);
Expression *newExpression (int op, Expression *left, Expression *right);
Expression *do_math (int op, Expression *left, Expression *right);
int try_to_evaluate (Expression *e, struct LabelTable *labels,
		     struct SegmentTable *st, Dword *value, Dword *segment);
int expression_overhead (Expression *e);
void list_segments (struct SegmentTable st);
void list_labels (struct LabelTable syms, struct SegmentTable segs);
Dword *build_expressions (Dword* code, int codesize, int *truesize/*, int *escapes*/);
int yyparse (void);
void yyrestart (FILE*);
int base64_decode (char *orig, Dword *decoded);
void init_module (struct Module *m, char *fname);
void component_error (const char *name, const char *msg, char *detail);
#endif
