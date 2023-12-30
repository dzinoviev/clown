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

#ifndef YY_YY_READCLOFPARSE_H_INCLUDED
# define YY_YY_READCLOFPARSE_H_INCLUDED
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
    T_BIN = 258,                   /* T_BIN  */
    T_CLOFEXE = 259,               /* T_CLOFEXE  */
    T_CLEFEXE = 260,               /* T_CLEFEXE  */
    T_CODESTAR = 261,              /* T_CODESTAR  */
    T_DEFINED = 262,               /* T_DEFINED  */
    T_FILE = 263,                  /* T_FILE  */
    T_GLOBAL = 264,                /* T_GLOBAL  */
    T_ID = 265,                    /* T_ID  */
    T_LINE = 266,                  /* T_LINE  */
    T_LINENO = 267,                /* T_LINENO  */
    T_NAME = 268,                  /* T_NAME  */
    T_OFFSET = 269,                /* T_OFFSET  */
    T_SEGMENT = 270,               /* T_SEGMENT  */
    T_SEGMENTS = 271,              /* T_SEGMENTS  */
    T_SYMBOL = 272,                /* T_SYMBOL  */
    T_SYMBOLS = 273,               /* T_SYMBOLS  */
    T_ESYMBOLS = 274,              /* T_ESYMBOLS  */
    T_TYPE = 275,                  /* T_TYPE  */
    T_LS = 276,                    /* T_LS  */
    T_SG = 277,                    /* T_SG  */
    T_L = 278,                     /* T_L  */
    T_G = 279,                     /* T_G  */
    T_CODE = 280,                  /* T_CODE  */
    T_STRING = 281,                /* T_STRING  */
    T_NUMBER = 282                 /* T_NUMBER  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
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

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
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

#line 136 "readclofparse.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_READCLOFPARSE_H_INCLUDED  */
