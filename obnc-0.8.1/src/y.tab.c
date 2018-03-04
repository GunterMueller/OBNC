/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 18 "Oberon.y" /* yacc.c:339  */

#include "../lib/obnc/OBNC_.h"
#include "Files.h"
#include "Generate.h"
#include "Args.h"
#include "lex.yy.h"
#include "Maps.h"
#include "Range.h"
#include "Table.h"
#include "Types.h"
#include "Trees.h"
#include "Util.h"
#include "yyerror.h"
#include <assert.h>
#include <float.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Trees_Node unresolvedPointerTypes;
static Trees_Node currentTypeIdentdef;
static Trees_Node currentCaseExpression;
static Trees_Node caseExpressionType;
static Trees_Node currentlyDefinedCaseLabels;
static Trees_Node procedureDeclarationStack;

static char *QualidentName(const char qualifier[], const char ident[]);

/*constant predicate functions*/

static int IsBoolean(Trees_Node node);
static int IsChar(Trees_Node node);
static int IsInteger(Trees_Node node);
static int IsReal(Trees_Node node);
static int IsString(Trees_Node node);
static int IsSet(Trees_Node node);

/*functions for type declaration productions*/

static Trees_Node ResolvedType(Trees_Node type);
static void ResolvePointerTypes(Trees_Node baseType);

/*functions for expression productions*/

static int IsDesignator(Trees_Node exp);
static void CheckIsValue(Trees_Node exp);
static Trees_Node Designator(const char ident[], Trees_Node selectorList);
static int HasDynamicType(Trees_Node designator);
static void SetSelectorTypes(Trees_Node identType, Trees_Node designator, int *parameterListFound);
static void RemoveActualParameters(Trees_Node *designator, Trees_Node *actualParameters);
static int TypeStructureSymbol(Trees_Node exp);

/*functions for statement productions*/

static int Writable(Trees_Node designator);
static void ValidateAssignment(Trees_Node expression, Trees_Node targetType, int isParamSubst);
static void HandleProcedureCall(Trees_Node designator, Trees_Node actualParameters, int isFunctionCall, Trees_Node *ast);
static void CheckCaseLabelUniqueness(Trees_Node label);

/*functions for module productions*/

static char *ModuleDirPath(const char module[]);
static void ExportSymbolTable(const char symfilePath[]);

#line 132 "y.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TOKEN_START = 258,
    ARRAY = 259,
    BEGIN_ = 260,
    BY = 261,
    CASE = 262,
    CONST = 263,
    DIV = 264,
    DO = 265,
    ELSE = 266,
    ELSIF = 267,
    END = 268,
    FALSE = 269,
    FOR = 270,
    IF = 271,
    IMPORT = 272,
    IN = 273,
    IS = 274,
    MOD = 275,
    MODULE = 276,
    NIL = 277,
    OF = 278,
    OR = 279,
    POINTER = 280,
    PROCEDURE = 281,
    RECORD = 282,
    REPEAT = 283,
    RETURN = 284,
    THEN = 285,
    TO = 286,
    TRUE = 287,
    TYPE = 288,
    UNTIL = 289,
    VAR = 290,
    WHILE = 291,
    BECOMES = 292,
    DOTDOT = 293,
    GE = 294,
    LE = 295,
    IDENT = 296,
    INTEGER = 297,
    REAL = 298,
    STRING = 299,
    TOKEN_END = 300
  };
#endif
/* Tokens.  */
#define TOKEN_START 258
#define ARRAY 259
#define BEGIN_ 260
#define BY 261
#define CASE 262
#define CONST 263
#define DIV 264
#define DO 265
#define ELSE 266
#define ELSIF 267
#define END 268
#define FALSE 269
#define FOR 270
#define IF 271
#define IMPORT 272
#define IN 273
#define IS 274
#define MOD 275
#define MODULE 276
#define NIL 277
#define OF 278
#define OR 279
#define POINTER 280
#define PROCEDURE 281
#define RECORD 282
#define REPEAT 283
#define RETURN 284
#define THEN 285
#define TO 286
#define TRUE 287
#define TYPE 288
#define UNTIL 289
#define VAR 290
#define WHILE 291
#define BECOMES 292
#define DOTDOT 293
#define GE 294
#define LE 295
#define IDENT 296
#define INTEGER 297
#define REAL 298
#define STRING 299
#define TOKEN_END 300

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 84 "Oberon.y" /* yacc.c:355  */

	const char *ident;
	int integer;
	double real;
	const char *string;
	Trees_Node node;

#line 270 "y.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 287 "y.tab.c" /* yacc.c:358  */

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
#else
typedef signed char yytype_int8;
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
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

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

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
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

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   236

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  68
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  97
/* YYNRULES -- Number of rules.  */
#define YYNRULES  181
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  284

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   300

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,    54,     2,     2,    60,     2,
      50,    51,    47,    57,    49,    58,    46,    59,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    53,    52,
      55,    48,    56,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    62,     2,    63,    64,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    65,    67,    66,    61,     2,     2,     2,
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
      45
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   198,   198,   202,   209,   227,   232,   241,   245,   255,
     270,   294,   320,   330,   331,   332,   333,   334,   338,   358,
     368,   372,   379,   396,   404,   415,   420,   426,   457,   463,
     469,   473,   480,   511,   515,   525,   557,   566,   574,   582,
     584,   593,   626,   627,  1112,  1116,  1120,  1124,  1128,  1132,
    1136,  1140,  1147,  1210,  1323,  1327,  1332,  1338,  1342,  1346,
    1353,  1354,  1498,  1502,  1506,  1510,  1514,  1521,  1522,  1526,
    1531,  1536,  1541,  1547,  1565,  1570,  1594,  1614,  1633,  1639,
    1647,  1665,  1670,  1675,  1682,  1686,  1693,  1694,  1706,  1726,
    1750,  1755,  1769,  1770,  1771,  1772,  1773,  1774,  1775,  1777,
    1783,  1826,  1846,  1854,  1862,  1873,  1896,  1909,  1914,  1920,
    1925,  1931,  1949,  1988,  1996,  2011,  2017,  2023,  2027,  2043,
    2049,  2093,  2102,  2116,  2191,  2198,  2203,  2209,  2224,  2242,
    2268,  2279,  2294,  2303,  2356,  2380,  2395,  2400,  2406,  2411,
    2417,  2421,  2422,  2426,  2427,  2431,  2443,  2449,  2456,  2457,
    2461,  2462,  2466,  2467,  2471,  2472,  2476,  2483,  2489,  2495,
    2500,  2516,  2536,  2542,  2567,  2572,  2578,  2582,  2589,  2604,
    2609,  2618,  2644,  2658,  2665,  2673,  2712,  2716,  2723,  2784,
    2789,  2795
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TOKEN_START", "ARRAY", "BEGIN_", "BY",
  "CASE", "CONST", "DIV", "DO", "ELSE", "ELSIF", "END", "FALSE", "FOR",
  "IF", "IMPORT", "IN", "IS", "MOD", "MODULE", "NIL", "OF", "OR",
  "POINTER", "PROCEDURE", "RECORD", "REPEAT", "RETURN", "THEN", "TO",
  "TRUE", "TYPE", "UNTIL", "VAR", "WHILE", "BECOMES", "DOTDOT", "GE", "LE",
  "IDENT", "INTEGER", "REAL", "STRING", "TOKEN_END", "'.'", "'*'", "'='",
  "','", "'('", "')'", "';'", "':'", "'#'", "'<'", "'>'", "'+'", "'-'",
  "'/'", "'&'", "'~'", "'['", "']'", "'^'", "'{'", "'}'", "'|'", "$accept",
  "qualident", "identdef", "ExportMarkOpt", "number", "ConstDeclaration",
  "ConstExpression", "TypeDeclaration", "TypeIdentDef", "type",
  "ArrayType", "ArrayLengthOf", "LengthRep", "length", "RecordType",
  "RecordHeading", "BaseTypeOpt", "BaseType", "FieldListSequenceOpt",
  "FieldListSequence", "FieldList", "IdentList", "PointerType",
  "PointerTo", "ProcedureType", "ProcedureTypeSansParam",
  "FormalParametersOpt", "VariableDeclaration", "expression", "relation",
  "SimpleExpression", "SignOpt", "AddOperator", "term", "MulOperator",
  "factor", "designator", "SelectorOptRep", "selector", "set",
  "ElementRep", "element", "ExpList", "statement", "assignment",
  "ProcedureCall", "StatementSequence", "StatementSequenceReversed",
  "IfStatement", "guard", "ElseIfThenOptRep", "ElseOpt", "CaseStatement",
  "CaseExpression", "CaseRep", "case", "CaseLabelList", "LabelRange",
  "label", "WhileStatement", "ElseIfDoOptRep", "RepeatStatement",
  "ForStatement", "ForInit", "ForLimit", "ByOpt", "ProcedureDeclaration",
  "ProcedureHeading", "ProcedureHeadingSansParam", "StatementSequenceOpt",
  "ReturnExpressionOpt", "DeclarationSequence", "ConstSectionOpt",
  "ConstDeclarationOptRep", "TypeSectionOpt", "TypeKeyword",
  "TypeDeclarationOptRep", "VariableSectionOpt",
  "VariableDeclarationOptRep", "ProcedureDeclarationOptRep",
  "FormalParameters", "FPSectionsOpt", "FPSectionRep", "ResultTypeOpt",
  "FPSection", "ParameterKindOpt", "IdentRep", "FormalType",
  "OpenArrayOpt", "module", "ModuleHeading", "ImportListOpt", "ImportList",
  "ImportRep", "import", "BecomesIdentOpt", "ModuleStatements", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,    46,    42,    61,    44,
      40,    41,    59,    58,    35,    60,    62,    43,    45,    47,
      38,   126,    91,    93,    94,   123,   125,   124
};
# endif

#define YYPACT_NINF -133

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-133)))

#define YYTABLE_NINF -166

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      18,   -24,    41,    -3,  -133,  -133,    34,    50,    98,  -133,
      75,   -21,  -133,  -133,   111,    85,    79,  -133,    50,  -133,
      80,    20,  -133,   109,  -133,    88,  -133,  -133,  -133,    77,
      83,    78,    25,    91,    25,    20,    25,  -133,    96,  -133,
    -133,  -133,  -133,    84,  -133,  -133,  -133,  -133,  -133,    93,
    -133,  -133,    80,  -133,  -133,    25,  -133,  -133,  -133,  -133,
      71,     2,   112,   100,   107,  -133,   110,   105,   131,   -32,
      25,    20,    97,    80,   116,    99,    92,    72,  -133,  -133,
    -133,  -133,  -133,  -133,  -133,  -133,  -133,  -133,  -133,  -133,
    -133,    25,     2,  -133,  -133,  -133,  -133,  -133,  -133,    25,
       2,     0,  -133,    13,  -133,  -133,  -133,    73,    25,    25,
      20,    25,    20,   104,    29,    25,  -133,  -133,  -133,  -133,
    -133,  -133,     6,    94,    80,   101,   102,   106,  -133,  -133,
      25,   117,  -133,   108,   103,  -133,  -133,  -133,    72,  -133,
      80,  -133,    72,  -133,   106,    -4,    13,   113,  -133,  -133,
     114,   -40,  -133,  -133,  -133,  -133,  -133,  -133,     2,  -133,
    -133,  -133,    -5,  -133,    15,  -133,   121,  -133,  -133,   144,
    -133,  -133,  -133,  -133,  -133,  -133,   -28,   -34,    80,    72,
    -133,  -133,  -133,    98,   -22,  -133,  -133,  -133,   -11,  -133,
    -133,   119,  -133,   124,  -133,   138,   115,  -133,    21,  -133,
    -133,  -133,    25,    25,  -133,  -133,  -133,    73,    73,    20,
      73,    25,   145,    82,    90,    25,  -133,  -133,  -133,  -133,
     111,  -133,   118,   125,  -133,   127,  -133,    25,  -133,   120,
    -133,  -133,    80,    72,  -133,  -133,  -133,  -133,  -133,  -133,
    -133,    20,    20,    25,   149,    25,  -133,  -133,   137,   123,
     135,  -133,    43,  -133,  -133,  -133,  -133,   165,  -133,   150,
    -133,   169,    25,   170,   119,  -133,  -133,   141,   180,  -133,
      20,    20,  -133,   146,  -133,  -133,   163,  -133,   119,  -133,
    -133,  -133,  -133,  -133
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,   172,     1,   174,     0,   142,   173,
     180,     0,   176,   144,   137,   146,     0,   178,     0,   175,
     141,    99,   181,     0,   147,   151,   149,   179,   177,     6,
       0,     0,    56,     0,    56,    99,    56,    78,   101,   103,
      92,    93,   136,   102,    94,    95,    96,    97,    98,     0,
     153,   155,   145,     5,     4,    56,   143,    54,    55,   112,
      42,     0,     0,     0,     0,   106,     0,     0,     0,    76,
      56,    99,     0,   150,   140,     0,     0,     0,     9,    10,
      50,    51,    59,    49,    47,    44,    45,    46,    48,    57,
      58,    56,     0,    71,    69,    70,     7,     8,    68,    56,
       0,    56,    67,    52,    60,    73,    72,   116,    56,    56,
      99,    56,    99,     0,    56,    56,    81,    77,   100,   104,
     171,    33,     0,     0,     0,     0,     0,    40,    12,   148,
      56,     0,    38,    26,     2,    13,    11,    14,     0,    15,
      29,    16,     0,    17,    40,    43,    53,     0,    75,    84,
      88,     0,    86,    64,    65,    62,    63,    66,     0,   121,
     122,   123,     0,   113,     0,   117,   119,   129,   130,   132,
     108,   127,   126,    79,    83,    90,     0,     0,     0,     0,
     152,   135,   154,   142,   158,   134,    39,    22,     0,    20,
      36,     0,    24,     0,    18,     0,    28,    30,     0,    35,
      37,    74,    56,    56,    85,    61,   111,   116,     0,    99,
       0,    56,     0,   110,     0,    56,    82,    80,    34,    41,
     137,   164,     0,   157,   159,     0,    19,    56,    27,     0,
       3,    23,     0,     0,    89,    87,   114,   118,   115,   120,
     131,    99,    99,    56,     0,    56,   124,    91,   139,   162,
     165,   166,     0,    21,    25,    31,    32,     0,   109,     0,
     105,     0,    56,     0,     0,   156,   160,     0,   170,   128,
      99,    99,   138,     0,   161,   167,     0,   163,     0,   107,
     125,   133,   169,   168
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -133,  -103,   -15,  -133,  -133,  -133,   -54,  -133,  -133,  -132,
    -133,  -133,  -133,   -39,  -133,  -133,  -133,  -133,  -133,  -133,
     -43,   122,  -133,  -133,  -133,  -133,    46,  -133,   -30,  -133,
     126,  -133,  -133,   128,  -133,   -93,   -50,  -133,  -133,  -133,
    -133,   -12,    81,   129,  -133,  -133,   -35,  -133,  -133,   -33,
    -133,  -133,  -133,  -133,  -133,   -14,  -133,   -16,    -9,  -133,
    -133,  -133,  -133,  -133,  -133,  -133,  -133,  -133,  -133,   -26,
    -133,    16,  -133,  -133,  -133,  -133,  -133,  -133,  -133,  -133,
    -133,  -133,  -133,  -133,   -52,  -133,  -133,  -133,  -133,  -133,
    -133,  -133,  -133,  -133,   184,  -133,  -133
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,   135,   121,    54,   102,    31,   187,    76,    77,   136,
     137,   138,   188,   189,   139,   140,   192,   229,   195,   196,
     197,   198,   141,   142,   143,   144,   185,   123,    65,    91,
      60,    61,    92,   103,   158,   104,    38,    69,   117,   106,
     151,   152,   176,    39,    40,    41,    42,    43,    44,    66,
     213,   244,    45,    62,   162,   163,   164,   165,   166,    46,
     214,    47,    48,    64,   169,   212,   125,   126,   127,    22,
     263,    14,    15,    20,    25,    26,    52,    51,    73,    74,
     186,   222,   223,   265,   224,   225,   252,   277,   278,     2,
       3,     8,     9,    11,    12,    17,    23
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      67,    78,    59,    68,   161,    30,   194,   148,   206,   203,
     199,   105,   226,   221,   113,   215,    93,     4,   114,  -165,
      82,   215,   153,   216,    94,    79,   204,    32,    18,   217,
     115,    19,   116,   154,    95,    33,    34,    75,   227,     1,
     118,     5,   105,    37,    96,    97,    98,   219,    35,     6,
     105,     7,    99,    89,    90,   178,    36,    57,    58,   179,
     155,    37,   207,   100,   208,   205,   149,   101,   209,   147,
     178,   150,   156,   157,   233,   170,   130,   172,   167,   168,
     174,   171,    57,    58,   175,   175,    57,    58,   228,    80,
      81,    10,   267,   242,   243,    82,   268,   131,   132,   133,
      79,   256,   245,   246,   161,   161,    13,   161,   105,   181,
      83,    84,    16,   134,   134,   159,    21,   160,    24,    85,
      27,    29,    49,    50,    53,    86,    87,    88,    89,    90,
      56,    55,    63,    70,    72,   107,    71,   108,   109,   111,
     110,   112,   124,   120,   129,   173,   180,   128,   190,   193,
     211,   231,   202,   182,   183,   241,   184,   240,   191,   210,
     134,   274,   260,   218,   201,   230,   262,   232,   251,   249,
     221,   254,   234,   150,   238,   283,   264,   250,   269,   271,
     270,    79,   275,   273,   276,   247,   282,   281,   253,   255,
     200,   235,   237,   236,   248,   122,   177,    79,   266,   220,
     119,   239,    28,     0,     0,     0,   257,   258,     0,     0,
     259,     0,   261,     0,     0,     0,     0,   145,     0,     0,
     146,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   272,     0,     0,   279,   280
};

