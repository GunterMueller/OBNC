/*Copyright (C) 2017 Karl Landstrom <karl@miasap.se>

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
%}

%union {
	const char *ident;
	int integer;
	double real;
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
%type <integer> OpenArrayOpt
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
		$$ = Trees_NewIdent(QualidentName($1, $3));
	}
	;

identdef:
	IDENT ExportMarkOpt
	{
		if (! Table_LocallyDeclared($1)) {
			$$ = Trees_NewIdent($1);
			if ($2) {
				Trees_SetExported($$);		
			}
			if (Table_ScopeLocal()) {
				Trees_SetLocal($$);
			}
		} else {
			yyerror("error: identifier already declared in this scope: %s", $1);
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
			Trees_SetValue($3, $1);
			Table_Put($1);
			Generate_ConstDeclaration($1);
		} else {
			yyerror("error: cannot export local constant: %s", Trees_Name($1));
			YYABORT;
		}
	}
	;

ConstExpression:
	expression
	{
		switch (Trees_Symbol($1)) {
			case TRUE:
			case FALSE:
			case STRING:
			case TREES_CHAR_CONSTANT:
			case INTEGER:
			case REAL:
			case TREES_SET_CONSTANT:
			case NIL:
				$$ = $1;
				break;
			default:
				yyerror("error: constant expression expected");
				YYABORT;				
		}
	}
	;


/*TYPE DECLARATION RULES*/

TypeDeclaration:
	TypeIdentDef type
	{
		Trees_Node sourceType;
	
		sourceType = ResolvedType($2);
		if (sourceType != NULL) {
			if (Trees_Exported($1) && Trees_Local($1)) {
				yyerror("error: cannot export local type: %s", Trees_Name($1));
				YYABORT;
			} else {
				Trees_SetType(sourceType, $1);
				if (! Table_LocallyDeclared(Trees_Name($1))) {
					Table_Put($1);
				}
				ResolvePointerTypes($1);
				currentTypeIdentdef = NULL;
				Generate_TypeDeclaration($1);
			}
		} else {
			yyerror("error: undeclared identifier: %s", Trees_Name($2));
			YYABORT;
		}
	}
	;

