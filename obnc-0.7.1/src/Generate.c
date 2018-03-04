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

#include "Generate.h"
#include "Args.h"
#include "Files.h"
#include "Maps.h"
#include "Types.h"
#include "Util.h"
#include "Version.h"
#include "y.tab.h"
#include <unistd.h> /*POSIX*/
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *headerComment;
static char tempCFilepath[PATH_MAX];
static char tempHFilepath[PATH_MAX];
static FILE *cFile;
static FILE *hFile; /*NULL for entry point modules*/

static Trees_Node importList;

static Trees_Node declaredTypeIdent;

static Trees_Node caseVariable;
static Trees_Node caseLabelType;

static long int procedureDeclStart;
static struct ProcedureDeclNode {
	Trees_Node procIdent;
	Maps_Map localProcedures;
	Trees_Node runtimeInitVars;
	char *partialDecl;
	struct ProcedureDeclNode *next;
} *procedureDeclStack;

static void Print(FILE *file, int indent, const char format[], ...)
{
	int i;
	va_list ap;

	assert(indent >= 0);

	if (file != NULL) {
		for (i = 0; i < indent; i++) {
			fputc('\t', file);
		}
		va_start(ap, format);
		vfprintf(file, format, ap);
		va_end(ap);
	}
}


static void Generate(Trees_Node tree, FILE *file, int indent);


/*IDENTIFIER GENERATORS*/

static int ModulePrefixNeeded(Trees_Node ident)
{
	int serving, imported, indirectlyImported, exported, global, isType, isField;

	serving = Args_Mode() != ARGS_ENTRY_POINT_MODE;
	imported = Trees_Imported(ident);
	indirectlyImported = ! imported && (strchr(Trees_Name(ident), '.') != NULL);
	exported = Trees_Exported(ident);
	global = ! Trees_Local(ident);
	isType = Types_IsType(ident);
	isField = Trees_Kind(ident) == TREES_FIELD_KIND;

	return serving && ! imported && ! indirectlyImported && ((exported && ! isField) || (global && isType));
}


static void GenerateLocalProcedurePrefix(Trees_Node ident, struct ProcedureDeclNode *node, FILE *file)
{
	if (node != NULL) {
		GenerateLocalProcedurePrefix(ident, node->next, file);
		Print(file, 0, "%s_", Trees_Name(node->procIdent));
	}
}


static void GenerateLocalProcedureIdent(Trees_Node ident, FILE *file, int indent)
{
	assert(procedureDeclStack != NULL);
	Print(file, indent, "");
	if (Maps_HasKey(Trees_Name(ident), procedureDeclStack->localProcedures)) {
		GenerateLocalProcedurePrefix(ident, procedureDeclStack, file);
	} else {
		GenerateLocalProcedurePrefix(ident, procedureDeclStack->next, file);
	}
	Print(file, 0, "%s_Local", Trees_Name(ident));
}


static void GenerateIdent(Trees_Node ident, FILE *file, int indent)
{
	const char *name;
	char *dotPtr;

	name = Trees_UnaliasedName(ident);
	if (Trees_Internal(ident)) {
		Print(file, indent, name);
	} else if (ModulePrefixNeeded(ident)) {
		Print(file, indent, "%s_%s_", Args_ModuleName(), name);
	} else if ((Trees_Kind(ident) == TREES_PROCEDURE_KIND) && Trees_Local(ident)) {
		GenerateLocalProcedureIdent(ident, file, indent);
	} else {
		dotPtr = strchr(name, '.');
		if (dotPtr != NULL) {
			*dotPtr = '_';
		}
		Print(file, indent, "%s_", name);
		if (dotPtr != NULL) {
			*dotPtr = '.';
		}
	}
}


/*LITERAL GENERATORS*/

static void GenerateReal(double value, FILE *file, int indent)
{
	char buffer[DBL_DIG + 8]; /*DBL_DIG + strlen("-") + strlen(".") + strlen("e+999") + 1*/
	int formattedAsInteger;

	sprintf(buffer, "%.*g", DBL_DIG, value);
	if (strcmp(buffer, "inf") == 0) {
		Print(file, indent, "(1.0 / 0.0)");
	} else if (strcmp(buffer, "-inf") == 0) {
		Print(file, indent, "(-1.0 / 0.0)");
	} else if (strcmp(buffer, "nan") == 0) {
		Print(file, indent, "(0.0 / 0.0)");
	} else if (strcmp(buffer, "-nan") == 0) {
		Print(file, indent, "(0.0 / 0.0)");
	} else {
		formattedAsInteger = (strchr(buffer, '.') == NULL) && (strchr(buffer, 'e') == 0);
		if (formattedAsInteger) {
			strcat(buffer, ".0");
		}
		Print(file, indent, buffer);
	}
}


static int CharCount(char ch, const char s[])
{
	int n;

	n = 0;
	s = strchr(s, ch);
	while (s != NULL) {
		n++;
		s = strchr(s + 1, ch);
	}
	return n;
}


static char *CString(const char s[]) /*returns s with escaped backslash characters*/
{
	int i, j;
	char *result;

	NEW_ARRAY(result, strlen(s) + CharCount('\\', s) + 1);
	i = 0;
	j = 0;
	while (s[i] != '\0') {
		result[j] = s[i];
		if (result[j] == '\\') {
			j++;
			result[j] = '\\';
		}
		i++;
		j++;
	}
	result[j] = '\0';
	return result;
}


static void GenerateString(const char string[], FILE *file, int indent)
{
	if (CharCount('\\', string) > 0) {
		Print(file, indent, "\"%s\"", CString(string));
	} else if (strcmp(string, "\"") == 0) { /*ordinal Oberon string 22X*/
		Print(file, indent, "\"\\\"\"");
	} else {
		Print(file, indent, "\"%s\"", string);
	}
}


static void GenerateChar(char ch, FILE *file, int indent)
{
	switch (ch) {
		case '\'':
		case '\\':
			Print(file, indent, "'\\%c'", ch);
			break;
		default:
			if (isprint(ch)) {
				Print(file, indent, "'%c'", ch);
			} else {
				Print(file, indent, "'\\x%x'", (unsigned char) ch);
			}
	}
}


/*CONSTANT DECLARATION GENERATORS*/

void Generate_ConstDeclaration(Trees_Node ident)
{
	if (Trees_Exported(ident)) {
		/*add constant declaration to header file to provide access to it from hand-written C file*/
		Print(hFile, 0, "\n#define ");
		Generate(ident, hFile, 0);
		Print(hFile, 0, " ");
		Generate(Trees_Value(ident), hFile, 0);
		Print(hFile, 0, "\n");
	}
}


/*TYPE DECLARATION GENERATORS*/

static void GenerateDeclaration(Trees_Node declaration, FILE *file, int indent);

static void GenerateFields(Trees_Node type, FILE *file, int indent)
{
	Trees_Node typeDesc, baseType, pointerBaseType, fieldListSeq, identList;

	assert(type != NULL);

	typeDesc = Types_Descriptor(type);
	fieldListSeq = Types_Fields(typeDesc);
	baseType = Types_RecordBaseType(typeDesc);
	if (baseType != NULL) {
		if (Types_IsPointer(baseType)) {
			pointerBaseType = Types_PointerBaseType(baseType);
			if (Trees_Symbol(pointerBaseType) == RECORD) {
				Print(file, indent, "struct ");
				Generate(baseType, file, 0);
			} else {
				assert(Trees_Symbol(pointerBaseType) == IDENT);
				Generate(pointerBaseType, file, indent);
			}
		} else {
			Generate(baseType, file, indent);
		}
		Print(file, 0, " base;\n");
	} else if (fieldListSeq == NULL) {
		Print(file, indent, "int dummy;\n");
	}
	while (fieldListSeq != NULL) {
		identList = Trees_Left(fieldListSeq);
		GenerateDeclaration(Trees_NewNode(TREES_NOSYM, identList, NULL), file, indent);
		fieldListSeq = Trees_Right(fieldListSeq);
	}
}


static void GenerateRecord(Trees_Node type, Trees_Node declIdent, FILE *file, int indent)
{
	Print(file, indent, "struct ");
	if ((declIdent != NULL) && (Trees_Kind(declIdent) == TREES_TYPE_KIND)) {
		Generate(declIdent, file, 0);
		Print(file, 0, " ");
	}
	Print(file, 0, "{\n");
	GenerateFields(type, file, indent + 1);
	Print(file, indent, "}");
}


static Trees_Node TypeDescIdent(Trees_Node type)
{
	Trees_Node result, initialIdent, unaliasedIdent, typeStruct, pointerBaseType;

	result = NULL;
	initialIdent = type;
	if (Trees_Symbol(type) == POINTER) {
		initialIdent = Trees_Left(type);
		assert(Trees_Symbol(initialIdent) == IDENT);
	}
	unaliasedIdent = Types_UnaliasedIdent(initialIdent);
	typeStruct = Types_Structure(unaliasedIdent);
	switch (Trees_Symbol(typeStruct)) {
		case RECORD:
			result = unaliasedIdent;
			break;
		case POINTER:
			pointerBaseType = Types_PointerBaseType(typeStruct);
			switch (Trees_Symbol(pointerBaseType)) {
				case RECORD:
					result = unaliasedIdent;
					break;
				case IDENT:
					result = Types_UnaliasedIdent(pointerBaseType);
					break;
				default:
					assert(0);
			}
			break;
		default:
			assert(0);
	}

	assert(result != NULL);

	return result;
}


static void GenerateStorageClassSpecifier(Trees_Node ident, FILE *file, int indent)
{
	if (Trees_Kind(ident) == TREES_TYPE_KIND) {
		Print(file, indent, "typedef ");
	} else if (! Trees_Local(ident)) {
		if (file == hFile) {
			Print(file, indent, "extern ");
		} else if (! Trees_Exported(ident)) {
			Print(file, indent, "static ");
		}
	}
}


