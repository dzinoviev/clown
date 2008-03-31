%{
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
int fileno(FILE *stream);	/* for ANSI compatibility */
#include "clowndev.h"
#include "clink-yacc.tab.h"
#include "clink.h"
 int yyerror (char *s);

static int chararraysize, charcodesize;
extern int codelimit;
static char *charcode;
%}

%x CDATA
%%

"<![cdata[" { BEGIN(CDATA); chararraysize = 0; charcodesize = 0; charcode = NULL; }
<CDATA>(.|\n) { 
    if (charcodesize == chararraysize) {
	chararraysize += 1024;
	charcode = realloc (charcode, chararraysize * sizeof (Dword));
	if (!charcode) {
	    perror ("realloc");
	    return YY_NULL;
	}
    }
    charcode[charcodesize++] = yytext[0]; 
    if (charcodesize == codelimit) { 
	BEGIN(INITIAL); 
    }
}
"]]>" {
    assert (!(charcodesize % sizeof (Dword)));
    yylval.code.size = 0;
    yylval.code.escapes = 0;
    yylval.code.bin = build_expressions ((Dword*)charcode, charcodesize / sizeof (Dword), 
					 &yylval.code.size, &yylval.code.escapes);
    free (charcode);
    return T_CODE; 
}

[<>=/\"] { return yytext[0]; }
[0-9]+ { 
    yylval.i = atoi (yytext);
    return T_NUMBER; 
}
"segments" { return T_SEGMENTS; }
"segment" { return T_SEGMENT; }
"name" { return T_NAME; }
"defined" { return T_DEFINED; }
"global" { return T_GLOBAL; }
"type" { return T_TYPE; }
"size" { return T_SIZE; }
"symbols" { return T_SYMBOLS; }
"symbol" { return T_SYMBOL; }
"bin" { return T_BIN; }
"offset" { return T_OFFSET; }
"id" { return T_ID; }
"clof" { return T_CLOFEXE; }
"code*" { return T_CODESTAR; }
[_a-zA-Z][_a-zA-Z0-9]* { 
    yylval.s = malloc (strlen (yytext) + 1); 
    if (!yylval.s) {
	perror ("malloc"); 
	return YY_NULL; 
    } 
    strcpy (yylval.s, yytext); 
    return T_STRING; 
}
[ \t\n]+
. { fprintf (stderr, "%c %x ", yytext[0], yytext[0]); yyerror ("invalid character"); }
%%