static const yytype_int16 yycheck[] =
{
      35,    55,    32,    36,   107,    20,   138,   100,    13,    49,
     142,    61,    23,    35,    46,    49,    14,    41,    50,    41,
      24,    49,     9,    51,    22,    55,    66,     7,    49,    63,
      62,    52,    64,    20,    32,    15,    16,    52,    49,    21,
      70,     0,    92,    41,    42,    43,    44,   179,    28,    52,
     100,    17,    50,    57,    58,    49,    36,    57,    58,    53,
      47,    41,    67,    61,    49,   158,    66,    65,    53,    99,
      49,   101,    59,    60,    53,   110,     4,   112,   108,   109,
      51,   111,    57,    58,   114,   115,    57,    58,   191,    18,
      19,    41,    49,    11,    12,    24,    53,    25,    26,    27,
     130,   233,    12,    13,   207,   208,     8,   210,   158,   124,
      39,    40,    37,    41,    41,    42,     5,    44,    33,    48,
      41,    41,    13,    35,    47,    54,    55,    56,    57,    58,
      52,    48,    41,    37,    41,    23,    52,    37,    31,    34,
      30,    10,    26,    46,    52,    41,    52,    48,    31,    46,
       6,    13,    38,    52,    52,    10,    50,   211,    50,    38,
      41,   264,    13,   178,    51,    41,    29,    52,    41,    51,
      35,    51,   202,   203,   209,   278,    53,    52,    13,    10,
      30,   211,    41,    13,     4,   215,    23,    41,   227,   232,
     144,   203,   208,   207,   220,    73,   115,   227,   250,   183,
      71,   210,    18,    -1,    -1,    -1,   241,   242,    -1,    -1,
     243,    -1,   245,    -1,    -1,    -1,    -1,    91,    -1,    -1,
      92,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   262,    -1,    -1,   270,   271
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    21,   157,   158,    41,     0,    52,    17,   159,   160,
      41,   161,   162,     8,   139,   140,    37,   163,    49,    52,
     141,     5,   137,   164,    33,   142,   143,    41,   162,    41,
      70,    73,     7,    15,    16,    28,    36,    41,   104,   111,
     112,   113,   114,   115,   116,   120,   127,   129,   130,    13,
      35,   145,   144,    47,    71,    48,    52,    57,    58,    96,
      98,    99,   121,    41,   131,    96,   117,   114,   117,   105,
      37,    52,    41,   146,   147,    70,    75,    76,    74,    96,
      18,    19,    24,    39,    40,    48,    54,    55,    56,    57,
      58,    97,   100,    14,    22,    32,    42,    43,    44,    50,
      61,    65,    72,   101,   103,   104,   107,    23,    37,    31,
      30,    34,    10,    46,    50,    62,    64,   106,    96,   111,
      46,    70,    89,    95,    26,   134,   135,   136,    48,    52,
       4,    25,    26,    27,    41,    69,    77,    78,    79,    82,
      83,    90,    91,    92,    93,    98,   101,    96,   103,    66,
      96,   108,   109,     9,    20,    47,    59,    60,   102,    42,
      44,    69,   122,   123,   124,   125,   126,    96,    96,   132,
     114,    96,   114,    41,    51,    96,   110,   110,    49,    53,
      52,    70,    52,    52,    50,    94,   148,    74,    80,    81,
      31,    50,    84,    46,    77,    86,    87,    88,    89,    77,
      94,    51,    38,    49,    66,   103,    13,    67,    49,    53,
      38,     6,   133,   118,   128,    49,    51,    63,    70,    77,
     139,    35,   149,   150,   152,   153,    23,    49,    69,    85,
      41,    13,    52,    53,    96,   109,   123,   125,   114,   126,
      74,    10,    11,    12,   119,    12,    13,    96,   137,    51,
      52,    41,   154,    81,    51,    88,    77,   114,   114,   117,
      13,   117,    29,   138,    53,   151,   152,    49,    53,    13,
      30,    10,    96,    13,    69,    41,     4,   155,   156,   114,
     114,    41,    23,    69
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    68,    69,    69,    70,    71,    71,    72,    72,    73,
      74,    75,    76,    77,    77,    77,    77,    77,    78,    79,
      80,    80,    81,    82,    83,    84,    84,    85,    86,    86,
      87,    87,    88,    89,    89,    90,    91,    92,    93,    94,
      94,    95,    96,    96,    97,    97,    97,    97,    97,    97,
      97,    97,    98,    98,    99,    99,    99,   100,   100,   100,
     101,   101,   102,   102,   102,   102,   102,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   104,   105,   105,   106,
     106,   106,   106,   106,   107,   107,   108,   108,   109,   109,
     110,   110,   111,   111,   111,   111,   111,   111,   111,   111,
     112,   113,   114,   115,   115,   116,   117,   118,   118,   119,
     119,   120,   121,   122,   122,   123,   123,   124,   124,   125,
     125,   126,   126,   126,   127,   128,   128,   129,   130,   131,
     132,   133,   133,   134,   135,   136,   137,   137,   138,   138,
     139,   140,   140,   141,   141,   142,   142,   143,   144,   144,
     145,   145,   146,   146,   147,   147,   148,   149,   149,   150,
     150,   151,   151,   152,   153,   153,   154,   154,   155,   156,
     156,   157,   158,   159,   159,   160,   161,   161,   162,   163,
     163,   164
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     3,     2,     1,     0,     1,     1,     3,
       1,     2,     2,     1,     1,     1,     1,     1,     2,     3,
       1,     3,     1,     3,     2,     3,     0,     1,     1,     0,
       1,     3,     3,     1,     3,     2,     2,     2,     1,     1,
       0,     3,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     3,     1,     1,     0,     1,     1,     1,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     3,     2,     2,     2,     0,     2,
       3,     1,     3,     2,     2,     3,     1,     3,     1,     3,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     0,
       3,     1,     1,     1,     3,     7,     1,     5,     0,     2,
       0,     5,     1,     1,     3,     3,     0,     1,     3,     1,
       3,     1,     1,     1,     6,     5,     0,     4,     8,     3,
       1,     2,     0,     7,     2,     2,     2,     0,     2,     0,
       4,     2,     0,     3,     0,     2,     0,     1,     3,     0,
       2,     0,     3,     0,     3,     0,     4,     1,     0,     1,
       3,     2,     0,     4,     1,     0,     1,     3,     2,     2,
       0,     8,     2,     1,     0,     3,     1,     3,     2,     2,
       0,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
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


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
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
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
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

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

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
      int yyn = yypact[*yyssp];
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
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
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
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
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
          yyp++;
          yyformat++;
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
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
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
  int yytoken = 0;
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
#line 199 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewIdent((yyvsp[0].ident));
	}
#line 1597 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 203 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewIdent(QualidentName((yyvsp[-2].ident), (yyvsp[0].ident)));
	}
#line 1605 "y.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 210 "Oberon.y" /* yacc.c:1646  */
    {
		if (! Table_LocallyDeclared((yyvsp[-1].ident))) {
			(yyval.node) = Trees_NewIdent((yyvsp[-1].ident));
			if ((yyvsp[0].integer)) {
				Trees_SetExported((yyval.node));		
			}
			if (Table_ScopeLocal()) {
				Trees_SetLocal((yyval.node));
			}
		} else {
			yyerror("error: identifier already declared in this scope: %s", (yyvsp[-1].ident));
			YYABORT;
		}
	}
#line 1624 "y.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 228 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = 1;
	}
#line 1632 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 232 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = 0;
	}
#line 1640 "y.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 242 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewInteger((yyvsp[0].integer));
	}
#line 1648 "y.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 246 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewReal((yyvsp[0].real));
	}
#line 1656 "y.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 256 "Oberon.y" /* yacc.c:1646  */
    {
		if (! (Trees_Exported((yyvsp[-2].node)) && Trees_Local((yyvsp[-2].node)))) {
			Trees_SetKind(TREES_CONSTANT_KIND, (yyvsp[-2].node));
			Trees_SetValue((yyvsp[0].node), (yyvsp[-2].node));
			Table_Put((yyvsp[-2].node));
			Generate_ConstDeclaration((yyvsp[-2].node));
		} else {
			yyerror("error: cannot export local constant: %s", Trees_Name((yyvsp[-2].node)));
			YYABORT;
		}
	}
#line 1672 "y.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 271 "Oberon.y" /* yacc.c:1646  */
    {
		switch (Trees_Symbol((yyvsp[0].node))) {
			case TRUE:
			case FALSE:
			case STRING:
			case TREES_CHAR_CONSTANT:
			case INTEGER:
			case REAL:
			case TREES_SET_CONSTANT:
			case NIL:
				(yyval.node) = (yyvsp[0].node);
				break;
			default:
				yyerror("error: constant expression expected");
				YYABORT;				
		}
	}
#line 1694 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 295 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node sourceType;
	
		sourceType = ResolvedType((yyvsp[0].node));
		if (sourceType != NULL) {
			if (Trees_Exported((yyvsp[-1].node)) && Trees_Local((yyvsp[-1].node))) {
				yyerror("error: cannot export local type: %s", Trees_Name((yyvsp[-1].node)));
				YYABORT;
			} else {
				Trees_SetType(sourceType, (yyvsp[-1].node));
				if (! Table_LocallyDeclared(Trees_Name((yyvsp[-1].node)))) {
					Table_Put((yyvsp[-1].node));
				}
				ResolvePointerTypes((yyvsp[-1].node));
				currentTypeIdentdef = NULL;
				Generate_TypeDeclaration((yyvsp[-1].node));
			}
		} else {
			yyerror("error: undeclared identifier: %s", Trees_Name((yyvsp[0].node)));
			YYABORT;
		}
	}
#line 1721 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 321 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_SetKind(TREES_TYPE_KIND, (yyvsp[-1].node));
		currentTypeIdentdef = (yyvsp[-1].node);
		Table_Put(currentTypeIdentdef);
		(yyval.node) = (yyvsp[-1].node);
	}
#line 1732 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 339 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node reversedLengths, length;
		
		(yyval.node) = ResolvedType((yyvsp[0].node));
		if ((yyval.node) != NULL) {
			reversedLengths = (yyvsp[-1].node);
			do {
				length = Trees_Left(reversedLengths);
				(yyval.node) = Types_NewArray(length, (yyval.node));
				reversedLengths = Trees_Right(reversedLengths);
			} while (reversedLengths != NULL);
		} else {
			yyerror("error: undeclared identifier: %s", Trees_Name((yyvsp[0].node)));
			exit(EXIT_FAILURE);
		}
	}
#line 1753 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 359 "Oberon.y" /* yacc.c:1646  */
    {
		if ((currentTypeIdentdef != NULL) && (Trees_Type(currentTypeIdentdef) == NULL)) {
			Trees_SetType(Trees_NewLeaf(ARRAY), currentTypeIdentdef); /*incomplete type*/
		}
		(yyval.node) = (yyvsp[-1].node);
	}
#line 1764 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 369 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewNode(TREES_NOSYM, (yyvsp[0].node), NULL);
	}
#line 1772 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 373 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewNode(TREES_NOSYM, (yyvsp[0].node), (yyvsp[-2].node));
	}
#line 1780 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 380 "Oberon.y" /* yacc.c:1646  */
    {
		if (IsInteger((yyvsp[0].node))) {
			if (Trees_Integer((yyvsp[0].node)) > 0) {
				(yyval.node) = (yyvsp[0].node);
			} else {
				yyerror("error: positive length expected");
				YYABORT;
			}
		} else {
			yyerror("error: integer length expected");
			YYABORT;		
		}
	}
#line 1798 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 397 "Oberon.y" /* yacc.c:1646  */
    {
		Table_CloseScope();
		(yyval.node) = Types_NewRecord(Types_RecordBaseType((yyvsp[-2].node)), (yyvsp[-1].node));
	}
#line 1807 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 405 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Types_NewRecord((yyvsp[0].node), NULL);
		if ((currentTypeIdentdef != NULL) && (Trees_Type(currentTypeIdentdef) == NULL)) {
			Trees_SetType((yyval.node), currentTypeIdentdef);
		}
		Table_OpenScope();
	}
#line 1819 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 416 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = (yyvsp[-1].node);
	}
#line 1827 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 420 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = NULL;
	}
#line 1835 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 427 "Oberon.y" /* yacc.c:1646  */
    {
		const char *name;
		Trees_Node symbol;
		
		(yyval.node) = NULL;
		name = Trees_Name((yyvsp[0].node));
		symbol = Table_At(name);
		if (symbol != NULL) {
			if (Trees_Kind(symbol) == TREES_TYPE_KIND) {
				switch (Trees_Symbol(Types_Structure(symbol))) {
					case RECORD:
					case POINTER:
						(yyval.node) = symbol;
						break;
					default:
						yyerror("error: record or pointer base type expected: %s", name);
						YYABORT;			
				}
			} else {
				yyerror("error: type name expected: %s", name);
				YYABORT;
			}
		} else {
			yyerror("error: undeclared identifier: %s", name);
			YYABORT;
		}
	}
#line 1867 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 458 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_ReverseList(&(yyvsp[0].node)); /*correct order*/
		(yyval.node) = (yyvsp[0].node);
	}
#line 1876 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 463 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = NULL;
	}
#line 1884 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 470 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewNode(TREES_FIELD_LIST_SEQUENCE, (yyvsp[0].node), NULL);
	}
#line 1892 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 474 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewNode(TREES_FIELD_LIST_SEQUENCE, (yyvsp[0].node), (yyvsp[-2].node));
	}
#line 1900 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 481 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node type, identList, ident;

		(yyval.node) = NULL;
		type = ResolvedType((yyvsp[0].node));
		if (type != NULL) {
			if (! ((type == currentTypeIdentdef) && ! Types_IsPointer(type))) {
				Trees_ReverseList(&(yyvsp[-2].node)); /*correct order*/
				identList = (yyvsp[-2].node);
				do {
					ident = Trees_Left(identList);
					Trees_SetKind(TREES_FIELD_KIND, ident);
					Trees_SetType(type, ident);
					Table_Put(ident);
					identList = Trees_Right(identList);
				} while (identList != NULL);
		
				(yyval.node) = (yyvsp[-2].node);			
			} else {
				yyerror("error: recursive field type must be a pointer: %s", Trees_Name((yyvsp[0].node)));
				YYABORT;					
			}
		} else {
			yyerror("error: undeclared type: %s", Trees_Name((yyvsp[0].node)));
			YYABORT;
		}
	}
#line 1932 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 512 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewNode(TREES_IDENT_LIST, (yyvsp[0].node), NULL);
	}
#line 1940 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 516 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node reversedIdents;
		
		reversedIdents = Trees_NewNode(TREES_IDENT_LIST, (yyvsp[0].node), (yyvsp[-2].node));
		(yyval.node) = reversedIdents;
	}
#line 1951 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 526 "Oberon.y" /* yacc.c:1646  */
    {
		const char *baseTypeName;
		Trees_Node declaredBaseType;

		(yyval.node) = NULL;
		if (Trees_Symbol((yyvsp[0].node)) == IDENT) {
			baseTypeName = Trees_Name((yyvsp[0].node));
			declaredBaseType = Table_At(baseTypeName);
			if (declaredBaseType != NULL) {
				if (Types_IsRecord(declaredBaseType)) {
					(yyval.node) = Types_NewPointer(declaredBaseType);
				} else {
					yyerror("error: record expected as pointer base type: %s", baseTypeName);
					YYABORT;
				}
			} else {
				Trees_SetKind(TREES_TYPE_KIND, (yyvsp[0].node));
				Trees_SetType(Types_NewRecord(NULL, NULL), (yyvsp[0].node));
				(yyval.node) = Types_NewPointer((yyvsp[0].node));
				unresolvedPointerTypes = Trees_NewNode(TREES_NOSYM, (yyval.node), unresolvedPointerTypes);
			}
		} else if(Trees_Symbol((yyvsp[0].node)) == RECORD) {
			(yyval.node) = Types_NewPointer((yyvsp[0].node));
		} else {
			yyerror("error: record expected as pointer base type");
			YYABORT;
		}
	}
#line 1984 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 558 "Oberon.y" /* yacc.c:1646  */
    {
		if ((currentTypeIdentdef != NULL) && (Trees_Type(currentTypeIdentdef) == NULL)) {
			Trees_SetType(Types_NewPointer(NULL), currentTypeIdentdef); /*incomplete type*/
		}
	}
#line 1994 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 567 "Oberon.y" /* yacc.c:1646  */
    {
		Table_CloseScope();
		(yyval.node) = (yyvsp[0].node);
	}
#line 2003 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 575 "Oberon.y" /* yacc.c:1646  */
    {
		Table_OpenScope();
		(yyval.node) = NULL;
	}
#line 2012 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 584 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewLeaf(PROCEDURE);
	}
#line 2020 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 594 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node type, identList, ident;

		type = ResolvedType((yyvsp[0].node));
		if (type != NULL) {
			Trees_ReverseList(&(yyvsp[-2].node)); /*correct order*/
			identList = (yyvsp[-2].node);
			do {
				ident = Trees_Left(identList);
				if (Trees_Exported(ident) && Trees_Local(ident)) {
					yyerror("error: cannot export local variable: %s", Trees_Name(ident));
					YYABORT;
				} else {
					Trees_SetKind(TREES_VARIABLE_KIND, ident);
					Trees_SetType(type, ident);
					Table_Put(ident);
				}			
				identList = Trees_Right(identList);
			} while (identList != NULL);
		
			Generate_VariableDeclaration((yyvsp[-2].node));
		} else {
			yyerror("error: undeclared identifier: %s", Trees_Name((yyvsp[0].node)));
			exit(EXIT_FAILURE);
		}
	}
