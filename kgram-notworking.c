/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "kgram.y"

# include <stdio.h>
# include "ansi_parse.h"
# include "lif.h"
	static	int     set_typedef = 0;
	//static  char    sccsid[] = "@(#)kgram.y	1.7  10/26/94";
	static	char	*parse_h_sccs_id = PARSE_H_SCCS_ID;
	static	char	*lif_h_sccs_id = LIF_H_SCCS_ID;
	
extern int yyerror(char *msg);

extern void connect_nodes (int from, int to);
extern void close_scope(void);
extern void open_scope(void);
extern void end_local_decl(void);
extern void start_local_decl(void);
extern void clear_type_flag(void);
extern void do_returns (int end_stmt);
extern void source_map (int n, token_ptr from, token_ptr to);
extern int yylex(void);
extern void merge_abstract_type (type_ptr old, type_ptr new);
extern void start_local_decls(void);
extern void end_locaal_decls(void);
extern void start_typedef(void);
extern void end_typedef(void);
extern void end_init(void);
extern void gen_require (int node_required, int from, int to);
extern void make_decl (token_ptr token, unsigned int flag);
void print_decl (token_ptr type_spec, token_ptr var_specs);
extern void insert_var_decl (token_ptr var);
extern void merge_abstract_type (type_ptr old, type_ptr new);
extern void decl (token_ptr type_desc, token_ptr var_list, int with_members);
extern void modify_type (token_ptr token, unsigned int flag, int style, token_ptr formals);


#line 106 "kgram-notworking.c"

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

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif


/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    IDENTIFIER = 258,
    CONSTANT = 259,
    STRING_LITERAL = 260,
    SIZEOF = 261,
    PTR_OP = 262,
    INC_OP = 263,
    DEC_OP = 264,
    LEFT_OP = 265,
    RIGHT_OP = 266,
    LE_OP = 267,
    GE_OP = 268,
    EQ_OP = 269,
    NE_OP = 270,
    AND_OP = 271,
    OR_OP = 272,
    MUL_ASSIGN = 273,
    DIV_ASSIGN = 274,
    MOD_ASSIGN = 275,
    ADD_ASSIGN = 276,
    SUB_ASSIGN = 277,
    LEFT_ASSIGN = 278,
    RIGHT_ASSIGN = 279,
    AND_ASSIGN = 280,
    XOR_ASSIGN = 281,
    OR_ASSIGN = 282,
    TYPE_NAME = 283,
    TYPEDEF = 284,
    EXTERN = 285,
    STATIC = 286,
    AUTO = 287,
    REGISTER = 288,
    CHAR = 289,
    SHORT = 290,
    INT = 291,
    LONG = 292,
    SIGNED = 293,
    UNSIGNED = 294,
    FLOAT = 295,
    DOUBLE = 296,
    CONST = 297,
    VOLATILE = 298,
    VOID = 299,
    STRUCT = 300,
    UNION = 301,
    ENUM = 302,
    ELIPSIS = 303,
    RANGE = 304,
    CASE = 305,
    DEFAULT = 306,
    IF = 307,
    ELSE = 308,
    SWITCH = 309,
    WHILE = 310,
    DO = 311,
    FOR = 312,
    GOTO = 313,
    CONTINUE = 314,
    BREAK = 315,
    RETURN = 316,
    LOWER_THAN_ELSE = 317
  };
#endif
/* Tokens.  */
#define IDENTIFIER 258
#define CONSTANT 259
#define STRING_LITERAL 260
#define SIZEOF 261
#define PTR_OP 262
#define INC_OP 263
#define DEC_OP 264
#define LEFT_OP 265
#define RIGHT_OP 266
#define LE_OP 267
#define GE_OP 268
#define EQ_OP 269
#define NE_OP 270
#define AND_OP 271
#define OR_OP 272
#define MUL_ASSIGN 273
#define DIV_ASSIGN 274
#define MOD_ASSIGN 275
#define ADD_ASSIGN 276
#define SUB_ASSIGN 277
#define LEFT_ASSIGN 278
#define RIGHT_ASSIGN 279
#define AND_ASSIGN 280
#define XOR_ASSIGN 281
#define OR_ASSIGN 282
#define TYPE_NAME 283
#define TYPEDEF 284
#define EXTERN 285
#define STATIC 286
#define AUTO 287
#define REGISTER 288
#define CHAR 289
#define SHORT 290
#define INT 291
#define LONG 292
#define SIGNED 293
#define UNSIGNED 294
#define FLOAT 295
#define DOUBLE 296
#define CONST 297
#define VOLATILE 298
#define VOID 299
#define STRUCT 300
#define UNION 301
#define ENUM 302
#define ELIPSIS 303
#define RANGE 304
#define CASE 305
#define DEFAULT 306
#define IF 307
#define ELSE 308
#define SWITCH 309
#define WHILE 310
#define DO 311
#define FOR 312
#define GOTO 313
#define CONTINUE 314
#define BREAK 315
#define RETURN 316
#define LOWER_THAN_ELSE 317

/* Value type.  */


extern YYSTYPE yylval;

int yyparse (void);





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
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
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
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


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
#define YYFINAL  61
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1267

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  87
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  81
/* YYNRULES -- Number of rules.  */
#define YYNRULES  234
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  373

#define YYUNDEFTOK  2
#define YYMAXUTOK   317


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    73,     2,     2,     2,    80,    68,     2,
      63,    64,    69,    71,    78,    72,    77,    79,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    67,    62,
      81,    86,    82,    85,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    75,     2,    76,    83,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    65,    84,    66,    70,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    74
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   108,   108,   110,   112,   114,   123,   124,   128,   129,
     131,   133,   135,   138,   141,   143,   148,   149,   153,   157,
     158,   160,   162,   166,   168,   173,   175,   177,   179,   181,
     183,   188,   189,   207,   208,   210,   212,   217,   218,   220,
     225,   226,   228,   233,   234,   236,   238,   240,   245,   246,
     248,   253,   254,   259,   260,   265,   266,   271,   272,   277,
     278,   283,   284,   291,   292,   297,   299,   301,   303,   305,
     307,   309,   311,   313,   315,   317,   322,   325,   326,   331,
     335,   340,   350,   351,   360,   361,   366,   367,   375,   376,
     397,   398,   403,   407,   409,   411,   413,   418,   419,   420,
     421,   422,   423,   424,   425,   426,   427,   428,   429,   434,
     433,   446,   454,   461,   463,   469,   470,   484,   485,   486,
     498,   515,   519,   526,   527,   548,   549,   551,   555,   557,
     559,   564,   565,   583,   587,   588,   592,   593,   602,   605,
     606,   607,   609,   611,   614,   616,   621,   623,   625,   627,
     632,   633,   636,   637,   659,   666,   667,   681,   682,   686,
     687,   701,   715,   716,   724,   725,   732,   744,   745,   762,
     764,   766,   768,   776,   784,   786,   788,   789,   793,   795,
     797,   802,   803,   807,   808,   809,   810,   811,   812,   816,
     818,   820,   825,   833,   840,   842,   844,   849,   857,   858,
     868,   869,   874,   882,   893,   895,   897,   901,   902,   905,
     907,   909,   924,   928,   930,   932,   934,   938,   939,   945,
     946,   950,   951,   952,   960,   990,   990,   991,   991,  1014,
    1013,  1047,  1050,  1049,  1096
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IDENTIFIER", "CONSTANT",
  "STRING_LITERAL", "SIZEOF", "PTR_OP", "INC_OP", "DEC_OP", "LEFT_OP",
  "RIGHT_OP", "LE_OP", "GE_OP", "EQ_OP", "NE_OP", "AND_OP", "OR_OP",
  "MUL_ASSIGN", "DIV_ASSIGN", "MOD_ASSIGN", "ADD_ASSIGN", "SUB_ASSIGN",
  "LEFT_ASSIGN", "RIGHT_ASSIGN", "AND_ASSIGN", "XOR_ASSIGN", "OR_ASSIGN",
  "TYPE_NAME", "TYPEDEF", "EXTERN", "STATIC", "AUTO", "REGISTER", "CHAR",
  "SHORT", "INT", "LONG", "SIGNED", "UNSIGNED", "FLOAT", "DOUBLE", "CONST",
  "VOLATILE", "VOID", "STRUCT", "UNION", "ENUM", "ELIPSIS", "RANGE",
  "CASE", "DEFAULT", "IF", "ELSE", "SWITCH", "WHILE", "DO", "FOR", "GOTO",
  "CONTINUE", "BREAK", "RETURN", "';'", "'('", "')'", "'{'", "'}'", "':'",
  "'&'", "'*'", "'~'", "'+'", "'-'", "'!'", "LOWER_THAN_ELSE", "'['",
  "']'", "'.'", "','", "'/'", "'%'", "'<'", "'>'", "'^'", "'|'", "'?'",
  "'='", "$accept", "primary_expr", "string_literal_list", "postfix_expr",
  "argument_expr_list", "comma_comming", "unary_expr", "unary_operator",
  "cast_expr", "multiplicative_expr", "additive_expr", "shift_expr",
  "relational_expr", "equality_expr", "and_expr", "exclusive_or_expr",
  "inclusive_or_expr", "logical_and_expr", "logical_or_expr",
  "conditional_expr", "assignment_expr", "assignment_operator", "expr",
  "exprXX", "constant_expr", "declaration", "declaration_specifiers",
  "init_declarator_list", "init_declarator", "storage_class_specifier",
  "type_specifier", "struct_or_union_specifier", "$@1", "struct_or_union",
  "struct_declaration_list", "specifier_qual_list", "struct_declaration",
  "struct_declarator_list", "struct_declarator", "enum_specifier",
  "enumerator_list", "enumerator", "declarator", "parms_next",
  "declarator2", "pointer", "type_qualifier", "type_qualifier_list",
  "parameter_identifier_list", "identifier_list", "parameter_type_list",
  "parameter_list", "parameter_declaration", "type_name",
  "abstract_declarator", "abstract_declarator2", "initializer",
  "initializer_list", "statement", "labeled_statement", "decl_end",
  "decl_start", "compound_statement", "declaration_list", "statement_list",
  "expression_statement", "selection_statement", "oexpr",
  "iteration_statement", "jump_statement", "program", "file",
  "external_definition", "function_start", "function_definition", "$@2",
  "$@3", "$@4", "function_body", "@5", "identifier", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,    59,    40,    41,   123,   125,    58,    38,    42,
     126,    43,    45,    33,   317,    91,    93,    46,    44,    47,
      37,    60,    62,    94,   124,    63,    61
};
# endif