TypeIdentDef:
	identdef '='
	{
		Trees_SetKind(TREES_TYPE_KIND, $1);
		currentTypeIdentdef = $1;
		Table_Put(currentTypeIdentdef);
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
		
		$$ = ResolvedType($2);
		if ($$ != NULL) {
			reversedLengths = $1;
			do {
				length = Trees_Left(reversedLengths);
				$$ = Types_NewArray(length, $$);
				reversedLengths = Trees_Right(reversedLengths);
			} while (reversedLengths != NULL);
		} else {
			yyerror("error: undeclared identifier: %s", Trees_Name($2));
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
		if (IsInteger($1)) {
			if (Trees_Integer($1) > 0) {
				$$ = $1;
			} else {
				yyerror("error: positive length expected");
				YYABORT;
			}
		} else {
			yyerror("error: integer length expected");
			YYABORT;		
		}
	}
	;

RecordType:
	RecordHeading FieldListSequenceOpt END
	{
		Table_CloseScope();
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
		Table_OpenScope();
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
				switch (Trees_Symbol(Types_Structure(symbol))) {
					case RECORD:
					case POINTER:
						$$ = symbol;
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
		$$ = Trees_NewNode(TREES_FIELD_LIST_SEQUENCE, $3, $1);
	}
	;

FieldList:
	IdentList ':' type
	{
		Trees_Node type, identList, ident;

		$$ = NULL;
		type = ResolvedType($3);
		if (type != NULL) {
			if (! ((type == currentTypeIdentdef) && ! Types_IsPointer(type))) {
				Trees_ReverseList(&$1); /*correct order*/
				identList = $1;
				do {
					ident = Trees_Left(identList);
					Trees_SetKind(TREES_FIELD_KIND, ident);
					Trees_SetType(type, ident);
					Table_Put(ident);
					identList = Trees_Right(identList);
				} while (identList != NULL);
		
				$$ = $1;			
			} else {
				yyerror("error: recursive field type must be a pointer: %s", Trees_Name($3));
				YYABORT;					
			}
		} else {
			yyerror("error: undeclared type: %s", Trees_Name($3));
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
					yyerror("error: record expected as pointer base type: %s", baseTypeName);
					YYABORT;
				}
			} else {
				Trees_SetKind(TREES_TYPE_KIND, $2);
				Trees_SetType(Types_NewRecord(NULL, NULL), $2);
				$$ = Types_NewPointer($2);
				unresolvedPointerTypes = Trees_NewNode(TREES_NOSYM, $$, unresolvedPointerTypes);
			}
		} else if(Trees_Symbol($2) == RECORD) {
			$$ = Types_NewPointer($2);
		} else {
			yyerror("error: record expected as pointer base type");
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
		Table_CloseScope();
		$$ = $2;
	}
	;

ProcedureTypeSansParam:
	PROCEDURE
	{
		Table_OpenScope();
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

		type = ResolvedType($3);
		if (type != NULL) {
			Trees_ReverseList(&$1); /*correct order*/
			identList = $1;
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
		
			Generate_VariableDeclaration($1);
		} else {
			yyerror("error: undeclared identifier: %s", Trees_Name($3));
			exit(EXIT_FAILURE);
		}
	}
	;


/*EXPRESSION RULES*/

expression:
	SimpleExpression
	| SimpleExpression relation SimpleExpression
	{
		Trees_Node leftExp, rightExp, leftType, rightType;
		int relation, leftSym, rightSym, error;
		
		leftExp = $1;
		rightExp = $3;
		relation = $2;
		leftSym = Trees_Symbol($1);
		rightSym = Trees_Symbol($3);
		leftType = Trees_Type($1);
		rightType = Trees_Type($3);

		CheckIsValue($1);
		if ($2 != IS) {
			CheckIsValue($3);
		}

		$$ = NULL;
		/*check types and evaluate constant expressions*/
		error = 0;
		switch (relation) {
			case '=':
				switch (Trees_Symbol(Types_Structure(leftType))) {
					case TREES_BOOLEAN_TYPE:
						if (Types_ExpressionCompatible(leftType, rightType)) {
							if (IsBoolean(leftExp) && IsBoolean(rightExp)) {
								$$ = Trees_NewLeaf((leftSym == rightSym)? TRUE: FALSE);
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
								$$ = Trees_NewLeaf((Trees_Char(leftExp) == Trees_Char(rightExp))? TRUE: FALSE);
							}
						} else {
							error = 1;
						}
						break;
					case TREES_INTEGER_TYPE:	
						if (Types_ExpressionCompatible(leftType, rightType)) {
							if (IsInteger(leftExp) && IsInteger(rightExp)) {
								$$ = Trees_NewLeaf((Trees_Integer(leftExp) == Trees_Integer(rightExp))? TRUE: FALSE);
							}
						} else {
							error = 1;
						}
						break;
					case TREES_REAL_TYPE:
						if (Types_ExpressionCompatible(leftType, rightType)) {
							if (IsReal(leftExp) && IsReal(rightExp)) {
								$$ = Trees_NewLeaf((Trees_Real(leftExp) == Trees_Real(rightExp))? TRUE: FALSE);
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
								$$ = Trees_NewLeaf((Trees_Set(leftExp) == Trees_Set(rightExp))? TRUE: FALSE);
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
								$$ = Trees_NewLeaf((strcmp(Trees_String(leftExp), Trees_String(rightExp)) == 0)? TRUE: FALSE);
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
								$$ = Trees_NewLeaf((leftSym != rightSym)? TRUE: FALSE);
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
								$$ = Trees_NewLeaf((Trees_Char(leftExp) != Trees_Char(rightExp))? TRUE: FALSE);
							}
						} else {
							error = 1;
						}
						break;
					case TREES_INTEGER_TYPE:	
						if (Types_ExpressionCompatible(leftType, rightType)) {
							if (IsInteger(leftExp) && IsInteger(rightExp)) {
								$$ = Trees_NewLeaf((Trees_Integer(leftExp) != Trees_Integer(rightExp))? TRUE: FALSE);
							}
						} else {
							error = 1;
						}
						break;
					case TREES_REAL_TYPE:
						if (Types_ExpressionCompatible(leftType, rightType)) {
							if (IsReal(leftExp) && IsReal(rightExp)) {
								$$ = Trees_NewLeaf((Trees_Real(leftExp) != Trees_Real(rightExp))? TRUE: FALSE);
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
								$$ = Trees_NewLeaf((Trees_Set(leftExp) != Trees_Set(rightExp))? TRUE: FALSE);
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
								$$ = Trees_NewLeaf((strcmp(Trees_String(leftExp), Trees_String(rightExp)) != 0)? TRUE: FALSE);
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
								$$ = Trees_NewLeaf((Trees_Char(leftExp) < Trees_Char(rightExp))? TRUE: FALSE);
							}
						} else {
							error = 1;
						}
						break;
					case TREES_INTEGER_TYPE:	
						if (Types_ExpressionCompatible(leftType, rightType)) {
							if (IsInteger(leftExp) && IsInteger(rightExp)) {
								$$ = Trees_NewLeaf((Trees_Integer(leftExp) < Trees_Integer(rightExp))? TRUE: FALSE);
							}
						} else {
							error = 1;
						}
						break;
					case TREES_REAL_TYPE:
						if (Types_ExpressionCompatible(leftType, rightType)) {
							if (IsReal(leftExp) && IsReal(rightExp)) {
								$$ = Trees_NewLeaf((Trees_Real(leftExp) < Trees_Real(rightExp))? TRUE: FALSE);
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
								$$ = Trees_NewLeaf((strcmp(Trees_String(leftExp), Trees_String(rightExp)) < 0)? TRUE: FALSE);
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
								$$ = Trees_NewLeaf((Trees_Char(leftExp) <= Trees_Char(rightExp))? TRUE: FALSE);
							}
						} else {
							error = 1;
						}
						break;
					case TREES_INTEGER_TYPE:	
						if (Types_ExpressionCompatible(leftType, rightType)) {
							if (IsInteger(leftExp) && IsInteger(rightExp)) {
								$$ = Trees_NewLeaf((Trees_Integer(leftExp) <= Trees_Integer(rightExp))? TRUE: FALSE);
							}
						} else {
							error = 1;
						}
						break;
					case TREES_REAL_TYPE:
						if (Types_ExpressionCompatible(leftType, rightType)) {
							if (IsReal(leftExp) && IsReal(rightExp)) {
								$$ = Trees_NewLeaf((Trees_Real(leftExp) <= Trees_Real(rightExp))? TRUE: FALSE);
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
								$$ = Trees_NewLeaf((strcmp(Trees_String(leftExp), Trees_String(rightExp)) <= 0)? TRUE: FALSE);
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
								$$ = Trees_NewLeaf((Trees_Char(leftExp) > Trees_Char(rightExp))? TRUE: FALSE);
							}
						} else {
							error = 1;
						}
						break;
					case TREES_INTEGER_TYPE:	
						if (Types_ExpressionCompatible(leftType, rightType)) {
							if (IsInteger(leftExp) && IsInteger(rightExp)) {
								$$ = Trees_NewLeaf((Trees_Integer(leftExp) > Trees_Integer(rightExp))? TRUE: FALSE);
							}
						} else {
							error = 1;
						}
						break;
					case TREES_REAL_TYPE:
						if (Types_ExpressionCompatible(leftType, rightType)) {
							if (IsReal(leftExp) && IsReal(rightExp)) {
								$$ = Trees_NewLeaf((Trees_Real(leftExp) > Trees_Real(rightExp))? TRUE: FALSE);
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
								$$ = Trees_NewLeaf((strcmp(Trees_String(leftExp), Trees_String(rightExp)) > 0)? TRUE: FALSE);
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
								$$ = Trees_NewLeaf((Trees_Char(leftExp) >= Trees_Char(rightExp))? TRUE: FALSE);
							}
						} else {
							error = 1;
						}
						break;
					case TREES_INTEGER_TYPE:	
						if (Types_ExpressionCompatible(leftType, rightType)) {
							if (IsInteger(leftExp) && IsInteger(rightExp)) {
								$$ = Trees_NewLeaf((Trees_Integer(leftExp) >= Trees_Integer(rightExp))? TRUE: FALSE);
							}
						} else {
							error = 1;
						}
						break;
					case TREES_REAL_TYPE:
						if (Types_ExpressionCompatible(leftType, rightType)) {
							if (IsReal(leftExp) && IsReal(rightExp)) {
								$$ = Trees_NewLeaf((Trees_Real(leftExp) >= Trees_Real(rightExp))? TRUE: FALSE);
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
								$$ = Trees_NewLeaf((strcmp(Trees_String(leftExp), Trees_String(rightExp)) >= 0)? TRUE: FALSE);
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
					if (IsInteger(leftExp)) {
						Range_CheckSetElement(Trees_Integer(leftExp));
						if (IsSet(rightExp)) {
							$$ = Trees_NewLeaf(OBNC_IN(Trees_Integer(leftExp), Trees_Set(rightExp))? TRUE: FALSE);
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
			if ($$ == NULL) {
				$$ = Trees_NewNode(relation, leftExp, rightExp);
			}
			Trees_SetType(Trees_NewLeaf(TREES_BOOLEAN_TYPE), $$);
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
		Trees_Node term;
		int sign;
		
		if ($1 >= 0) {
			CheckIsValue($2);
		}

		term = $2;
		sign = $1;
		
		$$ = NULL;
		switch (sign) {
			case '+':
				switch (TypeStructureSymbol(term)) {
					case TREES_INTEGER_TYPE:
					case TREES_REAL_TYPE:
					case TREES_BYTE_TYPE:
						break;
					default:
						yyerror("error: number expected in unary operation \"+\"");
						YYABORT;
				}
				if ($$ == NULL) {
					$$ = Trees_NewNode('+', term, NULL);
				}
				break;
			case '-':
				switch (TypeStructureSymbol(term)) {
					case TREES_INTEGER_TYPE:
						if (IsInteger(term)) {
							Range_CheckIntDiff(0, Trees_Integer(term));
							$$ = Trees_NewInteger(-Trees_Integer(term));
						}
						break;
					case TREES_REAL_TYPE:
						if (IsReal(term)) {
							Range_CheckRealDiff(0.0, Trees_Real(term));
							$$ = Trees_NewReal(-Trees_Real(term));
						}
						break;
					case TREES_BYTE_TYPE:
						break;
					case TREES_SET_TYPE:
						if (IsSet(term)) {
							$$ = Trees_NewSet(~Trees_Set(term));
						}
						break;
					default:
						yyerror("error: number or set expected in unary operation \"-\"");
						YYABORT;
				}
				if ($$ == NULL) {
					$$ = Trees_NewNode('-', term, NULL);
				}
				break;
			default:
				$$ = term;
		}
		assert($$ != NULL);
		Trees_SetType(Trees_Type(term), $$);
	}
	| SimpleExpression AddOperator term
	{
		Trees_Node leftExp, rightExp, leftType, rightType, resultType;
		int operator, leftSym, rightSym;

		CheckIsValue($1);
		CheckIsValue($3);

		leftExp = $1;
		rightExp = $3;
		operator = $2;
		leftSym = Trees_Symbol($1);
		rightSym = Trees_Symbol($3);
		leftType = Trees_Type($1);
		rightType = Trees_Type($3);
				
		/*check types and evaluate constant expressions*/
		$$ = NULL;
		resultType = NULL;
		if (Types_ExpressionCompatible(leftType, rightType)) {
			switch (operator) {
				case '+':
					switch (Trees_Symbol(Types_Structure(leftType))) {
						case TREES_INTEGER_TYPE:	
							if (IsInteger(leftExp) && IsInteger(rightExp)) {
								Range_CheckIntSum(Trees_Integer(leftExp), Trees_Integer(rightExp));
								$$ = Trees_NewInteger(Trees_Integer(leftExp) + Trees_Integer(rightExp));
							}
							break;
						case TREES_REAL_TYPE:	
							if (IsReal(leftExp) && IsReal(rightExp)) {
								Range_CheckRealSum(Trees_Real(leftExp), Trees_Real(rightExp));
								$$ = Trees_NewReal(Trees_Real(leftExp) + Trees_Real(rightExp));
							}
							break;
						case TREES_BYTE_TYPE:
							if (Types_IsInteger(rightType)) {
								resultType = rightType;
							}
							break;
						case TREES_SET_TYPE:	
							if (IsSet(leftExp) && IsSet(rightExp)) {
								$$ = Trees_NewSet(Trees_Set(leftExp) | Trees_Set(rightExp));
							}
							break;
						default:
							yyerror("error: numeric value or set expected in operation \"+\"");
							YYABORT;
					}
					break;
				case '-':
					switch (Trees_Symbol(Types_Structure(leftType))) {
						case TREES_INTEGER_TYPE:	
							if (IsInteger(leftExp) && IsInteger(rightExp)) {
								Range_CheckIntDiff(Trees_Integer(leftExp), Trees_Integer(rightExp));
								$$ = Trees_NewInteger(Trees_Integer(leftExp) - Trees_Integer(rightExp));
							}
							break;
						case TREES_REAL_TYPE:	
							if (IsReal(leftExp) && IsReal(rightExp)) {
								Range_CheckRealDiff(Trees_Real(leftExp), Trees_Real(rightExp));
								$$ = Trees_NewReal(Trees_Real(leftExp) - Trees_Real(rightExp));
							}
							break;
						case TREES_BYTE_TYPE:
							if (Types_IsInteger(rightType)) {
								resultType = rightType;
							}
							break;
						case TREES_SET_TYPE:	
							if (IsSet(leftExp) && IsSet(rightExp)) {
								$$ = Trees_NewSet(Trees_Set(leftExp) & ~Trees_Set(rightExp));
							}
							break;
						default:
							yyerror("error: numeric value or set expected in operation \"-\"");
							YYABORT;
					}
					break;
				case OR:
					if (IsBoolean(leftExp) && IsBoolean(rightType)) {
						$$ = Trees_NewLeaf(((leftSym == TRUE) || (rightSym == TRUE))? TRUE: FALSE);
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
		
		if ($$ == NULL) {
			$$ = Trees_NewNode(operator, leftExp, rightExp);
		}
		if (resultType == NULL) {
			resultType = Trees_Type($1);
		}
		Trees_SetType(resultType, $$);		
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
		Trees_Node leftExp, rightExp, leftType, rightType, resultType;
		int operator, leftSym, rightSym;

		CheckIsValue($1);
		CheckIsValue($3);

		leftExp = $1;
		rightExp = $3;
		operator = $2;
		leftSym = Trees_Symbol($1);
		rightSym = Trees_Symbol($3);
		leftType = Types_Structure(Trees_Type($1));
		rightType = Types_Structure(Trees_Type($3));

		/*check types and evaluate constant expressions*/
		$$ = NULL;
		resultType = NULL;
		if (Types_ExpressionCompatible(leftType, rightType)) {
			switch (operator) {
				case '*':
					switch (Trees_Symbol(leftType)) {
						case TREES_INTEGER_TYPE:	
							if (IsInteger(leftExp) && IsInteger(rightExp)) {
								Range_CheckIntProd(Trees_Integer(leftExp), Trees_Integer(rightExp));
								$$ = Trees_NewInteger(Trees_Integer(leftExp) * Trees_Integer(rightExp));
							}
							break;
						case TREES_REAL_TYPE:	
							if (IsReal(leftExp) && IsReal(rightExp)) {
								Range_CheckRealProd(Trees_Real(leftExp), Trees_Real(rightExp));
								$$ = Trees_NewReal(Trees_Real(leftExp) * Trees_Real(rightExp));
							}
							break;
						case TREES_BYTE_TYPE:
							if (Types_IsInteger(rightType)) {
								resultType = rightType;
							}
							break;
						case TREES_SET_TYPE:	
							if (IsSet(leftExp) && IsSet(rightExp)) {
								$$ = Trees_NewSet(Trees_Set(leftExp) & Trees_Set(rightExp));
							}
							break;
						default:
							yyerror("error: numeric value or set expected in operation \"*\"");
							YYABORT;
					}
					break;
				case '/':
					switch (Trees_Symbol(leftType)) {
						case TREES_REAL_TYPE:	
							if (IsReal(leftExp) && IsReal(rightExp)) {
								if (Trees_Real(rightExp) != 0) {
									$$ = Trees_NewReal(Trees_Real(leftExp) / Trees_Real(rightExp));
								} else {
									yyerror("warning: division by zero");
								}
							}
							break;
						case TREES_SET_TYPE:	
							if (IsSet(leftExp) && IsSet(rightExp)) {
								$$ = Trees_NewSet(Trees_Set(leftExp) ^ Trees_Set(rightExp));
							}
							break;
						default:
							yyerror("error: real number or set expected in operation \"/\"");
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
										$$ = Trees_NewInteger(OBNC_DIV(Trees_Integer(leftExp), Trees_Integer(rightExp)));
									} else {
										$$ = Trees_NewInteger(OBNC_MOD(Trees_Integer(leftExp), Trees_Integer(rightExp)));
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
								yyerror("error: integer expected in operation DIV");
							} else {
								yyerror("error: integer expected in operation MOD");
							}
							YYABORT;
					}
					break;
				case '&':
					if (IsBoolean(leftExp) && IsBoolean(rightExp)) {
						$$ = Trees_NewLeaf(((leftSym == TRUE) && (rightSym == TRUE))? TRUE: FALSE);
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
		
		if ($$ == NULL) {
			$$ = Trees_NewNode(operator, leftExp, rightExp);
		}
		if (resultType == NULL) {
			resultType = Trees_Type($1);
		}
		Trees_SetType(resultType, $$);		
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
		$$ = Trees_NewLeaf(TRUE);
		Trees_SetType(Trees_NewLeaf(TREES_BOOLEAN_TYPE), $$);
	}
	| FALSE
	{
		$$ = Trees_NewLeaf(FALSE);
		Trees_SetType(Trees_NewLeaf(TREES_BOOLEAN_TYPE), $$);
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
	}
	| '(' expression ')'
	{
		CheckIsValue($2);
		$$ = $2;
	}
	| '~' factor
	{
		CheckIsValue($2);
		if (Types_IsBoolean(Trees_Type($2))) {
			switch (Trees_Symbol($2)) {
				case TRUE:
					$$ = Trees_NewLeaf(FALSE);
					break;
				case FALSE:
					$$ = Trees_NewLeaf(TRUE);
					break;
				default:
					$$ = Trees_NewNode('~', $2, NULL);
			}
			Trees_SetType(Trees_NewLeaf(TREES_BOOLEAN_TYPE), $$);
		} else {
			yyerror("error: boolean operand expected in negation");
			YYABORT;
		}
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

		identType = Trees_Type(Trees_Left(designator));
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
				yyerror("error: integer array index expected");
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
	| '('  ')' /*actual parameters*/
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
		}
	}
	;

element:
	expression
	{
		int i;
		Trees_Node type;
		
		CheckIsValue($1);
		$$ = NULL;
		type = Trees_Type($1);
		if (IsInteger($1)) {
			i = Trees_Integer($1);
			Range_CheckSetElement(i);
			$$ = Trees_NewSet(1 << i);
		} else if (Types_IsInteger(type)) {
			$$ = Trees_NewNode(TREES_SINGLE_ELEMENT_SET, $1, NULL);
			Trees_SetType(Trees_NewLeaf(TREES_SET_TYPE), $$);
		} else {
			yyerror("error: element must have integer type");
			YYABORT;
		}
	}
	| expression DOTDOT expression
	{
		CheckIsValue($1);
		CheckIsValue($3);
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
			yyerror("error: element must have integer type");
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
		
		CheckIsValue($3);
		switch (Trees_Symbol($1)) {
			case TREES_DESIGNATOR:
				designator = $1;
				exp = $3;
				ident = Trees_Left($1);
				designatorType = Trees_Type($1);
				switch (Trees_Kind(ident)) {
					case TREES_VARIABLE_KIND:
					case TREES_VALUE_PARAM_KIND:
					case TREES_VAR_PARAM_KIND:
						if (Writable($1)) {
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
				$$ = Trees_NewNode(BECOMES, designator, exp);
				break;
			case TREES_PROCEDURE_CALL:
				yyerror("error: unexpected procedure call in assignment target");
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
		if ($$ == NULL) {
			YYABORT;
		}
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
		CheckIsValue($1);
		if (Types_IsBoolean(Trees_Type($1))) {
			$$ = $1;
		} else {
			yyerror("error: boolean expression expected");
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
		expType = Trees_Type($2);
		if (Types_IsRecord(expType) || Types_IsPointer(expType)) {
			/*reset original type*/
			caseVariable = Trees_Left($2);
			Trees_SetType(caseExpressionType, caseVariable);
		}
		$$ = Trees_NewNode(CASE, $2, $4);
	}
	;
	
CaseExpression:
	expression
	{
		Trees_Node typeStruct, caseVariable;

		CheckIsValue($1);
		typeStruct = Types_Structure(Trees_Type($1));
		switch (Trees_Symbol(typeStruct)) {
			case RECORD:
				/*fall through*/
			case POINTER:
				if (IsDesignator($1) && (Trees_Right($1) == NULL)) {
					caseVariable = Trees_Left($1);
					if (! Types_IsRecord(typeStruct) || (Trees_Kind(caseVariable) == TREES_VAR_PARAM_KIND)) {
						$$ = $1;
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
				currentCaseExpression = $1;
				caseExpressionType = Trees_Type($1);
				currentlyDefinedCaseLabels = NULL;
				$$ = $1;
				break;
			default:
				yyerror("error: invalid type of CASE expression");
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
				yyerror("error: unexpected list of type name labels");
				YYABORT;				
		}
	}
	;

LabelRange:
	label
	{
		$$ = $1;
		CheckCaseLabelUniqueness($1);
		currentlyDefinedCaseLabels = Trees_NewNode(TREES_NOSYM, $1, currentlyDefinedCaseLabels);
	}
	| label DOTDOT label
	{
		const int rangeLenMax = 255;
		int leftSym, rightSym;
		int rangeMin, rangeMax;
		
		leftSym = Trees_Symbol($1);
		rightSym = Trees_Symbol($3);
		if (leftSym == rightSym) {
			switch (leftSym) {
				case INTEGER:
					rangeMin = Trees_Integer($1);
					rangeMax = Trees_Integer($3);
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
					if (Trees_Char($1) >= Trees_Char($3)) {
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
		$$ = Trees_NewNode(DOTDOT, $1, $3);
		CheckCaseLabelUniqueness($$);
		currentlyDefinedCaseLabels = Trees_NewNode(TREES_NOSYM, $$, currentlyDefinedCaseLabels);
	}
	;

label:
	INTEGER
	{
		if (Types_IsInteger(Trees_Type(currentCaseExpression))) {
			$$ = Trees_NewInteger($1);
		} else {
			yyerror("error: unexpected integer label");
			YYABORT;
		}
	}
	| STRING
	{
		if (Types_IsChar(Trees_Type(currentCaseExpression))) {
			if (strlen($1) <= 1) {
				$$ = Trees_NewChar($1[0]);
			} else {
				yyerror("error: single-character string expected");
				YYABORT;
			}
		} else {
			yyerror("error: unexpected string label");
			YYABORT;
		}
	}
	| qualident
	{
		Trees_Node caseExpTypeStruct, constValue, caseVariable;
				
		$$ = Table_At(Trees_Name($1));
		if ($$ != NULL) {
			caseExpTypeStruct = Types_Structure(Trees_Type(currentCaseExpression));
			switch (Trees_Symbol(caseExpTypeStruct)) {
				case TREES_INTEGER_TYPE:
					constValue = Trees_Value($$);
					if (Trees_Symbol(constValue) == INTEGER) {
						if (Trees_Integer(constValue) >= 0) {
							$$ = constValue;
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
					constValue = Trees_Value($$);
					if (Trees_Symbol(constValue) == STRING) {
						if (Types_StringLength(Trees_Type(constValue)) <= 1) {
							$$ = Trees_NewChar(Trees_String(constValue)[0]);
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
					if (Types_IsRecord($$)) {
						if (Types_Extends(Trees_Type(currentCaseExpression), $$)) {
							caseVariable = Trees_Left(currentCaseExpression);
							Trees_SetType($$, caseVariable);
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
					if (Types_IsPointer($$)) {
						if (Types_Extends(Trees_Type(currentCaseExpression), $$)) {
							caseVariable = Trees_Left(currentCaseExpression);
							Trees_SetType($$, caseVariable);
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
			yyerror("error: undeclared identifier: %s", Trees_Name($1));
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
		CheckIsValue($4);
		$$ = NULL;
		if (Types_IsBoolean(Trees_Type($4))) {
			$$ = Trees_NewNode(REPEAT, $2, $4);
		} else {
			yyerror("error: boolean expression expected");
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

		CheckIsValue($3);
		ctrlVar = Table_At($1);
		if (ctrlVar != NULL) {
			ctrlVarType = Trees_Type(ctrlVar);
			if (Types_IsInteger(ctrlVarType)) {
				if (Types_IsInteger(Trees_Type($3))) {
					$$ = Trees_NewNode(BECOMES, ctrlVar, $3);
				} else {
					yyerror("error: integer expression expected as initial value");
					YYABORT;
				}
			} else {
				yyerror("error: integer control variable expected: %s", $1);
				YYABORT;				
			}
		} else {
			yyerror("error: undeclared control variable: %s", $1);
			YYABORT;
		}	
	}

ForLimit:
	expression
	{
		CheckIsValue($1);
		if (! Types_IsInteger(Trees_Type($1))) {
			yyerror("error: integer expression expected as upper limit");
			YYABORT;
		}
	}
	;

ByOpt:
	BY ConstExpression
	{
		if (IsInteger($2)) {
			if (Trees_Integer($2) != 0) {
				$$ = $2;
			} else {
				yyerror("error: non-zero increment expected");
				YYABORT;
			}
		} else {
			yyerror("error: integer increment expected");
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
	;

ProcedureHeading:
	ProcedureHeadingSansParam FormalParametersOpt
	{
		Trees_Node paramList, param;

		$$ = NULL;
		Table_CloseScope();
		Trees_SetType($2, $1);
		Table_OpenScope();
	
		/*reenter parameters in the symbol table*/
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
			yyerror("error: cannot export local procedure: %s", Trees_Name($2));
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
			yyerror("error: undeclared pointer base type: %s", Trees_Name(undeclaredBaseType));
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
		Trees_Node curr;

		/*make one list of the two lists*/
		$$ = $1;
		curr = $3;
		do {
			$$ = Trees_NewNode(TREES_IDENT_LIST, Trees_Left(curr), $$);
			curr = Trees_Right(curr);
		} while (curr != NULL);
		/*$$ in reversed order*/
	}
	;

ResultTypeOpt:
	':' qualident
	{
		$$ = ResolvedType($2);
		if ($$ != NULL) {
			if (Trees_Symbol($$) == IDENT) {
				if (Trees_Kind($$) != TREES_TYPE_KIND) {
					yyerror("error: type name expected as result type: %s", Trees_Name($2));
					YYABORT;
				}				
				if (! Types_Scalar($$)) {
					yyerror("error: scalar result type expected: %s", Trees_Name($2));
					YYABORT;					
				}
			}
		} else {
			yyerror("error: undeclared identifier: %s", Trees_Name($2));
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
			Trees_SetKind($1, ident);
			Trees_SetType($4, ident);
			Trees_SetLocal(ident);	
			if (! Table_LocallyDeclared(Trees_Name(ident))) {
				Table_Put(ident);
			} else {
				yyerror("error: redeclaration of identifier with the same name: %s", Trees_Name(ident));
				YYABORT;
			}
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
		$$ = Trees_NewNode(TREES_IDENT_LIST, Trees_NewIdent($3), $1);
	}
	;

FormalType:
	OpenArrayOpt qualident
	{
		$$ = ResolvedType($2);
		if ($$ != NULL) {
			if ($1) {
				$$ = Types_NewArray(NULL, $$);
			}
		} else {
			yyerror("error: undeclared identifier: %s", Trees_Name($2));
			exit(EXIT_FAILURE);
		}
	}
	;

OpenArrayOpt:
	ARRAY OF
	{
		$$ = 1;
	}
	| /*empty*/
	{
		$$ = 0;
	}
	;


/*MODULE RULES*/

module:
	ModuleHeading ';' ImportListOpt DeclarationSequence ModuleStatements END IDENT '.'
	{
		static char symfilePath[PATH_MAX + 1];
		
		if (strcmp($7, Args_ModuleName()) == 0) {
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
	;


ModuleHeading:
	MODULE IDENT
	{
		if (strcmp($2, Args_ModuleName()) == 0) {
			if (Args_Mode() != ARGS_IMPORT_LIST_MODE) {
				Generate_ModuleHeading();
			}
		} else {
			yyerror("error: module name does not match filename: %s", $2);
			YYABORT;
		}
	}
	;

ImportListOpt:
	ImportList
	{
		if (Args_Mode() == ARGS_IMPORT_LIST_MODE) {
			YYACCEPT;
		}
	}
	| /*empty*/
	{
		if (Args_Mode() == ARGS_IMPORT_LIST_MODE) {
			YYACCEPT;
		}
	}
	;

ImportList:
	IMPORT ImportRep ';'
	{
		static char impfilePath[PATH_MAX + 1];
		Trees_Node moduleAndDirPath, module, p;
		FILE *impFile;
		const char *name;
		
		Trees_ReverseList(&$2); /*correct order*/
		if (Args_Mode() == ARGS_IMPORT_LIST_MODE) {
			while ($2 != NULL) {
				module = Trees_Left($2);
				puts(Trees_Name(module));
				$2 = Trees_Right($2);
			}
		} else {
			sprintf(impfilePath, ".obnc/%s.imp", Args_ModuleName());
			if (Args_Mode() == ARGS_ENTRY_POINT_MODE) {
				if (Files_Exists(impfilePath)) {
					Files_Remove(impfilePath);
				}
			} else {
				impFile = Files_New(impfilePath);
				p = $2;
				do {
					moduleAndDirPath = Trees_Left(p);
					module = Trees_Left(moduleAndDirPath);
					name = Trees_UnaliasedName(module);
					fprintf(impFile, "%s\n", name);
					p = Trees_Right(p);
				} while (p != NULL);
				Files_Close(impFile);
			}
			Generate_ImportList($2);
		}
	}
	;

ImportRep:
	import
	{
		$$ = Trees_NewNode(TREES_NOSYM, $1, NULL);
	}
	| ImportRep ',' import
	{
		$$ = Trees_NewNode(TREES_NOSYM, $3, $1);
	}
	;

import:
	IDENT BecomesIdentOpt
	{
		static Maps_Map importedModules = NULL;
		static char symbolFileName[PATH_MAX + 1];
		const char *module, *qualifier;
		Trees_Node qualifierSym, moduleIdent;
		char *moduleDirPath;
		
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
		if (strcmp(module, Args_ModuleName()) != 0) {
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
					
					if (Args_Mode() == ARGS_IMPORT_LIST_MODE) {
						$$ = Trees_NewIdent(module);
					} else {
						moduleDirPath = ModuleDirPath(module);

						/*import identifiers into the symbol table*/
						sprintf(symbolFileName, "%s/.obnc/%s.sym", moduleDirPath, module);
						if (Files_Exists(symbolFileName)) {
							Table_Import(symbolFileName, module, qualifier);
						} else {
							yyerror("Symbol file not found for module %s", module);
							YYABORT;
						}
				
						moduleIdent = Trees_NewIdent(module);
						Trees_SetKind(TREES_QUALIFIER_KIND, moduleIdent);
						$$ = Trees_NewNode(TREES_NOSYM, moduleIdent, Trees_NewString(moduleDirPath));
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
					yyerror("error: unexpected element type of actual parameter %s", AssignmentErrorContext(paramPos));
					exit(EXIT_FAILURE);
				}
			} else if (Types_IsString(expType) && Types_IsChar(formalElementType)) {
				/*done*/
			} else {
				if (Types_IsChar(formalElementType)) {
					yyerror("error: character array or string parameter expected %s", AssignmentErrorContext(paramPos));
				} else {
					yyerror("error: array parameter expected %s", AssignmentErrorContext(paramPos));
				}
				exit(EXIT_FAILURE);
			}
		} else if ((kind == TREES_VAR_PARAM_KIND) && Types_IsPointer(formalType)) {
			if (! Types_Same(expType, formalType)) {
				yyerror("error: parameter of type %s expected %s", Trees_Name(formalType), AssignmentErrorContext(paramPos));
				exit(EXIT_FAILURE);
			}
		} else if ((kind == TREES_VAR_PARAM_KIND) && (Trees_Symbol(expType) == RECORD)) {
			assert(Types_RecordBaseType(expType) != NULL);
			yyerror("error: variable parameter requires actual parameter of named record type %s: %s", AssignmentErrorContext(paramPos), Trees_Name(formalType));
			exit(EXIT_FAILURE);
		} else {
			ValidateAssignment(exp, formalType, paramPos);
		}
	} else {
		yyerror("error: writable parameter expected %s", AssignmentErrorContext(paramPos));
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
								if (IsInteger(param[1])) {
									Range_CheckSetElement(Trees_Integer(param[1]));
								} else if (! Types_IsInteger(paramTypes[1])) {
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
