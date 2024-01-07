#ifndef CAS_H
#define CAS_H

#include <stdio.h>
#include "clowndev.h"

/* Some options */
extern char *cpp_options;

/* The current position */
extern FILE *yyin, *debugfile;
extern int line_no;
extern int global_offset;
extern Uword offset;
extern int current_segment;
extern struct SegmentTable segments;

struct labelDef {
    int global;
    int align8;
};

int begin_segment (int type, char *name);
void end_segment (Dword size);
int add_label (char *label, int segment, Uword offset, int global, int align8);
int mark_export_label (char *label);
int lookup_segment (char *name);
int use_label (char *label, int segment);
void mark_near_label (int i);
int yyerror (char *s);
//int yyparse (void);
int yylex (void);
int insert_into_export (char *s);
void emit (Dword i);
void store (Dword i);
int get_options (int argc, char *argv[], char **object, char **source, 
		 int *ecode);
int parse_and_assembly (FILE *infile, int outfile);
int copy_code_ram (int outfile, 
		   struct SegmentTable *segments,
		   struct LabelTable *labels);



#endif
