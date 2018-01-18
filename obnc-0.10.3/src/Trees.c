/*Copyright (C) 2017, 2018 Karl Landstrom <karl@miasap.se>

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

#include "Trees.h"
#include "Util.h"
#include "../lib/obnc/OBNC.h"
#include "y.tab.h"
#include <assert.h>
#include <ctype.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*value types*/
enum {
	NO_VALUE,
	IDENT_VALUE,
	INTEGER_VALUE,
	REAL_VALUE,
	STRING_VALUE,
	CHAR_VALUE,
	SET_VALUE,
	VALUE_TYPE_COUNT
};

struct Trees_NodeDesc {
	int valueType;
	int symbol;
	int marked;
	union {
		struct {
			const char *name, *unaliasedName;
			int kind;
			unsigned int local:1, imported:1, exported:1, internal:1;
			Trees_Node value;
		} ident;
		OBNC_LONGI int integer;
		OBNC_LONGR double real;
		char *string;
		char ch;
		OBNC_LONGI unsigned int set;
	} value;
	Trees_Node type;
	Trees_Node left, right;
};

Trees_Node Trees_NewNode(int symbol, Trees_Node left, Trees_Node right)
{
	Trees_Node result;

	NEW(result);
	result->valueType = NO_VALUE;
	result->symbol = symbol;
	result->marked = 0;
	result->type = NULL;
	result->left = left;
	result->right = right;
	return result;
}


Trees_Node Trees_NewLeaf(int symbol)
{
	return Trees_NewNode(symbol, NULL, NULL);
}


int Trees_IsLeaf(Trees_Node node)
{
	assert(node != NULL);
	
	return (node->left == NULL) && (node->right == NULL);
}


void Trees_SetType(Trees_Node type, Trees_Node node)
{
	assert(node != NULL);
	
	node->type = type;
}


Trees_Node Trees_Type(Trees_Node node)
{
	assert(node != NULL);

	return node->type;
}


int Trees_Symbol(Trees_Node node)
{
	assert(node != NULL);

	return node->symbol;
}


void Trees_SetLeft(Trees_Node newLeft, Trees_Node tree)
{
	assert(tree != NULL);
	tree->left = newLeft;
}


void Trees_SetRight(Trees_Node newRight, Trees_Node tree)
{
	assert(tree != NULL);
	tree->right = newRight;
}


Trees_Node Trees_Left(Trees_Node tree)
{
	assert(tree != NULL);

	return tree->left;
}


Trees_Node Trees_Right(Trees_Node tree)
{
	assert(tree != NULL);

	return tree->right;
}


void Trees_ReverseList(Trees_Node *list)
{
	Trees_Node current, previous, next;

	current = *list;
	previous = NULL;
	while (current != NULL) {
		next = current->right; /*save next node*/
		current->right = previous; /*reverse pointer*/
		previous = current; /*save current node*/
		current = next; /*advance current*/
	}
	*list = previous;
}


