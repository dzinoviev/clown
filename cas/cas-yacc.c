/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "cas-yacc.y"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cas.h"
#include "registers.h"

void yywarning(char *s);
static void emit_expression(Expression *target);
static void emit_displacement(int opc, Dword op1, Expression *e, Bit relative);

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

static void report(int severe, int id)
{
  char *message;
  if (_errors[id].count > 0)
    message = _errors[id].concise;
  else
    message = _errors[id].full;
  if (severe)
    yyerror(message);
  else
    yywarning(message);
  _errors[id].count++;
}


#line 154 "cas-yacc.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
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
#line 84 "cas-yacc.y"

  char  *s;
  Dword i;
  struct {
    int size;
    Dword *data;
  } v;
  struct labelDef sym;
  struct _Expression *expr;

#line 368 "cas-yacc.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_3_ = 3,                         /* '|'  */
  YYSYMBOL_4_ = 4,                         /* '^'  */
  YYSYMBOL_5_ = 5,                         /* '&'  */
  YYSYMBOL_6_ = 6,                         /* '+'  */
  YYSYMBOL_7_ = 7,                         /* '-'  */
  YYSYMBOL_8_ = 8,                         /* '*'  */
  YYSYMBOL_9_ = 9,                         /* '/'  */
  YYSYMBOL_10_ = 10,                       /* '%'  */
  YYSYMBOL_UNARY_MIN = 11,                 /* UNARY_MIN  */
  YYSYMBOL_12_ = 12,                       /* '!'  */
  YYSYMBOL_13_ = 13,                       /* '~'  */
  YYSYMBOL_T_ADD = 14,                     /* T_ADD  */
  YYSYMBOL_T_AND = 15,                     /* T_AND  */
  YYSYMBOL_T_CALL = 16,                    /* T_CALL  */
  YYSYMBOL_T_CHIO = 17,                    /* T_CHIO  */
  YYSYMBOL_T_CLC = 18,                     /* T_CLC  */
  YYSYMBOL_T_CLI = 19,                     /* T_CLI  */
  YYSYMBOL_T_CLRB = 20,                    /* T_CLRB  */
  YYSYMBOL_T_CMP = 21,                     /* T_CMP  */
  YYSYMBOL_T_DEC = 22,                     /* T_DEC  */
  YYSYMBOL_T_DIV = 23,                     /* T_DIV  */
  YYSYMBOL_T_REM = 24,                     /* T_REM  */
  YYSYMBOL_T_GETB = 25,                    /* T_GETB  */
  YYSYMBOL_T_HLT = 26,                     /* T_HLT  */
  YYSYMBOL_T_IN = 27,                      /* T_IN  */
  YYSYMBOL_T_INC = 28,                     /* T_INC  */
  YYSYMBOL_T_JC = 29,                      /* T_JC  */
  YYSYMBOL_T_JMP = 30,                     /* T_JMP  */
  YYSYMBOL_T_JNC = 31,                     /* T_JNC  */
  YYSYMBOL_T_JNO = 32,                     /* T_JNO  */
  YYSYMBOL_T_JNS = 33,                     /* T_JNS  */
  YYSYMBOL_T_JNZ = 34,                     /* T_JNZ  */
  YYSYMBOL_T_JO = 35,                      /* T_JO  */
  YYSYMBOL_T_JS = 36,                      /* T_JS  */
  YYSYMBOL_T_JZ = 37,                      /* T_JZ  */
  YYSYMBOL_T_MOV = 38,                     /* T_MOV  */
  YYSYMBOL_T_MUL = 39,                     /* T_MUL  */
  YYSYMBOL_T_NEG = 40,                     /* T_NEG  */
  YYSYMBOL_T_NOP = 41,                     /* T_NOP  */
  YYSYMBOL_T_NOT = 42,                     /* T_NOT  */
  YYSYMBOL_T_OR = 43,                      /* T_OR  */
  YYSYMBOL_T_OUT = 44,                     /* T_OUT  */
  YYSYMBOL_T_PEEK = 45,                    /* T_PEEK  */
  YYSYMBOL_T_POKE = 46,                    /* T_POKE  */
  YYSYMBOL_T_POP = 47,                     /* T_POP  */
  YYSYMBOL_T_POPF = 48,                    /* T_POPF  */
  YYSYMBOL_T_PUSH = 49,                    /* T_PUSH  */
  YYSYMBOL_T_PUSHF = 50,                   /* T_PUSHF  */
  YYSYMBOL_T_RETF = 51,                    /* T_RETF  */
  YYSYMBOL_T_RETN = 52,                    /* T_RETN  */
  YYSYMBOL_T_RETFI = 53,                   /* T_RETFI  */
  YYSYMBOL_T_RETNI = 54,                   /* T_RETNI  */
  YYSYMBOL_T_ROL = 55,                     /* T_ROL  */
  YYSYMBOL_T_ROR = 56,                     /* T_ROR  */
  YYSYMBOL_T_SAL = 57,                     /* T_SAL  */
  YYSYMBOL_T_SAR = 58,                     /* T_SAR  */
  YYSYMBOL_T_SETB = 59,                    /* T_SETB  */
  YYSYMBOL_T_STC = 60,                     /* T_STC  */
  YYSYMBOL_T_STI = 61,                     /* T_STI  */
  YYSYMBOL_T_STOP = 62,                    /* T_STOP  */
  YYSYMBOL_T_SUB = 63,                     /* T_SUB  */
  YYSYMBOL_T_TRAP = 64,                    /* T_TRAP  */
  YYSYMBOL_T_TST = 65,                     /* T_TST  */
  YYSYMBOL_T_XCHG = 66,                    /* T_XCHG  */
  YYSYMBOL_T_XOR = 67,                     /* T_XOR  */
  YYSYMBOL_T_LL = 68,                      /* T_LL  */
  YYSYMBOL_T_GG = 69,                      /* T_GG  */
  YYSYMBOL_T_NUMBER = 70,                  /* T_NUMBER  */
  YYSYMBOL_T_ADDRESS = 71,                 /* T_ADDRESS  */
  YYSYMBOL_T_LABEL = 72,                   /* T_LABEL  */
  YYSYMBOL_T_GREGISTER = 73,               /* T_GREGISTER  */
  YYSYMBOL_T_SREGISTER = 74,               /* T_SREGISTER  */
  YYSYMBOL_T_SEGMENT = 75,                 /* T_SEGMENT  */
  YYSYMBOL_T_STRING = 76,                  /* T_STRING  */
  YYSYMBOL_T_DEFSTRING = 77,               /* T_DEFSTRING  */
  YYSYMBOL_T_DEFWORD = 78,                 /* T_DEFWORD  */
  YYSYMBOL_T_ERROR = 79,                   /* T_ERROR  */
  YYSYMBOL_T_GLOBAL = 80,                  /* T_GLOBAL  */
  YYSYMBOL_T_ALIGN8 = 81,                  /* T_ALIGN8  */
  YYSYMBOL_T_PAGE = 82,                    /* T_PAGE  */
  YYSYMBOL_T_CODE = 83,                    /* T_CODE  */
  YYSYMBOL_T_DATA = 84,                    /* T_DATA  */
  YYSYMBOL_T_CONST = 85,                   /* T_CONST  */
  YYSYMBOL_86_ = 86,                       /* '('  */
  YYSYMBOL_87_ = 87,                       /* ')'  */
  YYSYMBOL_88_ = 88,                       /* '{'  */
  YYSYMBOL_89_ = 89,                       /* '}'  */
  YYSYMBOL_90_ = 90,                       /* '['  */
  YYSYMBOL_91_ = 91,                       /* ']'  */
  YYSYMBOL_92_ = 92,                       /* ','  */
  YYSYMBOL_93_ = 93,                       /* ':'  */
  YYSYMBOL_94_ = 94,                       /* '?'  */
  YYSYMBOL_YYACCEPT = 95,                  /* $accept  */
  YYSYMBOL_program = 96,                   /* program  */
  YYSYMBOL_97_1 = 97,                      /* $@1  */
  YYSYMBOL_lines = 98,                     /* lines  */
  YYSYMBOL_line = 99,                      /* line  */
  YYSYMBOL_segtype = 100,                  /* segtype  */
  YYSYMBOL_expression = 101,               /* expression  */
  YYSYMBOL_labels = 102,                   /* labels  */
  YYSYMBOL_label = 103,                    /* label  */
  YYSYMBOL_symdef = 104,                   /* symdef  */
  YYSYMBOL_datadef = 105,                  /* datadef  */
  YYSYMBOL_opt_oparen = 106,               /* opt_oparen  */
  YYSYMBOL_opt_cparen = 107,               /* opt_cparen  */
  YYSYMBOL_size = 108,                     /* size  */
  YYSYMBOL_expression_or_selector = 109,   /* expression_or_selector  */
  YYSYMBOL_values = 110,                   /* values  */
  YYSYMBOL_symbol = 111,                   /* symbol  */
  YYSYMBOL_segment = 112,                  /* segment  */
  YYSYMBOL_instruction = 113               /* instruction  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   946

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  95
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  19
/* YYNRULES -- Number of rules.  */
#define YYNRULES  155
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  316

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   330


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    12,     2,     2,     2,    10,     5,     2,
      86,    87,     8,     6,    92,     7,     2,     9,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    93,     2,
       2,     2,     2,    94,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    90,     2,    91,     4,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    88,     3,    89,    13,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,    11,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   192,   192,   192,   199,   200,   202,   213,   214,   215,
     216,   219,   220,   221,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   235,   236,   237,   238,   239,
     240,   243,   244,   247,   257,   260,   264,   268,   272,   276,
     280,   284,   286,   294,   311,   311,   313,   313,   315,   316,
     323,   325,   328,   333,   334,   341,   349,   358,   366,   368,
     371,   375,   378,   382,   388,   391,   394,   407,   420,   423,
     426,   429,   432,   435,   438,   444,   447,   450,   454,   458,
     461,   465,   469,   472,   475,   477,   480,   483,   489,   502,
     515,   517,   519,   521,   523,   525,   527,   529,   532,   538,
     541,   544,   550,   555,   558,   564,   567,   570,   584,   587,
     590,   596,   603,   612,   621,   624,   630,   633,   636,   639,
     642,   648,   651,   657,   660,   663,   666,   672,   675,   678,
     681,   684,   687,   690,   693,   696,   699,   702,   705,   708,
     711,   714,   717,   720,   723,   726,   729,   732,   735,   738,
     744,   747,   753,   758,   761,   764
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "'|'", "'^'", "'&'",
  "'+'", "'-'", "'*'", "'/'", "'%'", "UNARY_MIN", "'!'", "'~'", "T_ADD",
  "T_AND", "T_CALL", "T_CHIO", "T_CLC", "T_CLI", "T_CLRB", "T_CMP",
  "T_DEC", "T_DIV", "T_REM", "T_GETB", "T_HLT", "T_IN", "T_INC", "T_JC",
  "T_JMP", "T_JNC", "T_JNO", "T_JNS", "T_JNZ", "T_JO", "T_JS", "T_JZ",
  "T_MOV", "T_MUL", "T_NEG", "T_NOP", "T_NOT", "T_OR", "T_OUT", "T_PEEK",
  "T_POKE", "T_POP", "T_POPF", "T_PUSH", "T_PUSHF", "T_RETF", "T_RETN",
  "T_RETFI", "T_RETNI", "T_ROL", "T_ROR", "T_SAL", "T_SAR", "T_SETB",
  "T_STC", "T_STI", "T_STOP", "T_SUB", "T_TRAP", "T_TST", "T_XCHG",
  "T_XOR", "T_LL", "T_GG", "T_NUMBER", "T_ADDRESS", "T_LABEL",
  "T_GREGISTER", "T_SREGISTER", "T_SEGMENT", "T_STRING", "T_DEFSTRING",
  "T_DEFWORD", "T_ERROR", "T_GLOBAL", "T_ALIGN8", "T_PAGE", "T_CODE",
  "T_DATA", "T_CONST", "'('", "')'", "'{'", "'}'", "'['", "']'", "','",
  "':'", "'?'", "$accept", "program", "$@1", "lines", "line", "segtype",
  "expression", "labels", "label", "symdef", "datadef", "opt_oparen",
  "opt_cparen", "size", "expression_or_selector", "values", "symbol",
  "segment", "instruction", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-84)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-42)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -84,    27,   322,   -84,   813,   -43,    -6,    19,   673,   -84,
     -84,    -5,    -3,     5,   222,   434,    14,   -84,    31,    44,
     673,   448,   673,   673,   673,   673,   673,   673,   673,     1,
      47,    54,   -84,    59,    60,   451,    65,    66,    70,   -84,
     455,   -84,   -84,   -84,   -84,   -84,    73,    74,    75,    90,
      92,   -84,   -84,   -84,    96,   -84,    99,   100,   103,    12,
      17,   -57,    43,   110,   -84,   -84,   -84,   -84,   322,   104,
     746,    49,   108,   -84,   -84,   -84,    30,   101,   673,   673,
     673,   -84,   -84,   -84,   407,     3,   689,   -84,   135,   689,
     138,   140,   -84,   144,    34,   146,    42,   148,   149,   -84,
     689,   689,   151,   689,   689,   689,   689,   689,   689,   689,
     163,   -81,   164,    22,   165,   -84,   -84,   166,   167,    50,
     170,   172,   -84,   -84,   689,   173,   174,   175,   178,   179,
     180,   181,   183,   186,   -84,   409,   191,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   459,   476,   -84,
     -84,   -84,   105,   324,   189,   190,   673,   673,   673,   673,
     673,   673,   673,   673,   673,   673,   673,   499,   503,   526,
     209,   543,   213,   547,   193,   673,   215,   673,   383,   -63,
     198,   199,   203,    76,   551,   568,   204,   211,   673,   673,
     575,   579,   593,   597,   618,   644,   662,   207,   669,   218,
     192,   -84,   448,   -84,   689,   -84,   689,   -84,   -84,   -84,
     -84,   877,    56,   106,   127,   127,   -84,   -84,   -84,    91,
      91,   689,   -84,   689,   -84,   689,   -84,   689,   -84,   -84,
     689,   -84,   -84,   689,   673,   689,   -84,   422,   220,   -84,
     230,   689,   -84,   -84,   210,   219,   673,   223,   -84,   689,
     -84,   689,   673,   673,   689,   689,   -84,   689,   -84,   689,
     -84,   689,   -84,   689,   -84,   689,   -84,   689,   -84,   689,
     411,   -84,   689,   -84,   -84,   689,   -84,   -83,   -84,   689,
     225,   673,   227,   228,   217,   200,   241,   247,   208,   248,
     689,   689,   233,   216,   -84,   448,   -84,   252,   430,   -84,
     -84,   673,   -84,   -84,   -84,   234,   -84,   -84,   -84,   -84,
     -84,   -84,   244,   318,   -84,   -84
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     0,     1,     0,     0,     0,     0,     0,    69,
      70,     0,     0,     0,     0,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   117,     0,     0,     0,     0,     0,     0,   124,
       0,   129,   132,   130,   133,   131,     0,     0,     0,     0,
       0,   144,   145,   147,     0,   146,     0,     0,     0,     0,
      48,    34,    35,    36,    11,    12,    13,     3,     0,     0,
       0,    31,     0,     9,    10,    58,     0,     0,     0,     0,
       0,    14,    56,    57,     0,     0,    63,    15,    67,    68,
       0,     0,    75,     0,     0,     0,     0,     0,     0,    86,
      93,    87,    89,    92,    94,    96,    90,    95,    97,    91,
       0,     0,     0,     0,     0,   116,   118,     0,     0,     0,
       0,     0,   123,   125,   126,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    42,     0,    45,    38,    40,    37,
      39,     4,     6,     7,     8,    32,    33,     0,     0,    18,
      26,    27,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    44,    53,    59,    60,    61,    62,    30,    16,    65,
      64,    25,    23,    24,    17,    19,    20,    21,    22,    28,
      29,    66,    72,    71,    73,    74,    76,    77,    78,    79,
      80,    81,    82,    83,     0,    88,   100,     0,   106,   105,
       0,   101,   104,   107,     0,     0,     0,     0,   114,   115,
     119,   120,     0,     0,   127,   128,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   148,   149,   150,   151,
       0,   154,   155,    50,    49,    51,    54,    47,    52,    85,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     121,   122,     0,     0,    46,     0,    43,     0,     0,    99,
     108,     0,    98,   103,   109,     0,   102,   153,   152,    55,
     112,   113,     0,     0,   110,   111
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -84,   -84,   -84,   329,   -84,   -84,    -7,   327,   -84,   -84,
     331,   -84,   -84,   -84,   114,   -84,   -84,    15,    -1
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,     2,    67,    68,    69,   275,    70,    71,    72,
      73,   202,   296,   136,   276,   277,    87,   278,    74
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      86,    89,   110,    75,   154,   177,   294,    94,    96,   295,
     242,   178,    83,   100,   101,   103,   104,   105,   106,   107,
     108,   109,    88,   180,   137,   138,    78,     3,   119,    78,
      76,    79,    80,   124,    79,    80,   102,   156,   157,   158,
     159,   160,   161,   162,   163,   156,   157,   158,   159,   160,
     161,   162,   163,   156,   157,   158,   159,   160,   161,   162,
     163,   158,   159,   160,   161,   162,   163,    77,    90,   144,
      91,   149,   150,   151,   111,   112,   155,   153,    92,   156,
     157,   158,   159,   160,   161,   162,   163,    97,   134,    81,
      82,   113,    81,    82,    83,   181,   182,   159,   160,   161,
     162,   163,   164,   165,    98,    84,   183,   135,    84,    85,
     164,   165,   159,   160,   161,   162,   163,    99,   164,   165,
     114,   -41,   147,   139,   164,   165,   170,   115,   200,    61,
      62,    63,   116,   117,   172,   161,   162,   163,   120,   121,
     204,   206,   187,   122,   164,   165,   125,   126,   127,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     223,   225,   227,   128,   230,   129,   233,   247,   235,   130,
     237,   241,   131,   132,   164,   165,   133,   249,   251,   142,
     146,   254,   255,   257,   259,   261,   263,   265,   267,   269,
     140,   272,   207,   148,   243,   156,   157,   158,   159,   160,
     161,   162,   163,   156,   157,   158,   159,   160,   161,   162,
     163,   156,   157,   158,   159,   160,   161,   162,   163,   156,
     157,   158,   159,   160,   161,   162,   163,   279,   166,    78,
     167,   282,   168,   285,    79,    80,   169,    78,   171,   288,
     173,   174,    79,    80,   175,   290,   291,   156,   157,   158,
     159,   160,   161,   162,   163,   176,   179,   184,   185,   186,
     164,   165,   188,   293,   189,   190,   191,   192,   164,   165,
     193,   194,   195,   196,   298,   197,   164,   165,   198,   201,
     209,   210,   228,   274,   164,   165,   231,   234,   236,   244,
     245,   302,    81,    82,   312,    93,   246,   270,   252,   305,
      81,    82,   286,   283,   284,   253,   281,   308,    84,   273,
     301,   287,   164,   165,   303,   289,    84,   297,   299,   300,
     304,   306,    -5,     4,   307,   310,   313,   156,   157,   158,
     159,   160,   161,   162,   163,   314,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      78,   315,   164,   165,   -41,    79,    80,   141,   145,    59,
      60,   143,    61,    62,    63,    64,    65,    66,   152,   309,
     199,   208,   292,     0,    78,     0,    78,     0,    78,    79,
      80,    79,    80,    79,    80,   156,   157,   158,   159,   160,
     161,   162,   163,   156,   157,   158,   159,   160,   161,   162,
     163,    78,     0,     0,     0,     0,    79,    80,     0,     0,
       0,     0,     0,    81,    82,    78,   238,   239,    78,     0,
      79,    80,    78,    79,    80,     0,    78,    79,    80,    84,
       0,    79,    80,   240,     0,     0,     0,    81,    82,    81,
      82,    81,    82,    78,     0,     0,     0,     0,    79,    80,
     164,   165,     0,    84,     0,    84,     0,    84,   164,   165,
       0,     0,     0,     0,    81,    82,    78,    95,     0,   280,
      78,    79,    80,     0,     0,    79,    80,   311,    81,    82,
      84,    81,    82,    83,   118,    81,    82,     0,   123,    81,
      82,     0,   203,    78,    84,     0,     0,    84,    79,    80,
       0,    84,     0,     0,     0,    84,    81,    82,     0,   205,
      78,     0,     0,     0,    78,    79,    80,     0,    78,    79,
      80,     0,    84,    79,    80,     0,     0,     0,     0,    81,
      82,     0,   222,    81,    82,    78,   224,     0,     0,     0,
      79,    80,    78,     0,     0,    84,    78,    79,    80,    84,
       0,    79,    80,     0,     0,     0,    81,    82,     0,   226,
      78,     0,     0,     0,    78,    79,    80,     0,     0,    79,
      80,     0,    84,    81,    82,     0,   229,    81,    82,     0,
     232,    81,    82,     0,   248,    78,     0,     0,     0,    84,
      79,    80,     0,    84,     0,     0,     0,    84,    81,    82,
       0,   250,     0,     0,     0,    81,    82,     0,   256,    81,
      82,    78,   258,     0,    84,     0,    79,    80,     0,     0,
       0,    84,     0,    81,    82,    84,   260,    81,    82,    78,
     262,     0,     0,     0,    79,    80,    78,     0,     0,    84,
      78,    79,    80,    84,     0,    79,    80,     0,    81,    82,
       0,   264,   156,   157,   158,   159,   160,   161,   162,   163,
       0,     0,     0,     0,    84,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    81,    82,     0,   266,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      84,     0,    81,    82,     0,   268,     0,     0,     0,    81,
      82,     0,   271,    81,    82,     0,     0,     4,    84,     0,
       0,     0,     0,     0,     0,    84,     0,   164,   165,    84,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,     4,     0,     0,     0,     0,     0,
       0,     0,     0,    59,    60,     0,     0,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,   157,   158,   159,   160,   161,   162,   163,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   164,   165
};

