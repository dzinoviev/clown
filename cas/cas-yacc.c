
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "cas-yacc.y"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cas.h"
#include "registers.h"

void yywarning (char *s);
static void emit_expression (Expression *target);
static void emit_displacement (int opc, int op1, Expression *dspl, Bit relative);

 enum {PORT_NAME, PORT_NUMBER, ARRAY_SIZE, INDIRECTION, SEGMENTINBIN, VARINDEX, BADUSEOFCS, ALIGNSEGMENT};
 static struct {
     char *concise;
     char *full;
     int count;
 } _errors[] = {
     {
	 "port name must start with a question mark...",
	 "\n\tport name must start with a question mark, e.g., ?12; this is done"
	 "\n\tto avoid confusion with the parameters to the OUT instruction;"
	 "\n\tthe notation without a question mark will soon be obsolete",
	 0
     },

     {
	 "port number must be a positive constant...",
	 "\n\tport number must be an expression that evaluates to a positive "
	 "\n\tconstant and does not involve any symbols; e.g., 2+3",
	 0
     },

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



/* Line 189 of yacc.c  */
#line 171 "cas-yacc.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


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

/* Line 214 of yacc.c  */
#line 98 "cas-yacc.y"

    char  *s;
    Dword i;
    struct {
	int size;
	Dword *data;
    } v;
    struct labelDef sym;
    struct _Expression *expr;



/* Line 214 of yacc.c  */
#line 370 "cas-yacc.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 382 "cas-yacc.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   951

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  95
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  19
/* YYNRULES -- Number of rules.  */
#define YYNRULES  158
/* YYNRULES -- Number of states.  */
#define YYNSTATES  319

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   330

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    14,     2,     2,     2,    12,     5,     2,
      86,    87,    10,     8,    92,     9,     2,    11,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    93,     2,
       2,     2,     2,    94,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    90,     2,    91,     4,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    88,     3,    89,    15,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     1,     2,     6,     7,
      13,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     7,    10,    11,    14,    17,    20,
      22,    24,    26,    28,    30,    32,    34,    38,    42,    45,
      49,    53,    57,    61,    65,    69,    73,    76,    79,    83,
      87,    91,    93,    96,    99,   101,   103,   105,   108,   111,
     114,   117,   118,   121,   127,   129,   130,   132,   133,   134,
     138,   142,   144,   146,   147,   149,   153,   155,   157,   160,
     165,   170,   175,   180,   183,   188,   193,   198,   201,   204,
     206,   208,   213,   218,   223,   228,   231,   236,   241,   246,
     251,   256,   261,   266,   271,   273,   279,   284,   287,   290,
     295,   298,   301,   304,   307,   310,   313,   316,   319,   322,
     329,   336,   341,   346,   353,   360,   365,   370,   375,   380,
     387,   394,   403,   412,   420,   428,   433,   438,   441,   443,
     446,   451,   456,   462,   468,   473,   478,   481,   483,   486,
     489,   494,   499,   501,   503,   505,   507,   509,   514,   519,
     524,   529,   534,   539,   544,   549,   554,   559,   561,   563,
     565,   567,   572,   577,   582,   587,   594,   601,   606
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      96,     0,    -1,    -1,    97,    98,    -1,    99,    98,    -1,
      -1,   100,    75,    -1,   102,   105,    -1,   102,   113,    -1,
     105,    -1,   113,    -1,    83,    -1,    84,    -1,    85,    -1,
      70,    -1,   111,    -1,    86,   101,    87,    -1,   101,     8,
     101,    -1,     9,   101,    -1,   101,     9,   101,    -1,   101,
      10,   101,    -1,   101,    11,   101,    -1,   101,    12,   101,
      -1,   101,     4,   101,    -1,   101,     5,   101,    -1,   101,
       3,   101,    -1,    14,   101,    -1,    15,   101,    -1,   101,
       7,   101,    -1,   101,     6,   101,    -1,    86,     1,    87,
      -1,   103,    -1,   103,   102,    -1,   104,    72,    -1,    80,
      -1,    81,    -1,    82,    -1,    81,    80,    -1,    80,    81,
      -1,    82,    80,    -1,    80,    82,    -1,    -1,    77,    76,
      -1,    78,   108,   106,   110,   107,    -1,    88,    -1,    -1,
      89,    -1,    -1,    -1,    90,   101,    91,    -1,    90,     1,
      91,    -1,   101,    -1,   112,    -1,    -1,   109,    -1,   110,
      92,   109,    -1,    71,    -1,    75,    -1,     1,   113,    -1,
      16,    73,    92,    73,    -1,    16,    73,    92,   101,    -1,
      17,    73,    92,    73,    -1,    17,    73,    92,   101,    -1,
      18,   101,    -1,    18,    90,    73,    91,    -1,    18,    90,
       1,    91,    -1,    18,   112,    93,   101,    -1,    18,   112,
      -1,    19,   101,    -1,    20,    -1,    21,    -1,    22,    73,
      92,   101,    -1,    22,    73,    92,    73,    -1,    23,    73,
      92,    73,    -1,    23,    73,    92,   101,    -1,    24,    73,
      -1,    25,    73,    92,    73,    -1,    25,    73,    92,   101,
      -1,    25,   101,    92,    73,    -1,    26,    73,    92,    73,
      -1,    26,    73,    92,   101,    -1,    26,   101,    92,    73,
      -1,    27,    73,    92,    73,    -1,    27,    73,    92,   101,
      -1,    28,    -1,    29,    73,    92,    94,   101,    -1,    29,
      73,    92,   101,    -1,    30,    73,    -1,    32,   101,    -1,
      32,   112,    93,   101,    -1,    32,   112,    -1,    36,   101,
      -1,    39,   101,    -1,    33,   101,    -1,    31,   101,    -1,
      34,   101,    -1,    37,   101,    -1,    35,   101,    -1,    38,
     101,    -1,    40,    73,    92,    90,   101,    91,    -1,    40,
      73,    92,    90,     1,    91,    -1,    40,     1,    92,    73,
      -1,    40,    73,    92,   101,    -1,    40,    90,   101,    91,
      92,    73,    -1,    40,    90,     1,    91,    92,    73,    -1,
      40,    74,    92,    73,    -1,    40,    73,    92,    74,    -1,
      40,    73,    92,    73,    -1,    40,    74,    92,   112,    -1,
      40,    73,    92,    90,    73,    91,    -1,    40,    90,    73,
      91,    92,    73,    -1,    40,    73,    92,    90,    74,    93,
     101,    91,    -1,    40,    90,    74,    93,   101,    91,    92,
      73,    -1,    40,    73,    86,   101,    87,    92,    73,    -1,
      40,    73,    92,    73,    86,   101,    87,    -1,    41,    73,
      92,    73,    -1,    41,    73,    92,   101,    -1,    42,    73,
      -1,    43,    -1,    44,    73,    -1,    45,    73,    92,    73,
      -1,    45,    73,    92,   101,    -1,    46,    73,    92,    94,
     101,    -1,    46,   101,    92,    94,   101,    -1,    46,    73,
      92,   101,    -1,    46,   101,    92,   101,    -1,    49,    73,
      -1,    50,    -1,    51,    73,    -1,    51,   101,    -1,    47,
      73,    92,   101,    -1,    48,    73,    92,   101,    -1,    52,
      -1,    54,    -1,    56,    -1,    53,    -1,    55,    -1,    57,
      73,    92,    73,    -1,    57,    73,    92,   101,    -1,    58,
      73,    92,    73,    -1,    58,    73,    92,   101,    -1,    59,
      73,    92,    73,    -1,    59,    73,    92,   101,    -1,    60,
      73,    92,    73,    -1,    60,    73,    92,   101,    -1,    61,
      73,    92,    73,    -1,    61,    73,    92,   101,    -1,    62,
      -1,    63,    -1,    66,    -1,    64,    -1,    65,    73,    92,
      73,    -1,    65,    73,    92,   101,    -1,    67,    73,    92,
      73,    -1,    67,    73,    92,   101,    -1,    68,    73,    92,
      90,   101,    91,    -1,    68,    73,    92,    90,     1,    91,
      -1,    69,    73,    92,    73,    -1,    69,    73,    92,   101,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   206,   206,   206,   214,   215,   217,   229,   230,   231,
     232,   235,   236,   237,   240,   241,   242,   243,   244,   245,
     246,   247,   248,   249,   250,   251,   252,   253,   254,   255,
     256,   259,   260,   263,   273,   276,   280,   284,   288,   292,
     296,   300,   302,   310,   325,   325,   327,   327,   329,   330,
     337,   339,   342,   347,   348,   353,   359,   368,   376,   378,
     381,   385,   388,   392,   398,   401,   404,   418,   432,   435,
     438,   441,   444,   447,   450,   456,   459,   462,   466,   470,
     473,   477,   481,   484,   487,   489,   492,   498,   501,   507,
     521,   535,   537,   539,   541,   543,   545,   547,   549,   552,
     557,   560,   563,   569,   574,   577,   583,   586,   589,   604,
     607,   610,   616,   623,   632,   641,   644,   650,   653,   656,
     659,   662,   668,   671,   677,   683,   690,   693,   696,   699,
     705,   708,   711,   714,   717,   720,   723,   726,   729,   732,
     735,   738,   741,   744,   747,   750,   753,   756,   758,   761,
     764,   767,   770,   776,   779,   785,   790,   793,   796
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "'|'", "'^'", "'&'", "T_GG", "T_LL",
  "'+'", "'-'", "'*'", "'/'", "'%'", "UNARY_MIN", "'!'", "'~'", "T_ADD",
  "T_AND", "T_CALL", "T_CHIO", "T_CLC", "T_CLI", "T_CLRB", "T_CMP",
  "T_DEC", "T_DIV", "T_REM", "T_GETB", "T_HLT", "T_IN", "T_INC", "T_JC",
  "T_JMP", "T_JNC", "T_JNO", "T_JNS", "T_JNZ", "T_JO", "T_JS", "T_JZ",
  "T_MOV", "T_MUL", "T_NEG", "T_NOP", "T_NOT", "T_OR", "T_OUT", "T_PEEK",
  "T_POKE", "T_POP", "T_POPF", "T_PUSH", "T_PUSHF", "T_RETF", "T_RETN",
  "T_RETFI", "T_RETNI", "T_ROL", "T_ROR", "T_SAL", "T_SAR", "T_SETB",
  "T_STC", "T_STI", "T_STOP", "T_SUB", "T_TRAP", "T_TST", "T_XCHG",
  "T_XOR", "T_NUMBER", "T_ADDRESS", "T_LABEL", "T_GREGISTER",
  "T_SREGISTER", "T_SEGMENT", "T_STRING", "T_DEFSTRING", "T_DEFWORD",
  "T_ERROR", "T_GLOBAL", "T_ALIGN8", "T_PAGE", "T_CODE", "T_DATA",
  "T_CONST", "'('", "')'", "'{'", "'}'", "'['", "']'", "','", "':'", "'?'",
  "$accept", "program", "$@1", "lines", "line", "segtype", "expression",
  "labels", "label", "symdef", "datadef", "opt_oparen", "opt_cparen",
  "size", "expression_or_selector", "values", "symbol", "segment",
  "instruction", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   124,    94,    38,   258,   259,    43,    45,
      42,    47,    37,   260,    33,   126,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,    40,    41,   123,   125,
      91,    93,    44,    58,    63
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
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
     113,   113,   113,   113,   113,   113,   113,   113,   113
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     0,     2,     2,     2,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     2,     3,
       3,     3,     3,     3,     3,     3,     2,     2,     3,     3,
       3,     1,     2,     2,     1,     1,     1,     2,     2,     2,
       2,     0,     2,     5,     1,     0,     1,     0,     0,     3,
       3,     1,     1,     0,     1,     3,     1,     1,     2,     4,
       4,     4,     4,     2,     4,     4,     4,     2,     2,     1,
       1,     4,     4,     4,     4,     2,     4,     4,     4,     4,
       4,     4,     4,     4,     1,     5,     4,     2,     2,     4,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     6,
       6,     4,     4,     6,     6,     4,     4,     4,     4,     6,
       6,     8,     8,     7,     7,     4,     4,     2,     1,     2,
       4,     4,     5,     5,     4,     4,     2,     1,     2,     2,
       4,     4,     1,     1,     1,     1,     1,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     1,     1,     1,
       1,     4,     4,     4,     4,     6,     6,     4,     4
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     0,     1,     0,     0,     0,     0,     0,    69,
      70,     0,     0,     0,     0,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   118,     0,     0,     0,     0,     0,     0,   127,
       0,   132,   135,   133,   136,   134,     0,     0,     0,     0,
       0,   147,   148,   150,     0,   149,     0,     0,     0,     0,
      48,    34,    35,    36,    11,    12,    13,     3,     0,     0,
       0,    31,     0,     9,    10,    58,     0,     0,     0,     0,
       0,    14,    56,    57,     0,     0,    63,    15,    67,    68,
       0,     0,    75,     0,     0,     0,     0,     0,     0,    87,
      94,    88,    90,    93,    95,    97,    91,    96,    98,    92,
       0,     0,     0,     0,     0,   117,   119,     0,     0,     0,
       0,     0,   126,   128,   129,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    42,     0,    45,    38,    40,    37,
      39,     4,     6,     7,     8,    32,    33,     0,     0,    18,
      26,    27,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    44,    53,    59,    60,    61,    62,    30,    16,    65,
      64,    25,    23,    24,    29,    28,    17,    19,    20,    21,
      22,    66,    72,    71,    73,    74,    76,    77,    78,    79,
      80,    81,    82,    83,     0,    86,    89,   101,     0,   107,
     106,     0,   102,   105,   108,     0,     0,     0,     0,   115,
     116,   120,   121,     0,   124,     0,   125,   130,   131,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   151,
     152,   153,   154,     0,   157,   158,    50,    49,    51,    54,
      47,    52,    85,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   122,   123,     0,     0,    46,     0,    43,
       0,     0,   100,   109,     0,    99,   104,   110,     0,   103,
     156,   155,    55,   113,   114,     0,     0,   111,   112
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,    67,    68,    69,   278,    70,    71,    72,
      73,   202,   299,   136,   279,   280,    87,   281,    74
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -84
static const yytype_int16 yypact[] =
{
     -84,    29,   322,   -84,   882,   -43,   -42,   444,   761,   -84,
     -84,   -41,   -39,   -38,   580,   584,    -6,   -84,    -5,    30,
     761,   446,   761,   761,   761,   761,   761,   761,   761,     1,
      31,    32,   -84,    64,    65,   591,    66,    69,    70,   -84,
     598,   -84,   -84,   -84,   -84,   -84,    71,    72,    74,    75,
      90,   -84,   -84,   -84,    92,   -84,    99,   100,   101,    26,
      37,   -55,    95,   110,   -84,   -84,   -84,   -84,   322,   117,
     519,    -2,   121,   -84,   -84,   -84,   136,   141,   761,   761,
     761,   -84,   -84,   -84,   407,     3,   264,   -84,   145,   264,
     147,   149,   -84,   152,    34,   153,    44,   155,   172,   -84,
     264,   264,   185,   264,   264,   264,   264,   264,   264,   264,
     173,   -81,   187,   228,   188,   -84,   -84,   189,   190,    54,
     191,   194,   -84,   -84,   264,   197,   198,   201,   204,   208,
     214,   216,   218,   220,   -84,   409,   207,   -84,   -84,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   602,   609,   -84,
     -84,   -84,   222,    89,   225,   226,   761,   761,   761,   761,
     761,   761,   761,   761,   761,   761,   761,   616,   620,   627,
     212,   634,   232,   694,   217,   761,   240,   761,   436,   -63,
     227,   229,   231,    78,   701,   708,   221,   411,   761,   761,
     712,   719,   726,   730,   737,   744,   748,   235,   811,   230,
     104,   -84,   446,   -84,   264,   -84,   264,   -84,   -84,   -84,
     -84,   113,   457,   123,   251,   251,    13,    13,   -84,   -84,
     -84,   264,   -84,   264,   -84,   264,   -84,   264,   -84,   -84,
     264,   -84,   -84,   264,   761,   264,   264,   -84,   324,   233,
     -84,   400,   264,   -84,   -84,   234,   300,   761,   301,   -84,
     264,   -84,   264,   761,   264,   761,   264,   264,   264,   -84,
     264,   -84,   264,   -84,   264,   -84,   264,   -84,   264,   -84,
     264,   -84,   264,   418,   -84,   264,   -84,   -84,   264,   -84,
     -83,   -84,   264,   303,   761,   305,   306,   319,   193,   325,
     340,   203,   344,   264,   264,   337,   213,   -84,   446,   -84,
     356,   431,   -84,   -84,   761,   -84,   -84,   -84,   338,   -84,
     -84,   -84,   -84,   -84,   -84,   246,   358,   -84,   -84
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -84,   -84,   -84,   376,   -84,   -84,    -7,   375,   -84,   -84,
     377,   -84,   -84,   -84,   150,   -84,   -84,    15,    -1
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -42
static const yytype_int16 yytable[] =
{
      86,    89,   110,    75,   154,   177,   297,    94,    96,   298,
     243,   178,    83,   100,   101,   103,   104,   105,   106,   107,
     108,   109,    88,   163,   164,   165,   137,   138,   119,     3,
      76,    77,    90,   124,    91,    92,   102,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,    97,    98,   144,
     -41,   149,   150,   151,   111,   112,   155,   153,    61,    62,
      63,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   113,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   134,    99,   114,   115,   183,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   170,   135,   200,   159,
     160,   161,   162,   163,   164,   165,   172,   116,   117,   120,
     204,   206,   121,   122,   125,   126,   187,   127,   128,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     223,   225,   227,   129,   230,   130,   233,   235,   236,   248,
     238,   242,   131,   132,   133,   139,   208,   250,   252,   254,
     256,   257,   258,   260,   262,   264,   266,   268,   270,   272,
     140,   275,   142,   146,   244,   277,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,    78,   282,   147,   180,
      78,    79,    80,   148,   288,    79,    80,    78,   166,   167,
     291,   168,    79,    80,   169,   171,   293,   173,   294,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   161,
     162,   163,   164,   165,   174,   176,   296,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   301,   175,   179,
     184,   185,   186,   188,   305,   228,   189,    81,    82,   190,
     191,    81,    82,   192,   308,   201,   193,   315,    81,    82,
     194,   181,   182,    84,   311,   231,   195,    84,   196,   207,
     197,   234,   198,   237,    84,   253,   209,   210,   245,   284,
     246,   276,    -5,     4,   247,   273,   289,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   317,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,   290,   292,   -41,   300,   302,   303,   306,    59,
      60,   285,    61,    62,    63,    64,    65,    66,   152,    78,
     199,   283,   304,   307,    79,    80,    78,   309,    78,   295,
      78,    79,    80,    79,    80,    79,    80,    78,   310,   313,
     316,   318,    79,    80,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   141,    78,   145,   143,   312,     0,
      79,    80,     0,    78,     0,    78,     0,     0,    79,    80,
      79,    80,   158,   159,   160,   161,   162,   163,   164,   165,
      81,    82,     0,   286,   287,     0,     0,    81,    82,    81,
      82,    81,    82,     0,     0,     0,    84,     0,    81,    82,
       0,     0,     0,    84,     0,    84,     0,    84,     0,     0,
       0,     0,     0,     0,    84,   255,    81,    82,     0,   239,
     240,     0,     0,     0,    81,    82,    81,    82,   314,    83,
       4,    83,    84,     0,     0,     0,   241,     0,     0,     0,
      84,     0,    84,     0,    85,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    78,
       0,     0,     0,    78,    79,    80,    59,    60,    79,    80,
      78,     0,     0,     0,     0,    79,    80,    78,     0,     0,
       0,    78,    79,    80,     0,     0,    79,    80,    78,     0,
       0,     0,     0,    79,    80,    78,     0,     0,     0,    78,
      79,    80,     0,     0,    79,    80,    78,     0,     0,     0,
       0,    79,    80,    78,     0,     0,     0,     0,    79,    80,
      81,    82,     0,    93,    81,    82,     0,    95,     0,     0,
       0,    81,    82,     0,   118,     0,    84,     0,    81,    82,
      84,   123,    81,    82,     0,   203,     0,    84,     0,    81,
      82,     0,   205,     0,    84,     0,    81,    82,    84,   222,
      81,    82,     0,   224,     0,    84,     0,    81,    82,     0,
     226,     0,    84,    78,    81,    82,    84,   229,    79,    80,
      78,     0,     0,    84,     0,    79,    80,    78,     0,     0,
      84,    78,    79,    80,     0,     0,    79,    80,    78,     0,
       0,     0,     0,    79,    80,    78,     0,     0,     0,    78,
      79,    80,     0,     0,    79,    80,    78,     0,     0,     0,
       0,    79,    80,    78,     0,     0,     0,    78,    79,    80,
       0,     0,    79,    80,    81,    82,     0,   232,     0,     0,
      78,    81,    82,     0,   249,    79,    80,     0,    81,    82,
      84,   251,    81,    82,     0,   259,     0,    84,     0,    81,
      82,     0,   261,     0,    84,     0,    81,    82,    84,   263,
      81,    82,     0,   265,     0,    84,     0,    81,    82,     0,
     267,     0,    84,     0,    81,    82,    84,   269,    81,    82,
      78,   271,     0,    84,     0,    79,    80,     0,     0,     0,
      84,    81,    82,     0,    84,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    84,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    81,    82,     4,   274,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    84,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58
};

static const yytype_int16 yycheck[] =
{
       7,     8,     1,     4,     1,    86,    89,    14,    15,    92,
      73,    92,    75,    20,    21,    22,    23,    24,    25,    26,
      27,    28,     7,    10,    11,    12,    81,    82,    35,     0,
      73,    73,    73,    40,    73,    73,    21,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    73,    73,    70,
      72,    78,    79,    80,    73,    74,    73,    84,    80,    81,
      82,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    90,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    76,    73,    73,    73,   113,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    92,    90,   135,     6,
       7,     8,     9,    10,    11,    12,    92,    73,    73,    73,
     147,   148,    73,    73,    73,    73,    92,    73,    73,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,    73,   171,    73,   173,   174,   175,    91,
     177,   178,    73,    73,    73,    80,    87,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
      80,   198,    75,    72,   179,    91,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,     9,   234,    92,     1,
       9,    14,    15,    92,   241,    14,    15,     9,    93,    92,
     247,    92,    14,    15,    92,    92,   253,    92,   255,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,     8,
       9,    10,    11,    12,    92,    92,   273,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,   284,    93,    92,
      92,    92,    92,    92,    91,    73,    92,    70,    71,    92,
      92,    70,    71,    92,    91,    88,    92,   304,    70,    71,
      92,    73,    74,    86,    91,    73,    92,    86,    92,    87,
      92,    94,    92,    73,    86,    94,    91,    91,    91,    86,
      91,    91,     0,     1,    93,    90,    92,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    91,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    92,    92,    72,    92,    91,    91,    73,    77,
      78,     1,    80,    81,    82,    83,    84,    85,     1,     9,
       1,    87,    93,    73,    14,    15,     9,    73,     9,     1,
       9,    14,    15,    14,    15,    14,    15,     9,    91,    73,
      92,    73,    14,    15,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    68,     9,    71,    70,   298,    -1,
      14,    15,    -1,     9,    -1,     9,    -1,    -1,    14,    15,
      14,    15,     5,     6,     7,     8,     9,    10,    11,    12,
      70,    71,    -1,    73,    74,    -1,    -1,    70,    71,    70,
      71,    70,    71,    -1,    -1,    -1,    86,    -1,    70,    71,
      -1,    -1,    -1,    86,    -1,    86,    -1,    86,    -1,    -1,
      -1,    -1,    -1,    -1,    86,    94,    70,    71,    -1,    73,
      74,    -1,    -1,    -1,    70,    71,    70,    71,    87,    75,
       1,    75,    86,    -1,    -1,    -1,    90,    -1,    -1,    -1,
      86,    -1,    86,    -1,    90,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,     9,
      -1,    -1,    -1,     9,    14,    15,    77,    78,    14,    15,
       9,    -1,    -1,    -1,    -1,    14,    15,     9,    -1,    -1,
      -1,     9,    14,    15,    -1,    -1,    14,    15,     9,    -1,
      -1,    -1,    -1,    14,    15,     9,    -1,    -1,    -1,     9,
      14,    15,    -1,    -1,    14,    15,     9,    -1,    -1,    -1,
      -1,    14,    15,     9,    -1,    -1,    -1,    -1,    14,    15,
      70,    71,    -1,    73,    70,    71,    -1,    73,    -1,    -1,
      -1,    70,    71,    -1,    73,    -1,    86,    -1,    70,    71,
      86,    73,    70,    71,    -1,    73,    -1,    86,    -1,    70,
      71,    -1,    73,    -1,    86,    -1,    70,    71,    86,    73,
      70,    71,    -1,    73,    -1,    86,    -1,    70,    71,    -1,
      73,    -1,    86,     9,    70,    71,    86,    73,    14,    15,
       9,    -1,    -1,    86,    -1,    14,    15,     9,    -1,    -1,
      86,     9,    14,    15,    -1,    -1,    14,    15,     9,    -1,
      -1,    -1,    -1,    14,    15,     9,    -1,    -1,    -1,     9,
      14,    15,    -1,    -1,    14,    15,     9,    -1,    -1,    -1,
      -1,    14,    15,     9,    -1,    -1,    -1,     9,    14,    15,
      -1,    -1,    14,    15,    70,    71,    -1,    73,    -1,    -1,
       9,    70,    71,    -1,    73,    14,    15,    -1,    70,    71,
      86,    73,    70,    71,    -1,    73,    -1,    86,    -1,    70,
      71,    -1,    73,    -1,    86,    -1,    70,    71,    86,    73,
      70,    71,    -1,    73,    -1,    86,    -1,    70,    71,    -1,
      73,    -1,    86,    -1,    70,    71,    86,    73,    70,    71,
       9,    73,    -1,    86,    -1,    14,    15,    -1,    -1,    -1,
      86,    70,    71,    -1,    86,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    71,     1,    73,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    86,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    96,    97,     0,     1,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,    68,    69,    77,
      78,    80,    81,    82,    83,    84,    85,    98,    99,   100,
     102,   103,   104,   105,   113,   113,    73,    73,     9,    14,
      15,    70,    71,    75,    86,    90,   101,   111,   112,   101,
      73,    73,    73,    73,   101,    73,   101,    73,    73,    73,
     101,   101,   112,   101,   101,   101,   101,   101,   101,   101,
       1,    73,    74,    90,    73,    73,    73,    73,    73,   101,
      73,    73,    73,    73,   101,    73,    73,    73,    73,    73,
      73,    73,    73,    73,    76,    90,   108,    81,    82,    80,
      80,    98,    75,   105,   113,   102,    72,    92,    92,   101,
     101,   101,     1,   101,     1,    73,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    93,    92,    92,    92,
      92,    92,    92,    92,    92,    93,    92,    86,    92,    92,
       1,    73,    74,   101,    92,    92,    92,    92,    92,    92,
      92,    92,    92,    92,    92,    92,    92,    92,    92,     1,
     101,    88,   106,    73,   101,    73,   101,    87,    87,    91,
      91,   101,   101,   101,   101,   101,   101,   101,   101,   101,
     101,   101,    73,   101,    73,   101,    73,   101,    73,    73,
     101,    73,    73,   101,    94,   101,   101,    73,   101,    73,
      74,    90,   101,    73,   112,    91,    91,    93,    91,    73,
     101,    73,   101,    94,   101,    94,   101,   101,   101,    73,
     101,    73,   101,    73,   101,    73,   101,    73,   101,    73,
     101,    73,   101,    90,    73,   101,    91,    91,   101,   109,
     110,   112,   101,    87,    86,     1,    73,    74,   101,    92,
      92,   101,    92,   101,   101,     1,   101,    89,    92,   107,
      92,   101,    91,    91,    93,    91,    73,    73,    91,    73,
      91,    91,   109,    73,    87,   101,    92,    91,    73
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
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



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

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
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1455 of yacc.c  */
#line 206 "cas-yacc.y"
    { if ((current_segment = begin_segment (SEG_DEFAULT, "code*")) == NOT_FOUND) {
		      yyerror ("fatal error");
		      YYABORT;
		  } 
              }
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 212 "cas-yacc.y"
    { end_segment (current_segment, offset); }
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 217 "cas-yacc.y"
    {
              if (global_offset) {
                  yyerror ("explicit segment definitions not allowed with non-zero entry point");
                  YYABORT;
              }
		  end_segment (current_segment, offset);
		  if ((current_segment = begin_segment ((yyvsp[(1) - (2)].i), (yyvsp[(2) - (2)].s))) == NOT_FOUND) {
		      yyerror ("fatal error");
		      YYABORT;
		  }
		  offset = 0;
	      }
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 235 "cas-yacc.y"
    {(yyval.i) = SEG_CODE;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 236 "cas-yacc.y"
    {(yyval.i) = SEG_DATA;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 237 "cas-yacc.y"
    {(yyval.i) = SEG_CONST;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 240 "cas-yacc.y"
    { (yyval.expr) = newConstant ((yyvsp[(1) - (1)].i)); }
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 241 "cas-yacc.y"
    { (yyval.expr) = newLabel ((yyvsp[(1) - (1)].i)); }
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 242 "cas-yacc.y"
    { (yyval.expr) = (yyvsp[(2) - (3)].expr); }
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 243 "cas-yacc.y"
    { (yyval.expr) = do_math ('+', (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); }
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 244 "cas-yacc.y"
    { (yyval.expr) = do_math (C_UNARY_MIN, (yyvsp[(2) - (2)].expr), NULL); }
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 245 "cas-yacc.y"
    { (yyval.expr) = do_math ('-', (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); }
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 246 "cas-yacc.y"
    { (yyval.expr) = do_math ('*', (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); }
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 247 "cas-yacc.y"
    { (yyval.expr) = do_math ('/', (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); }
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 248 "cas-yacc.y"
    { (yyval.expr) = do_math ('%', (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); }
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 249 "cas-yacc.y"
    { (yyval.expr) = do_math ('^', (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); }
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 250 "cas-yacc.y"
    { (yyval.expr) = do_math ('&', (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); }
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 251 "cas-yacc.y"
    { (yyval.expr) = do_math ('|', (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); }
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 252 "cas-yacc.y"
    { (yyval.expr) = do_math ('!', (yyvsp[(2) - (2)].expr), NULL); }
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 253 "cas-yacc.y"
    { (yyval.expr) = do_math ('~', (yyvsp[(2) - (2)].expr), NULL); }
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 254 "cas-yacc.y"
    { (yyval.expr) = do_math (C_LL, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); }
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 255 "cas-yacc.y"
    { (yyval.expr) = do_math (C_GG, (yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); }
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 256 "cas-yacc.y"
    { (yyval.expr) = NULL; yyerror ("malformed expression"); }
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 263 "cas-yacc.y"
    {
                if (NOT_FOUND == add_label ((yyvsp[(2) - (2)].s), current_segment, offset, 
				     (yyvsp[(1) - (2)].sym).global, (yyvsp[(1) - (2)].sym).align8)) {
		    yyerror ("fatal error");
		    YYABORT;
		}
            }
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 273 "cas-yacc.y"
    { 
    (yyval.sym).align8 = 0; (yyval.sym).global = 1;
}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 276 "cas-yacc.y"
    {
    if (current_segment != DEFAULT_SEGMENT) report (0, ALIGNSEGMENT);
    (yyval.sym).align8 = 1; (yyval.sym).global = 0;
}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 280 "cas-yacc.y"
    {
    if (current_segment != DEFAULT_SEGMENT) report (0, ALIGNSEGMENT);
    (yyval.sym).align8 = 2; (yyval.sym).global = 0; 
}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 284 "cas-yacc.y"
    {
    if (current_segment != DEFAULT_SEGMENT) report (0, ALIGNSEGMENT);
    (yyval.sym).align8 = 1; (yyval.sym).global = 1;
}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 288 "cas-yacc.y"
    {
    if (current_segment != DEFAULT_SEGMENT) report (0, ALIGNSEGMENT);
    (yyval.sym).align8 = 1; (yyval.sym).global = 1;
}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 292 "cas-yacc.y"
    {
    if (current_segment != DEFAULT_SEGMENT) report (0, ALIGNSEGMENT);
    (yyval.sym).align8 = 2; (yyval.sym).global = 1;
}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 296 "cas-yacc.y"
    {
    if (current_segment != DEFAULT_SEGMENT) report (0, ALIGNSEGMENT);
    (yyval.sym).align8 = 2; (yyval.sym).global = 1;
}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 300 "cas-yacc.y"
    {(yyval.sym).global = (yyval.sym).align8 = 0; }
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 302 "cas-yacc.y"
    {
                char *s;
		for (s = (yyvsp[(2) - (2)].s); *s; s++) {
		    Dword d = *s;
		    emit (d);
		}
                emit (0);
	    }
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 310 "cas-yacc.y"
    {
		int i;
		if ((yyvsp[(2) - (5)].i) < (yyvsp[(4) - (5)].v).size) {
		    sprintf (error_buffer, "%d initializers given, %"PRIi32" expected",
			     (yyvsp[(4) - (5)].v).size, (yyvsp[(2) - (5)].i));
		    yywarning (error_buffer);
		    (yyvsp[(4) - (5)].v).size = (yyvsp[(2) - (5)].i);
		}
		for (i = 0; i < (yyvsp[(4) - (5)].v).size; i++) {		   
		    emit_expression ((Expression*)(yyvsp[(4) - (5)].v).data[i]);
		}
		for (     ; i < (yyvsp[(2) - (5)].i)     ; i++)
		    emit (0);
	    }
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 329 "cas-yacc.y"
    {(yyval.i) = 1;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 330 "cas-yacc.y"
    {
		if ((yyvsp[(2) - (3)].expr)->type != CONSTANT || (yyvsp[(2) - (3)].expr)->detail.constant <= 0) {
		    report (1, ARRAY_SIZE);
		    (yyval.i) = 0;
		} else
		    (yyval.i) = (yyvsp[(2) - (3)].expr)->detail.constant;
	    }
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 337 "cas-yacc.y"
    { (yyval.i) = -1;  yyerror ("malformed array size");}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 339 "cas-yacc.y"
    {
		(yyval.expr) = (yyvsp[(1) - (1)].expr);
		}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 342 "cas-yacc.y"
    {
		(yyval.expr) = newSelector (MK_SELECTOR ((yyvsp[(1) - (1)].i), 0, _LDT));
	    }
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 347 "cas-yacc.y"
    {(yyval.v).size = 0; (yyval.v).data = NULL;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 348 "cas-yacc.y"
    {
		(yyval.v).data = safe_malloc (sizeof (Dword)); 
		(yyval.v).data[0] = (Dword)(yyvsp[(1) - (1)].expr);
		(yyval.v).size = 1;
	    }
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 353 "cas-yacc.y"
    { 
		(yyval.v).data = safe_realloc ((yyval.v).data, ((yyval.v).size + 1) * sizeof (Dword)); 
		(yyval.v).data[(yyval.v).size] = (Dword)(yyvsp[(3) - (3)].expr);
		(yyval.v).size = (yyval.v).size + 1;
	    }
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 359 "cas-yacc.y"
    {
                int label = use_label ((yyvsp[(1) - (1)].s), current_segment);
		if (NOT_FOUND == label) {
		    yyerror ("fatal error");
		    YYABORT;
		}
                (yyval.i) = label;
            }
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 368 "cas-yacc.y"
    {
              (yyval.i) = lookup_segment ((yyvsp[(1) - (1)].s)); 
	      if (NOT_FOUND == (yyval.i)) {
		  yyerror ("fatal error");
		  YYABORT;
	      }
	  }
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 377 "cas-yacc.y"
    { yyerror ("unrecognized instruction, trying to recover"); }
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 379 "cas-yacc.y"
    {emit (BUILD_INSTRUCTION_A (ADD, (yyvsp[(2) - (4)].i), (yyvsp[(4) - (4)].i)));}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 382 "cas-yacc.y"
    {emit (BUILD_INSTRUCTION_A (xADDI, (yyvsp[(2) - (4)].i), 0));
	      emit_expression ((yyvsp[(4) - (4)].expr));}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 386 "cas-yacc.y"
    {emit (BUILD_INSTRUCTION_A (AND, (yyvsp[(2) - (4)].i), (yyvsp[(4) - (4)].i)));}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 389 "cas-yacc.y"
    {emit (BUILD_INSTRUCTION_A (xANDI, (yyvsp[(2) - (4)].i), 0));
		emit_expression ((yyvsp[(4) - (4)].expr));}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 393 "cas-yacc.y"
    { 
		  emit (BUILD_INSTRUCTION_A (xNCALL, 0, 0));
		  emit_expression ((yyvsp[(2) - (2)].expr));
              }
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 399 "cas-yacc.y"
    {emit (BUILD_INSTRUCTION_A (NCALLX, (yyvsp[(3) - (4)].i), 0));}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 402 "cas-yacc.y"
    { report (1, INDIRECTION); }
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 405 "cas-yacc.y"
    {
		  Selector s = MK_SELECTOR ((yyvsp[(2) - (4)].i), 0, _LDT); 
		  /* the next instruction contains a segment selector 
		     that must be adjusted if there is more than one module
		     in the program */
		  if (module_type == CLOF_EXE) {
		      emit_escape (FIX_SEGMENT); 
                      current_overhead++; /* NOT TESTED */
                  }
		  emit (BUILD_INSTRUCTION_C (xFCALL, 0, s));
		  emit_expression ((yyvsp[(4) - (4)].expr));
	      }
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 419 "cas-yacc.y"
    {
		  Selector s = MK_SELECTOR ((yyvsp[(2) - (2)].i), 0, _LDT); 
		  /* the next instruction contains a segment selector 
		     that must be adjusted if there is more than one module
		     in the program */
		  if (module_type == CLOF_EXE) {
		      emit_escape (FIX_SEGMENT); 
                      current_overhead++; /*NOT TESTED*/
                  }
		  emit (BUILD_INSTRUCTION_C (xFCALL, 0, s));
		  emit_expression (NULL);
	      }
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 433 "cas-yacc.y"
    { emit_displacement (CHIO, 0, (yyvsp[(2) - (2)].expr), 0); }
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 436 "cas-yacc.y"
    { emit (BUILD_INSTRUCTION_A (CLC, 0, 0));}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 439 "cas-yacc.y"
    { emit (BUILD_INSTRUCTION_A (CLI, 0, 0));}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 442 "cas-yacc.y"
    { emit_displacement (CLRBI, (yyvsp[(2) - (4)].i), (yyvsp[(4) - (4)].expr), 0); /* CLRBI */ }
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 445 "cas-yacc.y"
    { emit (BUILD_INSTRUCTION_A (CLRB, (yyvsp[(2) - (4)].i), (yyvsp[(4) - (4)].i)));}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 448 "cas-yacc.y"
    { emit (BUILD_INSTRUCTION_A (CMP, (yyvsp[(2) - (4)].i), (yyvsp[(4) - (4)].i)));}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 451 "cas-yacc.y"
    { 
		  emit (BUILD_INSTRUCTION_A (xCMPI, (yyvsp[(2) - (4)].i), 0));
		  emit_expression ((yyvsp[(4) - (4)].expr));
	      }
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 457 "cas-yacc.y"
    {emit (BUILD_INSTRUCTION_A (DEC, (yyvsp[(2) - (2)].i), 0));}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 460 "cas-yacc.y"
    {emit (BUILD_INSTRUCTION_A (DIV, (yyvsp[(2) - (4)].i), (yyvsp[(4) - (4)].i)));}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 463 "cas-yacc.y"
    {emit (BUILD_INSTRUCTION_A (xDIVI, (yyvsp[(2) - (4)].i), 0));
		emit_expression ((yyvsp[(4) - (4)].expr));}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 467 "cas-yacc.y"
    {emit (BUILD_INSTRUCTION_A (xDIVJ, (yyvsp[(4) - (4)].i), 0));
		emit_expression ((yyvsp[(2) - (4)].expr));}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 471 "cas-yacc.y"
    {emit (BUILD_INSTRUCTION_A (REM, (yyvsp[(2) - (4)].i), (yyvsp[(4) - (4)].i)));}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 474 "cas-yacc.y"
    {emit (BUILD_INSTRUCTION_A (xREMI, (yyvsp[(2) - (4)].i), 0));
		emit_expression ((yyvsp[(4) - (4)].expr));}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 478 "cas-yacc.y"
    {emit (BUILD_INSTRUCTION_A (xREMJ, (yyvsp[(4) - (4)].i), 0));
		emit_expression ((yyvsp[(2) - (4)].expr));}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 482 "cas-yacc.y"
    {emit (BUILD_INSTRUCTION_A (GETB, (yyvsp[(2) - (4)].i), (yyvsp[(4) - (4)].i)));}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 485 "cas-yacc.y"
    { emit_displacement (GETBI, (yyvsp[(2) - (4)].i), (yyvsp[(4) - (4)].expr), 0); }
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 487 "cas-yacc.y"
    {emit (BUILD_INSTRUCTION_A (HLT, 0, 0));}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 490 "cas-yacc.y"
    { emit_displacement (IN, (yyvsp[(2) - (5)].i), (yyvsp[(5) - (5)].expr), 0);}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 493 "cas-yacc.y"
    {
		  report (0, PORT_NAME);
		  emit_displacement (IN, (yyvsp[(2) - (4)].i), (yyvsp[(4) - (4)].expr), 0);
	      }
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 499 "cas-yacc.y"
    {emit (BUILD_INSTRUCTION_A (INC, (yyvsp[(2) - (2)].i), 0));}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 502 "cas-yacc.y"
    { 
		  emit (BUILD_INSTRUCTION_A (xNJMP, 0, 0));
		  emit_expression ((yyvsp[(2) - (2)].expr));
              }
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 508 "cas-yacc.y"
    {
		  Selector s = MK_SELECTOR ((yyvsp[(2) - (4)].i), 0, _LDT); 
		  /* the next instruction contains a segment selector 
		     that must be adjusted if there is more than one module
		     in the program */
		  if (module_type == CLOF_EXE) { /*NOT TESTED */
		      emit_escape (FIX_SEGMENT);
                      current_overhead++; 
                  }
		  emit (BUILD_INSTRUCTION_C (xFJMP, 0, s));
		  emit_expression ((yyvsp[(4) - (4)].expr));
	      }
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 522 "cas-yacc.y"
    {
		  Selector s = MK_SELECTOR ((yyvsp[(2) - (2)].i), 0, _LDT); 
		  /* the next instruction contains a segment selector 
		     that must be adjusted if there is more than one module
		     in the program */
		  if (module_type == CLOF_EXE) { /*NOT TESTED */
		      emit_escape (FIX_SEGMENT); 
                      current_overhead++; 
                  }
		  emit (BUILD_INSTRUCTION_C (xFJMP, 0, s));
		  emit_expression (NULL);
	      }
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 536 "cas-yacc.y"
    { emit_displacement (JNZ, 0, (yyvsp[(2) - (2)].expr), 1); }
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 538 "cas-yacc.y"
    { emit_displacement (JZ, 0, (yyvsp[(2) - (2)].expr), 1); }
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 540 "cas-yacc.y"
    { emit_displacement (JNC, 0, (yyvsp[(2) - (2)].expr), 1); }
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 542 "cas-yacc.y"
    { emit_displacement (JC, 0, (yyvsp[(2) - (2)].expr), 1); }
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 544 "cas-yacc.y"
    { emit_displacement (JNO, 0, (yyvsp[(2) - (2)].expr), 1); }
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 546 "cas-yacc.y"
    { emit_displacement (JO, 0, (yyvsp[(2) - (2)].expr), 1); }
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 548 "cas-yacc.y"
    { emit_displacement (JNS, 0, (yyvsp[(2) - (2)].expr), 1); }
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 550 "cas-yacc.y"
    { emit_displacement (JS, 0, (yyvsp[(2) - (2)].expr), 1); }
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 553 "cas-yacc.y"
    { 
		  emit (BUILD_INSTRUCTION_A (xLD, (yyvsp[(2) - (6)].i), 0));
		  emit_expression ((yyvsp[(5) - (6)].expr));
              }
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 558 "cas-yacc.y"
    { report (1, INDIRECTION); }
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 561 "cas-yacc.y"
    { yyerror ("Did you forget [] around the variable name?"); }
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 564 "cas-yacc.y"
    {
               emit (BUILD_INSTRUCTION_A (xLDI, (yyvsp[(2) - (4)].i), 0));
               emit_expression ((yyvsp[(4) - (4)].expr));
	       }
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 570 "cas-yacc.y"
    { 
		  emit (BUILD_INSTRUCTION_A (xST, (yyvsp[(6) - (6)].i), 0));
		  emit_expression ((yyvsp[(3) - (6)].expr));
              }
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 575 "cas-yacc.y"
    { report (1, INDIRECTION); }
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 578 "cas-yacc.y"
    {
                  if ((yyvsp[(2) - (4)].i)==_CODE) { report (1, BADUSEOFCS); YYABORT; }
                  emit (BUILD_INSTRUCTION_A (MOVTS, (yyvsp[(2) - (4)].i), (yyvsp[(4) - (4)].i)));
              }
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 584 "cas-yacc.y"
    {emit (BUILD_INSTRUCTION_A (MOVFS, (yyvsp[(2) - (4)].i), (yyvsp[(4) - (4)].i)));}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 587 "cas-yacc.y"
    {emit (BUILD_INSTRUCTION_A (MOV, (yyvsp[(2) - (4)].i), (yyvsp[(4) - (4)].i)));}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 590 "cas-yacc.y"
    {
		  Selector s;
                  if ((yyvsp[(2) - (4)].i)==_CODE) { report (1, BADUSEOFCS); YYABORT; }
		  s = MK_SELECTOR ((yyvsp[(4) - (4)].i), 0, _LDT); 
		  /* the next instruction contains a segment selector 
		     that must be adjusted if there is more than one module
		     in the program */
		  if (module_type == CLOF_EXE) {
		      emit_escape (FIX_SEGMENT); /*NOT TESTED*/
                      current_overhead++; 
                  }
		  emit (BUILD_INSTRUCTION_C (MOVSI, (yyvsp[(2) - (4)].i), s)); /*WORKS*/
	      }
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 605 "cas-yacc.y"
    {emit (BUILD_INSTRUCTION_A (LDX, (yyvsp[(2) - (6)].i), (yyvsp[(5) - (6)].i)));}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 608 "cas-yacc.y"
    {emit (BUILD_INSTRUCTION_A (STX, (yyvsp[(3) - (6)].i), (yyvsp[(6) - (6)].i)));}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 611 "cas-yacc.y"
    {
		  emit (BUILD_INSTRUCTION_A (xLDS, (yyvsp[(2) - (8)].i), (yyvsp[(5) - (8)].i)));
                  emit_expression ((yyvsp[(7) - (8)].expr));
	      }
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 617 "cas-yacc.y"
    {
                  if ((yyvsp[(3) - (8)].i)==_CODE) { report (1, BADUSEOFCS); YYABORT; }
		  emit (BUILD_INSTRUCTION_A (xSTS, (yyvsp[(3) - (8)].i), (yyvsp[(8) - (8)].i)));
                  emit_expression ((yyvsp[(5) - (8)].expr));
	      }
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 624 "cas-yacc.y"
    {
		  if ((yyvsp[(4) - (7)].expr)->type != CONSTANT) {
		      report (1, VARINDEX);
		  } else {
		      emit (BUILD_INSTRUCTION_D (SETBY, (yyvsp[(7) - (7)].i), (yyvsp[(2) - (7)].i), (yyvsp[(4) - (7)].expr)->detail.constant));
		  } /* SETBY */
	      }
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 633 "cas-yacc.y"
    {
		  if ((yyvsp[(6) - (7)].expr)->type != CONSTANT) {
		      report (1, VARINDEX);
		  } else {
		      emit (BUILD_INSTRUCTION_D (GETBY, (yyvsp[(2) - (7)].i), (yyvsp[(4) - (7)].i), (yyvsp[(6) - (7)].expr)->detail.constant));
		  } /* GETBY */
	      }
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 642 "cas-yacc.y"
    {emit (BUILD_INSTRUCTION_A (MUL, (yyvsp[(2) - (4)].i), (yyvsp[(4) - (4)].i)));}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 645 "cas-yacc.y"
    {
		  emit (BUILD_INSTRUCTION_A (xMULI, (yyvsp[(2) - (4)].i), 0));
		  emit_expression ((yyvsp[(4) - (4)].expr));
	      }
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 651 "cas-yacc.y"
    {emit (BUILD_INSTRUCTION_A (NEG, (yyvsp[(2) - (2)].i), 0));}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 654 "cas-yacc.y"
    {emit (BUILD_INSTRUCTION_A (NOP, 0, 0));}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 657 "cas-yacc.y"
    {emit (BUILD_INSTRUCTION_A (NOT, (yyvsp[(2) - (2)].i), 0));}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 660 "cas-yacc.y"
    {emit (BUILD_INSTRUCTION_A (OR, (yyvsp[(2) - (4)].i), (yyvsp[(4) - (4)].i)));}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 663 "cas-yacc.y"
    {
		  emit (BUILD_INSTRUCTION_A (xORI, (yyvsp[(2) - (4)].i), 0));
		  emit_expression ((yyvsp[(4) - (4)].expr));
	      }
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 669 "cas-yacc.y"
    { emit_displacement (OUT, (yyvsp[(2) - (5)].i), (yyvsp[(5) - (5)].expr), 0); }
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 672 "cas-yacc.y"
    { 
		  emit_displacement (xOUTI, 0, (yyvsp[(5) - (5)].expr), 0); 
		  emit_expression ((yyvsp[(2) - (5)].expr));
	      }
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 678 "cas-yacc.y"
    {
		  report (0, PORT_NAME);
		  emit_displacement (OUT, (yyvsp[(2) - (4)].i), (yyvsp[(4) - (4)].expr), 0); 
	      }
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 684 "cas-yacc.y"
    {
		  report (0, PORT_NAME);
		  emit_displacement (xOUTI, 0, (yyvsp[(4) - (4)].expr), 0); 
		  emit_expression ((yyvsp[(2) - (4)].expr));
	       }
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 691 "cas-yacc.y"
    {emit (BUILD_INSTRUCTION_A (POP, (yyvsp[(2) - (2)].i), 0));}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 694 "cas-yacc.y"
    {emit (BUILD_INSTRUCTION_A (POPF, 0, 0));}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 697 "cas-yacc.y"
    {emit (BUILD_INSTRUCTION_A (PUSH, (yyvsp[(2) - (2)].i), 0));}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 700 "cas-yacc.y"
    {
		  emit (BUILD_INSTRUCTION_A (xPUSHI, 0, 0));
		  emit_expression ((yyvsp[(2) - (2)].expr));
	      }
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 706 "cas-yacc.y"
    { emit_displacement (PEEK, (yyvsp[(2) - (4)].i), (yyvsp[(4) - (4)].expr), 0); }
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 709 "cas-yacc.y"
    { emit_displacement (POKE, (yyvsp[(2) - (4)].i), (yyvsp[(4) - (4)].expr), 0); }
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 712 "cas-yacc.y"
    {emit (BUILD_INSTRUCTION_A (PUSHF, 0, 0));}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 715 "cas-yacc.y"
    {emit (BUILD_INSTRUCTION_A (NRET, 0, 0));}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 718 "cas-yacc.y"
    {emit (BUILD_INSTRUCTION_A (INRET, 0, 0));}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 721 "cas-yacc.y"
    { emit (BUILD_INSTRUCTION_A (FRET, 0, 0));}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 724 "cas-yacc.y"
    { emit (BUILD_INSTRUCTION_A (IFRET, 0, 0));}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 727 "cas-yacc.y"
    { emit (BUILD_INSTRUCTION_A (ROL, (yyvsp[(2) - (4)].i), (yyvsp[(4) - (4)].i)));}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 730 "cas-yacc.y"
    { emit_displacement (ROLI, (yyvsp[(2) - (4)].i), (yyvsp[(4) - (4)].expr), 0); }
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 733 "cas-yacc.y"
    { emit (BUILD_INSTRUCTION_A (ROR, (yyvsp[(2) - (4)].i), (yyvsp[(4) - (4)].i)));}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 736 "cas-yacc.y"
    { emit_displacement (RORI, (yyvsp[(2) - (4)].i), (yyvsp[(4) - (4)].expr), 0); }
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 739 "cas-yacc.y"
    {emit (BUILD_INSTRUCTION_A (SAL, (yyvsp[(2) - (4)].i), (yyvsp[(4) - (4)].i)));}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 742 "cas-yacc.y"
    { emit_displacement (SALI, (yyvsp[(2) - (4)].i), (yyvsp[(4) - (4)].expr), 0); }
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 745 "cas-yacc.y"
    { emit (BUILD_INSTRUCTION_A (SAR, (yyvsp[(2) - (4)].i), (yyvsp[(4) - (4)].i)));}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 748 "cas-yacc.y"
    { emit_displacement (SARI, (yyvsp[(2) - (4)].i), (yyvsp[(4) - (4)].expr), 0); }
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 751 "cas-yacc.y"
    { emit (BUILD_INSTRUCTION_A (SETB, (yyvsp[(2) - (4)].i), (yyvsp[(4) - (4)].i)));}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 754 "cas-yacc.y"
    { emit_displacement (SETBI, (yyvsp[(2) - (4)].i), (yyvsp[(4) - (4)].expr), 0); }
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 756 "cas-yacc.y"
    {emit (BUILD_INSTRUCTION_A (STC, 0, 0));}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 759 "cas-yacc.y"
    {emit (BUILD_INSTRUCTION_A (STI, 0, 0));}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 762 "cas-yacc.y"
    {emit (BUILD_INSTRUCTION_A (TRAP, 0, 0));}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 765 "cas-yacc.y"
    {emit (BUILD_INSTRUCTION_A (STOP, 0, 0));}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 768 "cas-yacc.y"
    {emit (BUILD_INSTRUCTION_A (SUB, (yyvsp[(2) - (4)].i), (yyvsp[(4) - (4)].i)));}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 771 "cas-yacc.y"
    {
		  emit (BUILD_INSTRUCTION_A (xSUBI, (yyvsp[(2) - (4)].i), 0));
		  emit_expression ((yyvsp[(4) - (4)].expr));
	      }
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 777 "cas-yacc.y"
    {emit (BUILD_INSTRUCTION_A (TST, (yyvsp[(2) - (4)].i), (yyvsp[(4) - (4)].i)));}
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 780 "cas-yacc.y"
    {
		  emit (BUILD_INSTRUCTION_A (xTSTI, (yyvsp[(2) - (4)].i), 0));
		  emit_expression ((yyvsp[(4) - (4)].expr));
	      }
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 786 "cas-yacc.y"
    { 
		  emit (BUILD_INSTRUCTION_A (xXCHG, (yyvsp[(2) - (6)].i), 0));
		  emit_expression ((yyvsp[(5) - (6)].expr));
              }
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 791 "cas-yacc.y"
    { report (1, INDIRECTION); }
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 794 "cas-yacc.y"
    {emit (BUILD_INSTRUCTION_A (XOR, (yyvsp[(2) - (4)].i), (yyvsp[(4) - (4)].i)));}
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 797 "cas-yacc.y"
    {
		  emit (BUILD_INSTRUCTION_A (xXORI, (yyvsp[(2) - (4)].i), 0));
		  emit_expression ((yyvsp[(4) - (4)].expr));
	      }
    break;



/* Line 1455 of yacc.c  */
#line 3240 "cas-yacc.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
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
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 804 "cas-yacc.y"


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
	emit_escape ((Dword)e);
	current_overhead += 1 + expression_overhead (e);
	break;	
    case DUMMY:
	assert (e->type != DUMMY);
    }
}

static void emit_displacement (int opc, int op1, Expression *dspl, Bit relative) 
{
    emit_escape (relative ? FIX_RDISPLACEMENT : FIX_ADISPLACEMENT);
    emit (BUILD_INSTRUCTION_B (opc, op1, 0));
    emit_escape ((Dword)dspl);
    if (relative) {
	emit_escape (offset);   /* current_offset */
    } else {
	current_overhead += 1 + expression_overhead (dspl);
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


