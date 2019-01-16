/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

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
#define YYBISON_VERSION "3.0.2"

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

#include "Config.h"
#include "Error.h"
#include "Files.h"
#include "Generate.h"
#include "lex.yy.h"
#include "Maps.h"
#include "Oberon.h"
#include "ModulePaths.h"
#include "Paths.h"
#include "Range.h"
#include "Table.h"
#include "Types.h"
#include "Trees.h"
#include "Util.h"
#include "../lib/obnc/OBNC.h"
#include <assert.h>
#include <ctype.h>
#include <float.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*assignment contexts*/
#define ASSIGNMENT_CONTEXT 0
#define PARAM_SUBST_CONTEXT 1
#define PROC_RESULT_CONTEXT 2

static const char *inputFilename;
static int parseMode;
static char *inputModuleName;

static Trees_Node unresolvedPointerTypes;
static Trees_Node currentTypeIdentdef;
static Trees_Node recordDeclarationStack;
static Trees_Node caseExpressionStack;
static Trees_Node caseLabelsStack;
static Trees_Node procedureDeclarationStack;

/*constant predicate functions*/

static int IsBoolean(Trees_Node node);
static int IsChar(Trees_Node node);
static int IsInteger(Trees_Node node);
static int IsReal(Trees_Node node);
static int IsString(Trees_Node node);
static int IsSet(Trees_Node node);

/*functions for type declaration productions*/

static Trees_Node ResolvedType(Trees_Node type, int isTypeDecl);
static void ResolvePointerTypes(Trees_Node baseType);
static const char *TypeString(Trees_Node type);

/*functions for expression productions*/

static Trees_Node Designator(const char ident[], Trees_Node selectorList);
static int IsDesignator(Trees_Node exp);
static Trees_Node BaseIdent(Trees_Node designator);
static Trees_Node FirstSelector(Trees_Node designator);
static const char *DesignatorString(Trees_Node designator);
static void CheckIsValueExpression(Trees_Node exp);
static void SetSelectorTypes(Trees_Node identType, Trees_Node designator, int *parameterListFound);
static void RemoveActualParameters(Trees_Node *designator, Trees_Node *actualParameters);
static int IsConstExpression(Trees_Node exp);
static Trees_Node ExpressionConstValue(int relation, Trees_Node expA, Trees_Node expB);
static Trees_Node SimpleExpressionConstValue(int addOperator, Trees_Node expA, Trees_Node expB);
static Trees_Node TermConstValue(int mulOperator, Trees_Node expA, Trees_Node expB);
static const char *OperatorString(int operator);

/*functions for statement productions*/

static int Writable(Trees_Node designator);
static void ValidateAssignment(Trees_Node expression, Trees_Node targetType, int context, int paramPos);
static void HandleProcedureCall(Trees_Node designator, Trees_Node actualParameters, int isFunctionCall, Trees_Node *ast);
static void CheckCaseLabelUniqueness(Trees_Node label);

/*functions for module productions*/

static void ExportSymbolTable(const char symfilePath[]);

#line 150 "y.tab.c" /* yacc.c:339  */

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
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 102 "Oberon.y" /* yacc.c:355  */

	const char *ident;
	OBNC_LONGI int integer;
	OBNC_LONGR double real;
	const char *string;
	Trees_Node node;

#line 288 "y.tab.c" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 303 "y.tab.c" /* yacc.c:358  */

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
       0,   216,   216,   220,   227,   245,   250,   259,   263,   273,
     289,   304,   327,   337,   338,   339,   340,   341,   345,   365,
     375,   379,   386,   406,   414,   425,   430,   436,   472,   478,
     484,   488,   516,   565,   569,   579,   614,   623,   630,   637,
     639,   648,   686,   687,   734,   738,   742,   746,   750,   754,
     758,   762,   769,   791,   820,   824,   829,   835,   839,   843,
     850,   851,   881,   885,   889,   893,   897,   904,   905,   909,
     914,   918,   922,   928,   948,   953,   974,   994,  1013,  1019,
    1027,  1045,  1050,  1055,  1062,  1066,  1073,  1074,  1087,  1107,
    1131,  1136,  1150,  1151,  1152,  1153,  1154,  1155,  1156,  1158,
    1164,  1207,  1225,  1233,  1241,  1252,  1275,  1288,  1293,  1299,
    1304,  1310,  1331,  1369,  1377,  1392,  1398,  1404,  1408,  1424,
    1433,  1480,  1489,  1503,  1588,  1595,  1600,  1606,  1621,  1639,
    1665,  1676,  1694,  1703,  1752,  1773,  1788,  1793,  1799,  1804,
    1810,  1814,  1815,  1819,  1820,  1824,  1836,  1842,  1849,  1850,
    1854,  1855,  1859,  1860,  1864,  1865,  1869,  1876,  1882,  1888,
    1893,  1926,  1946,  1952,  1971,  1976,  1982,  1986,  2007,  2023,
    2028,  2037,  2063,  2077,  2084,  2092,  2128,  2136,  2147,  2222,
    2227,  2233
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
  "OpenArrayOptRep", "module", "ModuleHeading", "ImportListOpt",
  "ImportList", "ImportRep", "import", "BecomesIdentOpt",
  "ModuleStatements", YY_NULLPTR
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

#define YYPACT_NINF -132

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-132)))

#define YYTABLE_NINF -166

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      -5,   -21,    49,    -8,  -132,  -132,    37,    19,    54,  -132,
      36,    46,  -132,  -132,    75,    61,    58,  -132,    19,  -132,
      68,   100,  -132,    98,  -132,    90,  -132,  -132,  -132,    70,
      79,    80,    29,    92,    29,   100,    29,  -132,    94,  -132,
    -132,  -132,  -132,    83,  -132,  -132,  -132,  -132,  -132,    93,
    -132,  -132,    68,  -132,  -132,    29,  -132,  -132,  -132,  -132,
      64,     9,   114,   101,   108,  -132,   110,   109,   132,   -32,
      29,   100,    99,    68,   118,   103,    95,    20,  -132,  -132,
    -132,  -132,  -132,  -132,  -132,  -132,  -132,  -132,  -132,  -132,
    -132,    29,     9,  -132,  -132,  -132,  -132,  -132,  -132,    29,
       9,   -24,  -132,     8,  -132,  -132,  -132,    82,    29,    29,
     100,    29,   100,   105,     7,    29,  -132,  -132,  -132,  -132,
    -132,  -132,   -27,    96,    68,    97,   104,   111,  -132,  -132,
      29,   119,  -132,   113,   106,  -132,  -132,  -132,    20,  -132,
      68,  -132,    20,  -132,   111,    33,     8,   107,  -132,  -132,
     115,   -37,  -132,  -132,  -132,  -132,  -132,  -132,     9,  -132,
    -132,  -132,    -4,  -132,    23,  -132,   116,  -132,  -132,   149,
    -132,  -132,  -132,  -132,  -132,  -132,   -12,   -36,    68,    20,
    -132,  -132,  -132,    54,   -20,  -132,  -132,  -132,   -13,  -132,
    -132,   124,  -132,   125,  -132,   146,   126,  -132,    43,  -132,
    -132,  -132,    29,    29,  -132,  -132,  -132,    82,    82,   100,
      82,    29,   150,   102,   117,    29,  -132,  -132,  -132,  -132,
      75,  -132,   120,   127,  -132,   128,  -132,    29,  -132,   129,
    -132,  -132,    68,    20,  -132,  -132,  -132,  -132,  -132,  -132,
    -132,   100,   100,    29,   154,    29,  -132,  -132,   139,   123,
     135,  -132,    44,  -132,  -132,  -132,  -132,   164,  -132,   152,
    -132,   173,    29,   171,   124,  -132,  -132,   145,  -132,  -132,
     100,   100,  -132,   147,  -132,  -132,  -132,    15,  -132,  -132,
    -132,   166,  -132,  -132
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
      99,    99,   138,     0,   161,   167,   163,     0,   107,   125,
     133,     0,   168,   169
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -132,  -102,   -14,  -132,  -132,  -132,   -54,  -132,  -132,  -131,
    -132,  -132,  -132,   -40,  -132,  -132,  -132,  -132,  -132,  -132,
     -42,   121,  -132,  -132,  -132,  -132,    47,  -132,   -30,  -132,
     112,  -132,  -132,   122,  -132,   -92,   -57,  -132,  -132,  -132,
    -132,   -11,    78,   130,  -132,  -132,   -35,  -132,  -132,   -33,
    -132,  -132,  -132,  -132,  -132,    -9,  -132,    -6,   -15,  -132,
    -132,  -132,  -132,  -132,  -132,  -132,  -132,  -132,  -132,   -16,
    -132,    13,  -132,  -132,  -132,  -132,  -132,  -132,  -132,  -132,
    -132,  -132,  -132,  -132,   -51,  -132,  -132,  -132,  -132,  -132,
    -132,  -132,  -132,  -132,   182,  -132,  -132
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
     186,   222,   223,   265,   224,   225,   252,   276,   277,     2,
       3,     8,     9,    11,    12,    17,    23
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      67,    78,    59,    68,   105,   161,    30,   194,   148,   206,
     226,   199,   203,   215,   113,   221,     1,   153,   114,   281,
       4,  -165,   178,    93,   130,    79,   179,   217,   154,   204,
     115,    94,   116,    57,    58,   105,   227,   215,    75,   216,
     118,    95,   149,   105,     6,   131,   132,   133,   219,     5,
      37,    96,    97,    98,     7,   155,   134,    82,   174,    99,
      10,   134,    13,   207,    57,    58,   205,   156,   157,   147,
     100,   150,   208,    16,   101,   170,   209,   172,   167,   168,
      21,   171,    80,    81,   175,   175,    57,    58,    82,   228,
      89,    90,   178,   267,    24,    18,   233,   268,    19,    27,
      79,   105,   256,    83,    84,   161,   161,    32,   161,    29,
     181,    49,    85,   242,   243,    33,    34,    53,    86,    87,
      88,    89,    90,   134,   159,    50,   160,    55,    35,   245,
     246,    70,    56,    63,    72,    71,    36,   107,   108,   109,
     110,    37,   112,   111,   124,   120,   173,   129,   180,   182,
     190,   128,   193,   202,   210,   211,   183,   240,   201,   231,
     241,   184,   274,   191,   218,   134,   230,   260,   262,   251,
     221,   249,   234,   150,   238,   282,   264,   269,   232,   250,
     254,    79,   270,   271,   273,   247,   275,   253,   280,   283,
     255,   200,   235,   177,   122,   239,   220,    79,   236,   266,
      28,   119,   237,   145,   248,     0,   257,   258,     0,     0,
     259,     0,   261,     0,   146,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   272,     0,     0,   278,   279
};

static const yytype_int16 yycheck[] =
{
      35,    55,    32,    36,    61,   107,    20,   138,   100,    13,
      23,   142,    49,    49,    46,    35,    21,     9,    50,     4,
      41,    41,    49,    14,     4,    55,    53,    63,    20,    66,
      62,    22,    64,    57,    58,    92,    49,    49,    52,    51,
      70,    32,    66,   100,    52,    25,    26,    27,   179,     0,
      41,    42,    43,    44,    17,    47,    41,    24,    51,    50,
      41,    41,     8,    67,    57,    58,   158,    59,    60,    99,
      61,   101,    49,    37,    65,   110,    53,   112,   108,   109,
       5,   111,    18,    19,   114,   115,    57,    58,    24,   191,
      57,    58,    49,    49,    33,    49,    53,    53,    52,    41,
     130,   158,   233,    39,    40,   207,   208,     7,   210,    41,
     124,    13,    48,    11,    12,    15,    16,    47,    54,    55,
      56,    57,    58,    41,    42,    35,    44,    48,    28,    12,
      13,    37,    52,    41,    41,    52,    36,    23,    37,    31,
      30,    41,    10,    34,    26,    46,    41,    52,    52,    52,
      31,    48,    46,    38,    38,     6,    52,   211,    51,    13,
      10,    50,   264,    50,   178,    41,    41,    13,    29,    41,
      35,    51,   202,   203,   209,   277,    53,    13,    52,    52,
      51,   211,    30,    10,    13,   215,    41,   227,    41,    23,
     232,   144,   203,   115,    73,   210,   183,   227,   207,   250,
      18,    71,   208,    91,   220,    -1,   241,   242,    -1,    -1,
     243,    -1,   245,    -1,    92,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      30,    10,    96,    13,    69,    41,   155,   156,   114,   114,
      41,     4,    69,    23
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
       3,     2,     0,     4,     1,     0,     1,     3,     2,     3,
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
#line 217 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewIdent((yyvsp[0].ident));
	}
#line 1614 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 221 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewIdent(Util_String("%s.%s", (yyvsp[-2].ident), (yyvsp[0].ident)));
	}
#line 1622 "y.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 228 "Oberon.y" /* yacc.c:1646  */
    {
		if (! Table_LocallyDeclared((yyvsp[-1].ident)) || (recordDeclarationStack != NULL)) {
			(yyval.node) = Trees_NewIdent((yyvsp[-1].ident));
			if ((yyvsp[0].integer)) {
				Trees_SetExported((yyval.node));
			}
			if (Table_ScopeLocal()) {
				Trees_SetLocal((yyval.node));
			}
		} else {
			yyerror("error: redeclaration of identifier: %s", (yyvsp[-1].ident));
			YYABORT;
		}
	}
#line 1641 "y.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 246 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = 1;
	}
#line 1649 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 250 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = 0;
	}
#line 1657 "y.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 260 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewInteger((yyvsp[0].integer));
	}
#line 1665 "y.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 264 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewReal((yyvsp[0].real));
	}
#line 1673 "y.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 274 "Oberon.y" /* yacc.c:1646  */
    {
		if (! (Trees_Exported((yyvsp[-2].node)) && Trees_Local((yyvsp[-2].node)))) {
			Trees_SetKind(TREES_CONSTANT_KIND, (yyvsp[-2].node));
			Trees_SetType(Trees_Type((yyvsp[0].node)), (yyvsp[-2].node));
			Trees_SetValue((yyvsp[0].node), (yyvsp[-2].node));
			Table_Put((yyvsp[-2].node));
			Generate_ConstDeclaration((yyvsp[-2].node));
		} else {
			yyerror("error: cannot export local constant: %s", Trees_Name((yyvsp[-2].node)));
			YYABORT;
		}
	}