static void PrintLabel(int symbol)
{
	if (symbol < 128) {
		putchar(symbol);
	} else {
		switch (symbol) {
			case ARRAY:
				printf("array");
				break;
			case BECOMES:
				printf(":=");
				break;
			case BY:
				printf("BY");
				break;
			case CASE:
				printf("CASE");
				break;
			case DO:
				printf("DO");
				break;
			case DOTDOT:
				printf("..");
				break;
			case DIV:
				printf("DIV");
				break;
			case FALSE:
				printf("FALSE");
				break;
			case FOR:
				printf("FOR");
				break;
			case GE:
				printf(">=");
				break;
			case ELSE:
				printf("ELSE");
				break;
			case ELSIF:
				printf("ELSIF");
				break;
			case IN:
				printf("IN");
				break;
			case IF:
				printf("IF");
				break;
			case IS:
				printf("IS");
				break;
			case LE:
				printf("<=");
				break;
			case MOD:
				printf("MOD");
				break;
			case MODULE:
				printf("MODULE");
				break;
			case NIL:
				printf("NIL");
				break;
			case OR:
				printf("OR");
				break;
			case POINTER:
				printf("POINTER");
				break;
			case RECORD:
				printf("RECORD");
				break;
			case REPEAT:
				printf("REPEAT");
				break;
			case THEN:
				printf("THEN");
				break;
			case TO:
				printf("TO");
				break;
			case TREES_NOSYM:
				printf("(none)");
				break;
			case TREES_ABS_PROC:
				printf("ABS");
				break;
			case TREES_ASR_PROC:
				printf("ASR");
				break;
			case TREES_ASSERT_PROC:
				printf("ASSERT");
				break;
			case TREES_BOOLEAN_TYPE:
				printf("BooleanType");
				break;
			case TREES_BYTE_TYPE:
				printf("ByteType");
				break;
			case TREES_CASE:
				printf("case");
				break;
			case TREES_CASE_LABEL_LIST:
				printf("CaseLabelList");
				break;
			case TREES_CASE_REP:
				printf("CaseRep");
				break;
			case TREES_CHAR_TYPE:
				printf("CharType");
				break;
			case TREES_CHR_PROC:
				printf("CHR");
				break;
			case TREES_DEC_PROC:
				printf("DEC");
				break;
			case TREES_DESIGNATOR:
				printf("designator");
				break;
			case TREES_EXCL_PROC:
				printf("EXCL");
				break;
			case TREES_EXP_LIST:
				printf("ExpList");
				break;
			case TREES_FIELD_LIST_SEQUENCE:
				printf("FieldListSequence");
				break;
			case TREES_FILE_POSITION:
				printf("FilePosition");
				break;
			case TREES_FLOOR_PROC:
				printf("FLOOR");
				break;
			case TREES_FLT_PROC:
				printf("FLT");
				break;
			case TREES_IDENT_LIST:
				printf("IdentList");
				break;
			case TREES_INC_PROC:
				printf("INC");
				break;
			case TREES_INCL_PROC:
				printf("INCL");
				break;
			case TREES_INTEGER_TYPE:
				printf("IntegerType");
				break;
			case TREES_LEN_PROC:
				printf("LEN");
				break;
			case TREES_LSL_PROC:
				printf("LSL");
				break;
			case TREES_NEW_PROC:
				printf("NEW");
				break;
			case TREES_NIL_TYPE:
				printf("NilType");
				break;
			case TREES_ODD_PROC:
				printf("ODD");
				break;
			case TREES_ORD_PROC:
				printf("ORD");
				break;
			case TREES_PACK_PROC:
				printf("PACK");
				break;
			case TREES_PROCEDURE_CALL:
				printf("ProcedureCall");
				break;
			case PROCEDURE:
				printf("Procedure");
				break;
			case TREES_RANGE_SET:
				printf("RangeSet");
				break;
			case TREES_REAL_TYPE:
				printf("RealType");
				break;
			case TREES_ROR_PROC:
				printf("ROR");
				break;
			case TREES_SET_TYPE:
				printf("SetType");
				break;
			case TREES_SINGLE_ELEMENT_SET:
				printf("SingleElementSet");
				break;
			case TREES_STATEMENT_SEQUENCE:
				printf("StatementSequence");
				break;
			case TREES_STRING_TYPE:
				printf("StringType");
				break;
			case TREES_UNPK_PROC:
				printf("UNPK");
				break;
			case TRUE:
				printf("TRUE");
				break;
			case WHILE:
				printf("WHILE");
				break;
			default:
				fprintf(stderr, "error: no label for symbol: %d\n", symbol);
				assert(0);
		}
	}
}


static void Indent(int level)
{
	int i;

	for (i = 0; i < level; i++) {
		putchar('\t');
	}
}