static int TypeIncomplete(Trees_Node type, Trees_Node ident)
{
	return ((Trees_Kind(ident) == TREES_TYPE_KIND)
			&& ((Trees_Type(type) == NULL) || Types_IsRecord(type)))
		|| (type == declaredTypeIdent);
}


static void GenerateTypeSpecifier(Trees_Node ident, Trees_Node type, FILE *file, int indent)
{
	switch (Trees_Symbol(type)) {
		case IDENT:
			Print(file, indent, "");
			if (TypeIncomplete(type, ident)) {
				Print(file, 0, "struct ");
			}
			Generate(type, file, 0);
			break;
		case ARRAY:
			GenerateTypeSpecifier(ident, Types_ElementType(type), file, indent);
			break;
		case RECORD:
			GenerateRecord(type, ident, file, indent);
			break;
		case POINTER:
			GenerateTypeSpecifier(ident, Types_PointerBaseType(type), file, indent);
			break;
		case PROCEDURE:
			if (Types_ResultType(type) != NULL) {
				GenerateTypeSpecifier(ident, Types_ResultType(type), file, indent);
			} else {
				Print(file, indent, "void");
			}
			break;
		default:
			Generate(type, file, indent);
	}
}


static void GenerateFormalParameterList(Trees_Node paramList, FILE *file, int indent);

static void GenerateDeclarator(Trees_Node ident, FILE *file)
{
	Trees_Node type, firstNonArrayType, resultType;

	type = Trees_Type(ident);
	firstNonArrayType = type;
	while (Trees_Symbol(firstNonArrayType) == ARRAY) {
		firstNonArrayType = Types_ElementType(firstNonArrayType);
	}
	if ((Trees_Symbol(firstNonArrayType) == POINTER)
			|| (Types_IsPointer(firstNonArrayType) && TypeIncomplete(firstNonArrayType, ident))) {
		Print(file, 0, "*");
	} else if (Trees_Symbol(firstNonArrayType) == PROCEDURE) {
		resultType = Types_ResultType(firstNonArrayType);
		if ((declaredTypeIdent != NULL) && (resultType == declaredTypeIdent)) {
			Print(file, 0, "*");
		}
		Print(file, 0, "(*");
	}
	Generate(ident, file, 0);
	while (Trees_Symbol(type) == ARRAY) {
		Print(file, 0, "[%d]", Trees_Integer(Types_ArrayLength(type)));
		type = Types_ElementType(type);
	}
	if (Trees_Symbol(firstNonArrayType) == PROCEDURE) {
		Print(file, 0, ")(");
		if (Types_Parameters(type) != NULL) {
			GenerateFormalParameterList(Types_Parameters(firstNonArrayType), file, 0);
		} else {
			Print(file, 0, "void");
		}
		Print(file, 0, ")");
	}
}


static void GenerateDeclaration(Trees_Node declaration, FILE *file, int indent)
{
	Trees_Node identList, firstIdent, ident;

	if (Trees_Symbol(Trees_Left(declaration)) == IDENT) {
		identList = Trees_NewNode(TREES_IDENT_LIST, Trees_Left(declaration), NULL);
	} else {
		identList = Trees_Left(declaration);
	}
	firstIdent = Trees_Left(identList);

	GenerateStorageClassSpecifier(firstIdent, file, indent);
	GenerateTypeSpecifier(firstIdent, Trees_Type(firstIdent), file, indent);
	Print(file, 0, " ");

	do {
		ident = Trees_Left(identList);
		GenerateDeclarator(ident, file);

		if ((Trees_Kind(firstIdent) == TREES_VARIABLE_KIND) && Trees_Local(firstIdent) && (file != hFile)) {
			switch (Trees_Symbol(Types_Structure(Trees_Type(firstIdent)))) {
				case POINTER:
				case PROCEDURE:
					Print(file, 0, " = 0");
					break;
				case RECORD:
					Print(file, 0, " = {0}");
					break;
			}
		}
		if (Trees_Right(identList) != NULL) {
			Print(file, 0, ", ");
		}
		identList = Trees_Right(identList);
	} while (identList != NULL);

	Print(file, 0, ";\n");
}


static void GenerateTypeIDs(Trees_Node type)
{
	Trees_Node baseType;

	baseType = Types_RecordBaseType(type);
	if (baseType != NULL) {
		GenerateTypeIDs(baseType);
		Print(cFile, 0, ", ");
	}
	Print(cFile, 0, "&");
	Generate(TypeDescIdent(type), cFile, 0);
	Print(cFile, 0, "id");
}


static void GenerateHeapTypeDecl(Trees_Node typeIdent, FILE* file, int indent)
{
	Print(file, indent, "struct ");
	Generate(typeIdent, file, 0);
	Print(file, 0, "Heap {\n");
	Print(file, indent + 1, "const OBNC_Td *td;\n");
	Print(file, indent + 1, "struct ");
	Generate(typeIdent, file, 0);
	Print(file, indent, " fields;\n");
	Print(file, indent, "};\n");
}


static void GenerateTypeDescDecl(Trees_Node typeIdent, int indent)
{
	int extensionLevel;
	const char *storageClass;


	/*generate type descriptor (type ID used for its unique address only)*/
	extensionLevel = Types_ExtensionLevel(typeIdent);
	if (ModulePrefixNeeded(typeIdent)) {
		Print(hFile, indent, "\nextern const int ");
		Generate(typeIdent, hFile, 0);
		Print(hFile, 0, "id;\n");

		Print(hFile, indent, "extern const int *const ");
		Generate(typeIdent, hFile, 0);
		Print(hFile, 0, "ids[%d];\n", extensionLevel + 1);

		Print(hFile, indent, "extern const OBNC_Td ");
		Generate(typeIdent, hFile, 0);
		Print(hFile, 0, "td;\n");

		storageClass = "";
	} else {
		storageClass = "static ";
	}
	Print(cFile, 0, "\n");
	Print(cFile, indent, "%sconst int ", storageClass);
	Generate(typeIdent, cFile, 0);
	Print(cFile, 0, "id;\n");

	Print(cFile, indent, "%sconst int *const ", storageClass);
	Generate(typeIdent, cFile, 0);
	Print(cFile, 0, "ids[%d] = {", extensionLevel + 1);
	GenerateTypeIDs(typeIdent);
	Print(cFile, 0, "};\n");

	Print(cFile, indent, "%sconst OBNC_Td ", storageClass);
	Generate(typeIdent, cFile, 0);
	Print(cFile, 0, "td = {");
	Generate(typeIdent, cFile, 0);
	Print(cFile, 0, "ids, %d};\n", extensionLevel + 1);
}


void Generate_TypeDeclaration(Trees_Node ident)
{
	int indent = Trees_Local(ident)? 1: 0;
	Trees_Node type, declaration, typeDescIdent;
	int modulePrefixNeeded;

	type = Trees_Type(ident);
	modulePrefixNeeded = ModulePrefixNeeded(ident);

	declaredTypeIdent = ident;
	declaration = Trees_NewNode(TREES_NOSYM, ident, type);
	if (modulePrefixNeeded) {
		Print(hFile, 0, "\n");
		GenerateDeclaration(declaration, hFile, indent);
	} else {
		if (! Trees_Local(ident)) {
			Print(cFile, 0, "\n");
		}
		GenerateDeclaration(declaration, cFile, indent);
	}
	declaredTypeIdent = NULL;
	if ((Trees_Symbol(type) == RECORD)
			|| ((Trees_Symbol(type) == POINTER) && (Trees_Symbol(Types_PointerBaseType(type)) == RECORD))) {
		typeDescIdent = TypeDescIdent(ident);

		if (modulePrefixNeeded) {
			Print(hFile, 0, "\n");
			GenerateHeapTypeDecl(typeDescIdent, hFile, 0);
		} else {
			Print(cFile, 0, "\n");
			GenerateHeapTypeDecl(typeDescIdent, cFile, indent);
		}
		GenerateTypeDescDecl(typeDescIdent, indent);
	}
}


/*VARIABLE DECLARATION GENERATORS*/

static int HasExportedIdent(Trees_Node identList)
{
	while ((identList != NULL) && ! Trees_Exported(Trees_Left(identList))) {
		identList = Trees_Right(identList);
	}
	return identList != NULL;
}


static int NameEquivalenceNeeded(Trees_Node type)
{
	int result;
	
	assert(type != NULL);
	
	switch (Trees_Symbol(type)) {
		case ARRAY:
			result = NameEquivalenceNeeded(Types_ElementType(type));
			break;
		case RECORD:
			result = 1;
			break;
		case POINTER:
			result = (Trees_Symbol(Types_PointerBaseType(type)) == RECORD);
			break;
		default:
			result = 0;
	}
	return result;
}


static int DigitCount(int i)
{
	int n = 0;

	do {
		n++;
		i = i / 10;
	} while (i != 0);
	return n;
}


