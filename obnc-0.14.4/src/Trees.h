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

#ifndef TREES_H
#define TREES_H

#include "../lib/obnc/OBNC.h"
#include <stdio.h>

/*node symbols*/
enum {
	TREES_NOSYM = 1000, /*larger than largest terminal symbol*/

	TREES_CHAR_CONSTANT,
	TREES_SET_CONSTANT,

	TREES_BOOLEAN_TYPE,
	TREES_CHAR_TYPE,
	TREES_STRING_TYPE,
	TREES_INTEGER_TYPE,
	TREES_REAL_TYPE,
	TREES_SET_TYPE,
	TREES_BYTE_TYPE,
	TREES_NIL_TYPE,

	TREES_ABS_PROC,
	TREES_ASR_PROC,
	TREES_ASSERT_PROC,
	TREES_CHR_PROC,
	TREES_DEC_PROC,
	TREES_EXCL_PROC,
	TREES_FLOOR_PROC,
	TREES_FLT_PROC,
	TREES_INC_PROC,
	TREES_INCL_PROC,
	TREES_LEN_PROC,
	TREES_LSL_PROC,
	TREES_NEW_PROC,
	TREES_ODD_PROC,
	TREES_ORD_PROC,
	TREES_PACK_PROC,
	TREES_ROR_PROC,
	TREES_UNPK_PROC,

	TREES_CASE,
	TREES_CASE_LABEL_LIST,
	TREES_CASE_REP,
	TREES_DESIGNATOR,
	TREES_EXP_LIST,
	TREES_FIELD_LIST_SEQUENCE,
	TREES_IDENT_LIST,
	TREES_PROCEDURE_CALL,
	TREES_RANGE_SET,
	TREES_SINGLE_ELEMENT_SET,
	TREES_STATEMENT_SEQUENCE,
	TREES_FILE_POSITION,

	TREES_ADR_PROC,
	TREES_SIZE_PROC,
	TREES_BIT_PROC,
	TREES_GET_PROC,
	TREES_PUT_PROC,
	TREES_COPY_PROC,
	TREES_VAL_PROC,

	TREES_SYMBOL_END
};

/*identifier kinds*/
enum {
	TREES_UNSPECIFIED_KIND,
	TREES_QUALIFIER_KIND,
	TREES_CONSTANT_KIND,
	TREES_TYPE_KIND,
	TREES_FIELD_KIND,
	TREES_VARIABLE_KIND,
	TREES_PROCEDURE_KIND,
	TREES_VALUE_PARAM_KIND,
	TREES_VAR_PARAM_KIND,
	TREES_KIND_COUNT
};

typedef struct Trees_NodeDesc *Trees_Node;

Trees_Node Trees_NewNode(int symbol, Trees_Node left, Trees_Node right);

Trees_Node Trees_NewLeaf(int symbol);

int Trees_IsLeaf(Trees_Node node);

void Trees_SetType(Trees_Node type, Trees_Node ident);

Trees_Node Trees_Type(Trees_Node node);

int Trees_Symbol(Trees_Node node);

void Trees_SetLeft(Trees_Node newLeft, Trees_Node tree);

void Trees_SetRight(Trees_Node newRight, Trees_Node tree);

Trees_Node Trees_Left(Trees_Node tree);

Trees_Node Trees_Right(Trees_Node tree);

void Trees_ReverseList(Trees_Node *list);

void Trees_Print(Trees_Node tree);


/*Identifiers*/

Trees_Node Trees_NewIdent(const char name[]);

const char *Trees_Name(Trees_Node ident);
void Trees_SetName(const char name[], Trees_Node ident);

const char *Trees_UnaliasedName(Trees_Node ident);
void Trees_SetUnaliasedName(const char name[], Trees_Node ident);

int Trees_Kind(Trees_Node ident);
void Trees_SetKind(int kind, Trees_Node ident);

int Trees_Local(Trees_Node ident);
void Trees_SetLocal(Trees_Node ident);

int Trees_Imported(Trees_Node ident);
void Trees_SetImported(Trees_Node ident);

int Trees_Exported(Trees_Node ident);
void Trees_SetExported(Trees_Node ident);

int Trees_Internal(Trees_Node ident);
void Trees_SetInternal(Trees_Node ident);

Trees_Node Trees_Value(Trees_Node constIdent);
void Trees_SetValue(Trees_Node value, Trees_Node constIdent);


/*Boolean*/

Trees_Node Trees_NewBoolean(int b);

int Trees_Boolean(Trees_Node boolNode);


/*Characters*/

Trees_Node Trees_NewChar(char ch);

char Trees_Char(Trees_Node charNode);


/*Strings*/

Trees_Node Trees_NewString(const char string[]);

const char *Trees_String(Trees_Node stringNode);


/*Integers*/

Trees_Node Trees_NewInteger(OBNC_LONGI int value);

OBNC_LONGI int Trees_Integer(Trees_Node integerNode);


/*Real numbers*/

Trees_Node Trees_NewReal(OBNC_LONGR double value);

OBNC_LONGR double Trees_Real(Trees_Node realNode);


/*Set constants*/

Trees_Node Trees_NewSet(OBNC_LONGI unsigned int value);

OBNC_LONGI unsigned int Trees_Set(Trees_Node setNode);

#endif
