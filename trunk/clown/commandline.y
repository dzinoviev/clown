%{
#include <stdio.h>
#include <stdlib.h>
#include "registers.h"
int cmderror (char *s);
int cmdlex ();
static void do_help ();
%}

%token T_RUN
%token T_STEP
%token T_QUIT
%token T_RESET
%token T_HELP
%token T_DONE
%token T_EOL
%token <i> T_FORMAT
%type  <i> optformat
%type  <i> optnumber
%token <i> T_GREGISTER
%token <i> T_SREGISTER
%token <i> T_NUMBER
%token <s> T_STRING
%token T_ERROR
%token T_PC
%token T_IR
%token T_FLAGS
%token T_LOAD

%union {
    Dword i;
    char *s;
};

%%

operation: 
  T_EOL {return 0;}
| help
| quit { do_quit ();  return 0; }
| reset
| run 
| step
| show_flags
| show_ir
| show_pc
| set_pc
| show_register
| set_register
| show_refregister
| set_refregister
| show_memory
| set_memory
| load_memory
| show_number
| error T_EOL {return 0;}
;

show_flags: T_FLAGS optformat T_EOL {
    show_flags ($2); 
    return 0; 
};

show_ir: T_IR optformat T_EOL {
    show_ir ($2); 
    return 0; 
};

show_pc: T_PC optformat T_EOL {
    show_pc ($2); 
    return 0; 
};

set_pc: T_PC '=' T_NUMBER T_EOL {
    set_pc ($3); 
    return 0; 
};

show_register: T_GREGISTER optformat T_EOL {
    show_regs ($1, $2); 
    return 0; 
};

set_register: T_GREGISTER '=' T_NUMBER T_EOL {
    set_regs ($1, $3); 
    return 0; 
};

show_refregister: '[' T_GREGISTER ']' optformat T_EOL {
    show_refrange ($2, 1, $4); 
    return 0; 
} |  '[' T_GREGISTER ']' '\'' T_NUMBER optformat T_EOL {
    show_refrange ($2, $5, $6); 
    return 0; 
};

set_refregister: '[' T_GREGISTER ']' '=' T_NUMBER T_EOL {
    set_refregs ($2, $5); 
    return 0; 
};

show_memory: '[' T_NUMBER ']' optformat T_EOL {
    show_range ($2, 1, $4); 
    return 0; 
} |  '[' T_NUMBER ']' '\'' T_NUMBER optformat T_EOL {
    show_range ($2, $5, $6); 
    return 0; 
}
;

set_memory: '[' T_NUMBER ']' '=' T_NUMBER T_EOL {
    set_mem ($2, $5); 
    return 0; 
};

load_memory: T_LOAD T_STRING optnumber T_EOL {
    fprintf (stderr, "%u words loaded\n", 
	     load_memory ($2, $3) / sizeof (Dword));
    free ($2);
    return 0; 
};

show_number: T_NUMBER optformat T_EOL {
    show_nmb ($1, $2); 
    return 0; 
};

optnumber:
           { $$ = 0; }
| T_NUMBER { $$ = $1; };

optformat:
           { $$ = 'd'; }
| T_FORMAT { $$ = $1; };

run: T_RUN optnumber T_EOL { do_run ($2); return 0; };

help: T_HELP T_EOL { do_help (); return 0; };

reset: T_RESET T_EOL { do_reset (); return 0; };

step: T_STEP T_EOL { do_step (); return 0; };

quit: T_QUIT T_EOL | T_DONE ;

%%

static void do_help ()
{
  fputs ("List of some commands:\n", stderr);
  fputs ("\t[h]elp\t-- show help\n", stderr);
  fputs ("\t[l]oad \"binfile\" [offset]\n\t\t-- load a binary image\n", stderr);
  fputs ("\t[r]un\t-- start or resume the simulation\n", stderr);
  fputs ("\treset\t-- reset CLOWN\n", stderr);
  fputs ("\t[s]tep\t-- simulate one CLOWN cycle\n", stderr);
  fputs ("\t[q]uit\t-- exit CLOWN\n", stderr);
  fputs ("\texit\t-- exit CLOWN\n", stderr);
  fputs ("To suspend the running simulation, press ^C\n", stderr);
}

int cmderror (char *s)
{
    fprintf (stderr, "\t%s\n", s);
    return 0;
}

int cmdwrap ()
{
    return 1;
}