void Generate_VariableDeclaration(Trees_Node identList)
{
	static int typeCounter;

	char *newTypeName;
	int newTypeNameLen, allExported;
	Trees_Node ident, type, declaration, newTypeIdent, newTypeDecl, p, exportedIdents, nonExportedIdents, exportedDecl, nonExportedDecl;
	int indent;

	ident = Trees_Left(identList);
	indent = Trees_Local(ident)? 1: 0;
	type = Trees_Type(ident);
	declaration = Trees_NewNode(TREES_NOSYM, identList, type);
	if (! Trees_Local(ident)) {
		Print(cFile, 0, "\n");
	}
	if (HasExportedIdent(identList) && (Args_Mode() != ARGS_ENTRY_POINT_MODE)) {
		Print(hFile, 0, "\n");
		if (NameEquivalenceNeeded(type)) {
			/*declare anonymous type in header file*/
			newTypeNameLen = strlen(Args_ModuleName()) + strlen("_T") + DigitCount(typeCounter) + 1;
			NEW_ARRAY(newTypeName, newTypeNameLen);
			sprintf(newTypeName, "%s_T%d", Args_ModuleName(), typeCounter);

			newTypeIdent = Trees_NewIdent(newTypeName);
			Trees_SetKind(TREES_TYPE_KIND, newTypeIdent);
			Trees_SetType(type, newTypeIdent);
			Trees_SetInternal(newTypeIdent);
			newTypeDecl = Trees_NewNode(TREES_NOSYM, newTypeIdent, type);

			GenerateDeclaration(newTypeDecl, hFile, indent);

			/*replace anonymous type with named type*/
			p = identList;
			do {
				ident = Trees_Left(p);
				Trees_SetType(newTypeIdent, ident);
				p = Trees_Right(p);
			} while (p != NULL);

			typeCounter++;
		}

		allExported = 1;
		p = identList;
		do {
			ident = Trees_Left(p);
			if (! Trees_Exported(ident)) {
				allExported = 0;
			}
			p = Trees_Right(p);
		} while ((p != NULL) && allExported);

		if (allExported) {
			GenerateDeclaration(declaration, hFile, indent);
			GenerateDeclaration(declaration, cFile, indent);
		} else {
			exportedIdents = NULL;
			nonExportedIdents = NULL;
			p = identList;
			do {
				ident = Trees_Left(p);
				if (Trees_Exported(ident)) {
					exportedIdents = Trees_NewNode(TREES_IDENT_LIST, ident, exportedIdents);
				} else {
					nonExportedIdents = Trees_NewNode(TREES_IDENT_LIST, ident, nonExportedIdents);
				}
				p = Trees_Right(p);
			} while (p != NULL);
			assert(exportedIdents != NULL);
			Trees_ReverseList(&exportedIdents);
			exportedDecl = Trees_NewNode(TREES_NOSYM, exportedIdents, Trees_Right(declaration));
			GenerateDeclaration(exportedDecl, hFile, indent);
			GenerateDeclaration(exportedDecl, cFile, indent);
			if (nonExportedIdents != NULL) {
				Trees_ReverseList(&nonExportedIdents);
				nonExportedDecl = Trees_NewNode(TREES_NOSYM, nonExportedIdents, Trees_Right(declaration));
				GenerateDeclaration(nonExportedDecl, cFile, indent);
			}
		}

		if (Trees_Symbol(type) != IDENT) {
			/*reset original type*/
			p = identList;
			do {
				ident = Trees_Left(p);
				Trees_SetType(type, ident);
				p = Trees_Right(p);
			} while (p != NULL);
		}
	} else {
		GenerateDeclaration(declaration, cFile, indent);
	}
}


/*EXPRESSION GENERATORS*/

static Trees_Node VarIdent(Trees_Node var)
{
	assert(Trees_Symbol(var) == TREES_DESIGNATOR);
	return Trees_Left(var);
}


static Trees_Node VarSelector(Trees_Node var)
{
	assert(Trees_Symbol(var) == TREES_DESIGNATOR);
	return Trees_Right(var);
}


static int IsVarParam(Trees_Node var)
{
	return (Trees_Kind(VarIdent(var)) == TREES_VAR_PARAM_KIND) && (VarSelector(var) == NULL);
}


static int IsProcedureCall(int symbol)
{
	int result;

	switch (symbol) {
		case TREES_ABS_PROC:
		case TREES_ODD_PROC:
		case TREES_LEN_PROC:
		case TREES_LSL_PROC:
		case TREES_ASR_PROC:
		case TREES_ROR_PROC:
		case TREES_FLOOR_PROC:
		case TREES_FLT_PROC:
		case TREES_ORD_PROC:
		case TREES_CHR_PROC:
		case TREES_INC_PROC:
		case TREES_DEC_PROC:
		case TREES_INCL_PROC:
		case TREES_EXCL_PROC:
		/*case TREES_NEW_PROC*/
		case TREES_ASSERT_PROC:
		case TREES_PACK_PROC:
		case TREES_UNPK_PROC:
		case TREES_PROCEDURE_CALL:
			result = 1;
			break;
		default:
			result = 0;
	}
	return result;
}


static void PrintCOperator(Trees_Node opNode, FILE *file, int indent)
{
	int leftType, rightType;

	leftType = Trees_Symbol(Types_Structure(Trees_Type(Trees_Left(opNode))));
	if (Trees_Right(opNode) != NULL) {
		rightType = Trees_Symbol(Types_Structure(Trees_Type(Trees_Right(opNode))));
	} else {
		rightType = -1;
	}

	switch (Trees_Symbol(opNode)) {
		case '#':
			Print(file, indent, "!=");
			break;
		case '&':
			Print(file, indent, "&&");
			break;
		case '*':
			if (leftType == TREES_SET_TYPE) {
				Print(file, indent, "&");
			} else {
				Print(file, indent, "*");
			}
			break;
		case '+':
			if ((leftType == TREES_SET_TYPE) && (rightType >= 0)) {
				Print(file, indent, "|");
			} else {
				Print(file, indent, "+");
			}
			break;
		case '-':
			if (leftType == TREES_SET_TYPE) {
				if (rightType == -1) {
					Print(file, indent, "~");
				} else {
					Print(file, indent, "& ~");
				}
			} else {
				Print(file, indent, "-");
			}
			break;
		case '/':
			if (leftType == TREES_SET_TYPE) {
				Print(file, indent, "^");
			} else {
				Print(file, indent, "/");
			}
			break;
		case '<':
			Print(file, indent, "<");
			break;
		case '=':
			Print(file, indent, "==");
			break;
		case '>':
			Print(file, indent, ">");
			break;
		case '~':
			Print(file, indent, "! ");
			break;
		case OR:
			Print(file, indent, "||");
			break;
		case GE:
			Print(file, indent, ">=");
			break;
		case LE:
			Print(file, indent, "<=");
			break;
		default:
			assert(0);
	}
}


static void GenerateArrayLength(Trees_Node ident, Trees_Node arrayType, FILE *file)
{
	assert(Trees_Symbol(ident) == IDENT);
	assert(Types_IsArray(arrayType));

	if (Types_IsOpenArray(arrayType)) {
		Generate(ident, file, 0);
		Print(file, 0, "len");
	} else {
		Print(file, 0, "%d", Trees_Integer(Types_ArrayLength(arrayType)));
	}
}


static void GenerateNonScalarOperation(Trees_Node opNode, FILE *file, int indent)
{
	Trees_Node leftOperand, rightOperand;
	Trees_Node leftType, rightType;

	leftOperand = Trees_Left(opNode);
	rightOperand = Trees_Right(opNode);
	leftType = Types_Structure(Trees_Type(leftOperand));
	rightType = Types_Structure(Trees_Type(rightOperand));

	switch (Trees_Symbol(opNode)) {
		case '=':
		case '#':
		case '<':
		case LE:
		case '>':
		case GE:
			Print(file, indent, "OBNC_CMP(");
			Generate(leftOperand, file, indent);
			Print(file, 0, ", ");
			if (Trees_Symbol(leftType) == TREES_STRING_TYPE) {
				Print(file, 0, "%d", strlen(Trees_String(leftOperand)) + 1);
			} else {
				GenerateArrayLength(VarIdent(leftOperand), leftType, file);
			}
			Print(file, 0, ", ");
			Generate(rightOperand, file, indent);
			Print(file, 0, ", ");
			if (Trees_Symbol(rightType) == TREES_STRING_TYPE) {
				Print(file, 0, "%d", strlen(Trees_String(rightOperand)) + 1);
			} else {
				GenerateArrayLength(VarIdent(rightOperand), rightType, file);
			}
			Print(file, 0, ") ");
			PrintCOperator(opNode, file, indent);
			Print(file, 0, " 0");
			break;
		default:
			assert(0);
	}
}


static void GenerateWithPrecedence(Trees_Node exp, FILE *file, int indent)
{
	if (Trees_IsLeaf(exp)
			|| (Trees_Symbol(exp) == TREES_DESIGNATOR)
			|| IsProcedureCall(Trees_Symbol(exp))) {
		Generate(exp, file, indent);
	} else {
		Print(file, indent, "(");
		Generate(exp, file, indent);
		Print(file, indent, ")");
	}
}


static void GenerateTypeDescExp(Trees_Node var, FILE *file, int indent)
{
	if (Types_IsRecord(Trees_Type(var))) {
		if (IsVarParam(var)) {
			GenerateIdent(VarIdent(var), file, indent);
			Print(file, 0, "td");
		} else {
			Print(file, indent, "&");
			GenerateIdent(TypeDescIdent(Trees_Type(var)), file, 0);
			Print(file, 0, "td");
		}
	} else {
		assert(Types_IsPointer(Trees_Type(var)));
		Print(file, indent, "OBNC_TD(");
		Generate(var, file, 0);
		Print(file, 0, ", struct ");
		Generate(TypeDescIdent(Trees_Type(var)), file, 0);
		Print(file, 0, "Heap)");
	}
}


static void GenerateISExpression(Trees_Node var, Trees_Node type, FILE *file)
{
	int isPointer;

	isPointer = Types_IsPointer(Trees_Type(var));
	if (isPointer) {
		Print(file, 0, "((void) OBNC_PT(");
		Generate(var, file, 0);
		Print(file, 0, "), ");
	}
	Print(file, 0, "OBNC_IS(");
	GenerateTypeDescExp(var, file, 0);
	Print(file, 0, ", &");
	Generate(TypeDescIdent(type), file, 0);
	Print(file, 0, "id, %d)", Types_ExtensionLevel(type));
	if (isPointer) {
		Print(file, 0, ")");
	}
}


