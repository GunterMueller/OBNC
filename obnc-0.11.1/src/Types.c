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

#include "Types.h"
#include "Oberon.h"
#include "../lib/obnc/OBNC.h"
#include "y.tab.h"
#include <assert.h>
#include <string.h>

int Types_IsType(Trees_Node node)
{
	int result;

	assert(node != NULL);

	result = 0;
	switch (Trees_Symbol(node)) {
		case TREES_BOOLEAN_TYPE:
		case TREES_CHAR_TYPE:
		case TREES_INTEGER_TYPE:
		case TREES_REAL_TYPE:
		case TREES_BYTE_TYPE:
		case TREES_SET_TYPE:
		case TREES_STRING_TYPE:
		case TREES_ABS_PROC:
		case TREES_ASR_PROC:
		case TREES_ASSERT_PROC:
		case TREES_CHR_PROC:
		case TREES_DEC_PROC:
		case TREES_EXCL_PROC:
		case TREES_FLOOR_PROC:
		case TREES_FLT_PROC:
		case TREES_INC_PROC:
		case TREES_INCL_PROC:
		case TREES_LEN_PROC:
		case TREES_LSL_PROC:
		case TREES_NEW_PROC:
		case TREES_NIL_TYPE:
		case TREES_ODD_PROC:
		case TREES_ORD_PROC:
		case TREES_PACK_PROC:
		case TREES_ROR_PROC:
		case TREES_UNPK_PROC:
		case ARRAY:
		case RECORD:
		case POINTER:
		case PROCEDURE:
			result = 1;
			break;
		case IDENT:
			result = Trees_Kind(node) == TREES_TYPE_KIND;
			break;
	}
	return result;
}


int Types_IsBoolean(Trees_Node type)
{
	assert(Types_IsType(type));

	return Trees_Symbol(Types_Structure(type)) == TREES_BOOLEAN_TYPE;
}


int Types_IsChar(Trees_Node type)
{
	assert(Types_IsType(type));

	return Trees_Symbol(Types_Structure(type)) == TREES_CHAR_TYPE;
}


int Types_IsInteger(Trees_Node type)
{
	int sym;
	
	assert(Types_IsType(type));
	sym = Trees_Symbol(Types_Structure(type));
	return (sym == TREES_INTEGER_TYPE) || (sym == TREES_BYTE_TYPE);
}


int Types_IsReal(Trees_Node type)
{
	assert(Types_IsType(type));

	return Trees_Symbol(Types_Structure(type)) == TREES_REAL_TYPE;
}


int Types_IsByte(Trees_Node type)
{
	assert(Types_IsType(type));

	return Trees_Symbol(Types_Structure(type)) == TREES_BYTE_TYPE;
}


int Types_IsSet(Trees_Node type)
{
	assert(Types_IsType(type));

	return Trees_Symbol(Types_Structure(type)) == TREES_SET_TYPE;
}


int Types_IsString(Trees_Node type)
{
	assert(Types_IsType(type));
	
	return Trees_Symbol(type) == TREES_STRING_TYPE;
}


OBNC_LONGI int Types_StringLength(Trees_Node stringType)
{
	assert(Types_IsString(stringType));
	
	return Trees_Integer(Trees_Left(stringType));
}


int Types_Basic(Trees_Node type)
{
	int basic;
	
	assert(Types_IsType(type));

	basic = 0;
	switch (Trees_Symbol(type)) {	
		case TREES_BOOLEAN_TYPE:
		case TREES_CHAR_TYPE:
		case TREES_INTEGER_TYPE:
		case TREES_REAL_TYPE:
		case TREES_BYTE_TYPE:
		case TREES_SET_TYPE:
			basic = 1;
			break;
	}

	return basic;
}


int Types_Scalar(Trees_Node type)
{
	assert(Types_IsType(type));
	
	return ! Types_IsArray(type) && ! Types_IsRecord(type);
}


