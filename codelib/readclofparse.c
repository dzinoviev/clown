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
#line 1 "readclofparse.y"

#include <stdio.h>
#include "clowndev.h"
  struct Module *modules = NULL;
  int current_module;
  char **source = NULL;
  void yyerror(char *s);
  int yylex(); 
  
  static void new_segment(struct Segment *s);
  static void new_symbol(struct Label *s);

#line 84 "readclofparse.c"

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
    T_SIZE = 272,                  /* T_SIZE  */
    T_SYMBOL = 273,                /* T_SYMBOL  */
    T_SYMBOLS = 274,               /* T_SYMBOLS  */
    T_ESYMBOLS = 275,              /* T_ESYMBOLS  */
    T_TYPE = 276,                  /* T_TYPE  */
    T_LS = 277,                    /* T_LS  */
    T_SG = 278,                    /* T_SG  */
    T_L = 279,                     /* T_L  */
    T_G = 280,                     /* T_G  */
    T_CODE = 281,                  /* T_CODE  */
    T_STRING = 282,                /* T_STRING  */
    T_NUMBER = 283                 /* T_NUMBER  */
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
#define T_SIZE 272
#define T_SYMBOL 273
#define T_SYMBOLS 274
#define T_ESYMBOLS 275
#define T_TYPE 276
#define T_LS 277
#define T_SG 278
#define T_L 279
#define T_G 280
#define T_CODE 281
#define T_STRING 282
#define T_NUMBER 283

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 14 "readclofparse.y"

  char *s;
  int i;
  struct {
    int size;
    //	int escapes;
    Dword *bin;
  } code;
  struct Segment seg;
  struct Label sym;
  struct MyDebug di;
  struct DebugFile df;
  struct DebugInfo dl;