static void PrintRec(Trees_Node tree, int height)
{
	if (tree == NULL) {
		puts("(nil)");
	} else {
		Indent(height);
		switch (tree->valueType) {
			case NO_VALUE:
				PrintLabel(tree->symbol);
				putchar('\n');
				break;
			case IDENT_VALUE:
				printf("ident %s", tree->value.ident.name);
				if (tree->value.ident.unaliasedName != NULL
						&& (strcmp(tree->value.ident.unaliasedName, tree->value.ident.name) != 0)) {
					printf(" (%s)", tree->value.ident.unaliasedName);
				}
				/*printf(" (exp: %d, imp: %d)\n", tree->value.ident.exported, tree->value.ident.imported);*/
				putchar('\n');
				break;
			case INTEGER_VALUE:
				printf("%" OBNC_INT_MOD "d\n", tree->value.integer);
				break;
			case REAL_VALUE:
				printf("%.*" OBNC_REAL_MOD_W "g\n", DBL_DIG, tree->value.real);
				break;
			case STRING_VALUE:
				printf("\"%s\"\n", tree->value.string);
				break;
			case CHAR_VALUE:
				printf("'%c'\n", tree->value.ch);
				break;
			case SET_VALUE:
				printf("0x%" OBNC_INT_MOD "xu\n", tree->value.set);
				break;
			default:
				assert(0);
		}
		if ((tree->left != NULL) && (tree->right != NULL)) {
			PrintRec(tree->left, height + 1);
			PrintRec(tree->right, height + 1);
		} else if ((tree->left != NULL) && (tree->right == NULL)) {
			PrintRec(tree->left, height + 1);
			Indent(height + 1);
			puts("(nil)");
		} else if ((tree->left == NULL) && (tree->right != NULL)) {
			Indent(height + 1);
			puts("(nil)");
			PrintRec(tree->right, height + 1);
		}
	}
}


void Trees_Print(Trees_Node tree)
{
	PrintRec(tree, 0);
}


/*Identifiers*/

Trees_Node Trees_NewIdent(const char name[])
{
	Trees_Node result;

	assert(name != NULL);

	result = Trees_NewLeaf(IDENT);
	result->valueType = IDENT_VALUE;
	result->value.ident.name = name;
	result->value.ident.unaliasedName = name;
	result->value.ident.kind = TREES_UNSPECIFIED_KIND;
	result->value.ident.local = 0;
	result->value.ident.imported = 0;
	result->value.ident.exported = 0;
	result->value.ident.internal = 0;
	result->value.ident.value = NULL;
	return result;
}


const char *Trees_Name(Trees_Node node)
{
	assert(node != NULL);
	assert(node->valueType == IDENT_VALUE);
	
	return node->value.ident.name;
}


void Trees_SetName(const char name[], Trees_Node identNode)
{
	assert(name != NULL);
	assert(identNode != NULL);
	assert(identNode->valueType == IDENT_VALUE);

	identNode->value.ident.name = name;
}


const char *Trees_UnaliasedName(Trees_Node node)
{
	assert(node != NULL);
	assert(node->valueType == IDENT_VALUE);
	
	return node->value.ident.unaliasedName;
}


void Trees_SetUnaliasedName(const char name[], Trees_Node identNode)
{
	assert(name != NULL);
	assert(identNode != NULL);
	assert(identNode->valueType == IDENT_VALUE);

	identNode->value.ident.unaliasedName = name;
}


int Trees_Kind(Trees_Node identNode)
{
	assert(identNode != NULL);
	assert(identNode->valueType == IDENT_VALUE);
	
	return identNode->value.ident.kind;
}


void Trees_SetKind(int kind, Trees_Node identNode)
{
	assert(identNode != NULL);
	assert(identNode->valueType == IDENT_VALUE);
	assert(kind >= 0);
	assert(kind < TREES_KIND_COUNT);
	
	identNode->value.ident.kind = kind;
}


int Trees_Local(Trees_Node identNode)
{
	assert(identNode != NULL);
	assert(identNode->valueType == IDENT_VALUE);
	
	return identNode->value.ident.local;
}


void Trees_SetLocal(Trees_Node identNode)
{
	assert(identNode != NULL);
	assert(identNode->valueType == IDENT_VALUE);
	
	identNode->value.ident.local = 1;
}


int Trees_Imported(Trees_Node identNode)
{
	assert(identNode != NULL);
	assert(identNode->valueType == IDENT_VALUE);
	
	return identNode->value.ident.imported;
}


void Trees_SetImported(Trees_Node identNode)
{
	assert(identNode != NULL);
	assert(identNode->valueType == IDENT_VALUE);
	
	identNode->value.ident.imported = 1;
}


int Trees_Exported(Trees_Node identNode)
{
	assert(identNode != NULL);
	assert(identNode->valueType == IDENT_VALUE);
	
	return identNode->value.ident.exported;
}


void Trees_SetExported(Trees_Node identNode)
{
	assert(identNode != NULL);
	assert(identNode->valueType == IDENT_VALUE);
	
	identNode->value.ident.exported = 1;
}


