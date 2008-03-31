%{
#include <stdio.h>
#include "clowndev.h"
#include "clink.h"
int yyerror (char *s);
int codelimit;
int yylex();
%}

%union {
    char *s;
    int i;
    struct {
	int size;
	int escapes;
	Dword *bin;
    } code;
    struct Segment seg;
    struct Label sym;
};

%token T_NAME
%token T_SEGMENT
%token T_SEGMENTS
%token T_DEFINED
%token T_GLOBAL
%token T_TYPE
%token T_SIZE
%token T_SYMBOLS
%token T_SYMBOL
%token T_BIN
%token T_OFFSET
%token T_ID
%token T_CLOFEXE
%token T_CODESTAR
%token <code> T_CODE
%token <s> T_STRING
%token <i> T_NUMBER

%type <seg> seg_attribs
%type <sym> sym_attribs
%type <i> bin_attribs

%start module
%%

module:		'<' T_CLOFEXE '>' sec_segments sec_symbols bins '<' '/' T_CLOFEXE '>'
	;

sec_segments:	'<' T_SEGMENTS '>' segments '<' '/' T_SEGMENTS '>'
	;

segments:	segments segment
	|	
	;

segment:	'<' T_SEGMENT seg_attribs '/' '>' { if (!newSegment (&($3))) YYABORT; }
	;

seg_attribs:	seg_attribs T_NAME '=' '"' T_STRING '"' { $$.name = $5; }
	|	seg_attribs T_NAME '=' '"' T_CODESTAR '"' { $$.name = NULL; }
	|	seg_attribs T_DEFINED '=' '"' T_NUMBER '"' { $$.defined = $5; }
	|	seg_attribs T_TYPE '=''"'  T_NUMBER '"' { $$.type = $5; }
	|	seg_attribs T_SIZE '=''"'  T_NUMBER '"' { $$.file_size = $5; }
	|	seg_attribs T_ID   '=' '"' T_NUMBER '"' { $$.id = $5; }
	|	{}
	;

sec_symbols:	'<' T_SYMBOLS '>' symbols '<' '/' T_SYMBOLS '>'
	;

symbols:	symbols symbol
	|	
	;

symbol:	        '<' T_SYMBOL sym_attribs '/' '>' { if (!newSymbol (&($3))) YYABORT; }
	;

sym_attribs:	sym_attribs T_NAME '=''"'  T_STRING '"' { $$.name = $5; }
	|	sym_attribs T_SEGMENT '=' '"' T_NUMBER '"' { $$.segment = $5; }
	|	sym_attribs T_OFFSET '=' '"' T_NUMBER '"' { $$.address = $5; }
	|	sym_attribs T_ID '=' '"' T_NUMBER '"' { $$.id = $5; }
	|	sym_attribs T_DEFINED '=' '"'  T_NUMBER '"' { $$.defined = $5; }
	|	sym_attribs T_GLOBAL  '=' '"'  T_NUMBER '"' { $$.export = $5; }
	|	{}
	;

bins:		bins bin
	| 
	;

bin:	'<' 	T_BIN bin_attribs { 
                    codelimit = (modules[current_module].st.segments[$3].file_size) * sizeof (Dword); 
		} '>' T_CODE '<' '/' T_BIN '>' { 
		    modules[current_module].st.segments[$3].image_size = $6.size; 
		    modules[current_module].st.segments[$3].escapes = $6.escapes; 
		    modules[current_module].st.segments[$3].image = $6.bin; 
		}
	;

bin_attribs:	T_SEGMENT '=' '"' T_NUMBER '"' { $$ = $4; }
	;

%%

int yyerror (char *s)
{
    fprintf (stderr, "%s\n", s);
    return 0;
}