static const yytype_int16 yycheck[] =
{
       7,     8,     1,     4,     1,    86,    89,    14,    15,    92,
      73,    92,    75,    20,    21,    22,    23,    24,    25,    26,
      27,    28,     7,     1,    81,    82,     7,     0,    35,     7,
      73,    12,    13,    40,    12,    13,    21,     3,     4,     5,
       6,     7,     8,     9,    10,     3,     4,     5,     6,     7,
       8,     9,    10,     3,     4,     5,     6,     7,     8,     9,
      10,     5,     6,     7,     8,     9,    10,    73,    73,    70,
      73,    78,    79,    80,    73,    74,    73,    84,    73,     3,
       4,     5,     6,     7,     8,     9,    10,    73,    76,    70,
      71,    90,    70,    71,    75,    73,    74,     6,     7,     8,
       9,    10,    68,    69,    73,    86,   113,    90,    86,    90,
      68,    69,     6,     7,     8,     9,    10,    73,    68,    69,
      73,    72,    92,    80,    68,    69,    92,    73,   135,    80,
      81,    82,    73,    73,    92,     8,     9,    10,    73,    73,
     147,   148,    92,    73,    68,    69,    73,    73,    73,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,    73,   171,    73,   173,    91,   175,    73,
     177,   178,    73,    73,    68,    69,    73,   184,   185,    75,
      72,   188,   189,   190,   191,   192,   193,   194,   195,   196,
      80,   198,    87,    92,   179,     3,     4,     5,     6,     7,
       8,     9,    10,     3,     4,     5,     6,     7,     8,     9,
      10,     3,     4,     5,     6,     7,     8,     9,    10,     3,
       4,     5,     6,     7,     8,     9,    10,   234,    93,     7,
      92,     1,    92,   240,    12,    13,    92,     7,    92,   246,
      92,    92,    12,    13,    93,   252,   253,     3,     4,     5,
       6,     7,     8,     9,    10,    92,    92,    92,    92,    92,
      68,    69,    92,   270,    92,    92,    92,    92,    68,    69,
      92,    92,    92,    92,   281,    92,    68,    69,    92,    88,
      91,    91,    73,    91,    68,    69,    73,    94,    73,    91,
      91,    91,    70,    71,   301,    73,    93,    90,    94,    91,
      70,    71,    92,    73,    74,    94,    86,    91,    86,    91,
      93,    92,    68,    69,    73,    92,    86,    92,    91,    91,
      73,    73,     0,     1,    91,    73,    92,     3,     4,     5,
       6,     7,     8,     9,    10,    91,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
       7,    73,    68,    69,    72,    12,    13,    68,    71,    77,
      78,    70,    80,    81,    82,    83,    84,    85,     1,   295,
       1,    87,     1,    -1,     7,    -1,     7,    -1,     7,    12,
      13,    12,    13,    12,    13,     3,     4,     5,     6,     7,
       8,     9,    10,     3,     4,     5,     6,     7,     8,     9,
      10,     7,    -1,    -1,    -1,    -1,    12,    13,    -1,    -1,
      -1,    -1,    -1,    70,    71,     7,    73,    74,     7,    -1,
      12,    13,     7,    12,    13,    -1,     7,    12,    13,    86,
      -1,    12,    13,    90,    -1,    -1,    -1,    70,    71,    70,
      71,    70,    71,     7,    -1,    -1,    -1,    -1,    12,    13,
      68,    69,    -1,    86,    -1,    86,    -1,    86,    68,    69,
      -1,    -1,    -1,    -1,    70,    71,     7,    73,    -1,    87,
       7,    12,    13,    -1,    -1,    12,    13,    87,    70,    71,
      86,    70,    71,    75,    73,    70,    71,    -1,    73,    70,
      71,    -1,    73,     7,    86,    -1,    -1,    86,    12,    13,
      -1,    86,    -1,    -1,    -1,    86,    70,    71,    -1,    73,
       7,    -1,    -1,    -1,     7,    12,    13,    -1,     7,    12,
      13,    -1,    86,    12,    13,    -1,    -1,    -1,    -1,    70,
      71,    -1,    73,    70,    71,     7,    73,    -1,    -1,    -1,
      12,    13,     7,    -1,    -1,    86,     7,    12,    13,    86,
      -1,    12,    13,    -1,    -1,    -1,    70,    71,    -1,    73,
       7,    -1,    -1,    -1,     7,    12,    13,    -1,    -1,    12,
      13,    -1,    86,    70,    71,    -1,    73,    70,    71,    -1,
      73,    70,    71,    -1,    73,     7,    -1,    -1,    -1,    86,
      12,    13,    -1,    86,    -1,    -1,    -1,    86,    70,    71,
      -1,    73,    -1,    -1,    -1,    70,    71,    -1,    73,    70,
      71,     7,    73,    -1,    86,    -1,    12,    13,    -1,    -1,
      -1,    86,    -1,    70,    71,    86,    73,    70,    71,     7,
      73,    -1,    -1,    -1,    12,    13,     7,    -1,    -1,    86,
       7,    12,    13,    86,    -1,    12,    13,    -1,    70,    71,
      -1,    73,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    -1,    -1,    -1,    86,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    71,    -1,    73,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86,    -1,    70,    71,    -1,    73,    -1,    -1,    -1,    70,
      71,    -1,    73,    70,    71,    -1,    -1,     1,    86,    -1,
      -1,    -1,    -1,    -1,    -1,    86,    -1,    68,    69,    86,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,     1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    77,    78,    -1,    -1,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,     4,     5,     6,     7,     8,     9,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    68,    69
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    96,    97,     0,     1,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    77,
      78,    80,    81,    82,    83,    84,    85,    98,    99,   100,
     102,   103,   104,   105,   113,   113,    73,    73,     7,    12,
      13,    70,    71,    75,    86,    90,   101,   111,   112,   101,
      73,    73,    73,    73,   101,    73,   101,    73,    73,    73,
     101,   101,   112,   101,   101,   101,   101,   101,   101,   101,
       1,    73,    74,    90,    73,    73,    73,    73,    73,   101,
      73,    73,    73,    73,   101,    73,    73,    73,    73,    73,
      73,    73,    73,    73,    76,    90,   108,    81,    82,    80,
      80,    98,    75,   105,   113,   102,    72,    92,    92,   101,
     101,   101,     1,   101,     1,    73,     3,     4,     5,     6,
       7,     8,     9,    10,    68,    69,    93,    92,    92,    92,
      92,    92,    92,    92,    92,    93,    92,    86,    92,    92,
       1,    73,    74,   101,    92,    92,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    92,    92,    92,    92,     1,
     101,    88,   106,    73,   101,    73,   101,    87,    87,    91,
      91,   101,   101,   101,   101,   101,   101,   101,   101,   101,
     101,   101,    73,   101,    73,   101,    73,   101,    73,    73,
     101,    73,    73,   101,    94,   101,    73,   101,    73,    74,
      90,   101,    73,   112,    91,    91,    93,    91,    73,   101,
      73,   101,    94,    94,   101,   101,    73,   101,    73,   101,
      73,   101,    73,   101,    73,   101,    73,   101,    73,   101,
      90,    73,   101,    91,    91,   101,   109,   110,   112,   101,
      87,    86,     1,    73,    74,   101,    92,    92,   101,    92,
     101,   101,     1,   101,    89,    92,   107,    92,   101,    91,
      91,    93,    91,    73,    73,    91,    73,    91,    91,   109,
      73,    87,   101,    92,    91,    73
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    95,    97,    96,    98,    98,    99,    99,    99,    99,
      99,   100,   100,   100,   101,   101,   101,   101,   101,   101,
     101,   101,   101,   101,   101,   101,   101,   101,   101,   101,
     101,   102,   102,   103,   104,   104,   104,   104,   104,   104,
     104,   104,   105,   105,   106,   106,   107,   107,   108,   108,
     108,   109,   109,   110,   110,   110,   111,   112,   113,   113,
     113,   113,   113,   113,   113,   113,   113,   113,   113,   113,
     113,   113,   113,   113,   113,   113,   113,   113,   113,   113,
     113,   113,   113,   113,   113,   113,   113,   113,   113,   113,
     113,   113,   113,   113,   113,   113,   113,   113,   113,   113,
     113,   113,   113,   113,   113,   113,   113,   113,   113,   113,
     113,   113,   113,   113,   113,   113,   113,   113,   113,   113,
     113,   113,   113,   113,   113,   113,   113,   113,   113,   113,
     113,   113,   113,   113,   113,   113,   113,   113,   113,   113,
     113,   113,   113,   113,   113,   113,   113,   113,   113,   113,
     113,   113,   113,   113,   113,   113
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     2,     0,     2,     2,     2,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     2,     3,
       3,     3,     3,     3,     3,     3,     2,     2,     3,     3,
       3,     1,     2,     2,     1,     1,     1,     2,     2,     2,
       2,     0,     2,     5,     1,     0,     1,     0,     0,     3,
       3,     1,     1,     0,     1,     3,     1,     1,     2,     4,
       4,     4,     4,     2,     4,     4,     4,     2,     2,     1,
       1,     4,     4,     4,     4,     2,     4,     4,     4,     4,
       4,     4,     4,     4,     1,     5,     2,     2,     4,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     6,     6,
       4,     4,     6,     6,     4,     4,     4,     4,     6,     6,
       8,     8,     7,     7,     4,     4,     2,     1,     2,     4,
       4,     5,     5,     2,     1,     2,     2,     4,     4,     1,
       1,     1,     1,     1,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     1,     1,     1,     1,     4,     4,
       4,     4,     6,     6,     4,     4
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* $@1: %empty  */
#line 192 "cas-yacc.y"
              {
  if ((current_segment = begin_segment(SEG_DEFAULT, "code*")) == NOT_FOUND) {
    yyerror("fatal error");
    YYABORT;
  }
 }
#line 1802 "cas-yacc.c"
    break;

  case 6: /* line: segtype T_SEGMENT  */
#line 202 "cas-yacc.y"
                                {
  if (global_offset) {
    yyerror("explicit segment definitions not allowed with non-zero entry point");
    YYABORT;
  }
  if ((current_segment = begin_segment((yyvsp[-1].i), (yyvsp[0].s))) == NOT_FOUND) {
    yyerror("fatal error");
    YYABORT;
  }
  offset = 0;
 }
#line 1818 "cas-yacc.c"
    break;

  case 11: /* segtype: T_CODE  */
#line 219 "cas-yacc.y"
                     {(yyval.i) = SEG_CODE;}
#line 1824 "cas-yacc.c"
    break;

  case 12: /* segtype: T_DATA  */
#line 220 "cas-yacc.y"
         {(yyval.i) = SEG_DATA;}
#line 1830 "cas-yacc.c"
    break;

  case 13: /* segtype: T_CONST  */
#line 221 "cas-yacc.y"
          {(yyval.i) = SEG_CONST;}
#line 1836 "cas-yacc.c"
    break;

  case 14: /* expression: T_NUMBER  */
#line 224 "cas-yacc.y"
                                 { (yyval.expr) = newConstant((yyvsp[0].i)); }
#line 1842 "cas-yacc.c"
    break;

  case 15: /* expression: symbol  */
#line 225 "cas-yacc.y"
                                 { (yyval.expr) = newLabel((yyvsp[0].i)); }
#line 1848 "cas-yacc.c"
    break;

  case 16: /* expression: '(' expression ')'  */
#line 226 "cas-yacc.y"
                                 { (yyval.expr) = (yyvsp[-1].expr); }
#line 1854 "cas-yacc.c"
    break;

  case 17: /* expression: expression '+' expression  */
#line 227 "cas-yacc.y"
                                 { (yyval.expr) = do_math('+', (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1860 "cas-yacc.c"
    break;

  case 18: /* expression: '-' expression  */
#line 228 "cas-yacc.y"
                                 { (yyval.expr) = do_math(C_UNARY_MIN, (yyvsp[0].expr), NULL); }
#line 1866 "cas-yacc.c"
    break;

  case 19: /* expression: expression '-' expression  */
#line 229 "cas-yacc.y"
                                 { (yyval.expr) = do_math('-', (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1872 "cas-yacc.c"
    break;

  case 20: /* expression: expression '*' expression  */
#line 230 "cas-yacc.y"
                                 { (yyval.expr) = do_math('*', (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1878 "cas-yacc.c"
    break;

  case 21: /* expression: expression '/' expression  */
#line 231 "cas-yacc.y"
                                 { (yyval.expr) = do_math('/', (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1884 "cas-yacc.c"
    break;

  case 22: /* expression: expression '%' expression  */
#line 232 "cas-yacc.y"
                                 { (yyval.expr) = do_math('%', (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1890 "cas-yacc.c"
    break;

  case 23: /* expression: expression '^' expression  */
#line 233 "cas-yacc.y"
                                 { (yyval.expr) = do_math('^', (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1896 "cas-yacc.c"
    break;

  case 24: /* expression: expression '&' expression  */
#line 234 "cas-yacc.y"
                                 { (yyval.expr) = do_math('&', (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1902 "cas-yacc.c"
    break;

  case 25: /* expression: expression '|' expression  */
#line 235 "cas-yacc.y"
                                 { (yyval.expr) = do_math('|', (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1908 "cas-yacc.c"
    break;

  case 26: /* expression: '!' expression  */
#line 236 "cas-yacc.y"
                                 { (yyval.expr) = do_math('!', (yyvsp[0].expr), NULL); }
#line 1914 "cas-yacc.c"
    break;

  case 27: /* expression: '~' expression  */
#line 237 "cas-yacc.y"
                                 { (yyval.expr) = do_math('~', (yyvsp[0].expr), NULL); }
#line 1920 "cas-yacc.c"
    break;

  case 28: /* expression: expression T_LL expression  */
#line 238 "cas-yacc.y"
                                 { (yyval.expr) = do_math(C_LL, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1926 "cas-yacc.c"
    break;

  case 29: /* expression: expression T_GG expression  */
#line 239 "cas-yacc.y"
                                 { (yyval.expr) = do_math(C_GG, (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1932 "cas-yacc.c"
    break;

  case 30: /* expression: '(' error ')'  */
#line 240 "cas-yacc.y"
                                 { (yyval.expr) = NULL; yyerror("malformed expression"); }
#line 1938 "cas-yacc.c"
    break;

  case 33: /* label: symdef T_LABEL  */
#line 247 "cas-yacc.y"
                             {
  if (NOT_FOUND == add_label((yyvsp[0].s), current_segment, offset, 
			     (yyvsp[-1].sym).global, (yyvsp[-1].sym).align8)) {
    yyerror("fatal error");
    YYABORT;
  }
 }
#line 1950 "cas-yacc.c"
    break;

  case 34: /* symdef: T_GLOBAL  */
#line 257 "cas-yacc.y"
         { 
  (yyval.sym).align8 = 0; (yyval.sym).global = 1;
}
#line 1958 "cas-yacc.c"
    break;

  case 35: /* symdef: T_ALIGN8  */
#line 260 "cas-yacc.y"
           {
  if (current_segment != DEFAULT_SEGMENT) report(0, ALIGNSEGMENT);
  (yyval.sym).align8 = 1; (yyval.sym).global = 0;
}
#line 1967 "cas-yacc.c"
    break;

  case 36: /* symdef: T_PAGE  */
#line 264 "cas-yacc.y"
           {
  if (current_segment != DEFAULT_SEGMENT) report(0, ALIGNSEGMENT);
  (yyval.sym).align8 = 2; (yyval.sym).global = 0; 
}
#line 1976 "cas-yacc.c"
    break;

  case 37: /* symdef: T_ALIGN8 T_GLOBAL  */
#line 268 "cas-yacc.y"
                    {
  if (current_segment != DEFAULT_SEGMENT) report(0, ALIGNSEGMENT);
  (yyval.sym).align8 = 1; (yyval.sym).global = 1;
}
#line 1985 "cas-yacc.c"
    break;

  case 38: /* symdef: T_GLOBAL T_ALIGN8  */
#line 272 "cas-yacc.y"
                    {
  if (current_segment != DEFAULT_SEGMENT) report(0, ALIGNSEGMENT);
  (yyval.sym).align8 = 1; (yyval.sym).global = 1;
}
#line 1994 "cas-yacc.c"
    break;

  case 39: /* symdef: T_PAGE T_GLOBAL  */
#line 276 "cas-yacc.y"
                  {
  if (current_segment != DEFAULT_SEGMENT) report(0, ALIGNSEGMENT);
  (yyval.sym).align8 = 2; (yyval.sym).global = 1;
}
#line 2003 "cas-yacc.c"
    break;

  case 40: /* symdef: T_GLOBAL T_PAGE  */
#line 280 "cas-yacc.y"
                  {
  if (current_segment != DEFAULT_SEGMENT) report(0, ALIGNSEGMENT);
  (yyval.sym).align8 = 2; (yyval.sym).global = 1;
}
#line 2012 "cas-yacc.c"
    break;

  case 41: /* symdef: %empty  */
#line 284 "cas-yacc.y"
  {(yyval.sym).global = (yyval.sym).align8 = 0; }
#line 2018 "cas-yacc.c"
    break;

  case 42: /* datadef: T_DEFSTRING T_STRING  */
#line 286 "cas-yacc.y"
                                   {
  char *s;
  for (s = (yyvsp[0].s); *s; s++) {
    Dword d = *s;
    emit(d);
  }
  emit(0);
}
#line 2031 "cas-yacc.c"
    break;

  case 43: /* datadef: T_DEFWORD size opt_oparen values opt_cparen  */
#line 294 "cas-yacc.y"
                                              {
  int i;
  if ((yyvsp[-3].i) < (yyvsp[-1].v).size) {
    sprintf(error_buffer, "%d initializers given, %d expected",
	    (yyvsp[-1].v).size, (yyvsp[-3].i));
    yywarning(error_buffer);
    (yyvsp[-1].v).size = (yyvsp[-3].i);
  }
  for (i = 0; i < (yyvsp[-1].v).size; i += 2) {
    uint64_t e1 = (uint64_t)((yyvsp[-1].v).data[i]) << 32 >> 32;
    uint64_t e2 = (uint64_t)((yyvsp[-1].v).data[i + 1]) >> 32;
    emit_expression ((Expression*)(e1 + e2));
  }
  for (     ; i < (yyvsp[-3].i)     ; i++)
    emit(0);
}
#line 2052 "cas-yacc.c"
    break;

  case 48: /* size: %empty  */
#line 315 "cas-yacc.y"
              {(yyval.i) = 1;}
#line 2058 "cas-yacc.c"
    break;

  case 49: /* size: '[' expression ']'  */
#line 316 "cas-yacc.y"
                     {
  if ((yyvsp[-1].expr)->type != CONSTANT || (yyvsp[-1].expr)->detail.constant <= 0) {
    report(1, ARRAY_SIZE);
    (yyval.i) = 0;
  } else
    (yyval.i) = (yyvsp[-1].expr)->detail.constant;
	    }
#line 2070 "cas-yacc.c"
    break;

  case 50: /* size: '[' error ']'  */
#line 323 "cas-yacc.y"
                { (yyval.i) = -1;  yyerror("malformed array size");}
#line 2076 "cas-yacc.c"
    break;

  case 51: /* expression_or_selector: expression  */
#line 325 "cas-yacc.y"
                                   {
  (yyval.expr) = (yyvsp[0].expr);
}
#line 2084 "cas-yacc.c"
    break;

  case 52: /* expression_or_selector: segment  */
#line 328 "cas-yacc.y"
          {
  (yyval.expr) = newSelector(MK_SELECTOR((yyvsp[0].i), 0, _LDT));
}
#line 2092 "cas-yacc.c"
    break;

  case 53: /* values: %empty  */
#line 333 "cas-yacc.y"
              {(yyval.v).size = 0; (yyval.v).data = NULL;}
#line 2098 "cas-yacc.c"
    break;

  case 54: /* values: expression_or_selector  */
#line 334 "cas-yacc.y"
                         {
  (yyval.v).data = safe_malloc(2 * sizeof(Dword));
  puts("foo");
  (yyval.v).data[0] = (Uword)((int64_t)(yyvsp[0].expr) << 32 >> 32);
  (yyval.v).data[1] = (Uword)((int64_t)(yyvsp[0].expr) >> 32);
  (yyval.v).size = 2;
}
#line 2110 "cas-yacc.c"
    break;

  case 55: /* values: values ',' expression_or_selector  */
#line 341 "cas-yacc.y"
                                    { 
  (yyval.v).data = safe_realloc((yyval.v).data, ((yyval.v).size + 2) * sizeof(Dword)); 
  (yyval.v).data[(yyval.v).size] = (Uword)((int64_t)(yyvsp[0].expr) << 32 >> 32);
  (yyval.v).data[(yyval.v).size + 1] = (Uword)((int64_t)(yyvsp[0].expr) >> 32);
  puts("foo");
  (yyval.v).size = (yyval.v).size + 2;
}
#line 2122 "cas-yacc.c"
    break;

  case 56: /* symbol: T_ADDRESS  */
#line 349 "cas-yacc.y"
                       {
  int label = use_label((yyvsp[0].s), current_segment);
  if (NOT_FOUND == label) {
    yyerror("fatal error");
    YYABORT;
  }
  (yyval.i) = label;
}
#line 2135 "cas-yacc.c"
    break;

  case 57: /* segment: T_SEGMENT  */
#line 358 "cas-yacc.y"
                        {
  (yyval.i) = lookup_segment((yyvsp[0].s)); 
  if (NOT_FOUND == (yyval.i)) {
    yyerror("fatal error");
    YYABORT;
  }
}
#line 2147 "cas-yacc.c"
    break;

  case 58: /* instruction: error instruction  */
#line 367 "cas-yacc.y"
{ yyerror("unrecognized instruction, trying to recover"); }
#line 2153 "cas-yacc.c"
    break;

  case 59: /* instruction: T_ADD T_GREGISTER ',' T_GREGISTER  */
#line 369 "cas-yacc.y"
{emit(BUILD_INSTRUCTION_A(ADD, (yyvsp[-2].i), (yyvsp[0].i)));}
#line 2159 "cas-yacc.c"
    break;

  case 60: /* instruction: T_ADD T_GREGISTER ',' expression  */
#line 372 "cas-yacc.y"
{emit(BUILD_INSTRUCTION_A(xADDI, (yyvsp[-2].i), 0));
  emit_expression((yyvsp[0].expr));}
#line 2166 "cas-yacc.c"
    break;

  case 61: /* instruction: T_AND T_GREGISTER ',' T_GREGISTER  */
#line 376 "cas-yacc.y"
{emit(BUILD_INSTRUCTION_A(AND, (yyvsp[-2].i), (yyvsp[0].i)));}
#line 2172 "cas-yacc.c"
    break;

  case 62: /* instruction: T_AND T_GREGISTER ',' expression  */
#line 379 "cas-yacc.y"
{emit(BUILD_INSTRUCTION_A(xANDI, (yyvsp[-2].i), 0));
  emit_expression((yyvsp[0].expr));}
#line 2179 "cas-yacc.c"
    break;

  case 63: /* instruction: T_CALL expression  */
#line 383 "cas-yacc.y"
{
  emit(BUILD_INSTRUCTION_A(xNCALL, 0, 0));
  emit_expression((yyvsp[0].expr));
}
#line 2188 "cas-yacc.c"
    break;

  case 64: /* instruction: T_CALL '[' T_GREGISTER ']'  */
#line 389 "cas-yacc.y"
{emit(BUILD_INSTRUCTION_A(NCALLX, (yyvsp[-1].i), 0));}
#line 2194 "cas-yacc.c"
    break;

  case 65: /* instruction: T_CALL '[' error ']'  */
#line 392 "cas-yacc.y"
{ report(1, INDIRECTION); }
#line 2200 "cas-yacc.c"
    break;

  case 66: /* instruction: T_CALL segment ':' expression  */
#line 395 "cas-yacc.y"
{
  Selector s = MK_SELECTOR((yyvsp[-2].i), 0, _LDT); 
  /* the next instruction contains a segment selector 
     that must be adjusted if there is more than one module
     in the program */
  if (module_type == CLOF_EXE) {
    store(FIX_SEGMENT); 
  }
  emit(BUILD_INSTRUCTION_C(xFCALL, 0, s));
  emit_expression((yyvsp[0].expr));
}
#line 2216 "cas-yacc.c"
    break;

  case 67: /* instruction: T_CALL segment  */
#line 408 "cas-yacc.y"
{
  Selector s = MK_SELECTOR((yyvsp[0].i), 0, _LDT); 
  /* the next instruction contains a segment selector 
     that must be adjusted if there is more than one module
     in the program */
  if (module_type == CLOF_EXE) {
    store(FIX_SEGMENT); 
  }
  emit(BUILD_INSTRUCTION_C(xFCALL, 0, s));
  emit_expression(NULL);
}
#line 2232 "cas-yacc.c"
    break;

  case 68: /* instruction: T_CHIO expression  */
#line 421 "cas-yacc.y"
{ emit_displacement(CHIO, 0, (yyvsp[0].expr), 0); }
#line 2238 "cas-yacc.c"
    break;

  case 69: /* instruction: T_CLC  */
#line 424 "cas-yacc.y"
{ emit(BUILD_INSTRUCTION_A(CLC, 0, 0));}
#line 2244 "cas-yacc.c"
    break;

  case 70: /* instruction: T_CLI  */
#line 427 "cas-yacc.y"
{ emit(BUILD_INSTRUCTION_A(CLI, 0, 0));}
#line 2250 "cas-yacc.c"
    break;

  case 71: /* instruction: T_CLRB T_GREGISTER ',' expression  */
#line 430 "cas-yacc.y"
{ emit_displacement(CLRBI, (yyvsp[-2].i), (yyvsp[0].expr), 0); /* CLRBI */ }
#line 2256 "cas-yacc.c"
    break;

  case 72: /* instruction: T_CLRB T_GREGISTER ',' T_GREGISTER  */
#line 433 "cas-yacc.y"
{ emit(BUILD_INSTRUCTION_A(CLRB, (yyvsp[-2].i), (yyvsp[0].i)));}
#line 2262 "cas-yacc.c"
    break;

  case 73: /* instruction: T_CMP T_GREGISTER ',' T_GREGISTER  */
#line 436 "cas-yacc.y"
{ emit(BUILD_INSTRUCTION_A(CMP, (yyvsp[-2].i), (yyvsp[0].i)));}
#line 2268 "cas-yacc.c"
    break;

  case 74: /* instruction: T_CMP T_GREGISTER ',' expression  */
#line 439 "cas-yacc.y"
{ 
  emit(BUILD_INSTRUCTION_A(xCMPI, (yyvsp[-2].i), 0));
  emit_expression((yyvsp[0].expr));
}
#line 2277 "cas-yacc.c"
    break;

  case 75: /* instruction: T_DEC T_GREGISTER  */
#line 445 "cas-yacc.y"
{emit(BUILD_INSTRUCTION_A(DEC, (yyvsp[0].i), 0));}
#line 2283 "cas-yacc.c"
    break;

  case 76: /* instruction: T_DIV T_GREGISTER ',' T_GREGISTER  */
#line 448 "cas-yacc.y"
{emit(BUILD_INSTRUCTION_A(DIV, (yyvsp[-2].i), (yyvsp[0].i)));}
#line 2289 "cas-yacc.c"
    break;

  case 77: /* instruction: T_DIV T_GREGISTER ',' expression  */
#line 451 "cas-yacc.y"
{emit(BUILD_INSTRUCTION_A(xDIVI, (yyvsp[-2].i), 0));
  emit_expression((yyvsp[0].expr));}
#line 2296 "cas-yacc.c"
    break;

  case 78: /* instruction: T_DIV expression ',' T_GREGISTER  */
#line 455 "cas-yacc.y"
{emit(BUILD_INSTRUCTION_A(xDIVJ, (yyvsp[0].i), 0));
  emit_expression((yyvsp[-2].expr));}
#line 2303 "cas-yacc.c"
    break;

  case 79: /* instruction: T_REM T_GREGISTER ',' T_GREGISTER  */
#line 459 "cas-yacc.y"
{emit(BUILD_INSTRUCTION_A(REM, (yyvsp[-2].i), (yyvsp[0].i)));}
#line 2309 "cas-yacc.c"
    break;

  case 80: /* instruction: T_REM T_GREGISTER ',' expression  */
#line 462 "cas-yacc.y"
{emit(BUILD_INSTRUCTION_A(xREMI, (yyvsp[-2].i), 0));
  emit_expression((yyvsp[0].expr));}
#line 2316 "cas-yacc.c"
    break;

  case 81: /* instruction: T_REM expression ',' T_GREGISTER  */
#line 466 "cas-yacc.y"
{emit(BUILD_INSTRUCTION_A(xREMJ, (yyvsp[0].i), 0));
  emit_expression((yyvsp[-2].expr));}
#line 2323 "cas-yacc.c"
    break;

  case 82: /* instruction: T_GETB T_GREGISTER ',' T_GREGISTER  */
#line 470 "cas-yacc.y"
{emit(BUILD_INSTRUCTION_A(GETB, (yyvsp[-2].i), (yyvsp[0].i)));}
#line 2329 "cas-yacc.c"
    break;

  case 83: /* instruction: T_GETB T_GREGISTER ',' expression  */
#line 473 "cas-yacc.y"
{ emit_displacement(GETBI, (yyvsp[-2].i), (yyvsp[0].expr), 0); }
#line 2335 "cas-yacc.c"
    break;

  case 84: /* instruction: T_HLT  */
#line 475 "cas-yacc.y"
        {emit(BUILD_INSTRUCTION_A(HLT, 0, 0));}
#line 2341 "cas-yacc.c"
    break;

  case 85: /* instruction: T_IN T_GREGISTER ',' '?' expression  */
#line 478 "cas-yacc.y"
{ emit_displacement(IN, (yyvsp[-3].i), (yyvsp[0].expr), 0);}
#line 2347 "cas-yacc.c"
    break;

  case 86: /* instruction: T_INC T_GREGISTER  */
#line 481 "cas-yacc.y"
{emit(BUILD_INSTRUCTION_A(INC, (yyvsp[0].i), 0));}
#line 2353 "cas-yacc.c"
    break;

  case 87: /* instruction: T_JMP expression  */
#line 484 "cas-yacc.y"
{ 
  emit(BUILD_INSTRUCTION_A(xNJMP, 0, 0));
  emit_expression((yyvsp[0].expr));
}
#line 2362 "cas-yacc.c"
    break;

  case 88: /* instruction: T_JMP segment ':' expression  */
#line 490 "cas-yacc.y"
{
  Selector s = MK_SELECTOR((yyvsp[-2].i), 0, _LDT); 
  /* the next instruction contains a segment selector 
     that must be adjusted if there is more than one module
     in the program */
  if (module_type == CLOF_EXE) { /*NOT TESTED */
    store(FIX_SEGMENT);
  }
  emit(BUILD_INSTRUCTION_C(xFJMP, 0, s));
  emit_expression((yyvsp[0].expr));
}
#line 2378 "cas-yacc.c"
    break;

  case 89: /* instruction: T_JMP segment  */
#line 503 "cas-yacc.y"
{
  Selector s = MK_SELECTOR((yyvsp[0].i), 0, _LDT); 
  /* the next instruction contains a segment selector 
     that must be adjusted if there is more than one module
     in the program */
  if (module_type == CLOF_EXE) { /*NOT TESTED */
    store(FIX_SEGMENT); 
  }
  emit(BUILD_INSTRUCTION_C(xFJMP, 0, s));
  emit_expression(NULL);
}
#line 2394 "cas-yacc.c"
    break;

  case 90: /* instruction: T_JNZ expression  */
#line 516 "cas-yacc.y"
{ emit_displacement(JNZ, 0, (yyvsp[0].expr), 1); }
#line 2400 "cas-yacc.c"
    break;

  case 91: /* instruction: T_JZ expression  */
#line 518 "cas-yacc.y"
{ emit_displacement(JZ, 0, (yyvsp[0].expr), 1); }
#line 2406 "cas-yacc.c"
    break;

  case 92: /* instruction: T_JNC expression  */
#line 520 "cas-yacc.y"
{ emit_displacement(JNC, 0, (yyvsp[0].expr), 1); }
#line 2412 "cas-yacc.c"
    break;

  case 93: /* instruction: T_JC expression  */
#line 522 "cas-yacc.y"
{ emit_displacement(JC, 0, (yyvsp[0].expr), 1); }
#line 2418 "cas-yacc.c"
    break;

  case 94: /* instruction: T_JNO expression  */
#line 524 "cas-yacc.y"
{ emit_displacement(JNO, 0, (yyvsp[0].expr), 1); }
#line 2424 "cas-yacc.c"
    break;

  case 95: /* instruction: T_JO expression  */
#line 526 "cas-yacc.y"
{ emit_displacement(JO, 0, (yyvsp[0].expr), 1); }
#line 2430 "cas-yacc.c"
    break;

  case 96: /* instruction: T_JNS expression  */
#line 528 "cas-yacc.y"
{ emit_displacement(JNS, 0, (yyvsp[0].expr), 1); }
#line 2436 "cas-yacc.c"
    break;

  case 97: /* instruction: T_JS expression  */
#line 530 "cas-yacc.y"
{ emit_displacement(JS, 0, (yyvsp[0].expr), 1); }
#line 2442 "cas-yacc.c"
    break;

  case 98: /* instruction: T_MOV T_GREGISTER ',' '[' expression ']'  */
#line 533 "cas-yacc.y"
{ 
  emit(BUILD_INSTRUCTION_A(xLD, (yyvsp[-4].i), 0));
  emit_expression((yyvsp[-1].expr));
}
#line 2451 "cas-yacc.c"
    break;

  case 99: /* instruction: T_MOV T_GREGISTER ',' '[' error ']'  */
#line 539 "cas-yacc.y"
{ report(1, INDIRECTION); }
#line 2457 "cas-yacc.c"
    break;

  case 100: /* instruction: T_MOV error ',' T_GREGISTER  */
#line 542 "cas-yacc.y"
{ yyerror("Did you forget [] around the variable name?"); }
#line 2463 "cas-yacc.c"
    break;

  case 101: /* instruction: T_MOV T_GREGISTER ',' expression  */
#line 545 "cas-yacc.y"
{
  emit(BUILD_INSTRUCTION_A(xLDI, (yyvsp[-2].i), 0));
  emit_expression((yyvsp[0].expr));
}
#line 2472 "cas-yacc.c"
    break;

  case 102: /* instruction: T_MOV '[' expression ']' ',' T_GREGISTER  */
#line 551 "cas-yacc.y"
{ 
  emit(BUILD_INSTRUCTION_A(xST, (yyvsp[0].i), 0));
  emit_expression((yyvsp[-3].expr));
}
#line 2481 "cas-yacc.c"
    break;

  case 103: /* instruction: T_MOV '[' error ']' ',' T_GREGISTER  */
#line 556 "cas-yacc.y"
{ report(1, INDIRECTION); }
#line 2487 "cas-yacc.c"
    break;

  case 104: /* instruction: T_MOV T_SREGISTER ',' T_GREGISTER  */
#line 559 "cas-yacc.y"
{
  if ((yyvsp[-2].i)==_CODE) { report(1, BADUSEOFCS); YYABORT; }
  emit(BUILD_INSTRUCTION_A(MOVTS, (yyvsp[-2].i), (yyvsp[0].i)));
}
#line 2496 "cas-yacc.c"
    break;

  case 105: /* instruction: T_MOV T_GREGISTER ',' T_SREGISTER  */
#line 565 "cas-yacc.y"
{emit(BUILD_INSTRUCTION_A(MOVFS, (yyvsp[-2].i), (yyvsp[0].i)));}
#line 2502 "cas-yacc.c"
    break;

  case 106: /* instruction: T_MOV T_GREGISTER ',' T_GREGISTER  */
#line 568 "cas-yacc.y"
{emit(BUILD_INSTRUCTION_A(MOV, (yyvsp[-2].i), (yyvsp[0].i)));}
#line 2508 "cas-yacc.c"
    break;

  case 107: /* instruction: T_MOV T_SREGISTER ',' segment  */
#line 571 "cas-yacc.y"
{
  Selector s;
  if ((yyvsp[-2].i)==_CODE) { report(1, BADUSEOFCS); YYABORT; }
  s = MK_SELECTOR((yyvsp[0].i), 0, _LDT); 
  /* the next instruction contains a segment selector 
     that must be adjusted if there is more than one module
     in the program */
  if (module_type == CLOF_EXE) {
    store(FIX_SEGMENT); /*NOT TESTED*/
  }
  emit(BUILD_INSTRUCTION_C(MOVSI, (yyvsp[-2].i), s)); /*WORKS*/
}
#line 2525 "cas-yacc.c"
    break;

  case 108: /* instruction: T_MOV T_GREGISTER ',' '[' T_GREGISTER ']'  */
#line 585 "cas-yacc.y"
{emit(BUILD_INSTRUCTION_A(LDX, (yyvsp[-4].i), (yyvsp[-1].i)));}
#line 2531 "cas-yacc.c"
    break;

  case 109: /* instruction: T_MOV '[' T_GREGISTER ']' ',' T_GREGISTER  */
#line 588 "cas-yacc.y"
{emit(BUILD_INSTRUCTION_A(STX, (yyvsp[-3].i), (yyvsp[0].i)));}
#line 2537 "cas-yacc.c"
    break;

  case 110: /* instruction: T_MOV T_GREGISTER ',' '[' T_SREGISTER ':' expression ']'  */
#line 591 "cas-yacc.y"
{
  emit(BUILD_INSTRUCTION_A(xLDS, (yyvsp[-6].i), (yyvsp[-3].i)));
  emit_expression((yyvsp[-1].expr));
}
#line 2546 "cas-yacc.c"
    break;

  case 111: /* instruction: T_MOV '[' T_SREGISTER ':' expression ']' ',' T_GREGISTER  */
#line 597 "cas-yacc.y"
{
  if ((yyvsp[-5].i)==_CODE) { report(1, BADUSEOFCS); YYABORT; }
  emit(BUILD_INSTRUCTION_A(xSTS, (yyvsp[-5].i), (yyvsp[0].i)));
  emit_expression((yyvsp[-3].expr));
}
#line 2556 "cas-yacc.c"
    break;

  case 112: /* instruction: T_MOV T_GREGISTER '(' expression ')' ',' T_GREGISTER  */
#line 604 "cas-yacc.y"
{
  if ((yyvsp[-3].expr)->type != CONSTANT) {
    report(1, VARINDEX);
  } else {
    emit(BUILD_INSTRUCTION_D(SETBY, (yyvsp[0].i), (yyvsp[-5].i), (yyvsp[-3].expr)->detail.constant));
  } /* SETBY */
}
#line 2568 "cas-yacc.c"
    break;

  case 113: /* instruction: T_MOV T_GREGISTER ',' T_GREGISTER '(' expression ')'  */
#line 613 "cas-yacc.y"
{
  if ((yyvsp[-1].expr)->type != CONSTANT) {
    report(1, VARINDEX);
  } else {
    emit(BUILD_INSTRUCTION_D(GETBY, (yyvsp[-5].i), (yyvsp[-3].i), (yyvsp[-1].expr)->detail.constant));
  } /* GETBY */
}
#line 2580 "cas-yacc.c"
    break;

  case 114: /* instruction: T_MUL T_GREGISTER ',' T_GREGISTER  */
#line 622 "cas-yacc.y"
{emit(BUILD_INSTRUCTION_A(MUL, (yyvsp[-2].i), (yyvsp[0].i)));}
#line 2586 "cas-yacc.c"
    break;

  case 115: /* instruction: T_MUL T_GREGISTER ',' expression  */
#line 625 "cas-yacc.y"
{
  emit(BUILD_INSTRUCTION_A(xMULI, (yyvsp[-2].i), 0));
  emit_expression((yyvsp[0].expr));
}
#line 2595 "cas-yacc.c"
    break;

  case 116: /* instruction: T_NEG T_GREGISTER  */
#line 631 "cas-yacc.y"
{emit(BUILD_INSTRUCTION_A(NEG, (yyvsp[0].i), 0));}
#line 2601 "cas-yacc.c"
    break;

  case 117: /* instruction: T_NOP  */
#line 634 "cas-yacc.y"
{emit(BUILD_INSTRUCTION_A(NOP, 0, 0));}
#line 2607 "cas-yacc.c"
    break;

  case 118: /* instruction: T_NOT T_GREGISTER  */
#line 637 "cas-yacc.y"
{emit(BUILD_INSTRUCTION_A(NOT, (yyvsp[0].i), 0));}
#line 2613 "cas-yacc.c"
    break;

  case 119: /* instruction: T_OR T_GREGISTER ',' T_GREGISTER  */
#line 640 "cas-yacc.y"
{emit(BUILD_INSTRUCTION_A(OR, (yyvsp[-2].i), (yyvsp[0].i)));}
#line 2619 "cas-yacc.c"
    break;

  case 120: /* instruction: T_OR T_GREGISTER ',' expression  */
#line 643 "cas-yacc.y"
{
  emit(BUILD_INSTRUCTION_A(xORI, (yyvsp[-2].i), 0));
  emit_expression((yyvsp[0].expr));
}
#line 2628 "cas-yacc.c"
    break;

  case 121: /* instruction: T_OUT T_GREGISTER ',' '?' expression  */
#line 649 "cas-yacc.y"
{ emit_displacement(OUT, (yyvsp[-3].i), (yyvsp[0].expr), 0); }
#line 2634 "cas-yacc.c"
    break;

  case 122: /* instruction: T_OUT expression ',' '?' expression  */
#line 652 "cas-yacc.y"
{ 
  emit_displacement(xOUTI, 0, (yyvsp[0].expr), 0); 
  emit_expression((yyvsp[-3].expr));
}
#line 2643 "cas-yacc.c"
    break;

  case 123: /* instruction: T_POP T_GREGISTER  */
#line 658 "cas-yacc.y"
{emit(BUILD_INSTRUCTION_A(POP, (yyvsp[0].i), 0));}
#line 2649 "cas-yacc.c"
    break;

  case 124: /* instruction: T_POPF  */
#line 661 "cas-yacc.y"
{emit(BUILD_INSTRUCTION_A(POPF, 0, 0));}
#line 2655 "cas-yacc.c"
    break;

  case 125: /* instruction: T_PUSH T_GREGISTER  */
#line 664 "cas-yacc.y"
{emit(BUILD_INSTRUCTION_A(PUSH, (yyvsp[0].i), 0));}
#line 2661 "cas-yacc.c"
    break;

  case 126: /* instruction: T_PUSH expression  */
#line 667 "cas-yacc.y"
{
  emit(BUILD_INSTRUCTION_A(xPUSHI, 0, 0));
  emit_expression((yyvsp[0].expr));
}
#line 2670 "cas-yacc.c"
    break;

  case 127: /* instruction: T_PEEK T_GREGISTER ',' expression  */
#line 673 "cas-yacc.y"
{ emit_displacement(PEEK, (yyvsp[-2].i), (yyvsp[0].expr), 0); }
#line 2676 "cas-yacc.c"
    break;

  case 128: /* instruction: T_POKE T_GREGISTER ',' expression  */
#line 676 "cas-yacc.y"
{ emit_displacement(POKE, (yyvsp[-2].i), (yyvsp[0].expr), 0); }
#line 2682 "cas-yacc.c"
    break;

  case 129: /* instruction: T_PUSHF  */
#line 679 "cas-yacc.y"
{emit(BUILD_INSTRUCTION_A(PUSHF, 0, 0));}
#line 2688 "cas-yacc.c"
    break;

  case 130: /* instruction: T_RETN  */
#line 682 "cas-yacc.y"
{emit(BUILD_INSTRUCTION_A(NRET, 0, 0));}
#line 2694 "cas-yacc.c"
    break;

  case 131: /* instruction: T_RETNI  */
#line 685 "cas-yacc.y"
{emit(BUILD_INSTRUCTION_A(INRET, 0, 0));}
#line 2700 "cas-yacc.c"
    break;

  case 132: /* instruction: T_RETF  */
#line 688 "cas-yacc.y"
{ emit(BUILD_INSTRUCTION_A(FRET, 0, 0));}
#line 2706 "cas-yacc.c"
    break;

  case 133: /* instruction: T_RETFI  */
#line 691 "cas-yacc.y"
{ emit(BUILD_INSTRUCTION_A(IFRET, 0, 0));}
#line 2712 "cas-yacc.c"
    break;

  case 134: /* instruction: T_ROL T_GREGISTER ',' T_GREGISTER  */
#line 694 "cas-yacc.y"
{ emit(BUILD_INSTRUCTION_A(ROL, (yyvsp[-2].i), (yyvsp[0].i)));}
#line 2718 "cas-yacc.c"
    break;

  case 135: /* instruction: T_ROL T_GREGISTER ',' expression  */
#line 697 "cas-yacc.y"
{ emit_displacement(ROLI, (yyvsp[-2].i), (yyvsp[0].expr), 0); }
#line 2724 "cas-yacc.c"
    break;

  case 136: /* instruction: T_ROR T_GREGISTER ',' T_GREGISTER  */
#line 700 "cas-yacc.y"
{ emit(BUILD_INSTRUCTION_A(ROR, (yyvsp[-2].i), (yyvsp[0].i)));}
#line 2730 "cas-yacc.c"
    break;

  case 137: /* instruction: T_ROR T_GREGISTER ',' expression  */
#line 703 "cas-yacc.y"
{ emit_displacement(RORI, (yyvsp[-2].i), (yyvsp[0].expr), 0); }
#line 2736 "cas-yacc.c"
    break;

  case 138: /* instruction: T_SAL T_GREGISTER ',' T_GREGISTER  */
#line 706 "cas-yacc.y"
{emit(BUILD_INSTRUCTION_A(SAL, (yyvsp[-2].i), (yyvsp[0].i)));}
#line 2742 "cas-yacc.c"
    break;

  case 139: /* instruction: T_SAL T_GREGISTER ',' expression  */
#line 709 "cas-yacc.y"
{ emit_displacement(SALI, (yyvsp[-2].i), (yyvsp[0].expr), 0); }
#line 2748 "cas-yacc.c"
    break;

  case 140: /* instruction: T_SAR T_GREGISTER ',' T_GREGISTER  */
#line 712 "cas-yacc.y"
{ emit(BUILD_INSTRUCTION_A(SAR, (yyvsp[-2].i), (yyvsp[0].i)));}
#line 2754 "cas-yacc.c"
    break;

  case 141: /* instruction: T_SAR T_GREGISTER ',' expression  */
#line 715 "cas-yacc.y"
{ emit_displacement(SARI, (yyvsp[-2].i), (yyvsp[0].expr), 0); }
#line 2760 "cas-yacc.c"
    break;

  case 142: /* instruction: T_SETB T_GREGISTER ',' T_GREGISTER  */
#line 718 "cas-yacc.y"
{ emit(BUILD_INSTRUCTION_A(SETB, (yyvsp[-2].i), (yyvsp[0].i)));}
#line 2766 "cas-yacc.c"
    break;

  case 143: /* instruction: T_SETB T_GREGISTER ',' expression  */
#line 721 "cas-yacc.y"
{ emit_displacement(SETBI, (yyvsp[-2].i), (yyvsp[0].expr), 0); }
#line 2772 "cas-yacc.c"
    break;

  case 144: /* instruction: T_STC  */
#line 724 "cas-yacc.y"
{emit(BUILD_INSTRUCTION_A(STC, 0, 0));}
#line 2778 "cas-yacc.c"
    break;

  case 145: /* instruction: T_STI  */
#line 727 "cas-yacc.y"
{emit(BUILD_INSTRUCTION_A(STI, 0, 0));}
#line 2784 "cas-yacc.c"
    break;

  case 146: /* instruction: T_TRAP  */
#line 730 "cas-yacc.y"
{emit(BUILD_INSTRUCTION_A(TRAP, 0, 0));}
#line 2790 "cas-yacc.c"
    break;

  case 147: /* instruction: T_STOP  */
#line 733 "cas-yacc.y"
{emit(BUILD_INSTRUCTION_A(STOP, 0, 0));}
#line 2796 "cas-yacc.c"
    break;

  case 148: /* instruction: T_SUB T_GREGISTER ',' T_GREGISTER  */
#line 736 "cas-yacc.y"
{emit(BUILD_INSTRUCTION_A(SUB, (yyvsp[-2].i), (yyvsp[0].i)));}
#line 2802 "cas-yacc.c"
    break;

  case 149: /* instruction: T_SUB T_GREGISTER ',' expression  */
#line 739 "cas-yacc.y"
{
  emit(BUILD_INSTRUCTION_A(xSUBI, (yyvsp[-2].i), 0));
  emit_expression((yyvsp[0].expr));
}
#line 2811 "cas-yacc.c"
    break;

  case 150: /* instruction: T_TST T_GREGISTER ',' T_GREGISTER  */
#line 745 "cas-yacc.y"
{emit(BUILD_INSTRUCTION_A(TST, (yyvsp[-2].i), (yyvsp[0].i)));}
#line 2817 "cas-yacc.c"
    break;

  case 151: /* instruction: T_TST T_GREGISTER ',' expression  */
#line 748 "cas-yacc.y"
{
  emit(BUILD_INSTRUCTION_A(xTSTI, (yyvsp[-2].i), 0));
  emit_expression((yyvsp[0].expr));
}
#line 2826 "cas-yacc.c"
    break;

  case 152: /* instruction: T_XCHG T_GREGISTER ',' '[' expression ']'  */
#line 754 "cas-yacc.y"
{ 
  emit(BUILD_INSTRUCTION_A(xXCHG, (yyvsp[-4].i), 0));
  emit_expression((yyvsp[-1].expr));
}
#line 2835 "cas-yacc.c"
    break;

  case 153: /* instruction: T_XCHG T_GREGISTER ',' '[' error ']'  */
#line 759 "cas-yacc.y"
{ report(1, INDIRECTION); }
#line 2841 "cas-yacc.c"
    break;

  case 154: /* instruction: T_XOR T_GREGISTER ',' T_GREGISTER  */
#line 762 "cas-yacc.y"
{emit(BUILD_INSTRUCTION_A(XOR, (yyvsp[-2].i), (yyvsp[0].i)));}
#line 2847 "cas-yacc.c"
    break;

  case 155: /* instruction: T_XOR T_GREGISTER ',' expression  */
#line 765 "cas-yacc.y"
{
  emit(BUILD_INSTRUCTION_A(xXORI, (yyvsp[-2].i), 0));
  emit_expression((yyvsp[0].expr));
}
#line 2856 "cas-yacc.c"
    break;


#line 2860 "cas-yacc.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 772 "cas-yacc.y"


static void store_ptr(void *e)
{
  Uword d1 = (uint64_t)e << 32 >> 32; // LSB
  Uword d2 = (uint64_t)e >> 32; // MSB
  store(d1);
  store(d2);
  //  printf("------ %p %X %X\n", e, d1, d2);
}

static void emit_expression(Expression *e) 
{
  assert(e && e->type != DUMMY); // ????
  
  if (e->type == CONSTANT) {
    emit(e->detail.constant);
  } else {
    // EXPRESSION, LABEL, SELECTOR
    store(FIX_EXPRESSION);
    emit(0);		/* fake instruction */
    //    printf("---- %X %p\n", FIX_EXPRESSION, e);
    store_ptr(e);
  }
}

static void emit_displacement(int opc, int op1, Expression *e, Bit relative) 
{
  store(relative ? FIX_RDISPLACEMENT : FIX_ADISPLACEMENT);
  emit(BUILD_INSTRUCTION_B(opc, op1, 0));
  
  store_ptr(e);

  if (relative) 
    store(offset);   /* current_offset */
}

void yywarning(char *s)
{
  fprintf(stderr, "warning:%s:%d: %s\n", *source, line_no, s);
}

int yyerror(char *s)
{
  fprintf(stderr, "%s:%d: %s\n", *source, line_no, s);
  success = 0;
  return 0;
}