#define YYPACT_NINF (-227)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-226)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     899,  -227,  -227,  -227,  -227,  -227,  -227,  -227,  -227,  -227,
    -227,  -227,  -227,  -227,  -227,  -227,  -227,  -227,  -227,  -227,
    -227,     4,  -227,    61,    93,  -227,    60,  1200,  1200,  -227,
       8,  -227,   -52,   -14,    24,  1200,    46,   899,  -227,  -227,
    -227,    45,   -10,    -7,  -227,  -227,    93,  -227,   -19,  -227,
     -18,  -227,  -227,  1220,    26,  -227,  -227,   353,    25,   -14,
    -227,  -227,  -227,   103,  -227,    10,    45,  -227,  -227,  -227,
    -227,    61,   745,  -227,  -227,   828,   607,  -227,  -227,    48,
      58,  1104,  -227,  -227,   430,   838,   838,   486,  -227,  -227,
    -227,  -227,  -227,  -227,  -227,  -227,   138,    95,  -227,   849,
    -227,    94,   108,   195,    12,   197,    82,    63,   129,   155,
      -2,  -227,   132,  -227,   944,  -227,    45,   849,   136,  -227,
     130,   745,   229,  -227,  -227,  -227,  1104,  -227,  -227,  -227,
     849,  -227,    23,  -227,   162,  -227,  1220,  -227,   531,  -227,
      60,  -227,  1200,  -227,   486,  -227,   849,  -227,  -227,  -227,
     153,   170,  1023,   173,  -227,    45,  -227,  -227,   756,   849,
      45,  -227,   849,   849,   849,   849,   849,   849,   849,   849,
     849,   849,   849,   849,   849,   849,   849,   849,   849,   849,
     849,  -227,  -227,     9,   175,   165,   199,   183,  -227,  -227,
    -227,  -227,  -227,  -227,   141,  -227,  -227,  -227,  -227,  -227,
    -227,  -227,  -227,  -227,  -227,  -227,   849,  -227,  -227,  -227,
      64,   849,  1065,   849,   200,   208,   209,   210,   723,   214,
      45,   219,   223,   827,  -227,  -227,    27,  -227,  -227,  -227,
    1200,  -227,   557,  -227,  -227,  -227,  -227,   211,  -227,   225,
     224,   849,  -227,   981,   377,   152,  -227,   163,   849,  -227,
    -227,   233,  -227,   226,  -227,  -227,  -227,  -227,    94,    94,
     108,   108,   195,   195,   195,   195,    12,    12,   197,    82,
      63,   129,   155,    31,   441,  -227,    11,  -227,  -227,    45,
    -227,  1179,  -227,   734,  -227,  -227,  -227,  -227,   232,   723,
     849,   849,   849,   245,   849,   244,  -227,  -227,  -227,    70,
    -227,   628,  1200,  -227,  -227,   723,  -227,  -227,  -227,  -227,
     243,   246,  -227,   235,   163,   392,   249,  -227,  -227,   230,
    -227,   849,  -227,  -227,  -227,  -227,  -227,   723,  -227,   -22,
      78,    87,   250,   153,   247,  -227,  -227,  -227,   652,  -227,
    -227,  -227,  -227,  -227,   238,  1142,   849,  -227,  -227,   723,
     723,   723,   849,   849,  -227,  -227,  -227,   252,  -227,   265,
    -227,  -227,    89,   257,  -227,   723,   258,   849,  -227,  -227,
     259,   723,  -227
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
     217,   234,   108,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   150,   151,   105,   113,
     114,     0,   223,     0,   146,   222,     0,    82,    86,   106,
       0,   107,   227,   136,     0,    84,     0,   218,   219,   221,
     139,     0,   130,     0,   148,   152,   147,    80,     0,    88,
     229,    83,    87,     0,   112,   224,   224,     0,     0,   137,
      85,     1,   220,     0,   131,   134,     0,   140,   149,   153,
      81,     0,     0,   224,   117,     0,     0,   115,   118,     0,
       0,     0,     3,     6,     0,     0,     0,     0,    25,    26,
      29,    27,    28,    30,   141,     8,     4,    19,    31,     0,
      33,    37,    40,    43,    48,    51,    53,    55,    57,    59,
      61,    79,     0,     2,     0,   128,   133,     0,     0,    89,
      90,     0,    31,    63,   178,    91,     0,   111,   116,   122,
       0,   120,     0,   123,   125,   119,     0,   226,   193,   198,
       0,   231,   232,   228,     0,    23,     0,    20,    21,    77,
       0,    76,   164,     0,     7,     0,    14,    15,     0,     0,
       0,    22,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   142,   143,   163,     0,   154,     0,   157,   159,   155,
     132,   135,   129,   181,     0,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    65,     0,   230,   126,   121,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   202,   194,     0,    76,   200,   183,
       0,   184,     0,   185,   186,   187,   188,     2,   199,     0,
       0,     0,     5,     0,     0,   166,   165,   167,     0,    13,
      10,    18,    16,    76,    12,    34,    35,    36,    38,    39,
      41,    42,    46,    47,    44,    45,    49,    50,    52,    54,
      56,    58,    60,     0,     0,   161,   166,   162,   145,     0,
     144,     0,   179,     0,    64,   124,   127,   110,     0,     0,
       0,     0,     0,     0,   207,     0,   213,   214,   215,     0,
     203,     0,   192,   195,   201,     0,   233,    24,    78,   174,
       0,     0,   170,     0,   168,     0,     0,    32,    11,     0,
       9,     0,   156,   158,   160,   180,   182,     0,   191,     0,
       0,     0,     0,   208,     0,   212,   216,   196,     0,   189,
     175,   169,   171,   172,     0,     0,     0,    62,   190,     0,
       0,     0,     0,   207,   197,   173,   176,     0,    17,   204,
     206,   209,     0,     0,   177,     0,     0,   207,   205,   210,
       0,     0,   211
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -227,  -227,  -227,  -227,  -227,  -227,    22,  -227,   -45,    76,
      90,   124,    91,   146,   148,   145,   147,   149,  -227,   -35,
     -59,  -227,   -69,   -34,   -85,     2,     1,  -227,   255,  -227,
       5,  -227,  -227,  -227,   193,   -47,   -67,  -227,   120,  -227,
     266,   215,    -6,  -214,   -31,   -15,   216,  -227,  -227,  -227,
    -109,  -227,    53,   191,  -136,  -226,  -117,  -227,    28,  -227,
    -227,  -227,   -75,   106,    38,  -227,  -227,  -192,  -227,  -227,
    -227,  -227,   303,   -21,  -227,  -227,  -227,  -227,   218,  -227,
       0
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    95,    96,    97,   251,   319,   122,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   123,
     149,   206,   226,   227,   112,   139,   140,    48,    49,    27,
      28,    29,    79,    30,    75,    76,    77,   132,   133,    31,
      63,    64,    32,    58,    33,    34,    35,    46,   184,   185,
     310,   187,   188,   153,   311,   247,   125,   194,   228,   229,
     301,   230,   231,   142,   232,   233,   234,   334,   235,   236,
      36,    37,    38,    80,    39,    55,    56,    73,   143,   239,
     113
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      40,    26,    25,    59,   193,   186,   141,     1,   128,    44,
    -225,     1,     1,   124,     1,   179,   246,    43,   150,   314,
      50,    42,   111,    40,   169,   170,    40,     1,    51,    52,
      54,    68,   191,   316,    40,    81,    60,    40,    26,    25,
     152,    65,   349,    70,   -90,   208,    61,   277,     1,  -138,
     314,   141,   126,   151,   161,    66,   241,    67,    74,    71,
     -90,    57,   124,     1,     1,   120,    65,     1,    72,    41,
     134,    40,   274,    53,   274,   150,    40,   150,    24,    98,
      74,   131,   111,   180,   244,   209,   244,    23,   114,   300,
     150,  -109,    74,   171,   172,   111,   117,   152,   321,   252,
     316,   210,   155,   156,   157,   241,   145,   147,   148,   241,
     151,   273,   151,   136,   189,   183,    65,   255,   256,   257,
     137,    98,    47,    23,    23,   253,   286,    23,   288,    24,
      24,   130,   336,    24,   120,    16,    17,   245,   237,    98,
      40,    74,   350,   154,   238,   128,   176,   284,   241,    74,
     175,   351,    98,   366,   299,   249,   241,   131,   158,   313,
     254,   363,    24,   162,   306,   241,   326,   241,   276,   115,
     159,   178,   160,   163,   164,   370,   111,   275,   111,   165,
     166,   116,   308,    40,    98,    98,    98,    98,    98,    98,
      98,    98,    98,    98,    98,    98,    98,    98,    98,    98,
      98,    98,   192,   317,   134,   167,   168,   282,   181,   111,
      40,   173,   174,   177,   116,   243,    72,    74,   237,   283,
     295,   329,   330,   331,   124,   333,  -138,   244,   245,   211,
     344,   241,   237,    98,   242,    98,   357,   248,   315,   278,
      45,   258,   259,   279,   183,    59,   293,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   260,   261,   276,
     304,   281,    69,   280,   266,   267,    98,   289,    43,    78,
      98,   290,   291,   292,    40,   183,    40,   294,   305,   322,
     111,   296,   183,   362,   333,   297,   347,   358,   307,   237,
     138,    78,   135,   262,   263,   264,   265,   318,   333,   327,
     332,   237,   320,    78,   238,   237,   335,   340,   346,   353,
     341,   342,   345,   352,   355,   205,   364,   328,   365,   367,
     369,   268,   270,   371,   269,   271,   119,   237,   272,   212,
     285,   190,   118,   339,   324,   240,   302,    98,   237,   338,
      62,     0,     0,    98,   207,     0,   183,     0,     0,   237,
     237,   237,    78,     0,     0,   348,     1,    82,    83,    84,
      78,    85,    86,     0,     0,   237,   304,     0,   135,     0,
       0,   237,     0,     0,     0,     0,     0,   359,   360,   361,
       1,    82,    83,    84,     0,    85,    86,     0,     0,     0,
       0,     0,     0,   368,     0,     1,    82,    83,    84,   372,
      85,    86,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    87,     0,     0,     0,
       0,    88,    89,    90,    91,    92,    93,     0,    78,    94,
       0,     0,     0,     1,    82,    83,    84,     0,    85,    86,
      87,     0,     0,     0,     1,    88,    89,    90,    91,    92,
      93,     0,     0,   312,     0,    87,     0,     0,     0,     0,
      88,    89,    90,    91,    92,    93,     0,     0,   343,     2,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,     1,
      82,    83,    84,   144,    85,    86,     0,     0,    88,    89,
      90,    91,    92,    93,   274,   309,     0,     0,     0,     0,
      24,     0,     0,     0,     2,     0,   244,     0,     0,     0,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,     1,    82,    83,    84,     0,    85,
      86,     0,     0,     0,     0,     0,     0,     0,     0,    87,
       0,     0,     0,     0,    88,    89,    90,    91,    92,    93,
       1,    82,    83,    84,     0,    85,    86,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   213,   214,   215,     0,   216,   217,   218,   219,   220,
     221,   222,   223,   224,    87,     0,   138,   225,     0,    88,
      89,    90,    91,    92,    93,     0,     0,   213,   214,   215,
       1,   216,   217,   218,   219,   220,   221,   222,   223,   224,
      87,     0,   138,   303,     0,    88,    89,    90,    91,    92,
      93,     1,    82,    83,    84,     2,    85,    86,     0,     0,
       0,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,     1,    82,    83,    84,     0,
      85,    86,     0,     0,     0,     0,     0,     0,     0,   129,
      23,     0,     0,     0,   130,     0,    24,     0,   213,   214,
     215,     0,   216,   217,   218,   219,   220,   221,   222,   223,
     224,    87,     0,   138,   337,     0,    88,    89,    90,    91,
      92,    93,   213,   214,   215,     0,   216,   217,   218,   219,
     220,   221,   222,   223,   224,    87,     0,   138,   354,     0,
      88,    89,    90,    91,    92,    93,     1,    82,    83,    84,
       0,    85,    86,     0,     0,     0,     0,     1,    82,    83,
      84,     0,    85,    86,     0,     0,     0,     0,     1,    82,
      83,    84,     0,    85,    86,     0,     0,     0,     0,     1,
      82,    83,    84,     0,    85,    86,     0,     0,     0,     0,
       0,     0,     0,   213,   214,   215,     0,   216,   217,   218,
     219,   220,   221,   222,   223,   224,    87,     0,   138,     0,
       0,    88,    89,    90,    91,    92,    93,    87,     0,   121,
     325,     0,    88,    89,    90,    91,    92,    93,    87,     0,
     121,     0,     0,    88,    89,    90,    91,    92,    93,    87,
     250,     0,     0,     0,    88,    89,    90,    91,    92,    93,
       1,    82,    83,    84,     0,    85,    86,     0,     0,     0,
       0,     1,    82,    83,    84,     0,    85,    86,     0,     0,
       0,     0,     1,    82,    83,    84,     2,    85,    86,     0,
       0,     0,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   298,
      87,     0,     0,     0,   127,    88,    89,    90,    91,    92,
      93,   146,     1,     0,     0,     0,    88,    89,    90,    91,
      92,    93,    87,     0,     0,     0,     0,    88,    89,    90,
      91,    92,    93,     0,     0,     0,     0,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,     1,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    22,    23,     0,     0,     0,     0,     0,    24,     0,
       0,     0,     2,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   182,     2,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   243,   309,     0,     0,     0,     0,
      24,     2,     0,     0,     0,     0,   244,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   243,     0,     0,     0,
       0,     0,    24,     2,     0,     0,     0,     0,   244,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   287,     2,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   138,
       2,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   356,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,   323,     2,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,     2,     0,
       0,     0,     0,     0,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21
};