#line 2051 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 628 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node leftExp, rightExp, leftType, rightType;
		int relation, leftSym, rightSym, error;
		
		leftExp = (yyvsp[-2].node);
		rightExp = (yyvsp[0].node);
		relation = (yyvsp[-1].integer);
		leftSym = Trees_Symbol((yyvsp[-2].node));
		rightSym = Trees_Symbol((yyvsp[0].node));
		leftType = Trees_Type((yyvsp[-2].node));
		rightType = Trees_Type((yyvsp[0].node));

		CheckIsValue((yyvsp[-2].node));
		if ((yyvsp[-1].integer) != IS) {
			CheckIsValue((yyvsp[0].node));
		}

		(yyval.node) = NULL;
		/*check types and evaluate constant expressions*/
		error = 0;
		switch (relation) {
			case '=':
				switch (Trees_Symbol(Types_Structure(leftType))) {
					case TREES_BOOLEAN_TYPE:
						if (Types_ExpressionCompatible(leftType, rightType)) {
							if (IsBoolean(leftExp) && IsBoolean(rightExp)) {
								(yyval.node) = Trees_NewLeaf((leftSym == rightSym)? TRUE: FALSE);
							}
						} else {
							error = 1;
						}
						break;
					case TREES_CHAR_TYPE:
						if (Types_ExpressionCompatible(leftType, rightType)) {
							if (IsString(rightExp)) {
								rightExp = Trees_NewChar(Trees_String(rightExp)[0]);
							}
							if (IsChar(leftExp) && IsChar(rightExp)) {
								(yyval.node) = Trees_NewLeaf((Trees_Char(leftExp) == Trees_Char(rightExp))? TRUE: FALSE);
							}
						} else {
							error = 1;
						}
						break;
					case TREES_INTEGER_TYPE:	
						if (Types_ExpressionCompatible(leftType, rightType)) {
							if (IsInteger(leftExp) && IsInteger(rightExp)) {
								(yyval.node) = Trees_NewLeaf((Trees_Integer(leftExp) == Trees_Integer(rightExp))? TRUE: FALSE);
							}
						} else {
							error = 1;
						}
						break;
					case TREES_REAL_TYPE:
						if (Types_ExpressionCompatible(leftType, rightType)) {
							if (IsReal(leftExp) && IsReal(rightExp)) {
								(yyval.node) = Trees_NewLeaf((Trees_Real(leftExp) == Trees_Real(rightExp))? TRUE: FALSE);
							}
						} else {
							error = 1;
						}
						break;
					case TREES_BYTE_TYPE:
						error = ! Types_ExpressionCompatible(leftType, rightType);
						break;
					case TREES_SET_TYPE:
						if (Types_ExpressionCompatible(leftType, rightType)) {
							if (IsSet(leftExp) && IsSet(rightExp)) {
								(yyval.node) = Trees_NewLeaf((Trees_Set(leftExp) == Trees_Set(rightExp))? TRUE: FALSE);
							}
						} else {
							error = 1;
						}
						break;
					case TREES_STRING_TYPE:
						if (Types_ExpressionCompatible(leftType, rightType)) {
							if (Types_IsChar(rightType)) {
								leftExp = Trees_NewChar(Trees_String(leftExp)[0]);
							}
							if (IsString(leftExp) && IsString(rightExp)) {
								(yyval.node) = Trees_NewLeaf((strcmp(Trees_String(leftExp), Trees_String(rightExp)) == 0)? TRUE: FALSE);
							}
						} else {
							error = 1;
						}
						break;
					case ARRAY:
						error = ! Types_ExpressionCompatible(leftType, rightType);
						break;
					case POINTER:
						error = ! ((Trees_Symbol(rightExp) == NIL) || Types_Extends(leftType, rightType) || Types_Extends(rightType, leftType));
						break;
					case PROCEDURE:
						error = ! ((Trees_Symbol(rightExp) == NIL) || Types_Same(leftType, rightType));
						break;
					case TREES_NIL_TYPE:
						error = ! (Types_IsPointer(rightType) || Types_IsProcedure(rightType));
						break;
					default:						
						error = 1;
				}
				break;
			case '#':
				switch (Trees_Symbol(Types_Structure(leftType))) {
					case TREES_BOOLEAN_TYPE:
						if (Types_ExpressionCompatible(leftType, rightType)) {
							if (IsBoolean(leftExp) && IsBoolean(rightExp)) {
								(yyval.node) = Trees_NewLeaf((leftSym != rightSym)? TRUE: FALSE);
							}
						} else {
							error = 1;
						}
						break;
					case TREES_CHAR_TYPE:
						if (Types_ExpressionCompatible(leftType, rightType)) {
							if (IsString(rightExp)) {
								rightExp = Trees_NewChar(Trees_String(rightExp)[0]);
							}
							if (IsChar(leftExp) && IsChar(rightExp)) {
								(yyval.node) = Trees_NewLeaf((Trees_Char(leftExp) != Trees_Char(rightExp))? TRUE: FALSE);
							}
						} else {
							error = 1;
						}
						break;
					case TREES_INTEGER_TYPE:	
						if (Types_ExpressionCompatible(leftType, rightType)) {
							if (IsInteger(leftExp) && IsInteger(rightExp)) {
								(yyval.node) = Trees_NewLeaf((Trees_Integer(leftExp) != Trees_Integer(rightExp))? TRUE: FALSE);
							}
						} else {
							error = 1;
						}
						break;
					case TREES_REAL_TYPE:
						if (Types_ExpressionCompatible(leftType, rightType)) {
							if (IsReal(leftExp) && IsReal(rightExp)) {
								(yyval.node) = Trees_NewLeaf((Trees_Real(leftExp) != Trees_Real(rightExp))? TRUE: FALSE);
							}
						} else {
							error = 1;
						}
						break;
					case TREES_BYTE_TYPE:
						error = ! Types_ExpressionCompatible(leftType, rightType);
						break;
					case TREES_SET_TYPE:
						if (Types_ExpressionCompatible(leftType, rightType)) {
							if (IsSet(leftExp) && IsSet(rightExp)) {
								(yyval.node) = Trees_NewLeaf((Trees_Set(leftExp) != Trees_Set(rightExp))? TRUE: FALSE);
							}
						} else {
							error = 1;
						}
						break;
					case TREES_STRING_TYPE:
						if (Types_ExpressionCompatible(leftType, rightType)) {
							if (Types_IsChar(rightType)) {
								leftExp = Trees_NewChar(Trees_String(leftExp)[0]);
							}
							if (IsString(leftExp) && IsString(rightExp)) {
								(yyval.node) = Trees_NewLeaf((strcmp(Trees_String(leftExp), Trees_String(rightExp)) != 0)? TRUE: FALSE);
							}
						} else {
							error = 1;
						}
						break;
					case ARRAY:
						error = ! Types_ExpressionCompatible(leftType, rightType);
						break;
					case POINTER:
						error = ! ((Trees_Symbol(rightExp) == NIL) || Types_Same(leftType, rightType));
						break;
					case PROCEDURE:
						error = ! ((Trees_Symbol(rightExp) == NIL) || Types_Same(leftType, rightType));
						break;
					case TREES_NIL_TYPE:
						error = ! (Types_IsPointer(rightType) || Types_IsProcedure(rightType));
						break;
					default:						
						error = 1;
				}
				break;
			case '<':
				switch (Trees_Symbol(Types_Structure(leftType))) {
					case TREES_CHAR_TYPE:
						if (Types_ExpressionCompatible(leftType, rightType)) {
							if (IsString(rightExp)) {
								rightExp = Trees_NewChar(Trees_String(rightExp)[0]);
							}
							if (IsChar(leftExp) && IsChar(rightExp)) {
								(yyval.node) = Trees_NewLeaf((Trees_Char(leftExp) < Trees_Char(rightExp))? TRUE: FALSE);
							}
						} else {
							error = 1;
						}
						break;
					case TREES_INTEGER_TYPE:	
						if (Types_ExpressionCompatible(leftType, rightType)) {
							if (IsInteger(leftExp) && IsInteger(rightExp)) {
								(yyval.node) = Trees_NewLeaf((Trees_Integer(leftExp) < Trees_Integer(rightExp))? TRUE: FALSE);
							}
						} else {
							error = 1;
						}
						break;
					case TREES_REAL_TYPE:
						if (Types_ExpressionCompatible(leftType, rightType)) {
							if (IsReal(leftExp) && IsReal(rightExp)) {
								(yyval.node) = Trees_NewLeaf((Trees_Real(leftExp) < Trees_Real(rightExp))? TRUE: FALSE);
							}
						} else {
							error = 1;
						}
						break;
					case TREES_BYTE_TYPE:
						error = ! Types_ExpressionCompatible(leftType, rightType);
						break;
					case TREES_STRING_TYPE:
						if (Types_ExpressionCompatible(leftType, rightType)) {
							if (Types_IsChar(rightType)) {
								leftExp = Trees_NewChar(Trees_String(leftExp)[0]);
							}
							if (IsString(leftExp) && IsString(rightExp)) {
								(yyval.node) = Trees_NewLeaf((strcmp(Trees_String(leftExp), Trees_String(rightExp)) < 0)? TRUE: FALSE);
							}
						} else {
							error = 1;
						}
						break;
					case ARRAY:
						error = ! Types_ExpressionCompatible(leftType, rightType);
						break;
					default:						
						error = 1;
				}
				break;
			case LE:
				switch (Trees_Symbol(Types_Structure(leftType))) {
					case TREES_CHAR_TYPE:
						if (Types_ExpressionCompatible(leftType, rightType)) {
							if (IsString(rightExp)) {
								rightExp = Trees_NewChar(Trees_String(rightExp)[0]);
							}
							if (IsChar(leftExp) && IsChar(rightExp)) {
								(yyval.node) = Trees_NewLeaf((Trees_Char(leftExp) <= Trees_Char(rightExp))? TRUE: FALSE);
							}
						} else {
							error = 1;
						}
						break;
					case TREES_INTEGER_TYPE:	
						if (Types_ExpressionCompatible(leftType, rightType)) {
							if (IsInteger(leftExp) && IsInteger(rightExp)) {
								(yyval.node) = Trees_NewLeaf((Trees_Integer(leftExp) <= Trees_Integer(rightExp))? TRUE: FALSE);
							}
						} else {
							error = 1;
						}
						break;
					case TREES_REAL_TYPE:
						if (Types_ExpressionCompatible(leftType, rightType)) {
							if (IsReal(leftExp) && IsReal(rightExp)) {
								(yyval.node) = Trees_NewLeaf((Trees_Real(leftExp) <= Trees_Real(rightExp))? TRUE: FALSE);
							}
						} else {
							error = 1;
						}
						break;
					case TREES_BYTE_TYPE:
						error = ! Types_ExpressionCompatible(leftType, rightType);
						break;
					case TREES_STRING_TYPE:
						if (Types_ExpressionCompatible(leftType, rightType)) {
							if (Types_IsChar(rightType)) {
								leftExp = Trees_NewChar(Trees_String(leftExp)[0]);
							}
							if (IsString(leftExp) && IsString(rightExp)) {
								(yyval.node) = Trees_NewLeaf((strcmp(Trees_String(leftExp), Trees_String(rightExp)) <= 0)? TRUE: FALSE);
							}
						} else {
							error = 1;
						}
						break;
					case ARRAY:
						error = ! Types_ExpressionCompatible(leftType, rightType);
						break;
					default:						
						error = 1;
				}
				break;
			case '>':
				switch (Trees_Symbol(Types_Structure(leftType))) {
					case TREES_CHAR_TYPE:
						if (Types_ExpressionCompatible(leftType, rightType)) {
							if (IsString(rightExp)) {
								rightExp = Trees_NewChar(Trees_String(rightExp)[0]);
							}
							if (IsChar(leftExp) && IsChar(rightExp)) {
								(yyval.node) = Trees_NewLeaf((Trees_Char(leftExp) > Trees_Char(rightExp))? TRUE: FALSE);
							}
						} else {
							error = 1;
						}
						break;
					case TREES_INTEGER_TYPE:	
						if (Types_ExpressionCompatible(leftType, rightType)) {
							if (IsInteger(leftExp) && IsInteger(rightExp)) {
								(yyval.node) = Trees_NewLeaf((Trees_Integer(leftExp) > Trees_Integer(rightExp))? TRUE: FALSE);
							}
						} else {
							error = 1;
						}
						break;
					case TREES_REAL_TYPE:
						if (Types_ExpressionCompatible(leftType, rightType)) {
							if (IsReal(leftExp) && IsReal(rightExp)) {
								(yyval.node) = Trees_NewLeaf((Trees_Real(leftExp) > Trees_Real(rightExp))? TRUE: FALSE);
							}
						} else {
							error = 1;
						}
						break;
					case TREES_BYTE_TYPE:
						error = ! Types_ExpressionCompatible(leftType, rightType);
						break;
					case TREES_STRING_TYPE:
						if (Types_ExpressionCompatible(leftType, rightType)) {
							if (Types_IsChar(rightType)) {
								leftExp = Trees_NewChar(Trees_String(leftExp)[0]);
							}
							if (IsString(leftExp) && IsString(rightExp)) {
								(yyval.node) = Trees_NewLeaf((strcmp(Trees_String(leftExp), Trees_String(rightExp)) > 0)? TRUE: FALSE);
							}
						} else {
							error = 1;
						}
						break;
					case ARRAY:
						error = ! Types_ExpressionCompatible(leftType, rightType);
						break;
					default:						
						error = 1;
				}
				break;
			case GE:
				switch (Trees_Symbol(Types_Structure(leftType))) {
					case TREES_CHAR_TYPE:
						if (Types_ExpressionCompatible(leftType, rightType)) {
							if (IsString(rightExp)) {
								rightExp = Trees_NewChar(Trees_String(rightExp)[0]);
							}
							if (IsChar(leftExp) && IsChar(rightExp)) {
								(yyval.node) = Trees_NewLeaf((Trees_Char(leftExp) >= Trees_Char(rightExp))? TRUE: FALSE);
							}
						} else {
							error = 1;
						}
						break;
					case TREES_INTEGER_TYPE:	
						if (Types_ExpressionCompatible(leftType, rightType)) {
							if (IsInteger(leftExp) && IsInteger(rightExp)) {
								(yyval.node) = Trees_NewLeaf((Trees_Integer(leftExp) >= Trees_Integer(rightExp))? TRUE: FALSE);
							}
						} else {
							error = 1;
						}
						break;
					case TREES_REAL_TYPE:
						if (Types_ExpressionCompatible(leftType, rightType)) {
							if (IsReal(leftExp) && IsReal(rightExp)) {
								(yyval.node) = Trees_NewLeaf((Trees_Real(leftExp) >= Trees_Real(rightExp))? TRUE: FALSE);
							}
						} else {
							error = 1;
						}
						break;
					case TREES_BYTE_TYPE:
						error = ! Types_ExpressionCompatible(leftType, rightType);
						break;
					case TREES_STRING_TYPE:
						if (Types_ExpressionCompatible(leftType, rightType)) {
							if (Types_IsChar(rightType)) {
								leftExp = Trees_NewChar(Trees_String(leftExp)[0]);
							}
							if (IsString(leftExp) && IsString(rightExp)) {
								(yyval.node) = Trees_NewLeaf((strcmp(Trees_String(leftExp), Trees_String(rightExp)) >= 0)? TRUE: FALSE);
							}
						} else {
							error = 1;
						}
						break;
					case ARRAY:
						error = ! Types_ExpressionCompatible(leftType, rightType);
						break;
					default:						
						error = 1;
				}
				break;
			case IN:
				if (Types_IsInteger(leftType) && Types_IsSet(rightType)) {
					if (IsInteger(leftExp) && IsSet(rightExp)) {
						if (Trees_Integer(leftExp) >= 0) {
							(yyval.node) = Trees_NewLeaf((((0x1u << Trees_Integer(leftExp)) & Trees_Set(rightExp)) != 0)? TRUE: FALSE);
						} else {
							yyerror("error: non-negative element expected");
							YYABORT;
						}
					}
				} else {
					error = 1;
				}
				break;
			case IS:
				{
					Trees_Node typeIdent;
					
					if (IsDesignator(leftExp) && HasDynamicType(leftExp)) {
						if (IsDesignator(rightExp)) {
							typeIdent = Trees_Left(rightExp);
							if (Trees_Kind(typeIdent) == TREES_TYPE_KIND) {
								if (Types_Extensible(rightType)){
									if (Types_Extends(leftType, typeIdent)) {
										rightExp = typeIdent;
									} else {
										yyerror("error: extended type expected in operation IS");
										YYABORT;
									}
								} else {
									yyerror("error: record or pointer type expected as right operand of IS");
									YYABORT;
								}
							} else {
								yyerror("error: type name expected as right operand of IS");
								YYABORT;
							}
						} else {
							yyerror("error: identifier expected as right operand of IS");
							YYABORT;
						}
					} else {
						yyerror("error: variable record parameter or pointer variable expected as left operand of IS");
						YYABORT;
					}
					break;
				}
			default:
				assert(0);
		}
		
		if (! error) {
			if ((yyval.node) == NULL) {
				(yyval.node) = Trees_NewNode(relation, leftExp, rightExp);
			}
			Trees_SetType(Trees_NewLeaf(TREES_BOOLEAN_TYPE), (yyval.node));
		} else {
			switch (relation) {
				case '=':
				case '#':
				case '<':
				case '>':
					yyerror("error: incompatible types in relation \"%c\"", relation);
					break;
				case LE:
					yyerror("error: incompatible types in relation <=");
					break;
				case GE:
					yyerror("error: incompatible types in relation >=");
					break;
				case IN:
					yyerror("error: incompatible types in relation IN");
					break;
				case IS:
					yyerror("error: incompatible types in relation IS");
					break;
				default:
					assert(0);
			}
			YYABORT;
		}
	}
#line 2537 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 1113 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = '=';
	}
#line 2545 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 1117 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = '#';
	}
#line 2553 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 1121 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = '<';
	}
#line 2561 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 1125 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = LE;
	}
#line 2569 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 1129 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = '>';
	}
#line 2577 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 1133 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = GE;
	}
#line 2585 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 1137 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = IN;
	}
#line 2593 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 1141 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = IS;
	}
#line 2601 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 1148 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node term;
		int sign;
		
		if ((yyvsp[-1].integer) >= 0) {
			CheckIsValue((yyvsp[0].node));
		}

		term = (yyvsp[0].node);
		sign = (yyvsp[-1].integer);
		
		(yyval.node) = NULL;
		switch (sign) {
			case '+':
				switch (TypeStructureSymbol(term)) {
					case TREES_INTEGER_TYPE:
					case TREES_REAL_TYPE:
					case TREES_BYTE_TYPE:
						break;
					default:
						yyerror("error: numeric operand expected in identity operation");
						YYABORT;
				}
				if ((yyval.node) == NULL) {
					(yyval.node) = Trees_NewNode('+', term, NULL);
				}
				break;
			case '-':
				switch (TypeStructureSymbol(term)) {
					case TREES_INTEGER_TYPE:
						if (IsInteger(term)) {
							Range_CheckIntDiff(0, Trees_Integer(term));
							(yyval.node) = Trees_NewInteger(-Trees_Integer(term));
						}
						break;
					case TREES_REAL_TYPE:
						if (IsReal(term)) {
							Range_CheckRealDiff(0.0, Trees_Real(term));
							(yyval.node) = Trees_NewReal(-Trees_Real(term));
						}
						break;
					case TREES_BYTE_TYPE:
						break;
					case TREES_SET_TYPE:
						if (IsSet(term)) {
							(yyval.node) = Trees_NewSet(~Trees_Set(term));
						}
						break;
					default:
						yyerror("error: numeric or set operand expected in inversion operation");
						YYABORT;
				}
				if ((yyval.node) == NULL) {
					(yyval.node) = Trees_NewNode('-', term, NULL);
				}
				break;
			default:
				(yyval.node) = term;
		}
		assert((yyval.node) != NULL);
		Trees_SetType(Trees_Type(term), (yyval.node));
	}