Trees_Node Types_Structure(Trees_Node type)
{
	Trees_Node result;

	assert(type != NULL);
	assert(Types_IsType(type));
	
	/*NOTE: the type of an unresolved type identifier may be NULL*/
	result = type;
	while ((result != NULL) && (Trees_Symbol(result) == IDENT)) {
		result = Trees_Type(result);
	}
	return result;
}


Trees_Node Types_UnaliasedIdent(Trees_Node type)
{
	Trees_Node result;
	
	assert(type != NULL);
	assert(Trees_Symbol(type) == IDENT);
	
	result = type;
	while (Trees_Symbol(Trees_Type(result)) == IDENT) {
		result = Trees_Type(result);
	}
	return result;
}


int Types_IsArray(Trees_Node type)
{
	assert(Types_IsType(type));
	
	return Trees_Symbol(Types_Structure(type)) == ARRAY;
}


Trees_Node Types_NewArray(Trees_Node length, Trees_Node elemType)
{
	assert((length == NULL) || Types_IsInteger(Trees_Type(length)));
	assert(Types_IsType(elemType));
	
	return Trees_NewNode(ARRAY, length, elemType);
}


int Types_IsOpenArray(Trees_Node type)
{
	assert(Types_IsType(type));
	
	return (Trees_Symbol(type) == ARRAY) && (Types_ArrayLength(type) == NULL);
}


int Types_IsCharacterArray(Trees_Node type)
{
	assert(Types_IsType(type));
	
	return Types_IsArray(type) && Types_IsChar(Types_ElementType(type));
}


Trees_Node Types_ArrayLength(Trees_Node arrayType)
{
	assert(Types_IsArray(arrayType));
	
	return Trees_Left(Types_Structure(arrayType));
}


Trees_Node Types_ElementType(Trees_Node arrayType)
{
	assert(Types_IsArray(arrayType));
	
	return Trees_Right(Types_Structure(arrayType));
}


void Types_SetElementType(Trees_Node elemType, Trees_Node arrayType)
{
	assert(Types_IsType(elemType));
	assert(Types_IsArray(arrayType));
	
	Trees_SetRight(elemType, Types_Structure(arrayType));
}


Trees_Node Types_NewRecord(Trees_Node recBaseType, Trees_Node fields)
{
	assert((recBaseType == NULL) || (Trees_Type(recBaseType) == NULL) 
		|| Types_IsRecord(recBaseType) || Types_IsPointer(recBaseType));
	assert((fields == NULL) || (Trees_Symbol(fields) == TREES_FIELD_LIST_SEQUENCE));
	
	return Trees_NewNode(RECORD, recBaseType, fields);
}


int Types_IsRecord(Trees_Node type)
{
	return Trees_Symbol(Types_Structure(type)) == RECORD;
}


Trees_Node Types_RecordBaseType(Trees_Node type)
{
	Trees_Node typeStruct, result, record;
	
	typeStruct = Types_Structure(type);
	result = NULL;
	switch (Trees_Symbol(typeStruct)) {
		case RECORD:
			result = Trees_Left(typeStruct);
			break;
		case POINTER:
			record = Types_PointerBaseType(typeStruct);
			result = Trees_Left(Types_Structure(record));
			break;
		default:
			assert(0);
	}
	return result;
}


void Types_SetRecordBaseType(Trees_Node recBaseType, Trees_Node recordType)
{
	assert(Types_IsRecord(recBaseType));
	assert(Types_IsRecord(recordType));
	
	Trees_SetLeft(recBaseType, Types_Structure(recordType));
}


Trees_Node Types_Fields(Trees_Node record)
{
	Trees_Node typeStruct;
	
	typeStruct = Types_Structure(record);
	assert(Trees_Symbol(typeStruct) == RECORD);
	return Trees_Right(typeStruct);
}


