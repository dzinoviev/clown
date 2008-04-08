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
    int l = strlen (cmdtext) - 2;
    cmdlval.s = malloc (l + 1); 
    strncpy (cmdlval.s, cmdtext + 1, l);
    cmdlval.s[l] = 0;
    return T_STRING;
}

%r[0-9]+     {cmdlval.i = atoi (&cmdtext[2]); 
		 if (cmdlval.i >= CLOWN_NGPR) {
		     fprintf (stderr, "\t\"%s\": illegal register name\n", 
			      cmdtext);
		     return T_ERROR;
		 }
		 return T_GREGISTER;}

%sp    {cmdlval.i = CLOWN_NGPR - 3; return T_GREGISTER;}
%page  {cmdlval.i = CLOWN_NGPR - 2; return T_GREGISTER;}
%far   {cmdlval.i = CLOWN_NGPR - 1; return T_GREGISTER;}
%all   {cmdlval.i = -1; return T_GREGISTER;}

%isr   {cmdlval.i = _ISR  ; return T_SREGISTER;}
%gdt   {cmdlval.i = _GDT  ; return T_SREGISTER;}
%ldt   {cmdlval.i = _LDT  ; return T_SREGISTER;}
%cs    {cmdlval.i = _CODE ; return T_SREGISTER;}
%ss    {cmdlval.i = _STACK; return T_SREGISTER;}
%ds    {cmdlval.i = _DATA ; return T_SREGISTER;}
%es    {cmdlval.i = _ES   ; return T_SREGISTER;}
%fs    {cmdlval.i = _FS   ; return T_SREGISTER;}
%pc    { return T_PC; }
%ir    { return T_IR; }
%fl|%flags { return T_FLAGS; }

\\[bcodhpa]  {cmdlval.i = cmdtext[1]; return T_FORMAT;}
\\[^bcodhpa] {fprintf (stderr, "\t\"%s\": bad format code; valid codes are \"abcdhop\"\n", cmdtext); 
	      return T_ERROR;}

%[a-z0-9]+ {fprintf (stderr, "\t\"%s\": illegal register name\n", cmdtext); 
	    return T_ERROR;}

0[xX]{HD}+ {sscanf (cmdtext, "%lx", &cmdlval.i); return T_NUMBER;}
{HD}+[hH]  {sscanf (cmdtext, "%lx", &cmdlval.i); return T_NUMBER;}
{OD}+[oO]  {sscanf (cmdtext, "%lo", &cmdlval.i); return T_NUMBER;}
-?{DD}+    {sscanf (cmdtext, "%ld", &cmdlval.i); return T_NUMBER;}
[01]+[bB]       {int i = 0;
		 cmdlval.i = 0;
		 
		 while (cmdtext[i] != 'b' && cmdtext[i] != 'B') {
		     cmdlval.i <<= 1;
		     if (cmdtext[i] == '1')
			 cmdlval.i++;
		     i++;
		 }
		 return T_NUMBER;}

\n {return T_EOL;}
<<EOF>> { return T_DONE; }

[\[\]=\']   {return cmdtext[0];}

[ \t]+

[^\n] { fprintf (stderr, "\t\"%s\": unrecognized command. Type 'help' to get help\n", cmdtext); return T_ERROR;}


%% T_DONE