#line 2668 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 1211 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node leftExp, rightExp, leftType, rightType, resultType;
		int operator, leftSym, rightSym;

		CheckIsValue((yyvsp[-2].node));
		CheckIsValue((yyvsp[0].node));

		leftExp = (yyvsp[-2].node);
		rightExp = (yyvsp[0].node);
		operator = (yyvsp[-1].integer);
		leftSym = Trees_Symbol((yyvsp[-2].node));
		rightSym = Trees_Symbol((yyvsp[0].node));
		leftType = Trees_Type((yyvsp[-2].node));
		rightType = Trees_Type((yyvsp[0].node));
				
		/*check types and evaluate constant expressions*/
		(yyval.node) = NULL;
		resultType = NULL;
		if (Types_ExpressionCompatible(leftType, rightType)) {
			switch (operator) {
				case '+':
					switch (Trees_Symbol(Types_Structure(leftType))) {
						case TREES_INTEGER_TYPE:	
							if (IsInteger(leftExp) && IsInteger(rightExp)) {
								Range_CheckIntSum(Trees_Integer(leftExp), Trees_Integer(rightExp));
								(yyval.node) = Trees_NewInteger(Trees_Integer(leftExp) + Trees_Integer(rightExp));
							}
							break;
						case TREES_REAL_TYPE:	
							if (IsReal(leftExp) && IsReal(rightExp)) {
								Range_CheckRealSum(Trees_Real(leftExp), Trees_Real(rightExp));
								(yyval.node) = Trees_NewReal(Trees_Real(leftExp) + Trees_Real(rightExp));
							}
							break;
						case TREES_BYTE_TYPE:
							if (Types_IsInteger(rightType)) {
								resultType = rightType;
							}
							break;
						case TREES_SET_TYPE:	
							if (IsSet(leftExp) && IsSet(rightExp)) {
								(yyval.node) = Trees_NewSet(Trees_Set(leftExp) | Trees_Set(rightExp));
							}
							break;
						default:
							yyerror("error: numeric value or set expected in operation +");
							YYABORT;
					}
					break;
				case '-':
					switch (Trees_Symbol(Types_Structure(leftType))) {
						case TREES_INTEGER_TYPE:	
							if (IsInteger(leftExp) && IsInteger(rightExp)) {
								Range_CheckIntDiff(Trees_Integer(leftExp), Trees_Integer(rightExp));
								(yyval.node) = Trees_NewInteger(Trees_Integer(leftExp) - Trees_Integer(rightExp));
							}
							break;
						case TREES_REAL_TYPE:	
							if (IsReal(leftExp) && IsReal(rightExp)) {
								Range_CheckRealDiff(Trees_Real(leftExp), Trees_Real(rightExp));
								(yyval.node) = Trees_NewReal(Trees_Real(leftExp) - Trees_Real(rightExp));
							}
							break;
						case TREES_BYTE_TYPE:
							if (Types_IsInteger(rightType)) {
								resultType = rightType;
							}
							break;
						case TREES_SET_TYPE:	
							if (IsSet(leftExp) && IsSet(rightExp)) {
								(yyval.node) = Trees_NewSet(Trees_Set(leftExp) & ~Trees_Set(rightExp));
							}
							break;
						default:
							yyerror("error: numeric value or set expected in operation -");
							YYABORT;
					}
					break;
				case OR:
					if (IsBoolean(leftExp) && IsBoolean(rightType)) {
						(yyval.node) = Trees_NewLeaf(((leftSym == TRUE) || (rightSym == TRUE))? TRUE: FALSE);
					}
					break;
				default:
					assert(0);
			}
		} else {
			switch (operator) {
				case '+':
				case '-':
					yyerror("error: incompatible types in operation \"%c\"", operator);
					break;
				case OR:
					yyerror("error: incompatible types in operation OR");
					break;
				default:
					assert(0);
			}
			YYABORT;
		}
		
		if ((yyval.node) == NULL) {
			(yyval.node) = Trees_NewNode(operator, leftExp, rightExp);
		}
		if (resultType == NULL) {
			resultType = Trees_Type((yyvsp[-2].node));
		}
		Trees_SetType(resultType, (yyval.node));		
	}
#line 2782 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 1324 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = '+';
	}
#line 2790 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 1328 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = '-';
	}
#line 2798 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 1332 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = -1;
	}
#line 2806 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 1339 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = '+';
	}
#line 2814 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 1343 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = '-';
	}
#line 2822 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 1347 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = OR;
	}
#line 2830 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 1355 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node leftExp, rightExp, leftType, rightType, resultType;
		int operator, leftSym, rightSym;

		CheckIsValue((yyvsp[-2].node));
		CheckIsValue((yyvsp[0].node));

		leftExp = (yyvsp[-2].node);
		rightExp = (yyvsp[0].node);
		operator = (yyvsp[-1].integer);
		leftSym = Trees_Symbol((yyvsp[-2].node));
		rightSym = Trees_Symbol((yyvsp[0].node));
		leftType = Types_Structure(Trees_Type((yyvsp[-2].node)));
		rightType = Types_Structure(Trees_Type((yyvsp[0].node)));

		/*check types and evaluate constant expressions*/
		(yyval.node) = NULL;
		resultType = NULL;
		if (Types_ExpressionCompatible(leftType, rightType)) {
			switch (operator) {
				case '*':
					switch (Trees_Symbol(leftType)) {
						case TREES_INTEGER_TYPE:	
							if (IsInteger(leftExp) && IsInteger(rightExp)) {
								Range_CheckIntProd(Trees_Integer(leftExp), Trees_Integer(rightExp));
								(yyval.node) = Trees_NewInteger(Trees_Integer(leftExp) * Trees_Integer(rightExp));
							}
							break;
						case TREES_REAL_TYPE:	
							if (IsReal(leftExp) && IsReal(rightExp)) {
								Range_CheckRealProd(Trees_Real(leftExp), Trees_Real(rightExp));
								(yyval.node) = Trees_NewReal(Trees_Real(leftExp) * Trees_Real(rightExp));
							}
							break;
						case TREES_BYTE_TYPE:
							if (Types_IsInteger(rightType)) {
								resultType = rightType;
							}
							break;
						case TREES_SET_TYPE:	
							if (IsSet(leftExp) && IsSet(rightExp)) {
								(yyval.node) = Trees_NewSet(Trees_Set(leftExp) & Trees_Set(rightExp));
							}
							break;
						default:
							yyerror("error: numeric value or set expected in operation *");
							YYABORT;
					}
					break;
				case '/':
					switch (Trees_Symbol(leftType)) {
						case TREES_REAL_TYPE:	
							if (IsReal(leftExp) && IsReal(rightExp)) {
								if (Trees_Real(rightExp) != 0) {
									(yyval.node) = Trees_NewReal(Trees_Real(leftExp) / Trees_Real(rightExp));
								} else {
									yyerror("warning: division by zero");
								}
							}
							break;
						case TREES_SET_TYPE:	
							if (IsSet(leftExp) && IsSet(rightExp)) {
								(yyval.node) = Trees_NewSet(Trees_Set(leftExp) ^ Trees_Set(rightExp));
							}
							break;
						default:
							yyerror("error: real value or set expected in division");
							YYABORT;
					}
					break;
				case DIV: /*fall through*/
				case MOD:
					switch (Trees_Symbol(leftType)) {
						case TREES_INTEGER_TYPE:	
							if (IsInteger(leftExp) && IsInteger(rightExp)) {
								if (Trees_Integer(rightExp) > 0) {
									if (operator == DIV) {
										(yyval.node) = Trees_NewInteger(OBNC_DIV(Trees_Integer(leftExp), Trees_Integer(rightExp)));
									} else {
										(yyval.node) = Trees_NewInteger(OBNC_MOD(Trees_Integer(leftExp), Trees_Integer(rightExp)));
									}
								} else {
									yyerror("error: positive divisor expected");
									YYABORT;
								}
							}
							break;
						case TREES_BYTE_TYPE:
							if (Types_IsInteger(rightType)) {
								if (IsInteger(rightExp) && (Trees_Integer(rightExp) <= 0)) {
									yyerror("error: positive divisor expected");
									YYABORT;
								}
								resultType = rightType;
							}
							break;
						default:
							if (operator == DIV) {
								yyerror("error: integer expected in DIV operation");
							} else {
								yyerror("error: integer expected in modulo operation");
							}
							YYABORT;
					}
					break;
				case '&':
					if (IsBoolean(leftExp) && IsBoolean(rightExp)) {
						(yyval.node) = Trees_NewLeaf(((leftSym == TRUE) && (rightSym == TRUE))? TRUE: FALSE);
					}
					break;
				default:
					assert(0);
			}
		} else {
			switch (operator) {
				case '*':
				case '/':
				case '&':
					yyerror("error: incompatible types in operation \"%c\"", operator);
					break;
				case DIV:
					yyerror("error: incompatible types in operation DIV");
					break;
				case MOD:
					yyerror("error: incompatible types in operation MOD");
					break;
				default:
					assert(0);
			}
			YYABORT;
		}
		
		if ((yyval.node) == NULL) {
			(yyval.node) = Trees_NewNode(operator, leftExp, rightExp);
		}
		if (resultType == NULL) {
			resultType = Trees_Type((yyvsp[-2].node));
		}
		Trees_SetType(resultType, (yyval.node));		
	}
#line 2975 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 1499 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = '*';
	}
#line 2983 "y.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 1503 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = '/';
	}
#line 2991 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 1507 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = DIV;
	}
#line 2999 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 1511 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = MOD;
	}
#line 3007 "y.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 1515 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = '&';
	}
#line 3015 "y.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 1523 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewString((yyvsp[0].string));
	}
#line 3023 "y.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 1527 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewLeaf(NIL);
		Trees_SetType(Trees_NewLeaf(TREES_NIL_TYPE), (yyval.node));
	}
#line 3032 "y.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 1532 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewLeaf(TRUE);
		Trees_SetType(Trees_NewLeaf(TREES_BOOLEAN_TYPE), (yyval.node));
	}
#line 3041 "y.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 1537 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewLeaf(FALSE);
		Trees_SetType(Trees_NewLeaf(TREES_BOOLEAN_TYPE), (yyval.node));
	}
#line 3050 "y.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 1542 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = (yyvsp[0].node);
		Trees_SetType(Trees_NewLeaf(TREES_SET_TYPE), (yyval.node));
	}
#line 3059 "y.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 1548 "Oberon.y" /* yacc.c:1646  */
    {
		const int isFunctionCall = 1;
		Trees_Node designator, actualParameters, ident;
		
		if (Trees_Symbol((yyvsp[0].node)) == TREES_PROCEDURE_CALL) {
			designator = Trees_Left((yyvsp[0].node));
			actualParameters = Trees_Right((yyvsp[0].node));
			HandleProcedureCall(designator, actualParameters, isFunctionCall, &(yyval.node));
		} else {
			ident = Trees_Left((yyvsp[0].node));
			if (Trees_Kind(ident) == TREES_CONSTANT_KIND) {
				(yyval.node) = Trees_Value(ident);
			} else {
				(yyval.node) = (yyvsp[0].node);
			}
		}
	}
#line 3081 "y.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 1566 "Oberon.y" /* yacc.c:1646  */
    {
		CheckIsValue((yyvsp[-1].node));
		(yyval.node) = (yyvsp[-1].node);
	}
#line 3090 "y.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 1571 "Oberon.y" /* yacc.c:1646  */
    {
		CheckIsValue((yyvsp[0].node));
		if (Types_IsBoolean(Trees_Type((yyvsp[0].node)))) {
			switch (Trees_Symbol((yyvsp[0].node))) {
				case TRUE:
					(yyval.node) = Trees_NewLeaf(FALSE);
					break;
				case FALSE:
					(yyval.node) = Trees_NewLeaf(TRUE);
					break;
				default:
					(yyval.node) = Trees_NewNode('~', (yyvsp[0].node), NULL);
			}
			Trees_SetType(Trees_NewLeaf(TREES_BOOLEAN_TYPE), (yyval.node));
		} else {
			yyerror("error: boolean operand expected in negation");
			YYABORT;
		}
	}
#line 3114 "y.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 1595 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node designator, identType, actualParameters;
		int parameterListFound; /*possibly empty*/

		Trees_ReverseList(&(yyvsp[0].node)); /*correct order*/
		designator = Designator((yyvsp[-1].ident), (yyvsp[0].node));

		identType = Trees_Type(Trees_Left(designator));
		SetSelectorTypes(identType, designator, &parameterListFound);
		if (parameterListFound) {
			RemoveActualParameters(&designator, &actualParameters);
			(yyval.node) = Trees_NewNode(TREES_PROCEDURE_CALL, designator, actualParameters);
		} else {
			(yyval.node) = designator;
		}
	}
#line 3135 "y.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 1615 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node curr;
		
		if ((Trees_Symbol((yyvsp[0].node)) == '[') && (Trees_Right((yyvsp[0].node)) != NULL)) { /*multi-dimensional element selector*/
			/*attatch last element selector node to $1*/
			Trees_ReverseList(&(yyvsp[0].node));
			(yyval.node) = (yyvsp[-1].node);
			curr = (yyvsp[0].node);
			do {
				(yyval.node) = Trees_NewNode('[', Trees_Left(curr), (yyval.node));
				curr = Trees_Right(curr);
			} while (curr != NULL);
			Trees_ReverseList(&(yyval.node));
		} else {
			(yyval.node) = Trees_NewNode(Trees_Symbol((yyvsp[0].node)), Trees_Left((yyvsp[0].node)), (yyvsp[-1].node));
		}
	}
#line 3157 "y.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 1633 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = NULL;
	}
#line 3165 "y.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 1640 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node field;
		
		field = Trees_NewIdent((yyvsp[0].ident));
		Trees_SetKind(TREES_FIELD_KIND, field);
		(yyval.node) = Trees_NewNode('.', field, NULL);
	}
#line 3177 "y.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 1648 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node curr, exp;
		
		/*create one selector node per index*/
		(yyval.node) = NULL;
		curr = (yyvsp[-1].node); /*NOTE: ExpList is reversed*/
		do {
			exp = Trees_Left(curr);
			if (Types_IsInteger(Trees_Type(exp))) {
				(yyval.node) = Trees_NewNode('[', Trees_Left(curr), (yyval.node));
			} else {
				yyerror("error: integer array index expected");
				YYABORT;
			}
			curr = Trees_Right(curr);
		} while (curr != NULL);		
	}
#line 3199 "y.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 1666 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewNode('^', NULL, NULL);
	}
#line 3207 "y.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 1671 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_ReverseList(&(yyvsp[-1].node)); /*correct order*/
		(yyval.node) = Trees_NewNode('(', (yyvsp[-1].node), NULL);
	}
#line 3216 "y.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 1676 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewNode('(', NULL, NULL);
	}
#line 3224 "y.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 1683 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewSet(0x0u);
	}
#line 3232 "y.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 1687 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = (yyvsp[-1].node);
	}
#line 3240 "y.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 1695 "Oberon.y" /* yacc.c:1646  */
    {
		if ((Trees_Symbol((yyvsp[-2].node)) == TREES_SET_CONSTANT)
				&& (Trees_Symbol((yyvsp[0].node)) == TREES_SET_CONSTANT)) {
			(yyval.node) = Trees_NewSet(Trees_Set((yyvsp[-2].node)) | Trees_Set((yyvsp[0].node)));
		} else {
			(yyval.node) = Trees_NewNode('+', (yyvsp[-2].node), (yyvsp[0].node));
		}
	}
#line 3253 "y.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 1707 "Oberon.y" /* yacc.c:1646  */
    {
		int i;
		Trees_Node type;
		
		CheckIsValue((yyvsp[0].node));
		(yyval.node) = NULL;
		type = Trees_Type((yyvsp[0].node));
		if (IsInteger((yyvsp[0].node))) {
			i = Trees_Integer((yyvsp[0].node));
			Range_CheckSetElement(i);
			(yyval.node) = Trees_NewSet(1 << i);
		} else if (Types_IsInteger(type)) {
			(yyval.node) = Trees_NewNode(TREES_SINGLE_ELEMENT_SET, (yyvsp[0].node), NULL);
			Trees_SetType(Trees_NewLeaf(TREES_SET_TYPE), (yyval.node));
		} else {
			yyerror("error: element must have integer type");
			YYABORT;
		}
	}
#line 3277 "y.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 1727 "Oberon.y" /* yacc.c:1646  */
    {
		CheckIsValue((yyvsp[-2].node));
		CheckIsValue((yyvsp[0].node));
		(yyval.node) = NULL;
		if (IsInteger((yyvsp[-2].node))) {
			Range_CheckSetElement(Trees_Integer((yyvsp[-2].node)));
		}
		if (IsInteger((yyvsp[0].node))) {
			Range_CheckSetElement(Trees_Integer((yyvsp[0].node)));
		}
		if (IsInteger((yyvsp[-2].node)) && IsInteger((yyvsp[0].node))) {
			(yyval.node) = Trees_NewSet(OBNC_RANGE(Trees_Integer((yyvsp[-2].node)), Trees_Integer((yyvsp[0].node))));
		} else if (Types_IsInteger(Trees_Type((yyvsp[-2].node))) && Types_IsInteger(Trees_Type((yyvsp[0].node)))) {
			(yyval.node) = Trees_NewNode(TREES_RANGE_SET, (yyvsp[-2].node), (yyvsp[0].node));
			Trees_SetType(Trees_NewLeaf(TREES_SET_TYPE), (yyval.node));
		} else {
			yyerror("error: element must have integer type");
			YYABORT;
		}
	}
#line 3302 "y.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 1751 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewNode(TREES_EXP_LIST, (yyvsp[0].node), NULL);
		Trees_SetType(Trees_Type((yyvsp[0].node)), (yyval.node));
	}
#line 3311 "y.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 1756 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node reversedList;
		
		reversedList = Trees_NewNode(TREES_EXP_LIST, (yyvsp[0].node), (yyvsp[-2].node));
		(yyval.node) = reversedList;
		Trees_SetType(Trees_Type((yyvsp[0].node)), (yyval.node));
	}
#line 3323 "y.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 1777 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = NULL;
	}
#line 3331 "y.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 1784 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node designator, ident, designatorType, exp;
		
		CheckIsValue((yyvsp[0].node));
		switch (Trees_Symbol((yyvsp[-2].node))) {
			case TREES_DESIGNATOR:
				designator = (yyvsp[-2].node);
				exp = (yyvsp[0].node);
				ident = Trees_Left((yyvsp[-2].node));
				designatorType = Trees_Type((yyvsp[-2].node));
				switch (Trees_Kind(ident)) {
					case TREES_VARIABLE_KIND:
					case TREES_VALUE_PARAM_KIND:
					case TREES_VAR_PARAM_KIND:
						if (Writable((yyvsp[-2].node))) {
							ValidateAssignment(exp, designatorType, -1);
							if (Types_IsChar(designatorType) && IsString(exp)) {
								exp = Trees_NewChar(Trees_String(exp)[0]);
							}
						} else {
							yyerror("error: assignment to read-only variable");
							YYABORT;
						}
						break;
					default:
						yyerror("error: assignment to non-variable");
						YYABORT;
				}
				(yyval.node) = Trees_NewNode(BECOMES, designator, exp);
				break;
			case TREES_PROCEDURE_CALL:
				yyerror("error: unexpected procedure call in assignment target");
				YYABORT;
				break;
			default:
				assert(0);
		}
	}