static void GenerateOperator(Trees_Node opNode, FILE *file, int indent)
{
	Trees_Node leftOperand, rightOperand, leftType, rightType;
	int opSym;

	leftOperand = Trees_Left(opNode);
	rightOperand = Trees_Right(opNode);
	opSym = Trees_Symbol(opNode);

	if (Trees_Right(opNode) == NULL) {
		/*unary operator*/
		PrintCOperator(opNode, file, indent);
		GenerateWithPrecedence(leftOperand, file, 0);
	} else {
		/*binary operator*/
		leftType = Trees_Type(leftOperand);
		rightType = Trees_Type(rightOperand);

		if ((Types_IsString(leftType) || Types_IsCharacterArray(leftType))
				&& (Types_IsString(rightType) || Types_IsCharacterArray(rightType))) {
			GenerateNonScalarOperation(opNode, file, indent);
		} else {
			switch (opSym) {
				case DIV:
				case MOD:
					if (opSym == DIV) {
						Print(file, indent, "OBNC_DIV(");
					} else {
						Print(file, indent, "OBNC_MOD(");
					}
					Generate(leftOperand, file, 0);
					Print(file, 0, ", ");
					Generate(rightOperand, file, 0);
					Print(file, 0, ")");
					break;
				case '<':
				case LE:
				case '>':
				case GE:
					if (Types_IsChar(Trees_Type(leftOperand))) {
						Print(file, indent, "(unsigned char) ");
					}
					GenerateWithPrecedence(leftOperand, file, 0);
					Print(file, 0, " ");
					PrintCOperator(opNode, file, 0);
					Print(file, 0, " ");
					if (Types_IsChar(Trees_Type(rightOperand))) {
						Print(file, indent, "(unsigned char) ");
					}
					GenerateWithPrecedence(rightOperand, file, 0);
					break;
				default:
					if (Types_IsPointer(leftType) && (Trees_Symbol(leftOperand) != NIL) && ! Types_Same(leftType, rightType) && (Trees_Symbol(rightOperand) != NIL)) {
						if (Types_Extends(leftType, rightType)) {
							GenerateWithPrecedence(leftOperand, file, indent);
							Print(file, 0, " ");
							PrintCOperator(opNode, file, 0);
							Print(file, 0, " (");
							Generate(leftType, file, 0);
							Print(file, indent, ") ");
							GenerateWithPrecedence(rightOperand, file, 0);
						} else {
							Print(file, indent, "(");
							Generate(rightType, file, 0);
							Print(file, indent, ") ");
							GenerateWithPrecedence(leftOperand, file, indent);
							Print(file, 0, " ");
							PrintCOperator(opNode, file, 0);
							Print(file, 0, " ");
							GenerateWithPrecedence(rightOperand, file, 0);
						}
					} else {
						GenerateWithPrecedence(leftOperand, file, indent);
						Print(file, 0, " ");
						PrintCOperator(opNode, file, 0);
						Print(file, 0, " ");
						GenerateWithPrecedence(rightOperand, file, 0);
					}
			}
		}
	}
}


static int IsConstExpression(Trees_Node exp)
{
	int result;

	result = 0;
	switch (Trees_Symbol(exp)) {
		case TRUE:
		case FALSE:
		case STRING:
		case INTEGER:
		case REAL:
		case TREES_SET_CONSTANT:
			result = 1;
	}
	return result;
}


static int ContainsProcedureCall(Trees_Node exp)
{
	int result;

	result = 0;
	if (exp != NULL) {
		if (Trees_Symbol(exp) == TREES_PROCEDURE_CALL) {
			result = 1;
		} else {
			result = ContainsProcedureCall(Trees_Left(exp));
			if (result == 0) {
				result = ContainsProcedureCall(Trees_Right(exp));
			}
		}
	}
	return result;
}


static void GenerateArrayIndex(Trees_Node designator, Trees_Node elemSelector, FILE *file)
{
	Trees_Node indexExp, type, ident;

	assert(designator != NULL);
	assert(Trees_Symbol(designator) == TREES_DESIGNATOR);
	assert(elemSelector != NULL);
	assert(Trees_Symbol(elemSelector) == '[');

	ident = Trees_Left(designator);
	indexExp = Trees_Left(elemSelector);
	type = Trees_Type(elemSelector);
	assert(Types_IsArray(type));

	if (IsConstExpression(indexExp)) {
		Generate(indexExp, file, 0);
	} else {
		if (ContainsProcedureCall(indexExp)) {
			Print(file, 0, "OBNC_IT1(");
		} else {
			Print(file, 0, "OBNC_IT(");
		}
		Generate(indexExp, file, 0);
		Print(file, 0, ", ");
		GenerateArrayLength(ident, type, file);
		Print(file, 0, ")");
	}
}


static void GenerateDesignatorVar(Trees_Node ident, FILE *file)
{
	int identKind, paramDerefNeeded;
	Trees_Node identType;

	identKind = Trees_Kind(ident);
	identType = Trees_Type(ident);
	paramDerefNeeded = ((identKind == TREES_VALUE_PARAM_KIND) && Types_IsRecord(identType))
			|| ((identKind == TREES_VAR_PARAM_KIND) && ! Types_IsArray(identType));

	if (paramDerefNeeded) {
		Print(file, 0, "(*");
		Generate(ident, file, 0);
		Print(file, 0, ")");
	} else {
		Generate(ident, file, 0);
	}
}


static void GenerateDesignatorRec(Trees_Node des, Trees_Node reversedSelectors, FILE *file)
{
	Trees_Node field, fieldIdent, fieldBaseType, typeIdent;
	int castNeeded;
	
	if (reversedSelectors == NULL) {
		if ((caseVariable != NULL) && (caseLabelType != NULL) && (VarIdent(des) == caseVariable) && ! Types_Same(Trees_Type(caseVariable), caseLabelType)) {
			Print(file, 0, "(*((");
			Generate(caseLabelType, file, 0);
			Print(file, 0, " *) &");
			GenerateDesignatorVar(VarIdent(des), file);
			Print(file, 0, "))");
		} else {
			GenerateDesignatorVar(VarIdent(des), file);
		}
	} else {
		switch (Trees_Symbol(reversedSelectors)) {
			case '[':
				GenerateDesignatorRec(des, Trees_Right(reversedSelectors), file);
				Print(file, 0, "[");
				GenerateArrayIndex(des, reversedSelectors, file);
				Print(file, 0, "]");
				break;
			case '.':
				field = Trees_Left(reversedSelectors);
				Types_GetFieldIdent(Trees_Name(field), Trees_Type(reversedSelectors), Trees_Imported(VarIdent(des)), &fieldIdent, &fieldBaseType);
				castNeeded = ! Types_Same(fieldBaseType, Trees_Type(reversedSelectors));
				if (castNeeded) {
					Print(file, 0, "(*((");
					Generate(fieldBaseType, file, 0);
					if (Types_IsRecord(fieldBaseType)) {
						Print(file, 0, " *");
					}
					Print(file, 0, ") &");
				}
				GenerateDesignatorRec(des, Trees_Right(reversedSelectors), file);
				if (castNeeded) {
					Print(file, 0, "))");
				}
				Print(file, 0, ".");
				Generate(Trees_Left(reversedSelectors), file, 0);
				break;
			case '^':
				Print(file, 0, "(*OBNC_PT(");
				GenerateDesignatorRec(des, Trees_Right(reversedSelectors), file);
				Print(file, 0, "))");
				break;
			case '(':
				typeIdent = Trees_Left(reversedSelectors);

				Print(file, 0, "(*((");
				Generate(typeIdent, file, 0);
				if (Types_IsRecord(typeIdent)) {
					Print(file, 0, "*) OBNC_RTT(&(");
				} else {
					Print(file, 0, "*) OBNC_PTT(&(");
				}
				GenerateDesignatorRec(des, Trees_Right(reversedSelectors), file);
				Print(file, 0, "), ");
				if (Types_IsRecord(typeIdent)) {
					if (IsVarParam(des) && (reversedSelectors == VarSelector(des))) {
						GenerateIdent(VarIdent(des), file, 0);
						Print(file, 0, "td");
					} else {
						Print(file, 0, "&");
						GenerateIdent(TypeDescIdent(Trees_Type(reversedSelectors)), file, 0);
						Print(file, 0, "td");
					}
				} else {
					assert(Types_IsPointer(typeIdent));
					Print(file, 0, "OBNC_TD(");
					GenerateDesignatorRec(des, Trees_Right(reversedSelectors), file);
					Print(file, 0, ", struct ");
					Generate(TypeDescIdent(Trees_Type(reversedSelectors)), file, 0);
					Print(file, 0, "Heap)");
				}
				Print(file, 0, ", &");
				Generate(TypeDescIdent(typeIdent), file, 0);
				Print(file, 0, "id, %d)))", Types_ExtensionLevel(typeIdent));				
				break;
			default:
				assert(0);
		}
	}
}


static void GenerateDesignator(Trees_Node des, FILE *file, int indent)
{
	Trees_Node selectors;

	Print(file, indent, "");
	selectors = Trees_Right(des);
	Trees_ReverseList(&selectors);
	GenerateDesignatorRec(des, selectors, file);
	Trees_ReverseList(&selectors); /*reset order*/
}


static void GenerateSingleElementSet(Trees_Node node, FILE *file, int indent)
{
	Print(file, indent, "(0x1u << ");
	GenerateWithPrecedence(Trees_Left(node), file, indent);
	Print(file, indent, ")");
}


