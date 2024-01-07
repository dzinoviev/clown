%{
#include <stdio.h>
#include "clowndev.h"
  struct Module *modules = NULL;
  int current_module;
  char **source = NULL;
  void yyerror(char *s);
  int yylex(); 
  
  static void new_segment(struct Segment *s);
  static void new_symbol(struct Label *s);
%}

%union {
  char *s;
  int i;
  struct {
    int size;
    //	int escapes;
    Dword *bin;
  } code;
  struct Segment seg;
  struct Label sym;
  struct MyDebug di;
  struct DebugFile df;
  struct DebugInfo dl;
};

%token T_BIN
%token T_CLOFEXE
%token T_CLEFEXE
%token T_CODESTAR
%token T_DEFINED
%token T_FILE
%token T_GLOBAL
%token T_ID
%token T_LINE
%token T_LINENO
%token T_NAME
%token T_OFFSET
%token T_SEGMENT
%token T_SEGMENTS
%token T_SIZE
%token T_SYMBOL
%token T_SYMBOLS
%token T_ESYMBOLS
%token T_TYPE
%token T_LS
%token T_SG
%token T_L
%token T_G

%token <code> T_CODE
%token <s> T_STRING
%token <i> T_NUMBER

%type <seg> seg_attribs
%type <sym> sym_attribs
%type <i> bin_segno bin_segsize
%type <i> exetype
%type <di> debuginfo
%type <df> debuglines debugfile
%type <dl> debugline

%start module
%%

module:		T_L exetype T_G sec_segments sec_symbols bins T_LS exetype T_G {
  if ($2 != $8) {
    yyerror("opening tag does not match closing tag");
    YYABORT;
  }
  modules[current_module].type = $2;
 }
;

exetype:	T_CLOFEXE { $$ = CLOF_EXE; }
|	T_CLEFEXE { $$ = CLOF_BIN; }
;

sec_segments:	T_L T_SEGMENTS T_G segments T_LS T_SEGMENTS T_G
;

segments:	segments segment
|	
;

segment:	T_L T_SEGMENT seg_attribs T_G { new_segment(&($3)); }
debuginfo T_LS T_SEGMENT T_G { 
  modules[current_module].st.segments[$3.id].nfiles = $6.nfiles;
  modules[current_module].st.segments[$3.id].files = $6.files;
}
;

seg_attribs:	seg_attribs T_NAME    '=' '"' T_STRING   '"' { $$.name = $5; }
|	seg_attribs T_NAME    '=' '"' T_CODESTAR '"' { $$.name = NULL; }
|	seg_attribs T_DEFINED '=' oq  T_NUMBER   oq  { $$.defined = $5; }
|	seg_attribs T_TYPE    '=' oq  T_NUMBER   oq  { $$.type = $5; }
|	seg_attribs T_ID      '=' oq  T_NUMBER   oq  { $$.id = $5; }
|	{}
;

sec_symbols:	T_SYMBOLS T_G symbols T_ESYMBOLS T_G
|	
;

symbols:	symbols symbol
|	
;

symbol:	        T_L T_SYMBOL sym_attribs T_SG { new_symbol(&($3)); }
;

sym_attribs:	sym_attribs T_NAME    '=' '"' T_STRING '"' { $$.name = $5;  }
|	sym_attribs T_SEGMENT '=' oq  T_NUMBER oq  { $$.segment = $5; }
|	sym_attribs T_OFFSET  '=' oq  T_NUMBER oq  { $$.address = $5; }
|	sym_attribs T_ID      '=' oq  T_NUMBER oq  { $$.id = $5;      }
|	sym_attribs T_DEFINED '=' oq  T_NUMBER oq  { $$.defined = $5; }
|	sym_attribs T_GLOBAL  '=' oq  T_NUMBER oq  { $$.export = $5;  }
|	{}
;

bins:		bins bin
|
;

bin:		T_L T_BIN bin_segno bin_segsize T_G T_CODE T_LS T_BIN T_G { 
  modules[current_module].st.segments[$3].code_size = $4; 
  modules[current_module].st.segments[$3].image_size = $6.size; 
  //  modules[current_module].st.segments[$3].escapes = $6.escapes; 
  modules[current_module].st.segments[$3].image = $6.bin; 
  //  modules[current_module].st.segments[$3].link_overhead = link_overhead; 
}
;

bin_segno:	T_SEGMENT '=' oq T_NUMBER oq { $$ = $4; } ;

bin_segsize:	T_SIZE '=' oq T_NUMBER oq { $$ = $4; } ;

debuginfo: 	debuginfo debugfile { 
  $$.nfiles = $1.nfiles + 1; 
  $$.files = safe_realloc($1.files, sizeof(struct DebugFile) * $$.nfiles); 
  $$.files[$$.nfiles - 1] = $2;
}
| { $$.nfiles = 0; $$.files = NULL; }
;

debugfile:	T_L T_FILE T_NAME '=' '"' T_STRING '"' T_G debuglines T_LS T_FILE T_G
{ $$ = $9; $$.file = $6; }
;

debuglines:	debuglines debugline {
  if ($$.nlines_inuse >= $$.nlines) {
    $$.nlines = $$.nlines + IMAGE_CHUNK;
    $$.flines = safe_realloc($$.flines, $$.nlines * sizeof(struct DebugInfo));
  }
  $$.flines[$$.nlines_inuse] = $2;
  $$.nlines_inuse = $$.nlines_inuse + 1;
}
| {$$.nlines = $$.nlines_inuse = 0; $$.flines = NULL; }
;

debugline:	T_L T_LINE T_OFFSET '=' oq T_NUMBER oq T_LINENO '=' oq T_NUMBER oq T_SG
{ $$.offset = $6; $$.line = $11; }
;

oq: '"'	| ;			/*optional quotation mark */

%%

static void new_segment(struct Segment *s)
{  
  struct SegmentTable *st = &modules[current_module].st; 
  if (st->size <= s->id) {
    st->segments = safe_realloc(st->segments, 
				(s->id + 1) * sizeof(struct Segment));
    st->size = s->id + 1;
  }
  s->in_use = 1;
  st->segments[s->id] = *s;
  st->segments[s->id].module = current_module;
}

static void new_symbol(struct Label *s)
{
  struct LabelTable *lt = &modules[current_module].lt;
  if (lt->size <= s->id) {
    lt->labels = safe_realloc(lt->labels, 
			      (s->id + 1) * sizeof(struct Label));
    lt->size = s->id + 1;
  }
  s->in_use = 1;
  lt->labels[s->id] = *s;
}

void yyerror(char *s)
{
  fprintf(stderr, "%s:%d: %s\n", source[current_module], line_no, s);
}

int yywrap(void)
{
  return 1;
}