#line 3374 "y.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 1827 "Oberon.y" /* yacc.c:1646  */
    {
		const int isFunctionCall = 0;
		Trees_Node designator, actualParameters;

		if (Trees_Symbol((yyvsp[0].node)) == TREES_PROCEDURE_CALL) {
			designator = Trees_Left((yyvsp[0].node));
			actualParameters = Trees_Right((yyvsp[0].node));
		} else {
			designator = (yyvsp[0].node);
			actualParameters = NULL;
		}
		HandleProcedureCall(designator, actualParameters, isFunctionCall, &(yyval.node));
		if ((yyval.node) == NULL) {
			YYABORT;
		}
	}
#line 3395 "y.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 1847 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_ReverseList(&(yyvsp[0].node)); /*correct order*/
		(yyval.node) = (yyvsp[0].node);
	}
#line 3404 "y.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 1855 "Oberon.y" /* yacc.c:1646  */
    {
		if ((yyvsp[0].node) == NULL) {
			(yyval.node) = NULL;
		} else {
			(yyval.node) = Trees_NewNode(TREES_STATEMENT_SEQUENCE, (yyvsp[0].node), NULL);
		}
	}
#line 3416 "y.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 1863 "Oberon.y" /* yacc.c:1646  */
    {
		if ((yyvsp[0].node) != NULL) {
			(yyval.node) = Trees_NewNode(TREES_STATEMENT_SEQUENCE, (yyvsp[0].node), (yyvsp[-2].node));
		} else {
			(yyval.node) = (yyvsp[-2].node);
		}
	}
#line 3428 "y.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 1874 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node currElsif, currExp, currThen, currStmt;
		
		if ((yyvsp[-2].node) == NULL) {
			(yyval.node) = Trees_NewNode(IF, (yyvsp[-5].node), Trees_NewNode(THEN, (yyvsp[-3].node), (yyvsp[-1].node)));
		} else {
			/*correct order of elsif nodes*/
			(yyval.node) = (yyvsp[-1].node);
			currElsif = (yyvsp[-2].node);
			do {
				currExp = Trees_Left(currElsif);
				currThen = Trees_Right(currElsif);
				currStmt = Trees_Left(currThen);
				(yyval.node) = Trees_NewNode(ELSIF, currExp, Trees_NewNode(THEN, currStmt, (yyval.node)));
				currElsif = Trees_Right(currThen);
			} while (currElsif != NULL);
			(yyval.node) = Trees_NewNode(IF, (yyvsp[-5].node), Trees_NewNode(THEN, (yyvsp[-3].node), (yyval.node)));
		}
	}
#line 3452 "y.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 1897 "Oberon.y" /* yacc.c:1646  */
    {
		CheckIsValue((yyvsp[0].node));
		if (Types_IsBoolean(Trees_Type((yyvsp[0].node)))) {
			(yyval.node) = (yyvsp[0].node);
		} else {
			yyerror("error: boolean expression expected");
			YYABORT;
		}		
	}
#line 3466 "y.tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 1910 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewNode(ELSIF, (yyvsp[-2].node), Trees_NewNode(THEN, (yyvsp[0].node), (yyvsp[-4].node)));
	}
#line 3474 "y.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 1914 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = NULL;
	}
#line 3482 "y.tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 1921 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewNode(ELSE, (yyvsp[0].node), NULL);
	}
#line 3490 "y.tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 1925 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = NULL;
	}
#line 3498 "y.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 1932 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node expType, caseVariable;
		
		if ((yyvsp[-1].node) != NULL) {
			Trees_ReverseList(&(yyvsp[-1].node)); /*correct order*/
		}
		expType = Trees_Type((yyvsp[-3].node));
		if (Types_IsRecord(expType) || Types_IsPointer(expType)) {
			/*reset original type*/
			caseVariable = Trees_Left((yyvsp[-3].node));
			Trees_SetType(caseExpressionType, caseVariable);
		}
		(yyval.node) = Trees_NewNode(CASE, (yyvsp[-3].node), (yyvsp[-1].node));
	}
#line 3517 "y.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 1950 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node typeStruct, caseVariable;

		CheckIsValue((yyvsp[0].node));
		typeStruct = Types_Structure(Trees_Type((yyvsp[0].node)));
		switch (Trees_Symbol(typeStruct)) {
			case RECORD:
				/*fall through*/
			case POINTER:
				if (IsDesignator((yyvsp[0].node)) && (Trees_Right((yyvsp[0].node)) == NULL)) {
					caseVariable = Trees_Left((yyvsp[0].node));
					if (! Types_IsRecord(typeStruct) || (Trees_Kind(caseVariable) == TREES_VAR_PARAM_KIND)) {
						(yyval.node) = (yyvsp[0].node);
					} else {
						yyerror("error: record CASE expression must be a variable parameter");
						YYABORT;
					}
				} else {
					yyerror("error: non-integral CASE expression must be a variable");
					YYABORT;
				}
				/*fall through*/
			case TREES_INTEGER_TYPE:
				/*fall through*/
			case TREES_CHAR_TYPE:
				currentCaseExpression = (yyvsp[0].node);
				caseExpressionType = Trees_Type((yyvsp[0].node));
				currentlyDefinedCaseLabels = NULL;
				(yyval.node) = (yyvsp[0].node);
				break;
			default:
				yyerror("error: invalid type of CASE expression");
				YYABORT;
		}
	}
#line 3557 "y.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 1989 "Oberon.y" /* yacc.c:1646  */
    {
		if ((yyvsp[0].node) != NULL) {
			(yyval.node) = Trees_NewNode(TREES_CASE_REP, (yyvsp[0].node), NULL);
		} else {
			(yyval.node) = NULL;	
		}
	}
#line 3569 "y.tab.c" /* yacc.c:1646  */
    break;

  case 114:
#line 1997 "Oberon.y" /* yacc.c:1646  */
    {
		if ((yyvsp[0].node) != NULL) {
			if ((yyvsp[-2].node) != NULL) {
				(yyval.node) = Trees_NewNode(TREES_CASE_REP, (yyvsp[0].node), (yyvsp[-2].node));
			} else {
				(yyval.node) = Trees_NewNode(TREES_CASE_REP, (yyvsp[0].node), NULL);
			}
		} else {
			(yyval.node) = NULL;		
		}
	}
#line 3585 "y.tab.c" /* yacc.c:1646  */
    break;

  case 115:
#line 2012 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_ReverseList(&(yyvsp[-2].node)); /*correct order*/
		(yyval.node) = Trees_NewNode(TREES_CASE, (yyvsp[-2].node), (yyvsp[0].node));
	}
#line 3594 "y.tab.c" /* yacc.c:1646  */
    break;

  case 116:
#line 2017 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = NULL;
	}
#line 3602 "y.tab.c" /* yacc.c:1646  */
    break;

  case 117:
#line 2024 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewNode(TREES_CASE_LABEL_LIST, (yyvsp[0].node), NULL); 
	}
#line 3610 "y.tab.c" /* yacc.c:1646  */
    break;

  case 118:
#line 2028 "Oberon.y" /* yacc.c:1646  */
    {
		switch (Trees_Symbol((yyvsp[0].node))) {
			case INTEGER:
			case TREES_CHAR_CONSTANT:
			case DOTDOT:
				(yyval.node) = Trees_NewNode(TREES_CASE_LABEL_LIST, (yyvsp[0].node), (yyvsp[-2].node));
				break;
			default:
				yyerror("error: unexpected list of type name labels");
				YYABORT;				
		}
	}
#line 3627 "y.tab.c" /* yacc.c:1646  */
    break;

  case 119:
#line 2044 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = (yyvsp[0].node);
		CheckCaseLabelUniqueness((yyvsp[0].node));
		currentlyDefinedCaseLabels = Trees_NewNode(TREES_NOSYM, (yyvsp[0].node), currentlyDefinedCaseLabels);
	}
#line 3637 "y.tab.c" /* yacc.c:1646  */
    break;

  case 120:
#line 2050 "Oberon.y" /* yacc.c:1646  */
    {
		const int rangeLenMax = 255;
		int leftSym, rightSym;
		int rangeMin, rangeMax;
		
		leftSym = Trees_Symbol((yyvsp[-2].node));
		rightSym = Trees_Symbol((yyvsp[0].node));
		if (leftSym == rightSym) {
			switch (leftSym) {
				case INTEGER:
					rangeMin = Trees_Integer((yyvsp[-2].node));
					rangeMax = Trees_Integer((yyvsp[0].node));
					if (rangeMin <= rangeMax) {
						if (rangeMax - rangeMin > rangeLenMax) {
							yyerror("error: maximum range length of %d exceeded", rangeLenMax);
							YYABORT;
						}
					} else {
						yyerror("error: left integer must be less than right integer in case range");
						YYABORT;
					}
					break;
				case TREES_CHAR_CONSTANT:
					if (Trees_Char((yyvsp[-2].node)) >= Trees_Char((yyvsp[0].node))) {
						yyerror("error: left string must be less than right string in case range");
						YYABORT;
					}
					break;
				default:
					yyerror("error: case label ranges must contain integers or single-character strings");
					YYABORT;
			}
		} else {
			yyerror("error: case labels in a range must have the same type");
			YYABORT;
		}
		(yyval.node) = Trees_NewNode(DOTDOT, (yyvsp[-2].node), (yyvsp[0].node));
		CheckCaseLabelUniqueness((yyval.node));
		currentlyDefinedCaseLabels = Trees_NewNode(TREES_NOSYM, (yyval.node), currentlyDefinedCaseLabels);
	}
#line 3682 "y.tab.c" /* yacc.c:1646  */
    break;

  case 121:
#line 2094 "Oberon.y" /* yacc.c:1646  */
    {
		if (Types_IsInteger(Trees_Type(currentCaseExpression))) {
			(yyval.node) = Trees_NewInteger((yyvsp[0].integer));
		} else {
			yyerror("error: unexpected integer label");
			YYABORT;
		}
	}
#line 3695 "y.tab.c" /* yacc.c:1646  */
    break;

  case 122:
#line 2103 "Oberon.y" /* yacc.c:1646  */
    {
		if (Types_IsChar(Trees_Type(currentCaseExpression))) {
			if (strlen((yyvsp[0].string)) <= 1) {
				(yyval.node) = Trees_NewChar((yyvsp[0].string)[0]);
			} else {
				yyerror("error: single-character string expected");
				YYABORT;
			}
		} else {
			yyerror("error: unexpected string label");
			YYABORT;
		}
	}
#line 3713 "y.tab.c" /* yacc.c:1646  */
    break;

  case 123:
#line 2117 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node caseExpTypeStruct, constValue, caseVariable;
				
		(yyval.node) = Table_At(Trees_Name((yyvsp[0].node)));
		if ((yyval.node) != NULL) {
			caseExpTypeStruct = Types_Structure(Trees_Type(currentCaseExpression));
			switch (Trees_Symbol(caseExpTypeStruct)) {
				case TREES_INTEGER_TYPE:
					constValue = Trees_Value((yyval.node));
					if (Trees_Symbol(constValue) == INTEGER) {
						if (Trees_Integer(constValue) >= 0) {
							(yyval.node) = constValue;
						} else {
							yyerror("error: non-negative case label expected");
							YYABORT;
						}
					} else {
						yyerror("error: integer label expected");
						YYABORT;					
					}
					break;
				case TREES_CHAR_TYPE:
					constValue = Trees_Value((yyval.node));
					if (Trees_Symbol(constValue) == STRING) {
						if (Types_StringLength(Trees_Type(constValue)) <= 1) {
							(yyval.node) = Trees_NewChar(Trees_String(constValue)[0]);
						} else {
							yyerror("error: single-character string expected");
							YYABORT;
						}
					} else {
						yyerror("error: character label expected");
						YYABORT;					
					}
					break;
				case RECORD:
					if (Types_IsRecord((yyval.node))) {
						if (Types_Extends(Trees_Type(currentCaseExpression), (yyval.node))) {
							caseVariable = Trees_Left(currentCaseExpression);
							Trees_SetType((yyval.node), caseVariable);
						} else {
							yyerror("error: extended type expected in label");
							YYABORT;				
						}
					} else {
						yyerror("error: record type label expected");
						YYABORT;
					}
					break;
				case POINTER:
					if (Types_IsPointer((yyval.node))) {
						if (Types_Extends(Trees_Type(currentCaseExpression), (yyval.node))) {
							caseVariable = Trees_Left(currentCaseExpression);
							Trees_SetType((yyval.node), caseVariable);
						} else {
							yyerror("error: extended type expected in label");
							YYABORT;						
						}
					} else {
						yyerror("error: pointer type label expected");
						YYABORT;
					}
					break;
				default:
					assert(0);
			}				
		} else {
			yyerror("error: undeclared identifier: %s", Trees_Name((yyvsp[0].node)));
			YYABORT;
		}
	}
#line 3789 "y.tab.c" /* yacc.c:1646  */
    break;

  case 124:
#line 2192 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewNode(WHILE, (yyvsp[-4].node), Trees_NewNode(DO, (yyvsp[-2].node), (yyvsp[-1].node)));
	}
#line 3797 "y.tab.c" /* yacc.c:1646  */
    break;

  case 125:
#line 2199 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewNode(ELSIF, (yyvsp[-2].node), Trees_NewNode(THEN, (yyvsp[0].node), (yyvsp[-4].node)));
	}
#line 3805 "y.tab.c" /* yacc.c:1646  */
    break;

  case 126:
#line 2203 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = NULL;
	}
#line 3813 "y.tab.c" /* yacc.c:1646  */
    break;

  case 127:
#line 2210 "Oberon.y" /* yacc.c:1646  */
    {
		CheckIsValue((yyvsp[0].node));
		(yyval.node) = NULL;
		if (Types_IsBoolean(Trees_Type((yyvsp[0].node)))) {
			(yyval.node) = Trees_NewNode(REPEAT, (yyvsp[-2].node), (yyvsp[0].node));
		} else {
			yyerror("error: boolean expression expected");
			YYABORT;
		}
	}
#line 3828 "y.tab.c" /* yacc.c:1646  */
    break;

  case 128:
#line 2225 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node byExp;
		
		if ((yyvsp[-3].node) != NULL) {
			byExp = (yyvsp[-3].node);
		} else {
			byExp = Trees_NewInteger(1);
		}
		(yyval.node) = Trees_NewNode(FOR,
			(yyvsp[-6].node), 
			Trees_NewNode(TO, 
				(yyvsp[-4].node),
				Trees_NewNode(BY, byExp, (yyvsp[-1].node))));
	}
#line 3847 "y.tab.c" /* yacc.c:1646  */
    break;

  case 129:
#line 2243 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node ctrlVar, ctrlVarType;

		CheckIsValue((yyvsp[0].node));
		ctrlVar = Table_At((yyvsp[-2].ident));
		if (ctrlVar != NULL) {
			ctrlVarType = Trees_Type(ctrlVar);
			if (Types_IsInteger(ctrlVarType)) {
				if (Types_IsInteger(Trees_Type((yyvsp[0].node)))) {
					(yyval.node) = Trees_NewNode(BECOMES, ctrlVar, (yyvsp[0].node));
				} else {
					yyerror("error: integer expression expected as initial value");
					YYABORT;
				}
			} else {
				yyerror("error: integer control variable expected: %s", (yyvsp[-2].ident));
				YYABORT;				
			}
		} else {
			yyerror("error: undeclared control variable: %s", (yyvsp[-2].ident));
			YYABORT;
		}	
	}
#line 3875 "y.tab.c" /* yacc.c:1646  */
    break;

  case 130:
#line 2269 "Oberon.y" /* yacc.c:1646  */
    {
		CheckIsValue((yyvsp[0].node));
		if (! Types_IsInteger(Trees_Type((yyvsp[0].node)))) {
			yyerror("error: integer expression expected as upper limit");
			YYABORT;
		}
	}
#line 3887 "y.tab.c" /* yacc.c:1646  */
    break;

  case 131:
#line 2280 "Oberon.y" /* yacc.c:1646  */
    {
		if (IsInteger((yyvsp[0].node))) {
			if (Trees_Integer((yyvsp[0].node)) != 0) {
				(yyval.node) = (yyvsp[0].node);
			} else {
				yyerror("error: non-zero increment expected");
				YYABORT;
			}
		} else {
			yyerror("error: integer increment expected");
			YYABORT;
		}
	}
#line 3905 "y.tab.c" /* yacc.c:1646  */
    break;

  case 132:
#line 2294 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = NULL;
	}
#line 3913 "y.tab.c" /* yacc.c:1646  */
    break;

  case 133:
#line 2304 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node procIdent, procType, resultType, procStatements, returnExp;
		const char *procName;
		
		procIdent = (yyvsp[-6].node);
		procName = Trees_Name(procIdent);
		procType = Trees_Type((yyvsp[-6].node));
		resultType = Types_ResultType(procType);
		procStatements = (yyvsp[-3].node);
		returnExp = (yyvsp[-2].node);
		
		if (strcmp(procName, (yyvsp[0].ident)) == 0) {
			if (resultType == NULL) {
				if (returnExp != NULL) {
					yyerror("error: unexpected return expression");
					YYABORT;
				}
			} else {
				if (returnExp != NULL) {
					CheckIsValue(returnExp);
					if (Types_AssignmentCompatible(returnExp, resultType)) {
						if (Types_IsChar(resultType) && (Trees_Symbol(returnExp) == STRING)) {
							returnExp = Trees_NewChar(Trees_String(returnExp)[0]);
						} 
					} else {
						yyerror("error: unexpected type of return expression");
						YYABORT;
					}
				} else {
					yyerror("error: return expression expected");
					YYABORT;
				}
			}
			if (procStatements != NULL) {
				Generate_ProcedureStatements(procStatements);
			}
			if (returnExp != NULL) {
				Generate_ReturnClause(returnExp);
			}
			if (procedureDeclarationStack != NULL) {
				procedureDeclarationStack = Trees_Right(procedureDeclarationStack);
			}
			Generate_ProcedureEnd(procIdent);
			Table_CloseScope();
		} else {
			yyerror("error: expected procedure name: %s", procName);
			YYABORT;
		}
	}
#line 3967 "y.tab.c" /* yacc.c:1646  */
    break;

  case 134:
#line 2357 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node paramList, param;

		(yyval.node) = NULL;
		Table_CloseScope();
		Trees_SetType((yyvsp[0].node), (yyvsp[-1].node));
		Table_OpenScope();
	
		/*reenter parameters in the symbol table*/
		paramList = Types_Parameters((yyvsp[0].node));
		while (paramList != NULL) {
			param = Trees_Left(paramList);
			Table_Put(param);
			paramList = Trees_Right(paramList);
		}

		procedureDeclarationStack = Trees_NewNode(TREES_NOSYM, (yyvsp[-1].node), procedureDeclarationStack);
		Generate_ProcedureHeading((yyvsp[-1].node));
		(yyval.node) = (yyvsp[-1].node);
	}
