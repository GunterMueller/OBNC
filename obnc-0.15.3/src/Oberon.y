/*Copyright (C) 2017, 2018, 2019 Karl Landstrom <karl@miasap.se>

This file is part of OBNC.

OBNC is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

OBNC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with OBNC.  If not, see <http://www.gnu.org/licenses/>.*/

%{
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

static int initialized = 0;

static const char *inputFilename;
static int parseMode;
static char *inputModuleName;

static Trees_Node unresolvedPointerTypes;
static Trees_Node currentTypeIdentdef;
static Trees_Node recordDeclarationStack;
static Trees_Node caseExpressionStack;
static Trees_Node caseLabelsStack;
static Trees_Node procedureDeclarationStack;

void yyerror(const char msg[]);
static void CheckUnusedIdentifiers(void);

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
%}

%union {
	const char *ident;
	OBNC_LONGI int integer;
	OBNC_LONGR double real;
	const char *string;
	Trees_Node node;
}

%token TOKEN_START

/*reserved words (underscore avoids name clash)*/
%token ARRAY BEGIN_ BY CASE CONST DIV DO ELSE ELSIF END FALSE FOR IF IMPORT IN IS MOD MODULE NIL OF OR POINTER PROCEDURE RECORD REPEAT RETURN THEN TO TRUE TYPE UNTIL VAR WHILE

/*two-character operators and delimiters*/
%token BECOMES DOTDOT GE LE

/*tokens with semantic values*/
%token <ident> IDENT
%token <integer> INTEGER
%token <real> REAL
%token <string> STRING

%token TOKEN_END

/*nonterminals with semantic values*/
%type <integer> AddOperator
%type <node> ArrayLengthOf
%type <node> ArrayType
%type <node> assignment
%type <node> BaseType
%type <node> BaseTypeOpt
%type <ident> BecomesIdentOpt
%type <node> ByOpt
%type <node> case
%type <node> CaseExpression
%type <node> CaseLabelList
%type <node> CaseRep
%type <node> CaseStatement
%type <node> ConstExpression
%type <node> designator
%type <node> element
%type <node> ElementRep
%type <node> ElseIfDoOptRep
%type <node> ElseIfThenOptRep
%type <node> ElseOpt
%type <node> ExpList
%type <integer> ExportMarkOpt
%type <node> expression
%type <node> factor
%type <node> FieldList
%type <node> FieldListSequence
%type <node> FieldListSequenceOpt
%type <node> ForInit
%type <node> ForLimit
%type <node> FormalParameters
%type <node> FormalParametersOpt
%type <node> FormalType
%type <node> ForStatement
%type <node> FPSection
%type <node> FPSectionRep
%type <node> FPSectionsOpt
%type <node> guard
%type <node> identdef
%type <node> IdentRep
%type <node> IfStatement
%type <node> IdentList
%type <node> import
%type <node> ImportRep
%type <node> length
%type <node> label
%type <node> LabelRange
%type <node> LengthRep
%type <node> ModuleStatements
%type <integer> MulOperator
%type <node> number
%type <node> OpenArrayOptRep
%type <integer> ParameterKindOpt
%type <node> PointerTo
%type <node> PointerType
%type <node> ProcedureCall
%type <node> ProcedureHeading
%type <node> ProcedureHeadingSansParam
%type <node> ProcedureType
%type <node> ProcedureTypeSansParam
%type <node> qualident
%type <node> RecordHeading
%type <node> RecordType
%type <integer> relation
%type <node> RepeatStatement
%type <node> ResultTypeOpt
%type <node> ReturnExpressionOpt
%type <node> selector
%type <node> SelectorOptRep
%type <node> set
%type <integer> SignOpt
%type <node> SimpleExpression
%type <node> statement
%type <node> StatementSequence
%type <node> StatementSequenceOpt
%type <node> StatementSequenceReversed
%type <node> term
%type <node> type
%type <node> TypeIdentDef
%type <integer> TypeKeyword
%type <node> TypeSectionOpt
%type <node> WhileStatement

%start module

%%

/*IDENTIFIER RULES*/

qualident:
	IDENT
	{
		$$ = Trees_NewIdent($1);
	}
	| IDENT '.' IDENT
	{
		$$ = Trees_NewIdent(Util_String("%s.%s", $1, $3));
	}
	;

identdef:
	IDENT ExportMarkOpt
	{
		if (! Table_LocallyDeclared($1) || (recordDeclarationStack != NULL)) {
			$$ = Trees_NewIdent($1);
			if ($2) {
				Trees_SetExported($$);
			}
			if (Table_ScopeLocal()) {
				Trees_SetLocal($$);
			}
		} else {
			Oberon_PrintError("error: redeclaration of identifier: %s", $1);
			YYABORT;
		}
	}
	;

ExportMarkOpt:
	'*'
	{
		$$ = 1;
	}
	| /*empty*/
	{
		$$ = 0;
	}
	;


/*NUMBER RULE*/

number:
	INTEGER
	{
		$$ = Trees_NewInteger($1);
	}
	| REAL
	{
		$$ = Trees_NewReal($1);
	}
	;


/*CONSTANT DECLARATION RULES*/

ConstDeclaration:
	identdef '=' ConstExpression
	{
		if (! (Trees_Exported($1) && Trees_Local($1))) {
			Trees_SetKind(TREES_CONSTANT_KIND, $1);
			Trees_SetType(Trees_Type($3), $1);
			Trees_SetValue($3, $1);
			Table_Put($1);
			Generate_ConstDeclaration($1);
		} else {
			Oberon_PrintError("error: cannot export local constant: %s", Trees_Name($1));
			YYABORT;
		}
	}
	;

ConstExpression:
	expression
	{
		if (IsConstExpression($1)) {
			$$ = $1;
		} else {
			Oberon_PrintError("error: constant expression expected");
			YYABORT;
		}
	}
	;


/*TYPE DECLARATION RULES*/

TypeDeclaration:
	TypeIdentDef type
	{
		Trees_Node sourceType;

		sourceType = ResolvedType($2, 1);
		if (sourceType != NULL) {
			if (! (Trees_Exported($1) && Trees_Local($1))) {
				Trees_SetType(sourceType, $1);
				ResolvePointerTypes($1);
				currentTypeIdentdef = NULL;
				Generate_TypeDeclaration($1);
			} else {
				Oberon_PrintError("error: cannot export local type: %s", Trees_Name($1));
				YYABORT;
			}
		} else {
			Oberon_PrintError("error: undeclared identifier: %s", Trees_Name($2));
			YYABORT;
		}
	}
	;

TypeIdentDef:
	identdef '='
	{
		Trees_SetKind(TREES_TYPE_KIND, $1);
		currentTypeIdentdef = $1;
		Table_Put($1);
		$$ = $1;
	}
	;

type:
	qualident
	| ArrayType
	| RecordType
	| PointerType
	| ProcedureType
	;

ArrayType:
	ArrayLengthOf type
	{
		Trees_Node reversedLengths, length;

		$$ = ResolvedType($2, 0);
		if ($$ != NULL) {
			reversedLengths = $1;
			do {
				length = Trees_Left(reversedLengths);
				$$ = Types_NewArray(length, $$);
				reversedLengths = Trees_Right(reversedLengths);
			} while (reversedLengths != NULL);
		} else {
			Oberon_PrintError("error: undeclared identifier: %s", Trees_Name($2));
			exit(EXIT_FAILURE);
		}
	}
	;

ArrayLengthOf:
	ARRAY LengthRep OF
	{
		if ((currentTypeIdentdef != NULL) && (Trees_Type(currentTypeIdentdef) == NULL)) {
			Trees_SetType(Trees_NewLeaf(ARRAY), currentTypeIdentdef); /*incomplete type*/
		}
		$$ = $2;
	}
	;

LengthRep:
	length
	{
		$$ = Trees_NewNode(TREES_NOSYM, $1, NULL);
	}
	| LengthRep ',' length
	{
		$$ = Trees_NewNode(TREES_NOSYM, $3, $1);
	}
	;

length:
	ConstExpression
	{
		if (Types_IsInteger(Trees_Type($1))) {
			if (IsInteger($1)) {
				if (Trees_Integer($1) <= 0) {
					Oberon_PrintError("error: positive length expected: %" OBNC_INT_MOD "d", Trees_Integer($1));
					YYABORT;
				}
			} else {
				Oberon_PrintError("error: fully evaluated constant expression expected as increment");
				YYABORT;
			}
		} else {
			Oberon_PrintError("error: integer length expected");
			YYABORT;
		}
	}
	;

RecordType:
	RecordHeading FieldListSequenceOpt END
	{
		recordDeclarationStack = Trees_Right(recordDeclarationStack);
		$$ = Types_NewRecord(Types_RecordBaseType($1), $2);
	}
	;

RecordHeading:
	RECORD BaseTypeOpt
	{
		$$ = Types_NewRecord($2, NULL);
		if ((currentTypeIdentdef != NULL) && (Trees_Type(currentTypeIdentdef) == NULL)) {
			Trees_SetType($$, currentTypeIdentdef);
		}
		recordDeclarationStack = Trees_NewNode(TREES_NOSYM, $$, recordDeclarationStack);
	}
	;

BaseTypeOpt:
	'(' BaseType ')'
	{
		$$ = $2;
	}
	| /*empty*/
	{
		$$ = NULL;
	}
	;