#line 208 "readclofparse.c"

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
  YYSYMBOL_T_BIN = 3,                      /* T_BIN  */
  YYSYMBOL_T_CLOFEXE = 4,                  /* T_CLOFEXE  */
  YYSYMBOL_T_CLEFEXE = 5,                  /* T_CLEFEXE  */
  YYSYMBOL_T_CODESTAR = 6,                 /* T_CODESTAR  */
  YYSYMBOL_T_DEFINED = 7,                  /* T_DEFINED  */
  YYSYMBOL_T_FILE = 8,                     /* T_FILE  */
  YYSYMBOL_T_GLOBAL = 9,                   /* T_GLOBAL  */
  YYSYMBOL_T_ID = 10,                      /* T_ID  */
  YYSYMBOL_T_LINE = 11,                    /* T_LINE  */
  YYSYMBOL_T_LINENO = 12,                  /* T_LINENO  */
  YYSYMBOL_T_NAME = 13,                    /* T_NAME  */
  YYSYMBOL_T_OFFSET = 14,                  /* T_OFFSET  */
  YYSYMBOL_T_SEGMENT = 15,                 /* T_SEGMENT  */
  YYSYMBOL_T_SEGMENTS = 16,                /* T_SEGMENTS  */
  YYSYMBOL_T_SIZE = 17,                    /* T_SIZE  */
  YYSYMBOL_T_SYMBOL = 18,                  /* T_SYMBOL  */
  YYSYMBOL_T_SYMBOLS = 19,                 /* T_SYMBOLS  */
  YYSYMBOL_T_ESYMBOLS = 20,                /* T_ESYMBOLS  */
  YYSYMBOL_T_TYPE = 21,                    /* T_TYPE  */
  YYSYMBOL_T_LS = 22,                      /* T_LS  */
  YYSYMBOL_T_SG = 23,                      /* T_SG  */
  YYSYMBOL_T_L = 24,                       /* T_L  */
  YYSYMBOL_T_G = 25,                       /* T_G  */
  YYSYMBOL_T_CODE = 26,                    /* T_CODE  */
  YYSYMBOL_T_STRING = 27,                  /* T_STRING  */
  YYSYMBOL_T_NUMBER = 28,                  /* T_NUMBER  */
  YYSYMBOL_29_ = 29,                       /* '='  */
  YYSYMBOL_30_ = 30,                       /* '"'  */
  YYSYMBOL_YYACCEPT = 31,                  /* $accept  */
  YYSYMBOL_module = 32,                    /* module  */
  YYSYMBOL_exetype = 33,                   /* exetype  */
  YYSYMBOL_sec_segments = 34,              /* sec_segments  */
  YYSYMBOL_segments = 35,                  /* segments  */
  YYSYMBOL_segment = 36,                   /* segment  */
  YYSYMBOL_37_1 = 37,                      /* $@1  */
  YYSYMBOL_seg_attribs = 38,               /* seg_attribs  */
  YYSYMBOL_sec_symbols = 39,               /* sec_symbols  */
  YYSYMBOL_symbols = 40,                   /* symbols  */
  YYSYMBOL_symbol = 41,                    /* symbol  */
  YYSYMBOL_sym_attribs = 42,               /* sym_attribs  */
  YYSYMBOL_bins = 43,                      /* bins  */
  YYSYMBOL_bin = 44,                       /* bin  */
  YYSYMBOL_bin_segno = 45,                 /* bin_segno  */
  YYSYMBOL_bin_segsize = 46,               /* bin_segsize  */
  YYSYMBOL_debuginfo = 47,                 /* debuginfo  */
  YYSYMBOL_debugfile = 48,                 /* debugfile  */
  YYSYMBOL_debuglines = 49,                /* debuglines  */
  YYSYMBOL_debugline = 50,                 /* debugline  */
  YYSYMBOL_oq = 51                         /* oq  */
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
typedef yytype_uint8 yy_state_t;

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
#define YYFINAL  6
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   122

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  31
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  21
/* YYNRULES -- Number of rules.  */
#define YYNRULES  40
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  141

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   283


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
       2,     2,     2,     2,    30,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    29,     2,     2,     2,     2,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    68,    68,    77,    78,    81,    84,    85,    88,    88,
      95,    96,    97,    98,    99,   100,   103,   104,   107,   108,
     111,   114,   115,   116,   117,   118,   119,   120,   123,   124,
     127,   136,   138,   140,   145,   148,   152,   160,   163,   167,
     167
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
  "\"end of file\"", "error", "\"invalid token\"", "T_BIN", "T_CLOFEXE",
  "T_CLEFEXE", "T_CODESTAR", "T_DEFINED", "T_FILE", "T_GLOBAL", "T_ID",
  "T_LINE", "T_LINENO", "T_NAME", "T_OFFSET", "T_SEGMENT", "T_SEGMENTS",
  "T_SIZE", "T_SYMBOL", "T_SYMBOLS", "T_ESYMBOLS", "T_TYPE", "T_LS",
  "T_SG", "T_L", "T_G", "T_CODE", "T_STRING", "T_NUMBER", "'='", "'\"'",
  "$accept", "module", "exetype", "sec_segments", "segments", "segment",
  "$@1", "seg_attribs", "sec_symbols", "symbols", "symbol", "sym_attribs",
  "bins", "bin", "bin_segno", "bin_segsize", "debuginfo", "debugfile",
  "debuglines", "debugline", "oq", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-57)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
     -20,    15,    17,   -57,   -57,     7,   -57,     3,    20,    22,
      23,    24,   -57,   -57,   -57,    -8,     1,   -14,    15,    42,
     -57,    38,    40,   -57,    28,    39,   -57,    31,    43,    34,
     -57,   -57,   -57,   -57,    32,    45,   -57,     8,    37,    33,
      35,    41,    36,    44,    47,    49,   -57,    50,    52,    54,
      55,    56,    57,   -57,   -57,    46,    33,    61,    33,    33,
      58,    33,   -57,    33,    33,    33,    59,    33,    33,    33,
      62,    48,    63,    64,    -5,    65,     2,    66,    67,    68,
      70,    71,    72,   -57,    33,    69,    33,    33,    73,    74,
      33,    53,    90,   -57,    33,    33,    33,    75,    33,    33,
     -57,    76,   -57,   -57,   -57,   -57,   -57,    77,    93,   -57,
     -57,   -57,   -57,   -57,   -57,   -57,   -57,    78,    79,    81,
      80,    86,   -57,    13,   104,    60,   -57,    88,   100,   -57,
      87,    33,    89,    33,   103,    91,    33,    94,    33,    95,
     -57
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     3,     4,     0,     1,     0,     0,    17,
       0,     0,    29,     7,    19,     0,     0,     0,     0,     0,
      28,     0,     0,     6,     0,     0,    18,     0,     0,     0,
      15,    16,    27,     2,     0,     0,     5,     0,     0,    40,
       0,     0,     0,     0,     0,     0,     8,     0,     0,     0,
       0,     0,     0,    20,    39,     0,    40,     0,    40,    40,
       0,    40,    34,    40,    40,    40,     0,    40,    40,    40,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    31,    40,     0,    40,    40,     0,     0,
      40,     0,     0,    33,    40,    40,    40,     0,    40,    40,
      32,     0,    12,    14,    11,    10,    13,     0,     0,    25,
      26,    24,    21,    23,    22,    30,     9,     0,     0,     0,
       0,     0,    37,     0,     0,     0,    36,     0,     0,    35,
       0,    40,     0,    40,     0,     0,    40,     0,    40,     0,
      38
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -57,   -57,    51,   -57,   -57,   -57,   -57,   -57,   -57,   -57,
     -57,   -57,   -57,   -57,   -57,   -57,   -57,   -57,   -57,   -57,
     -56
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     2,     5,     9,    16,    23,    62,    37,    12,    17,
      26,    38,    15,    20,    35,    41,    76,    93,   123,   126,
      55
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      70,    88,    72,    73,     1,    75,    24,    77,    78,    79,
      25,    81,    82,    83,    18,    42,    19,     6,    43,     3,
       4,    44,    89,    21,    91,    22,    92,     8,   100,    45,
     102,   103,     7,    46,   106,   124,    10,   125,   109,   110,
     111,    11,   113,   114,    47,    28,    48,    49,    13,    14,
      50,    51,    52,    31,    29,    30,    33,    32,    34,    36,
      53,    39,    40,    54,    56,    58,    57,     0,   107,    27,
      85,   128,   101,    59,    69,   132,    60,   134,    61,    63,
     137,    64,   139,    65,    66,    67,    68,    71,    74,    80,
      84,    86,    87,    90,    94,    95,    96,    97,   108,    98,
      99,   115,   116,   104,   105,   112,   117,   118,   120,   119,
     121,   122,   127,   129,   130,   135,   131,   133,   140,     0,
     136,     0,   138
};

