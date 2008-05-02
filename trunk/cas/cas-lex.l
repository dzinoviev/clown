%{
#include <string.h>
#include <stdlib.h>
#include "cas.h"
#include "registers.h"
#include "cas-yacc.tab.h"
int fileno(FILE *stream);	/* for ANSI compatibility */
static char string_buf[1024];
static char *string_buf_ptr;
int line_no = 1;
char *strdup(const char *s);
 static char dummy[1024];

static void lex_error (const char *message)
{
    fprintf (stderr, "cas:%s:%d: %s: %s\n", *source, line_no, message, yytext);
}
%}

ID [a-zA-Z_][a-zA-Z_0-9]*
D [0-9]
HD [0-9A-Fa-f]

%x STRING
%x COMMENT

%%

\#(\ )*[0-9]+(\ )+\"[^\"]*\"(\ [0-9]+)* {	/* preprocessor line directives */
sscanf (yytext, "# %d \"%[^\"]s\"", &line_no, dummy);
line_no--;
}

\#.* {    			/* all other preprocessor directives */
lex_error ("hmmm... looks like I need to be preprocessed first");
return T_ERROR;
}

;.*      			/* eat up comments */
\/\/.*      			/* eat up comments */
\/\*            BEGIN(COMMENT);
<COMMENT>\*\/   BEGIN(INITIAL);
<COMMENT>\n     {line_no++;}
<COMMENT>.      
<COMMENT><<EOF>>  {
    lex_error ("unterminated comment");
    return T_ERROR;
}


\.[gG][lL][oO][bB][aA][lL] {return T_GLOBAL;}
\.[aA][lL][iI][gG][nN]8 {return T_ALIGN8;}
\.[pP][aA][gG][eE] {return T_PAGE;}

\.[sS][tT][rR][iI][nN][gG] {return T_DEFSTRING;}
\.[wW][oO][rR][dD]         {return T_DEFWORD;}

\.[cC][oO][dD][eE]         {return T_CODE;}
\.[dD][aA][tT][aA]         {return T_DATA;}
\.[cC][oO][nN][sS][tT]     {return T_CONST;}

\${ID}          {yylval.s = strdup (yytext + 1); return T_SEGMENT;}

{ID}:           {strtok (yytext, " \t\n:"); yylval.s = strdup (yytext); 
		 return T_LABEL;}

%[rR][0-9]+     {yylval.i = atoi (&yytext[2]); 
		 if (yylval.i >= CLOWN_NGPR) {
		     lex_error ("illegal register");
		     return T_ERROR;
		 }
		 return T_GREGISTER;}

%[sS][pP]          {yylval.i = CLOWN_NGPR - 3; return T_GREGISTER;}
%[pP][aA][gG][eE]  {yylval.i = CLOWN_NGPR - 2; return T_GREGISTER;}
%[fF][aA][rR]      {yylval.i = CLOWN_NGPR - 1; return T_GREGISTER;}

%[iI][sS][rR]   {yylval.i = _ISR  ; return T_SREGISTER;}
%[gG][dD][tT]   {yylval.i = _GDT  ; return T_SREGISTER;}
%[lL][dD][tT]   {yylval.i = _LDT  ; return T_SREGISTER;}
%[cC][sS]    {yylval.i = _CODE ; return T_SREGISTER;}
%[sS][sS]    {yylval.i = _STACK; return T_SREGISTER;}
%[dD][sS]    {yylval.i = _DATA ; return T_SREGISTER;}
%[eE][sS]    {yylval.i = _ES   ; return T_SREGISTER;}
%[fF][sS]    {yylval.i = _FS   ; return T_SREGISTER;}

%[a-zA-Z0-9]+   {lex_error ("illegal register"); return T_ERROR;}

0[xX]{HD}+ {sscanf (yytext, "%lx", (long unsigned int *)&yylval.i); return T_NUMBER;}
{HD}+[hH]  {sscanf (yytext, "%lx", (long unsigned int *)&yylval.i); return T_NUMBER;}
{D}+       {sscanf (yytext, "%ld", &yylval.i); return T_NUMBER;}
[01]+[bB]       {int i = 0;
		 yylval.i = 0;
		 
		 while (yytext[i] != 'b' && yytext[i] != 'B') {
		     yylval.i <<= 1;
		     if (yytext[i] == '1')
			 yylval.i++;
		     i++;
		 }
		 return T_NUMBER;}

\'[^\\][^\'] |
\'\\.[^\']             {
    /* error - unterminated string constant */
    /* generate error message */
    lex_error ("multibyte character constant");
    return T_ERROR;
}

\'\\\\\'         {yylval.i = '\\'; return T_NUMBER;}
\'\\n\'          {yylval.i = '\n'; return T_NUMBER;}
\'\\t\'          {yylval.i = '\t'; return T_NUMBER;}
\'\\r\'          {yylval.i = '\r'; return T_NUMBER;}
\'\\b\'          {yylval.i = '\b'; return T_NUMBER;}
\'\\f\'          {yylval.i = '\f'; return T_NUMBER;}
\'.\'            {yylval.i = yytext[1]; return T_NUMBER;}

\'\n             {
    /* error - unterminated string constant */
    /* generate error message */
    lex_error ("unterminated character constant");
    return T_ERROR;
}

\'\\[0-7]{1,3}\' {/* octal escape sequence */
    unsigned result;
    sscanf (yytext + 2, "%o", &result);
    if (result > 0xff) {/* error, constant is out-of-bounds */
	lex_error ("character constant out of bounds");
	return T_ERROR;
    }
    yylval.i = result; return T_NUMBER;
}

