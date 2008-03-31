#ifndef CAS_H
#define CAS_H

#include <stdio.h>
#include "clowndev.h"

/* Some options */
extern char *cpp_options;
extern Clof_Type module_type;

/* The current position */
extern char source[];
extern int listing;
extern int line_no;
extern FILE *yyin, *debugfile;
extern Dword offset;
extern int current_segment;
extern int current_overhead;
extern struct SegmentTable segments;

struct labelDef {
    int global;
    int align8;
};

int begin_segment (int type, char *name);
void component_error (const char *msg, char *detail);
void end_segment (int segment, Dword size);
int add_label (char *label, int segment, Dword offset, int global, int align8);
int mark_export_label (char *label);
int lookup_segment (char *name);
int use_label (char *label, int segment);
void mark_near_label (int i);
int yyerror (char *s);
int yyparse (void);
int yylex (void);
int insert_into_export (char *s);
void emit (Dword i);
void emit_escape (Dword i);
int get_options (int argc, char *argv[], char **object, char source[], 
		 int *ecode);
int parse_and_assembly (FILE *infile, int outfile);
int copy_code_ram (int outfile, 
		   struct SegmentTable *segments,
		   struct LabelTable *labels);



#endif