BaseType:
	qualident
	{
		const char *name;
		Trees_Node symbol;

		$$ = NULL;
		name = Trees_Name($1);
		symbol = Table_At(name);
		if (symbol != NULL) {
			if (Trees_Kind(symbol) == TREES_TYPE_KIND) {
				if (symbol != currentTypeIdentdef) {
					switch (Trees_Symbol(Types_Structure(symbol))) {
						case POINTER:
							if (Types_Same(Types_PointerBaseType(symbol), currentTypeIdentdef)) {
								Oberon_PrintError("error: self-referring base type: %s", name);
								YYABORT;
							}
							/*fall through*/
						case RECORD:
							$$ = symbol;
							break;
						default:
							Oberon_PrintError("error: record or pointer base type expected: %s", name);
							YYABORT;
					}
				} else {
					Oberon_PrintError("error: self-referring base type: %s", name);
					YYABORT;
				}
			} else {
				Oberon_PrintError("error: type name expected: %s", name);
				YYABORT;
			}
		} else {
			Oberon_PrintError("error: undeclared identifier: %s", name);
			YYABORT;
		}
	}
	;

FieldListSequenceOpt:
	FieldListSequence
	{
		Trees_ReverseList(&$1); /*correct order*/
		$$ = $1;
	}
	| /*empty*/
	{
		$$ = NULL;
	}
	;

FieldListSequence:
	FieldList
	{
		$$ = Trees_NewNode(TREES_FIELD_LIST_SEQUENCE, $1, NULL);
	}
	| FieldListSequence ';' FieldList
	{
		Trees_Node currSeq, currList, currSeqList;
		const char *seqIdentName, *listIdentName;

		currList = $3;
		while (currList != NULL) {
			listIdentName = Trees_Name(Trees_Left(currList));
			currSeq = $1;
			while (currSeq != NULL) {
				currSeqList = Trees_Left(currSeq);
				while (currSeqList != NULL) {
					seqIdentName = Trees_Name(Trees_Left(currSeqList));
					if (strcmp(listIdentName, seqIdentName) == 0) {
						Oberon_PrintError("error: redeclaration of field: %s", listIdentName);
						YYABORT;
					}
					currSeqList = Trees_Right(currSeqList);
				}
				currSeq = Trees_Right(currSeq);
			}
			currList = Trees_Right(currList);
		}
		$$ = Trees_NewNode(TREES_FIELD_LIST_SEQUENCE, $3, $1);
	}
	;

FieldList:
	IdentList ':' type
	{
		Trees_Node type, tail, ident, p, directBaseType, baseTypeField, baseType;

		$$ = NULL;
		type = ResolvedType($3, 0);
		if (type != NULL) {
			if (! ((type == currentTypeIdentdef) && ! Types_IsPointer(type))) {
				Trees_ReverseList(&$1); /*correct order*/
				tail = $1;
				do {
					ident = Trees_Left(tail);
					p = $1;
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
							Oberon_PrintError("error: redeclaration of field: %s defined in %s", Trees_Name(ident), Trees_Name(baseType));
							YYABORT;
						}
					} else {
						Oberon_PrintError("error: redeclaration of field: %s", Trees_Name(ident));
						YYABORT;
					}
					tail = Trees_Right(tail);
				} while (tail != NULL);

				$$ = $1;
			} else {
				Oberon_PrintError("error: recursive field type must be a pointer: %s", Trees_Name($3));
				YYABORT;
			}
		} else {
			Oberon_PrintError("error: undeclared type: %s", Trees_Name($3));
			YYABORT;
		}
	}
	;

IdentList:
	identdef
	{
		$$ = Trees_NewNode(TREES_IDENT_LIST, $1, NULL);
	}
	| IdentList ',' identdef
	{
		Trees_Node reversedIdents;

		reversedIdents = Trees_NewNode(TREES_IDENT_LIST, $3, $1);
		$$ = reversedIdents;
	}
	;

PointerType:
	PointerTo type
	{
		const char *baseTypeName;
		Trees_Node declaredBaseType;

		$$ = NULL;
		if (Trees_Symbol($2) == IDENT) {
			baseTypeName = Trees_Name($2);
			declaredBaseType = Table_At(baseTypeName);
			if (declaredBaseType != NULL) {
				if (Types_IsRecord(declaredBaseType)) {
					$$ = Types_NewPointer(declaredBaseType);
				} else {
					Oberon_PrintError("error: record expected as pointer base type: %s", baseTypeName);
					YYABORT;
				}
			} else if (currentTypeIdentdef != NULL) {
				Trees_SetKind(TREES_TYPE_KIND, $2);
				Trees_SetType(Types_NewRecord(NULL, NULL), $2);
				$$ = Types_NewPointer($2);
				unresolvedPointerTypes = Trees_NewNode(TREES_NOSYM, $$, unresolvedPointerTypes);
			} else {
				Oberon_PrintError("error: undeclared type: %s", baseTypeName);
				YYABORT;
			}
		} else if(Trees_Symbol($2) == RECORD) {
			$$ = Types_NewPointer($2);
		} else {
			Oberon_PrintError("error: record expected as pointer base type");
			YYABORT;
		}
	}
	;

PointerTo:
	POINTER TO
	{
		if ((currentTypeIdentdef != NULL) && (Trees_Type(currentTypeIdentdef) == NULL)) {
			Trees_SetType(Types_NewPointer(NULL), currentTypeIdentdef); /*incomplete type*/
		}
	}
	;

ProcedureType:
	ProcedureTypeSansParam FormalParametersOpt
	{
		$$ = $2;
	}
	;

ProcedureTypeSansParam:
	PROCEDURE
	{
		$$ = NULL;
	}
	;

FormalParametersOpt:
	FormalParameters
	| /*empty*/
	{
		$$ = Trees_NewLeaf(PROCEDURE);
	}
	;


/*VARIABLE DECLARATION RULE*/

VariableDeclaration:
	IdentList ':' type
	{
		Trees_Node type, identList, ident;

		type = ResolvedType($3, 0);
		if (type != NULL) {
			Trees_ReverseList(&$1); /*correct order*/
			identList = $1;
			do {
				ident = Trees_Left(identList);
				if (! (Trees_Exported(ident) && Trees_Local(ident))) {
					if (! Table_LocallyDeclared(Trees_Name(ident))) {
						Trees_SetKind(TREES_VARIABLE_KIND, ident);
						Trees_SetType(type, ident);
						Table_Put(ident);
					} else {
						Oberon_PrintError("error: redeclaration of identifier with the same name: %s", Trees_Name(ident));
						YYABORT;
					}
				} else {
					Oberon_PrintError("error: cannot export local variable: %s", Trees_Name(ident));
					YYABORT;
				}
				identList = Trees_Right(identList);
			} while (identList != NULL);

			Generate_VariableDeclaration($1);
		} else {
			Oberon_PrintError("error: undeclared identifier: %s", Trees_Name($3));
			exit(EXIT_FAILURE);
		}
	}
	;


/*EXPRESSION RULES*/

expression:
	SimpleExpression
	| SimpleExpression relation SimpleExpression
	{
		Trees_Node expA, expB, typeA, typeB;
		int op = (int) $2;

		expA = $1;
		expB = $3;
		typeA = Trees_Type($1);
		typeB = Trees_Type($3);

		CheckIsValueExpression($1);
		if (op == IS) {
			if (IsDesignator($1)) {
				if (! (Types_IsRecord(typeA) && (Trees_Kind(BaseIdent($1)) != TREES_VAR_PARAM_KIND))) {
					expB = BaseIdent($3);
					typeB = BaseIdent($3);
				} else {
					Oberon_PrintError("error: variable parameter expected as first operand of IS");
					YYABORT;
				}
			} else {
				Oberon_PrintError("error: identifier expected as first operand of IS");
				YYABORT;
			}
		} else {
			CheckIsValueExpression($3);
		}
		if (Types_ExpressionCompatible(op, typeA, typeB)) {
			$$ = ExpressionConstValue(op, expA, expB);
			if ($$ == NULL) {
				if (IsString(expA) && Types_IsChar(typeB)) {
					expA = Trees_NewChar(Trees_String(expA)[0]);
				} else if (Types_IsChar(typeA) && IsString(expB)) {
					expB = Trees_NewChar(Trees_String(expB)[0]);
				}
				$$ = Trees_NewNode(op, expA, expB);
				Trees_SetType(Trees_NewLeaf(TREES_BOOLEAN_TYPE), $$);
			}
		} else {
			Oberon_PrintError("error: incompatible types in relation \"%s\": %s, %s",
				OperatorString(op), TypeString(typeA), TypeString(typeB));
			YYABORT;
		}
	}
	;

relation:
	'='
	{
		$$ = '=';
	}
	| '#'
	{
		$$ = '#';
	}
	| '<'
	{
		$$ = '<';
	}
	| LE
	{
		$$ = LE;
	}
	| '>'
	{
		$$ = '>';
	}
	| GE
	{
		$$ = GE;
	}
	| IN
	{
		$$ = IN;
	}
	| IS
	{
		$$ = IS;
	}
	;

