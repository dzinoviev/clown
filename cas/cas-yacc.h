/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_CAS_YACC_H_INCLUDED
# define YY_YY_CAS_YACC_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    UNARY_MIN = 258,               /* UNARY_MIN  */
    T_ADD = 259,                   /* T_ADD  */
    T_AND = 260,                   /* T_AND  */
    T_CALL = 261,                  /* T_CALL  */
    T_CHIO = 262,                  /* T_CHIO  */
    T_CLC = 263,                   /* T_CLC  */
    T_CLI = 264,                   /* T_CLI  */
    T_CLRB = 265,                  /* T_CLRB  */
    T_CMP = 266,                   /* T_CMP  */
    T_DEC = 267,                   /* T_DEC  */
    T_DIV = 268,                   /* T_DIV  */
    T_REM = 269,                   /* T_REM  */
    T_GETB = 270,                  /* T_GETB  */
    T_HLT = 271,                   /* T_HLT  */
    T_IN = 272,                    /* T_IN  */
    T_INC = 273,                   /* T_INC  */
    T_JC = 274,                    /* T_JC  */
    T_JMP = 275,                   /* T_JMP  */
    T_JNC = 276,                   /* T_JNC  */
    T_JNO = 277,                   /* T_JNO  */
    T_JNS = 278,                   /* T_JNS  */
    T_JNZ = 279,                   /* T_JNZ  */
    T_JO = 280,                    /* T_JO  */
    T_JS = 281,                    /* T_JS  */
    T_JZ = 282,                    /* T_JZ  */
    T_MOV = 283,                   /* T_MOV  */
    T_MUL = 284,                   /* T_MUL  */
    T_NEG = 285,                   /* T_NEG  */
    T_NOP = 286,                   /* T_NOP  */
    T_NOT = 287,                   /* T_NOT  */
    T_OR = 288,                    /* T_OR  */
    T_OUT = 289,                   /* T_OUT  */
    T_PEEK = 290,                  /* T_PEEK  */
    T_POKE = 291,                  /* T_POKE  */
    T_POP = 292,                   /* T_POP  */
    T_POPF = 293,                  /* T_POPF  */
    T_PUSH = 294,                  /* T_PUSH  */
    T_PUSHF = 295,                 /* T_PUSHF  */
    T_RETF = 296,                  /* T_RETF  */
    T_RETN = 297,                  /* T_RETN  */
    T_RETFI = 298,                 /* T_RETFI  */
    T_RETNI = 299,                 /* T_RETNI  */
    T_ROL = 300,                   /* T_ROL  */
    T_ROR = 301,                   /* T_ROR  */
    T_SAL = 302,                   /* T_SAL  */
    T_SAR = 303,                   /* T_SAR  */
    T_SETB = 304,                  /* T_SETB  */
    T_STC = 305,                   /* T_STC  */
    T_STI = 306,                   /* T_STI  */
    T_STOP = 307,                  /* T_STOP  */
    T_SUB = 308,                   /* T_SUB  */
    T_TRAP = 309,                  /* T_TRAP  */
    T_TST = 310,                   /* T_TST  */
    T_XCHG = 311,                  /* T_XCHG  */
    T_XOR = 312,                   /* T_XOR  */
    T_LL = 313,                    /* T_LL  */
    T_GG = 314,                    /* T_GG  */
    T_NUMBER = 315,                /* T_NUMBER  */
    T_ADDRESS = 316,               /* T_ADDRESS  */
    T_LABEL = 317,                 /* T_LABEL  */
    T_GREGISTER = 318,             /* T_GREGISTER  */
    T_SREGISTER = 319,             /* T_SREGISTER  */
    T_SEGMENT = 320,               /* T_SEGMENT  */
    T_STRING = 321,                /* T_STRING  */
    T_DEFSTRING = 322,             /* T_DEFSTRING  */
    T_DEFWORD = 323,               /* T_DEFWORD  */
    T_ERROR = 324,                 /* T_ERROR  */
    T_GLOBAL = 325,                /* T_GLOBAL  */
    T_ALIGN8 = 326,                /* T_ALIGN8  */
    T_PAGE = 327,                  /* T_PAGE  */
    T_CODE = 328,                  /* T_CODE  */
    T_DATA = 329,                  /* T_DATA  */
    T_CONST = 330                  /* T_CONST  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define UNARY_MIN 258
#define T_ADD 259
#define T_AND 260
#define T_CALL 261
#define T_CHIO 262
#define T_CLC 263
#define T_CLI 264
#define T_CLRB 265
#define T_CMP 266
#define T_DEC 267
#define T_DIV 268
#define T_REM 269
#define T_GETB 270
#define T_HLT 271
#define T_IN 272
#define T_INC 273
#define T_JC 274
#define T_JMP 275
#define T_JNC 276
#define T_JNO 277
#define T_JNS 278
#define T_JNZ 279
#define T_JO 280
#define T_JS 281
#define T_JZ 282
#define T_MOV 283
#define T_MUL 284
#define T_NEG 285
#define T_NOP 286
#define T_NOT 287
#define T_OR 288
#define T_OUT 289
#define T_PEEK 290
#define T_POKE 291
#define T_POP 292
#define T_POPF 293
#define T_PUSH 294
#define T_PUSHF 295
#define T_RETF 296
#define T_RETN 297
#define T_RETFI 298
#define T_RETNI 299
#define T_ROL 300
#define T_ROR 301
#define T_SAL 302
#define T_SAR 303
#define T_SETB 304
#define T_STC 305
#define T_STI 306
#define T_STOP 307
#define T_SUB 308
#define T_TRAP 309
#define T_TST 310
#define T_XCHG 311
#define T_XOR 312
#define T_LL 313
#define T_GG 314
#define T_NUMBER 315
#define T_ADDRESS 316
#define T_LABEL 317
#define T_GREGISTER 318
#define T_SREGISTER 319
#define T_SEGMENT 320
#define T_STRING 321
#define T_DEFSTRING 322
#define T_DEFWORD 323
#define T_ERROR 324
#define T_GLOBAL 325
#define T_ALIGN8 326
#define T_PAGE 327
#define T_CODE 328
#define T_DATA 329
#define T_CONST 330

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 83 "cas-yacc.y"

    char  *s;
    Dword i;
    struct {
	int size;
	Dword *data;
    } v;
    struct labelDef sym;
    struct _Expression *expr;

#line 228 "cas-yacc.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_CAS_YACC_H_INCLUDED  */
