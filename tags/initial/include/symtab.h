#ifndef SYMTAB_H
#define SYMTAB_H 1

#include <stdlib.h>
#include <string.h>
#ifdef __STRICT_ANSI__
char *strdup(const char *s);
#endif
#include "isa.h"

typedef struct {
    int  size;
    int  type;
} ST_Segment; 

typedef struct {
    int   segment;
    Bit   align8;
    Bit   global;
    Bit   near;		/* must be defined where used */
} ST_Label;

typedef struct {
    char   *name;
    Bit    defined;
    Dword  offset;		/* label address and segment file offset */
    union {
	ST_Segment seg;
	ST_Label lab;
    } body;
    unsigned lineno;
    int        index;
    /* This field is only used by the linker */
    int    new_index;
} ST_Symbol;
	
typedef struct {
    ST_Symbol *symbols;
    unsigned size;
    unsigned used;
} ST_Bucket;

#define N_BUCKETS 17
typedef struct {
    int       size;
    int       used;
    ST_Symbol **index;
    ST_Bucket buckets[N_BUCKETS];
} ST_Table;

extern ST_Table  symbol_table;
extern ST_Table segment_table;

void       ST_Init      (ST_Table *table);
ST_Symbol *ST_Lookup    (ST_Table *table, char *name);
ST_Symbol *ST_Fetch     (ST_Table *table, int i);
ST_Symbol *ST_Insert    (ST_Table *table, char *name);
#endif /* SYMTAB_H */
