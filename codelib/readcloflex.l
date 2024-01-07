%{
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
int fileno(FILE *stream);	/* for ANSI compatibility */
#include "clowndev.h"
#include "readclofparse.h"
 int yyerror(char *s);

int line_no = 1;
#define CDATA_BEGIN (sizeof("<![CDATA[")-1) /* 9 */
#define CDATA_END (sizeof("]]>")-1) /* 3 */
%}

%option nounput
%option noinput

%%

"<![CDATA["(.*)"]]>" { 
  int  charcodesize = strlen(yytext) - CDATA_BEGIN - CDATA_END;
  yytext[charcodesize + CDATA_BEGIN] = 0;

  Dword *wordcode = safe_malloc(charcodesize * sizeof(Dword));

  int wordcodesize = base64_decode(yytext + CDATA_BEGIN, wordcode);
  if (-1 == wordcodesize) {
    free(wordcode);
    return '.';
  }
  wordcode = safe_realloc(wordcode, wordcodesize * sizeof(Dword));

  yylval.code.size = 0;
  yylval.code.bin = build_expressions(wordcode, wordcodesize,
				       &yylval.code.size);
  free(wordcode);

  return T_CODE; 
}

[=\"] { return yytext[0]; }
[0-9]+ { 
  yylval.i = atoi(yytext);
  return T_NUMBER; 
}
"bin" { return T_BIN; }
"clof" { return T_CLOFEXE; }
"clef" { return T_CLEFEXE; }
"code*" { return T_CODESTAR; }
"defined" { return T_DEFINED; }
"file" { return T_FILE; }
"global" { return T_GLOBAL; }
"id" { return T_ID; }
"lineno" { return T_LINENO; }
"line" { return T_LINE; }
"name" { return T_NAME; }
"offset" { return T_OFFSET; }
"segment" { return T_SEGMENT; }
"segments" { return T_SEGMENTS; }
"size" { return T_SIZE; }
"<symbols" { return T_SYMBOLS; }
"</symbols" { return T_ESYMBOLS; }
"symbol" { return T_SYMBOL; }
"</" { return T_LS; }
"/>" { return T_SG; }
"<" { return T_L; }
">" { return T_G; }
"type" { return T_TYPE; }
[-_a-zA-Z/\.][-_a-zA-Z0-9/\.]* {
  yylval.s = safe_malloc(strlen(yytext) + 1); 
  strcpy(yylval.s, yytext); 
  return T_STRING; 
}
[ \t\v\f]+
\n { line_no++; }
. { fprintf(stderr, "'%c' [0x%02X] ", yytext[0], yytext[0]); 
  yyerror("invalid character; make sure your file names do not contain \"funny\" characters"); }
%%