static const yytype_int16 yycheck[] =
{
      56,     6,    58,    59,    24,    61,    20,    63,    64,    65,
      24,    67,    68,    69,    22,     7,    24,     0,    10,     4,
       5,    13,    27,    22,    22,    24,    24,    24,    84,    21,
      86,    87,    25,    25,    90,    22,    16,    24,    94,    95,
      96,    19,    98,    99,     7,     3,     9,    10,    25,    25,
      13,    14,    15,    25,    16,    15,    25,    18,    15,    25,
      23,    29,    17,    30,    29,    29,    25,    -1,    15,    18,
      22,    11,     3,    29,    28,   131,    29,   133,    29,    29,
     136,    29,   138,    29,    29,    29,    29,    26,    30,    30,
      28,    28,    28,    28,    28,    28,    28,    27,     8,    28,
      28,    25,    25,    30,    30,    30,    13,    29,    27,    30,
      30,    25,     8,    25,    14,    12,    29,    28,    23,    -1,
      29,    -1,    28
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    24,    32,     4,     5,    33,     0,    25,    24,    34,
      16,    19,    39,    25,    25,    43,    35,    40,    22,    24,
      44,    22,    24,    36,    20,    24,    41,    33,     3,    16,
      15,    25,    18,    25,    15,    45,    25,    38,    42,    29,
      17,    46,     7,    10,    13,    21,    25,     7,     9,    10,
      13,    14,    15,    23,    30,    51,    29,    25,    29,    29,
      29,    29,    37,    29,    29,    29,    29,    29,    29,    28,
      51,    26,    51,    51,    30,    51,    47,    51,    51,    51,
      30,    51,    51,    51,    28,    22,    28,    28,     6,    27,
      28,    22,    24,    48,    28,    28,    28,    27,    28,    28,
      51,     3,    51,    51,    30,    30,    51,    15,     8,    51,
      51,    51,    30,    51,    51,    25,    25,    13,    29,    30,
      27,    30,    25,    49,    22,    24,    50,     8,    11,    25,
      14,    29,    51,    28,    51,    12,    29,    51,    28,    51,
      23
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    31,    32,    33,    33,    34,    35,    35,    37,    36,
      38,    38,    38,    38,    38,    38,    39,    39,    40,    40,
      41,    42,    42,    42,    42,    42,    42,    42,    43,    43,
      44,    45,    46,    47,    47,    48,    49,    49,    50,    51,
      51
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     9,     1,     1,     7,     2,     0,     0,     9,
       6,     6,     6,     6,     6,     0,     5,     0,     2,     0,
       4,     6,     6,     6,     6,     6,     6,     0,     2,     0,
       9,     5,     5,     2,     0,    12,     2,     0,    13,     1,
       0
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
  case 2: /* module: T_L exetype T_G sec_segments sec_symbols bins T_LS exetype T_G  */
#line 68 "readclofparse.y"
                                                                               {
  if ((yyvsp[-7].i) != (yyvsp[-1].i)) {
    yyerror("opening tag does not match closing tag");
    YYABORT;
  }
  modules[current_module].type = (yyvsp[-7].i);
 }
#line 1322 "readclofparse.c"
    break;

  case 3: /* exetype: T_CLOFEXE  */
#line 77 "readclofparse.y"
                          { (yyval.i) = CLOF_EXE; }
#line 1328 "readclofparse.c"
    break;

  case 4: /* exetype: T_CLEFEXE  */
#line 78 "readclofparse.y"
                  { (yyval.i) = CLOF_BIN; }
#line 1334 "readclofparse.c"
    break;

  case 8: /* $@1: %empty  */
#line 88 "readclofparse.y"
                                              { new_segment(&((yyvsp[-1].seg))); }
#line 1340 "readclofparse.c"
    break;

  case 9: /* segment: T_L T_SEGMENT seg_attribs T_G $@1 debuginfo T_LS T_SEGMENT T_G  */
#line 89 "readclofparse.y"
                             { 
  modules[current_module].st.segments[(yyvsp[-6].seg).id].nfiles = (yyvsp[-3].di).nfiles;
  modules[current_module].st.segments[(yyvsp[-6].seg).id].files = (yyvsp[-3].di).files;
}
#line 1349 "readclofparse.c"
    break;

  case 10: /* seg_attribs: seg_attribs T_NAME '=' '"' T_STRING '"'  */
#line 95 "readclofparse.y"
                                                             { (yyval.seg).name = (yyvsp[-1].s); }
#line 1355 "readclofparse.c"
    break;

  case 11: /* seg_attribs: seg_attribs T_NAME '=' '"' T_CODESTAR '"'  */
#line 96 "readclofparse.y"
                                                     { (yyval.seg).name = NULL; }
#line 1361 "readclofparse.c"
    break;

  case 12: /* seg_attribs: seg_attribs T_DEFINED '=' oq T_NUMBER oq  */
#line 97 "readclofparse.y"
                                                     { (yyval.seg).defined = (yyvsp[-1].i); }
#line 1367 "readclofparse.c"
    break;

  case 13: /* seg_attribs: seg_attribs T_TYPE '=' oq T_NUMBER oq  */
#line 98 "readclofparse.y"
                                                     { (yyval.seg).type = (yyvsp[-1].i); }
#line 1373 "readclofparse.c"
    break;

  case 14: /* seg_attribs: seg_attribs T_ID '=' oq T_NUMBER oq  */
#line 99 "readclofparse.y"
                                                     { (yyval.seg).id = (yyvsp[-1].i); }
#line 1379 "readclofparse.c"
    break;

  case 15: /* seg_attribs: %empty  */
#line 100 "readclofparse.y"
        {}
#line 1385 "readclofparse.c"
    break;

  case 20: /* symbol: T_L T_SYMBOL sym_attribs T_SG  */
#line 111 "readclofparse.y"
                                              { new_symbol(&((yyvsp[-1].sym))); }
#line 1391 "readclofparse.c"
    break;

  case 21: /* sym_attribs: sym_attribs T_NAME '=' '"' T_STRING '"'  */
#line 114 "readclofparse.y"
                                                           { (yyval.sym).name = (yyvsp[-1].s);  }
#line 1397 "readclofparse.c"
    break;

  case 22: /* sym_attribs: sym_attribs T_SEGMENT '=' oq T_NUMBER oq  */
#line 115 "readclofparse.y"
                                                   { (yyval.sym).segment = (yyvsp[-1].i); }
#line 1403 "readclofparse.c"
    break;

  case 23: /* sym_attribs: sym_attribs T_OFFSET '=' oq T_NUMBER oq  */
#line 116 "readclofparse.y"
                                                   { (yyval.sym).address = (yyvsp[-1].i); }
#line 1409 "readclofparse.c"
    break;

  case 24: /* sym_attribs: sym_attribs T_ID '=' oq T_NUMBER oq  */
#line 117 "readclofparse.y"
                                                   { (yyval.sym).id = (yyvsp[-1].i);      }
#line 1415 "readclofparse.c"
    break;

  case 25: /* sym_attribs: sym_attribs T_DEFINED '=' oq T_NUMBER oq  */
#line 118 "readclofparse.y"
                                                   { (yyval.sym).defined = (yyvsp[-1].i); }
#line 1421 "readclofparse.c"
    break;

  case 26: /* sym_attribs: sym_attribs T_GLOBAL '=' oq T_NUMBER oq  */
#line 119 "readclofparse.y"
                                                   { (yyval.sym).export = (yyvsp[-1].i);  }
#line 1427 "readclofparse.c"
    break;

  case 27: /* sym_attribs: %empty  */
#line 120 "readclofparse.y"
        {}
#line 1433 "readclofparse.c"
    break;

  case 30: /* bin: T_L T_BIN bin_segno bin_segsize T_G T_CODE T_LS T_BIN T_G  */
#line 127 "readclofparse.y"
                                                                          { 
  modules[current_module].st.segments[(yyvsp[-6].i)].code_size = (yyvsp[-5].i); 
  modules[current_module].st.segments[(yyvsp[-6].i)].image_size = (yyvsp[-3].code).size; 
  //  modules[current_module].st.segments[$3].escapes = $6.escapes; 
  modules[current_module].st.segments[(yyvsp[-6].i)].image = (yyvsp[-3].code).bin; 
  //  modules[current_module].st.segments[$3].link_overhead = link_overhead; 
}
#line 1445 "readclofparse.c"
    break;

  case 31: /* bin_segno: T_SEGMENT '=' oq T_NUMBER oq  */
#line 136 "readclofparse.y"
                                             { (yyval.i) = (yyvsp[-1].i); }
#line 1451 "readclofparse.c"
    break;

  case 32: /* bin_segsize: T_SIZE '=' oq T_NUMBER oq  */
#line 138 "readclofparse.y"
                                          { (yyval.i) = (yyvsp[-1].i); }
#line 1457 "readclofparse.c"
    break;

  case 33: /* debuginfo: debuginfo debugfile  */
#line 140 "readclofparse.y"
                                    { 
  (yyval.di).nfiles = (yyvsp[-1].di).nfiles + 1; 
  (yyval.di).files = safe_realloc((yyvsp[-1].di).files, sizeof(struct DebugFile) * (yyval.di).nfiles); 
  (yyval.di).files[(yyval.di).nfiles - 1] = (yyvsp[0].df);
}
#line 1467 "readclofparse.c"
    break;

  case 34: /* debuginfo: %empty  */
#line 145 "readclofparse.y"
  { (yyval.di).nfiles = 0; (yyval.di).files = NULL; }
#line 1473 "readclofparse.c"
    break;

  case 35: /* debugfile: T_L T_FILE T_NAME '=' '"' T_STRING '"' T_G debuglines T_LS T_FILE T_G  */
#line 149 "readclofparse.y"
{ (yyval.df) = (yyvsp[-3].df); (yyval.df).file = (yyvsp[-6].s); }
#line 1479 "readclofparse.c"
    break;

  case 36: /* debuglines: debuglines debugline  */
#line 152 "readclofparse.y"
                                     {
  if ((yyval.df).nlines_inuse >= (yyval.df).nlines) {
    (yyval.df).nlines = (yyval.df).nlines + IMAGE_CHUNK;
    (yyval.df).flines = safe_realloc((yyval.df).flines, (yyval.df).nlines * sizeof(struct DebugInfo));
  }
  (yyval.df).flines[(yyval.df).nlines_inuse] = (yyvsp[0].dl);
  (yyval.df).nlines_inuse = (yyval.df).nlines_inuse + 1;
}
#line 1492 "readclofparse.c"
    break;

  case 37: /* debuglines: %empty  */
#line 160 "readclofparse.y"
  {(yyval.df).nlines = (yyval.df).nlines_inuse = 0; (yyval.df).flines = NULL; }
#line 1498 "readclofparse.c"
    break;

  case 38: /* debugline: T_L T_LINE T_OFFSET '=' oq T_NUMBER oq T_LINENO '=' oq T_NUMBER oq T_SG  */
#line 164 "readclofparse.y"
{ (yyval.dl).offset = (yyvsp[-7].i); (yyval.dl).line = (yyvsp[-2].i); }
#line 1504 "readclofparse.c"
    break;


#line 1508 "readclofparse.c"

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

#line 169 "readclofparse.y"


static void new_segment(struct Segment *s)
{  
  struct SegmentTable *st = &modules[current_module].st; 
  if (st->size <= s->id) {
    st->segments = safe_realloc(st->segments, 
				(s->id + 1) * sizeof(struct Segment));
    st->size = s->id + 1;
  }
  s->in_use = 1;
  st->segments[s->id] = *s;
  st->segments[s->id].module = current_module;
}

static void new_symbol(struct Label *s)
{
  struct LabelTable *lt = &modules[current_module].lt;
  if (lt->size <= s->id) {
    lt->labels = safe_realloc(lt->labels, 
			      (s->id + 1) * sizeof(struct Label));
    lt->size = s->id + 1;
  }
  s->in_use = 1;
  lt->labels[s->id] = *s;
}

void yyerror(char *s)
{
  fprintf(stderr, "%s:%d: %s\n", source[current_module], line_no, s);
}

int yywrap(void)
{
  return 1;
}