SimpleExpression:
	SignOpt term
	{
		int op = (int) $1;
		$$ = $2;
		if (op >= 0) {
			CheckIsValueExpression($2);
			if (Types_ExpressionCompatible(op, Trees_Type($2), NULL)) {
				$$ = SimpleExpressionConstValue(op, $2, NULL);
				if ($$ == NULL) {
					$$ = Trees_NewNode(op, $2, NULL);
					if (Types_IsByte(Trees_Type($2))) {
						Trees_SetType(Trees_NewLeaf(TREES_INTEGER_TYPE), $$);
					} else {
						Trees_SetType(Trees_Type($2), $$);
					}
				}
			} else {
				Oberon_PrintError("error: incompatible type in unary operation \"%s\": %s", OperatorString(op), TypeString(Trees_Type($2)));
				YYABORT;
			}
		}
	}
	| SimpleExpression AddOperator term
	{
		int op = (int) $2;

		$$ = NULL;

		CheckIsValueExpression($1);
		CheckIsValueExpression($3);

		if (Types_ExpressionCompatible(op, Trees_Type($1), Trees_Type($3))) {
			$$ = SimpleExpressionConstValue(op, $1, $3);
			if ($$ == NULL) {
				$$ = Trees_NewNode(op, $1, $3);
				if (Types_IsByte(Trees_Type($1)) || Types_IsByte(Trees_Type($3))) {
					Trees_SetType(Trees_NewLeaf(TREES_INTEGER_TYPE), $$);
				} else {
					Trees_SetType(Trees_Type($1), $$);
				}
			}
		} else {
			Oberon_PrintError("error: incompatible types in operation \"%s\": %s, %s",
				OperatorString(op), TypeString(Trees_Type($1)), TypeString(Trees_Type($3)));
			YYABORT;
		}
		assert($$ != NULL);
	}
	;

SignOpt:
	'+'
	{
		$$ = '+';
	}
	| '-'
	{
		$$ = '-';
	}
	| /*empty*/
	{
		$$ = -1;
	}
	;

AddOperator:
	'+'
	{
		$$ = '+';
	}
	| '-'
	{
		$$ = '-';
	}
	| OR
	{
		$$ = OR;
	}
	;

term:
	factor
	| term MulOperator factor
	{
		int op = (int) $2;

		$$ = NULL;

		CheckIsValueExpression($1);
		CheckIsValueExpression($3);

		if (Types_ExpressionCompatible(op, Trees_Type($1), Trees_Type($3))) {
			$$ = TermConstValue(op, $1, $3);
			if ($$ == NULL) {
				$$ = Trees_NewNode(op, $1, $3);
				if (Types_IsByte(Trees_Type($1)) || Types_IsByte(Trees_Type($3))) {
					Trees_SetType(Trees_NewLeaf(TREES_INTEGER_TYPE), $$);
				} else {
					Trees_SetType(Trees_Type($1), $$);
				}
			}
		} else {
			Oberon_PrintError("error: incompatible types in operation \"%s\": %s, %s",
				OperatorString(op), TypeString(Trees_Type($1)), TypeString(Trees_Type($3)));
			YYABORT;
		}

		assert($$ != NULL);
	}
	;

MulOperator:
	'*'
	{
		$$ = '*';
	}
	| '/'
	{
		$$ = '/';
	}
	| DIV
	{
		$$ = DIV;
	}
	| MOD
	{
		$$ = MOD;
	}
	| '&'
	{
		$$ = '&';
	}
	;

factor:
	number
	| STRING
	{
		$$ = Trees_NewString($1);
	}
	| NIL
	{
		$$ = Trees_NewLeaf(NIL);
		Trees_SetType(Trees_NewLeaf(TREES_NIL_TYPE), $$);
	}
	| TRUE
	{
		$$ = Trees_NewBoolean(1);
	}
	| FALSE
	{
		$$ = Trees_NewBoolean(0);
	}
	| set
	{
		$$ = $1;
		Trees_SetType(Trees_NewLeaf(TREES_SET_TYPE), $$);
	}
	/*NOTE: actual parameters are parsed by rule `designator'*/
	| designator
	{
		const int isFunctionCall = 1;
		Trees_Node designator, actualParameters, ident;

		$$ = NULL;
		if (Trees_Symbol($1) == TREES_PROCEDURE_CALL) {
			designator = Trees_Left($1);
			actualParameters = Trees_Right($1);
			HandleProcedureCall(designator, actualParameters, isFunctionCall, &$$);
		} else {
			ident = Trees_Left($1);
			if (Trees_Kind(ident) == TREES_CONSTANT_KIND) {
				$$ = Trees_Value(ident);
			} else {
				$$ = $1;
			}
		}
		assert($$ != NULL);
	}
	| '(' expression ')'
	{
		CheckIsValueExpression($2);
		$$ = $2;
	}
	| '~' factor
	{
		$$ = NULL;
		CheckIsValueExpression($2);
		if (Types_ExpressionCompatible('~', Trees_Type($2), NULL)) {
			if (IsBoolean($2)) {
				$$ = Trees_NewBoolean(! Trees_Boolean($2));
			} else {
				$$ = Trees_NewNode('~', $2, NULL);
				Trees_SetType(Trees_NewLeaf(TREES_BOOLEAN_TYPE), $$);
			}
		} else {
			Oberon_PrintError("error: incompatible type in operation \"~\": %s", TypeString(Trees_Type($2)));
			YYABORT;
		}
		assert($$ != NULL);
	}
	;

designator:
	/*NOTE: qualified (imported) identifiers are parsed as field designators and detected semantically*/
	IDENT SelectorOptRep
	{
		Trees_Node designator, identType, actualParameters;
		int parameterListFound; /*possibly empty*/

		Trees_ReverseList(&$2); /*correct order*/
		designator = Designator($1, $2);

		identType = Trees_Type(BaseIdent(designator));
		SetSelectorTypes(identType, designator, &parameterListFound);
		if (parameterListFound) {
			RemoveActualParameters(&designator, &actualParameters);
			$$ = Trees_NewNode(TREES_PROCEDURE_CALL, designator, actualParameters);
		} else {
			$$ = designator;
		}
	}
	;

SelectorOptRep:
	SelectorOptRep selector
	{
		Trees_Node curr;

		if ((Trees_Symbol($2) == '[') && (Trees_Right($2) != NULL)) { /*multi-dimensional element selector*/
			/*attatch last element selector node to $1*/
			Trees_ReverseList(&$2);
			$$ = $1;
			curr = $2;
			do {
				$$ = Trees_NewNode('[', Trees_Left(curr), $$);
				curr = Trees_Right(curr);
			} while (curr != NULL);
			Trees_ReverseList(&$$);
		} else {
			$$ = Trees_NewNode(Trees_Symbol($2), Trees_Left($2), $1);
		}
	}
	| /*empty*/
	{
		$$ = NULL;
	}
	;

selector:
	'.' IDENT
	{
		Trees_Node field;

		field = Trees_NewIdent($2);
		Trees_SetKind(TREES_FIELD_KIND, field);
		$$ = Trees_NewNode('.', field, NULL);
	}
	| '[' ExpList ']'
	{
		Trees_Node curr, exp;

		/*create one selector node per index*/
		$$ = NULL;
		curr = $2; /*NOTE: ExpList is reversed*/
		do {
			exp = Trees_Left(curr);
			if (Types_IsInteger(Trees_Type(exp))) {
				$$ = Trees_NewNode('[', Trees_Left(curr), $$);
			} else {
				Oberon_PrintError("error: integer array index expected");
				YYABORT;
			}
			curr = Trees_Right(curr);
		} while (curr != NULL);
	}
	| '^'
	{
		$$ = Trees_NewNode('^', NULL, NULL);
	}
	/*NOTE: Procedure calls are parsed as designators and distinguished from type guards through semantic analysis.*/
	| '(' ExpList ')' /*type guard or actual parameters*/
	{
		Trees_ReverseList(&$2); /*correct order*/
		$$ = Trees_NewNode('(', $2, NULL);
	}
	| '(' ')' /*actual parameters*/
	{
		$$ = Trees_NewNode('(', NULL, NULL);
	}
	;

set:
	'{' '}'
	{
		$$ = Trees_NewSet(0x0u);
	}
	| '{' ElementRep '}'
	{
		$$ = $2;
	}
	;

ElementRep:
	element
	| ElementRep ',' element
	{
		if ((Trees_Symbol($1) == TREES_SET_CONSTANT)
				&& (Trees_Symbol($3) == TREES_SET_CONSTANT)) {
			$$ = Trees_NewSet(Trees_Set($1) | Trees_Set($3));
		} else {
			$$ = Trees_NewNode('+', $1, $3);
			Trees_SetType(Trees_NewLeaf(TREES_SET_TYPE), $$);
		}
	}
	;

element:
	expression
	{
		OBNC_LONGI int i;
		Trees_Node type;

		CheckIsValueExpression($1);
		$$ = NULL;
		type = Trees_Type($1);
		if (IsInteger($1)) {
			i = Trees_Integer($1);
			Range_CheckSetElement(i);
			$$ = Trees_NewSet(1u << i);
		} else if (Types_IsInteger(type)) {
			$$ = Trees_NewNode(TREES_SINGLE_ELEMENT_SET, $1, NULL);
			Trees_SetType(Trees_NewLeaf(TREES_SET_TYPE), $$);
		} else {
			Oberon_PrintError("error: element must have integer type");
			YYABORT;
		}
	}
	| expression DOTDOT expression
	{
		CheckIsValueExpression($1);
		CheckIsValueExpression($3);
		$$ = NULL;
		if (IsInteger($1)) {
			Range_CheckSetElement(Trees_Integer($1));
		}
		if (IsInteger($3)) {
			Range_CheckSetElement(Trees_Integer($3));
		}
		if (IsInteger($1) && IsInteger($3)) {
			$$ = Trees_NewSet(OBNC_RANGE(Trees_Integer($1), Trees_Integer($3)));
		} else if (Types_IsInteger(Trees_Type($1)) && Types_IsInteger(Trees_Type($3))) {
			$$ = Trees_NewNode(TREES_RANGE_SET, $1, $3);
			Trees_SetType(Trees_NewLeaf(TREES_SET_TYPE), $$);
		} else {
			Oberon_PrintError("error: element must have integer type");
			YYABORT;
		}
	}
	;

