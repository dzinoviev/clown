
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     T_GG = 258,
     T_LL = 259,
     UNARY_MIN = 260,
     T_ADD = 261,
     T_AND = 262,
     T_CALL = 263,
     T_CHIO = 264,
     T_CLC = 265,
     T_CLI = 266,
     T_CLRB = 267,
     T_CMP = 268,
     T_DEC = 269,
     T_DIV = 270,
     T_REM = 271,
     T_GETB = 272,
     T_HLT = 273,
     T_IN = 274,
     T_INC = 275,
     T_JC = 276,
     T_JMP = 277,
     T_JNC = 278,
     T_JNO = 279,
     T_JNS = 280,
     T_JNZ = 281,
     T_JO = 282,
     T_JS = 283,
     T_JZ = 284,
     T_MOV = 285,
     T_MUL = 286,
     T_NEG = 287,
     T_NOP = 288,
     T_NOT = 289,
     T_OR = 290,
     T_OUT = 291,
     T_PEEK = 292,
     T_POKE = 293,
     T_POP = 294,
     T_POPF = 295,
     T_PUSH = 296,
     T_PUSHF = 297,
     T_RETF = 298,
     T_RETN = 299,
     T_RETFI = 300,
     T_RETNI = 301,
     T_ROL = 302,
     T_ROR = 303,
     T_SAL = 304,
     T_SAR = 305,
     T_SETB = 306,
     T_STC = 307,
     T_STI = 308,
     T_STOP = 309,
     T_SUB = 310,
     T_TRAP = 311,
     T_TST = 312,
     T_XCHG = 313,
     T_XOR = 314,
     T_NUMBER = 315,
     T_ADDRESS = 316,
     T_LABEL = 317,
     T_GREGISTER = 318,
     T_SREGISTER = 319,
     T_SEGMENT = 320,
     T_STRING = 321,
     T_DEFSTRING = 322,
     T_DEFWORD = 323,
     T_ERROR = 324,
     T_GLOBAL = 325,
     T_ALIGN8 = 326,
     T_PAGE = 327,
     T_CODE = 328,
     T_DATA = 329,
     T_CONST = 330
   };
#endif
/* Tokens.  */
#define T_GG 258
#define T_LL 259
#define UNARY_MIN 260
#define T_ADD 261
#define T_AND 262
#define T_CALL 263
#define T_CHIO 264
#define T_CLC 265
#define T_CLI 266
#define T_CLRB 267
#define T_CMP 268
#define T_DEC 269
#define T_DIV 270
#define T_REM 271
#define T_GETB 272
#define T_HLT 273
#define T_IN 274
#define T_INC 275
#define T_JC 276
#define T_JMP 277
#define T_JNC 278
#define T_JNO 279
#define T_JNS 280
#define T_JNZ 281
#define T_JO 282
#define T_JS 283
#define T_JZ 284
#define T_MOV 285
#define T_MUL 286
#define T_NEG 287
#define T_NOP 288
#define T_NOT 289
#define T_OR 290
#define T_OUT 291
#define T_PEEK 292
#define T_POKE 293
#define T_POP 294
#define T_POPF 295
#define T_PUSH 296
#define T_PUSHF 297
#define T_RETF 298
#define T_RETN 299
#define T_RETFI 300
#define T_RETNI 301
#define T_ROL 302
#define T_ROR 303
#define T_SAL 304
#define T_SAR 305
#define T_SETB 306
#define T_STC 307
#define T_STI 308
#define T_STOP 309
#define T_SUB 310
#define T_TRAP 311
#define T_TST 312
#define T_XCHG 313
#define T_XOR 314
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




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 98 "cas-yacc.y"

    char  *s;
    Dword i;
    struct {
	int size;
	Dword *data;
    } v;
    struct labelDef sym;
    struct _Expression *expr;



/* Line 1676 of yacc.c  */
#line 215 "cas-yacc.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


