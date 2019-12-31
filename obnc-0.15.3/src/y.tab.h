/* A Bison parser, made by GNU Bison 3.3.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

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
#line 107 "Oberon.y" /* yacc.c:1921  */

	const char *ident;
	OBNC_LONGI int integer;
	OBNC_LONGR double real;
	const char *string;
	Trees_Node node;

#line 156 "y.tab.h" /* yacc.c:1921  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