static void GenerateRangeSet(Trees_Node node, FILE *file, int indent)
{
	Print(file, indent, "OBNC_RANGE(");
	Generate(Trees_Left(node), file, indent);
	Print(file, indent, ", ");
	Generate(Trees_Right(node), file, indent);
	Print(file, indent, ")");
}


static void GenerateExpList(Trees_Node expList, FILE *file)
{
	Trees_Node exp, tail;

	exp = Trees_Right(expList);
	Generate(exp, file, 0);
	tail = Trees_Right(expList);
	if (tail != NULL) {
		Print(file, 0, ", ");
		Generate(tail, file, 0);
	}
}


/*STATEMENT GENERATORS*/

static void GenerateArrayAssignment(Trees_Node source, Trees_Node target, FILE *file, int indent)
{
	Trees_Node sourceIdent, targetIdent, sourceType, targetType;

	assert(Trees_Symbol(target) == TREES_DESIGNATOR);

	if (Trees_Symbol(source) == TREES_DESIGNATOR) {
		sourceIdent = Trees_Left(source);
	} else {
		sourceIdent = source;
	}
	targetIdent = Trees_Left(target);
	sourceType = Trees_Type(source);
	targetType = Types_Structure(Trees_Type(target));
	assert(Trees_Symbol(targetType) == ARRAY);

	if (Types_IsOpenArray(sourceType) || Types_IsOpenArray(targetType)) {
		Print(file, indent, "OBNC_AAT(");
		if (Trees_Symbol(source) == STRING) {
			Print(file, 0, "%d", strlen(Trees_String(source)) + 1);
		} else {
			GenerateArrayLength(sourceIdent, sourceType, file);
		}
		Print(file, 0, ", ");
		GenerateArrayLength(targetIdent, targetType, file);
		Print(file, 0, ");\n");
	}
	Print(file, indent, "OBNC_COPY_ARRAY(");
	Generate(source, file, 0);
	Print(file, 0, ", ");
	Generate(target, file, 0);
	Print(file, 0, ", ");
	if (Trees_Symbol(source) == STRING) {
		Print(file, 0, "%d", strlen(Trees_String(source)) + 1);
	} else {
		GenerateArrayLength(sourceIdent, sourceType, file);
	}
	Print(file, 0, ");\n");
}


static void GenerateRecordAssignment(Trees_Node source, Trees_Node target, FILE *file, int indent)
{
	Trees_Node sourceType, targetType;

	sourceType = Trees_Type(source);
	targetType = Trees_Type(target);

	if (IsVarParam(target)) {
		Print(file, indent, "OBNC_RAT(");
		GenerateTypeDescExp(source, file, 0);
		Print(file, 0, ", ");
		GenerateTypeDescExp(target, file, 0);
		Print(file, 0, ");\n");
	}
	if (Types_Same(sourceType, targetType) && ! IsVarParam(target)) {
		GenerateDesignator(target, file, indent);
		Print(file, 0, " = ");
		Generate(source, file, 0);
		Print(file, 0, ";\n");
	} else {
		Generate(target, file, indent);
		Print(file, 0, " = ");
		if (! Types_Same(sourceType, targetType)) {
			assert(Types_Extends(targetType, sourceType));
			Print(file, 0, "*(");
			Generate(targetType, file, 0);
			Print(file, 0, " *) &");
		}
		Generate(source, file, 0);
		Print(file, 0, ";\n");
	}
}


static int CastNeeded(Trees_Node sourceType, Trees_Node targetType)
{
	return (Types_IsByte(targetType) && (Trees_Symbol(Types_Structure(sourceType)) == INTEGER))
		|| ((Types_IsRecord(targetType) || Types_IsPointer(targetType))
			&& (Trees_Symbol(sourceType) != TREES_NIL_TYPE)
			&& Types_Extends(targetType, sourceType)
			&& ! Types_Same(targetType, sourceType));
}


static void GenerateAssignment(Trees_Node becomesNode, FILE *file, int indent)
{
	Trees_Node source, target;
	Trees_Node sourceType, targetType;

	source = Trees_Right(becomesNode);
	target = Trees_Left(becomesNode);
	sourceType = Trees_Type(source);
	targetType = Trees_Type(target);

	switch (Trees_Symbol(Types_Structure(targetType))) {
		case ARRAY:
			GenerateArrayAssignment(source, target, file, indent);
			break;
		case RECORD:
			GenerateRecordAssignment(source, target, file, indent);
			break;
		default:
			GenerateDesignator(target, file, indent);
			Print(file, 0, " = ");
			if (CastNeeded(sourceType, targetType)) {
				Print(file, 0, "(");
				Generate(targetType, file, 0);
				Print(file, 0, ") ");
			}
			GenerateWithPrecedence(source, file, 0);
			Print(file, 0, ";\n");
	}
}


static void GenerateProcedureCall(Trees_Node call, FILE *file, int indent)
{
	Trees_Node designator, designatorTypeStruct, expList, fpList, fpType, exp, expType, resultType;
	int procKind, isProcVar, isValueParam, isVarParam;

	designator = Trees_Left(call);
	designatorTypeStruct = Types_Structure(Trees_Type(designator));
	procKind = Trees_Kind(Trees_Left(designator));
	assert(Types_IsProcedure(designatorTypeStruct));
	resultType = Types_ResultType(designatorTypeStruct);
	isProcVar = procKind != TREES_PROCEDURE_KIND;

	if (isProcVar) {
		Print(file, indent, "OBNC_PCT(");
		Generate(designator, file, 0);
		Print(file, 0, ")");
	} else {
		Generate(designator, file, indent);
	}

	Print(file, 0, "(");

	expList = Trees_Right(call);
	fpList = Types_Parameters(designatorTypeStruct);
	while (expList != NULL) {
		assert(fpList != NULL);
		exp = Trees_Left(expList);
		expType = Trees_Type(exp);
		isValueParam = Trees_Kind(Trees_Left(fpList)) == TREES_VALUE_PARAM_KIND;
		isVarParam = Trees_Kind(Trees_Left(fpList)) == TREES_VAR_PARAM_KIND;
		fpType = Trees_Type(Trees_Left(fpList));

		if (isValueParam && Types_IsArray(fpType) && Types_IsArray(Types_ElementType(fpType))) {
			/*cast to const needed for array of array parameters*/
			Print(file, 0, "(const ");
			Generate(Types_ElementType(expType), file, 0);
			Print(file, 0, " *) ");
		} else if (CastNeeded(expType, fpType)) {
			Print(file, 0, "(");
			Generate(fpType, file, 0);
			if ((isVarParam && ! Types_IsArray(fpType)) || Types_IsRecord(fpType)) {
				Print(file, 0, " *");
			}
			Print(file, 0, ") ");
		}
		if ((isValueParam && Types_IsRecord(fpType)) || (isVarParam && ! Types_IsArray(fpType))) {
			Print(file, 0, "&");
		}
		GenerateWithPrecedence(exp, file, 0);

		/*additional type info parameters*/
		if (Types_IsArray(expType)) {
			Print(file, 0, ", ");
			if (Trees_Symbol(exp) == TREES_DESIGNATOR) {
				GenerateArrayLength(Trees_Left(exp), expType, file);
			} else {
				GenerateArrayLength(exp, expType, file);
			}
		} else if (Trees_Symbol(exp) == STRING) {
			Print(file, 0, ", %d", strlen(Trees_String(exp)) + 1);
		} else if (isVarParam && Types_IsRecord(fpType)) {
			Print(file, 0, ", ");
			GenerateTypeDescExp(exp, file, 0);
		}

		if (Trees_Right(expList) != NULL) {
			Print(file, 0, ", ");
		}
		expList = Trees_Right(expList);
		fpList = Trees_Right(fpList);
	}

	Print(file, 0, ")");
	if (resultType == NULL) {
		Print(file, 0, ";\n");
	}
}


static void GenerateAssert(Trees_Node node, FILE *file, int indent)
{
	Trees_Node exp, filename, line;

	exp = Trees_Left(node);
	filename = Trees_Left(Trees_Right(node));
	line = Trees_Right(Trees_Right(node));

	if (Trees_Symbol(exp) == FALSE) {
		/*unconditional ASSERT(FALSE) replaces HALT(1)*/
		Print(file, indent, "exit(1);\n");
	} else {
		Print(file, indent, "OBNC_ASSERT(");
		Generate(exp, file, 0);
		Print(file, 0, ", ");
		Generate(filename, file, 0);
		Print(file, 0, ", ");
		Generate(line, file, 0);
		Print(file, 0, ");\n");
	}
}