ExpList:
	expression
	{
		$$ = Trees_NewNode(TREES_EXP_LIST, $1, NULL);
		Trees_SetType(Trees_Type($1), $$);
	}
	| ExpList ',' expression
	{
		Trees_Node reversedList;

		reversedList = Trees_NewNode(TREES_EXP_LIST, $3, $1);
		$$ = reversedList;
		Trees_SetType(Trees_Type($3), $$);
	}
	;


/*STATEMENT RULES*/

statement:
	assignment
	| ProcedureCall
	| IfStatement
	| CaseStatement
	| WhileStatement
	| RepeatStatement
	| ForStatement
	| /*empty*/
	{
		$$ = NULL;
	}
	;

assignment:
	designator BECOMES expression
	{
		Trees_Node designator, ident, designatorType, exp;

		CheckIsValueExpression($3);
		switch (Trees_Symbol($1)) {
			case TREES_DESIGNATOR:
				designator = $1;
				exp = $3;
				ident = BaseIdent($1);
				designatorType = Trees_Type($1);
				switch (Trees_Kind(ident)) {
					case TREES_VARIABLE_KIND:
					case TREES_VALUE_PARAM_KIND:
					case TREES_VAR_PARAM_KIND:
						if (Writable($1)) {
							ValidateAssignment(exp, designatorType, ASSIGNMENT_CONTEXT, 0);
							if (Types_IsChar(designatorType) && IsString(exp)) {
								exp = Trees_NewChar(Trees_String(exp)[0]);
							}
						} else {
							Oberon_PrintError("error: assignment to read-only variable");
							YYABORT;
						}
						break;
					default:
						Oberon_PrintError("error: assignment to non-variable");
						YYABORT;
				}
				$$ = Trees_NewNode(BECOMES, designator, exp);
				break;
			case TREES_PROCEDURE_CALL:
				Oberon_PrintError("error: unexpected procedure call in assignment target");
				YYABORT;
				break;
			default:
				assert(0);
		}
	}
	;

ProcedureCall:
	/*NOTE: actual parameters are parsed by rule `designator'*/
	designator
	{
		const int isFunctionCall = 0;
		Trees_Node designator, actualParameters;

		if (Trees_Symbol($1) == TREES_PROCEDURE_CALL) {
			designator = Trees_Left($1);
			actualParameters = Trees_Right($1);
		} else {
			designator = $1;
			actualParameters = NULL;
		}
		HandleProcedureCall(designator, actualParameters, isFunctionCall, &$$);
		assert($$ != NULL);
	}
	;

StatementSequence:
	StatementSequenceReversed
	{
		Trees_ReverseList(&$1); /*correct order*/
		$$ = $1;
	}
	;

StatementSequenceReversed:
	statement
	{
		if ($1 == NULL) {
			$$ = NULL;
		} else {
			$$ = Trees_NewNode(TREES_STATEMENT_SEQUENCE, $1, NULL);
		}
	}
	| StatementSequenceReversed ';' statement
	{
		if ($3 != NULL) {
			$$ = Trees_NewNode(TREES_STATEMENT_SEQUENCE, $3, $1);
		} else {
			$$ = $1;
		}
	}
	;

IfStatement:
	IF guard THEN StatementSequence ElseIfThenOptRep ElseOpt END
	{
		Trees_Node currElsif, currExp, currThen, currStmt;

		if ($5 == NULL) {
			$$ = Trees_NewNode(IF, $2, Trees_NewNode(THEN, $4, $6));
		} else {
			/*correct order of elsif nodes*/
			$$ = $6;
			currElsif = $5;
			do {
				currExp = Trees_Left(currElsif);
				currThen = Trees_Right(currElsif);
				currStmt = Trees_Left(currThen);
				$$ = Trees_NewNode(ELSIF, currExp, Trees_NewNode(THEN, currStmt, $$));
				currElsif = Trees_Right(currThen);
			} while (currElsif != NULL);
			$$ = Trees_NewNode(IF, $2, Trees_NewNode(THEN, $4, $$));
		}
	}
	;

guard:
	expression
	{
		CheckIsValueExpression($1);
		if (Types_IsBoolean(Trees_Type($1))) {
			$$ = $1;
		} else {
			Oberon_PrintError("error: boolean expression expected");
			YYABORT;
		}
	}
	;

ElseIfThenOptRep:
	ElseIfThenOptRep ELSIF guard THEN StatementSequence
	{
		$$ = Trees_NewNode(ELSIF, $3, Trees_NewNode(THEN, $5, $1));
	}
	| /*empty*/
	{
		$$ = NULL;
	}
	;

ElseOpt:
	ELSE StatementSequence
	{
		$$ = Trees_NewNode(ELSE, $2, NULL);
	}
	| /*empty*/
	{
		$$ = NULL;
	}
	;

CaseStatement:
	CASE CaseExpression OF CaseRep END
	{
		Trees_Node expType, caseVariable;

		if ($4 != NULL) {
			Trees_ReverseList(&$4); /*correct order*/
		}
		assert(caseLabelsStack != NULL);
		caseLabelsStack = Trees_Right(caseLabelsStack);
		expType = Trees_Type($2);
		if (Types_IsRecord(expType) || Types_IsPointer(expType)) {
			/*reset original type*/
			caseVariable = Trees_Left($2);
			Trees_SetType(Trees_Type($2), caseVariable);
			caseExpressionStack = Trees_Right(caseExpressionStack);
		}
		$$ = Trees_NewNode(CASE, $2, $4);
	}
	;

CaseExpression:
	expression
	{
		Trees_Node typeStruct, caseVariable;

		CheckIsValueExpression($1);
		typeStruct = Types_Structure(Trees_Type($1));
		switch (Trees_Symbol(typeStruct)) {
			case RECORD:
				/*fall through*/
			case POINTER:
				if (IsDesignator($1) && (FirstSelector($1) == NULL)) {
					caseVariable = BaseIdent($1);
					if (! Types_IsRecord(typeStruct) || (Trees_Kind(caseVariable) == TREES_VAR_PARAM_KIND)) {
						$$ = $1;
					} else {
						Oberon_PrintError("error: record CASE expression must be a variable parameter");
						YYABORT;
					}
				} else {
					Oberon_PrintError("error: non-integral CASE expression must be a variable");
					YYABORT;
				}
				/*fall through*/
			case TREES_INTEGER_TYPE:
				/*fall through*/
			case TREES_CHAR_TYPE:
				caseExpressionStack = Trees_NewNode(TREES_NOSYM, $1, caseExpressionStack);
				caseLabelsStack = Trees_NewNode(TREES_NOSYM, NULL, caseLabelsStack);
				$$ = $1;
				break;
			default:
				Oberon_PrintError("error: invalid type of CASE expression");
				YYABORT;
		}
	}
	;

CaseRep:
	case
	{
		if ($1 != NULL) {
			$$ = Trees_NewNode(TREES_CASE_REP, $1, NULL);
		} else {
			$$ = NULL;
		}
	}
	| CaseRep '|' case
	{
		if ($3 != NULL) {
			if ($1 != NULL) {
				$$ = Trees_NewNode(TREES_CASE_REP, $3, $1);
			} else {
				$$ = Trees_NewNode(TREES_CASE_REP, $3, NULL);
			}
		} else {
			$$ = NULL;
		}
	}
	;

case:
	CaseLabelList ':' StatementSequence
	{
		Trees_ReverseList(&$1); /*correct order*/
		$$ = Trees_NewNode(TREES_CASE, $1, $3);
	}
	| /*empty*/
	{
		$$ = NULL;
	}
	;

CaseLabelList:
	LabelRange
	{
		$$ = Trees_NewNode(TREES_CASE_LABEL_LIST, $1, NULL);
	}
	| CaseLabelList ',' LabelRange
	{
		switch (Trees_Symbol($3)) {
			case INTEGER:
			case TREES_CHAR_CONSTANT:
			case DOTDOT:
				$$ = Trees_NewNode(TREES_CASE_LABEL_LIST, $3, $1);
				break;
			default:
				Oberon_PrintError("error: unexpected list of type name case labels");
				YYABORT;
		}
	}
	;

LabelRange:
	label
	{
		$$ = $1;
		CheckCaseLabelUniqueness($1);
		assert(caseLabelsStack != NULL);
		caseLabelsStack = Trees_NewNode(TREES_NOSYM,
			Trees_NewNode(TREES_NOSYM, $1, Trees_Left(caseLabelsStack)),
			Trees_Right(caseLabelsStack));
	}
	| label DOTDOT label
	{
		const int rangeLenMax = 255;
		int leftSym, rightSym;
		OBNC_LONGI int rangeMin, rangeMax;

		leftSym = Trees_Symbol($1);
		rightSym = Trees_Symbol($3);
		if (leftSym == rightSym) {
			switch (leftSym) {
				case INTEGER:
					rangeMin = Trees_Integer($1);
					rangeMax = Trees_Integer($3);
					if (rangeMin <= rangeMax) {
						if (rangeMax - rangeMin > rangeLenMax) {
							Oberon_PrintError("warning: maximum range length of %d exceeded", rangeLenMax);
							YYABORT;
						}
					} else {
						Oberon_PrintError("error: left integer must be less than right integer in case range");
						YYABORT;
					}
					break;
				case TREES_CHAR_CONSTANT:
					if (Trees_Char($1) >= Trees_Char($3)) {
						Oberon_PrintError("error: left string must be less than right string in case range");
						YYABORT;
					}
					break;
				default:
					Oberon_PrintError("error: case label ranges must contain integers or single-character strings");
					YYABORT;
			}
		} else {
			Oberon_PrintError("error: case labels in a range must have the same type");
			YYABORT;
		}
		$$ = Trees_NewNode(DOTDOT, $1, $3);
		CheckCaseLabelUniqueness($$);
		assert(caseLabelsStack != NULL);
		caseLabelsStack = Trees_NewNode(TREES_NOSYM,
			Trees_NewNode(TREES_NOSYM, $$, Trees_Left(caseLabelsStack)),
			Trees_Right(caseLabelsStack));
	}
	;

