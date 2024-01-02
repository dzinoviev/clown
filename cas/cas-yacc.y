%{
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cas.h"
#include "registers.h"

void yywarning (char *s);
static void emit_expression (Expression *target);
static void emit_displacement (int opc, Dword op1, Expression *dspl, Bit relative);

 enum {ARRAY_SIZE, INDIRECTION, SEGMENTINBIN, VARINDEX, BADUSEOFCS, ALIGNSEGMENT};
 static struct {
     char *concise;
     char *full;
     int count;
 } _errors[] = {
     {
	 "array size must be a positive constant...",
	 "\n\tarray size must be an expression that evaluates to a positive "
	 "\n\tconstant and does not involve any symbols; e.g., 2+3",
	 0
     },

     {
	 "invalid indirection...",
	 "\n\tinvalid indirection: there must be a register name or an expression"
	 "\n\tin the brackets, e.g., [%r0] or [lock+5]",
	 0
     },

     {
	 "named segment in s CLE module...",
	 "\n\ta CLE module consists of one implicit anonymous segment;"
	 "\n\tnamed segments are allowed only in CLO modules",
	 0
     },

     {
	 "byte access index must be a positive constant...",
	 "\n\tbyte access index must be an expression that evaluates to a positive "
	 "\n\tconstant and does not involve any symbols; e.g., 2+3",
	 0
     },

     {
	 "%cs can be initialized only through CALL or JMP...",
	 "\n\tit is not allowed to load a value into %cs with a MOV command:"
	 "\n\tyou must use CALL or JMP instead",
	 0
     },

     {
	 ".page or .align8 in the explicit segmentation mode...",
	 "\n\tthe symbol will not be aligned to a page or 8-word boundary"
	 "\n\tsegment bases are in general are not aligned:",
	 0
     },

};

#define MAX_ERROR 1024    
int current_segment = DEFAULT_SEGMENT;
static char error_buffer[MAX_ERROR];
int success = 1;

static void report (int severe, int id)
{
    char *message;
    if (_errors[id].count > 0)
	message = _errors[id].concise;
    else
	message = _errors[id].full;
    if (severe)
	yyerror (message);
    else
	yywarning (message);
    _errors[id].count++;
}

%}