int Types_Extensible(Trees_Node type)
{
	int sym;
	
	sym = Trees_Symbol(Types_Structure(type));
	return (sym == RECORD) || (sym == POINTER);
}


int Types_Extends(Trees_Node baseType, Trees_Node extendedType)
{
	int result;
	Trees_Node baseTypeRecord, intermediateType;
	
	assert(baseType != NULL);
	assert(extendedType != NULL);

	if (Types_Same(Types_Descriptor(baseType), Types_Descriptor(extendedType))) {
		result = 1;
	} else {
		baseTypeRecord = Types_Descriptor(baseType);		
		intermediateType = Types_RecordBaseType(extendedType);	
		while ((intermediateType != NULL)
				&& ! Types_Same(Types_Descriptor(intermediateType), baseTypeRecord)) {
			intermediateType = Types_RecordBaseType(intermediateType);
		}
		
		result = intermediateType != NULL;
	}
	return result;
}


int Types_ExtensionLevel(Trees_Node type)
{
	int n = -1;
	
	do {
		n++;
		type = Types_RecordBaseType(type);
	} while (type != NULL);
	return n;
}


Trees_Node Types_Descriptor(Trees_Node type)
{
	Trees_Node result, typeStruct;
	
	assert(type != NULL);
	
	result = NULL;
	typeStruct = Types_Structure(type);
	switch (Trees_Symbol(typeStruct)) {
		case RECORD:
			result = type;
			break;
		case POINTER:
			result = Types_PointerBaseType(typeStruct);
			break;
		default:
			assert(0);
	}
	assert(result != NULL);
	return result;
}


void Types_GetFieldIdent(const char fieldName[], Trees_Node type, int varImported, Trees_Node *fieldIdent, Trees_Node *fieldBaseType)
{
	Trees_Node baseType, baseTypeDesc, fieldListSeq, identList, ident;
	int imported;
	
	assert(Types_IsRecord(type) || Types_IsPointer(type));

	*fieldIdent = NULL;
	*fieldBaseType = NULL;
	baseType = type;
	imported = varImported;
	do {
		if (! imported && (Trees_Symbol(baseType) == IDENT) && Trees_Imported(baseType)) {
			imported = 1;
		}
		baseTypeDesc = Types_Descriptor(baseType);
		/*search current base type for field name*/
		fieldListSeq = Types_Fields(baseTypeDesc);
		while ((fieldListSeq != NULL) && (*fieldIdent == NULL)) {
			identList = Trees_Left(fieldListSeq);
			do {
				ident = Trees_Left(identList);
				if ((! imported || Trees_Exported(ident)) && (strcmp(Trees_Name(ident), fieldName) == 0)) {
					*fieldIdent = ident;
					*fieldBaseType = baseType;
				}
				identList = Trees_Right(identList);
			} while (identList != NULL);
			fieldListSeq = Trees_Right(fieldListSeq);
		}
		
		baseType = Types_RecordBaseType(baseTypeDesc);
	} while ((baseType != NULL) && (*fieldIdent == NULL));

	assert((*fieldIdent == NULL) || (*fieldBaseType != NULL));
}


Trees_Node Types_NewPointer(Trees_Node ptrBaseType)
{
	assert((ptrBaseType == NULL) || (Trees_Type(ptrBaseType) == NULL) || Types_IsRecord(ptrBaseType));
	
	return Trees_NewNode(POINTER, ptrBaseType, NULL);
}


int Types_IsPointer(Trees_Node type)
{
	return Trees_Symbol(Types_Structure(type)) == POINTER;
}


Trees_Node Types_PointerBaseType(Trees_Node ptrType)
{
	assert(Types_IsPointer(ptrType));
	
	return Trees_Left(Types_Structure(ptrType));
}


void Types_SetPointerBaseType(Trees_Node ptrBaseType, Trees_Node ptrType)
{
	assert(Types_IsRecord(ptrBaseType));
	assert(Types_IsPointer(ptrType));
	
	Trees_SetLeft(ptrBaseType, Types_Structure(ptrType));
}