label:
	INTEGER
	{
		if (Types_IsInteger(Trees_Type(Trees_Left(caseExpressionStack)))) {
			$$ = Trees_NewInteger($1);
		} else {
			Oberon_PrintError("error: unexpected integer case label");
			YYABORT;
		}
	}
	| STRING
	{
		if (Types_IsChar(Trees_Type(Trees_Left(caseExpressionStack)))) {
			if (strlen($1) <= 1) {
				$$ = Trees_NewChar($1[0]);
			} else {
				Oberon_PrintError("error: single-character string expected: \"%s\"", $1);
				YYABORT;
			}
		} else {
			Oberon_PrintError("error: unexpected string case label: \"%s\"", $1);
			YYABORT;
		}
	}
	| qualident
	{
		Trees_Node caseExp, constValue, caseVariable;

		$$ = Table_At(Trees_Name($1));
		if ($$ != NULL) {
			caseExp = Trees_Left(caseExpressionStack);
			switch (Trees_Symbol(Types_Structure(Trees_Type(caseExp)))) {
				case TREES_INTEGER_TYPE:
					if (Trees_Kind($$) == TREES_CONSTANT_KIND) {
						constValue = Trees_Value($$);
						if (Trees_Symbol(constValue) == INTEGER) {
							if (Trees_Integer(constValue) >= 0) {
								$$ = constValue;
							} else {
								Oberon_PrintError("error: non-negative case label expected: %" OBNC_INT_MOD "d", Trees_Integer(constValue));
								YYABORT;
							}
						} else {
							Oberon_PrintError("error: integer case label expected");
							YYABORT;
						}
					} else {
						Oberon_PrintError("error: constant identifier expected: %s", Trees_Name($$));
						YYABORT;
					}
					break;
				case TREES_CHAR_TYPE:
					if (Trees_Kind($$) == TREES_CONSTANT_KIND) {
						constValue = Trees_Value($$);
						if (Trees_Symbol(constValue) == STRING) {
							if (Types_StringLength(Trees_Type(constValue)) <= 1) {
								$$ = Trees_NewChar(Trees_String(constValue)[0]);
							} else {
								Oberon_PrintError("error: single-character string expected: %s", Trees_String(constValue));
								YYABORT;
							}
						} else {
							Oberon_PrintError("error: character case label expected");
							YYABORT;
						}
					} else {
						Oberon_PrintError("error: constant identifier expected: %s", Trees_Name($$));
						YYABORT;
					}
					break;
				case RECORD:
					if (Types_IsType($$) && Types_IsRecord($$)) {
						if (Types_Extends(Trees_Type(caseExp), $$)) {
							caseVariable = Trees_Left(caseExp);
							Trees_SetType($$, caseVariable);
						} else {
							Oberon_PrintError("error: case label is not an extension of %s: %s", Trees_Name(Trees_Type(caseExp)), Trees_Name($$));
							YYABORT;
						}
					} else {
						Oberon_PrintError("error: record type case label expected");
						YYABORT;
					}
					break;
				case POINTER:
					if (Types_IsType($$) && Types_IsPointer($$)) {
						if (Types_Extends(Trees_Type(caseExp), $$)) {
							caseVariable = Trees_Left(caseExp);
							Trees_SetType($$, caseVariable);
						} else {
							Oberon_PrintError("error: case label is not an extension of %s: %s", Trees_Name(Trees_Type(caseExp)), Trees_Name($$));
							YYABORT;
						}
					} else {
						Oberon_PrintError("error: pointer type case label expected");
						YYABORT;
					}
					break;
				default:
					assert(0);
			}
		} else {
			Oberon_PrintError("error: undeclared identifier: %s", Trees_Name($1));
			YYABORT;
		}
	}
	;

WhileStatement:
	WHILE guard DO StatementSequence ElseIfDoOptRep END
	{
		$$ = Trees_NewNode(WHILE, $2, Trees_NewNode(DO, $4, $5));
	}
	;

ElseIfDoOptRep:
	ElseIfDoOptRep ELSIF guard DO StatementSequence
	{
		$$ = Trees_NewNode(ELSIF, $3, Trees_NewNode(THEN, $5, $1));
	}
	| /*empty*/
	{
		$$ = NULL;
	}
	;

RepeatStatement:
	REPEAT StatementSequence UNTIL expression
	{
		CheckIsValueExpression($4);
		$$ = NULL;
		if (Types_IsBoolean(Trees_Type($4))) {
			$$ = Trees_NewNode(REPEAT, $2, $4);
		} else {
			Oberon_PrintError("error: boolean expression expected");
			YYABORT;
		}
	}
	;


ForStatement:
	FOR ForInit TO ForLimit ByOpt DO StatementSequence END
	{
		Trees_Node byExp;

		if ($5 != NULL) {
			byExp = $5;
		} else {
			byExp = Trees_NewInteger(1);
		}
		$$ = Trees_NewNode(FOR,
			$2,
			Trees_NewNode(TO,
				$4,
				Trees_NewNode(BY, byExp, $7)));
	}
	;

ForInit:
	IDENT BECOMES expression
	{
		Trees_Node ctrlVar, ctrlVarType;

		CheckIsValueExpression($3);
		ctrlVar = Table_At($1);
		if (ctrlVar != NULL) {
			ctrlVarType = Trees_Type(ctrlVar);
			if (Types_IsInteger(ctrlVarType)) {
				if (Types_IsInteger(Trees_Type($3))) {
					$$ = Trees_NewNode(BECOMES, ctrlVar, $3);
				} else {
					Oberon_PrintError("error: integer expression expected as initial value");
					YYABORT;
				}
			} else {
				Oberon_PrintError("error: integer control variable expected: %s", $1);
				YYABORT;
			}
		} else {
			Oberon_PrintError("error: undeclared control variable: %s", $1);
			YYABORT;
		}
	}

ForLimit:
	expression
	{
		CheckIsValueExpression($1);
		if (! Types_IsInteger(Trees_Type($1))) {
			Oberon_PrintError("error: integer expression expected as upper limit");
			YYABORT;
		}
	}
	;

ByOpt:
	BY ConstExpression
	{
		if (Types_IsInteger(Trees_Type($2))) {
			if (IsInteger($2)) {
				if (Trees_Integer($2) == 0) {
					Oberon_PrintError("warning: steps by zero leads to infinite loop");
				}
				$$ = $2;
			} else {
				Oberon_PrintError("error: fully evaluated constant expression expected as increment");
				YYABORT;
			}
		} else {
			Oberon_PrintError("error: integer increment expected");
			YYABORT;
		}
	}
	| /*empty*/
	{
		$$ = NULL;
	}
	;


/*PROCEDURE DECLARATION RULES*/

