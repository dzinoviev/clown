%{
#include <stdio.h>
#include <stdlib.h>
#include "cas.h"
#include "registers.h"

Dword offset = 0;
static int current_segment = -1;

static void emit_relative_jump (struct yyLabel l, int opc) 
{
    mark_near_label (l.index);
    emit_escape (FIX_DISPLACEMENT);
    emit (BUILD_INSTRUCTION_B (opc, 0, 0));
    emit_escape (l.index);  /* label index */
    emit_escape (l.offset); /* label dispacement */
    emit_escape (offset);   /* current_offset */
}

typedef enum {LOCAL, GLOBAL} Visibility;
static void emit_absolute_ref (struct yyLabel l, Visibility type) 
{
    if (type == LOCAL)
	emit_escape (FIX_ADDRESS_LOC);
    else
	emit_escape (FIX_ADDRESS_GLOB);
    emit (0);
    emit_escape (l.index);  /* label index */
    emit_escape (l.offset); /* label dispacement */
}

%}

%union {
    char  *s;
    Dword i;
    struct {
	int size;
	Dword *data;
    } v;
    struct yyLabel label;
    struct labelDef sym;
};

%left '|'
%left '^'
%left '&'
%left T_LL T_GG
%left '+' '-'
%left '*' '/' '%'
%right UNARY_MIN
%right '!' '~'

%token T_ADD    
%token T_AND    
%token T_CALL   
%token T_CHIO   
%token T_CLC    
%token T_CLI    
%token T_CLRB   
%token T_CMP    
%token T_DEC    
%token T_DIV    
%token T_GETB   
%token T_HLT    
%token T_IN     
%token T_INC    
%token T_JC     
%token T_JMP    
%token T_JNC    
%token T_JNO    
%token T_JNS    
%token T_JNZ    
%token T_JO     
%token T_JS     
%token T_JZ     
%token T_MOV    
%token T_MUL    
%token T_NEG    
%token T_NOP    
%token T_NOT    
%token T_OR     
%token T_OUT    
%token T_PEEK
%token T_POKE
%token T_POP    
%token T_POPF   
%token T_PUSH   
%token T_PUSHF  
%token T_RETF
%token T_RETN
%token T_RETFI
%token T_RETNI
%token T_ROL    
%token T_ROR    
%token T_SAL    
%token T_SAR    
%token T_SETB   
%token T_STC    
%token T_STI    
%token T_STOP   
%token T_SUB    
%token T_TRAP   
%token T_TST    
%token T_XCHG  
%token T_XOR 

%token T_LL 
%token T_GG

%token <i> T_NUMBER
%token <s> T_ADDRESS
%token <s> T_LABEL
%token <i> T_GREGISTER
%token <i> T_SREGISTER
%token <s> T_SEGMENT
%token <s> T_STRING
%token T_DEFSTRING
%token T_DEFWORD
%token T_ERROR
%token T_GLOBAL
%token T_ALIGN8
%token <i>T_CODE
%token <i>T_DATA

%type <i>segtype
%type <i>expression
%type <i>size
%type <i>segment
%type <v>values
%type <label>address
%type <sym>symdef

%start program

%%

program     : lines {		  
    if (current_segment != -1)
	end_segment (current_segment, offset);
};

lines       : line lines 
            | ;

line        : T_GLOBAL T_ADDRESS { 
                  yyerror ("this use of .global declaration is depricated");
                  mark_export_label ($2) 
	      }
            | segtype T_SEGMENT {    
		  if (current_segment != -1)
		      end_segment (current_segment, offset);
		  if ((current_segment = begin_segment ($1, $2)) == -1) {
		      yyerror ("fatal error");
		      YYABORT;
		  }
		  offset = 0;
	      } 
            | labels datadef {}
            | labels instruction  {}
            | datadef {}
            | instruction  {}
            ;

segtype     : T_CODE {$$ = SEG_CODE} 
            | T_DATA {$$ = SEG_DATA};

