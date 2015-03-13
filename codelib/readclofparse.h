
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
     T_BIN = 258,
     T_CLOFEXE = 259,
     T_CLEFEXE = 260,
     T_CODESTAR = 261,
     T_DEFINED = 262,
     T_FILE = 263,
     T_GLOBAL = 264,
     T_ID = 265,
     T_LINE = 266,
     T_LINENO = 267,
     T_NAME = 268,
     T_OFFSET = 269,
     T_SEGMENT = 270,
     T_SEGMENTS = 271,
     T_SYMBOL = 272,
     T_SYMBOLS = 273,
     T_ESYMBOLS = 274,
     T_TYPE = 275,
     T_LS = 276,
     T_SG = 277,
     T_L = 278,
     T_G = 279,
     T_CODE = 280,
     T_STRING = 281,
     T_NUMBER = 282
   };
#endif
/* Tokens.  */
#define T_BIN 258
#define T_CLOFEXE 259
#define T_CLEFEXE 260
#define T_CODESTAR 261
#define T_DEFINED 262
#define T_FILE 263
#define T_GLOBAL 264
#define T_ID 265
#define T_LINE 266
#define T_LINENO 267
#define T_NAME 268
#define T_OFFSET 269
#define T_SEGMENT 270
#define T_SEGMENTS 271
#define T_SYMBOL 272
#define T_SYMBOLS 273
#define T_ESYMBOLS 274
#define T_TYPE 275
#define T_LS 276
#define T_SG 277
#define T_L 278
#define T_G 279
#define T_CODE 280
#define T_STRING 281
#define T_NUMBER 282




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 14 "readclofparse.y"

    char *s;
    int i;
    struct {
	int size;
	int escapes;
	Dword *bin;
    } code;
    struct Segment seg;
    struct Label sym;
    struct MyDebug di;
    struct DebugFile df;
    struct DebugInfo dl;



/* Line 1676 of yacc.c  */
#line 123 "readclofparse.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