ProcedureDeclaration:
	ProcedureHeading ';' DeclarationSequence StatementSequenceOpt ReturnExpressionOpt END IDENT
	{
		Trees_Node procIdent, procType, resultType, procStatements, returnExp;
		const char *procName;

		procIdent = $1;
		procName = Trees_Name(procIdent);
		procType = Trees_Type($1);
		resultType = Types_ResultType(procType);
		procStatements = $4;
		returnExp = $5;

		if (strcmp(procName, $7) == 0) {
			if (resultType == NULL) {
				if (returnExp != NULL) {
					Oberon_PrintError("error: unexpected return expression");
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
					Oberon_PrintError("error: return expression expected");
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
			CheckUnusedIdentifiers();
			Table_CloseScope();
		} else {
			Oberon_PrintError("error: expected procedure name: %s", procName);
			YYABORT;
		}
	}
	;

ProcedureHeading:
	ProcedureHeadingSansParam FormalParametersOpt
	{
		Trees_Node paramList, param;

		$$ = NULL;
		Trees_SetType($2, $1);

		paramList = Types_Parameters($2);
		while (paramList != NULL) {
			param = Trees_Left(paramList);
			Table_Put(param);
			paramList = Trees_Right(paramList);
		}

		procedureDeclarationStack = Trees_NewNode(TREES_NOSYM, $1, procedureDeclarationStack);
		Generate_ProcedureHeading($1);
		$$ = $1;
	}
	;

ProcedureHeadingSansParam:
	PROCEDURE identdef
	{
		if (! (Trees_Exported($2) && Trees_Local($2))) {
			Trees_SetKind(TREES_PROCEDURE_KIND, $2);
			Table_Put($2);
			Table_OpenScope();
		} else {
			Oberon_PrintError("error: cannot export local procedure: %s", Trees_Name($2));
			YYABORT;
		}
		$$ = $2;
	}
	;

StatementSequenceOpt:
	BEGIN_ StatementSequence
	{
		$$ = $2;
	}
	| /*empty*/
	{
		$$ = NULL;
	}
	;

ReturnExpressionOpt:
	RETURN expression
	{
		$$ = $2;
	}
	| /*empty*/
	{
		$$ = NULL;
	}
	;

DeclarationSequence:
	ConstSectionOpt TypeSectionOpt VariableSectionOpt ProcedureDeclarationOptRep
	;

ConstSectionOpt:
	CONST ConstDeclarationOptRep
	| /*empty*/
	;

ConstDeclarationOptRep:
	ConstDeclarationOptRep ConstDeclaration ';'
	| /*empty*/
	;

TypeSectionOpt:
	TypeKeyword TypeDeclarationOptRep
	{
		Trees_Node unresolvedPointerType, undeclaredBaseType;

		if (unresolvedPointerTypes != NULL) {
			unresolvedPointerType = Trees_Left(unresolvedPointerTypes);
			undeclaredBaseType = Types_PointerBaseType(unresolvedPointerType);
			Oberon_PrintError("error: undeclared pointer base type: %s", Trees_Name(undeclaredBaseType));
			YYABORT;
		}
	}
	| /*empty*/
	{
		$$ = NULL;
	}
	;

TypeKeyword:
	TYPE
	{
		unresolvedPointerTypes = NULL;
	}
	;

TypeDeclarationOptRep:
	TypeDeclarationOptRep TypeDeclaration ';'
	| /*empty*/
	;

VariableSectionOpt:
	VAR VariableDeclarationOptRep
	| /*empty*/
	;

VariableDeclarationOptRep:
	VariableDeclarationOptRep VariableDeclaration ';'
	| /*empty*/
	;

ProcedureDeclarationOptRep:
	ProcedureDeclarationOptRep ProcedureDeclaration ';'
	| /*empty*/
	;

FormalParameters:
	'(' FPSectionsOpt ')' ResultTypeOpt
	{
		$$ = Types_NewProcedure($2, $4);
	}
	;

FPSectionsOpt:
	FPSectionRep
	{
		Trees_ReverseList(&$1); /*correct order*/
		$$ = $1;
	}
	| /*empty*/
	{
		$$ = NULL;
	}
	;

FPSectionRep:
	FPSection
	{
		$$ = $1;
		Trees_ReverseList(&$$);
	}
	| FPSectionRep ';' FPSection
	{
		Trees_Node p, p1;
		const char *paramName, *paramName1;

		/*make sure no parameter is repeated*/
		p = $3;
		while (p != NULL) {
			paramName = Trees_Name(Trees_Left(p));
			p1 = $1;
			while (p1 != NULL) {
				paramName1 = Trees_Name(Trees_Left(p1));
				if (strcmp(paramName1, paramName) == 0) {
					Oberon_PrintError("error: repeated parameter: %s", paramName);
					YYABORT;
				}
				p1 = Trees_Right(p1);
			}
			p = Trees_Right(p);
		}

		/*make one list of the two lists*/
		$$ = $1;
		p = $3;
		do {
			$$ = Trees_NewNode(TREES_IDENT_LIST, Trees_Left(p), $$);
			p = Trees_Right(p);
		} while (p != NULL);
		/*$$ in reversed order*/
	}
	;

ResultTypeOpt:
	':' qualident
	{
		$$ = ResolvedType($2, 0);
		if ($$ != NULL) {
			if (Trees_Symbol($$) == IDENT) {
				if (Trees_Kind($$) != TREES_TYPE_KIND) {
					Oberon_PrintError("error: type name expected as result type: %s", Trees_Name($2));
					YYABORT;
				}
				if (! Types_Scalar($$)) {
					Oberon_PrintError("error: scalar result type expected: %s", Trees_Name($2));
					YYABORT;
				}
			}
		} else {
			Oberon_PrintError("error: undeclared identifier: %s", Trees_Name($2));
			YYABORT;
		}
	}
	| /*empty*/
	{
		$$ = NULL;
	}
	;

FPSection:
	ParameterKindOpt IdentRep ':' FormalType
	{
		Trees_Node curr, ident;

		Trees_ReverseList(&$2); /*correct order*/
		curr = $2;
		do {
			ident = Trees_Left(curr);
			Trees_SetKind((int) $1, ident);
			Trees_SetType($4, ident);
			Trees_SetLocal(ident);
			curr = Trees_Right(curr);
		} while (curr != NULL);

		$$ = $2;
	}
	;

ParameterKindOpt:
	VAR
	{
		$$ = TREES_VAR_PARAM_KIND;
	}
	| /*empty*/
	{
		$$ = TREES_VALUE_PARAM_KIND;
	}
	;

IdentRep:
	IDENT
	{
		$$ = Trees_NewNode(TREES_IDENT_LIST, Trees_NewIdent($1), NULL);
	}
	| IdentRep ',' IDENT
	{
		Trees_Node curr;
		const char *identName;

		/*make sure no name is repeated*/
		curr = $1;
		while (curr != NULL) {
			identName = Trees_Name(Trees_Left(curr));
			if (strcmp(identName, $3) == 0) {
				Oberon_PrintError("error: repeated identifier: %s", identName);
				YYABORT;
			}
			curr = Trees_Right(curr);
		}

		$$ = Trees_NewNode(TREES_IDENT_LIST, Trees_NewIdent($3), $1);
	}
	;

FormalType:
	OpenArrayOptRep qualident
	{
		$$ = ResolvedType($2, 0);
		if ($$ != NULL) {
			while ($1 != NULL) {
				$$ = Types_NewArray(NULL, $$);
				$1 = Trees_Right($1);
			}
		} else {
			Oberon_PrintError("error: undeclared identifier: %s", Trees_Name($2));
			exit(EXIT_FAILURE);
		}
	}
	;

OpenArrayOptRep:
	OpenArrayOptRep ARRAY OF
	{
		$$ = Trees_NewNode(ARRAY, NULL, $1);
	}
	| /*empty*/
	{
		$$ = NULL;
	}
	;


/*MODULE RULES*/

module:
	ModuleHeading ';' ImportListOpt DeclarationSequence ModuleStatements END IDENT '.'
	{
		const char *symfilePath;

		if (strcmp($7, inputModuleName) == 0) {
			CheckUnusedIdentifiers();
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
			Oberon_PrintError("error: expected identifier %s", inputModuleName);
			YYABORT;
		}
	}
	;


ModuleHeading:
	MODULE IDENT
	{
		if (strcmp($2, inputModuleName) == 0) {
			if (parseMode != OBERON_IMPORT_LIST_MODE) {
				Generate_ModuleHeading();
			}
		} else {
			Oberon_PrintError("error: module name does not match filename: %s", $2);
			YYABORT;
		}
	}
	;

ImportListOpt:
	ImportList
	{
		if (parseMode == OBERON_IMPORT_LIST_MODE) {
			YYACCEPT;
		}
	}
	| /*empty*/
	{
		if (parseMode == OBERON_IMPORT_LIST_MODE) {
			YYACCEPT;
		}
	}
	;

ImportList:
	IMPORT ImportRep ';'
	{
		const char *impfilePath;
		Trees_Node moduleAndDirPath, module, p;
		FILE *impFile;
		const char *name;

		if ($2 != NULL) {
			Trees_ReverseList(&$2); /*correct order*/
			if (parseMode == OBERON_IMPORT_LIST_MODE) {
				while ($2 != NULL) {
					name = Trees_Name(Trees_Left($2));
					puts(name);
					$2 = Trees_Right($2);
				}
			} else {
				if (parseMode == OBERON_NORMAL_MODE) {
					impfilePath = Util_String(".obnc/%s.imp", inputModuleName);
					impFile = Files_Exists(impfilePath)? Files_Old(impfilePath, FILES_WRITE): Files_New(impfilePath);
					p = $2;
					do {
						moduleAndDirPath = Trees_Left(p);
						module = Trees_Left(moduleAndDirPath);
						name = Trees_UnaliasedName(module);
						fprintf(impFile, "%s\n", name);
						p = Trees_Right(p);
					} while (p != NULL);
					Files_Close(&impFile);
				}
				Generate_ImportList($2);
			}
		}
	}
	;

ImportRep:
	import
	{
		if ($1 != NULL) {
			$$ = Trees_NewNode(TREES_NOSYM, $1, NULL);
		} else {
			$$ = NULL;
		}
	}
	| ImportRep ',' import
	{
		if ($3 != NULL) {
			$$ = Trees_NewNode(TREES_NOSYM, $3, $1);
		} else {
			$$ = $1;
		}
	}
	;

import:
	IDENT BecomesIdentOpt
	{
		static Maps_Map importedModules = NULL;
		const char *module, *qualifier, *symbolFileDir, *symbolFileName, *moduleDirPath;
		Trees_Node qualifierSym, moduleIdent;

		if (importedModules == NULL) {
			importedModules = Maps_New();
		}
		if ($2 != NULL) {
			module = $2;
			qualifier = $1;
		} else {
			module = $1;
			qualifier = $1;
		}
		$$ = NULL;
		if (strcmp(module, inputModuleName) != 0) {
			if (! Maps_HasKey(module, importedModules)) {
				Maps_Put(module, NULL, &importedModules);
				qualifierSym = Table_At(qualifier);
				if (qualifierSym == NULL) {
					qualifierSym = Trees_NewIdent(qualifier);
					if ($2 != NULL) {
						Trees_SetUnaliasedName(module, qualifierSym);
					}
					Trees_SetKind(TREES_QUALIFIER_KIND, qualifierSym);
					Table_Put(qualifierSym);

					if (strcmp(module, "SYSTEM") == 0) {
						if (parseMode != OBERON_IMPORT_LIST_MODE) {
							Table_ImportSystem(qualifier);
						}
					} else if (parseMode == OBERON_IMPORT_LIST_MODE) {
						$$ = Trees_NewIdent(module);
					} else {
						moduleDirPath = ModulePaths_Directory(module, ".", 0);
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
								Oberon_PrintError("error: symbol file not found for module %s: %s", module, symbolFileName);
								YYABORT;
							}

							moduleIdent = Trees_NewIdent(module);
							Trees_SetKind(TREES_QUALIFIER_KIND, moduleIdent);
							$$ = Trees_NewNode(TREES_NOSYM, moduleIdent, Trees_NewString(moduleDirPath));
						} else {
							Oberon_PrintError("error: imported module not found: %s", module);
							YYABORT;
						}
					}
				} else {
					Oberon_PrintError("error: qualifier already used: %s", qualifier);
					YYABORT;
				}
			} else {
				Oberon_PrintError("error: module already imported: %s", module);
				YYABORT;
			}
		} else {
			Oberon_PrintError("error: a module cannot import itself");
			YYABORT;
		}
	}
	;

