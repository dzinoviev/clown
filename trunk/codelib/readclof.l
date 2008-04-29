%{
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
int fileno(FILE *stream);	/* for ANSI compatibility */
#include "clowndev.h"
#include "readclof.tab.h"
 int rderror (char *s);

int line_no = 1;
#define CDATA_BEGIN (sizeof("<![CDATA[")-1) /* 9 */
#define CDATA_END (sizeof("]]>")-1) /* 3 */
%}

%%

"<![CDATA["(.*)"]]>" { 
  int charcodesize;
  int wordcodesize;
  Dword *wordcode;

  link_overhead = 0;
  
  charcodesize = strlen (rdtext) - CDATA_BEGIN - CDATA_END;
  rdtext[charcodesize + CDATA_BEGIN] = 0;

  wordcodesize = charcodesize / sizeof (Dword) + 1; 
  wordcode = safe_malloc (wordcodesize * sizeof (Dword));

  if (-1 == (wordcodesize = base64_decode (rdtext + CDATA_BEGIN, wordcode))) {
    free (wordcode);
    return '.';
  }
  wordcode = safe_realloc (wordcode, wordcodesize * sizeof (Dword));

  rdlval.code.size = 0;
  rdlval.code.escapes = 0;
  rdlval.code.bin = build_expressions (wordcode, wordcodesize, &rdlval.code.size, &rdlval.code.escapes);
  free (wordcode);

  return T_CODE; 
}

[=\"] { return rdtext[0]; }
[0-9]+ { 
    rdlval.i = atoi (rdtext);
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
"<symbols" { return T_SYMBOLS; }
"</symbols" { return T_ESYMBOLS; }
"symbol" { return T_SYMBOL; }
"</" { return T_LS; }
"/>" { return T_SG; }
"<" { return T_L; }
">" { return T_G; }
"type" { return T_TYPE; }
[-_a-zA-Z/\.][-_a-zA-Z0-9/\.]* { 
    rdlval.s = safe_malloc (strlen (rdtext) + 1); 
    strcpy (rdlval.s, rdtext); 
    return T_STRING; 
}
[ \t\v\f]+
\n { line_no++; }
. { fprintf (stderr, "'%c' [0x%02X] ", rdtext[0], rdtext[0]); rderror ("invalid character"); }
%%