expression  : T_NUMBER {$$ = $1}
            | '(' expression ')' {$$ = $2}
            | '(' error ')' {$$ = 0; yyerror ("malformed expression")}  
            | expression '+' expression {$$ = $1 + $3}
            | '-' expression %prec UNARY_MIN {$$ = -$2}
            | expression '-' expression {$$ = $1 - $3}
            | expression '*' expression {$$ = $1 * $3}
            | expression '/' expression {$$ = $1 / $3}
            | expression '%' expression {$$ = $1 % $3}
            | expression '^' expression {$$ = $1 ^ $3}
            | expression '&' expression {$$ = $1 & $3}
            | expression '|' expression {$$ = $1 | $3}
            |            '!' expression {$$ = !$2}
            |            '~' expression {$$ = ~$2}
            | expression T_LL expression {$$ = $1 << $3}
            | expression T_GG expression {$$ = $1 >> $3};

labels      : label
            | label labels
            ;

label       : symdef T_LABEL {
                if (-1 == add_label ($2, current_segment, offset, 
				     $1.global, $1.align8)) {
		    yyerror ("fatal error");
		    YYABORT;
		}
            }
            ;

symdef      : T_GLOBAL {$$.align8 = 0; $$.global = 1 }
            | T_ALIGN8 {$$.align8 = 1; $$.global = 0 }
            | T_ALIGN8 T_GLOBAL {$$.align8 = 1; $$.global = 1 }
            | T_GLOBAL T_ALIGN8 {$$.align8 = 1; $$.global = 1 }
            | {$$.global = $$.align8 = 0 };

datadef     : T_DEFSTRING T_STRING {
                char *s;
		for (s = $2; *s; s++) {
		    Dword d = *s;
		    emit (d);
		}
	    }
            | T_DEFWORD  size values {
		int i;
		if ($2 < $3.size)
		    yyerror ("warning: too many initializers");
		for (i = 0; i < $3.size; i++)
		    emit ($3.data[i]);
		for (     ; i < $2     ; i++)
		    emit (0);
	    };

size        : {$$ = 1}
            | '[' expression ']' {
		if ($2 <= 0) {
		    yyerror ("invalid array size");
		    YYABORT;
		} 
		$$ = $2
	    }
            | '[' error ']' { $$ = -1;  yyerror ("malformed array")};

values      : {$$.size = 0; $$.data = NULL}
            | expression { 
		$$.data = malloc (sizeof (Dword)); 
		$$.data[0] = $1;
		$$.size = 1;
	    }
            | values ',' expression { 
		$$.data = realloc ($1.data, ($1.size + 1) * sizeof (Dword)); 
		$$.data[$1.size] = $3;
		$$.size = $1.size + 1;
	    } ;

address     : T_ADDRESS {
                int label = use_label ($1, current_segment);
		if (-1 == label) {
		    yyerror ("fatal error");
		    YYABORT;
		}
                $$.index = label;
		$$.offset = 0;
	    }
            | address '+' expression {
		$$.index = $1.index;
		$$.offset += $3;
	    }
            | address '-' expression {
		$$.index = $1.index;
		$$.offset += -$3;
	    };

segment     : T_SEGMENT {
              $$ = lookup_segment ($1); 
	      if (-1 == $$) {
		  yyerror ("fatal error");
		  YYABORT;
	      }
	  };