Trees_Node Types_NewProcedure(Trees_Node fields, Trees_Node resultType)
{
	assert((fields == NULL) || (Trees_Symbol(fields) == TREES_IDENT_LIST));
	assert((resultType == NULL) || Types_IsType(resultType));
	
	return Trees_NewNode(PROCEDURE, fields, resultType);
}


int Types_IsProcedure(Trees_Node type)
{
	Trees_Node typeStruct;
	
	assert(Types_IsType(type));
	
	typeStruct = Types_Structure(type);
	return (Trees_Symbol(typeStruct) == PROCEDURE) || Types_IsPredeclaredProcedure(typeStruct);
}


int Types_IsPredeclaredProcedure(Trees_Node type)
{
	int predeclared;
	
	assert(Types_IsType(type));

	predeclared = 0;
	switch (Trees_Symbol(Types_Structure(type))) {
		case TREES_ABS_PROC:
		case TREES_ASR_PROC:
		case TREES_ASSERT_PROC:
		case TREES_CHR_PROC:
		case TREES_DEC_PROC:
		case TREES_EXCL_PROC:
		case TREES_FLOOR_PROC:
		case TREES_FLT_PROC:
		case TREES_INC_PROC:
		case TREES_INCL_PROC:
		case TREES_LEN_PROC:
		case TREES_LSL_PROC:
		case TREES_NEW_PROC:
		case TREES_ODD_PROC:
		case TREES_ORD_PROC:
		case TREES_PACK_PROC:
		case TREES_ROR_PROC:
		case TREES_UNPK_PROC:
			predeclared = 1;
			break;
	}
	return predeclared;
}


Trees_Node Types_Parameters(Trees_Node procType)
{
	assert(Types_IsProcedure(procType));
	
	return Trees_Left(Types_Structure(procType));
}


Trees_Node Types_ResultType(Trees_Node procType)
{
	assert(Types_IsProcedure(procType));
	
	return Trees_Right(Types_Structure(procType));
}


void Types_SetResultType(Trees_Node resultType, Trees_Node procType)
{
	assert((resultType == NULL) || Types_IsType(resultType));
	assert(Types_IsProcedure(procType));
	
	Trees_SetRight(resultType, Types_Structure(procType));	
}


int Types_Same(Trees_Node typeA, Trees_Node typeB)
{
	assert(Types_IsType(typeA));
	assert(Types_IsType(typeB));

	return (Types_Structure(typeA) == Types_Structure(typeB))
		|| (Types_Basic(typeA) && Types_Basic(typeB) && (Trees_Symbol(typeA) == Trees_Symbol(typeB)))
		|| ((Trees_Symbol(typeA) == IDENT) && (Trees_Symbol(typeB) == IDENT)
			&& (strcmp(Trees_UnaliasedName(typeA), Trees_UnaliasedName(typeB)) == 0));
}


static int FormalParametersMatch(Trees_Node procTypeA, Trees_Node procTypeB);

static int TypesEqual(Trees_Node typeA, Trees_Node typeB)
{
	return Types_Same(typeA, typeB)
		|| (Types_IsOpenArray(typeA) && Types_IsOpenArray(typeB)
			&& TypesEqual(Types_ElementType(typeA), Types_ElementType(typeB)))
		|| (Types_IsProcedure(typeA) && Types_IsProcedure(typeB)
			&& FormalParametersMatch(typeA, typeB));
}


