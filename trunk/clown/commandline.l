%{
  int fileno(FILE *stream);	/* to make th CC happy  */
#include <string.h>
#include "registers.h"
#include "commandline.tab.h"
%}

OD [0-7]
DD [0-9]
HD [0-9A-Fa-f]

%%

"h"|"help"        { return T_HELP; }
"l"|"load"        { return T_LOAD; }
    "reset"       { return T_RESET; }
"r"|"run"         { return T_RUN; }
"s"|"step"        { return T_STEP; }
"q"|"quit"|"exit" { return T_QUIT; }

\"[^\"]*\"   {
    int l = strlen (yytext) - 2;
    yylval.s = malloc (l + 1); 
    strncpy (yylval.s, yytext + 1, l);
    yylval.s[l] = 0;
    return T_STRING;
}

%r[0-9]+     {yylval.i = atoi (&yytext[2]); 
		 if (yylval.i >= CLOWN_NGPR) {
		     fprintf (stderr, "\t\"%s\": illegal register name\n", 
			      yytext);
		     return T_ERROR;
		 }
		 return T_GREGISTER;}

%sp    {yylval.i = CLOWN_NGPR - 3; return T_GREGISTER;}
%page  {yylval.i = CLOWN_NGPR - 2; return T_GREGISTER;}
%far   {yylval.i = CLOWN_NGPR - 1; return T_GREGISTER;}
%all   {yylval.i = -1; return T_GREGISTER;}

%isr   {yylval.i = _ISR  ; return T_SREGISTER;}
%gdt   {yylval.i = _GDT  ; return T_SREGISTER;}
%ldt   {yylval.i = _LDT  ; return T_SREGISTER;}
%cs    {yylval.i = _CODE ; return T_SREGISTER;}
%ss    {yylval.i = _STACK; return T_SREGISTER;}
%ds    {yylval.i = _DATA ; return T_SREGISTER;}
%es    {yylval.i = _ES   ; return T_SREGISTER;}
%fs    {yylval.i = _FS   ; return T_SREGISTER;}
%pc    { return T_PC; }
%ir    { return T_IR; }
%fl|%flags { return T_FLAGS; }

\\[bcodhpa]  {yylval.i = yytext[1]; return T_FORMAT;}
\\[^bcodhpa] {fprintf (stderr, "\t\"%s\": bad format code; valid codes are \"abcdhop\"\n", yytext); 
	      return T_ERROR;}

%[a-z0-9]+ {fprintf (stderr, "\t\"%s\": illegal register name\n", yytext); 
	    return T_ERROR;}

0[xX]{HD}+ {sscanf (yytext, "%lx", &yylval.i); return T_NUMBER;}
{HD}+[hH]  {sscanf (yytext, "%lx", &yylval.i); return T_NUMBER;}
{OD}+[oO]  {sscanf (yytext, "%lo", &yylval.i); return T_NUMBER;}
-?{DD}+    {sscanf (yytext, "%ld", &yylval.i); return T_NUMBER;}
[01]+[bB]       {int i = 0;
		 yylval.i = 0;
		 
		 while (yytext[i] != 'b' && yytext[i] != 'B') {
		     yylval.i <<= 1;
		     if (yytext[i] == '1')
			 yylval.i++;
		     i++;
		 }
		 return T_NUMBER;}

\n {return T_EOL;}
<<EOF>> { return T_DONE; }

[\[\]=]   {return yytext[0];}

[ \t]+

[^\n] { fprintf (stderr, "\t\"%s\": unrecognized command. Type 'help' to get help\n", yytext); return T_ERROR;}


%% T_DONE