static void GenerateIntegralCaseStatement(Trees_Node caseStmtNode, FILE *file, int indent)
{
	Trees_Node expNode, currCaseRepNode, currCaseNode, currCaseLabelListNode, currStmtSeqNode, currLabelRangeNode;
	int rangeMin, rangeMax, label;

	expNode = Trees_Left(caseStmtNode);

	Print(file, indent, "switch (");
	Generate(expNode, file, 0);
	Print(file, 0, ") {\n");
	currCaseRepNode = Trees_Right(caseStmtNode);
	while (currCaseRepNode != NULL) {
		currCaseNode = Trees_Left(currCaseRepNode);
		currStmtSeqNode = Trees_Right(currCaseNode);

		/*generate case labels for current case*/
		currCaseLabelListNode = Trees_Left(currCaseNode);
		do {
			currLabelRangeNode = Trees_Left(currCaseLabelListNode);
			if (Trees_Right(currLabelRangeNode) == NULL) {
				/*generate single label*/
				Print(file, indent + 1, "case ");
				Generate(currLabelRangeNode, file, 0);
				Print(file, 0, ":\n");
			} else {
				/*generate label range*/
				if (Trees_Symbol(Trees_Left(currLabelRangeNode)) == INTEGER) {
					rangeMin = Trees_Integer(Trees_Left(currLabelRangeNode));
					rangeMax = Trees_Integer(Trees_Right(currLabelRangeNode));
					for (label = rangeMin; label <= rangeMax; label++) {
						Print(file, indent + 1, "case %d:\n", label);
					}
				} else {
					rangeMin = Trees_Char(Trees_Left(currLabelRangeNode));
					rangeMax = Trees_Char(Trees_Right(currLabelRangeNode));
					for (label = rangeMin; label <= rangeMax; label++) {
						Print(file, indent + 1, "case ");
						GenerateChar(label, file, 0);
						Print(file, 0, ":\n");
					}
				}
			}
			currCaseLabelListNode = Trees_Right(currCaseLabelListNode);
		} while (currCaseLabelListNode != NULL);

		/*generate statement sequence for current case*/
		Generate(currStmtSeqNode, file, indent + 2);
		Print(file, indent + 2, "break;\n");

		currCaseRepNode = Trees_Right(currCaseRepNode);
	}
	Print(file, indent + 1, "default:\n");
	Print(file, indent +2, "fprintf(stderr, \"exception: unmatched expression in case statement\\n\");\n");
	Print(file, indent +2, "abort();\n");
	Print(file, indent, "}\n");
}


static void GenerateTypeCaseStatement(Trees_Node caseStmtNode, FILE *file, int indent)
{
	Trees_Node caseExp, caseList, caseNode, label, statementSeq;
	int caseNumber;

	caseExp = Trees_Left(caseStmtNode);
	assert(Trees_Symbol(caseExp) == TREES_DESIGNATOR);
	caseVariable = Trees_Left(caseExp);

	caseList = Trees_Right(caseStmtNode);
	caseNumber = 0;
	while (caseList != NULL) {
		caseNode = Trees_Left(caseList);
		label = Trees_Left(Trees_Left(caseNode));
		statementSeq = Trees_Right(caseNode);

		if (caseNumber == 0) {
			Print(file, indent, "if (");
		} else {
			Print(file, 0, " else if (");
		}
		GenerateISExpression(caseExp, label, file);
		Print(file, 0, ") {\n");
		caseLabelType = label;
		Generate(statementSeq, file, indent + 1);
		caseLabelType = NULL;
		Print(file, indent, "}");
		caseList = Trees_Right(caseList);
		if (caseList == NULL) {
			Print(file, 0, "\n");
		}
		caseNumber++;
	}

	caseVariable = NULL;
}


static void GenerateCaseStatement(Trees_Node caseStmtNode, FILE *file, int indent)
{
	Trees_Node expNode, expType;

	expNode = Trees_Left(caseStmtNode);
	expType = Trees_Type(expNode);
	if (Types_IsInteger(expType) || Types_IsChar(expType)) {
		GenerateIntegralCaseStatement(caseStmtNode, file, indent);
	} else {
		GenerateTypeCaseStatement(caseStmtNode, file, indent);
	}
}


static void GenerateWhileStatement(Trees_Node whileNode, FILE *file, int indent)
{
	Trees_Node expNode, doNode, stmtSeqNode, elsifNode;

	expNode = Trees_Left(whileNode);
	doNode = Trees_Right(whileNode);
	stmtSeqNode = Trees_Left(doNode);
	elsifNode = Trees_Right(doNode);
	if (elsifNode == NULL) {
		Print(file, indent, "while (");
		Generate(expNode, file, 0);
		Print(file, 0, ") {\n");
		Generate(stmtSeqNode, file, indent + 1);
		Print(file, indent, "}\n");
	} else {
		Print(file, indent, "while (1) {\n");
		Print(file, indent + 1, "if (");
		Generate(expNode, file, 0);
		Print(file, 0, ") {\n");
		Generate(stmtSeqNode, file, indent + 2);
		Print(file, indent + 1, "}\n");
		Generate(elsifNode, file, indent + 1);
		Print(file, indent + 1, "else {\n");
		Print(file, indent + 2, "break;\n");
		Print(file, indent + 1, "}\n");
		Print(file, indent, "}\n");
	}
}


static void GenerateForStatement(Trees_Node forNode, FILE *file, int indent)
{
	Trees_Node initNode, controlVarNode, toNode, limit, byNode, statementSeq;
	int inc;

	initNode = Trees_Left(forNode);
	controlVarNode = Trees_Left(initNode);
	toNode = Trees_Right(forNode);
	limit = Trees_Left(toNode);
	byNode = Trees_Right(toNode);
	inc = Trees_Integer(Trees_Left(byNode));
	assert(inc != 0);
	statementSeq = Trees_Right(byNode);

	Print(file, indent, "for (");
	Generate(controlVarNode, file, 0);
	Print(file, 0, " = ");
	Generate(Trees_Right(initNode), file, 0);
	Print(file, 0, "; ");
	Generate(controlVarNode, file, 0);
	if (inc > 0)  {
		Print(file, 0, " <= ");
	} else {
		Print(file, 0, " >= ");
	}
	Generate(limit, file, 0);
	Print(file, 0, "; ");
	Generate(controlVarNode, file, 0);
	Print(file, 0, " += %d) {\n", inc);
	Generate(statementSeq, file, indent + 1);
	Print(file, indent, "}\n");
}


static void GenerateMemoryAllocation(Trees_Node var, FILE *file, int indent)
{
	Trees_Node type;

	assert(var != NULL);
	assert(Trees_Symbol(var) == TREES_DESIGNATOR);

	type = Trees_Type(var);
	if ((Trees_Symbol(type) == IDENT) || (Trees_Symbol(Types_PointerBaseType(type)) == IDENT)) {
		Print(file, indent, "OBNC_NEW(");
		Generate(var, file, 0);
		Print(file, 0, ", &");
		Generate(TypeDescIdent(type), file, 0);
		Print(file, 0, "td, struct ");
		Generate(TypeDescIdent(type), file, 0);
		Print(file, 0, "Heap);\n");
	} else {
		Print(file, indent, "OBNC_NEW0(");
		Generate(var, file, 0);
		Print(file, 0, ");\n");
	}
}


/*PROCEDURE DECLARATION GENERATORS*/