static const yytype_int16 yycheck[] =
{
       0,     0,     0,    34,   121,   114,    81,     3,    75,    24,
      62,     3,     3,    72,     3,    17,   152,    23,    87,   245,
      26,    21,    57,    23,    12,    13,    26,     3,    27,    28,
      30,    46,   117,   247,    34,    56,    35,    37,    37,    37,
      87,    41,    64,    62,    62,   130,     0,   183,     3,    63,
     276,   126,    73,    87,    99,    65,    78,    64,    53,    78,
      78,    75,   121,     3,     3,    71,    66,     3,    86,    65,
      76,    71,    63,    65,    63,   144,    76,   146,    69,    57,
      75,    76,   117,    85,    75,    62,    75,    63,    63,    62,
     159,    65,    87,    81,    82,   130,    86,   144,    67,   158,
     314,    78,     7,     8,     9,    78,    84,    85,    86,    78,
     144,   180,   146,    65,   114,   114,   116,   162,   163,   164,
      62,    99,    62,    63,    63,   159,   211,    63,   213,    69,
      69,    67,    62,    69,   140,    42,    43,   152,   138,   117,
     140,   136,    64,     5,   142,   212,    83,   206,    78,   144,
      68,    64,   130,    64,   223,   155,    78,   152,    63,   244,
     160,   353,    69,    69,   239,    78,   283,    78,   183,    66,
      75,    16,    77,    79,    80,   367,   211,   183,   213,    71,
      72,    78,   241,   183,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,    66,   248,   210,    10,    11,    66,    76,   244,
     210,    14,    15,    84,    78,    63,    86,   212,   218,    78,
     220,   290,   291,   292,   283,   294,    63,    75,   243,    67,
     315,    78,   232,   211,    64,   213,   345,    64,    75,    64,
      24,   165,   166,    78,   243,   276,   218,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,   167,   168,   274,
     232,    78,    46,    64,   173,   174,   244,    67,   274,    53,
     248,    63,    63,    63,   274,   274,   276,    63,    67,   279,
     315,    62,   281,   352,   353,    62,   321,   346,    64,   289,
      65,    75,    76,   169,   170,   171,   172,    64,   367,    67,
      55,   301,    76,    87,   302,   305,    62,    64,    78,    62,
      64,    76,    63,    63,    76,    86,    64,   289,    53,    62,
      62,   175,   177,    64,   176,   178,    71,   327,   179,   136,
     210,   116,    66,   305,   281,   144,   230,   315,   338,   301,
      37,    -1,    -1,   321,   126,    -1,   345,    -1,    -1,   349,
     350,   351,   136,    -1,    -1,   327,     3,     4,     5,     6,
     144,     8,     9,    -1,    -1,   365,   338,    -1,   152,    -1,
      -1,   371,    -1,    -1,    -1,    -1,    -1,   349,   350,   351,
       3,     4,     5,     6,    -1,     8,     9,    -1,    -1,    -1,
      -1,    -1,    -1,   365,    -1,     3,     4,     5,     6,   371,
       8,     9,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,
      -1,    68,    69,    70,    71,    72,    73,    -1,   212,    76,
      -1,    -1,    -1,     3,     4,     5,     6,    -1,     8,     9,
      63,    -1,    -1,    -1,     3,    68,    69,    70,    71,    72,
      73,    -1,    -1,    76,    -1,    63,    -1,    -1,    -1,    -1,
      68,    69,    70,    71,    72,    73,    -1,    -1,    76,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,     3,
       4,     5,     6,    63,     8,     9,    -1,    -1,    68,    69,
      70,    71,    72,    73,    63,    64,    -1,    -1,    -1,    -1,
      69,    -1,    -1,    -1,    28,    -1,    75,    -1,    -1,    -1,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,     3,     4,     5,     6,    -1,     8,
       9,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    -1,    -1,    68,    69,    70,    71,    72,    73,
       3,     4,     5,     6,    -1,     8,     9,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    50,    51,    52,    -1,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    -1,    65,    66,    -1,    68,
      69,    70,    71,    72,    73,    -1,    -1,    50,    51,    52,
       3,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    -1,    65,    66,    -1,    68,    69,    70,    71,    72,
      73,     3,     4,     5,     6,    28,     8,     9,    -1,    -1,
      -1,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,     3,     4,     5,     6,    -1,
       8,     9,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    -1,    -1,    -1,    67,    -1,    69,    -1,    50,    51,
      52,    -1,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    -1,    65,    66,    -1,    68,    69,    70,    71,
      72,    73,    50,    51,    52,    -1,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    -1,    65,    66,    -1,
      68,    69,    70,    71,    72,    73,     3,     4,     5,     6,
      -1,     8,     9,    -1,    -1,    -1,    -1,     3,     4,     5,
       6,    -1,     8,     9,    -1,    -1,    -1,    -1,     3,     4,
       5,     6,    -1,     8,     9,    -1,    -1,    -1,    -1,     3,
       4,     5,     6,    -1,     8,     9,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    50,    51,    52,    -1,    54,    55,    56,
      57,    58,    59,    60,    61,    62,    63,    -1,    65,    -1,
      -1,    68,    69,    70,    71,    72,    73,    63,    -1,    65,
      66,    -1,    68,    69,    70,    71,    72,    73,    63,    -1,
      65,    -1,    -1,    68,    69,    70,    71,    72,    73,    63,
      64,    -1,    -1,    -1,    68,    69,    70,    71,    72,    73,
       3,     4,     5,     6,    -1,     8,     9,    -1,    -1,    -1,
      -1,     3,     4,     5,     6,    -1,     8,     9,    -1,    -1,
      -1,    -1,     3,     4,     5,     6,    28,     8,     9,    -1,
      -1,    -1,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    62,
      63,    -1,    -1,    -1,    66,    68,    69,    70,    71,    72,
      73,    63,     3,    -1,    -1,    -1,    68,    69,    70,    71,
      72,    73,    63,    -1,    -1,    -1,    -1,    68,    69,    70,
      71,    72,    73,    -1,    -1,    -1,    -1,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,     3,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    62,    63,    -1,    -1,    -1,    -1,    -1,    69,    -1,
      -1,    -1,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    64,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    63,    64,    -1,    -1,    -1,    -1,
      69,    28,    -1,    -1,    -1,    -1,    75,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    63,    -1,    -1,    -1,
      -1,    -1,    69,    28,    -1,    -1,    -1,    -1,    75,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    64,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    28,    -1,
      -1,    -1,    -1,    -1,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    62,    63,    69,   112,   113,   116,   117,   118,
     120,   126,   129,   131,   132,   133,   157,   158,   159,   161,
     167,    65,   167,   129,   132,   133,   134,    62,   114,   115,
     129,   113,   113,    65,   167,   162,   163,    75,   130,   131,
     113,     0,   159,   127,   128,   167,    65,    64,   132,   133,
      62,    78,    86,   164,   117,   121,   122,   123,   133,   119,
     160,   160,     4,     5,     6,     8,     9,    63,    68,    69,
      70,    71,    72,    73,    76,    88,    89,    90,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   111,   167,    63,    66,    78,    86,   127,   115,
     129,    65,    93,   106,   107,   143,   160,    66,   123,    62,
      67,   117,   124,   125,   129,   133,    65,    62,    65,   112,
     113,   149,   150,   165,    63,    93,    63,    93,    93,   107,
     109,   110,   122,   140,     5,     7,     8,     9,    63,    75,
      77,    95,    69,    79,    80,    71,    72,    10,    11,    12,
      13,    81,    82,    14,    15,    68,    83,    84,    16,    17,
      85,    76,    64,   113,   135,   136,   137,   138,   139,   167,
     128,   111,    66,   143,   144,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    86,   108,   165,   111,    62,
      78,    67,   121,    50,    51,    52,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    66,   109,   110,   145,   146,
     148,   149,   151,   152,   153,   155,   156,   167,   112,   166,
     140,    78,    64,    63,    75,   132,   141,   142,    64,   167,
      64,    91,   107,   110,   167,    95,    95,    95,    96,    96,
      97,    97,    98,    98,    98,    98,    99,    99,   100,   101,
     102,   103,   104,   109,    63,   129,   132,   141,    64,    78,
      64,    78,    66,    78,   107,   125,   111,    66,   111,    67,
      63,    63,    63,   145,    63,   167,    62,    62,    62,   109,
      62,   147,   150,    66,   145,    67,   149,    64,   107,    64,
     137,   141,    76,   111,   142,    75,   130,    95,    64,    92,
      76,    67,   167,    48,   139,    66,   143,    67,   145,   109,
     109,   109,    55,   109,   154,    62,    62,    66,   151,   145,
      64,    64,    76,    76,   111,    63,    78,   106,   145,    64,
      64,    64,    63,    62,    66,    76,    64,   137,   107,   145,
     145,   145,   109,   154,    64,    53,    64,    62,   145,    62,
     154,    64,   145
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    87,    88,    88,    88,    88,    89,    89,    90,    90,
      90,    90,    90,    90,    90,    90,    91,    91,    92,    93,
      93,    93,    93,    93,    93,    94,    94,    94,    94,    94,
      94,    95,    95,    96,    96,    96,    96,    97,    97,    97,
      98,    98,    98,    99,    99,    99,    99,    99,   100,   100,
     100,   101,   101,   102,   102,   103,   103,   104,   104,   105,
     105,   106,   106,   107,   107,   108,   108,   108,   108,   108,
     108,   108,   108,   108,   108,   108,   109,   110,   110,   111,
     112,   112,   113,   113,   113,   113,   113,   113,   114,   114,
     115,   115,   116,   116,   116,   116,   116,   117,   117,   117,
     117,   117,   117,   117,   117,   117,   117,   117,   117,   119,
     118,   118,   118,   120,   120,   121,   121,   122,   122,   122,
     122,   123,   123,   124,   124,   125,   125,   125,   126,   126,
     126,   127,   127,   127,   128,   128,   129,   129,   130,   131,
     131,   131,   131,   131,   131,   131,   132,   132,   132,   132,
     133,   133,   134,   134,   135,   136,   136,   137,   137,   138,
     138,   139,   139,   139,   140,   140,   141,   141,   141,   142,
     142,   142,   142,   142,   142,   142,   142,   142,   143,   143,
     143,   144,   144,   145,   145,   145,   145,   145,   145,   146,
     146,   146,   147,   148,   149,   149,   149,   149,   150,   150,
     151,   151,   152,   152,   153,   153,   153,   154,   154,   155,
     155,   155,   156,   156,   156,   156,   156,   157,   157,   158,
     158,   159,   159,   159,   160,   162,   161,   163,   161,   164,
     161,   165,   166,   165,   167
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     1,     3,     1,     2,     1,     4,
       3,     4,     3,     3,     2,     2,     1,     4,     0,     1,
       2,     2,     2,     2,     4,     1,     1,     1,     1,     1,
       1,     1,     4,     1,     3,     3,     3,     1,     3,     3,
       1,     3,     3,     1,     3,     3,     3,     3,     1,     3,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     5,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     1,
       2,     3,     1,     2,     1,     2,     1,     2,     1,     3,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       6,     4,     2,     1,     1,     1,     2,     1,     1,     2,
       2,     3,     2,     1,     3,     1,     2,     3,     4,     5,
       2,     1,     3,     2,     1,     3,     1,     2,     0,     1,
       3,     3,     4,     4,     5,     5,     1,     2,     2,     3,
       1,     1,     1,     2,     1,     1,     3,     1,     3,     1,
       3,     2,     2,     1,     1,     2,     1,     1,     2,     3,
       2,     3,     3,     4,     2,     3,     4,     5,     1,     3,
       4,     1,     3,     1,     1,     1,     1,     1,     1,     3,
       4,     3,     1,     0,     2,     3,     4,     5,     1,     2,
       1,     2,     1,     2,     5,     7,     5,     0,     1,     5,
       7,     9,     3,     2,     2,     2,     3,     0,     1,     1,
       2,     1,     1,     1,     0,     0,     4,     0,     4,     0,
       5,     1,     0,     3,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


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

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



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

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
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
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
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
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
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


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
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
static char *
yystpcpy (char *yydest, const char *yysrc)
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
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
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
            else
              goto append;

          append:
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

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
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
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
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

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
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
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
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

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
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
  case 2:
#line 109 "kgram.y"
                {(yyval.tree) = make_leaf(ID_OP,(yyvsp[0].token));}
#line 1925 "kgram-notworking.c"
    break;

  case 3:
#line 111 "kgram.y"
                {(yyval.tree) = make_leaf(CON_OP,(yyvsp[0].token));}
#line 1931 "kgram-notworking.c"
    break;

  case 4:
#line 113 "kgram.y"
                {(yyval.tree) = make_leaf(CON_OP,(yyvsp[0].token));}
#line 1937 "kgram-notworking.c"
    break;

  case 5:
#line 115 "kgram.y"
                {
			tree_ptr	t;
			t = make_tree (LEFTP_OP,(yyvsp[-1].tree),(token_ptr)NULL);
			(yyval.tree) = make_tree (RIGHTP_OP,t,(token_ptr)NULL);
			t->token = (yyvsp[-2].token);
			(yyval.tree)->token = (yyvsp[0].token);}
#line 1948 "kgram-notworking.c"
    break;

  case 9:
#line 130 "kgram.y"
                {(yyval.tree) = make_tree (ARRAY_OP,(yyvsp[-3].tree),(yyvsp[-1].tree));}
#line 1954 "kgram-notworking.c"
    break;

  case 10:
#line 132 "kgram.y"
                {(yyval.tree) = make_tree (CALL_OP,(yyvsp[-2].tree),(token_ptr)NULL);}
#line 1960 "kgram-notworking.c"
    break;

  case 11:
#line 134 "kgram.y"
                {(yyval.tree) = make_tree (CALL_OP,(yyvsp[-3].tree),(yyvsp[-1].tree));}
#line 1966 "kgram-notworking.c"
    break;

  case 12:
#line 136 "kgram.y"
                {(yyval.tree) = make_tree (DOT_OP,(yyvsp[-2].tree),
			make_leaf(ID_OP,(yyvsp[0].token)));}
#line 1973 "kgram-notworking.c"
    break;

  case 13:
#line 139 "kgram.y"
                {(yyval.tree) = make_tree (POINTER_OP,(yyvsp[-2].tree),
			make_leaf(ID_OP,(yyvsp[0].token)));}
#line 1980 "kgram-notworking.c"
    break;

  case 14:
#line 142 "kgram.y"
                {(yyval.tree) = make_tree (POST_OP,(yyvsp[-1].tree),(token_ptr)NULL); (yyval.tree)->token = (yyvsp[0].token);}
#line 1986 "kgram-notworking.c"
    break;

  case 15:
#line 144 "kgram.y"
                {(yyval.tree) = make_tree (POST_OP,(yyvsp[-1].tree),(token_ptr)NULL); (yyval.tree)->token = (yyvsp[0].token);}
#line 1992 "kgram-notworking.c"
    break;

  case 17:
#line 150 "kgram.y"
                {(yyval.tree) = make_tree (ARG_OP,(yyvsp[-3].tree),(yyvsp[0].tree));}
#line 1998 "kgram-notworking.c"
    break;

  case 18:
#line 153 "kgram.y"
                        {if(!in_init)type_seen = 0;}
#line 2004 "kgram-notworking.c"
    break;

  case 20:
#line 159 "kgram.y"
                {(yyval.tree) = make_tree (PRE_OP,(yyvsp[0].tree),(token_ptr)NULL);(yyval.tree)->token = (yyvsp[-1].token);}
#line 2010 "kgram-notworking.c"
    break;

  case 21:
#line 161 "kgram.y"
                {(yyval.tree) = make_tree (PRE_OP,(yyvsp[0].tree),(token_ptr)NULL);(yyval.tree)->token = (yyvsp[-1].token);}
#line 2016 "kgram-notworking.c"
    break;

  case 22:
#line 163 "kgram.y"
                {(yyval.tree) = make_tree ((yyvsp[-1].tree)->op_code,(yyvsp[0].tree),(token_ptr)NULL);
		if ((yyvsp[-1].tree)->op_code == DEREF_OP)
			(yyval.tree)->token = (yyvsp[-1].tree)->token;}
#line 2024 "kgram-notworking.c"
    break;

  case 23:
#line 167 "kgram.y"
                {(yyval.tree) = make_tree (UN_OP,(yyvsp[0].tree),(token_ptr)NULL);}
#line 2030 "kgram-notworking.c"
    break;

  case 24:
#line 169 "kgram.y"
                {(yyval.tree) = make_leaf(CON_OP,(yyvsp[-3].token));}
#line 2036 "kgram-notworking.c"
    break;

  case 25:
#line 174 "kgram.y"
                {(yyval.tree) = make_leaf(ADDR_OP,(yyvsp[0].token));}
#line 2042 "kgram-notworking.c"
    break;

  case 26:
#line 176 "kgram.y"
                {(yyval.tree) = make_leaf(DEREF_OP,(yyvsp[0].token));}
#line 2048 "kgram-notworking.c"
    break;

  case 27:
#line 178 "kgram.y"
                {(yyval.tree) = make_leaf(UN_OP,(yyvsp[0].token));}
#line 2054 "kgram-notworking.c"
    break;

  case 28:
#line 180 "kgram.y"
                {(yyval.tree) = make_leaf(UN_OP,(yyvsp[0].token));}
#line 2060 "kgram-notworking.c"
    break;

  case 29:
#line 182 "kgram.y"
                {(yyval.tree) = make_leaf(UN_OP,(yyvsp[0].token));}
#line 2066 "kgram-notworking.c"
    break;

  case 30:
#line 184 "kgram.y"
                {(yyval.tree) = make_leaf(UN_OP,(yyvsp[0].token));}
#line 2072 "kgram-notworking.c"
    break;

  case 32:
#line 190 "kgram.y"
                {
			/*
			if ($2){if($2->desc.decl->decl_flags & FLAG_TYPE_USER_NAME)
					$$ = make_tree (CAST_OP,
						make_leaf(ID_OP,$2),$4);
				else 
					$$ = make_tree (CAST_OP,(token_ptr)NULL,$4);
			}
			else 
				$$ = make_tree (CAST_OP,(token_ptr)NULL,$4);
			*/
			(yyval.tree) = make_tree (CAST_OP,
						make_leaf(TYPENAME_OP,(yyvsp[-2].token)),(yyvsp[0].tree));
		}
#line 2091 "kgram-notworking.c"
    break;

  case 34:
#line 209 "kgram.y"
                {(yyval.tree) = make_tree (BIN_OP,(yyvsp[-2].tree),(yyvsp[0].tree));}
#line 2097 "kgram-notworking.c"
    break;

  case 35:
#line 211 "kgram.y"
                {(yyval.tree) = make_tree (BIN_OP,(yyvsp[-2].tree),(yyvsp[0].tree));}
#line 2103 "kgram-notworking.c"
    break;

  case 36:
#line 213 "kgram.y"
                {(yyval.tree) = make_tree (BIN_OP,(yyvsp[-2].tree),(yyvsp[0].tree));}
#line 2109 "kgram-notworking.c"
    break;

  case 38:
#line 219 "kgram.y"
                {(yyval.tree) = make_tree (BIN_OP,(yyvsp[-2].tree),(yyvsp[0].tree));}
#line 2115 "kgram-notworking.c"
    break;

  case 39:
#line 221 "kgram.y"
                {(yyval.tree) = make_tree (BIN_OP,(yyvsp[-2].tree),(yyvsp[0].tree));}
#line 2121 "kgram-notworking.c"
    break;

  case 41:
#line 227 "kgram.y"
                {(yyval.tree) = make_tree (BIN_OP,(yyvsp[-2].tree),(yyvsp[0].tree));}
#line 2127 "kgram-notworking.c"
    break;

  case 42:
#line 229 "kgram.y"
                {(yyval.tree) = make_tree (BIN_OP,(yyvsp[-2].tree),(yyvsp[0].tree));}
#line 2133 "kgram-notworking.c"
    break;

  case 44:
#line 235 "kgram.y"
                {(yyval.tree) = make_tree (BIN_OP,(yyvsp[-2].tree),(yyvsp[0].tree));}
#line 2139 "kgram-notworking.c"
    break;

  case 45:
#line 237 "kgram.y"
                {(yyval.tree) = make_tree (BIN_OP,(yyvsp[-2].tree),(yyvsp[0].tree));}
#line 2145 "kgram-notworking.c"
    break;

  case 46:
#line 239 "kgram.y"
                {(yyval.tree) = make_tree (BIN_OP,(yyvsp[-2].tree),(yyvsp[0].tree));}
#line 2151 "kgram-notworking.c"
    break;

  case 47:
#line 241 "kgram.y"
                {(yyval.tree) = make_tree (BIN_OP,(yyvsp[-2].tree),(yyvsp[0].tree));}
#line 2157 "kgram-notworking.c"
    break;

  case 49:
#line 247 "kgram.y"
                {(yyval.tree) = make_tree (BIN_OP,(yyvsp[-2].tree),(yyvsp[0].tree));}
#line 2163 "kgram-notworking.c"
    break;

  case 50:
#line 249 "kgram.y"
                {(yyval.tree) = make_tree (BIN_OP,(yyvsp[-2].tree),(yyvsp[0].tree));}
#line 2169 "kgram-notworking.c"
    break;

  case 52:
#line 255 "kgram.y"
                {(yyval.tree) = make_tree (BIN_OP,(yyvsp[-2].tree),(yyvsp[0].tree));}
#line 2175 "kgram-notworking.c"
    break;

  case 54:
#line 261 "kgram.y"
                {(yyval.tree) = make_tree (BIN_OP,(yyvsp[-2].tree),(yyvsp[0].tree));}
#line 2181 "kgram-notworking.c"
    break;

  case 56:
#line 267 "kgram.y"
                {(yyval.tree) = make_tree (BIN_OP,(yyvsp[-2].tree),(yyvsp[0].tree));}
#line 2187 "kgram-notworking.c"
    break;

  case 58:
#line 273 "kgram.y"
                {(yyval.tree) = make_tree (BIN_OP,(yyvsp[-2].tree),(yyvsp[0].tree));}
#line 2193 "kgram-notworking.c"
    break;

  case 60:
#line 279 "kgram.y"
                {(yyval.tree) = make_tree (BIN_OP,(yyvsp[-2].tree),(yyvsp[0].tree));}
#line 2199 "kgram-notworking.c"
    break;

  case 62:
#line 285 "kgram.y"
                {(yyval.tree) = make_tree (BIN_OP,(yyvsp[-4].tree),
			make_tree (BIN_OP,(yyvsp[-2].tree),(yyvsp[0].tree)));
		}
#line 2207 "kgram-notworking.c"
    break;

  case 64:
#line 293 "kgram.y"
                {(yyval.tree) = make_tree (((yyvsp[-1].op) == 0)?RELASGN_OP:ASGN_OP,(yyvsp[-2].tree),(yyvsp[0].tree));}
#line 2213 "kgram-notworking.c"
    break;

  case 65:
#line 298 "kgram.y"
                { (yyval.op) = 1;}
#line 2219 "kgram-notworking.c"
    break;

  case 66:
#line 300 "kgram.y"
                { (yyval.op) = 0;}
#line 2225 "kgram-notworking.c"
    break;

  case 67:
#line 302 "kgram.y"
                { (yyval.op) = 0;}
#line 2231 "kgram-notworking.c"
    break;

  case 68:
#line 304 "kgram.y"
                { (yyval.op) = 0;}
#line 2237 "kgram-notworking.c"
    break;

  case 69:
#line 306 "kgram.y"
                { (yyval.op) = 0;}
#line 2243 "kgram-notworking.c"
    break;

  case 70:
#line 308 "kgram.y"
                { (yyval.op) = 0;}
#line 2249 "kgram-notworking.c"
    break;

  case 71:
#line 310 "kgram.y"
                { (yyval.op) = 0;}
#line 2255 "kgram-notworking.c"
    break;

  case 72:
#line 312 "kgram.y"
                { (yyval.op) = 0;}
#line 2261 "kgram-notworking.c"
    break;

  case 73:
#line 314 "kgram.y"
                { (yyval.op) = 0;}
#line 2267 "kgram-notworking.c"
    break;

  case 74:
#line 316 "kgram.y"
                { (yyval.op) = 0;}
#line 2273 "kgram-notworking.c"
    break;

  case 75:
#line 318 "kgram.y"
                { (yyval.op) = 0;}
#line 2279 "kgram-notworking.c"
    break;

  case 78:
#line 327 "kgram.y"
                {(yyval.tree) = make_tree (COMMA_OP,(yyvsp[-2].tree),(yyvsp[0].tree));}
#line 2285 "kgram-notworking.c"
    break;

  case 80:
#line 336 "kgram.y"
                {
			decl ((yyvsp[-1].token),(token_ptr)NULL,False);
			end_typedef();
		}
#line 2294 "kgram-notworking.c"
    break;

  case 81:
#line 341 "kgram.y"
                {
			decl ((yyvsp[-2].token),(yyvsp[-1].token),False);
			if(c_opt)print_decl ((yyvsp[-2].token),(yyvsp[-1].token));
			(yyvsp[-2].token)->desc.decl->vars = (yyvsp[-1].token);
			end_typedef();
		}
#line 2305 "kgram-notworking.c"
    break;

  case 83:
#line 352 "kgram.y"
                {(yyvsp[-1].token)->desc.decl->decl_flags |=
			(yyvsp[0].token)->desc.decl->decl_flags ;
		(yyvsp[-1].token)->desc.decl->tag = (yyvsp[0].token)->desc.decl->tag;
		(yyvsp[-1].token)->desc.decl->vars = (yyvsp[0].token)->desc.decl->vars;
		(yyvsp[-1].token)->desc.decl->members = (yyvsp[0].token)->desc.decl->members;
		(yyvsp[-1].token)->desc.decl->member_ste = (yyvsp[0].token)->desc.decl->member_ste;
		if (!(yyvsp[-1].token)->text) (yyvsp[-1].token)->text = (yyvsp[0].token)->text;
		}
#line 2318 "kgram-notworking.c"
    break;

  case 85:
#line 362 "kgram.y"
                {if ((yyvsp[-1].token) && (yyvsp[0].token))(yyvsp[0].token)->desc.decl->decl_flags |=
				(yyvsp[-1].token)->desc.decl->decl_flags ;
			(yyval.token) = (yyvsp[0].token);
		}
#line 2327 "kgram-notworking.c"
    break;

  case 87:
#line 368 "kgram.y"
                {(yyvsp[-1].token)->desc.decl->decl_flags |=
			(yyvsp[0].token)->desc.decl->decl_flags ;
		if (!(yyvsp[-1].token)->text) (yyvsp[-1].token)->text = (yyvsp[0].token)->text;
		}
#line 2336 "kgram-notworking.c"
    break;

  case 89:
#line 377 "kgram.y"
                {
			token_ptr	t;
/*
**********************************************************
*                                                        *
*  Collect list of declarators                           *
*                                                        *
**********************************************************
*/

			if ((yyvsp[-2].token)){
				t = (yyvsp[-2].token);
				while (t->next) t = t->next;
				t->next = (yyvsp[0].token);
			}
			else (yyval.token) = (yyvsp[0].token);
		}
#line 2358 "kgram-notworking.c"
    break;

  case 91:
#line 399 "kgram.y"
                { end_init();}
#line 2364 "kgram-notworking.c"
    break;

  case 92:
#line 404 "kgram.y"
                {make_decl ((yyvsp[0].token), FLAG_SC_TYPEDEF);
			start_typedef();
			set_typedef = True;}
#line 2372 "kgram-notworking.c"
    break;

  case 93:
#line 408 "kgram.y"
                {make_decl ((yyvsp[0].token), FLAG_SC_EXTERN);}
#line 2378 "kgram-notworking.c"
    break;

  case 94:
#line 410 "kgram.y"
                {make_decl ((yyvsp[0].token), FLAG_SC_STATIC);}
#line 2384 "kgram-notworking.c"
    break;

  case 95:
#line 412 "kgram.y"
                {make_decl ((yyvsp[0].token), FLAG_SC_AUTO);}
#line 2390 "kgram-notworking.c"
    break;

  case 96:
#line 414 "kgram.y"
                {make_decl ((yyvsp[0].token), FLAG_SC_REGISTER);}
#line 2396 "kgram-notworking.c"
    break;

  case 97:
#line 418 "kgram.y"
               {make_decl ((yyvsp[0].token), FLAG_TYPE_CHAR);}
#line 2402 "kgram-notworking.c"
    break;

  case 98:
#line 419 "kgram.y"
                {make_decl ((yyvsp[0].token), FLAG_TYPE_SHORT);}
#line 2408 "kgram-notworking.c"
    break;

  case 99:
#line 420 "kgram.y"
              {make_decl ((yyvsp[0].token), FLAG_TYPE_INT);}
#line 2414 "kgram-notworking.c"
    break;

  case 100:
#line 421 "kgram.y"
               {make_decl ((yyvsp[0].token), FLAG_TYPE_LONG);}
#line 2420 "kgram-notworking.c"
    break;

  case 101:
#line 422 "kgram.y"
                 {make_decl ((yyvsp[0].token), FLAG_TYPE_SIGNED);}
#line 2426 "kgram-notworking.c"
    break;

  case 102:
#line 423 "kgram.y"
                   {make_decl ((yyvsp[0].token), FLAG_TYPE_UNSIGNED);}
#line 2432 "kgram-notworking.c"
    break;

  case 103:
#line 424 "kgram.y"
                {make_decl ((yyvsp[0].token), FLAG_TYPE_FLOAT);}
#line 2438 "kgram-notworking.c"
    break;

  case 104:
#line 425 "kgram.y"
                 {make_decl ((yyvsp[0].token), FLAG_TYPE_DOUBLE);}
#line 2444 "kgram-notworking.c"
    break;

  case 105:
#line 426 "kgram.y"
               {make_decl ((yyvsp[0].token), FLAG_TYPE_VOID);}
#line 2450 "kgram-notworking.c"
    break;

  case 108:
#line 429 "kgram.y"
                    {make_decl ((yyvsp[0].token), FLAG_TYPE_USER_NAME);}
#line 2456 "kgram-notworking.c"
    break;

  case 109:
#line 434 "kgram.y"
                {
			(yyvsp[-1].token)->desc.decl->tag = (yyvsp[0].token);
			decl ((yyvsp[-1].token),(token_ptr)NULL,True);
		}
#line 2465 "kgram-notworking.c"
    break;

  case 110:
#line 439 "kgram.y"
                {
			int		offset;

			(yyvsp[-5].token)->desc.decl->member_ste = struct_decl((yyvsp[-1].token),&offset);
			(yyvsp[-5].token)->desc.decl->members = (yyvsp[-1].token);
			(yyvsp[-5].token)->desc.decl->offset = offset;
		}
#line 2477 "kgram-notworking.c"
    break;

  case 111:
#line 447 "kgram.y"
                {
			int		offset;

			(yyvsp[-3].token)->desc.decl->members = (yyvsp[-1].token);
			(yyvsp[-3].token)->desc.decl->member_ste = struct_decl((yyvsp[-1].token),&offset);
			(yyvsp[-3].token)->desc.decl->offset = offset;
		}
#line 2489 "kgram-notworking.c"
    break;

  case 112:
#line 455 "kgram.y"
                {
			(yyvsp[-1].token)->desc.decl->tag = (yyvsp[0].token);
		}
#line 2497 "kgram-notworking.c"
    break;

  case 113:
#line 462 "kgram.y"
                {make_decl ((yyvsp[0].token), FLAG_TYPE_US | FLAG_TYPE_STRUCT); }
#line 2503 "kgram-notworking.c"
    break;

  case 114:
#line 464 "kgram.y"
                {make_decl ((yyvsp[0].token), FLAG_TYPE_US | FLAG_TYPE_UNION); }
#line 2509 "kgram-notworking.c"
    break;

  case 116:
#line 471 "kgram.y"
                {
			token_ptr	t;

			t = (yyvsp[-1].token);
			if (t){
				while (t->next) t = t->next;
				t->next = (yyvsp[0].token);
			}
			else (yyval.token) = (yyvsp[0].token);
		}
#line 2524 "kgram-notworking.c"
    break;

  case 119:
#line 487 "kgram.y"
                {
			(yyvsp[-1].token)->desc.decl->decl_flags |= (yyvsp[0].token)->desc.decl->decl_flags;
			if ((yyvsp[-1].token)->desc.decl->tag == NULL)
				(yyvsp[-1].token)->desc.decl->tag = (yyvsp[0].token)->desc.decl->tag;
			if ((yyvsp[-1].token)->desc.decl->vars == NULL)
				(yyvsp[-1].token)->desc.decl->vars = (yyvsp[0].token)->desc.decl->vars;
			if ((yyvsp[-1].token)->desc.decl->members == NULL)
				(yyvsp[-1].token)->desc.decl->members = (yyvsp[0].token)->desc.decl->members;
			if ((yyvsp[-1].token)->desc.decl->member_ste == NULL)
				(yyvsp[-1].token)->desc.decl->member_ste = (yyvsp[0].token)->desc.decl->member_ste;
		}
#line 2540 "kgram-notworking.c"
    break;

  case 120:
#line 499 "kgram.y"
                {
			(yyvsp[-1].token)->desc.decl->decl_flags |= (yyvsp[0].token)->desc.decl->decl_flags;
			if ((yyvsp[-1].token)->desc.decl->tag == NULL)
				(yyvsp[-1].token)->desc.decl->tag = (yyvsp[0].token)->desc.decl->tag;
			if ((yyvsp[-1].token)->desc.decl->vars == NULL)
				(yyvsp[-1].token)->desc.decl->vars = (yyvsp[0].token)->desc.decl->vars;
			if ((yyvsp[-1].token)->desc.decl->members == NULL)
				(yyvsp[-1].token)->desc.decl->members = (yyvsp[0].token)->desc.decl->members;
			if ((yyvsp[-1].token)->desc.decl->member_ste == NULL)
				(yyvsp[-1].token)->desc.decl->member_ste = (yyvsp[0].token)->desc.decl->member_ste;
		}
#line 2556 "kgram-notworking.c"
    break;

  case 121:
#line 516 "kgram.y"
                {
			(yyvsp[-2].token)->desc.decl->vars = (yyvsp[-1].token);
		}
#line 2564 "kgram-notworking.c"
    break;

  case 122:
#line 520 "kgram.y"
                {
			(yyvsp[-1].token)->desc.decl->vars = NULL;
		}
#line 2572 "kgram-notworking.c"
    break;

  case 124:
#line 528 "kgram.y"
                {
			token_ptr	t;
/*
**********************************************************
*                                                        *
*  Collect list of declarators                           *
*                                                        *
**********************************************************
*/

			if ((yyvsp[-2].token)){
				t = (yyvsp[-2].token);
				while (t->next) t = t->next;
				t->next = (yyvsp[0].token);
			}
			else (yyval.token) = (yyvsp[0].token);
		}
#line 2594 "kgram-notworking.c"
    break;

  case 126:
#line 550 "kgram.y"
                {(yyval.token) = NULL;}
#line 2600 "kgram-notworking.c"
    break;

  case 128:
#line 556 "kgram.y"
                { make_decl ((yyvsp[-3].token), FLAG_TYPE_ENUM);(yyvsp[-3].token)->desc.decl->members = (yyvsp[-1].token);}
#line 2606 "kgram-notworking.c"
    break;

  case 129:
#line 558 "kgram.y"
                { make_decl ((yyvsp[-4].token),FLAG_TYPE_ENUM);(yyvsp[-4].token)->desc.decl->members = (yyvsp[-1].token);}
#line 2612 "kgram-notworking.c"
    break;

  case 130:
#line 560 "kgram.y"
                { make_decl ((yyvsp[-1].token),FLAG_TYPE_ENUM); (yyvsp[-1].token)->desc.decl->members = NULL;}
#line 2618 "kgram-notworking.c"
    break;

  case 132:
#line 566 "kgram.y"
                {
			token_ptr	t;
/*
**********************************************************
*                                                        *
*  Collect list of enumerators                           *
*                                                        *
**********************************************************
*/

			if ((yyvsp[-2].token)){
				t = (yyvsp[-2].token);
				while (t->next) t = t->next;
				t->next = (yyvsp[0].token);
			}
			else (yyval.token) = (yyvsp[0].token);
		}
#line 2640 "kgram-notworking.c"
    break;

  case 137:
#line 594 "kgram.y"
                {	int i;
			for (i = 0; i < (int)(yyvsp[-1].token); i++)
				modify_type ((yyvsp[0].token),IS_PTR_TO,NO_STYLE,(token_ptr)NULL);
			(yyval.token) = (yyvsp[0].token);
		}
#line 2650 "kgram-notworking.c"
    break;

  case 138:
#line 602 "kgram.y"
                { /*printf ("PARM(%d)\n",lineno);*/in_parms = 1;}
#line 2656 "kgram-notworking.c"
    break;

  case 140:
#line 606 "kgram.y"
                                {(yyval.token) = (yyvsp[-1].token);}
#line 2662 "kgram-notworking.c"
    break;

  case 141:
#line 608 "kgram.y"
                {modify_type ((yyvsp[-2].token),IS_ARRAY_OF,NO_STYLE,(token_ptr)NULL);}
#line 2668 "kgram-notworking.c"
    break;

  case 142:
#line 610 "kgram.y"
                {modify_type ((yyvsp[-3].token),IS_ARRAY_OF,NO_STYLE,(token_ptr)NULL);}
#line 2674 "kgram-notworking.c"
    break;

  case 143:
#line 612 "kgram.y"
                {modify_type ((yyvsp[-3].token),IS_FUNC_RET,OLD_STYLE_VOID,(token_ptr)NULL);
			in_parms = 0;}
#line 2681 "kgram-notworking.c"
    break;

  case 144:
#line 615 "kgram.y"
                {modify_type ((yyvsp[-4].token),IS_FUNC_RET,NEW_STYLE,(yyvsp[-1].token)); in_parms = 0;}
#line 2687 "kgram-notworking.c"
    break;

  case 145:
#line 617 "kgram.y"
                {modify_type ((yyvsp[-4].token),IS_FUNC_RET,OLD_STYLE,(yyvsp[-1].token)); in_parms = 0;}
#line 2693 "kgram-notworking.c"
    break;

  case 146:
#line 622 "kgram.y"
                        {(yyval.token) = (token_ptr)1;}
#line 2699 "kgram-notworking.c"
    break;

  case 147:
#line 624 "kgram.y"
                        {(yyval.token) = (token_ptr)1;}
#line 2705 "kgram-notworking.c"
    break;

  case 148:
#line 626 "kgram.y"
                        {(yyval.token) = (token_ptr)((int)(yyvsp[0].token) + 1);}
#line 2711 "kgram-notworking.c"
    break;

  case 149:
#line 628 "kgram.y"
                        {(yyval.token) = (token_ptr)((int)(yyvsp[0].token) + 1);}
#line 2717 "kgram-notworking.c"
    break;

  case 150:
#line 632 "kgram.y"
                {make_decl ((yyvsp[0].token), FLAG_TYPE_CONST);}
#line 2723 "kgram-notworking.c"
    break;

  case 151:
#line 633 "kgram.y"
                   {make_decl ((yyvsp[0].token), FLAG_TYPE_VOLATILE);}
#line 2729 "kgram-notworking.c"
    break;

  case 156:
#line 668 "kgram.y"
                {
			token_ptr	t;

			if ((yyvsp[-2].token)){
				t = (yyvsp[-2].token);
				while (t->next) t = t->next;
				t->next = (yyvsp[0].token);
			}
			else (yyval.token) = (yyvsp[0].token);
		}
#line 2744 "kgram-notworking.c"
    break;

  case 160:
#line 688 "kgram.y"
                {
			token_ptr	t;

			if ((yyvsp[-2].token)){
				t = (yyvsp[-2].token);
				while (t->next) t = t->next;
				t->next = (yyvsp[0].token);
			}
			else (yyval.token) = (yyvsp[0].token);
		}
#line 2759 "kgram-notworking.c"
    break;

  case 161:
#line 703 "kgram.y"
                {
	/* : declaration_specifiers declarator */
			(yyvsp[-1].token)->desc.decl->vars = (yyvsp[0].token);
		}
#line 2768 "kgram-notworking.c"
    break;

  case 165:
#line 726 "kgram.y"
                {
			(yyvsp[-1].token)->desc.type_desc = (type_ptr) (yyvsp[0].token);
		}
#line 2776 "kgram-notworking.c"
    break;

  case 166:
#line 733 "kgram.y"
                {	int i;
			type_ptr	old = NULL,new;

			for (i = 0; i < (int)(yyvsp[0].token); i++){
				new = make_abstract_type (
					IS_PTR_TO,NO_STYLE,(token_ptr)NULL);
				if (!old) old = new;
				else merge_abstract_type (old,new);
			}
			(yyval.token) = (token_ptr)old;
		}
#line 2792 "kgram-notworking.c"
    break;

  case 168:
#line 746 "kgram.y"
                {	int i;
			type_ptr	old = NULL,new;

			if ((yyvsp[0].token)) old = (type_ptr)(yyvsp[0].token);

			for (i = 0; i < (int)(yyvsp[-1].token); i++){
				new = make_abstract_type (
					IS_PTR_TO,NO_STYLE,(token_ptr)NULL);
				if (!old) old = new;
				else merge_abstract_type (old,new);
			}
			(yyval.token) = (token_ptr)old;
		}
#line 2810 "kgram-notworking.c"
    break;

  case 169:
#line 763 "kgram.y"
                {(yyval.token) = (yyvsp[-1].token);}
#line 2816 "kgram-notworking.c"
    break;

  case 170:
#line 765 "kgram.y"
                {(yyval.token) = (token_ptr) make_abstract_type (IS_ARRAY_OF,NO_STYLE,NULL);}
#line 2822 "kgram-notworking.c"
    break;

  case 171:
#line 767 "kgram.y"
                {(yyval.token) = (token_ptr) make_abstract_type (IS_ARRAY_OF,NO_STYLE,NULL);}
#line 2828 "kgram-notworking.c"
    break;

  case 172:
#line 769 "kgram.y"
                {type_ptr	new;
			new = make_abstract_type (IS_ARRAY_OF,NO_STYLE,NULL);
			if ((yyvsp[-2].token)){
				merge_abstract_type ((yyvsp[-2].token),new);
			}
			else (yyval.token) = (token_ptr) new;
		}
#line 2840 "kgram-notworking.c"
    break;

  case 173:
#line 777 "kgram.y"
                {type_ptr	new;
			new = make_abstract_type (IS_ARRAY_OF,NO_STYLE,NULL);
			if ((yyvsp[-3].token)){
				merge_abstract_type ((yyvsp[-3].token),new);
			}
			else (yyval.token) = (token_ptr) new;
		}
#line 2852 "kgram-notworking.c"
    break;

  case 174:
#line 785 "kgram.y"
                {(yyval.token) = NULL;}
#line 2858 "kgram-notworking.c"
    break;

  case 175:
#line 787 "kgram.y"
                {(yyval.token) = NULL;}
#line 2864 "kgram-notworking.c"
    break;

  case 178:
#line 794 "kgram.y"
                { (yyval.token) = NULL;}
#line 2870 "kgram-notworking.c"
    break;

  case 179:
#line 796 "kgram.y"
                {(yyval.token) = NULL;}
#line 2876 "kgram-notworking.c"
    break;

  case 180:
#line 798 "kgram.y"
                {(yyval.token) = NULL;}
#line 2882 "kgram-notworking.c"
    break;

  case 189:
#line 817 "kgram.y"
                {(yyval.stmt) = stmt_label((yyvsp[-2].token),(yyvsp[0].stmt));}
#line 2888 "kgram-notworking.c"
    break;

  case 190:
#line 819 "kgram.y"
                {(yyval.stmt) = case_stmt ((yyvsp[-3].token),(yyvsp[-1].token),(yyvsp[0].stmt),False);}
#line 2894 "kgram-notworking.c"
    break;

  case 191:
#line 821 "kgram.y"
                {(yyval.stmt) = case_stmt ((yyvsp[-2].token),(yyvsp[-1].token),(yyvsp[0].stmt),True);}
#line 2900 "kgram-notworking.c"
    break;

  case 192:
#line 826 "kgram.y"
                {
			end_local_decl();
		}
#line 2908 "kgram-notworking.c"
    break;

  case 193:
#line 833 "kgram.y"
                {
			open_scope();
			start_local_decl();
		}
#line 2917 "kgram-notworking.c"
    break;

  case 194:
#line 841 "kgram.y"
                        {(yyval.stmt) = brace_stmt ((yyvsp[-1].token),(token_ptr)NULL,(yyvsp[0].token));}
#line 2923 "kgram-notworking.c"
    break;

  case 195:
#line 843 "kgram.y"
                        {(yyval.stmt) = brace_stmt ((yyvsp[-2].token),(yyvsp[-1].stmt),(yyvsp[0].token));}
#line 2929 "kgram-notworking.c"
    break;

  case 196:
#line 845 "kgram.y"
                        {
				close_scope();
				(yyval.stmt) = brace_stmt ((yyvsp[-3].token),(token_ptr)NULL,(yyvsp[0].token));
			}
#line 2938 "kgram-notworking.c"
    break;

  case 197:
#line 850 "kgram.y"
                        {
				close_scope();
				(yyval.stmt) = brace_stmt ((yyvsp[-4].token),(yyvsp[-1].stmt),(yyvsp[0].token));
			}
#line 2947 "kgram-notworking.c"
    break;

  case 199:
#line 859 "kgram.y"
                {
			if ((yyvsp[-1].token)){
				(yyvsp[-1].token)->next = (yyvsp[0].token);
			}
			else (yyval.token) = (yyvsp[0].token);
		}
#line 2958 "kgram-notworking.c"
    break;

  case 201:
#line 870 "kgram.y"
                { (yyval.stmt) = join_stmt ((yyvsp[-1].stmt),(yyvsp[0].stmt));}
#line 2964 "kgram-notworking.c"
    break;

  case 202:
#line 875 "kgram.y"
                {
			int		node;

			node = get_stmt_no();
			source_map (node,(yyvsp[0].token),(yyvsp[0].token));
			(yyval.stmt) = make_stmt (node,node,node,node);
		}
#line 2976 "kgram-notworking.c"
    break;

  case 203:
#line 883 "kgram.y"
                {int	from_node,to_node;
		int		node_id;

		node_id = xpr_gen ((yyvsp[-1].tree), &from_node,&to_node);
		(yyval.stmt) = make_stmt (node_id,stmtno,from_node,to_node);
		source_map (node_id,leftmost((yyvsp[-1].tree)),(yyvsp[0].token));
		}
#line 2988 "kgram-notworking.c"
    break;

  case 204:
#line 894 "kgram.y"
                {(yyval.stmt) = if_stmt ((yyvsp[-4].token),(yyvsp[-2].tree),(yyvsp[-1].token),(yyvsp[0].stmt),(token_ptr)NULL,(stmt_ptr)NULL);}
#line 2994 "kgram-notworking.c"
    break;

  case 205:
#line 896 "kgram.y"
                {(yyval.stmt) = if_stmt ((yyvsp[-6].token),(yyvsp[-4].tree),(yyvsp[-3].token),(yyvsp[-2].stmt),(yyvsp[-1].token),(yyvsp[0].stmt));}
#line 3000 "kgram-notworking.c"
    break;

  case 206:
#line 898 "kgram.y"
                {(yyval.stmt) = switch_stmt ((yyvsp[-4].token),(yyvsp[-2].tree),(yyvsp[-1].token),(yyvsp[0].stmt));}
#line 3006 "kgram-notworking.c"
    break;

  case 207:
#line 901 "kgram.y"
                {(yyval.tree) = NULL;}
#line 3012 "kgram-notworking.c"
    break;

  case 209:
#line 906 "kgram.y"
                {(yyval.stmt) = while_stmt ((yyvsp[-4].token),(yyvsp[-2].tree),(yyvsp[-1].token),(yyvsp[0].stmt));}
#line 3018 "kgram-notworking.c"
    break;

  case 210:
#line 908 "kgram.y"
                {(yyval.stmt) = do_stmt((yyvsp[-6].token),(yyvsp[-5].stmt),(yyvsp[-4].token),(yyvsp[-2].tree),(yyvsp[0].token));}
#line 3024 "kgram-notworking.c"
    break;

  case 211:
#line 910 "kgram.y"
                {(yyval.stmt) = for_stmt ((yyvsp[-8].token),(yyvsp[-7].token),(yyvsp[-6].tree),(yyvsp[-5].token),(yyvsp[-4].tree),(yyvsp[-3].token),(yyvsp[-2].tree),(yyvsp[-1].token),(yyvsp[0].stmt));}
#line 3030 "kgram-notworking.c"
    break;

  case 212:
#line 925 "kgram.y"
                {
			(yyval.stmt) = goto_stmt ((yyvsp[-2].token),(yyvsp[-1].token),(yyvsp[0].token));
		}
#line 3038 "kgram-notworking.c"
    break;

  case 213:
#line 929 "kgram.y"
                {(yyval.stmt) = continue_stmt ((yyvsp[-1].token),(yyvsp[0].token));}
#line 3044 "kgram-notworking.c"
    break;

  case 214:
#line 931 "kgram.y"
                {(yyval.stmt) = break_stmt ((yyvsp[-1].token),(yyvsp[0].token));}
#line 3050 "kgram-notworking.c"
    break;

  case 215:
#line 933 "kgram.y"
                {(yyval.stmt) = return_stmt ((yyvsp[-1].token),(tree_ptr)NULL,(yyvsp[0].token));}
#line 3056 "kgram-notworking.c"
    break;

  case 216:
#line 935 "kgram.y"
                {(yyval.stmt) = return_stmt ((yyvsp[-2].token),(yyvsp[-1].tree),(yyvsp[0].token));}
#line 3062 "kgram-notworking.c"
    break;

  case 217:
#line 938 "kgram.y"
          {(yyval.token) = NULL;}
#line 3068 "kgram-notworking.c"
    break;

  case 218:
#line 940 "kgram.y"
                {
			close_scope();
		}
#line 3076 "kgram-notworking.c"
    break;

  case 224:
#line 960 "kgram.y"
                {
			token_ptr	tk,parm;
			type_ptr	t;
			
			(yyval.stmt) = fun_stmt ((yyvsp[(-1) - (0)].token));
			open_scope();
			start_local_decls();
			tk = (yyvsp[(-1) - (0)].token);
				/*printf ("Start Fundtion %s",tk->text); */
				t = tk->desc.type_desc;
				while (t){
					/*printf ("(mod %d style %d)",
						t->is,t->style); */
					if ((t->is == IS_FUNC_RET) &&
						(t->style == NEW_STYLE)){
						parm = t->formals;
						while (parm){

							decl (parm, parm->desc.decl->vars,False);
							parm = parm->next;
						}
					}
					t=t->next;
				}
				/*printf ("\n"); */
			clear_type_flag();
		}
#line 3108 "kgram-notworking.c"
    break;

  case 225:
#line 990 "kgram.y"
                     { }
#line 3114 "kgram-notworking.c"
    break;

  case 227:
#line 991 "kgram.y"
                     { }
#line 3120 "kgram-notworking.c"
    break;

  case 228:
#line 992 "kgram.y"
                        {
				int			end_stmt;

				end_stmt = (yyvsp[0].stmt)->exit;
				do_returns (end_stmt);
				connect_nodes ((yyvsp[-1].stmt)->exit,(yyvsp[0].stmt)->entry);
				gen_require ((yyvsp[0].stmt)->entry,(yyvsp[-1].stmt)->exit,(yyvsp[-1].stmt)->exit);
				if (current_proc){
					current_proc->exit_node = (yyvsp[0].stmt)->exit;
					current_proc->n_locals = current_local();
				}

				close_scope();
				fprintf (outfile,"%d(%d",LIF_PROC_END,(yyvsp[0].stmt)->exit);
				if (current_proc->has_return_xpr)
					fprintf (outfile,",R");
				fprintf (outfile,")");
				if (z_opt)fprintf (outfile,"    end function %s",
					(yyvsp[-3].token)->text);
				fprintf (outfile,"\n");
			}
#line 3146 "kgram-notworking.c"
    break;

  case 229:
#line 1014 "kgram.y"
                        {
				decl ((yyvsp[-1].token),NULL,False);
			}
#line 3154 "kgram-notworking.c"
    break;

  case 230:
#line 1018 "kgram.y"
                        {
				int	is_static = 0;

				do_returns ((yyvsp[0].stmt)->exit);
				connect_nodes ((yyvsp[-1].stmt)->exit,(yyvsp[0].stmt)->entry);
				gen_require ((yyvsp[0].stmt)->entry,(yyvsp[-1].stmt)->exit,(yyvsp[-1].stmt)->exit);
				close_scope();
				fprintf (outfile,"%d(%d",LIF_PROC_END,(yyvsp[0].stmt)->exit);
				if((yyvsp[-4].token)->desc.decl->decl_flags & FLAG_SC_STATIC){
					fprintf (outfile,",S");
					is_static = 1;
				}
				if (current_proc->has_return_xpr)
					fprintf (outfile,",R");
				fprintf (outfile,")");
				if (z_opt)fprintf (outfile,"    end %sfunction %s",
					is_static?"static ":"",
					(yyvsp[-3].token)->text);
				fprintf (outfile,"\n");
				if (current_proc){
					if (is_static) current_proc->static_or_extern = 'S';
					current_proc->type_decl = (yyvsp[-4].token);
					current_proc->exit_node = (yyvsp[0].stmt)->exit;
					current_proc->n_locals = current_local();
				}
			}
#line 3185 "kgram-notworking.c"
    break;

  case 231:
#line 1048 "kgram.y"
                {(yyval.stmt) = (yyvsp[0].stmt);}
#line 3191 "kgram-notworking.c"
    break;

  case 232:
#line 1050 "kgram.y"
                {
			token_ptr	fun,parm;
			type_ptr	t;
			int			pn,level;
			var_ste_ptr	v;

			(yyval.token) = NULL;
			fun = (yyvsp[(-2) - (1)].token);
			/*
			printf ("\tOld Fundtion body %s: ",$<token>-1->text); 
			*/
				t = fun->desc.type_desc;
				while (t){
					/* printf ("(mod %d style %d)",
						t->is,t->style);  */
					if ((t->is == IS_FUNC_RET) &&
						(t->style == OLD_STYLE)){
						parm = t->formals;
						pn = 1;
						while (parm){
							/* printf ("%s(%d) ",parm->text,pn); */
							v = look_up_id (NULL,parm->text,&level);
							if (!v || (level != 2)){
								/* printf ("not a parm  "); */
								insert_var_decl (parm);
								v = look_up_id (NULL,parm->text,&level);
							}
							/* if(v)printf ("[%d]  ",v->id) */;
							if (v->id != pn) v->id = pn;
							pn++;
							parm = parm->next;
						}
					}
					t=t->next;
				}
			/* printf ("\n"); */
		}
#line 3233 "kgram-notworking.c"
    break;

  case 233:
#line 1088 "kgram.y"
                {(yyval.stmt) = (yyvsp[0].stmt);}
#line 3239 "kgram-notworking.c"
    break;


#line 3243 "kgram-notworking.c"

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
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

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
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

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

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
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

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
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
                  yystos[+*yyssp], yyvsp);
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
  return yyresult;
}
#line 1098 "kgram.y"