#line 3992 "y.tab.c" /* yacc.c:1646  */
    break;

  case 135:
#line 2381 "Oberon.y" /* yacc.c:1646  */
    {
		if (! (Trees_Exported((yyvsp[0].node)) && Trees_Local((yyvsp[0].node)))) {
			Trees_SetKind(TREES_PROCEDURE_KIND, (yyvsp[0].node));
			Table_Put((yyvsp[0].node));
			Table_OpenScope();
		} else {
			yyerror("error: cannot export local procedure: %s", Trees_Name((yyvsp[0].node)));
			YYABORT;
		}
		(yyval.node) = (yyvsp[0].node);
	}
#line 4008 "y.tab.c" /* yacc.c:1646  */
    break;

  case 136:
#line 2396 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = (yyvsp[0].node);
	}
#line 4016 "y.tab.c" /* yacc.c:1646  */
    break;

  case 137:
#line 2400 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = NULL;
	}
#line 4024 "y.tab.c" /* yacc.c:1646  */
    break;

  case 138:
#line 2407 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = (yyvsp[0].node);
	}
#line 4032 "y.tab.c" /* yacc.c:1646  */
    break;

  case 139:
#line 2411 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = NULL;
	}
#line 4040 "y.tab.c" /* yacc.c:1646  */
    break;

  case 145:
#line 2432 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node unresolvedPointerType, undeclaredBaseType;
		
		if (unresolvedPointerTypes != NULL) {
			unresolvedPointerType = Trees_Left(unresolvedPointerTypes);
			undeclaredBaseType = Types_PointerBaseType(unresolvedPointerType);
			yyerror("error: undeclared pointer base type: %s", Trees_Name(undeclaredBaseType));
			YYABORT;
		}
	}
#line 4055 "y.tab.c" /* yacc.c:1646  */
    break;

  case 146:
#line 2443 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = NULL;
	}
#line 4063 "y.tab.c" /* yacc.c:1646  */
    break;

  case 147:
#line 2450 "Oberon.y" /* yacc.c:1646  */
    {
		unresolvedPointerTypes = NULL;
	}
#line 4071 "y.tab.c" /* yacc.c:1646  */
    break;

  case 156:
#line 2477 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Types_NewProcedure((yyvsp[-2].node), (yyvsp[0].node));
	}
#line 4079 "y.tab.c" /* yacc.c:1646  */
    break;

  case 157:
#line 2484 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_ReverseList(&(yyvsp[0].node)); /*correct order*/
		(yyval.node) = (yyvsp[0].node);
	}
#line 4088 "y.tab.c" /* yacc.c:1646  */
    break;

  case 158:
#line 2489 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = NULL;
	}
#line 4096 "y.tab.c" /* yacc.c:1646  */
    break;

  case 159:
#line 2496 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = (yyvsp[0].node);
		Trees_ReverseList(&(yyval.node));
	}
#line 4105 "y.tab.c" /* yacc.c:1646  */
    break;

  case 160:
#line 2501 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node curr;

		/*make one list of the two lists*/
		(yyval.node) = (yyvsp[-2].node);
		curr = (yyvsp[0].node);
		do {
			(yyval.node) = Trees_NewNode(TREES_IDENT_LIST, Trees_Left(curr), (yyval.node));
			curr = Trees_Right(curr);
		} while (curr != NULL);
		/*$$ in reversed order*/
	}
#line 4122 "y.tab.c" /* yacc.c:1646  */
    break;

  case 161:
#line 2517 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = ResolvedType((yyvsp[0].node));
		if ((yyval.node) != NULL) {
			if (Trees_Symbol((yyval.node)) == IDENT) {
				if (Trees_Kind((yyval.node)) != TREES_TYPE_KIND) {
					yyerror("error: type name expected as result type: %s", Trees_Name((yyvsp[0].node)));
					YYABORT;
				}				
				if (! Types_Scalar((yyval.node))) {
					yyerror("error: scalar result type expected: %s", Trees_Name((yyvsp[0].node)));
					YYABORT;					
				}
			}
		} else {
			yyerror("error: undeclared identifier: %s", Trees_Name((yyvsp[0].node)));
			YYABORT;
		}
	}
#line 4145 "y.tab.c" /* yacc.c:1646  */
    break;

  case 162:
#line 2536 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = NULL;
	}
#line 4153 "y.tab.c" /* yacc.c:1646  */
    break;

  case 163:
#line 2543 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node curr, ident;
		
		Trees_ReverseList(&(yyvsp[-2].node)); /*correct order*/
		curr = (yyvsp[-2].node);
		do {
			ident = Trees_Left(curr);
			Trees_SetKind((yyvsp[-3].integer), ident);
			Trees_SetType((yyvsp[0].node), ident);
			Trees_SetLocal(ident);	
			if (! Table_LocallyDeclared(Trees_Name(ident))) {
				Table_Put(ident);
			} else {
				yyerror("error: redeclaration of identifier with the same name: %s", Trees_Name(ident));
				YYABORT;
			}
			curr = Trees_Right(curr);
		} while (curr != NULL);
		
		(yyval.node) = (yyvsp[-2].node);
	}
#line 4179 "y.tab.c" /* yacc.c:1646  */
    break;

  case 164:
#line 2568 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = TREES_VAR_PARAM_KIND;
	}
#line 4187 "y.tab.c" /* yacc.c:1646  */
    break;

  case 165:
#line 2572 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = TREES_VALUE_PARAM_KIND;
	}
#line 4195 "y.tab.c" /* yacc.c:1646  */
    break;

  case 166:
#line 2579 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewNode(TREES_IDENT_LIST, Trees_NewIdent((yyvsp[0].ident)), NULL);
	}
#line 4203 "y.tab.c" /* yacc.c:1646  */
    break;

  case 167:
#line 2583 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewNode(TREES_IDENT_LIST, Trees_NewIdent((yyvsp[0].ident)), (yyvsp[-2].node));
	}
#line 4211 "y.tab.c" /* yacc.c:1646  */
    break;

  case 168:
#line 2590 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = ResolvedType((yyvsp[0].node));
		if ((yyval.node) != NULL) {
			if ((yyvsp[-1].integer)) {
				(yyval.node) = Types_NewArray(NULL, (yyval.node));
			}
		} else {
			yyerror("error: undeclared identifier: %s", Trees_Name((yyvsp[0].node)));
			exit(EXIT_FAILURE);
		}
	}
#line 4227 "y.tab.c" /* yacc.c:1646  */
    break;

  case 169:
#line 2605 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = 1;
	}
#line 4235 "y.tab.c" /* yacc.c:1646  */
    break;

  case 170:
#line 2609 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = 0;
	}
#line 4243 "y.tab.c" /* yacc.c:1646  */
    break;

  case 171:
#line 2619 "Oberon.y" /* yacc.c:1646  */
    {
		static char symfilePath[PATH_MAX + 1];
		
		if (strcmp((yyvsp[-1].ident), Args_ModuleName()) == 0) {
			Generate_ModuleEnd();
			Generate_Close();
			
			sprintf(symfilePath, ".obnc/%s.sym", Args_ModuleName());
			if (Args_Mode() == ARGS_ENTRY_POINT_MODE) {
				if (Files_Exists(symfilePath)) {
					Files_Remove(symfilePath);
				}
			} else {
				ExportSymbolTable(symfilePath);
			}
			YYACCEPT;
		} else {
			yyerror("error: expected identifier %s", Args_ModuleName());
			YYABORT;
		}
	}
#line 4269 "y.tab.c" /* yacc.c:1646  */
    break;

  case 172:
#line 2645 "Oberon.y" /* yacc.c:1646  */
    {
		if (strcmp((yyvsp[0].ident), Args_ModuleName()) == 0) {
			if (Args_Mode() != ARGS_IMPORT_LIST_MODE) {
				Generate_ModuleHeading();
			}
		} else {
			yyerror("error: module name and filename do not match");
			YYABORT;
		}
	}
#line 4284 "y.tab.c" /* yacc.c:1646  */
    break;

  case 173:
#line 2659 "Oberon.y" /* yacc.c:1646  */
    {
		if (Args_Mode() == ARGS_IMPORT_LIST_MODE) {
			YYACCEPT;
		}
	}
#line 4294 "y.tab.c" /* yacc.c:1646  */
    break;

  case 174:
#line 2665 "Oberon.y" /* yacc.c:1646  */
    {
		if (Args_Mode() == ARGS_IMPORT_LIST_MODE) {
			YYACCEPT;
		}
	}
#line 4304 "y.tab.c" /* yacc.c:1646  */
    break;

  case 175:
#line 2674 "Oberon.y" /* yacc.c:1646  */
    {
		static char impfilePath[PATH_MAX + 1];
		Trees_Node moduleAndDirPath, module, p;
		FILE *impFile;
		const char *name;
		
		Trees_ReverseList(&(yyvsp[-1].node)); /*correct order*/
		if (Args_Mode() == ARGS_IMPORT_LIST_MODE) {
			while ((yyvsp[-1].node) != NULL) {
				moduleAndDirPath = Trees_Left((yyvsp[-1].node));
				module = Trees_Left(moduleAndDirPath);
				puts(Trees_Name(module));
				(yyvsp[-1].node) = Trees_Right((yyvsp[-1].node));
			}
		} else {
			sprintf(impfilePath, ".obnc/%s.imp", Args_ModuleName());
			if (Args_Mode() == ARGS_ENTRY_POINT_MODE) {
				if (Files_Exists(impfilePath)) {
					Files_Remove(impfilePath);
				}
			} else {
				impFile = Files_New(impfilePath);
				p = (yyvsp[-1].node);
				do {
					moduleAndDirPath = Trees_Left(p);
					module = Trees_Left(moduleAndDirPath);
					name = Trees_UnaliasedName(module);
					fprintf(impFile, "%s\n", name);
					p = Trees_Right(p);
				} while (p != NULL);
				Files_Close(impFile);
			}
			Generate_ImportList((yyvsp[-1].node));
		}
	}
#line 4344 "y.tab.c" /* yacc.c:1646  */
    break;

  case 176:
#line 2713 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewNode(TREES_NOSYM, (yyvsp[0].node), NULL);
	}
#line 4352 "y.tab.c" /* yacc.c:1646  */
    break;

  case 177:
#line 2717 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewNode(TREES_NOSYM, (yyvsp[0].node), (yyvsp[-2].node));
	}
#line 4360 "y.tab.c" /* yacc.c:1646  */
    break;

  case 178:
#line 2724 "Oberon.y" /* yacc.c:1646  */
    {
		static Maps_Map importedModules = NULL;
		static char symbolFileName[PATH_MAX + 1];
		const char *module, *qualifier;
		Trees_Node qualifierSym, moduleIdent;
		char *moduleDirPath;
		
		if (importedModules == NULL) {
			importedModules = Maps_New();
		}
		if ((yyvsp[0].ident) != NULL) {
			module = (yyvsp[0].ident);
			qualifier = (yyvsp[-1].ident);
		} else {
			module = (yyvsp[-1].ident);
			qualifier = (yyvsp[-1].ident);
		}
		(yyval.node) = NULL;
		if (strcmp(module, Args_ModuleName()) != 0) {
			if (! Maps_HasKey(module, importedModules)) {
				Maps_Put(module, NULL, &importedModules);
				qualifierSym = Table_At(qualifier);
				if ((qualifierSym == NULL) || (Trees_Kind(qualifierSym) != TREES_QUALIFIER_KIND)) {
					qualifierSym = Trees_NewIdent(qualifier);
					if ((yyvsp[0].ident) != NULL) {
						Trees_SetUnaliasedName(module, qualifierSym);
					}
					Trees_SetKind(TREES_QUALIFIER_KIND, qualifierSym);
					Table_Put(qualifierSym);
					
					moduleDirPath = ModuleDirPath(module);
					if (Args_Mode() != ARGS_IMPORT_LIST_MODE) {
						/*import identifiers into the symbol table*/
						sprintf(symbolFileName, "%s/.obnc/%s.sym", moduleDirPath, module);
						if (Files_Exists(symbolFileName)) {
							Table_Import(symbolFileName, module, qualifier);
						} else {
							yyerror("Symbol file not found for module %s", module);
							YYABORT;
						}
					}
					moduleIdent = Trees_NewIdent(module);
					Trees_SetKind(TREES_QUALIFIER_KIND, moduleIdent);
					(yyval.node) = Trees_NewNode(TREES_NOSYM, moduleIdent, Trees_NewString(moduleDirPath));
				} else {
					yyerror("error: qualifier already used: %s", qualifier);
					YYABORT;
				}
			} else {
				yyerror("error: module already imported: %s", module);
				YYABORT;
			}
		} else {
			yyerror("error: a module cannot import itself");
			YYABORT;
		}
	}
#line 4422 "y.tab.c" /* yacc.c:1646  */
    break;

  case 179:
#line 2785 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.ident) = (yyvsp[0].ident);
	}
#line 4430 "y.tab.c" /* yacc.c:1646  */
    break;

  case 180:
#line 2789 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.ident) = NULL;
	}
#line 4438 "y.tab.c" /* yacc.c:1646  */
    break;

  case 181:
#line 2796 "Oberon.y" /* yacc.c:1646  */
    {
		Generate_ModuleStatements((yyvsp[0].node));
	}
#line 4446 "y.tab.c" /* yacc.c:1646  */
    break;


#line 4450 "y.tab.c" /* yacc.c:1646  */
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

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

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
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

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
  return yyresult;
}
#line 2801 "Oberon.y" /* yacc.c:1906  */


void Oberon_Init(void)
{
	if (Args_Mode() != ARGS_IMPORT_LIST_MODE) {
		Generate_Open();
	}
	Table_Init();
}


/*accessor functions*/

static char *QualidentName(const char qualifier[], const char ident[])
{
	int resultLen;
	char *result;
	
	resultLen = strlen(qualifier) + strlen(".") + strlen(ident) + 1;
	NEW_ARRAY(result, resultLen);
	sprintf(result, "%s.%s", qualifier, ident);
	return result;
}


/*constant predicate functions*/

static int IsBoolean(Trees_Node node)
{
	return (Trees_Symbol(node) == TRUE) || (Trees_Symbol(node) == FALSE);
}


static int IsChar(Trees_Node node)
{
	return Trees_Symbol(node) == TREES_CHAR_CONSTANT;
}


static int IsInteger(Trees_Node node)
{
	return Trees_Symbol(node) == INTEGER;
}


static int IsReal(Trees_Node node)
{
	return Trees_Symbol(node) == REAL;
}


static int IsString(Trees_Node node)
{
	return Trees_Symbol(node) == STRING;
}


static int IsSet(Trees_Node node)
{
	return Trees_Symbol(node) == TREES_SET_CONSTANT;
}


/*functions for type declaration productions*/

static Trees_Node ResolvedType(Trees_Node type)
{
	Trees_Node result, identDef, typeStruct;
	const char *name;
	
	result = NULL;
	if (Trees_Symbol(type) == IDENT) {
		name = Trees_Name(type);
		identDef = Table_At(name);
		if (identDef != NULL) {
			if (Trees_Kind(identDef) == TREES_TYPE_KIND) {
				typeStruct = Types_Structure(identDef);
				if (typeStruct != NULL) {
					if (Types_Basic(typeStruct)) {
						result = typeStruct;
					} else {
						result = identDef;
					}
				} else {
					yyerror("error: unresolved type: %s", name);
					exit(EXIT_FAILURE);
				}
			} else {
				yyerror("error: type expected: %s", name);
				exit(EXIT_FAILURE);
			}
		}
	} else {
		result = type;
	}
	return result;
}


static void ResolvePointerTypes(Trees_Node baseType)
{
	const char *baseTypeName;
	Trees_Node prev, curr, currPointerType, currBaseType;
	
	assert(Trees_Symbol(baseType) == IDENT);	
	baseTypeName = Trees_Name(baseType);

	prev = NULL;
	curr = unresolvedPointerTypes;
	while (curr != NULL) {
		currPointerType = Trees_Left(curr);
		currBaseType = Types_PointerBaseType(currPointerType);
		if (strcmp(Trees_Name(currBaseType), baseTypeName) == 0) {
			if (Types_IsRecord(baseType)) {
				/*update pointer base type*/
				Types_SetPointerBaseType(baseType, currPointerType);
				/*delete current node*/
				if (curr == unresolvedPointerTypes) {
					unresolvedPointerTypes = Trees_Right(curr);
				} else {
					Trees_SetRight(Trees_Right(curr), prev);
				}
			} else {
				yyerror("error: record type expected in declaration of pointer base type: %s", baseTypeName);
				exit(EXIT_FAILURE);
			}			
		}
		prev = curr;
		curr = Trees_Right(curr);
	}	
}


/*functions for expression productions*/

static int IsDesignator(Trees_Node exp)
{
	return Trees_Symbol(exp) == TREES_DESIGNATOR;
}


static void CheckIsValue(Trees_Node exp)
{
	Trees_Node ident;
	
	if (Trees_Symbol(exp) == TREES_DESIGNATOR) {
		ident = Trees_Left(exp);
		switch (Trees_Kind(ident)) {
			case TREES_CONSTANT_KIND:
			case TREES_FIELD_KIND:
			case TREES_VARIABLE_KIND:
			case TREES_PROCEDURE_KIND:
			case TREES_VALUE_PARAM_KIND:
			case TREES_VAR_PARAM_KIND:
				break;
			default:
				yyerror("error: value expected: %s", Trees_Name(ident));
				exit(EXIT_FAILURE);
		}
	}
}


static Trees_Node Designator(const char identName[], Trees_Node selectorList)
{
	Trees_Node identSym, qualidentSym, designator, qualidentSelectorList;
	const char *qualidentName;

	/*set qualident name, symbol and selector list*/
	qualidentSym = NULL;
	qualidentSelectorList = NULL;
	if ((procedureDeclarationStack != NULL)
			&& (strcmp(identName, Trees_Name(Trees_Left(procedureDeclarationStack))) == 0)) {
		qualidentSym = Trees_Left(procedureDeclarationStack);
		qualidentSelectorList = selectorList;			
	} else {
		identSym = Table_At(identName);
		if (identSym != NULL) {
			if (Trees_Kind(identSym) == TREES_QUALIFIER_KIND) {
				if ((selectorList != NULL) && (Trees_Symbol(selectorList) == '.')) {
					qualidentName = QualidentName(identName, Trees_Name(Trees_Left(selectorList)));
					qualidentSym = Table_At(qualidentName);
					qualidentSelectorList = Trees_Right(selectorList);
					if (qualidentSym == NULL) {
						yyerror("error: undeclared identifier: %s", qualidentName);
						exit(EXIT_FAILURE);
					}
				} else {
					yyerror("error: '.' expected after qualifier: %s", identName);
					exit(EXIT_FAILURE);
				}
			} else {
				qualidentSym = identSym;
				qualidentSelectorList = selectorList;
			}

		} else {
			yyerror("error: undeclared identifier: %s", identName);
			exit(EXIT_FAILURE);
		}
	}
	assert(qualidentSym != NULL);

	designator = Trees_NewNode(TREES_DESIGNATOR, qualidentSym, qualidentSelectorList);

	return designator;
}