static void CopyText(FILE *source, long int pos, int count, FILE *target)
{
	long int oldPos;
	int i, ch;

	assert(source != NULL);
	assert(pos >= 0);
	assert(count >= 0);
	assert(target != NULL);

	oldPos = ftell(source);
	if (oldPos >= 0) {
		fseek(source, pos, SEEK_SET);
		if (! ferror(source)) {
			i = 0;
			ch = fgetc(source);
			while ((i < count) && (ch != EOF)) {
				fputc(ch, target);
				i++;
				ch = fgetc(source);
			}
		}
	}
	fseek(source, oldPos, SEEK_SET);

	if (ferror(source) || ferror(target)) {
		fprintf(stderr, "CopyText failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
}


static void ReadText(FILE *fp, long int count, char result[], int resultLen)
{
	int i, ch;

	assert(count < resultLen);

	if (! ferror(fp)) {
		i = 0;
		ch = fgetc(fp);
		while ((ch != EOF) && (i < count)) {
			result[i] = ch;
			i++;
			ch = fgetc(fp);
		}
		result[count] = '\0';
	} else {
		perror(NULL);
		exit(EXIT_FAILURE);
	}
}


static void PushProcedureDeclaration(Trees_Node procIdent)
{
	struct ProcedureDeclNode *node;
	int generatedLen, ch;

	NEW(node);
	node->procIdent = procIdent;
	node->localProcedures = Maps_New();
	node->runtimeInitVars = NULL;
	if (Trees_Local(procIdent)) {
		/*save unfinished procedure declaration*/
		generatedLen = ftell(cFile) - procedureDeclStart + 1;
		NEW_ARRAY(node->partialDecl, generatedLen);
		fseek(cFile, procedureDeclStart, SEEK_SET);
		ReadText(cFile, generatedLen - 1, node->partialDecl, generatedLen);
	} else {
		node->partialDecl = NULL;
	}
	node->next = procedureDeclStack;

	if (Trees_Local(procIdent)) {
		Maps_Put(Trees_Name(procIdent), NULL, &(procedureDeclStack->localProcedures));

		/*set file position for writing local procedure*/
		fseek(cFile, procedureDeclStart, SEEK_SET);
		assert(procedureDeclStack != NULL);
		if (procedureDeclStack->next == NULL) {
			/*keep function signature for global procedure*/
			do {
				ch = fgetc(cFile);
			} while ((ch != EOF) && (ch != ')'));
			assert(ch == ')');
			Print(cFile, 0, ";\n");
		}
	}

	procedureDeclStack = node;
}


static void PopProcedureDeclaration(void)
{
	assert(procedureDeclStack != NULL);
	procedureDeclStart = ftell(cFile);
	if (procedureDeclStack->partialDecl != NULL) {
		Print(cFile, 0, procedureDeclStack->partialDecl);
	}
	procedureDeclStack = procedureDeclStack->next;
}


static void GenerateFormalParameter(Trees_Node param, FILE *file, int indent)
{
	int kind;
	Trees_Node type;

	kind = Trees_Kind(param);
	type = Trees_Type(param);
	if (kind == TREES_VALUE_PARAM_KIND) {
		if (Types_IsArray(type) || Types_IsRecord(type)) {
			Print(file, 0, "const ");
		}
		if (Types_IsRecord(type) || (type == declaredTypeIdent)) {
			Print(file, 0, "struct ");
		}
		if (Types_IsArray(type)) {
			Generate(Types_ElementType(type), file, 0);
			Print(file, 0, " ");
			Generate(param, file, 0);
			Print(file, 0, "[], int ");
			Generate(param, file, 0);
			Print(file, 0, "len");
		} else {
			Generate(type, file, 0);
			Print(file, 0, " ");
			if (Types_IsRecord(type) || (type == declaredTypeIdent)) {
				Print(file, 0, "*");
			}
			Generate(param, file, 0);
		}
	} else {
		assert(kind == TREES_VAR_PARAM_KIND);
		if (type == declaredTypeIdent) {
			Print(file, 0, "struct ");
		}
		if (Types_IsArray(type)) {
			Generate(Types_ElementType(type), file, 0);
			Print(file, 0, " ");
			Generate(param, file, 0);
			Print(file, 0, "[], int ");
			Generate(param, file, 0);
			Print(file, 0, "len");
		} else {
			Generate(type, file, 0);
			Print(file, 0, " *");
			if (Types_IsPointer(type) && (type == declaredTypeIdent)) {
				Print(file, 0, "*");
			}
			Generate(param, file, 0);
			if (Types_IsRecord(type)) {
				Print(file, 0, ", const OBNC_Td *");
				Generate(param, file, 0);
				Print(file, 0, "td");
			}
		}
	}
}


static void GenerateFormalParameterList(Trees_Node paramList, FILE *file, int indent)
{
	Trees_Node param;

	assert(paramList != NULL);

	do {
		param = Trees_Left(paramList);
		GenerateFormalParameter(param, file, 0);
		if (Trees_Right(paramList) != NULL) {
			Print(file, 0, ", ");
		}
		paramList = Trees_Right(paramList);
	} while (paramList != NULL);
}


void Generate_ProcedureHeading(Trees_Node procIdent)
{
	Trees_Node procType, resultType, paramList;

	PushProcedureDeclaration(procIdent);
	procedureDeclStart = ftell(cFile);
	Print(cFile, 0, "\n");

	/*generate export status*/
	if (! Trees_Exported(procIdent)) {
		Print(cFile, 0, "static ");
	}

	/*generate return type*/
	procType = Trees_Type(procIdent);
	resultType = Types_ResultType(procType);
	if (resultType != NULL) {
		Generate(resultType, cFile, 0);
		Print(cFile, 0, " ");
	} else {
		Print(cFile, 0, "void ");
	}

	/*generate function identifier*/
	Generate(procIdent, cFile, 0);

	/*generate parameter list*/
	Print(cFile, 0, "(");
	paramList = Types_Parameters(procType);
	if (paramList != NULL) {
		GenerateFormalParameterList(paramList, cFile, 0);
	} else {
		Print(cFile, 0, "void");
	}
	Print(cFile, 0, ")");

	if (Trees_Exported(procIdent) && (hFile != NULL)) {
		CopyText(cFile, procedureDeclStart, ftell(cFile) - procedureDeclStart, hFile);
		Print(hFile, 0, ";\n");
	}

	Print(cFile, 0, "\n{\n");
}


void Generate_ProcedureStatements(Trees_Node stmtSeq)
{
	Print(cFile, 0, "\n");
	Generate(stmtSeq, cFile, 1);
}


void Generate_ReturnClause(Trees_Node exp)
{
	Print(cFile, 1, "return ");
	Generate(exp, cFile, 0);
	Print(cFile, 0, ";\n");
}


void Generate_ProcedureEnd(Trees_Node procIdent)
{
	Print(cFile, 0, "}\n\n");
	PopProcedureDeclaration();
}


/*MODULE GENERATORS*/

static void GenerateInitCalls(int indent)
{
	Trees_Node current;
	const char *module;

	current = importList;
	while (current != NULL) {
		module = Trees_Name(Trees_Left(current));
		Print(cFile, indent, "%s_Init();\n", module);
		current = Trees_Right(current);
	}
}


static int Generated(const char filename[])
{
	FILE *file;
	const char *p;
	int result, n, ch, i;

	assert(filename != NULL);

	result = 0;
	file = Files_Old(filename, FILES_READ);
	p = strrchr(headerComment, ' ');
	if (p != NULL) {
		n = p - headerComment; /*ignore version string*/
		i = 0;
		ch = fgetc(file);
		while ((ch != EOF) && (i < n) && (headerComment[i] == ch)) {
			i++;
			ch = fgetc(file);
		}
		result = (i == n) && (headerComment[i] == ch);
	}
	Files_Close(file);
	return result;
}


static void DeleteTemporaryFiles(void)
{
	if (Files_Exists(tempCFilepath)) {
		Files_Remove(tempCFilepath);
	}
	if (Files_Exists(tempHFilepath)) {
		Files_Remove(tempHFilepath);
	}
}


void Generate_Open(void)
{
	const char *template;

	/*initialize header comment*/
	if ((Version_obnc != NULL) && (Version_obnc[0] != '\0')) {
		template = "/*GENERATED BY OBNC %s*/";
		NEW_ARRAY(headerComment, strlen(template) + strlen(Version_obnc) + 1);
		sprintf(headerComment, "/*GENERATED BY OBNC %s*/", Version_obnc);
	} else {
		template = "/*GENERATED BY OBNC*/";
		NEW_ARRAY(headerComment, strlen(template) + 1);
		strcpy(headerComment, template);
	}

	/*make sure output directory exists*/
	if (! Files_Exists(".obnc")) {
		Files_CreateDir(".obnc");
	}

	/*create temporary C file*/
	sprintf(tempCFilepath, ".obnc/%s.c.%d", Args_ModuleName(), getpid());
	cFile = Files_New(tempCFilepath);

	if (Args_Mode() == ARGS_ENTRY_POINT_MODE) {
		/*entry point modules have no header file*/
		hFile = NULL;
	} else {
		/*create temporary header file*/
		sprintf(tempHFilepath, ".obnc/%s.h.%d", Args_ModuleName(), getpid());
		hFile = Files_New(tempHFilepath);
	}

	atexit(DeleteTemporaryFiles);
}


void Generate_ModuleHeading(void)
{
	Print(cFile, 0, "%s\n\n", headerComment);
	Print(cFile, 0, "#include \"OBNC_.h\"\n");
	if (hFile != NULL) {
		Print(cFile, 0, "#include \"%s.h\"\n", Args_ModuleName());

		Print(hFile, 0, "%s\n\n", headerComment);
		Print(hFile, 0, "#ifndef %s_h\n", Args_ModuleName());
		Print(hFile, 0, "#define %s_h\n\n", Args_ModuleName());
		Print(hFile, 0, "#include \"OBNC_.h\"\n");
	}
}


void Generate_ImportList(Trees_Node list)
{
	Trees_Node moduleNode;
	const char *moduleName;

	importList = list;

	while (list != NULL) {
		moduleNode = Trees_Left(list);
		moduleName = Trees_Name(moduleNode);
		Print(cFile, 0, "#include \"%s.h\"\n", moduleName);
		if (hFile != NULL) {
			Print(hFile, 0, "#include \"%s.h\"\n", moduleName);
		}
		list = Trees_Right(list);
	}
}


void Generate_ModuleStatements(Trees_Node stmtSeq)
{
	if (Args_Mode() == ARGS_ENTRY_POINT_MODE) {
		Print(cFile, 0, "\nint main(int argc, char *argv[])\n");
		Print(cFile, 0, "{\n");
		Print(cFile, 1, "OBNC_argc = argc;\n");
		Print(cFile, 1, "OBNC_argv = argv;\n");
		if (importList != NULL) {
			GenerateInitCalls(1);
		}
		Generate(stmtSeq, cFile, 1);
		Print(cFile, 1, "return 0;\n");
		Print(cFile, 0, "}\n");
	} else {
		Print(cFile, 0, "\nvoid %s_Init(void)\n", Args_ModuleName());
		Print(cFile, 0, "{\n");
		if ((importList != NULL) || (stmtSeq != NULL)) {
			Print(cFile, 1, "static int initialized = 0;\n\n");
			Print(cFile, 1, "if (! initialized) {\n");
			GenerateInitCalls(2);
			Generate(stmtSeq, cFile, 2);
			Print(cFile, 2, "initialized = 1;\n");
			Print(cFile, 1, "}\n");
		}
		Print(cFile, 0, "}\n");

		Print(hFile, 0, "\nvoid %s_Init(void);\n", Args_ModuleName());
	}
}


void Generate_ModuleEnd(void)
{
	if (hFile != NULL) {
		Print(hFile, 0, "\n#endif\n");
	}
}


void Generate_Close(void)
{
	static char cFilepath[PATH_MAX];
	static char hFilepath[PATH_MAX];

	/*close temporary files*/
	Files_Close(cFile);
	if (hFile != NULL) {
		Files_Close(hFile);
	}
	
	/*move temporary C file to permanent C file*/
	sprintf(cFilepath, ".obnc/%s.c", Args_ModuleName());
	if (! Files_Exists(cFilepath) || Generated(cFilepath)) {
		Files_Move(tempCFilepath, cFilepath);
	} else {
		fprintf(stderr, "obnc-compile: error: C file generated by obnc-compile expected, will not overwrite: %s\n", cFilepath);
		exit(EXIT_FAILURE);
	}

	sprintf(hFilepath, ".obnc/%s.h", Args_ModuleName());
	if (Args_Mode() == ARGS_ENTRY_POINT_MODE) {
		/*delete previously generated header file if any*/
		if (Files_Exists(hFilepath)) {
			if (Generated(hFilepath)) {
				Files_Remove(hFilepath);
			} else {
				fprintf(stderr, "obnc-compile: error: header file generated by obnc-compile expected, will not delete: %s\n", hFilepath);
				exit(EXIT_FAILURE);
			}
		}
	} else {
		/*move temporary header file to permanent header file*/
		if (! Files_Exists(hFilepath) || Generated(hFilepath)) {
			Files_Move(tempHFilepath, hFilepath);
		} else {
			fprintf(stderr, "obnc-compile: error: header file generated by obnc-compile expected, will not overwrite: %s\n", hFilepath);
			exit(EXIT_FAILURE);
		}
	}
}


/*GENERAL GENERATOR*/

static void Generate(Trees_Node node, FILE *file, int indent)
{
	int symbol;

	if (node != NULL) {
		symbol = Trees_Symbol(node);
		switch (symbol) {
			case '#':
			case '&':
			case '*':
			case '+':
			case '-':
			case '/':
			case '<':
			case '=':
			case '>':
			case '~':
			case DIV:
			case MOD:
			case OR:
			case GE:
			case LE:
				GenerateOperator(node, file, indent);
				break;
			case BECOMES:
				GenerateAssignment(node, file, indent);
				break;
			case CASE:
				GenerateCaseStatement(node, file, indent);
				break;
			case ELSE:
				Print(file, indent, "else {\n");
				Generate(Trees_Left(node), file, indent + 1);
				Print(file, indent, "}\n");
				break;
			case ELSIF:
				Print(file, indent, "else if (");
				Generate(Trees_Left(node), file, 0);
				Print(file, 0, ") ");
				Generate(Trees_Right(node), file, indent);
				break;
			case FALSE:
				Print(file, indent, "0");
				break;
			case FOR:
				GenerateForStatement(node, file, indent);
				break;
			case IDENT:
				GenerateIdent(node, file, indent);
				break;
			case IF:
				Print(file, indent, "if (");
				Generate(Trees_Left(node), file, 0);
				Print(file, 0, ") ");
				Generate(Trees_Right(node), file, indent);
				break;
			case IN:
				Print(file, indent, "((1 << (");
				Generate(Trees_Left(node), file, indent);
				Print(file, indent, ")) & (");
				Generate(Trees_Right(node), file, indent);
				Print(file, indent, ")) != 0");
				break;
			case INTEGER:
				Print(file, indent, "%d", Trees_Integer(node));
				break;
			case IS:
				GenerateISExpression(Trees_Left(node), Trees_Right(node), file);
				break;
			case NIL:
				Print(file, indent, "0");
				break;
			case POINTER:
				Generate(Trees_Left(node), file, indent);
				break;
			case REAL:
				GenerateReal(Trees_Real(node), file, indent);
				break;
			case REPEAT:
				Print(file, indent, "do {\n");
				Generate(Trees_Left(node), file, indent + 1);
				Print(file, indent, "} while (! (");
				Generate(Trees_Right(node), file, 0);
				Print(file, 0, "));\n");
				break;
			case STRING:
				GenerateString(Trees_String(node), file, indent);
				break;
			case THEN:
				Print(file, 0, "{\n");
				Generate(Trees_Left(node), file, indent + 1);
				Print(file, indent, "}\n");
				Generate(Trees_Right(node), file, indent);
				break;
			case TREES_NOSYM:
				Generate(Trees_Left(node), file, indent);
				Generate(Trees_Right(node), file, indent);
				break;
			case TREES_ABS_PROC:
				if (Types_IsInteger(Trees_Type(Trees_Left(node)))) {
					Print(file, indent, "OBNC_ABS_INT(");
				} else {
					Print(file, indent, "OBNC_ABS_FLT(");
				}
				Generate(Trees_Left(node), file, 0);
				Print(file, 0, ")");
				break;
			case TREES_ASR_PROC:
				Print(file, indent, "OBNC_ASR(");
				Generate(Trees_Left(node), file, 0);
				Print(file, 0, ", ");
				Generate(Trees_Right(node), file, 0);
				Print(file, 0, ")");
				break;
			case TREES_ASSERT_PROC:
				GenerateAssert(node, file, indent);
				break;
			case TREES_BOOLEAN_TYPE:
				Print(file, indent, "int");
				break;
			case TREES_BYTE_TYPE:
				Print(file, indent, "unsigned char");
				break;
			case TREES_CHAR_CONSTANT:
				GenerateChar(Trees_Char(node), file, indent);
				break;
			case TREES_CHAR_TYPE:
				Print(file, indent, "char");
				break;
			case TREES_CHR_PROC:
				Print(file, indent, "OBNC_CHR(");
				Generate(Trees_Left(node), file, 0);
				Print(file, 0, ")");
				break;
			case TREES_DEC_PROC:
				if (Trees_Right(node) == NULL) {
					Print(file, indent, "OBNC_DEC(");
					Generate(Trees_Left(node), file, 0);
					Print(file, 0, ");\n");
				} else {
					Print(file, indent, "OBNC_DEC_N(");
					Generate(Trees_Left(node), file, 0);
					Print(file, 0, ", ");
					Generate(Trees_Right(node), file, 0);
					Print(file, 0, ");\n");
				}
				break;
			case TREES_DESIGNATOR:
				GenerateDesignator(node, file, indent);
				break;
			case TREES_EXCL_PROC:
				Print(file, indent, "OBNC_EXCL(");
				Generate(Trees_Left(node), file, 0);
				Print(file, 0, ", ");
				Generate(Trees_Right(node), file, 0);
				Print(file, 0, ");\n");
				break;
			case TREES_EXP_LIST:
				GenerateExpList(node, file);
				break;
			case TREES_FIELD_LIST_SEQUENCE:
				Generate(Trees_Left(node), file, indent);
				Generate(Trees_Right(node), file, indent);
				break;
			case TREES_FLOOR_PROC:
				Print(file, indent, "OBNC_FLOOR(");
				Generate(Trees_Left(node), file, 0);
				Print(file, 0, ")");
				break;
			case TREES_FLT_PROC:
				Print(file, indent, "OBNC_FLT(");
				Generate(Trees_Left(node), file, 0);
				Print(file, 0, ")");
				break;
			case TREES_INC_PROC:
				if (Trees_Right(node) == NULL) {
					Print(file, indent, "OBNC_INC(");
					Generate(Trees_Left(node), file, 0);
					Print(file, 0, ");\n");
				} else {
					Print(file, indent, "OBNC_INC_N(");
					Generate(Trees_Left(node), file, 0);
					Print(file, 0, ", ");
					Generate(Trees_Right(node), file, 0);
					Print(file, 0, ");\n");
				}
				break;
			case TREES_INCL_PROC:
				Print(file, indent, "OBNC_INCL(");
				Generate(Trees_Left(node), file, 0);
				Print(file, 0, ", ");
				Generate(Trees_Right(node), file, 0);
				Print(file, 0, ");\n");
				break;
			case TREES_INTEGER_TYPE:
				Print(file, indent, "int");
				break;
			case TREES_LEN_PROC:
				GenerateArrayLength(Trees_Left(Trees_Left(node)), Trees_Type(Trees_Left(node)), file);
				break;
			case TREES_LSL_PROC:
				Print(file, indent, "OBNC_LSL(");
				Generate(Trees_Left(node), file, 0);
				Print(file, 0, ", ");
				Generate(Trees_Right(node), file, 0);
				Print(file, 0, ")");
				break;
			case TREES_NEW_PROC:
				GenerateMemoryAllocation(Trees_Left(node), file, indent);
				break;
			case TREES_ODD_PROC:
				Print(file, indent, "OBNC_ODD(");
				Generate(Trees_Left(node), file, 0);
				Print(file, 0, ")");
				break;
			case TREES_ORD_PROC:
				Print(file, indent, "OBNC_ORD(");
				if (Types_IsChar(Trees_Type(Trees_Left(node)))) {
					Print(file, 0, "(unsigned char) ");
				}
				GenerateWithPrecedence(Trees_Left(node), file, 0);
				Print(file, 0, ")");
				break;
			case TREES_PACK_PROC:
				Print(file, indent, "OBNC_PACK(");
				Generate(Trees_Left(node), file, 0);
				Print(file, 0, ", ");
				Generate(Trees_Right(node), file, 0);
				Print(file, 0, ");\n");
				break;
			case TREES_PROCEDURE_CALL:
				GenerateProcedureCall(node, file, indent);
				break;
			case TREES_RANGE_SET:
				GenerateRangeSet(node, file, indent);
				break;
			case TREES_REAL_TYPE:
				Print(file, indent, "double");
				break;
			case TREES_ROR_PROC:
				Print(file, indent, "OBNC_ROR(");
				Generate(Trees_Left(node), file, 0);
				Print(file, 0, ", ");
				Generate(Trees_Right(node), file, 0);
				Print(file, 0, ")");
				break;
			case TREES_SET_CONSTANT:
				Print(file, indent, "0x%Xu", Trees_Set(node));
				break;
			case TREES_SET_TYPE:
				Print(file, indent, "unsigned int");
				break;
			case TREES_SINGLE_ELEMENT_SET:
				GenerateSingleElementSet(node, file, indent);
				break;
			case TREES_STATEMENT_SEQUENCE:
				Generate(Trees_Left(node), file, indent);
				Generate(Trees_Right(node), file, indent);
				break;
			case TREES_UNPK_PROC:
				Print(file, indent, "OBNC_UNPK(");
				Generate(Trees_Left(node), file, 0);
				Print(file, 0, ", ");
				Generate(Trees_Right(node), file, 0);
				Print(file, 0, ");\n");
				break;
			case TRUE:
				Print(file, indent, "1");
				break;
			case WHILE:
				GenerateWhileStatement(node, file, indent);
				break;
			default:
				fprintf(stderr, "Unknown symbol: %d\n", Trees_Symbol(node));
				assert(0);
		}
	}
}