int Types_AssignmentCompatible(Trees_Node sourceExp, Trees_Node targetType)
{
	int result;
	Trees_Node sourceType;

	assert(sourceExp != NULL);
	assert(Types_IsType(targetType));
	
	result = 0;
	sourceType = Trees_Type(sourceExp);
	if ((sourceType != NULL) && (Types_Same(sourceType, targetType))) {
		result = ! Types_IsString(targetType);
	} else {
		switch (Trees_Symbol(Types_Structure(targetType))) {
			case TREES_CHAR_TYPE:
				result = Types_IsString(sourceType) && (Types_StringLength(sourceType) <= 1);
				break;
			case TREES_INTEGER_TYPE:
			case TREES_BYTE_TYPE:
				result = Types_IsInteger(sourceType);
				break;
			case ARRAY:
				if (Types_IsString(sourceType)) {
					result = Types_IsCharacterArray(targetType)
						&& (Types_IsOpenArray(targetType)
							|| (Types_StringLength(sourceType) < Trees_Integer(Types_ArrayLength(targetType))));
				} else if (Types_IsOpenArray(sourceType)) {
					result = ! Types_IsOpenArray(targetType)
						&& TypesEqual(Types_ElementType(sourceType), Types_ElementType(targetType));
				}								
				break;
			case RECORD:
				result = Types_IsRecord(sourceType) && Types_Extends(targetType, sourceType);
				break;
			case POINTER:
				result = (Trees_Symbol(sourceExp) == NIL)
					|| (Types_IsPointer(sourceType) && Types_Extends(targetType, sourceType));
				break;
			case PROCEDURE:
				result = (Trees_Symbol(sourceExp) == NIL)
					|| ((Trees_Symbol(sourceExp) == TREES_DESIGNATOR)
						&& (Trees_Kind(Trees_Left(sourceExp)) == TREES_PROCEDURE_KIND)
						&& ! Types_IsPredeclaredProcedure(sourceType)
						&& FormalParametersMatch(sourceType, targetType));
				break;
		}
	}
	return result;
}


int Types_ArrayCompatible(Trees_Node actualType, Trees_Node formalType)
{
	return Types_Same(formalType, actualType)
		|| (Types_IsOpenArray(formalType) && Types_IsArray(actualType)
			&& Types_ArrayCompatible(Types_ElementType(actualType), Types_ElementType(formalType)))
		|| (Types_IsCharacterArray(formalType) && Types_IsString(actualType));
}


