%{
#include <string.h>
#include <stdlib.h>
#include "cas.h"
#include "registers.h"
#include "cas-yacc.h"
  int fileno(FILE *stream);	/* for ANSI compatibility */
  static char string_buf[1024];
  static char *string_buf_ptr;
  int line_no = 1;
  static char dummy[1024];
  
static void lex_error (const char *message)
{
  fprintf (stderr, "cas:%s:%d: %s: %s\n", *source, line_no, message, yytext);
}
%}

%option nounput
%option noinput

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


\.(?i:global) {return T_GLOBAL;}
\.(?i:align8) {return T_ALIGN8;}
\.(?i:page)   {return T_PAGE;}

\.(?i:string) {return T_DEFSTRING;}
\.(?i:word)   {return T_DEFWORD;}

\.(?i:code)   {return T_CODE;}
\.(?i:data)   {return T_DATA;}
\.(?i:const)  {return T_CONST;}

\${ID}          {yylval.s = strdup (yytext + 1); return T_SEGMENT;}

{ID}:           {strtok (yytext, " \t\n:"); yylval.s = strdup (yytext); 
		 return T_LABEL;}

%[rR][0-9]+     {yylval.i = atoi (&yytext[2]); 
		 if (yylval.i >= CLOWN_NGPR) {
		     lex_error ("illegal register");
		     return T_ERROR;
		 }
		 return T_GREGISTER;}

%(?i:sp)          {yylval.i = CLOWN_NGPR - 3; return T_GREGISTER;}
%(?i:page) {yylval.i = CLOWN_NGPR - 2; return T_GREGISTER;}
%(?i:far)      {yylval.i = CLOWN_NGPR - 1; return T_GREGISTER;}

%(?i:isr)   {yylval.i = _ISR  ; return T_SREGISTER;}
%(?i:gdt)   {yylval.i = _GDT  ; return T_SREGISTER;}
%(?i:ldt)   {yylval.i = _LDT  ; return T_SREGISTER;}
%(?i:cs)    {yylval.i = _CODE ; return T_SREGISTER;}
%(?i:ss)    {yylval.i = _STACK; return T_SREGISTER;}
%(?i:ds)    {yylval.i = _DATA ; return T_SREGISTER;}
%(?i:es)    {yylval.i = _ES   ; return T_SREGISTER;}
%(?i:fs)    {yylval.i = _FS   ; return T_SREGISTER;}

%[a-zA-Z0-9]+   {lex_error ("illegal register"); return T_ERROR;}

0[xX]{HD}+ {sscanf (yytext, "%lx", (long unsigned int *)&yylval.i); return T_NUMBER;}
{HD}+[hH]  {sscanf (yytext, "%lx", (long unsigned int *)&yylval.i); return T_NUMBER;}
{D}+       {sscanf (yytext, "%"PRIi32, &yylval.i); return T_NUMBER;}
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

(?i:add)	     return T_ADD    ;
(?i:and)	     return T_AND    ;
(?i:call)     return T_CALL   ;
(?i:chio)     return T_CHIO   ;
(?i:clc)	     return T_CLC    ;
(?i:cli)	     return T_CLI    ;
(?i:clrb)     return T_CLRB   ;
(?i:cmp)	     return T_CMP    ;
(?i:dec)	     return T_DEC    ;
(?i:div)	     return T_DIV    ;
(?i:getb)     return T_GETB   ;
(?i:hlt)	     return T_HLT    ;
(?i:in)	     return T_IN     ;
(?i:inc)	     return T_INC    ;
(?i:jc)	     return T_JC     ;
(?i:jmp)	     return T_JMP    ;
(?i:jnc)	     return T_JNC    ;
(?i:jno)	     return T_JNO    ;
(?i:jns)	     return T_JNS    ;
(?i:jnz)     return T_JNZ    ;
(?i:jo)	     return T_JO     ;
(?i:js)	     return T_JS     ;
(?i:jz)	     return T_JZ     ;
(?i:mov)	     return T_MOV    ;
(?i:mul)	     return T_MUL    ;
(?i:neg)	     return T_NEG    ;
(?i:nop)	     return T_NOP    ;
(?i:not)	     return T_NOT    ;
(?i:or)	     return T_OR     ;
(?i:out)	     return T_OUT    ;
(?i:pop)     return T_POP    ;
(?i:popf)     return T_POPF   ;
(?i:poke)     return T_POKE   ;
(?i:push)     return T_PUSH   ;
(?i:pushf) return T_PUSHF  ;
(?i:peek)     return T_PEEK   ;
(?i:rem)	   return T_REM    ;
(?i:retf)    return T_RETF   ;
(?i:retfi) return T_RETFI  ;
(?i:retn)     return T_RETN   ;
(?i:retni) return T_RETNI  ;
(?i:rol)	   return T_ROL    ;
(?i:ror)	   return T_ROR    ;
(?i:sal)	   return T_SAL    ;
(?i:sar)	   return T_SAR    ;
(?i:setb)     return T_SETB   ;
(?i:stc)	   return T_STC    ;
(?i:sti)	   return T_STI    ;
(?i:stop)     return T_STOP   ;
(?i:sub)	   return T_SUB    ;
(?i:trap)     return T_TRAP   ;
(?i:tst)	   return T_TST    ;
(?i:xchg)     return T_XCHG   ;
(?i:xor)         return T_XOR    ;

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