#line 1690 "y.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 290 "Oberon.y" /* yacc.c:1646  */
    {
		if (IsConstExpression((yyvsp[0].node))) {
			(yyval.node) = (yyvsp[0].node);
		} else {
			yyerror("error: constant expression expected");
			YYABORT;
		}
	}
#line 1703 "y.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 305 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node sourceType;

		sourceType = ResolvedType((yyvsp[0].node), 1);
		if (sourceType != NULL) {
			if (! (Trees_Exported((yyvsp[-1].node)) && Trees_Local((yyvsp[-1].node)))) {
				Trees_SetType(sourceType, (yyvsp[-1].node));
				ResolvePointerTypes((yyvsp[-1].node));
				currentTypeIdentdef = NULL;
				Generate_TypeDeclaration((yyvsp[-1].node));
			} else {
				yyerror("error: cannot export local type: %s", Trees_Name((yyvsp[-1].node)));
				YYABORT;
			}
		} else {
			yyerror("error: undeclared identifier: %s", Trees_Name((yyvsp[0].node)));
			YYABORT;
		}
	}
#line 1727 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 328 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_SetKind(TREES_TYPE_KIND, (yyvsp[-1].node));
		currentTypeIdentdef = (yyvsp[-1].node);
		Table_Put((yyvsp[-1].node));
		(yyval.node) = (yyvsp[-1].node);
	}
#line 1738 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 346 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node reversedLengths, length;

		(yyval.node) = ResolvedType((yyvsp[0].node), 0);
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
#line 1759 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 366 "Oberon.y" /* yacc.c:1646  */
    {
		if ((currentTypeIdentdef != NULL) && (Trees_Type(currentTypeIdentdef) == NULL)) {
			Trees_SetType(Trees_NewLeaf(ARRAY), currentTypeIdentdef); /*incomplete type*/
		}
		(yyval.node) = (yyvsp[-1].node);
	}
#line 1770 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 376 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewNode(TREES_NOSYM, (yyvsp[0].node), NULL);
	}
#line 1778 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 380 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewNode(TREES_NOSYM, (yyvsp[0].node), (yyvsp[-2].node));
	}
#line 1786 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 387 "Oberon.y" /* yacc.c:1646  */
    {
		if (Types_IsInteger(Trees_Type((yyvsp[0].node)))) {
			if (IsInteger((yyvsp[0].node))) {
				if (Trees_Integer((yyvsp[0].node)) <= 0) {
					yyerror("error: positive length expected: %" OBNC_INT_MOD "d", Trees_Integer((yyvsp[0].node)));
					YYABORT;
				}
			} else {
				yyerror("error: fully evaluated constant expression expected as increment");
				YYABORT;
			}
		} else {
			yyerror("error: integer length expected");
			YYABORT;
		}
	}
#line 1807 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 407 "Oberon.y" /* yacc.c:1646  */
    {
		recordDeclarationStack = Trees_Right(recordDeclarationStack);
		(yyval.node) = Types_NewRecord(Types_RecordBaseType((yyvsp[-2].node)), (yyvsp[-1].node));
	}
#line 1816 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 415 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Types_NewRecord((yyvsp[0].node), NULL);
		if ((currentTypeIdentdef != NULL) && (Trees_Type(currentTypeIdentdef) == NULL)) {
			Trees_SetType((yyval.node), currentTypeIdentdef);
		}
		recordDeclarationStack = Trees_NewNode(TREES_NOSYM, (yyval.node), recordDeclarationStack);
	}
#line 1828 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 426 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = (yyvsp[-1].node);
	}
#line 1836 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 430 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = NULL;
	}
#line 1844 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 437 "Oberon.y" /* yacc.c:1646  */
    {
		const char *name;
		Trees_Node symbol;

		(yyval.node) = NULL;
		name = Trees_Name((yyvsp[0].node));
		symbol = Table_At(name);
		if (symbol != NULL) {
			if (Trees_Kind(symbol) == TREES_TYPE_KIND) {
				if (symbol != currentTypeIdentdef) {
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
					yyerror("error: invalid base type: %s", name);
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
#line 1881 "y.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 473 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_ReverseList(&(yyvsp[0].node)); /*correct order*/
		(yyval.node) = (yyvsp[0].node);
	}
#line 1890 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 478 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = NULL;
	}
#line 1898 "y.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 485 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewNode(TREES_FIELD_LIST_SEQUENCE, (yyvsp[0].node), NULL);
	}
#line 1906 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 489 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node currSeq, currList, currSeqList;
		const char *seqIdentName, *listIdentName;

		currList = (yyvsp[0].node);
		while (currList != NULL) {
			listIdentName = Trees_Name(Trees_Left(currList));
			currSeq = (yyvsp[-2].node);
			while (currSeq != NULL) {
				currSeqList = Trees_Left(currSeq);
				while (currSeqList != NULL) {
					seqIdentName = Trees_Name(Trees_Left(currSeqList));
					if (strcmp(listIdentName, seqIdentName) == 0) {
						yyerror("error: redeclaration of field: %s", listIdentName);
						YYABORT;
					}
					currSeqList = Trees_Right(currSeqList);
				}
				currSeq = Trees_Right(currSeq);
			}
			currList = Trees_Right(currList);
		}
		(yyval.node) = Trees_NewNode(TREES_FIELD_LIST_SEQUENCE, (yyvsp[0].node), (yyvsp[-2].node));
	}
#line 1935 "y.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 517 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node type, tail, ident, p, directBaseType, baseTypeField, baseType;

		(yyval.node) = NULL;
		type = ResolvedType((yyvsp[0].node), 0);
		if (type != NULL) {
			if (! ((type == currentTypeIdentdef) && ! Types_IsPointer(type))) {
				Trees_ReverseList(&(yyvsp[-2].node)); /*correct order*/
				tail = (yyvsp[-2].node);
				do {
					ident = Trees_Left(tail);
					p = (yyvsp[-2].node);
					while ((p != tail) && (strcmp(Trees_Name(ident), Trees_Name(Trees_Left(p))) != 0)) {
						p = Trees_Right(p);
					}
					if (p == tail) {
						assert(recordDeclarationStack != NULL);
						directBaseType = Types_RecordBaseType(Trees_Left(recordDeclarationStack));
						if (directBaseType != NULL) {
							Types_GetFieldIdent(Trees_Name(ident), directBaseType, 0, &baseTypeField, &baseType);
						}
						if ((directBaseType == NULL) || (baseTypeField == NULL)) {
							Trees_SetKind(TREES_FIELD_KIND, ident);
							Trees_SetType(type, ident);
						} else {
							yyerror("error: redeclaration of field: %s defined in %s", Trees_Name(ident), Trees_Name(baseType));
							YYABORT;
						}
					} else {
						yyerror("error: redeclaration of field: %s", Trees_Name(ident));
						YYABORT;
					}
					tail = Trees_Right(tail);
				} while (tail != NULL);

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
#line 1985 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 566 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewNode(TREES_IDENT_LIST, (yyvsp[0].node), NULL);
	}
#line 1993 "y.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 570 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node reversedIdents;

		reversedIdents = Trees_NewNode(TREES_IDENT_LIST, (yyvsp[0].node), (yyvsp[-2].node));
		(yyval.node) = reversedIdents;
	}
#line 2004 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 580 "Oberon.y" /* yacc.c:1646  */
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
			} else if (currentTypeIdentdef != NULL) {
				Trees_SetKind(TREES_TYPE_KIND, (yyvsp[0].node));
				Trees_SetType(Types_NewRecord(NULL, NULL), (yyvsp[0].node));
				(yyval.node) = Types_NewPointer((yyvsp[0].node));
				unresolvedPointerTypes = Trees_NewNode(TREES_NOSYM, (yyval.node), unresolvedPointerTypes);
			} else {
				yyerror("error: undeclared type: %s", baseTypeName);
				YYABORT;
			}
		} else if(Trees_Symbol((yyvsp[0].node)) == RECORD) {
			(yyval.node) = Types_NewPointer((yyvsp[0].node));
		} else {
			yyerror("error: record expected as pointer base type");
			YYABORT;
		}
	}
#line 2040 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 615 "Oberon.y" /* yacc.c:1646  */
    {
		if ((currentTypeIdentdef != NULL) && (Trees_Type(currentTypeIdentdef) == NULL)) {
			Trees_SetType(Types_NewPointer(NULL), currentTypeIdentdef); /*incomplete type*/
		}
	}
#line 2050 "y.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 624 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = (yyvsp[0].node);
	}
#line 2058 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 631 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = NULL;
	}
#line 2066 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 639 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewLeaf(PROCEDURE);
	}
#line 2074 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 649 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node type, identList, ident;

		type = ResolvedType((yyvsp[0].node), 0);
		if (type != NULL) {
			Trees_ReverseList(&(yyvsp[-2].node)); /*correct order*/
			identList = (yyvsp[-2].node);
			do {
				ident = Trees_Left(identList);
				if (! (Trees_Exported(ident) && Trees_Local(ident))) {
					if (! Table_LocallyDeclared(Trees_Name(ident))) {
						Trees_SetKind(TREES_VARIABLE_KIND, ident);
						Trees_SetType(type, ident);
						Table_Put(ident);
					} else {
						yyerror("error: redeclaration of identifier with the same name: %s", Trees_Name(ident));
						YYABORT;
					}
				} else {
					yyerror("error: cannot export local variable: %s", Trees_Name(ident));
					YYABORT;
				}
				identList = Trees_Right(identList);
			} while (identList != NULL);

			Generate_VariableDeclaration((yyvsp[-2].node));
		} else {
			yyerror("error: undeclared identifier: %s", Trees_Name((yyvsp[0].node)));
			exit(EXIT_FAILURE);
		}
	}
#line 2110 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 688 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node expA, expB, typeA, typeB;
		int op = (int) (yyvsp[-1].integer);

		expA = (yyvsp[-2].node);
		expB = (yyvsp[0].node);
		typeA = Trees_Type((yyvsp[-2].node));
		typeB = Trees_Type((yyvsp[0].node));

		CheckIsValueExpression((yyvsp[-2].node));
		if (op == IS) {
			if (IsDesignator((yyvsp[-2].node))) {
				if (! (Types_IsRecord(typeA) && (Trees_Kind(BaseIdent((yyvsp[-2].node))) != TREES_VAR_PARAM_KIND))) {
					expB = BaseIdent((yyvsp[0].node));
					typeB = BaseIdent((yyvsp[0].node));
				} else {
					yyerror("error: variable parameter expected as first operand of IS");
					YYABORT;
				}
			} else {
				yyerror("error: identifier expected as first operand of IS");
				YYABORT;
			}
		} else {
			CheckIsValueExpression((yyvsp[0].node));
		}
		if (Types_ExpressionCompatible(op, typeA, typeB)) {
			(yyval.node) = ExpressionConstValue(op, expA, expB);
			if ((yyval.node) == NULL) {
				if (IsString(expA) && Types_IsChar(typeB)) {
					expA = Trees_NewChar(Trees_String(expA)[0]);
				} else if (Types_IsChar(typeA) && IsString(expB)) {
					expB = Trees_NewChar(Trees_String(expB)[0]);
				}
				(yyval.node) = Trees_NewNode(op, expA, expB);
				Trees_SetType(Trees_NewLeaf(TREES_BOOLEAN_TYPE), (yyval.node));
			}
		} else {
			yyerror("error: incompatible types in relation \"%s\": %s, %s",
				OperatorString(op), TypeString(typeA), TypeString(typeB));
			YYABORT;
		}
	}
#line 2158 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 735 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = '=';
	}
#line 2166 "y.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 739 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = '#';
	}
#line 2174 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 743 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = '<';
	}
#line 2182 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 747 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = LE;
	}
#line 2190 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 751 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = '>';
	}
#line 2198 "y.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 755 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = GE;
	}
#line 2206 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 759 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = IN;
	}
#line 2214 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 763 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = IS;
	}
#line 2222 "y.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 770 "Oberon.y" /* yacc.c:1646  */
    {
		int op = (int) (yyvsp[-1].integer);
		(yyval.node) = (yyvsp[0].node);
		if (op >= 0) {
			CheckIsValueExpression((yyvsp[0].node));
			if (Types_ExpressionCompatible(op, Trees_Type((yyvsp[0].node)), NULL)) {
				(yyval.node) = SimpleExpressionConstValue(op, (yyvsp[0].node), NULL);
				if ((yyval.node) == NULL) {
					(yyval.node) = Trees_NewNode(op, (yyvsp[0].node), NULL);
					if (Types_IsByte(Trees_Type((yyvsp[0].node)))) {
						Trees_SetType(Trees_NewLeaf(TREES_INTEGER_TYPE), (yyval.node));
					} else {
						Trees_SetType(Trees_Type((yyvsp[0].node)), (yyval.node));
					}
				}
			} else {
				yyerror("error: incompatible type in unary operation \"%s\": %s", OperatorString(op), TypeString(Trees_Type((yyvsp[0].node))));
				YYABORT;
			}
		}
	}
#line 2248 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 792 "Oberon.y" /* yacc.c:1646  */
    {
		int op = (int) (yyvsp[-1].integer);

		(yyval.node) = NULL;

		CheckIsValueExpression((yyvsp[-2].node));
		CheckIsValueExpression((yyvsp[0].node));

		if (Types_ExpressionCompatible(op, Trees_Type((yyvsp[-2].node)), Trees_Type((yyvsp[0].node)))) {
			(yyval.node) = SimpleExpressionConstValue(op, (yyvsp[-2].node), (yyvsp[0].node));
			if ((yyval.node) == NULL) {
				(yyval.node) = Trees_NewNode(op, (yyvsp[-2].node), (yyvsp[0].node));
				if (Types_IsByte(Trees_Type((yyvsp[-2].node))) || Types_IsByte(Trees_Type((yyvsp[0].node)))) {
					Trees_SetType(Trees_NewLeaf(TREES_INTEGER_TYPE), (yyval.node));
				} else {
					Trees_SetType(Trees_Type((yyvsp[-2].node)), (yyval.node));
				}
			}
		} else {
			yyerror("error: incompatible types in operation \"%s\": %s, %s",
				OperatorString(op), TypeString(Trees_Type((yyvsp[-2].node))), TypeString(Trees_Type((yyvsp[0].node))));
			YYABORT;
		}
		assert((yyval.node) != NULL);
	}