int Types_ExpressionCompatible(int operator, Trees_Node firstType, Trees_Node secondType)
{
	int firstTypeSym, result;
	
	firstTypeSym = Trees_Symbol(Types_Structure(firstType));
	result = 0;
	switch (operator) {
		case '+':
		case '-':
			switch (firstTypeSym) {
				case TREES_INTEGER_TYPE:
				case TREES_BYTE_TYPE:
					result = (secondType == NULL) || Types_IsInteger(secondType);
					break;
				case TREES_REAL_TYPE:
				case TREES_SET_TYPE:
					result = (secondType == NULL) || Types_Same(firstType, secondType);
					break;
			}			
			break;
		case '*':
			switch (firstTypeSym) {
				case TREES_INTEGER_TYPE:
				case TREES_BYTE_TYPE:
					result = Types_IsInteger(secondType);
					break;
				case TREES_REAL_TYPE:
				case TREES_SET_TYPE:
					result = Types_Same(firstType, secondType);
					break;
			}
			break;
		case '/':
			switch (firstTypeSym) {
				case TREES_REAL_TYPE:
				case TREES_SET_TYPE:
					result = Types_Same(firstType, secondType);
					break;
			}
			break;
		case DIV:
		case MOD:
			result = Types_IsInteger(firstType) && Types_IsInteger(secondType);
			break;
		case OR:
		case '&':
			result = (firstTypeSym == TREES_BOOLEAN_TYPE) && Types_Same(firstType, secondType);
			break;
		case '~':
			result = firstTypeSym == TREES_BOOLEAN_TYPE;
			break;
		case '=':
		case '#':
			switch (firstTypeSym) {
				case TREES_BOOLEAN_TYPE:
				case TREES_REAL_TYPE:
				case TREES_SET_TYPE:
					result = Types_Same(firstType, secondType);
					break;
				case TREES_INTEGER_TYPE:
				case TREES_BYTE_TYPE:
					result = Types_IsInteger(secondType);
					break;
				case TREES_CHAR_TYPE:
					result = Types_IsChar(secondType)
						|| (Types_IsString(secondType) && (Types_StringLength(secondType) <= 1));
					break;
				case ARRAY:
					result = Types_IsCharacterArray(firstType)
						&& (Types_IsCharacterArray(secondType) || Types_IsString(secondType));
					break;
				case TREES_STRING_TYPE:
					result = Types_IsCharacterArray(secondType) || Types_IsString(secondType)
						|| (Types_IsChar(secondType) && (Types_StringLength(firstType) <= 1));
					break;
				case POINTER:
					result = (Trees_Symbol(secondType) == TREES_NIL_TYPE)
						|| (Types_IsPointer(secondType)
							&& (Types_Extends(firstType, secondType) || Types_Extends(secondType, firstType)));
					break;
				case PROCEDURE:
					result = (Trees_Symbol(secondType) == TREES_NIL_TYPE)
						|| Types_Same(firstType, secondType);
					break;
				case TREES_NIL_TYPE:
					result = Types_IsPointer(secondType) || Types_IsProcedure(secondType);
					break;
			}			
			break;
		case '<':
		case LE:
		case '>':
		case GE:
			switch (firstTypeSym) {
				case TREES_INTEGER_TYPE:
				case TREES_BYTE_TYPE:
					result = Types_IsInteger(secondType);
					break;
				case TREES_CHAR_TYPE:
					result = Types_IsChar(secondType)
						|| (Types_IsString(secondType) && (Types_StringLength(secondType) <= 1));
					break;
				case TREES_REAL_TYPE:
					result = Types_Same(firstType, secondType);
					break;
				case ARRAY:
					result = Types_IsCharacterArray(firstType)
						&& (Types_IsCharacterArray(secondType) || Types_IsString(secondType));
					break;
				case TREES_STRING_TYPE:
					result = Types_IsCharacterArray(secondType) || Types_IsString(secondType)
						|| (Types_IsChar(secondType) && (Types_StringLength(firstType) <= 1));
					break;
			}
			break;
		case IN:
			result = Types_IsInteger(firstType) && Types_IsSet(secondType);
			break;
		case IS:
			result = (Types_IsRecord(firstType) || Types_IsPointer(firstType))
				&& (Types_IsRecord(secondType) || Types_IsPointer(secondType))
				&& Types_Extends(firstType, secondType);
			break;
	}
	return result;
}


static int FormalParametersMatch(Trees_Node procTypeA, Trees_Node procTypeB)
{
	Trees_Node resultTypeA, resultTypeB, paramListA, paramListB, paramA, paramB;
	int match;
	
	assert(Types_IsProcedure(procTypeA));
	assert(Types_IsProcedure(procTypeB));
	
	resultTypeA = Types_ResultType(procTypeA);
	resultTypeB = Types_ResultType(procTypeB);
	
	match = 0;
	if (((resultTypeA == NULL) && (resultTypeB == NULL))
			|| ((resultTypeA != NULL) && (resultTypeB != NULL)
				&& Types_Same(resultTypeA, resultTypeB))) {
		match = 1;
		paramListA = Types_Parameters(procTypeA);
		paramListB = Types_Parameters(procTypeB);
		while (match && (paramListA != NULL) && (paramListB != NULL)) {
			paramA = Trees_Left(paramListA);
			paramB = Trees_Left(paramListB);
			match = match && TypesEqual(Trees_Type(paramA), Trees_Type(paramB))
				&& (Trees_Kind(paramA) == Trees_Kind(paramB));
			paramListA = Trees_Right(paramListA);
			paramListB = Trees_Right(paramListB);
		}
		match = match && (paramListA == NULL) && (paramListB == NULL);
	}
	return match;
}