instruction : error instruction
            | T_ADD T_GREGISTER ',' T_GREGISTER
              {emit (BUILD_INSTRUCTION_A (ADD, $2, $4))} /* ADD */

            | T_ADD T_GREGISTER ',' expression
              {emit (BUILD_INSTRUCTION_A (xADDI, $2, 0));
	       emit ($4)} /* xADDI */

            | T_AND T_GREGISTER ',' T_GREGISTER
              {emit (BUILD_INSTRUCTION_A (AND, $2, $4))} /* AND */

            | T_AND T_GREGISTER ',' expression 
              {emit (BUILD_INSTRUCTION_A (xANDI, $2, 0));
	       emit ($4)} /* xADDI */

            | T_CALL address	/* xNCALL */
              { 
              mark_near_label ($2.index);
              emit (BUILD_INSTRUCTION_A (xNCALL, 0, 0));
              emit_absolute_ref ($2, LOCAL);
              }
            | T_CALL expression	/* xNCALL */
              { 
              emit (BUILD_INSTRUCTION_A (xNCALL, 0, 0));
              emit ($2);
              }

            | T_CALL '[' T_GREGISTER ']'
              {emit (BUILD_INSTRUCTION_A (NCALLX, $3, 0))} /* NCALLX */
            | T_CALL '[' error ']'
              {yyerror ("invalid indirection")}

            | T_CALL segment ':' address
              {
		  Selector s = MK_SELECTOR ($2, 0, _LDT); 
		  /* the next instruction contains a segment selector 
		     that must be adjusted if there is more than one module
		     in the program */
		  if (module_type == CLOF_EXE)
		      emit_escape (FIX_SEGMENT); 
		  emit (BUILD_INSTRUCTION_C (xFCALL, 0, s));
                  emit_absolute_ref ($4, GLOBAL);
	      } /* xFCALL */

            | T_CALL segment	/* for doors */
              {
                  struct yyLabel dummy = {0, 0};
		  Selector s = MK_SELECTOR ($2, 0, _LDT); 
		  /* the next instruction contains a segment selector 
		     that must be adjusted if there is more than one module
		     in the program */
		  if (module_type == CLOF_EXE)
		      emit_escape (FIX_SEGMENT); 
		  emit (BUILD_INSTRUCTION_C (xFCALL, 0, s));
                  emit_absolute_ref (dummy, LOCAL);
	      } /* xFCALL */

            | T_CHIO expression 
              {emit (BUILD_INSTRUCTION_B (CHIO, 0, $2))} /* CHIO */

            | T_CLC 
              {emit (BUILD_INSTRUCTION_A (CLC, 0, 0))} /* CLC */

            | T_CLI 
              {emit (BUILD_INSTRUCTION_A (CLI, 0, 0))} /* CLI */

            | T_CLRB T_GREGISTER ',' expression 
              {emit (BUILD_INSTRUCTION_B (CLRBI, $2, $4))} /* CLRBI */

            | T_CLRB T_GREGISTER ','  T_GREGISTER
              {emit (BUILD_INSTRUCTION_A (CLRB, $2, $4))} /* CLRB */

            | T_CMP T_GREGISTER ',' T_GREGISTER
              {emit (BUILD_INSTRUCTION_A (CMP, $2, $4))} /* CMP */

            | T_CMP T_GREGISTER ',' expression
              {emit (BUILD_INSTRUCTION_A (xCMPI, $2, 0));
               emit ($4)} /* xCMPI */

            | T_DEC T_GREGISTER
              {emit (BUILD_INSTRUCTION_A (DEC, $2, 0))} /* DEC */

            | T_DIV T_GREGISTER ',' T_GREGISTER
              {emit (BUILD_INSTRUCTION_A (DIV, $2, $4))} /* DIV */

            | T_DIV T_GREGISTER ',' expression
              {emit (BUILD_INSTRUCTION_A (xDIVI, $2, 0));
               emit ($4)} /* xDIVI */

            | T_DIV expression ',' T_GREGISTER
              {emit (BUILD_INSTRUCTION_A (xDIVJ, $4, 0));
               emit ($2)} /* xDIVJ */

            | T_GETB T_GREGISTER ',' T_GREGISTER
              {emit (BUILD_INSTRUCTION_A (GETB, $2, $4))} /* GETB */

            | T_GETB T_GREGISTER ',' expression
              {emit (BUILD_INSTRUCTION_B (GETBI, $2, $4))} /* GETBI */

            | T_HLT {emit (BUILD_INSTRUCTION_A (HLT, 0, 0))} /* HLT */ 

            | T_IN T_GREGISTER ',' expression
              {emit (BUILD_INSTRUCTION_B (IN, $2, $4))} /* IN */

            | T_INC T_GREGISTER
              {emit (BUILD_INSTRUCTION_A (INC, $2, 0))} /* INC */

            | T_JMP address	/* absolute address */
              { 
              mark_near_label ($2.index);
              emit (BUILD_INSTRUCTION_A (xNJMP, 0, 0));
              emit_absolute_ref ($2, LOCAL);
              }

            | T_JMP expression	/* absolute address */
              { 
              emit (BUILD_INSTRUCTION_A (xNJMP, 0, 0));
              emit ($2);
              }

            | T_JMP segment ':' address
              {
		  Selector s = MK_SELECTOR ($2, 0, _LDT); 
		  /* the next instruction contains a segment selector 
		     that must be adjusted if there is more than one module
		     in the program */
		  if (module_type == CLOF_EXE)
		      emit_escape (FIX_SEGMENT); 
		  emit (BUILD_INSTRUCTION_C (xFJMP, 0, s));
                  emit_absolute_ref ($4, GLOBAL);
	      } /* xFJMP */

            | T_JMP segment
              {
                  struct yyLabel dummy = {0, 0};
		  Selector s = MK_SELECTOR ($2, 0, _LDT); 
		  /* the next instruction contains a segment selector 
		     that must be adjusted if there is more than one module
		     in the program */
		  if (module_type == CLOF_EXE)
		      emit_escape (FIX_SEGMENT); 
		  emit (BUILD_INSTRUCTION_C (xFJMP, 0, s));
                  emit_absolute_ref (dummy, LOCAL);
	      } /* xFJMP */

            | T_JNZ address
              { emit_relative_jump ($2, JNZ); }
            | T_JZ address
              { emit_relative_jump ($2, JZ); }
            | T_JNC address
              { emit_relative_jump ($2, JNC); }
            | T_JC address
              { emit_relative_jump ($2, JC); }
            | T_JNO address
              { emit_relative_jump ($2, JNO); }
            | T_JO address
              { emit_relative_jump ($2, JO); }
            | T_JNS address
              { emit_relative_jump ($2, JNS); }
            | T_JS address
              { emit_relative_jump ($2, JS); }

            | T_MOV T_GREGISTER ',' '['  address ']' /* xLD */
              { 
              emit (BUILD_INSTRUCTION_A (xLD, $2, 0));
              emit_absolute_ref ($5, GLOBAL);
              }
            | T_MOV T_GREGISTER ',' '['  error ']'
              {yyerror ("invalid indirection")}

            | T_MOV T_GREGISTER ',' address
              {
               emit (BUILD_INSTRUCTION_A (xLDI, $2, 0));
               emit_absolute_ref ($4, GLOBAL)
              } /* XLDI */

            | T_MOV address ',' T_GREGISTER
              {
                 yyerror ("Did you forget [] around the variable name?");
              } 

            | T_MOV T_GREGISTER ',' expression
              {
               emit (BUILD_INSTRUCTION_A (xLDI, $2, 0));
	       emit ($4)
              } /* XLDI */

            | T_MOV '[' address ']' ',' T_GREGISTER
              { 
              emit (BUILD_INSTRUCTION_A (xST, $6, 0));
              emit_absolute_ref ($3, GLOBAL);
              }
            | T_MOV '[' error ']' ',' T_GREGISTER
              {yyerror ("invalid indirection")}

            | T_MOV T_SREGISTER ',' T_GREGISTER
              {emit (BUILD_INSTRUCTION_A (MOVTS, $2, $4))} /* MOVTS */

            | T_MOV T_GREGISTER ',' T_SREGISTER
              {emit (BUILD_INSTRUCTION_A (MOVFS, $2, $4))} /* MOVFS */

            | T_MOV T_GREGISTER ',' T_GREGISTER
              {emit (BUILD_INSTRUCTION_A (MOV, $2, $4))} /* MOV */

            | T_MOV T_SREGISTER ',' segment 
              {
		  Selector s = MK_SELECTOR ($4, 0, _LDT); 
		  /* the next instruction contains a segment selector 
		     that must be adjusted if there is more than one module
		     in the program */
		  if (module_type == CLOF_EXE)
		      emit_escape (FIX_SEGMENT); 
		  emit (BUILD_INSTRUCTION_C (MOVSI, $2, s))
	      } /* MOVSI */

            | T_MOV T_GREGISTER ',' '[' T_GREGISTER ']'
              {emit (BUILD_INSTRUCTION_A (LDX, $2, $5))} /* LDX */

            | T_MOV '[' T_GREGISTER ']' ','  T_GREGISTER
              {emit (BUILD_INSTRUCTION_A (STX, $3, $6))} /* STX */

            | T_MOV T_GREGISTER ',' T_SREGISTER ':' address
              {
		  emit (BUILD_INSTRUCTION_A (xLDS, $2, $4));
                  emit_absolute_ref ($6, GLOBAL);
	      } /* xLDS */

            | T_MOV T_SREGISTER ':' address ',' T_GREGISTER 
              {
		  emit (BUILD_INSTRUCTION_A (xSTS, $2, $6));
                  emit_absolute_ref ($4, GLOBAL);
	      } /* xSTS */

            | T_MOV T_GREGISTER '(' expression ')' ',' T_GREGISTER
              {emit (BUILD_INSTRUCTION_D (SETBY, $7, $2, $4))} /* SETBY */

            | T_MOV T_GREGISTER ',' T_GREGISTER '(' expression ')'
              {emit (BUILD_INSTRUCTION_D (GETBY, $2, $4, $6))} /* GETBY */

            | T_MUL T_GREGISTER ',' T_GREGISTER
              {emit (BUILD_INSTRUCTION_A (MUL, $2, $4))} /* MUL */

            | T_MUL T_GREGISTER ',' expression
              {emit (BUILD_INSTRUCTION_A (xMULI, $2, 0));
               emit ($4)} /* xMULI */

            | T_NEG T_GREGISTER
              {emit (BUILD_INSTRUCTION_A (NEG, $2, 0))} /* NEG */

            | T_NOP 
              {emit (BUILD_INSTRUCTION_A (NOP, 0, 0))} /* NOP */

            | T_NOT T_GREGISTER 
              {emit (BUILD_INSTRUCTION_A (NOT, $2, 0))} /* NOT */

            | T_OR T_GREGISTER ',' T_GREGISTER
              {emit (BUILD_INSTRUCTION_A (OR, $2, $4))} /* OR */

            | T_OR T_GREGISTER ',' expression
              {emit (BUILD_INSTRUCTION_A (xORI, $2, 0));
               emit ($4)} /* xORI */

            | T_OUT T_GREGISTER ',' expression
              {emit (BUILD_INSTRUCTION_B (OUT, $2, $4))} /* OUT */

            | T_OUT expression ',' expression
              {emit (BUILD_INSTRUCTION_B (xOUTI, 0, $4));
               emit ($2)} /* xOUTI */

            | T_POP T_GREGISTER
              {emit (BUILD_INSTRUCTION_A (POP, $2, 0))} /* POP */

            | T_POPF 
              {emit (BUILD_INSTRUCTION_A (POPF, 0, 0))} /* POPF */

            | T_PUSH T_GREGISTER
              {emit (BUILD_INSTRUCTION_A (PUSH, $2, 0))} /* PUSH */

            | T_PUSH expression
              {emit (BUILD_INSTRUCTION_A (xPUSHI, 0, 0));
               emit ($2)} /* xPUSHI */

            | T_PEEK T_GREGISTER ',' expression
              {if ($4 < 0) {
		  yyerror ("negative offset in peek/poke");
		  YYABORT;
	      }
              emit (BUILD_INSTRUCTION_B (PEEK, $2, $4))} /* PEEK */

            | T_POKE T_GREGISTER ',' expression
              {if ($4 < 0) {
		  yyerror ("negative offset in peek/poke");
		  YYABORT;
	      }
              emit (BUILD_INSTRUCTION_B (POKE, $2, $4))} /* POKE */

            | T_PUSHF 
              {emit (BUILD_INSTRUCTION_A (PUSHF, 0, 0))} /* PUSHF */

            | T_RETN 
              {emit (BUILD_INSTRUCTION_A (NRET, 0, 0))} /* RETN */

            | T_RETNI
              {emit (BUILD_INSTRUCTION_A (INRET, 0, 0))} /* RETNI */

            | T_RETF 
              {emit (BUILD_INSTRUCTION_A (FRET, 0, 0))} /* RETF */

            | T_RETFI 
              {emit (BUILD_INSTRUCTION_A (IFRET, 0, 0))} /* RETFI */

            | T_ROL T_GREGISTER ',' T_GREGISTER
              {emit (BUILD_INSTRUCTION_A (ROL, $2, $4))} /* ROL */

            | T_ROL T_GREGISTER ',' expression
              {emit (BUILD_INSTRUCTION_B (ROLI, $2, $4))} /* ROLI */

            | T_ROR T_GREGISTER ',' T_GREGISTER 
              {emit (BUILD_INSTRUCTION_A (ROR, $2, $4))} /* ROR */

            | T_ROR T_GREGISTER ',' expression
              {emit (BUILD_INSTRUCTION_B (RORI, $2, $4))} /* RORI */

            | T_SAL T_GREGISTER ',' T_GREGISTER
              {emit (BUILD_INSTRUCTION_A (SAL, $2, $4))} /* SAL */

            | T_SAL T_GREGISTER ',' expression
              {emit (BUILD_INSTRUCTION_B (SALI, $2, $4))} /* SALI */

            | T_SAR T_GREGISTER ',' T_GREGISTER
              {emit (BUILD_INSTRUCTION_A (SAR, $2, $4))} /* SAR */

            | T_SAR T_GREGISTER ',' expression
              {emit (BUILD_INSTRUCTION_B (SARI, $2, $4))} /* SARI */

            | T_SETB T_GREGISTER ',' T_GREGISTER
              {emit (BUILD_INSTRUCTION_A (SETB, $2, $4))} /* SETB */

            | T_SETB T_GREGISTER ',' expression
              {emit (BUILD_INSTRUCTION_B (SETBI, $2, $4))} /* SETBI */

            | T_STC {emit (BUILD_INSTRUCTION_A (STC, 0, 0))} /* STC */

            | T_STI 
              {emit (BUILD_INSTRUCTION_A (STI, 0, 0))} /* STI */

            | T_TRAP
              {emit (BUILD_INSTRUCTION_A (TRAP, 0, 0))} /* TRAP */

            | T_STOP 
              {emit (BUILD_INSTRUCTION_A (STOP, 0, 0))} /* STOP */

            | T_SUB T_GREGISTER ',' T_GREGISTER
              {emit (BUILD_INSTRUCTION_A (SUB, $2, $4))} /* SUB */

            | T_SUB T_GREGISTER ',' expression
              {emit (BUILD_INSTRUCTION_A (xSUBI, $2, 0));
               emit ($4)} /* SUBI */

            | T_TST T_GREGISTER ',' T_GREGISTER
              {emit (BUILD_INSTRUCTION_A (TST, $2, $4))} /* TST */

            | T_TST T_GREGISTER ',' expression
              {emit (BUILD_INSTRUCTION_A (xTSTI, $2, 0));
               emit ($4)} /* xTSTI */

            | T_XCHG T_GREGISTER ',' '[' address ']' /* xXCHG */
              { 
              emit (BUILD_INSTRUCTION_A (xXCHG, $2, 0));
              emit_absolute_ref ($5, GLOBAL);
              }
            | T_XCHG T_GREGISTER ',' '[' error ']' /* xXCHG */
              {yyerror ("invalid indirection")}

            | T_XOR T_GREGISTER ',' T_GREGISTER
              {emit (BUILD_INSTRUCTION_A (XOR, $2, $4))} /* XOR */

            | T_XOR T_GREGISTER ',' expression
              {emit (BUILD_INSTRUCTION_A (xXORI, $2, 0));
               emit ($4)} /* xXORI */

;

%%

int yyerror (char *s)
{
    fprintf (stderr, "cas:%s:%d: %s\n", source, line_no, s);
    return 0;
}