#line 2278 "y.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 821 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = '+';
	}
#line 2286 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 825 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = '-';
	}
#line 2294 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 829 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = -1;
	}
#line 2302 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 836 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = '+';
	}
#line 2310 "y.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 840 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = '-';
	}
#line 2318 "y.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 844 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = OR;
	}
#line 2326 "y.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 852 "Oberon.y" /* yacc.c:1646  */
    {
		int op = (int) (yyvsp[-1].integer);

		(yyval.node) = NULL;

		CheckIsValueExpression((yyvsp[-2].node));
		CheckIsValueExpression((yyvsp[0].node));

		if (Types_ExpressionCompatible(op, Trees_Type((yyvsp[-2].node)), Trees_Type((yyvsp[0].node)))) {
			(yyval.node) = TermConstValue(op, (yyvsp[-2].node), (yyvsp[0].node));
			if ((yyval.node) == NULL) {
				(yyval.node) = Trees_NewNode(op, (yyvsp[-2].node), (yyvsp[0].node));
				if (Types_IsByte(Trees_Type((yyvsp[-2].node))) || Types_IsByte(Trees_Type((yyvsp[0].node)))) {
					Trees_SetType(Trees_NewLeaf(TREES_INTEGER_TYPE), (yyval.node));
				} else {
					Trees_SetType(Trees_Type((yyvsp[-2].node)), (yyval.node));
				}
			}
		} else {
			yyerror("error: incompatible types in operation \"%s\": %s, %s",
				OperatorString(op), TypeString(Trees_Type((yyvsp[-2].node))), TypeString(Trees_Type((yyvsp[0].node))));
			YYABORT;
		}

		assert((yyval.node) != NULL);
	}
#line 2357 "y.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 882 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = '*';
	}
#line 2365 "y.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 886 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = '/';
	}
#line 2373 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 890 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = DIV;
	}
#line 2381 "y.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 894 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = MOD;
	}
#line 2389 "y.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 898 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = '&';
	}
#line 2397 "y.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 906 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewString((yyvsp[0].string));
	}
#line 2405 "y.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 910 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewLeaf(NIL);
		Trees_SetType(Trees_NewLeaf(TREES_NIL_TYPE), (yyval.node));
	}
#line 2414 "y.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 915 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewBoolean(1);
	}
#line 2422 "y.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 919 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewBoolean(0);
	}
#line 2430 "y.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 923 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = (yyvsp[0].node);
		Trees_SetType(Trees_NewLeaf(TREES_SET_TYPE), (yyval.node));
	}
#line 2439 "y.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 929 "Oberon.y" /* yacc.c:1646  */
    {
		const int isFunctionCall = 1;
		Trees_Node designator, actualParameters, ident;

		(yyval.node) = NULL;
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
		assert((yyval.node) != NULL);
	}
#line 2463 "y.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 949 "Oberon.y" /* yacc.c:1646  */
    {
		CheckIsValueExpression((yyvsp[-1].node));
		(yyval.node) = (yyvsp[-1].node);
	}
#line 2472 "y.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 954 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = NULL;
		CheckIsValueExpression((yyvsp[0].node));
		if (Types_ExpressionCompatible('~', Trees_Type((yyvsp[0].node)), NULL)) {
			if (IsBoolean((yyvsp[0].node))) {
				(yyval.node) = Trees_NewBoolean(! Trees_Boolean((yyvsp[0].node)));
			} else {
				(yyval.node) = Trees_NewNode('~', (yyvsp[0].node), NULL);
				Trees_SetType(Trees_NewLeaf(TREES_BOOLEAN_TYPE), (yyval.node));
			}
		} else {
			yyerror("error: incompatible type in operation \"~\": %s", TypeString(Trees_Type((yyvsp[0].node))));
			YYABORT;
		}
		assert((yyval.node) != NULL);
	}
#line 2493 "y.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 975 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node designator, identType, actualParameters;
		int parameterListFound; /*possibly empty*/

		Trees_ReverseList(&(yyvsp[0].node)); /*correct order*/
		designator = Designator((yyvsp[-1].ident), (yyvsp[0].node));

		identType = Trees_Type(BaseIdent(designator));
		SetSelectorTypes(identType, designator, &parameterListFound);
		if (parameterListFound) {
			RemoveActualParameters(&designator, &actualParameters);
			(yyval.node) = Trees_NewNode(TREES_PROCEDURE_CALL, designator, actualParameters);
		} else {
			(yyval.node) = designator;
		}
	}
#line 2514 "y.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 995 "Oberon.y" /* yacc.c:1646  */
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
#line 2536 "y.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 1013 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = NULL;
	}
#line 2544 "y.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 1020 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node field;

		field = Trees_NewIdent((yyvsp[0].ident));
		Trees_SetKind(TREES_FIELD_KIND, field);
		(yyval.node) = Trees_NewNode('.', field, NULL);
	}
#line 2556 "y.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 1028 "Oberon.y" /* yacc.c:1646  */
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
#line 2578 "y.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 1046 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewNode('^', NULL, NULL);
	}
#line 2586 "y.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 1051 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_ReverseList(&(yyvsp[-1].node)); /*correct order*/
		(yyval.node) = Trees_NewNode('(', (yyvsp[-1].node), NULL);
	}
#line 2595 "y.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 1056 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewNode('(', NULL, NULL);
	}
#line 2603 "y.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 1063 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewSet(0x0u);
	}
#line 2611 "y.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 1067 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = (yyvsp[-1].node);
	}
#line 2619 "y.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 1075 "Oberon.y" /* yacc.c:1646  */
    {
		if ((Trees_Symbol((yyvsp[-2].node)) == TREES_SET_CONSTANT)
				&& (Trees_Symbol((yyvsp[0].node)) == TREES_SET_CONSTANT)) {
			(yyval.node) = Trees_NewSet(Trees_Set((yyvsp[-2].node)) | Trees_Set((yyvsp[0].node)));
		} else {
			(yyval.node) = Trees_NewNode('+', (yyvsp[-2].node), (yyvsp[0].node));
			Trees_SetType(Trees_NewLeaf(TREES_SET_TYPE), (yyval.node));
		}
	}
#line 2633 "y.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 1088 "Oberon.y" /* yacc.c:1646  */
    {
		OBNC_LONGI int i;
		Trees_Node type;

		CheckIsValueExpression((yyvsp[0].node));
		(yyval.node) = NULL;
		type = Trees_Type((yyvsp[0].node));
		if (IsInteger((yyvsp[0].node))) {
			i = Trees_Integer((yyvsp[0].node));
			Range_CheckSetElement(i);
			(yyval.node) = Trees_NewSet(1u << i);
		} else if (Types_IsInteger(type)) {
			(yyval.node) = Trees_NewNode(TREES_SINGLE_ELEMENT_SET, (yyvsp[0].node), NULL);
			Trees_SetType(Trees_NewLeaf(TREES_SET_TYPE), (yyval.node));
		} else {
			yyerror("error: element must have integer type");
			YYABORT;
		}
	}
#line 2657 "y.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 1108 "Oberon.y" /* yacc.c:1646  */
    {
		CheckIsValueExpression((yyvsp[-2].node));
		CheckIsValueExpression((yyvsp[0].node));
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
#line 2682 "y.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 1132 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewNode(TREES_EXP_LIST, (yyvsp[0].node), NULL);
		Trees_SetType(Trees_Type((yyvsp[0].node)), (yyval.node));
	}
#line 2691 "y.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 1137 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node reversedList;

		reversedList = Trees_NewNode(TREES_EXP_LIST, (yyvsp[0].node), (yyvsp[-2].node));
		(yyval.node) = reversedList;
		Trees_SetType(Trees_Type((yyvsp[0].node)), (yyval.node));
	}
#line 2703 "y.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 1158 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = NULL;
	}
#line 2711 "y.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 1165 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node designator, ident, designatorType, exp;

		CheckIsValueExpression((yyvsp[0].node));
		switch (Trees_Symbol((yyvsp[-2].node))) {
			case TREES_DESIGNATOR:
				designator = (yyvsp[-2].node);
				exp = (yyvsp[0].node);
				ident = BaseIdent((yyvsp[-2].node));
				designatorType = Trees_Type((yyvsp[-2].node));
				switch (Trees_Kind(ident)) {
					case TREES_VARIABLE_KIND:
					case TREES_VALUE_PARAM_KIND:
					case TREES_VAR_PARAM_KIND:
						if (Writable((yyvsp[-2].node))) {
							ValidateAssignment(exp, designatorType, ASSIGNMENT_CONTEXT, 0);
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
#line 2754 "y.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 1208 "Oberon.y" /* yacc.c:1646  */
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
		assert((yyval.node) != NULL);
	}
#line 2773 "y.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 1226 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_ReverseList(&(yyvsp[0].node)); /*correct order*/
		(yyval.node) = (yyvsp[0].node);
	}
#line 2782 "y.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 1234 "Oberon.y" /* yacc.c:1646  */
    {
		if ((yyvsp[0].node) == NULL) {
			(yyval.node) = NULL;
		} else {
			(yyval.node) = Trees_NewNode(TREES_STATEMENT_SEQUENCE, (yyvsp[0].node), NULL);
		}
	}
#line 2794 "y.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 1242 "Oberon.y" /* yacc.c:1646  */
    {
		if ((yyvsp[0].node) != NULL) {
			(yyval.node) = Trees_NewNode(TREES_STATEMENT_SEQUENCE, (yyvsp[0].node), (yyvsp[-2].node));
		} else {
			(yyval.node) = (yyvsp[-2].node);
		}
	}
#line 2806 "y.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 1253 "Oberon.y" /* yacc.c:1646  */
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
#line 2830 "y.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 1276 "Oberon.y" /* yacc.c:1646  */
    {
		CheckIsValueExpression((yyvsp[0].node));
		if (Types_IsBoolean(Trees_Type((yyvsp[0].node)))) {
			(yyval.node) = (yyvsp[0].node);
		} else {
			yyerror("error: boolean expression expected");
			YYABORT;
		}
	}
#line 2844 "y.tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 1289 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewNode(ELSIF, (yyvsp[-2].node), Trees_NewNode(THEN, (yyvsp[0].node), (yyvsp[-4].node)));
	}
#line 2852 "y.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 1293 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = NULL;
	}
#line 2860 "y.tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 1300 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewNode(ELSE, (yyvsp[0].node), NULL);
	}
#line 2868 "y.tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 1304 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = NULL;
	}
#line 2876 "y.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 1311 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node expType, caseVariable;

		if ((yyvsp[-1].node) != NULL) {
			Trees_ReverseList(&(yyvsp[-1].node)); /*correct order*/
		}
		assert(caseLabelsStack != NULL);
		caseLabelsStack = Trees_Right(caseLabelsStack);
		expType = Trees_Type((yyvsp[-3].node));
		if (Types_IsRecord(expType) || Types_IsPointer(expType)) {
			/*reset original type*/
			caseVariable = Trees_Left((yyvsp[-3].node));
			Trees_SetType(Trees_Type((yyvsp[-3].node)), caseVariable);
			caseExpressionStack = Trees_Right(caseExpressionStack);
		}
		(yyval.node) = Trees_NewNode(CASE, (yyvsp[-3].node), (yyvsp[-1].node));
	}
#line 2898 "y.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 1332 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node typeStruct, caseVariable;

		CheckIsValueExpression((yyvsp[0].node));
		typeStruct = Types_Structure(Trees_Type((yyvsp[0].node)));
		switch (Trees_Symbol(typeStruct)) {
			case RECORD:
				/*fall through*/
			case POINTER:
				if (IsDesignator((yyvsp[0].node)) && (FirstSelector((yyvsp[0].node)) == NULL)) {
					caseVariable = BaseIdent((yyvsp[0].node));
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
				caseExpressionStack = Trees_NewNode(TREES_NOSYM, (yyvsp[0].node), caseExpressionStack);
				caseLabelsStack = Trees_NewNode(TREES_NOSYM, NULL, caseLabelsStack);
				(yyval.node) = (yyvsp[0].node);
				break;
			default:
				yyerror("error: invalid type of CASE expression");
				YYABORT;
		}
	}
#line 2937 "y.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 1370 "Oberon.y" /* yacc.c:1646  */
    {
		if ((yyvsp[0].node) != NULL) {
			(yyval.node) = Trees_NewNode(TREES_CASE_REP, (yyvsp[0].node), NULL);
		} else {
			(yyval.node) = NULL;
		}
	}
#line 2949 "y.tab.c" /* yacc.c:1646  */
    break;

  case 114:
#line 1378 "Oberon.y" /* yacc.c:1646  */
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
#line 2965 "y.tab.c" /* yacc.c:1646  */
    break;

  case 115:
#line 1393 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_ReverseList(&(yyvsp[-2].node)); /*correct order*/
		(yyval.node) = Trees_NewNode(TREES_CASE, (yyvsp[-2].node), (yyvsp[0].node));
	}
#line 2974 "y.tab.c" /* yacc.c:1646  */
    break;

  case 116:
#line 1398 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = NULL;
	}
#line 2982 "y.tab.c" /* yacc.c:1646  */
    break;

  case 117:
#line 1405 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewNode(TREES_CASE_LABEL_LIST, (yyvsp[0].node), NULL);
	}
#line 2990 "y.tab.c" /* yacc.c:1646  */
    break;

  case 118:
#line 1409 "Oberon.y" /* yacc.c:1646  */
    {
		switch (Trees_Symbol((yyvsp[0].node))) {
			case INTEGER:
			case TREES_CHAR_CONSTANT:
			case DOTDOT:
				(yyval.node) = Trees_NewNode(TREES_CASE_LABEL_LIST, (yyvsp[0].node), (yyvsp[-2].node));
				break;
			default:
				yyerror("error: unexpected list of type name case labels");
				YYABORT;
		}
	}