static int HasDynamicType(Trees_Node designator)
{
	Trees_Node var, firstSelector, type;
	
	assert(Trees_Symbol(designator) == TREES_DESIGNATOR);

	var = Trees_Left(designator);
	firstSelector = Trees_Right(designator);
	type = Trees_Type(designator);

	return ((Trees_Kind(var) == TREES_VAR_PARAM_KIND)
			&& (firstSelector == NULL)
			&& Types_IsRecord(type))
		|| Types_IsPointer(type);
}


static void SetSelectorTypes(Trees_Node identType, Trees_Node designator, int *parameterListFound)
{
	Trees_Node currType, currTypeStruct, currSelector, prevSelector, indexExp, lengthNode, pointerNode, expList, extendedType, symbol, varField, typeField, fieldBaseType;
	int length, index;
	const char *fieldName;

	currType = identType;
	currSelector = Trees_Right(designator);
	prevSelector = designator;
	*parameterListFound = 0;
	while ((currSelector != NULL) && ! *parameterListFound) {
		currTypeStruct = Types_Structure(currType);
		switch (Trees_Symbol(currSelector)) {
			case '[':
				if ((currTypeStruct != NULL) && (Trees_Symbol(currTypeStruct) == ARRAY)) {
					indexExp = Trees_Left(currSelector);
					lengthNode = Types_ArrayLength(currTypeStruct);
					if ((lengthNode != NULL) && (Trees_Symbol(indexExp) == INTEGER)) {
						length = Trees_Integer(lengthNode);
						index = Trees_Integer(indexExp);
						if ((index < 0) || (index >= length)) {
							yyerror("error: invalid array index: %d not between 0 and %d", index, length - 1);
							exit(EXIT_FAILURE);
						}
					}
					Trees_SetType(currType, currSelector);
					currType = Types_ElementType(currTypeStruct);
				} else {
					yyerror("error: array variable expected in element selector");
					exit(EXIT_FAILURE);
				}
				break;
			case '.':
				if (currType != NULL) {
					switch (Trees_Symbol(currTypeStruct)) {
						case POINTER:
							pointerNode = Trees_NewNode('^', NULL, currSelector);
							Trees_SetType(currType, pointerNode);
							Trees_SetRight(pointerNode, prevSelector);
							currType = Types_PointerBaseType(currTypeStruct);
							/*fall through*/
						case RECORD:
							Trees_SetType(currType, currSelector);
							varField = Trees_Left(currSelector);
							fieldName = Trees_Name(varField);
							Types_GetFieldIdent(fieldName, currType, Trees_Imported(Trees_Left(designator)), &typeField, &fieldBaseType);
							if (typeField != NULL) {
								if (Trees_Exported(typeField)) {
									Trees_SetExported(varField);
								}
								currType = Trees_Type(typeField);
							} else {
								yyerror("error: undeclared field: %s", fieldName);
								exit(EXIT_FAILURE);
							}
							break;
						default:
							yyerror("error: record variable expected in field selector");
							exit(EXIT_FAILURE);
					}
				} else {						
					yyerror("error: record variable expected in field selector");
					exit(EXIT_FAILURE);
				}
				break;
			case '^':
				if ((currType != NULL) && (Trees_Symbol(currTypeStruct) == POINTER)) {
					Trees_SetType(currType, currSelector);
					currType = Types_PointerBaseType(currTypeStruct);
				} else {
					yyerror("error: pointer variable expected in pointer dereference");
					exit(EXIT_FAILURE);
				}
				break;
			case '(':
				if (Types_IsProcedure(currTypeStruct)) {
					*parameterListFound = 1;
				} else if (Types_IsRecord(currTypeStruct) || Types_IsPointer(currTypeStruct)) {
					/*type guard*/
					expList = Trees_Left(currSelector);
					if (Trees_Right(expList) == NULL) {
						if ((Trees_Symbol(Trees_Left(expList)) == TREES_DESIGNATOR)
								&& (Trees_Right(Trees_Left(expList)) == NULL)) {
							extendedType = Trees_Left(Trees_Left(expList));
							symbol = Table_At(Trees_Name(extendedType));
							if (symbol != NULL) {
								if (Trees_Kind(symbol) == TREES_TYPE_KIND) {
									if ((Types_IsRecord(currType) && Types_IsRecord(Trees_Type(symbol)))
											|| (Types_IsPointer(currType) && Types_IsPointer(Trees_Type(symbol)))) {
										if (Types_Extends(currType, Trees_Type(symbol))) {
											Trees_SetLeft(extendedType, currSelector);
											Trees_SetType(extendedType, currSelector);
											currType = extendedType;
										} else {
											yyerror("error: extended type expected: %s", Trees_Name(extendedType));
											exit(EXIT_FAILURE);								
										}
									} else {
										if (Types_IsRecord(currType)) {
											yyerror("error: record type expected in type guard: %s", Trees_Name(extendedType));
										} else {
											yyerror("error: pointer type expected in type guard: %s", Trees_Name(extendedType));
										}
										exit(EXIT_FAILURE);										
									}
								} else {
									yyerror("error: type name expected: %s", Trees_Name(extendedType));
									exit(EXIT_FAILURE);
								}
							} else {
								yyerror("error: undeclared identifier: %s", Trees_Name(extendedType));
								exit(EXIT_FAILURE);
							}
						} else {
							yyerror("error: identifier expected in type guard");
							exit(EXIT_FAILURE);
						}
					} else {
						yyerror("error: unexpected comma in type guard");
						exit(EXIT_FAILURE);
					}
				} else {
					yyerror("error: unexpected parenthesis in designator which is not a record, pointer or procedure");
					exit(EXIT_FAILURE);
				}
				break;
			default:
				assert(0);
		}
		prevSelector = currSelector;
		currSelector = Trees_Right(currSelector);
	}

	if (currSelector == NULL) {
		Trees_SetType(currType, designator);
	} else {
		yyerror("error: unexpected selector after procedure call");
		exit(EXIT_FAILURE);
	}
}


static void RemoveActualParameters(Trees_Node *designator, Trees_Node *actualParameters)
{
	Trees_Node currSelector;
	
	currSelector = Trees_Right(*designator);
	assert(currSelector != NULL);
	if (Trees_Right(currSelector) == NULL) {
		*actualParameters = Trees_Left(currSelector);
		Trees_SetRight(NULL, *designator);
	} else {
		while (Trees_Right(Trees_Right(currSelector)) != NULL) {
			currSelector = Trees_Right(currSelector);
		}
		*actualParameters = Trees_Left(Trees_Right(currSelector));
		Trees_SetRight(NULL, currSelector);
	}
}


static int TypeStructureSymbol(Trees_Node exp)
{
	return Trees_Symbol(Types_Structure(Trees_Type(exp)));
}


/*functions for statement productions*/

static int Writable(Trees_Node designator)
{
	Trees_Node ident, type;
	int kind, result;
	
	assert(IsDesignator(designator));

	ident = Trees_Left(designator);
	kind = Trees_Kind(ident);
	type = Trees_Type(ident);
	result = ((kind == TREES_VARIABLE_KIND) && ! Trees_Imported(ident)) 
		|| (kind == TREES_VAR_PARAM_KIND)
		|| ((kind == TREES_VALUE_PARAM_KIND) && ! Types_IsArray(type) && ! Types_IsRecord(type));
	return result;
}


static char *AssignmentErrorContext(int paramPos)
{
	char *result;
	
	NEW_ARRAY(result, 64);
	if (paramPos >= 0) {
		sprintf(result, "in substitution of parameter %d", paramPos + 1);
	} else {
		strcpy(result, "in assignment");
	}
	return result;
}


static void ValidateAssignment(Trees_Node expression, Trees_Node targetType, int paramPos)
{
	Trees_Node targetTypeStruct;

	assert(expression != NULL);
	assert(targetType != NULL);
	assert(paramPos >= -1);

	if (Types_AssignmentCompatible(expression, targetType)) {
		if (Types_IsByte(targetType) && IsInteger(expression)) {
			Range_CheckByte(Trees_Integer(expression));
		}
	} else {
		targetTypeStruct = Types_Structure(targetType);
		switch(Trees_Symbol(targetTypeStruct)) {
			case TREES_BOOLEAN_TYPE:
				yyerror("error: boolean expression expected %s", AssignmentErrorContext(paramPos));
				exit(EXIT_FAILURE);
				break;		
			case TREES_CHAR_TYPE:
				yyerror("error: character expression or single-character string expected %s", AssignmentErrorContext(paramPos));
				exit(EXIT_FAILURE);
				break;
			case TREES_INTEGER_TYPE:
				yyerror("error: integer expression expected %s", AssignmentErrorContext(paramPos));
				exit(EXIT_FAILURE);
				break;		
			case TREES_REAL_TYPE:
				yyerror("error: real-valued expression expected %s", AssignmentErrorContext(paramPos));
				exit(EXIT_FAILURE);
				break;		
			case TREES_BYTE_TYPE:
				yyerror("error: integer expression expected %s", AssignmentErrorContext(paramPos));
				exit(EXIT_FAILURE);
				break;
			case TREES_SET_TYPE:
				yyerror("error: set expression expected %s", AssignmentErrorContext(paramPos));
				exit(EXIT_FAILURE);
				break;
			case ARRAY:
				if (Types_IsCharacterArray(targetTypeStruct) && IsString(expression)) {
					yyerror("error: destination array is to small %s", AssignmentErrorContext(paramPos));
				} else {
					yyerror("error: incompatible types %s", AssignmentErrorContext(paramPos));
				}
				exit(EXIT_FAILURE);
				break;
			case RECORD:
				yyerror("error: record of extended type expected as source %s", AssignmentErrorContext(paramPos));
				exit(EXIT_FAILURE);
				break;
			case POINTER:
				yyerror("error: pointer of extended type expected as source %s", AssignmentErrorContext(paramPos));
				exit(EXIT_FAILURE);
				break;
			case PROCEDURE:
				yyerror("error: globally declared procedure with matching type signature expected %s", AssignmentErrorContext(paramPos));
				exit(EXIT_FAILURE);
				break;
			default:
				assert(0);
				break;
		}
	}
}


static void ValidateActualParameter(Trees_Node exp, Trees_Node formalParam, int paramPos)
{
	int kind;
	Trees_Node formalType, formalElementType, expType, expElementType;
	
	kind = Trees_Kind(formalParam);
	assert((kind == TREES_VALUE_PARAM_KIND) || (kind == TREES_VAR_PARAM_KIND));
	
	if ((kind == TREES_VALUE_PARAM_KIND) || (IsDesignator(exp) && Writable(exp))) {
		formalType = Trees_Type(formalParam);
		expType = Trees_Type(exp);
		if (Types_IsOpenArray(formalType)) {
			formalElementType = Types_ElementType(formalType);
			if (Types_IsArray(expType)) {
				expElementType = Types_ElementType(expType);
				if (Types_Same(expElementType, formalElementType)) {
					/*done*/
				} else {
					yyerror("error: unexpected element type of actual parameter");
					exit(EXIT_FAILURE);
				}
			} else if (Types_IsString(expType) && Types_IsChar(formalElementType)) {
				/*done*/
			} else {
				if (Types_IsChar(formalElementType)) {
					yyerror("error: character array or string parameter expected");
				} else {
					yyerror("error: array parameter expected");
				}
				exit(EXIT_FAILURE);
			}
		} else if ((kind == TREES_VAR_PARAM_KIND) && Types_IsPointer(formalType)) {
			if (! Types_Same(expType, formalType)) {
				yyerror("error: parameter of type %s expected", Trees_Name(formalType));
				exit(EXIT_FAILURE);
			}
		} else if ((kind == TREES_VAR_PARAM_KIND) && (Trees_Symbol(expType) == RECORD)) {
			assert(Types_RecordBaseType(expType) != NULL);
			yyerror("error: variable parameter requires actual parameter of named record type: %s", Trees_Name(formalType));
			exit(EXIT_FAILURE);
		} else {
			ValidateAssignment(exp, formalType, paramPos);
		}
	} else {
		yyerror("error: writable parameter expected");
		exit(EXIT_FAILURE);
	}
}


static void ValidateProcedureCall(Trees_Node expList, Trees_Node fpList)
{
	Trees_Node exp, formalParam, fpType;
	int pos;
		
	pos = 0;
	while ((expList != NULL) && (fpList != NULL)) {
		exp = Trees_Left(expList);
		CheckIsValue(exp);
		formalParam = Trees_Left(fpList);
		fpType = Trees_Type(formalParam);
		ValidateActualParameter(exp, formalParam, pos);

		if (Types_IsChar(fpType) && (Trees_Symbol(exp) == STRING)) {
			Trees_SetLeft(Trees_NewChar(Trees_String(exp)[0]), expList);
		}
		expList = Trees_Right(expList);
		fpList = Trees_Right(fpList);
		pos++;
	}
	if ((expList == NULL) && (fpList != NULL)) {
		yyerror("error: too few actual parameters");
		exit(EXIT_FAILURE);
	} else if ((expList != NULL) && (fpList == NULL)) {
		yyerror("error: too many actual parameters");
		exit(EXIT_FAILURE);
	}
}