void
print_flags (int flags)
{
	static char	*flag_names[] = {
		"char", "short", "int", "long", "signed",
		"unsigned", "float", "double", "const",
		"volatile", "void", "u/s", "enum", "user type",
		"typedef", "extern", "static", "auto", "register",
		"struct", "union"};
	int		i = 0;

	while (flags){
		if (flags & 1) printf ("%s ",flag_names[i]);
		i++;
		flags = flags >> 1;
	}
}

void
print_decl (type_spec,var_specs)
	token_ptr	type_spec,var_specs;
{
	token_ptr 	s,t;
	type_ptr	ty;

	printf ("Declaration: ");
	if (type_spec) {
		printf ("%8x ",type_spec->desc.decl->decl_flags);
		print_flags (type_spec->desc.decl->decl_flags);
		if (type_spec->desc.decl->decl_flags & FLAG_TYPE_US){
			printf ("\n    Struct/union: ");
			if (type_spec){
				if (type_spec->desc.decl->tag){
					printf ("(%s) ",
						type_spec->desc.decl->tag->text);
				}
				s = type_spec->desc.decl->members;
				while (s){
					if (s->desc.decl)print_decl (s,s->desc.decl->vars);
					s = s->next;
				}
			}
			printf ("\n    End Struct/union: ");
		}
		t = var_specs;
		while (t) {
			if(t->text)printf (" %s",t->text);
			if (ty = t->desc.type_desc){
				printf (" (");
				while(ty){
					if (ty->is == IS_FUNC_RET) 
						printf (" function returning");
					else if (ty->is == IS_ARRAY_OF) 
						printf (" array of");
					else if (ty->is == IS_PTR_TO) 
						printf (" pointer to");
					ty = ty -> next;
				}
				printf (")");
			}
			t = t -> next;
			if(t)printf ("\n%21s","");
		}
	}
	printf ("\n");
}

void
print_token(token_ptr t)
{
	printf ("TOKEN:");
	if (t){
		printf (" addr %p",t);
		if (t->text) printf (" (%s)",t->text);
		printf (" at %d(%d)-%d(%d)",
			t->at.line_start,
			t->at.col_start,
			t->at.line_end,
			t->at.col_end);
	}
	else printf (" is null");
	printf ("\n");
}