#line 3007 "y.tab.c" /* yacc.c:1646  */
    break;

  case 119:
#line 1425 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = (yyvsp[0].node);
		CheckCaseLabelUniqueness((yyvsp[0].node));
		assert(caseLabelsStack != NULL);
		caseLabelsStack = Trees_NewNode(TREES_NOSYM,
			Trees_NewNode(TREES_NOSYM, (yyvsp[0].node), Trees_Left(caseLabelsStack)),
			Trees_Right(caseLabelsStack));
	}
#line 3020 "y.tab.c" /* yacc.c:1646  */
    break;

  case 120:
#line 1434 "Oberon.y" /* yacc.c:1646  */
    {
		const int rangeLenMax = 255;
		int leftSym, rightSym;
		OBNC_LONGI int rangeMin, rangeMax;

		leftSym = Trees_Symbol((yyvsp[-2].node));
		rightSym = Trees_Symbol((yyvsp[0].node));
		if (leftSym == rightSym) {
			switch (leftSym) {
				case INTEGER:
					rangeMin = Trees_Integer((yyvsp[-2].node));
					rangeMax = Trees_Integer((yyvsp[0].node));
					if (rangeMin <= rangeMax) {
						if (rangeMax - rangeMin > rangeLenMax) {
							yyerror("warning: maximum range length of %d exceeded", rangeLenMax);
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
		assert(caseLabelsStack != NULL);
		caseLabelsStack = Trees_NewNode(TREES_NOSYM,
			Trees_NewNode(TREES_NOSYM, (yyval.node), Trees_Left(caseLabelsStack)),
			Trees_Right(caseLabelsStack));
	}
#line 3068 "y.tab.c" /* yacc.c:1646  */
    break;

  case 121:
#line 1481 "Oberon.y" /* yacc.c:1646  */
    {
		if (Types_IsInteger(Trees_Type(Trees_Left(caseExpressionStack)))) {
			(yyval.node) = Trees_NewInteger((yyvsp[0].integer));
		} else {
			yyerror("error: unexpected integer case label");
			YYABORT;
		}
	}
#line 3081 "y.tab.c" /* yacc.c:1646  */
    break;

  case 122:
#line 1490 "Oberon.y" /* yacc.c:1646  */
    {
		if (Types_IsChar(Trees_Type(Trees_Left(caseExpressionStack)))) {
			if (strlen((yyvsp[0].string)) <= 1) {
				(yyval.node) = Trees_NewChar((yyvsp[0].string)[0]);
			} else {
				yyerror("error: single-character string expected: \"%s\"", (yyvsp[0].string));
				YYABORT;
			}
		} else {
			yyerror("error: unexpected string case label: \"%s\"", (yyvsp[0].string));
			YYABORT;
		}
	}
#line 3099 "y.tab.c" /* yacc.c:1646  */
    break;

  case 123:
#line 1504 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node caseExp, constValue, caseVariable;

		(yyval.node) = Table_At(Trees_Name((yyvsp[0].node)));
		if ((yyval.node) != NULL) {
			caseExp = Trees_Left(caseExpressionStack);
			switch (Trees_Symbol(Types_Structure(Trees_Type(caseExp)))) {
				case TREES_INTEGER_TYPE:
					if (Trees_Kind((yyval.node)) == TREES_CONSTANT_KIND) {
						constValue = Trees_Value((yyval.node));
						if (Trees_Symbol(constValue) == INTEGER) {
							if (Trees_Integer(constValue) >= 0) {
								(yyval.node) = constValue;
							} else {
								yyerror("error: non-negative case label expected: %" OBNC_INT_MOD "d", Trees_Integer(constValue));
								YYABORT;
							}
						} else {
							yyerror("error: integer case label expected");
							YYABORT;
						}
					} else {
						yyerror("error: constant identifier expected: %s", Trees_Name((yyval.node)));
						YYABORT;
					}
					break;
				case TREES_CHAR_TYPE:
					if (Trees_Kind((yyval.node)) == TREES_CONSTANT_KIND) {
						constValue = Trees_Value((yyval.node));
						if (Trees_Symbol(constValue) == STRING) {
							if (Types_StringLength(Trees_Type(constValue)) <= 1) {
								(yyval.node) = Trees_NewChar(Trees_String(constValue)[0]);
							} else {
								yyerror("error: single-character string expected: %s", Trees_String(constValue));
								YYABORT;
							}
						} else {
							yyerror("error: character case label expected");
							YYABORT;
						}
					} else {
						yyerror("error: constant identifier expected: %s", Trees_Name((yyval.node)));
						YYABORT;
					}
					break;
				case RECORD:
					if (Types_IsType((yyval.node)) && Types_IsRecord((yyval.node))) {
						if (Types_Extends(Trees_Type(caseExp), (yyval.node))) {
							caseVariable = Trees_Left(caseExp);
							Trees_SetType((yyval.node), caseVariable);
						} else {
							yyerror("error: case label is not an extension of %s: %s", Trees_Name(Trees_Type(caseExp)), Trees_Name((yyval.node)));
							YYABORT;
						}
					} else {
						yyerror("error: record type case label expected");
						YYABORT;
					}
					break;
				case POINTER:
					if (Types_IsType((yyval.node)) && Types_IsPointer((yyval.node))) {
						if (Types_Extends(Trees_Type(caseExp), (yyval.node))) {
							caseVariable = Trees_Left(caseExp);
							Trees_SetType((yyval.node), caseVariable);
						} else {
							yyerror("error: case label is not an extension of %s: %s", Trees_Name(Trees_Type(caseExp)), Trees_Name((yyval.node)));
							YYABORT;
						}
					} else {
						yyerror("error: pointer type case label expected");
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
#line 3185 "y.tab.c" /* yacc.c:1646  */
    break;

  case 124:
#line 1589 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewNode(WHILE, (yyvsp[-4].node), Trees_NewNode(DO, (yyvsp[-2].node), (yyvsp[-1].node)));
	}
#line 3193 "y.tab.c" /* yacc.c:1646  */
    break;

  case 125:
#line 1596 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewNode(ELSIF, (yyvsp[-2].node), Trees_NewNode(THEN, (yyvsp[0].node), (yyvsp[-4].node)));
	}
#line 3201 "y.tab.c" /* yacc.c:1646  */
    break;

  case 126:
#line 1600 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = NULL;
	}
#line 3209 "y.tab.c" /* yacc.c:1646  */
    break;

  case 127:
#line 1607 "Oberon.y" /* yacc.c:1646  */
    {
		CheckIsValueExpression((yyvsp[0].node));
		(yyval.node) = NULL;
		if (Types_IsBoolean(Trees_Type((yyvsp[0].node)))) {
			(yyval.node) = Trees_NewNode(REPEAT, (yyvsp[-2].node), (yyvsp[0].node));
		} else {
			yyerror("error: boolean expression expected");
			YYABORT;
		}
	}
#line 3224 "y.tab.c" /* yacc.c:1646  */
    break;

  case 128:
#line 1622 "Oberon.y" /* yacc.c:1646  */
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
#line 3243 "y.tab.c" /* yacc.c:1646  */
    break;

  case 129:
#line 1640 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node ctrlVar, ctrlVarType;

		CheckIsValueExpression((yyvsp[0].node));
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
#line 3271 "y.tab.c" /* yacc.c:1646  */
    break;

  case 130:
#line 1666 "Oberon.y" /* yacc.c:1646  */
    {
		CheckIsValueExpression((yyvsp[0].node));
		if (! Types_IsInteger(Trees_Type((yyvsp[0].node)))) {
			yyerror("error: integer expression expected as upper limit");
			YYABORT;
		}
	}
#line 3283 "y.tab.c" /* yacc.c:1646  */
    break;

  case 131:
#line 1677 "Oberon.y" /* yacc.c:1646  */
    {
		if (Types_IsInteger(Trees_Type((yyvsp[0].node)))) {
			if (IsInteger((yyvsp[0].node))) {
				if (Trees_Integer((yyvsp[0].node)) == 0) {
					yyerror("warning: steps by zero leads to infinite loop");
				}
				(yyval.node) = (yyvsp[0].node);
			} else {
				yyerror("error: fully evaluated constant expression expected as increment");
				YYABORT;
			}
		} else {
			yyerror("error: integer increment expected");
			YYABORT;
		}
	}
#line 3304 "y.tab.c" /* yacc.c:1646  */
    break;

  case 132:
#line 1694 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = NULL;
	}
#line 3312 "y.tab.c" /* yacc.c:1646  */
    break;

  case 133:
#line 1704 "Oberon.y" /* yacc.c:1646  */
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
					CheckIsValueExpression(returnExp);
					ValidateAssignment(returnExp, resultType, PROC_RESULT_CONTEXT, 0);
					if ((Trees_Symbol(returnExp) == STRING) && Types_IsChar(resultType)) {
						returnExp = Trees_NewChar(Trees_String(returnExp)[0]);
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
#line 3362 "y.tab.c" /* yacc.c:1646  */
    break;

  case 134:
#line 1753 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node paramList, param;

		(yyval.node) = NULL;
		Trees_SetType((yyvsp[0].node), (yyvsp[-1].node));

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
#line 3384 "y.tab.c" /* yacc.c:1646  */
    break;

  case 135:
#line 1774 "Oberon.y" /* yacc.c:1646  */
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
#line 3400 "y.tab.c" /* yacc.c:1646  */
    break;

  case 136:
#line 1789 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = (yyvsp[0].node);
	}
#line 3408 "y.tab.c" /* yacc.c:1646  */
    break;

  case 137:
#line 1793 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = NULL;
	}
#line 3416 "y.tab.c" /* yacc.c:1646  */
    break;

  case 138:
#line 1800 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = (yyvsp[0].node);
	}
#line 3424 "y.tab.c" /* yacc.c:1646  */
    break;

  case 139:
#line 1804 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = NULL;
	}
#line 3432 "y.tab.c" /* yacc.c:1646  */
    break;

  case 145:
#line 1825 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node unresolvedPointerType, undeclaredBaseType;

		if (unresolvedPointerTypes != NULL) {
			unresolvedPointerType = Trees_Left(unresolvedPointerTypes);
			undeclaredBaseType = Types_PointerBaseType(unresolvedPointerType);
			yyerror("error: undeclared pointer base type: %s", Trees_Name(undeclaredBaseType));
			YYABORT;
		}
	}
#line 3447 "y.tab.c" /* yacc.c:1646  */
    break;

  case 146:
#line 1836 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = NULL;
	}
#line 3455 "y.tab.c" /* yacc.c:1646  */
    break;

  case 147:
#line 1843 "Oberon.y" /* yacc.c:1646  */
    {
		unresolvedPointerTypes = NULL;
	}
#line 3463 "y.tab.c" /* yacc.c:1646  */
    break;

  case 156:
#line 1870 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Types_NewProcedure((yyvsp[-2].node), (yyvsp[0].node));
	}
#line 3471 "y.tab.c" /* yacc.c:1646  */
    break;

  case 157:
#line 1877 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_ReverseList(&(yyvsp[0].node)); /*correct order*/
		(yyval.node) = (yyvsp[0].node);
	}
#line 3480 "y.tab.c" /* yacc.c:1646  */
    break;

  case 158:
#line 1882 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = NULL;
	}
#line 3488 "y.tab.c" /* yacc.c:1646  */
    break;

  case 159:
#line 1889 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = (yyvsp[0].node);
		Trees_ReverseList(&(yyval.node));
	}
#line 3497 "y.tab.c" /* yacc.c:1646  */
    break;

  case 160:
#line 1894 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node p, p1;
		const char *paramName, *paramName1;

		/*make sure no parameter is repeated*/
		p = (yyvsp[0].node);
		while (p != NULL) {
			paramName = Trees_Name(Trees_Left(p));
			p1 = (yyvsp[-2].node);
			while (p1 != NULL) {
				paramName1 = Trees_Name(Trees_Left(p1));
				if (strcmp(paramName1, paramName) == 0) {
					yyerror("error: repeated parameter: %s", paramName);
					YYABORT;
				}
				p1 = Trees_Right(p1);
			}
			p = Trees_Right(p);
		}

		/*make one list of the two lists*/
		(yyval.node) = (yyvsp[-2].node);
		p = (yyvsp[0].node);
		do {
			(yyval.node) = Trees_NewNode(TREES_IDENT_LIST, Trees_Left(p), (yyval.node));
			p = Trees_Right(p);
		} while (p != NULL);
		/*$$ in reversed order*/
	}
#line 3531 "y.tab.c" /* yacc.c:1646  */
    break;

  case 161:
#line 1927 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = ResolvedType((yyvsp[0].node), 0);
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
#line 3554 "y.tab.c" /* yacc.c:1646  */
    break;

  case 162:
#line 1946 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = NULL;
	}
#line 3562 "y.tab.c" /* yacc.c:1646  */
    break;

  case 163:
#line 1953 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node curr, ident;

		Trees_ReverseList(&(yyvsp[-2].node)); /*correct order*/
		curr = (yyvsp[-2].node);
		do {
			ident = Trees_Left(curr);
			Trees_SetKind((int) (yyvsp[-3].integer), ident);
			Trees_SetType((yyvsp[0].node), ident);
			Trees_SetLocal(ident);
			curr = Trees_Right(curr);
		} while (curr != NULL);

		(yyval.node) = (yyvsp[-2].node);
	}
#line 3582 "y.tab.c" /* yacc.c:1646  */
    break;

  case 164:
#line 1972 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = TREES_VAR_PARAM_KIND;
	}
#line 3590 "y.tab.c" /* yacc.c:1646  */
    break;

  case 165:
#line 1976 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.integer) = TREES_VALUE_PARAM_KIND;
	}
#line 3598 "y.tab.c" /* yacc.c:1646  */
    break;

  case 166:
#line 1983 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewNode(TREES_IDENT_LIST, Trees_NewIdent((yyvsp[0].ident)), NULL);
	}
#line 3606 "y.tab.c" /* yacc.c:1646  */
    break;

  case 167:
#line 1987 "Oberon.y" /* yacc.c:1646  */
    {
		Trees_Node curr;
		const char *identName;

		/*make sure no name is repeated*/
		curr = (yyvsp[-2].node);
		while (curr != NULL) {
			identName = Trees_Name(Trees_Left(curr));
			if (strcmp(identName, (yyvsp[0].ident)) == 0) {
				yyerror("error: repeated identifier: %s", identName);
				YYABORT;
			}
			curr = Trees_Right(curr);
		}

		(yyval.node) = Trees_NewNode(TREES_IDENT_LIST, Trees_NewIdent((yyvsp[0].ident)), (yyvsp[-2].node));
	}
#line 3628 "y.tab.c" /* yacc.c:1646  */
    break;

  case 168:
#line 2008 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = ResolvedType((yyvsp[0].node), 0);
		if ((yyval.node) != NULL) {
			while ((yyvsp[-1].node) != NULL) {
				(yyval.node) = Types_NewArray(NULL, (yyval.node));
				(yyvsp[-1].node) = Trees_Right((yyvsp[-1].node));
			}
		} else {
			yyerror("error: undeclared identifier: %s", Trees_Name((yyvsp[0].node)));
			exit(EXIT_FAILURE);
		}
	}
#line 3645 "y.tab.c" /* yacc.c:1646  */
    break;

  case 169:
#line 2024 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = Trees_NewNode(ARRAY, NULL, (yyvsp[-2].node));
	}
#line 3653 "y.tab.c" /* yacc.c:1646  */
    break;

  case 170:
#line 2028 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.node) = NULL;
	}
#line 3661 "y.tab.c" /* yacc.c:1646  */
    break;

  case 171:
#line 2038 "Oberon.y" /* yacc.c:1646  */
    {
		const char *symfilePath;

		if (strcmp((yyvsp[-1].ident), inputModuleName) == 0) {
			Generate_ModuleEnd();
			Generate_Close();

			symfilePath = Util_String(".obnc/%s.sym", inputModuleName);
			if (parseMode == OBERON_ENTRY_POINT_MODE) {
				if (Files_Exists(symfilePath)) {
					Files_Remove(symfilePath);
				}
			} else {
				ExportSymbolTable(symfilePath);
			}
			YYACCEPT;
		} else {
			yyerror("error: expected identifier %s", inputModuleName);
			YYABORT;
		}
	}
#line 3687 "y.tab.c" /* yacc.c:1646  */
    break;

  case 172:
#line 2064 "Oberon.y" /* yacc.c:1646  */
    {
		if (strcmp((yyvsp[0].ident), inputModuleName) == 0) {
			if (parseMode != OBERON_IMPORT_LIST_MODE) {
				Generate_ModuleHeading();
			}
		} else {
			yyerror("error: module name does not match filename: %s", (yyvsp[0].ident));
			YYABORT;
		}
	}
#line 3702 "y.tab.c" /* yacc.c:1646  */
    break;

  case 173:
#line 2078 "Oberon.y" /* yacc.c:1646  */
    {
		if (parseMode == OBERON_IMPORT_LIST_MODE) {
			YYACCEPT;
		}
	}
#line 3712 "y.tab.c" /* yacc.c:1646  */
    break;

  case 174:
#line 2084 "Oberon.y" /* yacc.c:1646  */
    {
		if (parseMode == OBERON_IMPORT_LIST_MODE) {
			YYACCEPT;
		}
	}
#line 3722 "y.tab.c" /* yacc.c:1646  */
    break;

  case 175:
#line 2093 "Oberon.y" /* yacc.c:1646  */
    {
		const char *impfilePath;
		Trees_Node moduleAndDirPath, module, p;
		FILE *impFile;
		const char *name;

		if ((yyvsp[-1].node) != NULL) {
			Trees_ReverseList(&(yyvsp[-1].node)); /*correct order*/
			if (parseMode == OBERON_IMPORT_LIST_MODE) {
				while ((yyvsp[-1].node) != NULL) {
					name = Trees_Name(Trees_Left((yyvsp[-1].node)));
					puts(name);
					(yyvsp[-1].node) = Trees_Right((yyvsp[-1].node));
				}
			} else {
				if (parseMode == OBERON_NORMAL_MODE) {
					impfilePath = Util_String(".obnc/%s.imp", inputModuleName);
					impFile = Files_Exists(impfilePath)? Files_Old(impfilePath, FILES_WRITE): Files_New(impfilePath);
					p = (yyvsp[-1].node);
					do {
						moduleAndDirPath = Trees_Left(p);
						module = Trees_Left(moduleAndDirPath);
						name = Trees_UnaliasedName(module);
						fprintf(impFile, "%s\n", name);
						p = Trees_Right(p);
					} while (p != NULL);
					Files_Close(&impFile);
				}
				Generate_ImportList((yyvsp[-1].node));
			}
		}
	}
#line 3759 "y.tab.c" /* yacc.c:1646  */
    break;

  case 176:
#line 2129 "Oberon.y" /* yacc.c:1646  */
    {
		if ((yyvsp[0].node) != NULL) {
			(yyval.node) = Trees_NewNode(TREES_NOSYM, (yyvsp[0].node), NULL);
		} else {
			(yyval.node) = NULL;
		}
	}
#line 3771 "y.tab.c" /* yacc.c:1646  */
    break;

  case 177:
#line 2137 "Oberon.y" /* yacc.c:1646  */
    {
		if ((yyvsp[0].node) != NULL) {
			(yyval.node) = Trees_NewNode(TREES_NOSYM, (yyvsp[0].node), (yyvsp[-2].node));
		} else {
			(yyval.node) = (yyvsp[-2].node);
		}
	}
#line 3783 "y.tab.c" /* yacc.c:1646  */
    break;

  case 178:
#line 2148 "Oberon.y" /* yacc.c:1646  */
    {
		static Maps_Map importedModules = NULL;
		const char *module, *qualifier, *symbolFileDir, *symbolFileName, *moduleDirPath;
		Trees_Node qualifierSym, moduleIdent;

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
		if (strcmp(module, inputModuleName) != 0) {
			if (! Maps_HasKey(module, importedModules)) {
				Maps_Put(module, NULL, &importedModules);
				qualifierSym = Table_At(qualifier);
				if (qualifierSym == NULL) {
					qualifierSym = Trees_NewIdent(qualifier);
					if ((yyvsp[0].ident) != NULL) {
						Trees_SetUnaliasedName(module, qualifierSym);
					}
					Trees_SetKind(TREES_QUALIFIER_KIND, qualifierSym);
					Table_Put(qualifierSym);

					if (strcmp(module, "SYSTEM") == 0) {
						if (parseMode != OBERON_IMPORT_LIST_MODE) {
							Table_ImportSystem(qualifier);
						}
					} else if (parseMode == OBERON_IMPORT_LIST_MODE) {
						(yyval.node) = Trees_NewIdent(module);
					} else {
						moduleDirPath = ModulePaths_Directory(module, ".");
						if (moduleDirPath != NULL) {
							/*import identifiers into the symbol table*/
							symbolFileDir = Util_String("%s/.obnc", moduleDirPath);
							if (! Files_Exists(symbolFileDir)) {
								symbolFileDir = Util_String("%s", moduleDirPath);
							}
							symbolFileName = Util_String("%s/%s.sym", symbolFileDir, module);
							if (Files_Exists(symbolFileName)) {
								Table_Import(symbolFileName, module, qualifier);
							} else {
								yyerror("error: symbol file not found for module %s: %s", module, symbolFileName);
								YYABORT;
							}

							moduleIdent = Trees_NewIdent(module);
							Trees_SetKind(TREES_QUALIFIER_KIND, moduleIdent);
							(yyval.node) = Trees_NewNode(TREES_NOSYM, moduleIdent, Trees_NewString(moduleDirPath));
						} else {
							yyerror("error: imported module not found: %s", module);
							YYABORT;
						}
					}
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
#line 3859 "y.tab.c" /* yacc.c:1646  */
    break;

  case 179:
#line 2223 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.ident) = (yyvsp[0].ident);
	}
#line 3867 "y.tab.c" /* yacc.c:1646  */
    break;

  case 180:
#line 2227 "Oberon.y" /* yacc.c:1646  */
    {
		(yyval.ident) = NULL;
	}
#line 3875 "y.tab.c" /* yacc.c:1646  */
    break;

  case 181:
#line 2234 "Oberon.y" /* yacc.c:1646  */
    {
		Generate_ModuleStatements((yyvsp[0].node));
	}
#line 3883 "y.tab.c" /* yacc.c:1646  */
    break;


#line 3887 "y.tab.c" /* yacc.c:1646  */
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
#line 2239 "Oberon.y" /* yacc.c:1906  */


void Oberon_Init(void)
{
	static int initialized = 0;

	if (! initialized) {
		Error_Init();
		Files_Init();
		ModulePaths_Init();
		Table_Init();
		initialized = 1;
	}
}


static char *ModuleName(const char filename[])
{
	char *base, *suffix;

	base = Paths_Basename(filename);
	suffix = strchr(base, '.');
	if (suffix != NULL) {
		*suffix = '\0';
	}
	return base;
}


void Oberon_Parse(const char inputFile[], int mode)
{
	const char *impFile;
	FILE *fp;
	int error;

	inputFilename = inputFile;
	parseMode = mode;
	inputModuleName = ModuleName(inputFile);

	yyin = fopen(inputFile, "r");
	if (yyin != NULL) {
		if (mode != OBERON_IMPORT_LIST_MODE) {
			Generate_Open(inputModuleName, mode == OBERON_ENTRY_POINT_MODE);

			impFile = Util_String(".obnc/%s.imp", inputModuleName);
			if (parseMode == OBERON_NORMAL_MODE) {
				if (! Files_Exists(impFile)) {
					fp = Files_New(impFile);
					Files_Close(&fp);
				}
			} else {
				assert(parseMode == OBERON_ENTRY_POINT_MODE);
				if (Files_Exists(impFile)) {
					Files_Remove(impFile);
				}
			}
		}
		error = yyparse();
		if (error) {
			exit(EXIT_FAILURE);
		}
	} else {
		Error_Handle(Util_String("error: cannot open file: %s: %s", inputFile, strerror(errno)));
	}
}


void yyerror(const char format[], ...)
{
	va_list ap;

	fprintf(stderr, "obnc-compile: %s:%d: ", inputFilename, yylineno);
	va_start(ap, format);
	vfprintf(stderr, format, ap);
	va_end(ap);
	fputc('\n', stderr);
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

static Trees_Node ResolvedType(Trees_Node type, int isTypeDecl)
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
					if (Types_Basic(Trees_Type(identDef)) && ! isTypeDecl) {
						result = Trees_Type(identDef);
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


static const char *TypeString(Trees_Node type)
{
	const char *result = Util_String("%s", "");

	assert(Types_IsType(type));

	switch (Trees_Symbol(Types_Structure(type))) {
		case TREES_STRING_TYPE:
			switch (Types_StringLength(type)) {
				case 0:
					result = Util_String("empty string");
					break;
				case 1:
					result = Util_String("single-char string");
					break;
				default:
					result = Util_String("multi-char string");
			}
			break;
		case TREES_BOOLEAN_TYPE:
			result = Util_String("BOOLEAN");
			break;
		case TREES_CHAR_TYPE:
			result = Util_String("CHAR");
			break;
		case TREES_INTEGER_TYPE:
			result = Util_String("INTEGER");
			break;
		case TREES_REAL_TYPE:
			result = Util_String("REAL");
			break;
		case TREES_BYTE_TYPE:
			result = Util_String("BYTE");
			break;
		case TREES_SET_TYPE:
			result = Util_String("SET");
			break;
		case ARRAY:
			if (Types_IsOpenArray(type)) {
				result = Util_String("open array type");
			} else {
				if (Trees_Symbol(type) != IDENT) {
					result = Util_String("anonymous ");
				}
				result = Util_String("array type");
			}
			break;
		case RECORD:
			if (Trees_Symbol(type) != IDENT) {
				result = Util_String("anonymous ");
			}
			result = Util_String("record type");
			break;
		case POINTER:
			if (Trees_Symbol(type) != IDENT) {
				result = Util_String("anonymous ");
			}
			result = Util_String("pointer type");
			break;
		case PROCEDURE:
			if (Trees_Symbol(type) != IDENT) {
				result = Util_String("anonymous ");
			}
			result = Util_String("procedure type");
			break;
		case TREES_NIL_TYPE:
			result = Util_String("NIL");
			break;
		default:
			assert(0);
	}
	if (Trees_Symbol(type) == IDENT) {
		result = Util_String("%s (%s)", Trees_Name(type), result);
	}
	return result;
}


/*functions for expression productions*/

static int IsDesignator(Trees_Node exp)
{
	return Trees_Symbol(exp) == TREES_DESIGNATOR;
}


static int IsValueExpression(Trees_Node exp)
{
	int result = 1;

	if (IsDesignator(exp)) {
		switch (Trees_Kind(BaseIdent(exp))) {
			case TREES_CONSTANT_KIND:
			case TREES_FIELD_KIND:
			case TREES_VARIABLE_KIND:
			case TREES_PROCEDURE_KIND:
			case TREES_VALUE_PARAM_KIND:
			case TREES_VAR_PARAM_KIND:
				break;
			default:
				result = 0;
		}
	}
	return result;
}


static void CheckIsValueExpression(Trees_Node exp)
{
	if (! IsValueExpression(exp)) {
		yyerror("error: value expected: %s", Trees_Name(BaseIdent(exp)));
		exit(EXIT_FAILURE);
	}
}


static Trees_Node Designator(const char identName[], Trees_Node selectorList)
{
	Trees_Node identSym, qualidentSym, designator, qualidentSelectorList;
	const char *qualidentName;

	/*set qualident name, symbol and selector list*/
	qualidentSym = NULL;
	qualidentSelectorList = NULL;
	identSym = Table_At(identName);
	if ((identSym == NULL) && (procedureDeclarationStack != NULL)
			&& (strcmp(identName, Trees_Name(Trees_Left(procedureDeclarationStack))) == 0)) {
		qualidentSym = Trees_Left(procedureDeclarationStack);
		qualidentSelectorList = selectorList;
	} else {
		if (identSym != NULL) {
			if (Trees_Kind(identSym) == TREES_QUALIFIER_KIND) {
				if ((selectorList != NULL) && (Trees_Symbol(selectorList) == '.')) {
					qualidentName = Util_String("%s.%s", identName, Trees_Name(Trees_Left(selectorList)));
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


static Trees_Node BaseIdent(Trees_Node designator)
{
	assert(Trees_Symbol(designator) == TREES_DESIGNATOR);

	return Trees_Left(designator);
}


static Trees_Node FirstSelector(Trees_Node designator)
{
	assert(Trees_Symbol(designator) == TREES_DESIGNATOR);

	return Trees_Right(designator);
}


static void SetSelectorTypes(Trees_Node identType, Trees_Node designator, int *parameterListFound)
{
	Trees_Node currType, currTypeStruct, currSelector, prevSelector, indexExp, lengthNode, pointerNode, expList, extendedType, symbol, varField, typeField, fieldBaseType;
	OBNC_LONGI int length, index;
	const char *fieldName;

	currType = identType;
	currSelector = FirstSelector(designator);
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
							yyerror("error: invalid array index: %" OBNC_INT_MOD "d not between 0 and %" OBNC_INT_MOD "d", index, length - 1);
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
							Types_GetFieldIdent(fieldName, currType, Trees_Imported(BaseIdent(designator)), &typeField, &fieldBaseType);
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
									if ((Types_IsRecord(currType) && Types_IsRecord(Trees_Type(symbol))
												&& (Trees_Kind(BaseIdent(designator)) == TREES_VAR_PARAM_KIND))
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
											if (Trees_Kind(BaseIdent(designator)) != TREES_VAR_PARAM_KIND) {
												yyerror("error: variable parameter expected in type guard");
											} else {
												yyerror("error: record type expected in type guard: %s", Trees_Name(extendedType));
											}
											exit(EXIT_FAILURE);
										} else {
											yyerror("error: pointer type expected in type guard: %s", Trees_Name(extendedType));
											exit(EXIT_FAILURE);
										}
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

	currSelector = FirstSelector(*designator);
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


static int IsConstExpression(Trees_Node exp)
{
	int result = 0;

	assert(exp != NULL);
	switch (Trees_Symbol(exp)) {
		case TRUE:
		case FALSE:
		case STRING:
		case TREES_CHAR_CONSTANT:
		case INTEGER:
		case REAL:
		case TREES_SET_CONSTANT:
		case NIL:
		case TREES_SIZE_PROC: /*type sizes cannot always be evaluated in the Oberon-to-C translation*/
			result = 1;
			break;
		case IDENT:
		case TREES_DESIGNATOR:
		case TREES_ADR_PROC:
		case TREES_BIT_PROC:
			result = 0;
			break;
		default:
			result = ((Trees_Left(exp) == NULL) || IsConstExpression(Trees_Left(exp)))
				&& ((Trees_Right(exp) == NULL) || IsConstExpression(Trees_Right(exp)));
	}
	return result;
}


static Trees_Node ExpressionConstValue(int relation, Trees_Node expA, Trees_Node expB)
{
	Trees_Node result = NULL;

	switch (relation) {
		case '=':
			switch (Trees_Symbol(expA)) {
				case TRUE:
				case FALSE:
					if (IsBoolean(expB)) {
						result = Trees_NewLeaf((Trees_Symbol(expA) == Trees_Symbol(expB))? TRUE: FALSE);
					}
					break;
				case TREES_CHAR_CONSTANT:
					if (IsString(expB)) {
						result = Trees_NewLeaf((Trees_Char(expA) == Trees_String(expB)[0])? TRUE: FALSE);
					} else if (IsChar(expB)) {
						result = Trees_NewLeaf((Trees_Char(expA) == Trees_Char(expB))? TRUE: FALSE);
					}
					break;
				case INTEGER:
					if (IsInteger(expB)) {
						result = Trees_NewLeaf((Trees_Integer(expA) == Trees_Integer(expB))? TRUE: FALSE);
					}
					break;
				case REAL:
					if (IsReal(expB)) {
						result = Trees_NewLeaf((Trees_Real(expA) == Trees_Real(expB))? TRUE: FALSE);
					}
					break;
				case TREES_SET_CONSTANT:
					if (IsSet(expB)) {
						result = Trees_NewLeaf((Trees_Set(expA) == Trees_Set(expB))? TRUE: FALSE);
					}
					break;
				case STRING:
					if (IsChar(expB)) {
						result = Trees_NewLeaf((Trees_String(expA)[0] == Trees_Char(expB))? TRUE: FALSE);
					} else if (IsString(expB)) {
						result = Trees_NewLeaf((strcmp(Trees_String(expA), Trees_String(expB)) == 0)? TRUE: FALSE);
					}
					break;
			}
			break;
		case '#':
			switch (Trees_Symbol(expA)) {
				case TRUE:
				case FALSE:
					if (IsBoolean(expB)) {
						result = Trees_NewLeaf((Trees_Symbol(expA) != Trees_Symbol(expB))? TRUE: FALSE);
					}
					break;
				case TREES_CHAR_CONSTANT:
					if (IsString(expB)) {
						result = Trees_NewLeaf((Trees_Char(expA) != Trees_String(expB)[0])? TRUE: FALSE);
					} else if (IsChar(expB)) {
						result = Trees_NewLeaf((Trees_Char(expA) != Trees_Char(expB))? TRUE: FALSE);
					}
					break;
				case INTEGER:
					if (IsInteger(expB)) {
						result = Trees_NewLeaf((Trees_Integer(expA) != Trees_Integer(expB))? TRUE: FALSE);
					}
					break;
				case REAL:
					if (IsReal(expB)) {
						result = Trees_NewLeaf((Trees_Real(expA) != Trees_Real(expB))? TRUE: FALSE);
					}
					break;
				case TREES_SET_CONSTANT:
					if (IsSet(expB)) {
						result = Trees_NewLeaf((Trees_Set(expA) != Trees_Set(expB))? TRUE: FALSE);
					}
					break;
				case STRING:
					if (IsChar(expB)) {
						result = Trees_NewLeaf((Trees_String(expA)[0] != Trees_Char(expB))? TRUE: FALSE);
					} else if (IsString(expB)) {
						result = Trees_NewLeaf((strcmp(Trees_String(expA), Trees_String(expB)) != 0)? TRUE: FALSE);
					}
					break;
			}
			break;
		case '<':
			switch (Trees_Symbol(expA)) {
				case TREES_CHAR_CONSTANT:
					if (IsString(expB)) {
						result = Trees_NewLeaf((Trees_Char(expA) < Trees_String(expB)[0])? TRUE: FALSE);
					} else if (IsChar(expB)) {
						result = Trees_NewLeaf((Trees_Char(expA) < Trees_Char(expB))? TRUE: FALSE);
					}
					break;
				case INTEGER:
					if (IsInteger(expB)) {
						result = Trees_NewLeaf((Trees_Integer(expA) < Trees_Integer(expB))? TRUE: FALSE);
					}
					break;
				case REAL:
					if (IsReal(expB)) {
						result = Trees_NewLeaf((Trees_Real(expA) < Trees_Real(expB))? TRUE: FALSE);
					}
					break;
				case STRING:
					if (IsChar(expB)) {
						result = Trees_NewLeaf((Trees_String(expA)[0] < Trees_Char(expB))? TRUE: FALSE);
					} else if (IsString(expB)) {
						result = Trees_NewLeaf((strcmp(Trees_String(expA), Trees_String(expB)) < 0)? TRUE: FALSE);
					}
					break;
			}
			break;
		case LE:
			switch (Trees_Symbol(expA)) {
				case TREES_CHAR_CONSTANT:
					if (IsString(expB)) {
						result = Trees_NewLeaf((Trees_Char(expA) <= Trees_String(expB)[0])? TRUE: FALSE);
					} else if (IsChar(expB)) {
						result = Trees_NewLeaf((Trees_Char(expA) <= Trees_Char(expB))? TRUE: FALSE);
					}
					break;
				case INTEGER:
					if (IsInteger(expB)) {
						result = Trees_NewLeaf((Trees_Integer(expA) <= Trees_Integer(expB))? TRUE: FALSE);
					}
					break;
				case REAL:
					if (IsReal(expB)) {
						result = Trees_NewLeaf((Trees_Real(expA) <= Trees_Real(expB))? TRUE: FALSE);
					}
					break;
				case STRING:
					if (IsChar(expB)) {
						result = Trees_NewLeaf((Trees_String(expA)[0] <= Trees_Char(expB))? TRUE: FALSE);
					} else if (IsString(expB)) {
						result = Trees_NewLeaf((strcmp(Trees_String(expA), Trees_String(expB)) <= 0)? TRUE: FALSE);
					}
					break;
			}
			break;
		case '>':
			switch (Trees_Symbol(expA)) {
				case TREES_CHAR_CONSTANT:
					if (IsString(expB)) {
						result = Trees_NewLeaf((Trees_Char(expA) > Trees_String(expB)[0])? TRUE: FALSE);
					} else if (IsChar(expB)) {
						result = Trees_NewLeaf((Trees_Char(expA) > Trees_Char(expB))? TRUE: FALSE);
					}
					break;
				case INTEGER:
					if (IsInteger(expB)) {
						result = Trees_NewLeaf((Trees_Integer(expA) > Trees_Integer(expB))? TRUE: FALSE);
					}
					break;
				case REAL:
					if (IsReal(expB)) {
						result = Trees_NewLeaf((Trees_Real(expA) > Trees_Real(expB))? TRUE: FALSE);
					}
					break;
				case STRING:
					if (IsChar(expB)) {
						result = Trees_NewLeaf((Trees_String(expA)[0] > Trees_Char(expB))? TRUE: FALSE);
					} else if (IsString(expB)) {
						result = Trees_NewLeaf((strcmp(Trees_String(expA), Trees_String(expB)) > 0)? TRUE: FALSE);
					}
					break;
			}
			break;
		case GE:
			switch (Trees_Symbol(expA)) {
				case TREES_CHAR_CONSTANT:
					if (IsString(expB)) {
						result = Trees_NewLeaf((Trees_Char(expA) >= Trees_String(expB)[0])? TRUE: FALSE);
					} else if (IsChar(expB)) {
						result = Trees_NewLeaf((Trees_Char(expA) >= Trees_Char(expB))? TRUE: FALSE);
					}
					break;
				case INTEGER:
					if (IsInteger(expB)) {
						result = Trees_NewLeaf((Trees_Integer(expA) >= Trees_Integer(expB))? TRUE: FALSE);
					}
					break;
				case REAL:
					if (IsReal(expB)) {
						result = Trees_NewLeaf((Trees_Real(expA) >= Trees_Real(expB))? TRUE: FALSE);
					}
					break;
				case STRING:
					if (IsChar(expB)) {
						result = Trees_NewLeaf((Trees_String(expA)[0] >= Trees_Char(expB))? TRUE: FALSE);
					} else if (IsString(expB)) {
						result = Trees_NewLeaf((strcmp(Trees_String(expA), Trees_String(expB)) >= 0)? TRUE: FALSE);
					}
					break;
			}
			break;
		case IN:
			if (IsInteger(expA)) {
				Range_CheckSetElement(Trees_Integer(expA));
				if (IsSet(expB)) {
					result = Trees_NewLeaf(OBNC_IN(Trees_Integer(expA), Trees_Set(expB))? TRUE: FALSE);
				}
			}
			break;
	}
	if (result != NULL) {
		Trees_SetType(Trees_NewLeaf(TREES_BOOLEAN_TYPE), result);
	}

	return result;
}


static Trees_Node SimpleExpressionConstValue(int operator, Trees_Node expA, Trees_Node expB)
{
	Trees_Node result = NULL;

	switch (operator) {
		case '+':
			switch (Trees_Symbol(expA)) {
				case INTEGER:
					if (expB == NULL) {
						result = expA;
					} else if (IsInteger(expB)) {
						Range_CheckIntSum(Trees_Integer(expA), Trees_Integer(expB));
						result = Trees_NewInteger(Trees_Integer(expA) + Trees_Integer(expB));
					}
					break;
				case REAL:
					if (expB == NULL) {
						result = expA;
					} else if (IsReal(expB)) {
						Range_CheckRealSum(Trees_Real(expA), Trees_Real(expB));
						result = Trees_NewReal(Trees_Real(expA) + Trees_Real(expB));
					}
					break;
				case TREES_SET_CONSTANT:
					if (expB == NULL) {
						result = expA;
					} else if (IsSet(expB)) {
						result = Trees_NewSet(Trees_Set(expA) | Trees_Set(expB));
					}
					break;
			}
			break;
		case '-':
			switch (Trees_Symbol(expA)) {
				case INTEGER:
					if (expB == NULL) {
						Range_CheckIntDiff(0, Trees_Integer(expA));
						result = Trees_NewInteger(-Trees_Integer(expA));
					} else if (IsInteger(expB)) {
						Range_CheckIntDiff(Trees_Integer(expA), Trees_Integer(expB));
						result = Trees_NewInteger(Trees_Integer(expA) - Trees_Integer(expB));
					}
					break;
				case REAL:
					if (expB == NULL) {
						Range_CheckRealDiff(0.0, Trees_Real(expA));
						result = Trees_NewReal(-Trees_Real(expA));
					} else if (IsReal(expB)) {
						Range_CheckRealDiff(Trees_Real(expA), Trees_Real(expB));
						result = Trees_NewReal(Trees_Real(expA) - Trees_Real(expB));
					}
					break;
				case TREES_SET_CONSTANT:
					if (expB == NULL) {
						result = Trees_NewSet(~Trees_Set(expA));
					} else if (IsSet(expB)) {
						result = Trees_NewSet(Trees_Set(expA) & ~Trees_Set(expB));
					}
					break;
			}
			break;
		case OR:
			if (IsBoolean(expA) && IsBoolean(expB)) {
				result = (Trees_Symbol(expA) == TRUE)? expA: expB;
			}
			break;
	}

	return result;
}


static Trees_Node TermConstValue(int operator, Trees_Node expA, Trees_Node expB)
{
	Trees_Node result = NULL;

	switch (operator) {
		case '*':
			switch (Trees_Symbol(expA)) {
				case INTEGER:
					if (IsInteger(expB)) {
						Range_CheckIntProd(Trees_Integer(expA), Trees_Integer(expB));
						result = Trees_NewInteger(Trees_Integer(expA) * Trees_Integer(expB));
					}
					break;
				case REAL:
					if (IsReal(expB)) {
						Range_CheckRealProd(Trees_Real(expA), Trees_Real(expB));
						result = Trees_NewReal(Trees_Real(expA) * Trees_Real(expB));
					}
					break;
				case TREES_SET_CONSTANT:
					if (IsSet(expB)) {
						result = Trees_NewSet(Trees_Set(expA) & Trees_Set(expB));
					}
					break;
			}
			break;
		case '/':
			switch (Trees_Symbol(expA)) {
				case REAL:
					if (IsReal(expA) && IsReal(expB)) {
						if (Trees_Real(expB) != 0) {
							result = Trees_NewReal(Trees_Real(expA) / Trees_Real(expB));
						} else {
							yyerror("warning: division by zero");
						}
					}
					break;
				case TREES_SET_CONSTANT:
					if (IsSet(expB)) {
						result = Trees_NewSet(Trees_Set(expA) ^ Trees_Set(expB));
					}
					break;
			}
			break;
		case DIV:
			if (IsInteger(expA) && IsInteger(expB)) {
				if (Trees_Integer(expB) > 0) {
					result = Trees_NewInteger(OBNC_DIV(Trees_Integer(expA), Trees_Integer(expB)));
				} else {
					yyerror("error: positive divisor expected in DIV expression: %" OBNC_INT_MOD "d", Trees_Integer(expB));
					exit(EXIT_FAILURE);
				}
			}
			break;
		case MOD:
			if (IsInteger(expA) && IsInteger(expB)) {
				if (Trees_Integer(expB) > 0) {
					result = Trees_NewInteger(OBNC_MOD(Trees_Integer(expA), Trees_Integer(expB)));
				} else {
					yyerror("error: positive divisor expected in MOD expression: %" OBNC_INT_MOD "d", Trees_Integer(expB));
					exit(EXIT_FAILURE);
				}
			}
			break;
		case '&':
			if (IsBoolean(expA) && IsBoolean(expB)) {
				if (Trees_Symbol(expA) == TRUE) {
					result = expB;
				} else {
					result = expA;
				}
			}
			break;
	}

	return result;
}


static const char *DesignatorString(Trees_Node designator)
{
	const char *baseName, *result;

	assert(IsDesignator(designator));

	baseName = Trees_Name(BaseIdent(designator));
	if (FirstSelector(designator) != NULL) {
		result = Util_String("%s...", baseName);
	} else {
		result = Util_String("%s", baseName);
	}
	return result;
}


static const char *OperatorString(int operator)
{
	const char *result = "";

	switch (operator) {
		case '+':
			result = "+";
			break;
		case '-':
			result = "-";
			break;
		case '*':
			result = "*";
			break;
		case '/':
			result = "/";
			break;
		case DIV:
			result = "DIV";
			break;
		case MOD:
			result = "MOD";
			break;
		case OR:
			result = "OR";
			break;
		case '&':
			result = "&";
			break;
		case '~':
			result = "~";
			break;
		case '=':
			result = "=";
			break;
		case '#':
			result = "#";
			break;
		case '<':
			result = "<";
			break;
		case LE:
			result = "<=";
			break;
		case '>':
			result = ">";
			break;
		case GE:
			result = ">=";
			break;
		case IN:
			result = "IN";
			break;
		case IS:
			result = "IS";
			break;
		default:
			assert(0);
	}
	return result;
}


/*functions for statement productions*/

static int Writable(Trees_Node designator)
{
	Trees_Node ident, type;
	int kind, result;

	assert(IsDesignator(designator));

	ident = BaseIdent(designator);
	kind = Trees_Kind(ident);
	type = Trees_Type(ident);
	result = ((kind == TREES_VARIABLE_KIND) && ! Trees_Imported(ident))
		|| (kind == TREES_VAR_PARAM_KIND)
		|| ((kind == TREES_VALUE_PARAM_KIND) && ! Types_IsArray(type) && ! Types_IsRecord(type));
	return result;
}


static const char *AssignmentErrorContext(int context, int paramPos)
{
	const char *result;

	switch (context) {
		case ASSIGNMENT_CONTEXT:
			result = Util_String("assignment");
			break;
		case PARAM_SUBST_CONTEXT:
			assert(paramPos >= 0);
			result = Util_String("substitution of parameter %d", paramPos + 1);
			break;
		case PROC_RESULT_CONTEXT:
			result = Util_String("return clause");
			break;
		default:
			assert(0);
	}
	return result;
}


static void ValidateAssignment(Trees_Node expression, Trees_Node targetType, int context, int paramPos)
{
	const char *errorContext;

	assert(expression != NULL);
	assert(targetType != NULL);
	assert(context >= 0);
	assert(paramPos >= 0);
	if (Types_AssignmentCompatible(expression, targetType)) {
		if (Types_IsByte(targetType) && IsInteger(expression)) {
			Range_CheckByte(Trees_Integer(expression));
		}
	} else {
		errorContext = AssignmentErrorContext(context, paramPos);
		if (IsString(expression) && Types_IsCharacterArray(targetType) && !Types_IsOpenArray(targetType)) {
			yyerror("error: string too long in %s: %" OBNC_INT_MOD "d + 1 > %" OBNC_INT_MOD "d", errorContext, Types_StringLength(Trees_Type(expression)), Trees_Integer(Types_ArrayLength(targetType)));
			exit(EXIT_FAILURE);
		} else if (Types_IsPredeclaredProcedure(Trees_Type(expression))
				&& Types_IsProcedure(targetType)) {
			yyerror("error: non-predeclared procedure expected in %s", errorContext);
			exit(EXIT_FAILURE);
		} else {
			yyerror("error: incompatible types in %s: %s -> %s",
				errorContext, TypeString(Trees_Type(expression)), TypeString(targetType));
			exit(EXIT_FAILURE);
		}
	}
}


static void ValidateActualParameter(Trees_Node actualParam, Trees_Node formalParam, int paramPos, Trees_Node procDesignator)
{
	Trees_Node formalType, actualType;

	formalType = Trees_Type(formalParam);
	actualType = Trees_Type(actualParam);

	if ((Trees_Kind(formalParam) == TREES_VALUE_PARAM_KIND)
			|| (IsDesignator(actualParam) && Writable(actualParam))) {
		if (Types_IsOpenArray(formalType)) {
			if (! Types_ArrayCompatible(actualType, formalType)) {
				yyerror("error: array incompatible types in substitution of parameter %d in %s: %s -> %s", paramPos + 1, DesignatorString(procDesignator), TypeString(actualType), TypeString(formalType));
				exit(EXIT_FAILURE);
			}
		} else if (Trees_Kind(formalParam) == TREES_VALUE_PARAM_KIND) {
			if (! Types_AssignmentCompatible(actualParam, formalType)) {
				if (Types_IsString(actualType) && Types_IsCharacterArray(formalType)) {
					yyerror("error: string too long in substitution of parameter %d: %" OBNC_INT_MOD "d + 1 > %" OBNC_INT_MOD "d", paramPos + 1, Types_StringLength(actualType), Trees_Integer(Types_ArrayLength(formalType)));
				} else {
					yyerror("error: assignment incompatible types in substitution of parameter %d in %s: %s -> %s", paramPos + 1, DesignatorString(procDesignator), TypeString(actualType), TypeString(formalType));
				}
				exit(EXIT_FAILURE);
			}
		} else if (Trees_Kind(formalParam) == TREES_VAR_PARAM_KIND) {
			if (Types_IsRecord(formalType)) {
				if (Types_IsRecord(actualType)) {
					if (! Types_Extends(formalType, actualType)) {
						yyerror("error: incompatible record types in substitution of parameter %d in %s: %s -> %s", paramPos + 1, DesignatorString(procDesignator), TypeString(actualType), TypeString(formalType));
						exit(EXIT_FAILURE);
					}
				} else {
					yyerror("error: record expected in substitution of parameter %d in %s: %s -> %s", paramPos + 1, DesignatorString(procDesignator), TypeString(actualType), TypeString(formalType));
					exit(EXIT_FAILURE);
				}
			} else {
				if (! Types_Same(actualType, formalType)) {
					yyerror("error: same types expected in substitution of parameter %d in %s: %s -> %s", paramPos + 1, DesignatorString(procDesignator), TypeString(actualType), TypeString(formalType));
					exit(EXIT_FAILURE);
				}
			}
		}
	} else {
		yyerror("error: writable variable expected in substitution of parameter %d in %s",
			paramPos + 1, DesignatorString(procDesignator));
		exit(EXIT_FAILURE);
	}
}


static void ValidateProcedureCall(Trees_Node expList, Trees_Node fpList, Trees_Node procDesignator)
{
	Trees_Node exp, formalParam, fpType;
	int pos;

	pos = 0;
	while ((expList != NULL) && (fpList != NULL)) {
		exp = Trees_Left(expList);
		CheckIsValueExpression(exp);
		formalParam = Trees_Left(fpList);
		fpType = Trees_Type(formalParam);
		ValidateActualParameter(exp, formalParam, pos, procDesignator);

		if (Types_IsChar(fpType) && (Trees_Symbol(exp) == STRING)) {
			Trees_SetLeft(Trees_NewChar(Trees_String(exp)[0]), expList);
		}
		expList = Trees_Right(expList);
		fpList = Trees_Right(fpList);
		pos++;
	}
	if ((expList == NULL) && (fpList != NULL)) {
		yyerror("error: too few actual parameters in procedure call: %s", DesignatorString(procDesignator));
		exit(EXIT_FAILURE);
	} else if ((expList != NULL) && (fpList == NULL)) {
		yyerror("error: too many actual parameters in procedure call: %s", DesignatorString(procDesignator));
		exit(EXIT_FAILURE);
	}
}


static void ValidateProcedureKind(const char procName[], int functionCallExpected, int isFunctionCall)
{
	if (isFunctionCall && ! functionCallExpected) {
		yyerror("error: function procedure expected: %s", procName);
		exit(EXIT_FAILURE);
	} else if (! isFunctionCall && functionCallExpected) {
		yyerror("error: proper procedure expected: %s", procName);
		exit(EXIT_FAILURE);
	}
}


static void ValidateParameterCount(const char procName[], int min, int max, int actual)
{
	assert(min >= 0);
	assert(min <= max);
	assert(actual >= 0);

	if ((actual < min) || (actual > max)) {
		if (min == max) {
			yyerror("error: %d parameter(s) expected: %s", min, procName);
		} else {
			yyerror("error: %d or %d parameters expected: %s", min, max, procName);
		}
		exit(EXIT_FAILURE);
	}
}


static void ValidateTypeParameter(const char procName[], Trees_Node param, int pos)
{
	if (! (IsDesignator(param) && (Trees_Kind(BaseIdent(param)) == TREES_TYPE_KIND))) {
		yyerror("error: type identifier expected in substitution of parameter %d: %s", pos + 1, procName);
		exit(EXIT_FAILURE);
	}
}


static void ValidateValueParameter(const char procName[], Trees_Node param, int pos)
{
	if (! IsValueExpression(param)) {
		yyerror("error: expression expected in substitution of parameter %d: %s", pos + 1, procName);
		exit(EXIT_FAILURE);
	}
}


static void ValidateVariableParameter(const char procName[], Trees_Node param, int pos)
{
	if (! IsDesignator(param)) {
		yyerror("error: variable expected in substitution of parameter %d: %s", pos + 1, procName);
		exit(EXIT_FAILURE);
	} else if (! Writable(param)) {
		yyerror("error: writable variable expected in substitution of parameter %d: %s", pos + 1, procName);
		exit(EXIT_FAILURE);
	}
}


static void ValidateScalarParameter(const char procName[], Trees_Node paramType, int pos)
{
	assert(Types_IsType(paramType));

	if (! Types_Scalar(paramType)) {
		yyerror("error: scalar type expected in substitution of parameter %d: %s", pos + 1, procName);
		exit(EXIT_FAILURE);
	}
}


static void ValidateIntegerParameter(const char procName[], Trees_Node param, int pos)
{
	if (! Types_IsInteger(Trees_Type(param))) {
		yyerror("error: integer expected in substitution of parameter %d: %s", pos + 1, procName);
		exit(EXIT_FAILURE);
	}
}


static void ValidateRealParameter(const char procName[], Trees_Node param, int pos)
{
	if (! Types_IsReal(Trees_Type(param))) {
		yyerror("error: parameter of type REAL expected in substitution of parameter %d: %s", pos + 1, procName);
		exit(EXIT_FAILURE);
	}
}


static void ValidateIntegerSize(const char procName[])
{
		if (sizeof (OBNC_LONGI int) != sizeof (void *)) {
			yyerror("error: integer size (%lu) and address size (%lu) are unequal: %s (requires OBNC to be compiled to use integers of size %lu)", (unsigned long) sizeof (OBNC_LONGI int), (unsigned long) sizeof (void *), procName, (unsigned long) sizeof (void *));
			exit(EXIT_FAILURE);
		}
}


static OBNC_LONGI int TypeSize(Trees_Node type)
{
	OBNC_LONGI int result = 0;

	switch (Trees_Symbol(Types_Structure(type))) {
		case TREES_BOOLEAN_TYPE:
			result = sizeof (int);
			break;
		case TREES_CHAR_TYPE:
			result = sizeof (char);
			break;
		case TREES_INTEGER_TYPE:
			result = sizeof (OBNC_LONGI int);
			break;
		case TREES_REAL_TYPE:
			result = sizeof (OBNC_LONGR double);
			break;
		case TREES_BYTE_TYPE:
			result = sizeof (unsigned char);
			break;
		case TREES_SET_TYPE:
			result = sizeof (unsigned OBNC_LONGI int);
			break;
		case ARRAY:
			result = Trees_Integer(Types_ArrayLength(type)) * TypeSize(Types_ElementType(type));
			break;
		case RECORD:
		case POINTER:
		case PROCEDURE:
			/*cannot be calculated in the Oberon-to-C translation*/
			break;
		default:
			assert(0);
	}
	return result;
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
		{"UNPK", TREES_UNPK_PROC},

		{"ADR", TREES_ADR_PROC},
		{"SIZE", TREES_SIZE_PROC},
		{"BIT", TREES_BIT_PROC},
		{"GET", TREES_GET_PROC},
		{"PUT", TREES_PUT_PROC},
		{"COPY", TREES_COPY_PROC},
		{"VAL", TREES_VAL_PROC}};

	int paramCount, pos, symbol;
	Trees_Node curr, resultType, result;
	Trees_Node param[3], paramTypes[3];
	const char *unqualProcName;

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
	unqualProcName = strchr(procName, '.');
	if (unqualProcName != NULL) {
		unqualProcName++;
	} else {
		unqualProcName = procName;
	}
	pos = 0;
	while ((pos < LEN(symbols)) && (strcmp(symbols[pos].name, unqualProcName) != 0)) {
		pos++;
	}
	assert(pos < LEN(symbols));
	symbol = symbols[pos].symbol;

	/*validate parameters and build syntax tree*/
	result = NULL;
	resultType = NULL;
	switch (symbol) {
		case TREES_ABS_PROC:
			ValidateProcedureKind(procName, 1, isFunctionCall);
			ValidateParameterCount(procName, 1, 1, paramCount);
			ValidateValueParameter(procName, param[0], 0);
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
			if (result == NULL) {
				resultType = paramTypes[0];
			}
			break;
		case TREES_ODD_PROC:
			ValidateProcedureKind(procName, 1, isFunctionCall);
			ValidateParameterCount(procName, 1, 1, paramCount);
			ValidateValueParameter(procName, param[0], 0);
			ValidateIntegerParameter(procName, param[0], 0);
			if (IsInteger(param[0])) {
				result = Trees_NewBoolean(OBNC_ODD(Trees_Integer(param[0])));
			} else {
				resultType = Trees_NewLeaf(TREES_BOOLEAN_TYPE);
			}
			break;
		case TREES_LEN_PROC:
			ValidateProcedureKind(procName, 1, isFunctionCall);
			ValidateParameterCount(procName, 1, 1, paramCount);
			ValidateValueParameter(procName, param[0], 0);
			if (Types_IsArray(paramTypes[0])) {
				if (! Types_IsOpenArray(paramTypes[0])) {
					result = Types_ArrayLength(paramTypes[0]);
				} else {
					resultType = Trees_NewLeaf(TREES_INTEGER_TYPE);
				}
			} else {
				yyerror("error: array parameter expected: %s", procName);
				exit(EXIT_FAILURE);
			}
			break;
		case TREES_LSL_PROC: /*fall through*/
		case TREES_ASR_PROC: /*fall through*/
		case TREES_ROR_PROC:
			ValidateProcedureKind(procName, 1, isFunctionCall);
			ValidateParameterCount(procName, 2, 2, paramCount);
			ValidateValueParameter(procName, param[0], 0);
			ValidateIntegerParameter(procName, param[0], 0);
			ValidateValueParameter(procName, param[1], 1);
			ValidateIntegerParameter(procName, param[1], 1);
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
			if (result == NULL) {
				resultType = paramTypes[0];
			}
			break;
		case TREES_FLOOR_PROC:
			ValidateProcedureKind(procName, 1, isFunctionCall);
			ValidateParameterCount(procName, 1, 1, paramCount);
			ValidateValueParameter(procName, param[0], 0);
			ValidateRealParameter(procName, param[0], 0);
			if (IsReal(param[0])) {
				OBNC_LONGR double x = Trees_Real(param[0]);
				Range_CheckFLOOR(x);
				result = Trees_NewInteger(OBNC_FLOOR(x));
			} else {
				resultType = Trees_NewLeaf(TREES_INTEGER_TYPE);
			}
			break;
		case TREES_FLT_PROC:
			ValidateProcedureKind(procName, 1, isFunctionCall);
			ValidateParameterCount(procName, 1, 1, paramCount);
			ValidateValueParameter(procName, param[0], 0);
			ValidateIntegerParameter(procName, param[0], 0);
			if (IsInteger(param[0])) {
				result = Trees_NewReal(OBNC_FLT(Trees_Integer(param[0])));
			} else {
				resultType = Trees_NewLeaf(TREES_REAL_TYPE);
			}
			break;
		case TREES_ORD_PROC:
			ValidateProcedureKind(procName, 1, isFunctionCall);
			ValidateParameterCount(procName, 1, 1, paramCount);
			ValidateValueParameter(procName, param[0], 0);
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
						result = Trees_NewInteger((OBNC_LONGI int) Trees_Set(param[0]));
					}
					break;
				default:
					yyerror("error: character parameter expected: %s", procName);
					exit(EXIT_FAILURE);
			}
			if (result == NULL) {
				resultType = Trees_NewLeaf(TREES_INTEGER_TYPE);
			}
			break;
		case TREES_CHR_PROC:
			ValidateProcedureKind(procName, 1, isFunctionCall);
			ValidateParameterCount(procName, 1, 1, paramCount);
			ValidateValueParameter(procName, param[0], 0);
			ValidateIntegerParameter(procName, param[0], 0);
			if (IsInteger(param[0])) {
				OBNC_LONGI int i = Trees_Integer(param[0]);
				Range_CheckCHR(i);
				result = Trees_NewChar(OBNC_CHR(i));
			} else {
				resultType = Trees_NewLeaf(TREES_CHAR_TYPE);
			}
			break;
		case TREES_INC_PROC: /*fall through*/
		case TREES_DEC_PROC:
			ValidateProcedureKind(procName, 0, isFunctionCall);
			ValidateParameterCount(procName, 1, 2, paramCount);
			ValidateVariableParameter(procName, param[0], 0);
			ValidateIntegerParameter(procName, param[0], 0);
			if (paramCount == 2) {
				ValidateIntegerParameter(procName, param[1], 1);
			}
			break;
		case TREES_INCL_PROC: /*fall through*/
		case TREES_EXCL_PROC:
			ValidateProcedureKind(procName, 0, isFunctionCall);
			ValidateParameterCount(procName, 1, 2, paramCount);
			ValidateVariableParameter(procName, param[0], 0);
			ValidateValueParameter(procName, param[1], 1);
			if (Types_IsSet(paramTypes[0])) {
				if (IsInteger(param[1])) {
					Range_CheckSetElement(Trees_Integer(param[1]));
				} else {
					ValidateIntegerParameter(procName, param[1], 1);
				}
			} else {
				yyerror("error: set expected in substitution of parameter 1: %s", procName);
				exit(EXIT_FAILURE);
			}
			break;
		case TREES_ASSERT_PROC:
			ValidateProcedureKind(procName, 0, isFunctionCall);
			ValidateParameterCount(procName, 1, 1, paramCount);
			ValidateValueParameter(procName, param[0], 0);
			if (Types_IsBoolean(paramTypes[0])) {
				result = Trees_NewNode(
					TREES_ASSERT_PROC,
					param[0],
					Trees_NewNode(TREES_FILE_POSITION,
						Trees_NewString(inputFilename),
						Trees_NewInteger(yylineno)));
			} else {
				yyerror("error: boolean parameter expected: %s", procName);
				exit(EXIT_FAILURE);
			}
			break;
		case TREES_NEW_PROC:
			ValidateProcedureKind(procName, 0, isFunctionCall);
			ValidateParameterCount(procName, 1, 1, paramCount);
			ValidateVariableParameter(procName, param[0], 0);
			if (! Types_IsPointer(paramTypes[0])) {
				yyerror("error: pointer parameter expected: %s", procName);
				exit(EXIT_FAILURE);
			}
			break;
		case TREES_PACK_PROC:
			ValidateProcedureKind(procName, 0, isFunctionCall);
			ValidateParameterCount(procName, 2, 2, paramCount);
			ValidateVariableParameter(procName, param[0], 0);
			ValidateRealParameter(procName, param[0], 0);
			ValidateValueParameter(procName, param[1], 1);
			ValidateIntegerParameter(procName, param[1], 1);
			break;
		case TREES_UNPK_PROC:
			ValidateProcedureKind(procName, 0, isFunctionCall);
			ValidateParameterCount(procName, 2, 2, paramCount);
			ValidateVariableParameter(procName, param[0], 0);
			ValidateRealParameter(procName, param[0], 0);
			ValidateVariableParameter(procName, param[1], 1);
			ValidateIntegerParameter(procName, param[1], 1);
			break;
		case TREES_ADR_PROC:
			ValidateIntegerSize(procName);
			ValidateProcedureKind(procName, 1, isFunctionCall);
			ValidateParameterCount(procName, 1, 1, paramCount);
			ValidateVariableParameter(procName, param[0], 0);
			resultType = Trees_NewLeaf(TREES_INTEGER_TYPE);
			break;
		case TREES_SIZE_PROC:
			{
				OBNC_LONGI int size;

				ValidateProcedureKind(procName, 1, isFunctionCall);
				ValidateParameterCount(procName, 1, 1, paramCount);
				ValidateTypeParameter(procName, param[0], 0);
				size = TypeSize(Trees_Type(BaseIdent(param[0])));
				if (size > 0) {
					result = Trees_NewInteger(size);
				} else {
					resultType = Trees_NewLeaf(TREES_INTEGER_TYPE);
				}
			}
			break;
		case TREES_BIT_PROC:
			ValidateIntegerSize(procName);
			ValidateProcedureKind(procName, 1, isFunctionCall);
			ValidateParameterCount(procName, 2, 2, paramCount);
			ValidateValueParameter(procName, param[0], 0);
			ValidateIntegerParameter(procName, param[0], 0);
			ValidateValueParameter(procName, param[1], 1);
			ValidateIntegerParameter(procName, param[1], 1);
			if (IsInteger(param[1])) {
				Range_CheckBIT(Trees_Integer(param[1]));
			}
			resultType = Trees_NewLeaf(TREES_BOOLEAN_TYPE);
			break;
		case TREES_GET_PROC:
			ValidateIntegerSize(procName);
			ValidateProcedureKind(procName, 0, isFunctionCall);
			ValidateParameterCount(procName, 2, 2, paramCount);
			ValidateValueParameter(procName, param[0], 0);
			ValidateIntegerParameter(procName, param[0], 0);
			ValidateVariableParameter(procName, param[1], 1);
			if (! Types_Basic(paramTypes[1])) {
				yyerror("error: variable of basic type expected in substitution of parameter 2: %s", procName);
				exit(EXIT_FAILURE);
			}
			break;
		case TREES_PUT_PROC:
			ValidateIntegerSize(procName);
			ValidateProcedureKind(procName, 0, isFunctionCall);
			ValidateParameterCount(procName, 2, 2, paramCount);
			ValidateValueParameter(procName, param[0], 0);
			ValidateIntegerParameter(procName, param[0], 0);
			ValidateValueParameter(procName, param[1], 1);
			if (! Types_Basic(paramTypes[1]) && ! Types_IsSingleCharString(paramTypes[1])) {
				yyerror("error: expression of basic type expected in substitution of parameter 2: %s", procName);
				exit(EXIT_FAILURE);
			}
			break;
		case TREES_COPY_PROC:
			ValidateIntegerSize(procName);
			ValidateProcedureKind(procName, 0, isFunctionCall);
			ValidateParameterCount(procName, 3, 3, paramCount);
			ValidateValueParameter(procName, param[0], 0);
			ValidateIntegerParameter(procName, param[0], 0);
			ValidateValueParameter(procName, param[1], 1);
			ValidateIntegerParameter(procName, param[1], 1);
			ValidateValueParameter(procName, param[2], 2);
			ValidateIntegerParameter(procName, param[2], 2);
			if (IsInteger(param[2]) && (Trees_Integer(param[2]) < 0)) {
				yyerror("warning: non-negative count expected in %s: %" OBNC_INT_MOD "d", procName, Trees_Integer(param[2]));
			}
			break;
		case TREES_VAL_PROC:
			ValidateProcedureKind(procName, 1, isFunctionCall);
			ValidateParameterCount(procName, 2, 2, paramCount);
			ValidateTypeParameter(procName, param[0], 0);
			ValidateScalarParameter(procName, paramTypes[0], 0);
			ValidateValueParameter(procName, param[1], 1);
			ValidateScalarParameter(procName, paramTypes[1], 1);
			resultType = paramTypes[0];
			if (IsConstExpression(param[1])) {
				result = param[1];
				Trees_SetType(resultType, result);
			}
			break;
		default:
			assert(0);
	}

	if (result == NULL) {
		result = Trees_NewNode(symbol, expList, NULL);
		if (isFunctionCall) {
			assert(resultType != NULL);
			Trees_SetType(resultType, result);
		}
	}
	return result;
}


static void HandleProcedureCall(Trees_Node designator, Trees_Node expList, int isFunctionCall, Trees_Node *ast)
{
	Trees_Node ident, designatorTypeStruct, fpList, resultType;

	ident = BaseIdent(designator);
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
			ValidateProcedureCall(expList, fpList, designator);
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
	OBNC_LONGI int aMin, aMax, bMin, bMax;

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
		yyerror("error: case label defined twice: %" OBNC_INT_MOD "d", aMin);
		exit(EXIT_FAILURE);
	} else if ((bMin >= aMin) && (bMin <= aMax)) {
		yyerror("error: case label defined twice: %" OBNC_INT_MOD "d", bMin);
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

	labelList = Trees_Left(caseLabelsStack);
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
					yyerror("error: type case label defined twice: %s", Trees_Name(newLabelRange));
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

static void ExportSymbolTable(const char symfilePath[])
{
	const char *tempSymfilePath;

	if (! Files_Exists(".obnc")) {
		Files_CreateDir(".obnc");
	}
	tempSymfilePath = Util_String(".obnc/%s.sym.%d", inputModuleName, getpid());
	Table_Export(tempSymfilePath);
	Files_Move(tempSymfilePath, symfilePath);
}
