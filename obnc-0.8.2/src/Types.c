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

#include "Types.h"
#include "y.tab.h"
#include "yyerror.h"
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


int Types_StringLength(Trees_Node stringType)
{
	assert(Types_IsString(stringType));
	
	return Trees_Integer(Trees_Left(stringType));
}


int Types_Basic(Trees_Node type)
{
	int basic;
	
	assert(Types_IsType(type));

	basic = 0;
	switch (Trees_Symbol(Types_Structure(type))) {	
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


Trees_Node Types_Descriptor(Trees_Node type)
{
	Trees_Node result, typeStruct;
	
	result = NULL;
	typeStruct = Types_Structure(type);
	switch (Trees_Symbol(typeStruct)) {
		case RECORD:
			result = typeStruct;
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
		fieldListSeq = Types_Fields(Types_Structure(baseTypeDesc));
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


static int ArrayLength(Trees_Node type)
{
	Trees_Node length;
	int result;
	
	length = Types_ArrayLength(type);
	if (length != NULL) {
		result = Trees_Integer(length);
	} else {
		result = -1;
	}
	return result;
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
				result = Types_IsByte(sourceType);
				break;
			case TREES_BYTE_TYPE:
				result = Types_IsInteger(sourceType);
				break;
			case ARRAY:
				if (Types_IsString(sourceType)) {
					result = Types_IsCharacterArray(targetType)
						&& (Types_IsOpenArray(targetType)
							|| (Types_StringLength(sourceType) < ArrayLength(targetType)));
				} else if (Types_IsOpenArray(sourceType) || Types_IsOpenArray(targetType)) {
					result = Types_Same(Types_ElementType(sourceType), Types_ElementType(targetType));
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
				result = Trees_Symbol(sourceExp) == NIL;
				break;
		}
	}
	return result;
}


int Types_ExpressionCompatible(Trees_Node leftType, Trees_Node rightType)
{
	int result;
	
	assert(leftType != NULL);
	assert(rightType != NULL);

	result = 0;
	if (Types_Same(leftType, rightType)) {
		result = 1;
	} else {
		switch (Trees_Symbol(Types_Structure(leftType))) {
			case TREES_CHAR_TYPE:
				result = Types_IsString(rightType) && (Types_StringLength(rightType) <= 1);
				break;
			case TREES_INTEGER_TYPE:
				result = Types_IsByte(rightType);
				break;
			case TREES_BYTE_TYPE:
				result = Types_IsInteger(rightType);
				break;
			case TREES_STRING_TYPE:
				result = Types_IsString(rightType)
					|| (Types_IsChar(rightType) && (Types_StringLength(leftType) <= 1))
					|| Types_IsCharacterArray(rightType);
				break;
			case ARRAY:
				result = Types_IsCharacterArray(leftType)
					&& (Types_IsCharacterArray(rightType) || Types_IsString(rightType));
				break;
		}
	}
	return result;
}


static int FormalParametersMatch(Trees_Node paramA, Trees_Node paramB)
{
	int kindA, kindB;
	Trees_Node typeA, typeB;
	
	assert(paramA != NULL);
	assert(paramB != NULL);

	kindA = Trees_Kind(paramA);
	kindB = Trees_Kind(paramB);
	typeA = Trees_Type(paramA);
	typeB = Trees_Type(paramB);

	return (kindA == kindB) && Types_Same(typeA, typeB);
}


static int FormalParameterListsMatch(Trees_Node fpListA, Trees_Node fpListB)
{
	Trees_Node currListA, currListB;
	
	assert(fpListA != NULL);
	assert(fpListB != NULL);

	currListA = fpListA;
	currListB = fpListB;
	while ((currListA != NULL) && (currListB != NULL)
			&& FormalParametersMatch(Trees_Left(currListA), Trees_Left(currListB))) {
		currListA = Trees_Right(currListA);
		currListB = Trees_Right(currListB);
	}
	return (currListA == NULL) && (currListB == NULL);
}


static int SignaturesMatch(Trees_Node procTypeA, Trees_Node procTypeB)
{
	Trees_Node fpListA, fpListB, resultTypeA, resultTypeB;
	int match;
	
	assert(procTypeA != NULL);
	assert(procTypeB != NULL);
	
	fpListA = Types_Parameters(procTypeA);
	fpListB = Types_Parameters(procTypeB);
	if (	((fpListA == NULL) && (fpListB == NULL))
			|| ((fpListA != NULL) && (fpListB != NULL) 
				&& FormalParameterListsMatch(fpListA, fpListB))) {
		resultTypeA = Types_ResultType(procTypeA);
		resultTypeB = Types_ResultType(procTypeB);
		match = ((resultTypeA == NULL) && (resultTypeB == NULL))
				|| ((resultTypeA != NULL) && (resultTypeB != NULL)
					&& Types_Same(resultTypeA, resultTypeB));
	} else {
		match = 0;
	}
	return match;
}


int Types_Same(Trees_Node type1, Trees_Node type2)
{
	Trees_Node typeStruct1, typeStruct2;
	const char *unaliasedName1, *unaliasedName2;
	int result;
	
	assert(Types_IsType(type1));
	assert(Types_IsType(type2));
	result = 0;
	if ((Trees_Symbol(type1) == IDENT) && (Trees_Symbol(type2) == IDENT)) {
		unaliasedName1 = Trees_UnaliasedName(Types_UnaliasedIdent(type1));
		unaliasedName2 = Trees_UnaliasedName(Types_UnaliasedIdent(type2));
		result = strcmp(unaliasedName1, unaliasedName2) == 0;
	} else {
		typeStruct1 = Types_Structure(type1);
		typeStruct2 = Types_Structure(type2);
		if (typeStruct1 == typeStruct2) {
			result = ! Types_IsOpenArray(type1);
		} else {
			if (Trees_Symbol(typeStruct1) == Trees_Symbol(typeStruct2)) {
				switch (Trees_Symbol(typeStruct1)) {
					case TREES_BOOLEAN_TYPE:
					case TREES_CHAR_TYPE:
					case TREES_INTEGER_TYPE:
					case TREES_REAL_TYPE:
					case TREES_BYTE_TYPE:
					case TREES_SET_TYPE:
						result = 1;
						break;
					case ARRAY:
						result = Types_Same(Types_ElementType(typeStruct1), Types_ElementType(typeStruct1))
							&& (ArrayLength(typeStruct1) == ArrayLength(typeStruct2));
						break;	
					case PROCEDURE:
						result = SignaturesMatch(typeStruct1, typeStruct2);
						break;
				}
			}
		}
	}
	return result;
}


int Types_Extends(Trees_Node baseType, Trees_Node extendedType)
{
	int result;
	Trees_Node baseTypeRecord, intermediateType;
	
	assert(baseType != NULL);
	assert(extendedType != NULL);
	
	if (Types_Same(baseType, extendedType)) {
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