static Trees_Node PredeclaredProcedureAST(const char procName[], Trees_Node expList, int isFunctionCall)
{	
	static const struct { const char *name; int symbol; } symbols[] = {
		{"ABS", TREES_ABS_PROC},
		{"ASR", TREES_ASR_PROC},
		{"ASSERT", TREES_ASSERT_PROC},
		{"CHR", TREES_CHR_PROC},
		{"DEC", TREES_DEC_PROC},
		{"EXCL", TREES_EXCL_PROC},
		{"FLOOR", TREES_FLOOR_PROC},
		{"FLT", TREES_FLT_PROC},
		{"INC", TREES_INC_PROC},
		{"INCL", TREES_INCL_PROC},
		{"LEN", TREES_LEN_PROC},
		{"LSL", TREES_LSL_PROC},
		{"NEW", TREES_NEW_PROC},
		{"ODD", TREES_ODD_PROC},
		{"ORD", TREES_ORD_PROC},
		{"PACK", TREES_PACK_PROC},
		{"ROR", TREES_ROR_PROC},
		{"UNPK", TREES_UNPK_PROC}};

	int paramCount, pos, symbol;
	Trees_Node curr, resultType, result;
	Trees_Node param[2], paramTypes[2];

	/*set actual parameters*/
	paramCount = 0;
	curr = expList;
	while ((paramCount < LEN(param)) && (curr != NULL)) {
		param[paramCount] = Trees_Left(curr);
		paramTypes[paramCount] = Trees_Type(Trees_Left(curr));
		paramCount++;
		curr = Trees_Right(curr);
	}

	/*find procedure symbol*/
	pos = 0;
	while ((pos < LEN(symbols)) && (strcmp(symbols[pos].name, procName) != 0)) {
		pos++;
	}
	assert(pos < LEN(symbols));
	symbol = symbols[pos].symbol;
	
	/*validate parameters and build syntax tree*/
	result = NULL;
	resultType = NULL;
	switch (symbol) {
		case TREES_ABS_PROC:
			if (isFunctionCall) {
				if (paramCount == 1) {
					switch (Trees_Symbol(Types_Structure(paramTypes[0]))) {
						case TREES_INTEGER_TYPE:
							if (IsInteger(param[0])) {
								result = Trees_NewInteger(OBNC_ABS_INT(Trees_Integer(param[0])));
							}
							break;
						case TREES_REAL_TYPE:
							if (IsReal(param[0])) {
								result = Trees_NewReal(OBNC_ABS_FLT(Trees_Real(param[0])));
							}
							break;
						case TREES_BYTE_TYPE:
							/*do nothing*/
							break;
						default:
							yyerror("error: numeric parameter expected: %s", procName);
							exit(EXIT_FAILURE);
					}
					resultType = paramTypes[0];
				} else {
					yyerror("error: one parameter expected: %s", procName);
					exit(EXIT_FAILURE);
				}			
			} else {
				yyerror("error: proper procedure expected: %s", procName);
				exit(EXIT_FAILURE);				
			}
			break;
		case TREES_ODD_PROC:			
			if (isFunctionCall) {
				if (paramCount == 1) {
					if (Types_IsInteger(paramTypes[0])) {
						if (IsInteger(param[0])) {
							result = Trees_NewInteger(OBNC_ODD(Trees_Integer(param[0])));
						}
						resultType = Trees_NewLeaf(TREES_BOOLEAN_TYPE);
					} else {
						yyerror("error: integer parameter expected: %s", procName);
						exit(EXIT_FAILURE);
					}
				} else {
					yyerror("error: one parameter expected: %s", procName);
					exit(EXIT_FAILURE);
				}			
			} else {
				yyerror("error: proper procedure expected: %s", procName);
				exit(EXIT_FAILURE);				
			}
			break;
		case TREES_LEN_PROC:			
			if (isFunctionCall) {
				if (paramCount == 1) {
					if (Types_IsArray(paramTypes[0])) {
						if (! Types_IsOpenArray(paramTypes[0])) {
							result = Types_ArrayLength(paramTypes[0]);
						}
						resultType = Trees_NewLeaf(TREES_INTEGER_TYPE);
					} else {
						yyerror("error: array parameter expected: %s", procName);
						exit(EXIT_FAILURE);
					}
				} else {
					yyerror("error: one parameter expected: %s", procName);
					exit(EXIT_FAILURE);
				}			
			} else {
				yyerror("error: proper procedure expected: %s", procName);
				exit(EXIT_FAILURE);				
			}
			break;
		case TREES_LSL_PROC: /*fall through*/
		case TREES_ASR_PROC: /*fall through*/
		case TREES_ROR_PROC:
			if (isFunctionCall) {
				if (paramCount == 2) {
					if (Types_IsInteger(paramTypes[0])) {
						if (Types_IsInteger(paramTypes[1])) {
							if (IsInteger(param[1])) {
								switch (symbol) {
									case TREES_LSL_PROC:
										Range_CheckLSL(Trees_Integer(param[1]));
										break;
									case TREES_ASR_PROC:
										Range_CheckASR(Trees_Integer(param[1]));
										break;
									case TREES_ROR_PROC:
										Range_CheckROR(Trees_Integer(param[1]));
										break;
									default:
										assert(0);
								}
							}
							if (IsInteger(param[0]) && IsInteger(param[1])) {
								switch (symbol) {
									case TREES_LSL_PROC:
										result = Trees_NewInteger(OBNC_LSL(Trees_Integer(param[0]), Trees_Integer(param[1])));
										break;
									case TREES_ASR_PROC:
										result = Trees_NewInteger(OBNC_ASR(Trees_Integer(param[0]), Trees_Integer(param[1])));
										break;
									case TREES_ROR_PROC:
										result = Trees_NewInteger(OBNC_ROR(Trees_Integer(param[0]), Trees_Integer(param[1])));
										break;
									default:
										assert(0);
								}
							}
							resultType = paramTypes[0];
						} else {
							yyerror("error: integer expression expected as second parameter: %s", procName);
							exit(EXIT_FAILURE);						
						}
					} else {
						yyerror("error: integer expression expected as first parameter: %s", procName);
						exit(EXIT_FAILURE);
					}
				} else {
					yyerror("error: two parameters expected: %s", procName);
					exit(EXIT_FAILURE);
				}
			} else {
				yyerror("error: proper procedure expected: %s", procName);
				exit(EXIT_FAILURE);
			}
			break;			
		case TREES_FLOOR_PROC:
			if (isFunctionCall) {
				if (paramCount == 1) {
					if (Types_IsReal(paramTypes[0])) {
						if (IsReal(param[0])) {
							double x = Trees_Real(param[0]);
							Range_CheckFLOOR(x);
							result = Trees_NewInteger(OBNC_FLOOR(x));
						}
						resultType = Trees_NewLeaf(TREES_INTEGER_TYPE);
					} else {
						yyerror("error: real-valued parameter expected: %s", procName);
						exit(EXIT_FAILURE);
					}
				} else {
					yyerror("error: one parameter expected: %s", procName);
					exit(EXIT_FAILURE);
				}			
			} else {
				yyerror("error: proper procedure expected: %s", procName);
				exit(EXIT_FAILURE);				
			}
			break;
		case TREES_FLT_PROC:
			if (isFunctionCall) {
				if (paramCount == 1) {
					if (Types_IsInteger(paramTypes[0])) {
						if (IsInteger(param[0])) {
							result = Trees_NewReal(OBNC_FLT(Trees_Integer(param[0])));
						}
						resultType = Trees_NewLeaf(TREES_REAL_TYPE);
					} else {
						yyerror("error: integer parameter expected: %s", procName);
						exit(EXIT_FAILURE);
					}
				} else {
					yyerror("error: one parameter expected: %s", procName);
					exit(EXIT_FAILURE);
				}			
			} else {
				yyerror("error: proper procedure expected: %s", procName);
				exit(EXIT_FAILURE);				
			}
			break;
		case TREES_ORD_PROC:
			if (isFunctionCall) {
				if (paramCount == 1) {
					switch (Trees_Symbol(Types_Structure(paramTypes[0]))) {
						case TREES_CHAR_TYPE:
							/*do nothing*/
							break;
						case TREES_STRING_TYPE:
							if (Types_StringLength(paramTypes[0]) <= 1) {
								result = Trees_NewInteger(Trees_String(param[0])[0]);
							} else {
								yyerror("error: single-character string parameter expected: %s", procName);
								exit(EXIT_FAILURE);						
							}
							break;
						case TREES_BOOLEAN_TYPE:
							if (Trees_Symbol(param[0]) == TRUE) {
								result = Trees_NewInteger(1);
							} else if (Trees_Symbol(param[0]) == FALSE) {
								result = Trees_NewInteger(0);
							}
							break;
						case TREES_SET_TYPE:
							if (IsSet(param[0])) {
								result = Trees_NewInteger(Trees_Set(param[0]));
							}
							break;
						default:
							yyerror("error: character parameter expected: %s", procName);
							exit(EXIT_FAILURE);						
					}
					resultType = Trees_NewLeaf(TREES_INTEGER_TYPE);
				} else {
					yyerror("error: one parameter expected: %s", procName);
					exit(EXIT_FAILURE);
				}			
			} else {
				yyerror("error: proper procedure expected: %s", procName);
				exit(EXIT_FAILURE);				
			}
			break;
		case TREES_CHR_PROC:
			if (isFunctionCall) {
				if (paramCount == 1) {
					if (Types_IsInteger(paramTypes[0])) {
						if (IsInteger(param[0])) {
							int i = Trees_Integer(param[0]);
							Range_CheckCHR(i);
							result = Trees_NewChar(OBNC_CHR(i));
						}
					} else {
						yyerror("error: integer parameter expected: %s", procName);
						exit(EXIT_FAILURE);
					}
					resultType = Trees_NewLeaf(TREES_CHAR_TYPE);
				} else {
					yyerror("error: one parameter expected: %s", procName);
					exit(EXIT_FAILURE);
				}			
			} else {
				yyerror("error: proper procedure expected: %s", procName);
				exit(EXIT_FAILURE);				
			}
			break;
		case TREES_INC_PROC: /*fall through*/
		case TREES_DEC_PROC:			
			if (! isFunctionCall) {
				if ((paramCount == 1) || (paramCount == 2)) {
					if (IsDesignator(param[0])) {
						if (Types_IsInteger(paramTypes[0])) {
							if (Writable(param[0])) {
								if ((paramCount == 2) && ! Types_IsInteger(paramTypes[1])) {
									yyerror("error: integer expression expected as second parameter: %s", procName);
									exit(EXIT_FAILURE);
								}
							} else {
								yyerror("error: writable parameter expected: %s", procName);
								exit(EXIT_FAILURE);							
							}
						} else {
							yyerror("error: integer parameter expected: %s", procName);
							exit(EXIT_FAILURE);
						}
					} else {
						yyerror("error: variable parameter expected: %s", procName);
						exit(EXIT_FAILURE);
					}
				} else {
					yyerror("error: one or two parameters expected: %s", procName);
					exit(EXIT_FAILURE);
				}
			} else {
				yyerror("error: function procedure expected: %s", procName);
				exit(EXIT_FAILURE);				
			}
			break;
		case TREES_INCL_PROC: /*fall through*/
		case TREES_EXCL_PROC:
			if (! isFunctionCall) {
				if (paramCount == 2) {
					if (IsDesignator(param[0])) {
						if (Types_IsSet(paramTypes[0])) {
							if (Writable(param[0])) {
								if (! Types_IsInteger(paramTypes[1])) {
									yyerror("error: integer expression expected as second parameter: %s", procName);
									exit(EXIT_FAILURE);
								}
							} else {
								yyerror("error: writable parameter expected: %s", procName);
								exit(EXIT_FAILURE);							
							}
						} else {
							yyerror("error: set expression expected as first parameter: %s", procName);
							exit(EXIT_FAILURE);
						}
					} else {
						yyerror("error: variable parameter expected: %s", procName);
						exit(EXIT_FAILURE);
					}
				} else {
					yyerror("error: two parameters expected: %s", procName);
					exit(EXIT_FAILURE);
				}
			} else {
				yyerror("error: function procedure expected: %s", procName);
				exit(EXIT_FAILURE);				
			}
			break;
		case TREES_ASSERT_PROC:
			if (! isFunctionCall) {
				if (paramCount == 1) {
					if (Types_IsBoolean(paramTypes[0])) {
						result = param[0];
						if (Trees_Symbol(param[0]) == TRUE) {
							result = Trees_NewLeaf(TRUE);
						} else if (Trees_Symbol(param[0]) == FALSE) {
							result = Trees_NewLeaf(FALSE);
						}
						result = Trees_NewNode(
							TREES_ASSERT_PROC, 
							result, 
							Trees_NewNode(TREES_FILE_POSITION,
								Trees_NewString(Args_Filename()),
								Trees_NewInteger(yylineno)));
					} else {
						yyerror("error: boolean parameter expected: %s", procName);
						exit(EXIT_FAILURE);
					}
				} else {
					yyerror("error: one parameter expected: %s", procName);
					exit(EXIT_FAILURE);
				}			
			} else {
				yyerror("error: function procedure expected: %s", procName);
				exit(EXIT_FAILURE);				
			}
			break;
		case TREES_NEW_PROC:		
			if (! isFunctionCall) {
				if (paramCount == 1) {
					if (IsDesignator(param[0])) {
						if (Trees_Symbol(Types_Structure(paramTypes[0])) == POINTER) {
							if (! Writable(param[0])) {
								yyerror("error: writable parameter expected: %s", procName);
								exit(EXIT_FAILURE);								
							}
						} else {
							yyerror("error: pointer parameter expected: %s", procName);
							exit(EXIT_FAILURE);
						}
					} else {
						yyerror("error: variable expected: %s", procName);
						exit(EXIT_FAILURE);
					}
				} else {
					yyerror("error: one parameters expected: %s", procName);
					exit(EXIT_FAILURE);
				}
			} else {
				yyerror("error: function procedure expected: %s", procName);
				exit(EXIT_FAILURE);				
			}
			break;
		case TREES_PACK_PROC:
			if (! isFunctionCall) {
				if (paramCount == 2) {
					if (IsDesignator(param[0])) {
						if (Types_IsReal(paramTypes[0])) {
							if (Writable(param[0])) {
								if (! Types_IsInteger(paramTypes[1])) {
									yyerror("error: integer expression expected as second parameter: %s", procName);
									exit(EXIT_FAILURE);
								}
							} else {
								yyerror("error: writable parameter expected: %s", procName);
								exit(EXIT_FAILURE);								
							}
						} else {
							yyerror("error: real-valued expression expected as first parameter: %s", procName);
							exit(EXIT_FAILURE);
						}
					} else {
						yyerror("error: variable parameter expected: %s", procName);
						exit(EXIT_FAILURE);
					}
				} else {
					yyerror("error: two parameters expected: %s", procName);
					exit(EXIT_FAILURE);
				}
			} else {
				yyerror("error: function procedure expected: %s", procName);
				exit(EXIT_FAILURE);				
			}
			break;
		case TREES_UNPK_PROC:
			if (! isFunctionCall) {
				if (paramCount == 2) {
					if (IsDesignator(param[0]) && IsDesignator(param[1])) {
						if (Types_IsReal(paramTypes[0])) {
							if (Writable(param[0])) {
								if (Types_IsInteger(paramTypes[1])) {
									if (! Writable(param[1])) {
										yyerror("error: second parameter is read-only: %s", procName);
										exit(EXIT_FAILURE);									
									}
								} else {
									yyerror("error: integer expression expected as second parameter: %s", procName);
									exit(EXIT_FAILURE);
								}
							} else {
								yyerror("error: first parameter is read-only: %s", procName);
								exit(EXIT_FAILURE);							
							}
						} else {
							yyerror("error: real-valued expression expected as first parameter: %s", procName);
							exit(EXIT_FAILURE);
						}
					} else {
						yyerror("error: two variable parameters expected: %s", procName);
						exit(EXIT_FAILURE);
					}
				} else {
					yyerror("error: two parameters expected: %s", procName);
					exit(EXIT_FAILURE);
				}
			} else {
				yyerror("error: function procedure expected: %s", procName);
				exit(EXIT_FAILURE);				
			}
			break;
		default:
			assert(0);	
	}
	
	if (result == NULL) {
		if (paramCount == 1) {
			result = Trees_NewNode(symbol, param[0], NULL);
		} else {
			result = Trees_NewNode(symbol, param[0], param[1]);
		}	
	}
	Trees_SetType(resultType, result);
	
	return result;
}


static void HandleProcedureCall(Trees_Node designator, Trees_Node expList, int isFunctionCall, Trees_Node *ast)
{
	Trees_Node ident, designatorTypeStruct, fpList, resultType;
	
	ident = Trees_Left(designator);
	if (Types_IsPredeclaredProcedure(Trees_Type(ident))) {
		*ast = PredeclaredProcedureAST(Trees_Name(ident), expList, isFunctionCall);
		if (*ast == NULL) {
			yyerror("error: procedure expected");
			exit(EXIT_FAILURE);
		}
	} else {
		/*handle non-predeclared procedure*/
		designatorTypeStruct = Types_Structure(Trees_Type(designator));
		if (Types_IsProcedure(designatorTypeStruct)) {
			fpList =Types_Parameters(designatorTypeStruct);
			resultType = Types_ResultType(designatorTypeStruct);
			ValidateProcedureCall(expList, fpList);
			*ast = Trees_NewNode(TREES_PROCEDURE_CALL, designator, expList);
			if (isFunctionCall) {
				if (resultType != NULL) {
					Trees_SetType(resultType, *ast);
				} else {
					yyerror("error: function procedure expected: %s", Trees_Name(ident));
					exit(EXIT_FAILURE);
				}
			} else if (resultType != NULL) {
				yyerror("error: proper procedure expected: %s", Trees_Name(ident));
				exit(EXIT_FAILURE);
			}
		}
	}
	assert(*ast != NULL);
}


static void CheckIntegerLabelDisjointness(Trees_Node rangeA, Trees_Node rangeB)
{
	int aMin, aMax, bMin, bMax;
	
	if (Trees_Symbol(rangeA) == DOTDOT) {
		aMin = Trees_Integer(Trees_Left(rangeA));
		aMax = Trees_Integer(Trees_Right(rangeA));
	} else {
		aMin = Trees_Integer(rangeA);
		aMax = Trees_Integer(rangeA);	
	}
	if (Trees_Symbol(rangeB) == DOTDOT) {
		bMin = Trees_Integer(Trees_Left(rangeB));
		bMax = Trees_Integer(Trees_Right(rangeB));
	} else {
		bMin = Trees_Integer(rangeB);
		bMax = Trees_Integer(rangeB);	
	}
	
	if ((aMin >= bMin) && (aMin <= bMax)) {
		yyerror("error: case label defined twice: %d", aMin);
		exit(EXIT_FAILURE);	
	} else if ((bMin >= aMin) && (bMin <= aMax)) {
		yyerror("error: case label defined twice: %d", bMin);
		exit(EXIT_FAILURE);		
	}
}


static void CheckCharLabelDisjointness(Trees_Node rangeA, Trees_Node rangeB)
{
	char aMin, aMax, bMin, bMax;
	
	if (Trees_Symbol(rangeA) == DOTDOT) {
		aMin = Trees_Char(Trees_Left(rangeA));
		aMax = Trees_Char(Trees_Right(rangeA));
	} else {
		aMin = Trees_Char(rangeA);
		aMax = Trees_Char(rangeA);	
	}
	if (Trees_Symbol(rangeB) == DOTDOT) {
		bMin = Trees_Char(Trees_Left(rangeB));
		bMax = Trees_Char(Trees_Right(rangeB));
	} else {
		bMin = Trees_Char(rangeB);
		bMax = Trees_Char(rangeB);	
	}

	if ((aMin >= bMin) && (aMin <= bMax)) {
		yyerror("error: case label defined twice: %c", aMin);
		exit(EXIT_FAILURE);	
	} else if ((bMin >= aMin) && (bMin <= aMax)) {
		yyerror("error: case label defined twice: %c", bMin);
		exit(EXIT_FAILURE);		
	}
}


static void CheckCaseLabelUniqueness(Trees_Node newLabelRange)
{
	int labelSymbol;
	Trees_Node labelList, definedLabelRange;
	
	if (Trees_Symbol(newLabelRange) == DOTDOT) {
		labelSymbol = Trees_Symbol(Trees_Left(newLabelRange));
	} else {
		labelSymbol = Trees_Symbol(newLabelRange);
	}
		
	labelList = currentlyDefinedCaseLabels;
	while (labelList != NULL) {
		definedLabelRange = Trees_Left(labelList);
		switch (labelSymbol) {
			case INTEGER:
				CheckIntegerLabelDisjointness(definedLabelRange, newLabelRange);
				break;
			case TREES_CHAR_CONSTANT:
				CheckCharLabelDisjointness(definedLabelRange, newLabelRange);
				break;
			case IDENT:
				if (Types_Same(definedLabelRange, newLabelRange)) {
					yyerror("error: type label defined twice: %s", Trees_Name(newLabelRange));
					exit(EXIT_FAILURE);
				}
				break;
			default:
				assert(0);
		}
		labelList = Trees_Right(labelList);
	}
}


/*functions for module productions*/

static void ReadLine(FILE *file, char result[], int resultLen)
{
	int ch, i;

	ch = fgetc(file);
	i = 0;
	while ((ch != EOF) && (ch != '\n') && (i < resultLen - 1)) {
		result[i] = ch;
		ch = fgetc(file);
		i++;
	}
	result[i] = '\0';
}


static char *EscapedString(const char s[])
{
	int i, j, quoteCount;
	char *result;
	
	quoteCount = 0;
	i = 0;
	while (s[i] != '\0') {
		if ((s[i] == '\'') || (s[i] == '"')) {
			quoteCount++;
		}
		i++;
	}
	
	NEW_ARRAY(result, strlen(s) + quoteCount + 1);
	
	i = 0;
	j = 0;
	while (s[i] != '\0') {
		if ((s[i] == '\'') || (s[i] == '"')) {
			result[j] = '\\';
			j++;
		}
		result[j] = s[i];
		i++;
		j++;
	}
	result[j] = '\0';
	
	return result;
}


static char *ModuleDirPath(const char module[])
{
	const char *installPath, *cmdTemplate;
	char *escInstallPath, *command, *result;
	int commandLen, resultLen;
	FILE *fp;

	result = NULL;
	installPath = getenv("OBNC_INSTALL_PATH");
	if (installPath != NULL) {
		if (strcmp(installPath, "") != 0) {
			escInstallPath = EscapedString(installPath); /*prevent command injection*/
			cmdTemplate = "'%s/bin/obnc-path' %s";
			commandLen = strlen(cmdTemplate) + strlen(escInstallPath) + strlen(module) + 1;
			NEW_ARRAY(command, commandLen);
			sprintf(command, cmdTemplate, escInstallPath, module);
			fp = popen(command, "r");
			if (fp != NULL) {
				resultLen = PATH_MAX + 1;
				NEW_ARRAY(result, resultLen);
				ReadLine(fp, result, resultLen);
				pclose(fp);
			} else {
				fprintf(stderr, "Command failed: %s: %s\n", command, strerror(errno));
				exit(EXIT_FAILURE);
			}
		} else {
			fprintf(stderr, "Non-empty environment variable expected: OBNC_INSTALL_PATH\n");
			exit(EXIT_FAILURE);
		}
	} else {
		fprintf(stderr, "Undefined environment variable: OBNC_INSTALL_PATH\n");
		exit(EXIT_FAILURE);
	}
	return result;
}


static void ExportSymbolTable(const char symfilePath[])
{
	static char tempSymfilePath[PATH_MAX + 1];

	if (! Files_Exists(".obnc")) {
		Files_CreateDir(".obnc");
	}	
	sprintf(tempSymfilePath, ".obnc/%s.sym.%d", Args_ModuleName(), getpid());
	Table_Export(tempSymfilePath);
	Files_Move(tempSymfilePath, symfilePath);
}