BecomesIdentOpt:
	BECOMES IDENT
	{
		$$ = $2;
	}
	| /*empty*/
	{
		$$ = NULL;
	}
	;

ModuleStatements:
	StatementSequenceOpt
	{
		Generate_ModuleStatements($1);
	}
	;

%%

void Oberon_Init(void)
{
	if (! initialized) {
		initialized = 1;
		Error_Init();
		Files_Init();
		Generate_Init();
		ModulePaths_Init();
		Table_Init();
	}
}


void Oberon_Parse(const char inputFile[], int mode)
{
	const char *impFile;
	FILE *fp;
	int error;

	assert(initialized);
	inputFilename = inputFile;
	parseMode = mode;
	inputModuleName = Paths_SansSuffix(Paths_Basename(inputFile));

	yyin = fopen(inputFile, "r");
	if (yyin != NULL) {
		if (mode != OBERON_IMPORT_LIST_MODE) {
			Generate_Open(inputFile, mode == OBERON_ENTRY_POINT_MODE);

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


void Oberon_PrintError(const char format[], ...)
{
	va_list ap;

	assert(initialized);
	fprintf(stderr, "obnc-compile: %s:%d: ", inputFilename, yylineno);
	va_start(ap, format);
	vfprintf(stderr, format, ap);
	va_end(ap);
	fputc('\n', stderr);
}


void yyerror(const char msg[])
{
	Oberon_PrintError("%s", msg);
}


static void PrintError(int line, const char format[], ...)
	__attribute__ ((format (printf, 2, 3)));

static void PrintError(int line, const char format[], ...)
{
	va_list ap;

	fprintf(stderr, "obnc-compile: %s:%d: ", inputFilename, line);
	va_start(ap, format);
	vfprintf(stderr, format, ap);
	va_end(ap);
	fputc('\n', stderr);
}


static char *IdentKindString(int kind)
{
	char *result;

	switch (kind) {
		case TREES_CONSTANT_KIND:
			result = Util_String("constant");
			break;
		case TREES_TYPE_KIND:
			result = Util_String("type");
			break;
		case TREES_VARIABLE_KIND:
			result = Util_String("variable");
			break;
		case TREES_PROCEDURE_KIND:
			result = Util_String("procedure");
			break;
		case TREES_QUALIFIER_KIND:
			result = Util_String("module");
			break;
		default:
			result = Util_String("identifier");
	}
	return result;
}


static void CheckUnusedIdentifiers(void)
{
	Trees_Node unusedIdents, ident;
	int kind;

	unusedIdents = Table_UnusedIdentifiers();
	while (unusedIdents != NULL) {
		ident = Trees_Left(unusedIdents);
		kind = Trees_Kind(ident);
		if (! Trees_Exported(ident)
				&& (kind != TREES_VALUE_PARAM_KIND)
				&& (kind != TREES_VAR_PARAM_KIND)) {
			PrintError(Trees_LineNumber(ident), "note: unused %s: %s", IdentKindString(Trees_Kind(ident)), Trees_UnaliasedName(ident));
		}
		unusedIdents = Trees_Right(unusedIdents);
	}
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
					Oberon_PrintError("error: unresolved type: %s", name);
					exit(EXIT_FAILURE);
				}
			} else {
				Oberon_PrintError("error: type expected: %s", name);
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
				Trees_SetUsed(baseType);
				/*update pointer base type*/
				Types_SetPointerBaseType(baseType, currPointerType);
				/*delete current node*/
				if (curr == unresolvedPointerTypes) {
					unresolvedPointerTypes = Trees_Right(curr);
				} else {
					Trees_SetRight(Trees_Right(curr), prev);
				}
			} else {
				Oberon_PrintError("error: record type expected in declaration of pointer base type: %s", baseTypeName);
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
		Oberon_PrintError("error: value expected: %s", Trees_Name(BaseIdent(exp)));
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
						Oberon_PrintError("error: undeclared identifier: %s", qualidentName);
						exit(EXIT_FAILURE);
					}
				} else {
					Oberon_PrintError("error: '.' expected after qualifier: %s", identName);
					exit(EXIT_FAILURE);
				}
			} else {
				qualidentSym = identSym;
				qualidentSelectorList = selectorList;
			}

		} else {
			Oberon_PrintError("error: undeclared identifier: %s", identName);
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
							Oberon_PrintError("error: invalid array index: %" OBNC_INT_MOD "d not between 0 and %" OBNC_INT_MOD "d", index, length - 1);
							exit(EXIT_FAILURE);
						}
					}
					Trees_SetType(currType, currSelector);
					currType = Types_ElementType(currTypeStruct);
				} else {
					Oberon_PrintError("error: array variable expected in element selector");
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
								Oberon_PrintError("error: undeclared field: %s", fieldName);
								exit(EXIT_FAILURE);
							}
							break;
						default:
							Oberon_PrintError("error: record variable expected in field selector");
							exit(EXIT_FAILURE);
					}
				} else {
					Oberon_PrintError("error: record variable expected in field selector");
					exit(EXIT_FAILURE);
				}
				break;
			case '^':
				if ((currType != NULL) && (Trees_Symbol(currTypeStruct) == POINTER)) {
					Trees_SetType(currType, currSelector);
					currType = Types_PointerBaseType(currTypeStruct);
				} else {
					Oberon_PrintError("error: pointer variable expected in pointer dereference");
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
											Oberon_PrintError("error: extended type expected: %s", Trees_Name(extendedType));
											exit(EXIT_FAILURE);
										}
									} else {
										if (Types_IsRecord(currType)) {
											if (Trees_Kind(BaseIdent(designator)) != TREES_VAR_PARAM_KIND) {
												Oberon_PrintError("error: variable parameter expected in type guard");
											} else {
												Oberon_PrintError("error: record type expected in type guard: %s", Trees_Name(extendedType));
											}
											exit(EXIT_FAILURE);
										} else {
											Oberon_PrintError("error: pointer type expected in type guard: %s", Trees_Name(extendedType));
											exit(EXIT_FAILURE);
										}
									}
								} else {
									Oberon_PrintError("error: type name expected: %s", Trees_Name(extendedType));
									exit(EXIT_FAILURE);
								}
							} else {
								Oberon_PrintError("error: undeclared identifier: %s", Trees_Name(extendedType));
								exit(EXIT_FAILURE);
							}
						} else {
							Oberon_PrintError("error: identifier expected in type guard");
							exit(EXIT_FAILURE);
						}
					} else {
						Oberon_PrintError("error: unexpected comma in type guard");
						exit(EXIT_FAILURE);
					}
				} else {
					Oberon_PrintError("error: unexpected parenthesis in designator which is not a record, pointer or procedure");
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
		Oberon_PrintError("error: unexpected selector after procedure call");
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
							Oberon_PrintError("warning: division by zero");
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
					Oberon_PrintError("error: positive divisor expected in DIV expression: %" OBNC_INT_MOD "d", Trees_Integer(expB));
					exit(EXIT_FAILURE);
				}
			}
			break;
		case MOD:
			if (IsInteger(expA) && IsInteger(expB)) {
				if (Trees_Integer(expB) > 0) {
					result = Trees_NewInteger(OBNC_MOD(Trees_Integer(expA), Trees_Integer(expB)));
				} else {
					Oberon_PrintError("error: positive divisor expected in MOD expression: %" OBNC_INT_MOD "d", Trees_Integer(expB));
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
			Oberon_PrintError("error: string too long in %s: %" OBNC_INT_MOD "d + 1 > %" OBNC_INT_MOD "d", errorContext, Types_StringLength(Trees_Type(expression)), Trees_Integer(Types_ArrayLength(targetType)));
			exit(EXIT_FAILURE);
		} else if (Types_IsPredeclaredProcedure(Trees_Type(expression))
				&& Types_IsProcedure(targetType)) {
			Oberon_PrintError("error: non-predeclared procedure expected in %s", errorContext);
			exit(EXIT_FAILURE);
		} else {
			Oberon_PrintError("error: incompatible types in %s: %s -> %s",
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
				Oberon_PrintError("error: array incompatible types in substitution of parameter %d in %s: %s -> %s", paramPos + 1, DesignatorString(procDesignator), TypeString(actualType), TypeString(formalType));
				exit(EXIT_FAILURE);
			}
		} else if (Trees_Kind(formalParam) == TREES_VALUE_PARAM_KIND) {
			if (! Types_AssignmentCompatible(actualParam, formalType)) {
				if (Types_IsString(actualType) && Types_IsCharacterArray(formalType)) {
					Oberon_PrintError("error: string too long in substitution of parameter %d: %" OBNC_INT_MOD "d + 1 > %" OBNC_INT_MOD "d", paramPos + 1, Types_StringLength(actualType), Trees_Integer(Types_ArrayLength(formalType)));
				} else {
					Oberon_PrintError("error: assignment incompatible types in substitution of parameter %d in %s: %s -> %s", paramPos + 1, DesignatorString(procDesignator), TypeString(actualType), TypeString(formalType));
				}
				exit(EXIT_FAILURE);
			}
		} else if (Trees_Kind(formalParam) == TREES_VAR_PARAM_KIND) {
			if (Types_IsRecord(formalType)) {
				if (Types_IsRecord(actualType)) {
					if (! Types_Extends(formalType, actualType)) {
						Oberon_PrintError("error: incompatible record types in substitution of parameter %d in %s: %s -> %s", paramPos + 1, DesignatorString(procDesignator), TypeString(actualType), TypeString(formalType));
						exit(EXIT_FAILURE);
					}
				} else {
					Oberon_PrintError("error: record expected in substitution of parameter %d in %s: %s -> %s", paramPos + 1, DesignatorString(procDesignator), TypeString(actualType), TypeString(formalType));
					exit(EXIT_FAILURE);
				}
			} else {
				if (! Types_Same(actualType, formalType)) {
					Oberon_PrintError("error: same types expected in substitution of parameter %d in %s: %s -> %s", paramPos + 1, DesignatorString(procDesignator), TypeString(actualType), TypeString(formalType));
					exit(EXIT_FAILURE);
				}
			}
		}
	} else {
		Oberon_PrintError("error: writable variable expected in substitution of parameter %d in %s",
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
		Oberon_PrintError("error: too few actual parameters in procedure call: %s", DesignatorString(procDesignator));
		exit(EXIT_FAILURE);
	} else if ((expList != NULL) && (fpList == NULL)) {
		Oberon_PrintError("error: too many actual parameters in procedure call: %s", DesignatorString(procDesignator));
		exit(EXIT_FAILURE);
	}
}