int Trees_Internal(Trees_Node identNode)
{
	assert(identNode != NULL);
	assert(identNode->valueType == IDENT_VALUE);
	
	return identNode->value.ident.internal;
}


void Trees_SetInternal(Trees_Node identNode)
{
	assert(identNode != NULL);
	assert(identNode->valueType == IDENT_VALUE);
	
	identNode->value.ident.internal = 1;
}


Trees_Node Trees_Value(Trees_Node node)
{
	Trees_Node value;
	
	assert(node != NULL);
	assert(node->valueType == IDENT_VALUE);
	assert(node->value.ident.kind == TREES_CONSTANT_KIND);
	
	value = node->value.ident.value;
	if (Trees_Symbol(value) == STRING) {
		/*string constants are sometimes put in char context so we cannot reuse the same node*/
		value = Trees_NewString(Trees_String(value));
	}
	return value;
}


void Trees_SetValue(Trees_Node valueNode, Trees_Node constNode)
{
	assert(valueNode != NULL);
	assert((valueNode->valueType != NO_VALUE)
		|| (valueNode->symbol == NIL)
		|| (valueNode->symbol == TRUE)
		|| (valueNode->symbol == FALSE));
	assert(valueNode->valueType != IDENT_VALUE);
	assert(constNode != NULL);
	assert(constNode->valueType == IDENT_VALUE);
	assert(constNode->value.ident.kind == TREES_CONSTANT_KIND);
	
	constNode->value.ident.value = valueNode;
}


/*Integers*/

Trees_Node Trees_NewInteger(OBNC_LONGI int value)
{
	Trees_Node result;

	result = Trees_NewLeaf(INTEGER);
	result->valueType = INTEGER_VALUE;
	result->value.integer = value;
	result->type = Trees_NewLeaf(TREES_INTEGER_TYPE);
	return result;
}


OBNC_LONGI int Trees_Integer(Trees_Node integerNode)
{
	assert(integerNode != NULL);
	assert(integerNode->valueType == INTEGER_VALUE);

	return integerNode->value.integer;
}


/*Real numbers*/

Trees_Node Trees_NewReal(OBNC_LONGR double value)
{
	Trees_Node result;

	result = Trees_NewLeaf(REAL);
	result->valueType = REAL_VALUE;
	result->value.real = value;
	result->type = Trees_NewLeaf(TREES_REAL_TYPE);
	return result;
}


OBNC_LONGR double Trees_Real(Trees_Node realNode)
{
	assert(realNode != NULL);
	assert(realNode->valueType == REAL_VALUE);

	return realNode->value.real;
}


/*Strings*/

Trees_Node Trees_NewString(const char string[])
{
	Trees_Node result;

	assert(string != NULL);

	result = Trees_NewLeaf(STRING);
	result->valueType = STRING_VALUE;
	NEW_ARRAY(result->value.string, strlen(string) + 1);
	strcpy(result->value.string, string);
	result->type = Trees_NewNode(TREES_STRING_TYPE, Trees_NewInteger(strlen(string)), NULL);
	return result;
}


const char *Trees_String(Trees_Node stringNode)
{
	assert(stringNode != NULL);
	assert(stringNode->valueType == STRING_VALUE);

	return stringNode->value.string;
}


/*Characters*/

Trees_Node Trees_NewChar(char value)
{
	Trees_Node result;

	result = Trees_NewLeaf(TREES_CHAR_CONSTANT);
	result->valueType = CHAR_VALUE;
	result->value.ch = value;
	result->type = Trees_NewLeaf(TREES_CHAR_TYPE);
	return result;
}


char Trees_Char(Trees_Node charNode)
{
	assert(charNode != NULL);
	assert(charNode->valueType == CHAR_VALUE);

	return charNode->value.ch;
}


/*Set constants*/

Trees_Node Trees_NewSet(OBNC_LONGI unsigned int value)
{
	Trees_Node result;

	result = Trees_NewLeaf(TREES_SET_CONSTANT);
	result->valueType = SET_VALUE;
	result->value.set = value;
	result->type = Trees_NewLeaf(TREES_SET_TYPE);
	return result;
}


OBNC_LONGI unsigned int Trees_Set(Trees_Node setNode)
{
	assert(setNode != NULL);
	assert(setNode->valueType == SET_VALUE);

	return setNode->value.set;
}