\'\\[0-9]+\' {
    /* generate error - bad escape sequence; something
       like '\48' or '\0777777' */
    lex_error ("bad escape sequence");
    return T_ERROR;
}


\"              {string_buf_ptr = string_buf; BEGIN(STRING);}

<STRING>\"      {BEGIN(INITIAL); *string_buf_ptr = '\0'; 
		 yylval.s = strdup (string_buf);
		 return (T_STRING) ;}

<STRING>\n      {
    /* error - unterminated string constant */
    /* generate error message */
    lex_error ("unterminated string constant");
    return T_ERROR;
}

<STRING>\\[0-7]{1,3} {/* octal escape sequence */
    unsigned result;
    sscanf (yytext + 1, "%o", &result);
    if (result > 0xff) {/* error, constant is out-of-bounds */
	lex_error ("character constant out of bounds");
	return T_ERROR;
    }
    *string_buf_ptr++ = result;
}

<STRING>\\[0-9]+ {
    /* generate error - bad escape sequence; something
       like '\48' or '\0777777' */
    lex_error ("bad escape sequence");
    return T_ERROR;
}

<STRING>\\n        *string_buf_ptr++ = '\n';
<STRING>\\t        *string_buf_ptr++ = '\t';
<STRING>\\r        *string_buf_ptr++ = '\r';
<STRING>\\b        *string_buf_ptr++ = '\b';
<STRING>\\f        *string_buf_ptr++ = '\f';

<STRING>\\(.|\n)   { *string_buf_ptr++ = yytext[1]; }

<STRING>[^\\\n\"]+ { char *yptr = yytext;
		     while (*yptr)
			 *string_buf_ptr++ = *yptr++;
		 }

[aA][dD][dD]	     return T_ADD    ;
[aA][nN][dD]	     return T_AND    ;
[cC][aA][lL][lL]     return T_CALL   ;
[cC][hH][iI][oO]     return T_CHIO   ;
[cC][lL][cC]	     return T_CLC    ;
[cC][lL][iI]	     return T_CLI    ;
[cC][lL][rR][bB]     return T_CLRB   ;
[cC][mM][pP]	     return T_CMP    ;
[dD][eE][cC]	     return T_DEC    ;
[dD][iI][vV]	     return T_DIV    ;
[gG][eE][tT][bB]     return T_GETB   ;
[hH][lL][tT]	     return T_HLT    ;
[iI][nN]	     return T_IN     ;
[iI][nN][cC]	     return T_INC    ;
[jJ][cC]	     return T_JC     ;
[jJ][mM][pP]	     return T_JMP    ;
[jJ][nN][cC]	     return T_JNC    ;
[jJ][nN][oO]	     return T_JNO    ;
[jJ][nN][sS]	     return T_JNS    ;
[jJ][nN][zZ]	     return T_JNZ    ;
[jJ][oO]	     return T_JO     ;
[jJ][sS]	     return T_JS     ;
[jJ][zZ]	     return T_JZ     ;
[mM][oO][vV]	     return T_MOV    ;
[mM][uU][lL]	     return T_MUL    ;
[nN][eE][gG]	     return T_NEG    ;
[nN][oO][pP]	     return T_NOP    ;
[nN][oO][tT]	     return T_NOT    ;
[oO][rR]	     return T_OR     ;
[oO][uU][tT]	     return T_OUT    ;
[pP][oO][pP]	     return T_POP    ;
[pP][oO][pP][fF]     return T_POPF   ;
[pP][oO][kK][eE]     return T_POKE   ;
[pP][uU][sS][hH]     return T_PUSH   ;
[pP][uU][sS][hH][fF] return T_PUSHF  ;
[pP][eE][eE][kK]     return T_PEEK   ;
[rR][eE][mM]	     return T_REM    ;
[rR][eE][tT][fF]     return T_RETF   ;
[rR][eE][tT][fF][iI] return T_RETFI  ;
[rR][eE][tT][nN]     return T_RETN   ;
[rR][eE][tT][nN][iI] return T_RETNI  ;
[rR][oO][lL]	     return T_ROL    ;
[rR][oO][rR]	     return T_ROR    ;
[sS][aA][lL]	     return T_SAL    ;
[sS][aA][rR]	     return T_SAR    ;
[sS][eE][tT][bB]     return T_SETB   ;
[sS][tT][cC]	     return T_STC    ;
[sS][tT][iI]	     return T_STI    ;
[sS][tT][oO][pP]     return T_STOP   ;
[sS][uU][bB]	     return T_SUB    ;
[tT][rR][aA][pP]     return T_TRAP   ;
[tT][sS][tT]	     return T_TST    ;
[xX][cC][hH][gG]     return T_XCHG   ;
[xX][oO][rR]         return T_XOR    ;

{ID} {yylval.s = strdup (yytext); return T_ADDRESS;}

\<\<		{return T_LL;}
\>\>		{return T_GG;}
[,\[\]\(\)\+-:^\|&~!%-/\?{}]    {return yytext[0];}
\n                 {line_no++;}
[ \t]*

.		   {
	char s[] = "invalid or unexpected character: '%c' (0xxx)\n";
	sprintf (s, "invalid or unexpected character: '%c' (0x%02x)\n",
		yytext[0], yytext[0]);
	lex_error (s); 
	return T_ERROR;}

%%

int yywrap ()
{
    return 1;
}