%union {
    char  *s;
    Dword i;
    struct {
	int size;
	Dword *data;
    } v;
    struct labelDef sym;
    struct _Expression *expr;
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
%token T_REM
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
%token T_PAGE
%token <i>T_CODE
%token <i>T_DATA
%token <i>T_CONST

%type <i>segtype
%type <expr>expression
%type <expr>expression_or_selector
%type <i>size
%type <i>segment
%type <v>values
%type <i>symbol
%type <sym>symdef

%start program

%%

program     : { if ((current_segment = begin_segment (SEG_DEFAULT, "code*")) == NOT_FOUND) {
		      yyerror ("fatal error");
		      YYABORT;
		  } 
              }
              lines
	      { end_segment (offset); };

lines       : line lines 
            | ;

line        : segtype T_SEGMENT {
              if (global_offset) {
                  yyerror ("explicit segment definitions not allowed with non-zero entry point");
                  YYABORT;
              }
		  end_segment (offset);
		  if ((current_segment = begin_segment ($1, $2)) == NOT_FOUND) {
		      yyerror ("fatal error");
		      YYABORT;
		  }
		  offset = 0;
	      } 
            | labels datadef
            | labels instruction
            | datadef
            | instruction
            ;

segtype     : T_CODE {$$ = SEG_CODE;} 
| T_DATA {$$ = SEG_DATA;}
| T_CONST {$$ = SEG_CONST;}
;

expression  : T_NUMBER                       { $$ = newConstant ($1); }
            | symbol                         { $$ = newLabel ($1); }
            | '(' expression ')'             { $$ = $2; }
            | expression '+' expression      { $$ = do_math ('+', $1, $3); }
            | '-' expression %prec UNARY_MIN { $$ = do_math (C_UNARY_MIN, $2, NULL); }
            | expression '-' expression      { $$ = do_math ('-', $1, $3); }
            | expression '*' expression      { $$ = do_math ('*', $1, $3); }
            | expression '/' expression      { $$ = do_math ('/', $1, $3); }
            | expression '%' expression      { $$ = do_math ('%', $1, $3); }
            | expression '^' expression      { $$ = do_math ('^', $1, $3); }
            | expression '&' expression      { $$ = do_math ('&', $1, $3); }
            | expression '|' expression      { $$ = do_math ('|', $1, $3); }
            |            '!' expression      { $$ = do_math ('!', $2, NULL); }
            |            '~' expression      { $$ = do_math ('~', $2, NULL); }
            | expression T_LL expression     { $$ = do_math (C_LL, $1, $3); }
            | expression T_GG expression     { $$ = do_math (C_GG, $1, $3); }
            | '(' error ')'                  { $$ = NULL; yyerror ("malformed expression"); }  
            ;

labels      : label
            | label labels
            ;

label       : symdef T_LABEL {
                if (NOT_FOUND == add_label ($2, current_segment, offset, 
				     $1.global, $1.align8)) {
		    yyerror ("fatal error");
		    YYABORT;
		}
            }
            ;

symdef      : 
T_GLOBAL { 
    $$.align8 = 0; $$.global = 1;
}
| T_ALIGN8 {
    if (current_segment != DEFAULT_SEGMENT) report (0, ALIGNSEGMENT);
    $$.align8 = 1; $$.global = 0;
}
| T_PAGE   {
    if (current_segment != DEFAULT_SEGMENT) report (0, ALIGNSEGMENT);
    $$.align8 = 2; $$.global = 0; 
}
| T_ALIGN8 T_GLOBAL {
    if (current_segment != DEFAULT_SEGMENT) report (0, ALIGNSEGMENT);
    $$.align8 = 1; $$.global = 1;
}
| T_GLOBAL T_ALIGN8 {
    if (current_segment != DEFAULT_SEGMENT) report (0, ALIGNSEGMENT);
    $$.align8 = 1; $$.global = 1;
}
| T_PAGE T_GLOBAL {
    if (current_segment != DEFAULT_SEGMENT) report (0, ALIGNSEGMENT);
    $$.align8 = 2; $$.global = 1;
}
| T_GLOBAL T_PAGE {
    if (current_segment != DEFAULT_SEGMENT) report (0, ALIGNSEGMENT);
    $$.align8 = 2; $$.global = 1;
}
| {$$.global = $$.align8 = 0; };

datadef     : T_DEFSTRING T_STRING {
                char *s;
		for (s = $2; *s; s++) {
		    Dword d = *s;
		    emit (d);
		}
                emit (0);
	    }
            | T_DEFWORD size opt_oparen values opt_cparen {
		int i;
		if ($2 < $4.size) {
		    sprintf (error_buffer, "%d initializers given, %"PRIi32" expected",
			     $4.size, $2);
		    yywarning (error_buffer);
		    $4.size = $2;
		}
		for (i = 0; i < $4.size; i += 2) {
		  uint64_t e1 = (uint64_t)($4.data[i]) << 32 >> 32;
		  uint64_t e2 = (uint64_t)($4.data[i + 1]) >> 32;
		  emit_expression ((Expression*)(e1 + e2));
		}
		for (     ; i < $2     ; i++)
		    emit (0);
	    };

opt_oparen: '{' | ;

opt_cparen: '}' | ;

size        : {$$ = 1;}
            | '[' expression ']' {
		if ($2->type != CONSTANT || $2->detail.constant <= 0) {
		    report (1, ARRAY_SIZE);
		    $$ = 0;
		} else
		    $$ = $2->detail.constant;
	    }
| '[' error ']' { $$ = -1;  yyerror ("malformed array size");};

expression_or_selector: expression {
		$$ = $1;
		}
	    | segment {
		$$ = newSelector (MK_SELECTOR ($1, 0, _LDT));
	    }
;

values      : {$$.size = 0; $$.data = NULL;}
            | expression_or_selector {
		$$.data = safe_malloc (2 * sizeof (Dword)); 
		$$.data[0] = (Dword)((int64_t)$1 << 32 >> 32);
		$$.data[1] = (Dword)((int64_t)$1 >> 32);
		$$.size = 2;
	    }
            | values ',' expression_or_selector { 
		$$.data = safe_realloc ($$.data, ($$.size + 2) * sizeof (Dword)); 
		$$.data[$$.size] = (Dword)((int64_t)$3 << 32 >> 32);
		$$.data[$$.size + 1] = (Dword)((int64_t)$3 >> 32);
		$$.size = $$.size + 2;
	    } ;

symbol     : T_ADDRESS {
                int label = use_label ($1, current_segment);
		if (NOT_FOUND == label) {
		    yyerror ("fatal error");
		    YYABORT;
		}
                $$ = label;
            } ;

segment     : T_SEGMENT {
              $$ = lookup_segment ($1); 
	      if (NOT_FOUND == $$) {
		  yyerror ("fatal error");
		  YYABORT;
	      }
	  };

instruction : error instruction
              { yyerror ("unrecognized instruction, trying to recover"); }
            | T_ADD T_GREGISTER ',' T_GREGISTER
	    {emit (BUILD_INSTRUCTION_A (ADD, $2, $4));} /* ADD */

            | T_ADD T_GREGISTER ',' expression
	    {emit (BUILD_INSTRUCTION_A (xADDI, $2, 0));
	      emit_expression ($4);} /* xADDI */

            | T_AND T_GREGISTER ',' T_GREGISTER
	    {emit (BUILD_INSTRUCTION_A (AND, $2, $4));} /* AND */

            | T_AND T_GREGISTER ',' expression 
              {emit (BUILD_INSTRUCTION_A (xANDI, $2, 0));
		emit_expression ($4);} /* xADDI */

            | T_CALL expression	/* xNCALL */
              { 
		  emit (BUILD_INSTRUCTION_A (xNCALL, 0, 0));
		  emit_expression ($2);
              }

            | T_CALL '[' T_GREGISTER ']'
	    {emit (BUILD_INSTRUCTION_A (NCALLX, $3, 0));} /* NCALLX */

            | T_CALL '[' error ']'
              { report (1, INDIRECTION); }

            | T_CALL segment ':' expression
              {
		  Selector s = MK_SELECTOR ($2, 0, _LDT); 
		  /* the next instruction contains a segment selector 
		     that must be adjusted if there is more than one module
		     in the program */
		  if (module_type == CLOF_EXE) {
		      emit_escape (FIX_SEGMENT); 
                      current_overhead++; /* NOT TESTED */
                  }
		  emit (BUILD_INSTRUCTION_C (xFCALL, 0, s));
		  emit_expression ($4);
	      } /* xFCALL */

            | T_CALL segment	/* for doors */
              {
		  Selector s = MK_SELECTOR ($2, 0, _LDT); 
		  /* the next instruction contains a segment selector 
		     that must be adjusted if there is more than one module
		     in the program */
		  if (module_type == CLOF_EXE) {
		      emit_escape (FIX_SEGMENT); 
                      current_overhead++; /*NOT TESTED*/
                  }
		  emit (BUILD_INSTRUCTION_C (xFCALL, 0, s));
		  emit_expression (NULL);
	      } /* xFCALL */

            | T_CHIO expression
              { emit_displacement (CHIO, 0, $2, 0); } /* CHIO */

            | T_CLC 
	    { emit (BUILD_INSTRUCTION_A (CLC, 0, 0));} /* CLC */

            | T_CLI 
	    { emit (BUILD_INSTRUCTION_A (CLI, 0, 0));} /* CLI */

            | T_CLRB T_GREGISTER ',' expression 
              { emit_displacement (CLRBI, $2, $4, 0); /* CLRBI */ }

            | T_CLRB T_GREGISTER ','  T_GREGISTER
	    { emit (BUILD_INSTRUCTION_A (CLRB, $2, $4));} /* CLRB */

            | T_CMP T_GREGISTER ',' T_GREGISTER
	    { emit (BUILD_INSTRUCTION_A (CMP, $2, $4));} /* CMP */

            | T_CMP T_GREGISTER ',' expression
              { 
		  emit (BUILD_INSTRUCTION_A (xCMPI, $2, 0));
		  emit_expression ($4);
	      } /* xCMPI */

            | T_DEC T_GREGISTER
	    {emit (BUILD_INSTRUCTION_A (DEC, $2, 0));} /* DEC */

            | T_DIV T_GREGISTER ',' T_GREGISTER
	    {emit (BUILD_INSTRUCTION_A (DIV, $2, $4));} /* DIV */

            | T_DIV T_GREGISTER ',' expression
              {emit (BUILD_INSTRUCTION_A (xDIVI, $2, 0));
		emit_expression ($4);} /* xDIVI */

            | T_DIV expression ',' T_GREGISTER
              {emit (BUILD_INSTRUCTION_A (xDIVJ, $4, 0));
		emit_expression ($2);} /* xDIVJ */

            | T_REM T_GREGISTER ',' T_GREGISTER
	    {emit (BUILD_INSTRUCTION_A (REM, $2, $4));} /* REM */

            | T_REM T_GREGISTER ',' expression
              {emit (BUILD_INSTRUCTION_A (xREMI, $2, 0));
		emit_expression ($4);} /* xREMI */

            | T_REM expression ',' T_GREGISTER
              {emit (BUILD_INSTRUCTION_A (xREMJ, $4, 0));
		emit_expression ($2);} /* xREMJ */

            | T_GETB T_GREGISTER ',' T_GREGISTER
	    {emit (BUILD_INSTRUCTION_A (GETB, $2, $4));} /* GETB */

            | T_GETB T_GREGISTER ',' expression
              { emit_displacement (GETBI, $2, $4, 0); } /* GETBI */

| T_HLT {emit (BUILD_INSTRUCTION_A (HLT, 0, 0));} /* HLT */ 

            | T_IN T_GREGISTER ',' '?' expression
              { emit_displacement (IN, $2, $5, 0);} /* IN */

            | T_INC T_GREGISTER
	    {emit (BUILD_INSTRUCTION_A (INC, $2, 0));} /* INC */

            | T_JMP expression	/* absolute address */
              { 
		  emit (BUILD_INSTRUCTION_A (xNJMP, 0, 0));
		  emit_expression ($2);
              }

            | T_JMP segment ':' expression
              {
		  Selector s = MK_SELECTOR ($2, 0, _LDT); 
		  /* the next instruction contains a segment selector 
		     that must be adjusted if there is more than one module
		     in the program */
		  if (module_type == CLOF_EXE) { /*NOT TESTED */
		      emit_escape (FIX_SEGMENT);
                      current_overhead++; 
                  }
		  emit (BUILD_INSTRUCTION_C (xFJMP, 0, s));
		  emit_expression ($4);
	      } /* xFJMP */

            | T_JMP segment
              {
		  Selector s = MK_SELECTOR ($2, 0, _LDT); 
		  /* the next instruction contains a segment selector 
		     that must be adjusted if there is more than one module
		     in the program */
		  if (module_type == CLOF_EXE) { /*NOT TESTED */
		      emit_escape (FIX_SEGMENT); 
                      current_overhead++; 
                  }
		  emit (BUILD_INSTRUCTION_C (xFJMP, 0, s));
		  emit_expression (NULL);
	      } /* xFJMP */

            | T_JNZ expression
              { emit_displacement (JNZ, 0, $2, 1); }
            | T_JZ expression
              { emit_displacement (JZ, 0, $2, 1); }
            | T_JNC expression
              { emit_displacement (JNC, 0, $2, 1); }
            | T_JC expression
              { emit_displacement (JC, 0, $2, 1); }
            | T_JNO expression
              { emit_displacement (JNO, 0, $2, 1); }
            | T_JO expression
              { emit_displacement (JO, 0, $2, 1); }
            | T_JNS expression
              { emit_displacement (JNS, 0, $2, 1); }
            | T_JS expression
              { emit_displacement (JS, 0, $2, 1); }

            | T_MOV T_GREGISTER ',' '['  expression ']' /* xLD */
              { 
		  emit (BUILD_INSTRUCTION_A (xLD, $2, 0));
		  emit_expression ($5);
              }
            | T_MOV T_GREGISTER ',' '['  error ']'
              { report (1, INDIRECTION); }

            | T_MOV error ',' T_GREGISTER
              { yyerror ("Did you forget [] around the variable name?"); } 

            | T_MOV T_GREGISTER ',' expression
              {
               emit (BUILD_INSTRUCTION_A (xLDI, $2, 0));
               emit_expression ($4);
	       } /* XLDI */

            | T_MOV '[' expression ']' ',' T_GREGISTER
              { 
		  emit (BUILD_INSTRUCTION_A (xST, $6, 0));
		  emit_expression ($3);
              }
            | T_MOV '[' error ']' ',' T_GREGISTER
              { report (1, INDIRECTION); }

            | T_MOV T_SREGISTER ',' T_GREGISTER
              {
                  if ($2==_CODE) { report (1, BADUSEOFCS); YYABORT; }
                  emit (BUILD_INSTRUCTION_A (MOVTS, $2, $4));
              } /* MOVTS */

            | T_MOV T_GREGISTER ',' T_SREGISTER
	    {emit (BUILD_INSTRUCTION_A (MOVFS, $2, $4));} /* MOVFS */

            | T_MOV T_GREGISTER ',' T_GREGISTER
	    {emit (BUILD_INSTRUCTION_A (MOV, $2, $4));} /* MOV */

            | T_MOV T_SREGISTER ',' segment 
              {
		  Selector s;
                  if ($2==_CODE) { report (1, BADUSEOFCS); YYABORT; }
		  s = MK_SELECTOR ($4, 0, _LDT); 
		  /* the next instruction contains a segment selector 
		     that must be adjusted if there is more than one module
		     in the program */
		  if (module_type == CLOF_EXE) {
		      emit_escape (FIX_SEGMENT); /*NOT TESTED*/
                      current_overhead++; 
                  }
		  emit (BUILD_INSTRUCTION_C (MOVSI, $2, s)); /*WORKS*/
	      } /* MOVSI */

            | T_MOV T_GREGISTER ',' '[' T_GREGISTER ']'
	    {emit (BUILD_INSTRUCTION_A (LDX, $2, $5));} /* LDX */

            | T_MOV '[' T_GREGISTER ']' ','  T_GREGISTER
	    {emit (BUILD_INSTRUCTION_A (STX, $3, $6));} /* STX */

            | T_MOV T_GREGISTER ',' '[' T_SREGISTER ':' expression ']'
              {
		  emit (BUILD_INSTRUCTION_A (xLDS, $2, $5));
                  emit_expression ($7);
	      } /* xLDS */

            | T_MOV '[' T_SREGISTER ':' expression ']' ',' T_GREGISTER 
              {
                  if ($3==_CODE) { report (1, BADUSEOFCS); YYABORT; }
		  emit (BUILD_INSTRUCTION_A (xSTS, $3, $8));
                  emit_expression ($5);
	      } /* xSTS */

            | T_MOV T_GREGISTER '(' expression ')' ',' T_GREGISTER
              {
		  if ($4->type != CONSTANT) {
		      report (1, VARINDEX);
		  } else {
		      emit (BUILD_INSTRUCTION_D (SETBY, $7, $2, $4->detail.constant));
		  } /* SETBY */
	      }

            | T_MOV T_GREGISTER ',' T_GREGISTER '(' expression ')'
              {
		  if ($6->type != CONSTANT) {
		      report (1, VARINDEX);
		  } else {
		      emit (BUILD_INSTRUCTION_D (GETBY, $2, $4, $6->detail.constant));
		  } /* GETBY */
	      }

            | T_MUL T_GREGISTER ',' T_GREGISTER
	    {emit (BUILD_INSTRUCTION_A (MUL, $2, $4));} /* MUL */

            | T_MUL T_GREGISTER ',' expression
              {
		  emit (BUILD_INSTRUCTION_A (xMULI, $2, 0));
		  emit_expression ($4);
	      } /* xMULI */

            | T_NEG T_GREGISTER
	    {emit (BUILD_INSTRUCTION_A (NEG, $2, 0));} /* NEG */

            | T_NOP 
	    {emit (BUILD_INSTRUCTION_A (NOP, 0, 0));} /* NOP */

            | T_NOT T_GREGISTER 
	    {emit (BUILD_INSTRUCTION_A (NOT, $2, 0));} /* NOT */

            | T_OR T_GREGISTER ',' T_GREGISTER
	    {emit (BUILD_INSTRUCTION_A (OR, $2, $4));} /* OR */

            | T_OR T_GREGISTER ',' expression
              {
		  emit (BUILD_INSTRUCTION_A (xORI, $2, 0));
		  emit_expression ($4);
	      } /* xORI */

            | T_OUT T_GREGISTER ',' '?' expression
              { emit_displacement (OUT, $2, $5, 0); } /* OUT */

            | T_OUT expression ',' '?' expression
              { 
		  emit_displacement (xOUTI, 0, $5, 0); 
		  emit_expression ($2);
	      } /* xOUTI */

            | T_POP T_GREGISTER
	    {emit (BUILD_INSTRUCTION_A (POP, $2, 0));} /* POP */

            | T_POPF 
	    {emit (BUILD_INSTRUCTION_A (POPF, 0, 0));} /* POPF */

            | T_PUSH T_GREGISTER
	    {emit (BUILD_INSTRUCTION_A (PUSH, $2, 0));} /* PUSH */

            | T_PUSH expression
              {
		  emit (BUILD_INSTRUCTION_A (xPUSHI, 0, 0));
		  emit_expression ($2);
	      } /* xPUSHI */

            | T_PEEK T_GREGISTER ',' expression	/* PEEK */
              { emit_displacement (PEEK, $2, $4, 0); }

            | T_POKE T_GREGISTER ',' expression	/* POKE */
              { emit_displacement (POKE, $2, $4, 0); }

            | T_PUSHF 
	    {emit (BUILD_INSTRUCTION_A (PUSHF, 0, 0));} /* PUSHF */

            | T_RETN 
	    {emit (BUILD_INSTRUCTION_A (NRET, 0, 0));} /* RETN */

            | T_RETNI
	    {emit (BUILD_INSTRUCTION_A (INRET, 0, 0));} /* RETNI */

            | T_RETF 
	    { emit (BUILD_INSTRUCTION_A (FRET, 0, 0));} /* RETF */

            | T_RETFI 
	    { emit (BUILD_INSTRUCTION_A (IFRET, 0, 0));} /* RETFI */

            | T_ROL T_GREGISTER ',' T_GREGISTER
	    { emit (BUILD_INSTRUCTION_A (ROL, $2, $4));} /* ROL */

            | T_ROL T_GREGISTER ',' expression
              { emit_displacement (ROLI, $2, $4, 0); } /* ROLI */

            | T_ROR T_GREGISTER ',' T_GREGISTER 
	    { emit (BUILD_INSTRUCTION_A (ROR, $2, $4));} /* ROR */

            | T_ROR T_GREGISTER ',' expression
              { emit_displacement (RORI, $2, $4, 0); } /* RORI */

            | T_SAL T_GREGISTER ',' T_GREGISTER
	    {emit (BUILD_INSTRUCTION_A (SAL, $2, $4));} /* SAL */

            | T_SAL T_GREGISTER ',' expression
              { emit_displacement (SALI, $2, $4, 0); } /* SALI */

            | T_SAR T_GREGISTER ',' T_GREGISTER
	    { emit (BUILD_INSTRUCTION_A (SAR, $2, $4));} /* SAR */

            | T_SAR T_GREGISTER ',' expression
              { emit_displacement (SARI, $2, $4, 0); } /* SARI */

            | T_SETB T_GREGISTER ',' T_GREGISTER
	    { emit (BUILD_INSTRUCTION_A (SETB, $2, $4));} /* SETB */

            | T_SETB T_GREGISTER ',' expression
              { emit_displacement (SETBI, $2, $4, 0); } /* SETBI */

| T_STC {emit (BUILD_INSTRUCTION_A (STC, 0, 0));} /* STC */

            | T_STI 
	    {emit (BUILD_INSTRUCTION_A (STI, 0, 0));} /* STI */

            | T_TRAP
	    {emit (BUILD_INSTRUCTION_A (TRAP, 0, 0));} /* TRAP */

            | T_STOP 
	    {emit (BUILD_INSTRUCTION_A (STOP, 0, 0));} /* STOP */

            | T_SUB T_GREGISTER ',' T_GREGISTER
	    {emit (BUILD_INSTRUCTION_A (SUB, $2, $4));} /* SUB */

            | T_SUB T_GREGISTER ',' expression
              {
		  emit (BUILD_INSTRUCTION_A (xSUBI, $2, 0));
		  emit_expression ($4);
	      } /* xSUBI */

            | T_TST T_GREGISTER ',' T_GREGISTER
	    {emit (BUILD_INSTRUCTION_A (TST, $2, $4));} /* TST */

            | T_TST T_GREGISTER ',' expression
              {
		  emit (BUILD_INSTRUCTION_A (xTSTI, $2, 0));
		  emit_expression ($4);
	      } /* xTSTI */

            | T_XCHG T_GREGISTER ',' '[' expression ']' /* xXCHG */
              { 
		  emit (BUILD_INSTRUCTION_A (xXCHG, $2, 0));
		  emit_expression ($5);
              }
            | T_XCHG T_GREGISTER ',' '[' error ']' /* xXCHG */
              { report (1, INDIRECTION); }

            | T_XOR T_GREGISTER ',' T_GREGISTER
	    {emit (BUILD_INSTRUCTION_A (XOR, $2, $4));} /* XOR */

            | T_XOR T_GREGISTER ',' expression
              {
		  emit (BUILD_INSTRUCTION_A (xXORI, $2, 0));
		  emit_expression ($4);
	      } /* xXORI */

;

%%

static void emit_expression (Expression *e) 
{
    if (!e) {		/* zero constant */
	emit (0);
	return;
    }
    switch (e->type) {
    case CONSTANT:
	emit (e->detail.constant);
	break;
    case EXPRESSION:
    case LABEL:
    case SELECTOR:
	emit_escape (FIX_EXPRESSION);
	emit (0);		/* fake instruction */
	
	Uword d1 = (Uword)((uint64_t)(e) << 32 >> 32);
	Uword d2 = (Uword)((uint64_t)(e) >> 32);
	emit_escape (d1);
	emit_escape (d2);

	current_overhead += 2 + expression_overhead (e);
	break;	
    case DUMMY:
	assert (e->type != DUMMY);
    }
}

static void emit_displacement (int opc, int op1, Expression *dspl, Bit relative) 
{
    emit_escape (relative ? FIX_RDISPLACEMENT : FIX_ADISPLACEMENT);
    emit (BUILD_INSTRUCTION_B (opc, op1, 0));

    Uword d1 = (Uword)((uint64_t)(dspl) << 32 >> 32);
    Uword d2 = (Uword)((uint64_t)(dspl) >> 32);
    /*
      fprintf(stderr, "IN: %p %p %p %d\n", dspl, d1, d2, current_overhead);
    */
    emit_escape (d1);
    emit_escape (d2);
    if (relative) {
	emit_escape (offset);   /* current_offset */
    } else {
	current_overhead += 2 + expression_overhead (dspl);
    }
}

void yywarning (char *s)
{
    fprintf (stderr, "warning:%s:%d: %s\n", *source, line_no, s);
}

int yyerror (char *s)
{
    fprintf (stderr, "%s:%d: %s\n", *source, line_no, s);
    success = 0;
    return 0;
}

