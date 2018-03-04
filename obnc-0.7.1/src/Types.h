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

#ifndef TYPES_H
#define TYPES_H

#include "Trees.h"

int Types_IsType(Trees_Node node);

int Types_IsBoolean(Trees_Node type);

int Types_IsChar(Trees_Node type);

int Types_IsInteger(Trees_Node type);

int Types_IsReal(Trees_Node type);

int Types_IsByte(Trees_Node type);

int Types_IsSet(Trees_Node type);

int Types_IsString(Trees_Node type);

int Types_StringLength(Trees_Node stringType);

int Types_Basic(Trees_Node type);

int Types_Scalar(Trees_Node type);

int Types_IsPredeclaredProcedure(Trees_Node type);

Trees_Node Types_Structure(Trees_Node type);

Trees_Node Types_UnaliasedIdent(Trees_Node type);

/*arrays*/

Trees_Node Types_NewArray(Trees_Node length, Trees_Node elemType);

int Types_IsArray(Trees_Node type);

int Types_IsOpenArray(Trees_Node type);

int Types_IsCharacterArray(Trees_Node type);

Trees_Node Types_ArrayLength(Trees_Node arrayType);

Trees_Node Types_ElementType(Trees_Node arrayType);

void Types_SetElementType(Trees_Node elemType, Trees_Node arrayType);

/*records*/

Trees_Node Types_NewRecord(Trees_Node recBaseType, Trees_Node fields);

int Types_IsRecord(Trees_Node type);

Trees_Node Types_RecordBaseType(Trees_Node type);

void Types_SetRecordBaseType(Trees_Node recBaseType, Trees_Node recordType);

Trees_Node Types_Fields(Trees_Node record);

Trees_Node Types_Descriptor(Trees_Node recordOrPointerType);

int Types_Extensible(Trees_Node type);

int Types_Extends(Trees_Node baseType, Trees_Node extendedType);

int Types_ExtensionLevel(Trees_Node type);

void Types_GetFieldIdent(const char fieldName[], Trees_Node type, int varImported, Trees_Node *ident, Trees_Node *baseType);

/*pointers*/

Trees_Node Types_NewPointer(Trees_Node ptrBaseType);

int Types_IsPointer(Trees_Node type);

Trees_Node Types_PointerBaseType(Trees_Node ptrType);

void Types_SetPointerBaseType(Trees_Node ptrBaseType, Trees_Node ptrType);

/*procedures*/

Trees_Node Types_NewProcedure(Trees_Node fields, Trees_Node resultType);

int Types_IsProcedure(Trees_Node type);

Trees_Node Types_Parameters(Trees_Node procType);

Trees_Node Types_ResultType(Trees_Node procType);

void Types_SetResultType(Trees_Node resultType, Trees_Node procType);

/*type compatibility*/

int Types_AssignmentCompatible(Trees_Node sourceExp, Trees_Node targetType);

int Types_ExpressionCompatible(Trees_Node leftType, Trees_Node rightType);

int Types_Same(Trees_Node type1, Trees_Node type2);

#endif