static void ValidateProcedureKind(const char procName[], int functionCallExpected, int isFunctionCall)
{
	if (isFunctionCall && ! functionCallExpected) {
		Oberon_PrintError("error: function procedure expected: %s", procName);
		exit(EXIT_FAILURE);
	} else if (! isFunctionCall && functionCallExpected) {
		Oberon_PrintError("error: proper procedure expected: %s", procName);
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
			Oberon_PrintError("error: %d parameter(s) expected: %s", min, procName);
		} else {
			Oberon_PrintError("error: %d or %d parameters expected: %s", min, max, procName);
		}
		exit(EXIT_FAILURE);
	}
}


static void ValidateTypeParameter(const char procName[], Trees_Node param, int pos)
{
	if (! (IsDesignator(param) && (Trees_Kind(BaseIdent(param)) == TREES_TYPE_KIND))) {
		Oberon_PrintError("error: type identifier expected in substitution of parameter %d: %s", pos + 1, procName);
		exit(EXIT_FAILURE);
	}
}


static void ValidateValueParameter(const char procName[], Trees_Node param, int pos)
{
	if (! IsValueExpression(param)) {
		Oberon_PrintError("error: expression expected in substitution of parameter %d: %s", pos + 1, procName);
		exit(EXIT_FAILURE);
	}
}


static void ValidateVariableParameter(const char procName[], Trees_Node param, int pos)
{
	if (! IsDesignator(param)) {
		Oberon_PrintError("error: variable expected in substitution of parameter %d: %s", pos + 1, procName);
		exit(EXIT_FAILURE);
	} else if (! Writable(param)) {
		Oberon_PrintError("error: writable variable expected in substitution of parameter %d: %s", pos + 1, procName);
		exit(EXIT_FAILURE);
	}
}


static void ValidateScalarParameter(const char procName[], Trees_Node paramType, int pos)
{
	assert(Types_IsType(paramType));

	if (! Types_Scalar(paramType)) {
		Oberon_PrintError("error: scalar type expected in substitution of parameter %d: %s", pos + 1, procName);
		exit(EXIT_FAILURE);
	}
}


static void ValidateIntegerParameter(const char procName[], Trees_Node param, int pos)
{
	if (! Types_IsInteger(Trees_Type(param))) {
		Oberon_PrintError("error: integer expected in substitution of parameter %d: %s", pos + 1, procName);
		exit(EXIT_FAILURE);
	}
}


static void ValidateRealParameter(const char procName[], Trees_Node param, int pos)
{
	if (! Types_IsReal(Trees_Type(param))) {
		Oberon_PrintError("error: parameter of type REAL expected in substitution of parameter %d: %s", pos + 1, procName);
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
					Oberon_PrintError("error: numeric parameter expected: %s", procName);
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
				Oberon_PrintError("error: array parameter expected: %s", procName);
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
						result = Trees_NewInteger((unsigned char) Trees_String(param[0])[0]);
					} else {
						Oberon_PrintError("error: single-character string parameter expected: %s", procName);
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
					Oberon_PrintError("error: character parameter expected: %s", procName);
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
				Oberon_PrintError("error: set expected in substitution of parameter 1: %s", procName);
				exit(EXIT_FAILURE);
			}
			break;
		case TREES_ASSERT_PROC:
			ValidateProcedureKind(procName, 0, isFunctionCall);
			ValidateParameterCount(procName, 1, 1, paramCount);
			ValidateValueParameter(procName, param[0], 0);
			if (! Types_IsBoolean(paramTypes[0])) {
				Oberon_PrintError("error: boolean parameter expected: %s", procName);
				exit(EXIT_FAILURE);
			}
			break;
		case TREES_NEW_PROC:
			ValidateProcedureKind(procName, 0, isFunctionCall);
			ValidateParameterCount(procName, 1, 1, paramCount);
			ValidateVariableParameter(procName, param[0], 0);
			if (! Types_IsPointer(paramTypes[0])) {
				Oberon_PrintError("error: pointer parameter expected: %s", procName);
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
			ValidateProcedureKind(procName, 0, isFunctionCall);
			ValidateParameterCount(procName, 2, 2, paramCount);
			ValidateValueParameter(procName, param[0], 0);
			ValidateIntegerParameter(procName, param[0], 0);
			ValidateVariableParameter(procName, param[1], 1);
			if (! Types_Basic(paramTypes[1])) {
				Oberon_PrintError("error: variable of basic type expected in substitution of parameter 2: %s", procName);
				exit(EXIT_FAILURE);
			}
			break;
		case TREES_PUT_PROC:
			ValidateProcedureKind(procName, 0, isFunctionCall);
			ValidateParameterCount(procName, 2, 2, paramCount);
			ValidateValueParameter(procName, param[0], 0);
			ValidateIntegerParameter(procName, param[0], 0);
			ValidateValueParameter(procName, param[1], 1);
			if (! Types_Basic(paramTypes[1]) && ! Types_IsSingleCharString(paramTypes[1])) {
				Oberon_PrintError("error: expression of basic type expected in substitution of parameter 2: %s", procName);
				exit(EXIT_FAILURE);
			}
			break;
		case TREES_COPY_PROC:
			ValidateProcedureKind(procName, 0, isFunctionCall);
			ValidateParameterCount(procName, 3, 3, paramCount);
			ValidateValueParameter(procName, param[0], 0);
			ValidateIntegerParameter(procName, param[0], 0);
			ValidateValueParameter(procName, param[1], 1);
			ValidateIntegerParameter(procName, param[1], 1);
			ValidateValueParameter(procName, param[2], 2);
			ValidateIntegerParameter(procName, param[2], 2);
			if (IsInteger(param[2]) && (Trees_Integer(param[2]) < 0)) {
				Oberon_PrintError("warning: non-negative count expected in %s: %" OBNC_INT_MOD "d", procName, Trees_Integer(param[2]));
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
			Oberon_PrintError("error: procedure expected");
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
					Oberon_PrintError("error: function procedure expected: %s", Trees_Name(ident));
					exit(EXIT_FAILURE);
				}
			} else if (resultType != NULL) {
				Oberon_PrintError("error: proper procedure expected: %s", Trees_Name(ident));
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
		Oberon_PrintError("error: case label defined twice: %" OBNC_INT_MOD "d", aMin);
		exit(EXIT_FAILURE);
	} else if ((bMin >= aMin) && (bMin <= aMax)) {
		Oberon_PrintError("error: case label defined twice: %" OBNC_INT_MOD "d", bMin);
		exit(EXIT_FAILURE);
	}
}


static void CheckCharLabelDisjointness(Trees_Node rangeA, Trees_Node rangeB)
{
	char aMin, aMax, bMin, bMax;
	int hasRepeatedLabel, repeatedLabel;

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
		hasRepeatedLabel = 1;
		repeatedLabel = aMin;
	} else if ((bMin >= aMin) && (bMin <= aMax)) {
		hasRepeatedLabel = 1;
		repeatedLabel = bMin;
	} else {
		hasRepeatedLabel = 0;
	}
	if (hasRepeatedLabel) {
		if (isprint(repeatedLabel)) {
			Oberon_PrintError("error: case label defined twice: \"%c\"", repeatedLabel);
		} else {
			Oberon_PrintError("error: case label defined twice: 0%XX", repeatedLabel);
		}
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
					Oberon_PrintError("error: type case label defined twice: %s", Trees_Name(newLabelRange));
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
