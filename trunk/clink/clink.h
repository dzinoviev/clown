#ifndef CLINK_H
#define CLINK_H 1

#include "clowndev.h"

struct Module {
    char *name;
    struct SegmentTable st;
    struct LabelTable lt;
    int nsegments;
    Dword **segments;
};

extern struct Module *modules;
extern int current_module;

int get_options (int argc, char *argv[], char **object,
		 char ***source, int *nsources, int *ecode);
Dword *build_expressions (Dword* code, int codesize, int *truesize, int *escapes);
int newSegment (struct Segment *s);
int newSymbol (struct Label *s);

extern int listing;
extern int silent;
extern Clof_Type module_type;
#endif
