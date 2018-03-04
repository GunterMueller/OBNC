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

#include "Generate.h"
#include "Config.h"
#include "Files.h"
#include "Maps.h"
#include "Oberon.h"
#include "Trees.h"
#include "Types.h"
#include "Util.h"
#include "../lib/obnc/OBNC.h"
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

static const char *inputModuleName;
static int isEntryPointModule;

static char *headerComment;
static char tempCFilepath[PATH_MAX];
static char tempHFilepath[PATH_MAX];
static FILE *cFile;
static FILE *hFile;

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

static void Indent(FILE *file, int n)
{
	int i;
	
	for (i = 0; i < n; i++) {
		fputc('\t', file);
	}
}

static void Generate(Trees_Node tree, FILE *file, int indent);


/*IDENTIFIER GENERATORS*/

static int ModulePrefixNeeded(Trees_Node ident)
{
	int imported, indirectlyImported, exported, global, isType, isField;

	imported = Trees_Imported(ident);
	indirectlyImported = ! imported && (strchr(Trees_Name(ident), '.') != NULL);
	exported = Trees_Exported(ident);
	global = ! Trees_Local(ident);
	isType = Types_IsType(ident);
	isField = Trees_Kind(ident) == TREES_FIELD_KIND;

	return ! isEntryPointModule && ! imported && ! indirectlyImported && ((exported && ! isField) || (global && isType));
}


static void GenerateLocalProcedurePrefix(Trees_Node ident, struct ProcedureDeclNode *node, FILE *file)
{
	if (node != NULL) {
		GenerateLocalProcedurePrefix(ident, node->next, file);
		fprintf(file, "%s_", Trees_Name(node->procIdent));
	}
}


static void GenerateLocalProcedureIdent(Trees_Node ident, FILE *file, int indent)
{
	assert(procedureDeclStack != NULL);
	Indent(file, indent);
	if (Maps_HasKey(Trees_Name(ident), procedureDeclStack->localProcedures)) {
		GenerateLocalProcedurePrefix(ident, procedureDeclStack, file);
	} else {
		GenerateLocalProcedurePrefix(ident, procedureDeclStack->next, file);
	}
	fprintf(file, "%s_Local", Trees_Name(ident));
}


static void GenerateIdent(Trees_Node ident, FILE *file, int indent)
{
	const char *name;
	char *dotPtr;

	name = Trees_UnaliasedName(ident);
	if ((Trees_Kind(ident) == TREES_TYPE_KIND) && Types_Basic(Trees_Type(ident))) {
		Generate(Trees_Type(ident), file, indent);
	} else if (Trees_Internal(ident)) {
		Indent(file, indent);
		fprintf(file, "%s", name);
	} else if (ModulePrefixNeeded(ident)) {
		Indent(file, indent);
		fprintf(file, "%s_%s_", inputModuleName, name);
	} else if ((Trees_Kind(ident) == TREES_PROCEDURE_KIND) && Trees_Local(ident)) {
		GenerateLocalProcedureIdent(ident, file, indent);
	} else {
		dotPtr = strchr(name, '.');
		if (dotPtr != NULL) {
			*dotPtr = '_';
		}
		Indent(file, indent);
		fprintf(file, "%s_", name);
		if (dotPtr != NULL) {
			*dotPtr = '.';
		}
	}
}


static const char *CurrentDirname(void)
{
	static char dir[PATH_MAX + 1];
	static const char *result = NULL;
	const char *p;

	if (result == NULL) {
		p = getcwd(dir, sizeof dir);
		if (p != NULL) {
			result = strrchr(dir, '/');
			assert(result != NULL);
			result++;
		} else {
			fprintf(stderr, "obnc-compile: cannot get current directory: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
	}
	assert(result != NULL);
	return result;
}


static const char *DirPrefix(void)
{
	static char result[16];
	static int initialized = 0;
	const char *dir;
	int i, j;
	
	if (! initialized) {
		dir = CurrentDirname();
		i = 0;
		j = 0;
		while ((dir[i] != '\0') && (j < LEN(result) - 2)) {
			if (((j == 0) && isalpha(dir[i])) || ((j > 0) && isalnum(dir[i]))) {
				result[j] = dir[i];
				j++;
			}
			i++;
		}
		result[j] = '_';
		result[j + 1] = '\0';
		initialized = 1;
	}
	return result;
}


static void GenerateObjectFileSymbolDefinitions(Trees_Node identList, const char *suffix, FILE *file, int indent)
{
	const char *dirPrefix;
	Trees_Node ident;

	/*NOTE: To prevent potential name collisions at link time when two modules with the same name (from different directories) are combined, we add a directory prefix to object file symbols with external linkage.*/

	dirPrefix = DirPrefix();
	if (strcmp(dirPrefix, "") != 0) {
		while (identList != NULL) {
			ident = Trees_Left(identList);
			Indent(file, indent);
			fprintf(file, "#define ");
			GenerateIdent(ident, file, 0);
			fprintf(file,  "%s %s", suffix, dirPrefix);
			GenerateIdent(ident, file, 0);
			fprintf(file, "%s\n", suffix);
			identList = Trees_Right(identList);
		}
	}
}


/*LITERAL GENERATORS*/

static void GenerateReal(OBNC_LONGR double value, FILE *file)
{
	int formattedAsInteger;
#ifdef OBNC_CONFIG_USE_LONG_REAL
	char buffer[LDBL_DIG + 10]; /*LDBL_DIG + strlen("-") + strlen(".") + strlen("e+9999") + strlen("L") + 1*/

	sprintf(buffer, "%.*" OBNC_REAL_MOD_W "g", LDBL_DIG, value);
#else
	char buffer[DBL_DIG + 8]; /*DBL_DIG + strlen("-") + strlen(".") + strlen("e+999") + 1*/
	
	sprintf(buffer, "%.*g", DBL_DIG, value);
#endif
	if (strcmp(buffer, "inf") == 0) {
		fprintf(file, "(1.0 / 0.0)");
	} else if (strcmp(buffer, "-inf") == 0) {
		fprintf(file, "(-1.0 / 0.0)");
	} else if (strcmp(buffer, "nan") == 0) {
		fprintf(file, "(0.0 / 0.0)");
	} else if (strcmp(buffer, "-nan") == 0) {
		fprintf(file, "(0.0 / 0.0)");
	} else {
		formattedAsInteger = (strchr(buffer, '.') == NULL) && (strchr(buffer, 'e') == 0);
		if (formattedAsInteger) {
			strcat(buffer, ".0");
		}
#ifdef OBNC_CONFIG_USE_LONG_REAL
		strcat(buffer, "L");
#endif
		fprintf(file, "%s", buffer);
	}
}


static void GenerateString(const char s[], FILE *file)
{
	int i;
	
	fputc('"', file);
	i = 0;
	while (s[i] != '\0') {
		if ((s[i] >= 0) && ((unsigned char) s[i] <= 127)) {
			if (isprint(s[i])) {
				if ((s[i] == '"') || (s[i] == '\\')) {
					fputc('\\', file);
				}
				fputc(s[i], file);
			} else {
				fprintf(file, "\" \"\\x%02x\" \"", (unsigned char) s[i]);
			}
		} else {
			fputc(s[i], file);
		}
		i++;
	}
	fputc('"', file);
}


static void GenerateChar(char ch, FILE *file)
{
	switch (ch) {
		case '\'':
		case '\\':
			fprintf(file, "'\\%c'", ch);
			break;
		default:
			if (isprint(ch)) {
				fprintf(file, "'%c'", ch);
			} else {
				fprintf(file, "'\\x%02x'", (unsigned char) ch);
			}
	}
}


/*CONSTANT DECLARATION GENERATORS*/

void Generate_ConstDeclaration(Trees_Node ident)
{
	if (Trees_Exported(ident)) {
		/*add constant declaration to header file to provide access to it from hand-written C file*/
		fprintf(hFile, "\n#define ");
		Generate(ident, hFile, 0);
		fprintf(hFile, " ");
		Generate(Trees_Value(ident), hFile, 0);
		fprintf(hFile, "\n");
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
				Indent(file, indent);
				fprintf(file, "struct ");
				Generate(baseType, file, 0);
			} else {
				assert(Trees_Symbol(pointerBaseType) == IDENT);
				Generate(pointerBaseType, file, indent);
			}
		} else {
			Generate(baseType, file, indent);
		}
		fprintf(file, " base;\n");
	} else if (fieldListSeq == NULL) {
		Indent(file, indent);
		fprintf(file, "char dummy;\n");
	}
	while (fieldListSeq != NULL) {
		identList = Trees_Left(fieldListSeq);
		GenerateDeclaration(Trees_NewNode(TREES_NOSYM, identList, NULL), file, indent);
		fieldListSeq = Trees_Right(fieldListSeq);
	}
}


static void GenerateRecord(Trees_Node type, Trees_Node declIdent, FILE *file, int indent)
{
	Indent(file, indent);
	fprintf(file, "struct ");
	if ((declIdent != NULL) && (Trees_Kind(declIdent) == TREES_TYPE_KIND)) {
		Generate(declIdent, file, 0);
		fprintf(file, " ");
	}
	fprintf(file, "{\n");
	GenerateFields(type, file, indent + 1);
	Indent(file, indent);
	fprintf(file, "}");
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


static void GenerateStorageClassSpecifier(Trees_Node ident, FILE *file)
{
	if (Trees_Kind(ident) == TREES_TYPE_KIND) {
		fprintf(file, "typedef ");
	} else if ((Trees_Kind(ident) == TREES_VARIABLE_KIND) && ! Trees_Local(ident)) {
		if (file == hFile) {
			fprintf(file, "extern ");
		} else if (! Trees_Exported(ident)) {
			fprintf(file, "static ");
		}
	}
}


static int TypePossiblyIncomplete(Trees_Node type, Trees_Node ident)
{
	return ((Trees_Symbol(Trees_Type(ident)) == POINTER)
			&& ((Trees_Kind(ident) == TREES_TYPE_KIND) || (Trees_Kind(ident) == TREES_FIELD_KIND))
			&& ((Trees_Type(type) == NULL) || Types_IsRecord(type)))
		|| (type == declaredTypeIdent);
}


static void GenerateTypeSpecifier(Trees_Node ident, Trees_Node type, FILE *file, int indent)
{
	Trees_Node elementType;
	
	switch (Trees_Symbol(type)) {
		case IDENT:
			if (TypePossiblyIncomplete(type, ident)) {
				fprintf(file, "struct ");
			}
			Generate(type, file, 0);
			break;
		case ARRAY:
			elementType = Types_ElementType(type);
			while (Types_IsArray(elementType)) {
				elementType = Types_ElementType(elementType);
			}
			GenerateTypeSpecifier(ident, elementType, file, indent);
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
				fprintf(file, "void");
			}
			break;
		default:
			Generate(type, file, indent);
	}
}


static void GenerateFormalParameterList(Trees_Node paramList, FILE *file);

static void GenerateDeclarator(Trees_Node ident, FILE *file)
{
	Trees_Node type, firstNonArrayType, resultType;
	int dim;

	type = Trees_Type(ident);
	firstNonArrayType = type;
	while (Trees_Symbol(firstNonArrayType) == ARRAY) {
		firstNonArrayType = Types_ElementType(firstNonArrayType);
	}
	if ((Trees_Symbol(firstNonArrayType) == POINTER)
			|| (Types_IsPointer(firstNonArrayType) && TypePossiblyIncomplete(firstNonArrayType, ident))) {
		fprintf(file, "*");
	} else if (Trees_Symbol(firstNonArrayType) == PROCEDURE) {
		resultType = Types_ResultType(firstNonArrayType);
		if ((declaredTypeIdent != NULL) && (resultType == declaredTypeIdent)) {
			fprintf(file, "*");
		}
		fprintf(file, "(*");
	}
	Generate(ident, file, 0);
	if (Trees_Symbol(type) == ARRAY) {
		/*NOTE: Since multi-dimensional open array parameters must be generated as one-dimensional arrays, we must also generate (non-open) multi-dimensional arrays as one-dimensional arrays to enable parameter substitution with correct type.*/
		fprintf(file, "[");
		dim = -1;
		do {
			dim++;
			if (dim > 0) {
				fprintf(file, " * ");
			}
			fprintf(file, "%" OBNC_INT_MOD "d", Trees_Integer(Types_ArrayLength(type)));
			type = Types_ElementType(type);
		} while (Types_IsArray(type));
		fprintf(file, "]");
	}
	if (Trees_Symbol(firstNonArrayType) == PROCEDURE) {
		fprintf(file, ")(");
		if (Types_Parameters(type) != NULL) {
			GenerateFormalParameterList(Types_Parameters(firstNonArrayType), file);
		} else {
			fprintf(file, "void");
		}
		fprintf(file, ")");
	}
}


static void SearchPointersAndProceduresRec(Trees_Node type, int *hasPointer, int *hasProcedure)
{
	Trees_Node recordBaseType, fieldListSeq, fieldList, ident;
	
	if ((type != NULL) && ! (*hasPointer && *hasProcedure)) {
		switch (Trees_Symbol(Types_Structure(type))) {
			case	ARRAY:
				SearchPointersAndProceduresRec(Types_ElementType(type), hasPointer, hasProcedure);
				break;
			case RECORD:
				recordBaseType = Types_RecordBaseType(type);
				if (recordBaseType != NULL) {
					SearchPointersAndProceduresRec(
						Types_Descriptor(recordBaseType), hasPointer, hasProcedure);
				}
				fieldListSeq = Types_Fields(type);
				while ((fieldListSeq != NULL) && ! (*hasPointer && *hasProcedure)) {
					fieldList = Trees_Left(fieldListSeq);
					ident = Trees_Left(fieldList);
					SearchPointersAndProceduresRec(Trees_Type(ident), hasPointer, hasProcedure);
					fieldListSeq = Trees_Right(fieldListSeq);
				}
				break;
			case POINTER:
				*hasPointer = 1;
				break;
			case PROCEDURE:
				*hasProcedure = 1;
				break;
		}
	}
}


static void SearchPointersAndProcedures(Trees_Node type, int *hasPointer, int *hasProcedure)
{
	*hasPointer = 0;
	*hasProcedure = 0;
	SearchPointersAndProceduresRec(type, hasPointer, hasProcedure);
}


static void GenerateDeclaration(Trees_Node declaration, FILE *file, int indent)
{
	Trees_Node identList, firstIdent, ident;
	int hasPointer, hasProcedure;

	if (Trees_Symbol(Trees_Left(declaration)) == IDENT) {
		identList = Trees_NewNode(TREES_IDENT_LIST, Trees_Left(declaration), NULL);
	} else {
		identList = Trees_Left(declaration);
	}
	firstIdent = Trees_Left(identList);

	Indent(file, indent);
	GenerateStorageClassSpecifier(firstIdent, file);
	GenerateTypeSpecifier(firstIdent, Trees_Type(firstIdent), file, indent);
	fprintf(file, " ");

	do {
		ident = Trees_Left(identList);
		GenerateDeclarator(ident, file);

		if ((Trees_Kind(firstIdent) == TREES_VARIABLE_KIND) && Trees_Local(firstIdent) && (file != hFile)) {
			switch (Trees_Symbol(Types_Structure(Trees_Type(firstIdent)))) {
				case ARRAY:
				case RECORD:
					SearchPointersAndProcedures(Trees_Type(firstIdent), &hasPointer, &hasProcedure);
					if (hasPointer || hasProcedure) {
						fprintf(file, " = {0}");
					}
					break;
				case POINTER:
				case PROCEDURE:
					fprintf(file, " = 0");
					break;
			}
		}
		if (Trees_Right(identList) != NULL) {
			fprintf(file, ", ");
		}
		identList = Trees_Right(identList);
	} while (identList != NULL);

	fprintf(file, ";\n");
}


static void GenerateTypeIDs(Trees_Node type)
{
	Trees_Node baseType;

	baseType = Types_RecordBaseType(type);
	if (baseType != NULL) {
		GenerateTypeIDs(baseType);
		fprintf(cFile, ", ");
	}
	fprintf(cFile, "&");
	Generate(TypeDescIdent(type), cFile, 0);
	fprintf(cFile, "id");
}


static void GenerateHeapTypeDecl(Trees_Node typeIdent, FILE* file, int indent)
{
	Indent(file, indent);
	fprintf(file, "struct ");
	Generate(typeIdent, file, 0);
	fprintf(file, "Heap {\n");
	Indent(file, indent + 1);
	fprintf(file, "const OBNC_Td *td;\n");
	Indent(file, indent + 1);
	fprintf(file, "struct ");
	Generate(typeIdent, file, 0);
	Indent(file, indent);
	fprintf(file, " fields;\n");
	Indent(file, indent);
	fprintf(file, "};\n");
}


static void GenerateTypeDescDecl(Trees_Node typeIdent, int indent)
{
	int extensionLevel;
	Trees_Node identList;
	const char *storageClass;


	/*generate type descriptor (type ID used for its unique address only)*/
	extensionLevel = Types_ExtensionLevel(typeIdent);
	if (ModulePrefixNeeded(typeIdent)) {
		identList = Trees_NewNode(TREES_NOSYM, typeIdent, NULL);
		
		fprintf(hFile, "\n");
		GenerateObjectFileSymbolDefinitions(identList, "id", hFile, 0);
		Indent(hFile, indent);
		fprintf(hFile, "extern const int ");
		Generate(typeIdent, hFile, 0);
		fprintf(hFile, "id;\n\n");

		GenerateObjectFileSymbolDefinitions(identList, "ids", hFile, 0);
		Indent(hFile, indent);
		fprintf(hFile, "extern const int *const ");
		Generate(typeIdent, hFile, 0);
		fprintf(hFile, "ids[%d];\n\n", extensionLevel + 1);

		GenerateObjectFileSymbolDefinitions(identList, "td", hFile, 0);
		Indent(hFile, indent);
		fprintf(hFile, "extern const OBNC_Td ");
		Generate(typeIdent, hFile, 0);
		fprintf(hFile, "td;\n");

		storageClass = "";
	} else {
		storageClass = "static ";
	}
	fprintf(cFile, "\n");
	Indent(cFile, indent);
	fprintf(cFile, "%sconst int ", storageClass);
	Generate(typeIdent, cFile, 0);
	fprintf(cFile, "id;\n");

	Indent(cFile, indent);
	fprintf(cFile, "%sconst int *const ", storageClass);
	Generate(typeIdent, cFile, 0);
	fprintf(cFile, "ids[%d] = {", extensionLevel + 1);
	GenerateTypeIDs(typeIdent);
	fprintf(cFile, "};\n");

	Indent(cFile, indent);
	fprintf(cFile, "%sconst OBNC_Td ", storageClass);
	Generate(typeIdent, cFile, 0);
	fprintf(cFile, "td = {");
	Generate(typeIdent, cFile, 0);
	fprintf(cFile, "ids, %d};\n", extensionLevel + 1);
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
		fprintf(hFile, "\n");
		GenerateDeclaration(declaration, hFile, indent);
	} else {
		if (! Trees_Local(ident)) {
			fprintf(cFile, "\n");
		}
		GenerateDeclaration(declaration, cFile, indent);
	}
	declaredTypeIdent = NULL;
	if ((Trees_Symbol(type) == RECORD)
			|| ((Trees_Symbol(type) == POINTER) && (Trees_Symbol(Types_PointerBaseType(type)) == RECORD))) {
		typeDescIdent = TypeDescIdent(ident);

		if (modulePrefixNeeded) {
			fprintf(hFile, "\n");
			GenerateHeapTypeDecl(typeDescIdent, hFile, 0);
		} else {
			fprintf(cFile, "\n");
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
		fprintf(cFile, "\n");
	}
	if (HasExportedIdent(identList) && ! isEntryPointModule) {
		fprintf(hFile, "\n");
		if (NameEquivalenceNeeded(type)) {
			/*declare anonymous type in header file*/
			newTypeNameLen = strlen(inputModuleName) + strlen("_T") + DigitCount(typeCounter) + 1;
			NEW_ARRAY(newTypeName, newTypeNameLen);
			sprintf(newTypeName, "%s_T%d", inputModuleName, typeCounter);

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
			GenerateObjectFileSymbolDefinitions(identList, "", hFile, indent);
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
			GenerateObjectFileSymbolDefinitions(exportedIdents, "", hFile, indent);
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

static Trees_Node EntireVar(Trees_Node var)
{
	assert(Trees_Symbol(var) == TREES_DESIGNATOR);
	return Trees_Left(var);
}


static Trees_Node NextSelector(Trees_Node var)
{
	return Trees_Right(var);
}


static Trees_Node PrevSelector(Trees_Node var, Trees_Node selector)
{
	Trees_Node p;

	assert(Trees_Symbol(var) == TREES_DESIGNATOR);
	assert(selector != NULL);
	
	p = Trees_Right(var);
	while ((p != NULL) && (NextSelector(p) != selector)) {
		p = NextSelector(p);
	}
	return p;
}


static int IsVarParam(Trees_Node var)
{
	return (Trees_Kind(EntireVar(var)) == TREES_VAR_PARAM_KIND) && (NextSelector(var) == NULL);
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


static void PrintCOperator(Trees_Node opNode, FILE *file)
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
			fprintf(file, "!=");
			break;
		case '&':
			fprintf(file, "&&");
			break;
		case '*':
			if (leftType == TREES_SET_TYPE) {
				fprintf(file, "&");
			} else {
				fprintf(file, "*");
			}
			break;
		case '+':
			if ((leftType == TREES_SET_TYPE) && (rightType >= 0)) {
				fprintf(file, "|");
			} else {
				fprintf(file, "+");
			}
			break;
		case '-':
			if (leftType == TREES_SET_TYPE) {
				if (rightType == -1) {
					fprintf(file, "~");
				} else {
					fprintf(file, "& ~");
				}
			} else {
				fprintf(file, "-");
			}
			break;
		case '/':
			if (leftType == TREES_SET_TYPE) {
				fprintf(file, "^");
			} else {
				fprintf(file, "/");
			}
			break;
		case '<':
			fprintf(file, "<");
			break;
		case '=':
			fprintf(file, "==");
			break;
		case '>':
			fprintf(file, ">");
			break;
		case '~':
			fprintf(file, "! ");
			break;
		case OR:
			fprintf(file, "||");
			break;
		case GE:
			fprintf(file, ">=");
			break;
		case LE:
			fprintf(file, "<=");
			break;
		default:
			assert(0);
	}
}


static void GenerateArrayLength(Trees_Node var, Trees_Node arrayType, int dim, FILE *file)
{
	assert(Trees_Symbol(var) == TREES_DESIGNATOR);
	assert(Types_IsArray(arrayType));

	if (Types_IsOpenArray(arrayType)) {
		Generate(EntireVar(var), file, 0);
		fprintf(file, "len");
		if (dim > 0) {
			fprintf(file, "%d", dim);
		}
	} else {
		fprintf(file, "%" OBNC_INT_MOD "d", Trees_Integer(Types_ArrayLength(arrayType)));
	}
}


static int ArrayDimension(Trees_Node arrayVar)
{
	Trees_Node selector;
	int dim;
	
	assert(Types_IsArray(Trees_Type(arrayVar)));
	
	dim = 0;
	selector = Trees_Right(arrayVar);
	while (selector != NULL) {
		if (Trees_Symbol(selector) == '[') {
			dim++;
		} else {
			dim = 0;
		}
		selector = NextSelector(selector);
	}
	return dim;	
}


static void GenerateWithPrecedence(Trees_Node exp, FILE *file)
{
	if (Trees_IsLeaf(exp)
			|| (Trees_Symbol(exp) == TREES_DESIGNATOR)
			|| IsProcedureCall(Trees_Symbol(exp))) {
		Generate(exp, file, 0);
	} else {
		fprintf(file, "(");
		Generate(exp, file, 0);
		fprintf(file, ")");
	}
}


static void GenerateNonScalarOperation(Trees_Node opNode, FILE *file, int indent)
{
	Trees_Node operands[2];
	Trees_Node types[2];
	int i;

	operands[0] = Trees_Left(opNode);
	operands[1] = Trees_Right(opNode);
	types[0] = Types_Structure(Trees_Type(operands[0]));
	types[1] = Types_Structure(Trees_Type(operands[1]));

	switch (Trees_Symbol(opNode)) {
		case '=':
		case '#':
		case '<':
		case LE:
		case '>':
		case GE:
			Indent(file, indent);
			fprintf(file, "OBNC_CMP(");
			for (i = 0; i < 2; i++) {
				if (i > 0) {
					fprintf(file, ", ");
				}
				if (Types_IsArray(types[i]) && (ArrayDimension(operands[i]) > 0)) {
					fprintf(file, "&");
				}
				GenerateWithPrecedence(operands[i], file);
				fprintf(file, ", ");
				if (Trees_Symbol(types[i]) == TREES_STRING_TYPE) {
					fprintf(file, "%lu", (long unsigned int) strlen(Trees_String(operands[i])) + 1);
				} else {
					GenerateArrayLength(operands[i], types[i], ArrayDimension(operands[i]), file);
				}
			}
			fprintf(file, ") ");
			PrintCOperator(opNode, file);
			fprintf(file, " 0");
			break;
		default:
			assert(0);
	}
}


static Trees_Node LastSelector(Trees_Node var)
{
	Trees_Node result;
	
	assert(var != NULL);
	assert(Trees_Symbol(var) == TREES_DESIGNATOR);
	
	result = Trees_Right(var);
	while ((result != NULL) && (Trees_Right(result) != NULL)) {
		result = Trees_Right(result);
	}
	return result;
}


static void GenerateTypeDescExp(Trees_Node var, FILE *file, int indent)
{
	Trees_Node type = Trees_Type(var);
	Trees_Node lastSelector = LastSelector(var);
	
	if (Types_IsPointer(type)) {
		fprintf(file, "OBNC_TD(");
		Generate(var, file, 0);
		fprintf(file, ", struct ");
		Generate(TypeDescIdent(type), file, 0);
		fprintf(file, "Heap)");
	} else if ((lastSelector != NULL) && Types_IsPointer(Trees_Type(lastSelector))) {
		fprintf(file, "OBNC_TD(&(");
		Generate(var, file, 0);
		fprintf(file, "), struct ");
		Generate(TypeDescIdent(type), file, 0);
		fprintf(file, "Heap)");		
	} else {
		assert(Types_IsRecord(type));
		if (IsVarParam(var)) {
			GenerateIdent(EntireVar(var), file, indent);
			fprintf(file, "td");
		} else {
			fprintf(file, "&");
			GenerateIdent(TypeDescIdent(type), file, 0);
			fprintf(file, "td");
		}
	}
}


static void GenerateISExpression(Trees_Node var, Trees_Node type, FILE *file)
{
	int isPointer;

	isPointer = Types_IsPointer(Trees_Type(var));
	if (isPointer) {
		fprintf(file, "((void) OBNC_PT(");
		Generate(var, file, 0);
		fprintf(file, "), ");
	}
	fprintf(file, "OBNC_IS(");
	GenerateTypeDescExp(var, file, 0);
	fprintf(file, ", &");
	Generate(TypeDescIdent(type), file, 0);
	fprintf(file, "id, %d)", Types_ExtensionLevel(type));
	if (isPointer) {
		fprintf(file, ")");
	}
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


static void GenerateOperator(Trees_Node opNode, FILE *file)
{
	Trees_Node leftOperand, rightOperand, leftType, rightType;
	int opSym;

	leftOperand = Trees_Left(opNode);
	rightOperand = Trees_Right(opNode);
	opSym = Trees_Symbol(opNode);

	if (Trees_Right(opNode) == NULL) {
		/*unary operator*/
		PrintCOperator(opNode, file);
		GenerateWithPrecedence(leftOperand, file);
	} else {
		/*binary operator*/
		leftType = Trees_Type(leftOperand);
		rightType = Trees_Type(rightOperand);

		if ((Types_IsString(leftType) || Types_IsCharacterArray(leftType))
				&& (Types_IsString(rightType) || Types_IsCharacterArray(rightType))) {
			GenerateNonScalarOperation(opNode, file, 0);
		} else {
			switch (opSym) {
				case DIV:
				case MOD:
					if (opSym == DIV) {
						if (ContainsProcedureCall(leftOperand) || ContainsProcedureCall(rightOperand)) {
							fprintf(file, "OBNC_Div(");
						} else {
							fprintf(file, "OBNC_DIV(");
						}
					} else {
						if (ContainsProcedureCall(leftOperand) || ContainsProcedureCall(rightOperand)) {
							fprintf(file, "OBNC_Mod(");
						} else {
							fprintf(file, "OBNC_MOD(");
						}
					}
					Generate(leftOperand, file, 0);
					fprintf(file, ", ");
					Generate(rightOperand, file, 0);
					fprintf(file, ")");
					break;
				case '<':
				case LE:
				case '>':
				case GE:
					if (Types_IsChar(Trees_Type(leftOperand))) {
						fprintf(file, "(unsigned char) ");
					}
					GenerateWithPrecedence(leftOperand, file);
					fprintf(file, " ");
					PrintCOperator(opNode, file);
					fprintf(file, " ");
					if (Types_IsChar(Trees_Type(rightOperand))) {
						fprintf(file, "(unsigned char) ");
					}
					GenerateWithPrecedence(rightOperand, file);
					break;
				default:
					if (Types_IsPointer(leftType) && (Trees_Symbol(leftOperand) != NIL) && ! Types_Same(leftType, rightType) && (Trees_Symbol(rightOperand) != NIL)) {
						if (Types_Extends(leftType, rightType)) {
							GenerateWithPrecedence(leftOperand, file);
							fprintf(file, " ");
							PrintCOperator(opNode, file);
							fprintf(file, " (");
							Generate(leftType, file, 0);
							fprintf(file, ") ");
							GenerateWithPrecedence(rightOperand, file);
						} else {
							fprintf(file, "(");
							Generate(rightType, file, 0);
							fprintf(file, ") ");
							GenerateWithPrecedence(leftOperand, file);
							fprintf(file, " ");
							PrintCOperator(opNode, file);
							fprintf(file, " ");
							GenerateWithPrecedence(rightOperand, file);
						}
					} else {
						GenerateWithPrecedence(leftOperand, file);
						fprintf(file, " ");
						PrintCOperator(opNode, file);
						fprintf(file, " ");
						GenerateWithPrecedence(rightOperand, file);
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


static void GenerateArrayIndex(Trees_Node var, Trees_Node indexSelector, FILE *file)
{
	Trees_Node arrayType, indexExp, selector, currArrayType, currArrayType1;
	int trapNeeded, dim, dim1;

	arrayType = Trees_Type(indexSelector);
	assert(Types_IsArray(arrayType));

	selector = indexSelector;
	currArrayType = arrayType;
	dim = 0;
	do {
		if (dim > 0) {
			fprintf(file, " + ");
		}
		indexExp = Trees_Left(selector);
		trapNeeded = Types_IsOpenArray(arrayType) || ! IsConstExpression(indexExp);
		if (trapNeeded) {
			if (ContainsProcedureCall(indexExp)) {
				fprintf(file, "OBNC_IT1(");
			} else {
				fprintf(file, "OBNC_IT(");
			}
		}
		Generate(indexExp, file, 0);
		if (trapNeeded) {
			fprintf(file, ", ");
			GenerateArrayLength(var, currArrayType, dim, file);
			fprintf(file, ")");
		}
		currArrayType1 = Types_ElementType(currArrayType);
		dim1 = dim + 1;
		while ((currArrayType1 != NULL) && Types_IsArray(currArrayType1)) {
			fprintf(file, " * ");
			GenerateArrayLength(var, currArrayType1, dim1, file);
			currArrayType1 = Types_ElementType(currArrayType1);
			dim1++;
		}
		selector = NextSelector(selector);
		currArrayType = Types_ElementType(currArrayType);
		dim++;
	} while ((selector != NULL) && (Trees_Symbol(selector) == '['));	
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
		fprintf(file, "(*");
		Generate(ident, file, 0);
		fprintf(file, ")");
	} else {
		Generate(ident, file, 0);
	}
}


static void GenerateDesignatorRec(Trees_Node des, Trees_Node selector, FILE *file)
{
	Trees_Node field, fieldIdent, fieldBaseType, typeIdent, prevSelector, firstDimSelector;
	int castNeeded;
	
	if (selector == NULL) {
		if ((caseVariable != NULL) && (caseLabelType != NULL) && (EntireVar(des) == caseVariable) && ! Types_Same(Trees_Type(caseVariable), caseLabelType)) {
			fprintf(file, "(*((");
			Generate(caseLabelType, file, 0);
			fprintf(file, " *) &");
			GenerateDesignatorVar(EntireVar(des), file);
			fprintf(file, "))");
		} else {
			GenerateDesignatorVar(EntireVar(des), file);
		}
	} else {
		switch (Trees_Symbol(selector)) {
			case '[':
				firstDimSelector = selector;
				prevSelector = PrevSelector(des, selector);
				while ((prevSelector != NULL) && (Trees_Symbol(prevSelector) == '[')) {
					firstDimSelector = prevSelector;
					prevSelector = PrevSelector(des, prevSelector);
				}
				GenerateDesignatorRec(des, prevSelector, file);
				fprintf(file, "[");
				GenerateArrayIndex(des, firstDimSelector, file);
				fprintf(file, "]");
				break;
			case '.':
				field = Trees_Left(selector);
				Types_GetFieldIdent(Trees_Name(field), Trees_Type(selector), Trees_Imported(EntireVar(des)), &fieldIdent, &fieldBaseType);
				castNeeded = ! Types_Same(fieldBaseType, Trees_Type(selector));
				if (castNeeded) {
					fprintf(file, "(*((");
					Generate(fieldBaseType, file, 0);
					if (Types_IsRecord(fieldBaseType)) {
						fprintf(file, " *");
					}
					fprintf(file, ") &");
				}
				GenerateDesignatorRec(des, PrevSelector(des, selector), file);
				if (castNeeded) {
					fprintf(file, "))");
				}
				fprintf(file, ".");
				Generate(Trees_Left(selector), file, 0);
				break;
			case '^':
				fprintf(file, "(*OBNC_PT(");
				GenerateDesignatorRec(des, PrevSelector(des, selector), file);
				fprintf(file, "))");
				break;
			case '(':
				typeIdent = Trees_Left(selector);

				fprintf(file, "(*((");
				Generate(typeIdent, file, 0);
				if (Types_IsRecord(typeIdent)) {
					fprintf(file, "*) OBNC_RTT(&(");
				} else {
					fprintf(file, "*) OBNC_PTT(&(");
				}
				GenerateDesignatorRec(des, PrevSelector(des, selector), file);
				fprintf(file, "), ");
				if (Types_IsRecord(typeIdent)) {
					if (IsVarParam(des) && (selector == NextSelector(des))) {
						GenerateIdent(EntireVar(des), file, 0);
						fprintf(file, "td");
					} else {
						fprintf(file, "&");
						GenerateIdent(TypeDescIdent(Trees_Type(selector)), file, 0);
						fprintf(file, "td");
					}
				} else {
					assert(Types_IsPointer(typeIdent));
					fprintf(file, "OBNC_TD(");
					GenerateDesignatorRec(des, PrevSelector(des, selector), file);
					fprintf(file, ", struct ");
					Generate(TypeDescIdent(Trees_Type(selector)), file, 0);
					fprintf(file, "Heap)");
				}
				fprintf(file, ", &");
				Generate(TypeDescIdent(typeIdent), file, 0);
				fprintf(file, "id, %d)))", Types_ExtensionLevel(typeIdent));				
				break;
			default:
				assert(0);
		}
	}
}


static void GenerateDesignator(Trees_Node des, FILE *file)
{
	GenerateDesignatorRec(des, LastSelector(des), file);
}


static void GenerateSingleElementSet(Trees_Node node, FILE *file)
{
	fprintf(file, "(0x1u << ");
	GenerateWithPrecedence(Trees_Left(node), file);
	fprintf(file, ")");
}


static void GenerateRangeSet(Trees_Node node, FILE *file)
{
	Trees_Node low = Trees_Left(node);
	Trees_Node high = Trees_Right(node);

	if (ContainsProcedureCall(low) || ContainsProcedureCall(high)) {
		fprintf(file, "OBNC_Range(");
	} else {
		fprintf(file, "OBNC_RANGE(");
	}
	Generate(low, file, 0);
	fprintf(file, ", ");
	Generate(high, file, 0);
	fprintf(file, ")");
}


static void GenerateExpList(Trees_Node expList, FILE *file)
{
	Trees_Node exp, tail;

	exp = Trees_Right(expList);
	Generate(exp, file, 0);
	tail = Trees_Right(expList);
	if (tail != NULL) {
		fprintf(file, ", ");
		Generate(tail, file, 0);
	}
}


/*STATEMENT GENERATORS*/

static void GenerateArrayAssignment(Trees_Node source, Trees_Node target, FILE *file, int indent)
{
	Trees_Node sourceType, targetType;

	assert(Trees_Symbol(target) == TREES_DESIGNATOR);

	sourceType = Trees_Type(source);
	targetType = Types_Structure(Trees_Type(target));
	assert(Trees_Symbol(targetType) == ARRAY);

	if (Types_IsOpenArray(sourceType) || Types_IsOpenArray(targetType)) {
		Indent(file, indent);
		fprintf(file, "OBNC_AAT(");
		if (Trees_Symbol(source) == STRING) {
			fprintf(file, "%lu", (long unsigned int) strlen(Trees_String(source)) + 1);
		} else {
			GenerateArrayLength(source, sourceType, ArrayDimension(source), file);
		}
		fprintf(file, ", ");
		GenerateArrayLength(target, targetType, ArrayDimension(target), file);
		fprintf(file, ");\n");
	}
	Indent(file, indent);
	fprintf(file, "OBNC_COPY_ARRAY(");
	if (Types_IsArray(sourceType) && (ArrayDimension(source) > 0)) {
		fprintf(file, "&");
	}
	GenerateWithPrecedence(source, file);
	fprintf(file, ", ");
	if (Types_IsArray(targetType) && (ArrayDimension(target) > 0)) {
		fprintf(file, "&");
	}
	GenerateWithPrecedence(target, file);
	fprintf(file, ", ");
	if (Trees_Symbol(source) == STRING) {
		fprintf(file, "%lu", (long unsigned int) strlen(Trees_String(source)) + 1);
	} else {
		GenerateArrayLength(source, sourceType, ArrayDimension(source), file);
	}
	fprintf(file, ");\n");
}


static void GenerateRecordAssignment(Trees_Node source, Trees_Node target, FILE *file, int indent)
{
	Trees_Node sourceType, targetType;

	sourceType = Trees_Type(source);
	targetType = Trees_Type(target);

	if (IsVarParam(target)) {
		Indent(file, indent);
		fprintf(file, "OBNC_RAT(");
		GenerateTypeDescExp(source, file, 0);
		fprintf(file, ", ");
		GenerateTypeDescExp(target, file, 0);
		fprintf(file, ");\n");
	}
	if (Types_Same(sourceType, targetType) && ! IsVarParam(target)) {
		GenerateDesignator(target, file);
		fprintf(file, " = ");
		Generate(source, file, 0);
		fprintf(file, ";\n");
	} else {
		Generate(target, file, indent);
		fprintf(file, " = ");
		if (! Types_Same(sourceType, targetType)) {
			assert(Types_Extends(targetType, sourceType));
			fprintf(file, "*(");
			Generate(targetType, file, 0);
			fprintf(file, " *) &");
		}
		Generate(source, file, 0);
		fprintf(file, ";\n");
	}
}


static int CastNeeded(Trees_Node sourceType, Trees_Node targetType)
{
	return (Types_IsByte(targetType) && ! Types_IsByte(sourceType))
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
			Indent(file, indent);
			GenerateDesignator(target, file);
			fprintf(file, " = ");
			if (CastNeeded(sourceType, targetType)) {
				fprintf(file, "(");
				Generate(targetType, file, 0);
				fprintf(file, ") ");
			}
			GenerateWithPrecedence(source, file);
			fprintf(file, ";\n");
	}
}


static void GenerateProcedureCall(Trees_Node call, FILE *file, int indent)
{
	Trees_Node designator, designatorTypeStruct, expList, fpList, fpType, exp, expType, resultType, componentFPType, componentExpType;
	int procKind, isProcVar, isValueParam, isVarParam, dim;

	designator = Trees_Left(call);
	designatorTypeStruct = Types_Structure(Trees_Type(designator));
	procKind = Trees_Kind(Trees_Left(designator));
	assert(Types_IsProcedure(designatorTypeStruct));
	resultType = Types_ResultType(designatorTypeStruct);
	isProcVar = procKind != TREES_PROCEDURE_KIND;

	Indent(file, indent);
	if (isProcVar) {
		fprintf(file, "OBNC_PCT(");
		Generate(designator, file, 0);
		fprintf(file, ")");
	} else {
		Generate(designator, file, 0);
	}

	fprintf(file, "(");

	expList = Trees_Right(call);
	fpList = Types_Parameters(designatorTypeStruct);
	while (expList != NULL) {
		assert(fpList != NULL);
		exp = Trees_Left(expList);
		expType = Trees_Type(exp);
		isValueParam = Trees_Kind(Trees_Left(fpList)) == TREES_VALUE_PARAM_KIND;
		isVarParam = Trees_Kind(Trees_Left(fpList)) == TREES_VAR_PARAM_KIND;
		fpType = Trees_Type(Trees_Left(fpList));

		if (CastNeeded(expType, fpType)) {
			fprintf(file, "(");
			Generate(fpType, file, 0);
			if ((isVarParam && ! Types_IsArray(fpType)) || Types_IsRecord(fpType)) {
				fprintf(file, " *");
			}
			fprintf(file, ") ");
		}
		if ((Types_IsArray(expType) && (ArrayDimension(exp) > 0))
				|| (isValueParam && Types_IsRecord(fpType)) 
				|| (isVarParam && ! Types_IsArray(fpType))) {
			fprintf(file, "&");
		}
		GenerateWithPrecedence(exp, file);

		/*additional type info parameters*/
		if (Types_IsOpenArray(fpType)) {
			if (Trees_Symbol(exp) == STRING) {
				fprintf(file, ", %lu", (long unsigned int) strlen(Trees_String(exp)) + 1);
			} else {
				componentFPType = fpType;
				componentExpType = expType;
				dim = ArrayDimension(exp);
				do {
					fprintf(file, ", ");
					GenerateArrayLength(exp, componentExpType, dim, file);
					componentFPType = Types_ElementType(componentFPType);
					componentExpType = Types_ElementType(componentExpType);
					dim++;
				} while (Types_IsArray(componentFPType));
			}
		} else if (isVarParam && Types_IsRecord(fpType)) {
			fprintf(file, ", ");
			GenerateTypeDescExp(exp, file, 0);
		}

		if (Trees_Right(expList) != NULL) {
			fprintf(file, ", ");
		}
		expList = Trees_Right(expList);
		fpList = Trees_Right(fpList);
	}

	fprintf(file, ")");
	if (resultType == NULL) {
		fprintf(file, ";\n");
	}
}


static void GenerateAssert(Trees_Node node, FILE *file, int indent)
{
	Trees_Node exp, filename, line;

	exp = Trees_Left(node);
	filename = Trees_Left(Trees_Right(node));
	line = Trees_Right(Trees_Right(node));

	Indent(file, indent);
	if (Trees_Symbol(exp) == FALSE) {
		/*unconditional ASSERT(FALSE) replaces HALT(1)*/
		fprintf(file, "exit(1);\n");
	} else {	
		fprintf(file, "OBNC_ASSERT(");
		Generate(exp, file, 0);
		fprintf(file, ", ");
		Generate(filename, file, 0);
		fprintf(file, ", ");
		Generate(line, file, 0);
		fprintf(file, ");\n");
	}
}


static void GenerateIntegralCaseStatement(Trees_Node caseStmtNode, FILE *file, int indent)
{
	Trees_Node expNode, currCaseRepNode, currCaseNode, currCaseLabelListNode, currStmtSeqNode, currLabelRangeNode;
	int rangeMin, rangeMax, label;

	expNode = Trees_Left(caseStmtNode);

	Indent(file, indent);
	fprintf(file, "switch (");
	Generate(expNode, file, 0);
	fprintf(file, ") {\n");
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
				Indent(file, indent + 1);
				fprintf(file, "case ");
				Generate(currLabelRangeNode, file, 0);
				fprintf(file, ":\n");
			} else {
				/*generate label range*/
				if (Trees_Symbol(Trees_Left(currLabelRangeNode)) == INTEGER) {
					rangeMin = Trees_Integer(Trees_Left(currLabelRangeNode));
					rangeMax = Trees_Integer(Trees_Right(currLabelRangeNode));
					for (label = rangeMin; label <= rangeMax; label++) {
						Indent(file, indent + 1);
						fprintf(file, "case %d:\n", label);
					}
				} else {
					rangeMin = Trees_Char(Trees_Left(currLabelRangeNode));
					rangeMax = Trees_Char(Trees_Right(currLabelRangeNode));
					for (label = rangeMin; label <= rangeMax; label++) {
						Indent(file, indent + 1);
						fprintf(file, "case ");
						GenerateChar(label, file);
						fprintf(file, ":\n");
					}
				}
			}
			currCaseLabelListNode = Trees_Right(currCaseLabelListNode);
		} while (currCaseLabelListNode != NULL);

		/*generate statement sequence for current case*/
		Generate(currStmtSeqNode, file, indent + 2);
		Indent(file, indent + 2);
		fprintf(file, "break;\n");

		currCaseRepNode = Trees_Right(currCaseRepNode);
	}
	Indent(file, indent + 1);
	fprintf(file, "default:\n");
	Indent(file, indent + 2);
	fprintf(file, "OBNC_CT;\n");
	Indent(file, indent);
	fprintf(file, "}\n");
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
			Indent(file, indent);
			fprintf(file, "if (");
		} else {
			fprintf(file, " else if (");
		}
		GenerateISExpression(caseExp, label, file);
		fprintf(file, ") {\n");
		caseLabelType = label;
		Generate(statementSeq, file, indent + 1);
		caseLabelType = NULL;
		Indent(file, indent);
		fprintf(file, "}");
		caseList = Trees_Right(caseList);
		if (caseList == NULL) {
			fprintf(file, "\n");
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
		Indent(file, indent);
		fprintf(file, "while (");
		Generate(expNode, file, 0);
		fprintf(file, ") {\n");
		Generate(stmtSeqNode, file, indent + 1);
		Indent(file, indent);
		fprintf(file, "}\n");
	} else {
		Indent(file, indent);
		fprintf(file, "while (1) {\n");
		Indent(file, indent + 1);
		fprintf(file, "if (");
		Generate(expNode, file, 0);
		fprintf(file, ") {\n");
		Generate(stmtSeqNode, file, indent + 2);
		Indent(file, indent + 1);
		fprintf(file, "}\n");
		Generate(elsifNode, file, indent + 1);
		Indent(file, indent + 1);
		fprintf(file, "else {\n");
		Indent(file, indent + 2);
		fprintf(file, "break;\n");
		Indent(file, indent + 1);
		fprintf(file, "}\n");
		Indent(file, indent);
		fprintf(file, "}\n");
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

	Indent(file, indent);
	fprintf(file, "for (");
	Generate(controlVarNode, file, 0);
	fprintf(file, " = ");
	Generate(Trees_Right(initNode), file, 0);
	fprintf(file, "; ");
	Generate(controlVarNode, file, 0);
	if (inc > 0)  {
		fprintf(file, " <= ");
	} else {
		fprintf(file, " >= ");
	}
	Generate(limit, file, 0);
	fprintf(file, "; ");
	Generate(controlVarNode, file, 0);
	fprintf(file, " += %d) {\n", inc);
	Generate(statementSeq, file, indent + 1);
	Indent(file, indent);
	fprintf(file, "}\n");
}


static void GenerateMemoryAllocation(Trees_Node var, FILE *file, int indent)
{
	Trees_Node type;
	int hasPointer, hasProcedure;
	const char *allocKind;

	assert(var != NULL);
	assert(Trees_Symbol(var) == TREES_DESIGNATOR);

	type = Trees_Type(var);
	SearchPointersAndProcedures(Types_PointerBaseType(type), &hasPointer, &hasProcedure);
	allocKind = "OBNC_ATOMIC_NOINIT_ALLOC";
	if (hasPointer) {
		allocKind = "OBNC_REGULAR_ALLOC";
	} else if (hasProcedure) {
		allocKind = "OBNC_ATOMIC_ALLOC";
	}
	if ((Trees_Symbol(type) == IDENT) || (Trees_Symbol(Types_PointerBaseType(type)) == IDENT)) {
		Indent(file, indent);
		fprintf(file, "OBNC_NEW(");
		Generate(var, file, 0);
		fprintf(file, ", &");
		Generate(TypeDescIdent(type), file, 0);
		fprintf(file, "td, struct ");
		Generate(TypeDescIdent(type), file, 0);
		fprintf(file, "Heap, %s);\n", allocKind);
	} else {
		Indent(file, indent);
		fprintf(file, "OBNC_NEW_ANON(");
		Generate(var, file, 0);
		fprintf(file, ", %s);\n", allocKind);
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
		fseek(source, oldPos, SEEK_SET);
	}

	if (ferror(source) || ferror(target)) {
		fprintf(stderr, "obnc-compile: file input/output failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
}


static void ReadText(FILE *fp, long int pos, long int count, char result[], int resultLen)
{
	int i, ch;

	assert(count < resultLen);

	fseek(fp, pos, SEEK_SET);
	if (! ferror(fp)) {
		i = 0;
		ch = fgetc(fp);
		while ((ch != EOF) && (i < count)) {
			result[i] = ch;
			i++;
			ch = fgetc(fp);
		}
		result[count] = '\0';
		fseek(fp, 0, SEEK_CUR);
	} else {
		perror(NULL);
		exit(EXIT_FAILURE);
	}
}


static void PushProcedureDeclaration(Trees_Node procIdent)
{
	struct ProcedureDeclNode *node;
	int generatedLen, isFirstLocalProc, ch;

	NEW(node);
	node->procIdent = procIdent;
	node->localProcedures = Maps_New();
	node->runtimeInitVars = NULL;
	if (Trees_Local(procIdent)) {
		/*save unfinished procedure declaration*/
		generatedLen = ftell(cFile) - procedureDeclStart + 1;
		NEW_ARRAY(node->partialDecl, generatedLen);
		ReadText(cFile, procedureDeclStart, generatedLen - 1, node->partialDecl, generatedLen);
	} else {
		node->partialDecl = NULL;
	}
	node->next = procedureDeclStack;

	if (Trees_Local(procIdent)) {
		assert(procedureDeclStack != NULL);
		isFirstLocalProc = (procedureDeclStack->next == NULL) && Maps_IsEmpty(procedureDeclStack->localProcedures);
		Maps_Put(Trees_Name(procIdent), NULL, &(procedureDeclStack->localProcedures));

		/*set file position for writing local procedure*/
		fseek(cFile, procedureDeclStart, SEEK_SET);
		if (isFirstLocalProc) {
			/*keep function signature for global procedure*/
			do {
				ch = fgetc(cFile);
			} while ((ch != EOF) && (ch != ')'));
			assert(ch == ')');
			fseek(cFile, 0, SEEK_CUR);
			fprintf(cFile, ";\n");
		}
	}

	procedureDeclStack = node;
}


static void PopProcedureDeclaration(void)
{
	assert(procedureDeclStack != NULL);
	procedureDeclStart = ftell(cFile);
	if (procedureDeclStack->partialDecl != NULL) {
		fprintf(cFile, "%s", procedureDeclStack->partialDecl);
	}
	procedureDeclStack = procedureDeclStack->next;
}


static void GenerateOpenArrayParameter(Trees_Node param, FILE *file)
{
	Trees_Node elementType;
	int ndims, i;

	elementType = Types_ElementType(Trees_Type(param));
	ndims = 1;
	while (Types_IsArray(elementType)) {
		elementType = Types_ElementType(elementType);
		ndims++;
	}
	Generate(elementType, file, 0);
	fprintf(file, " ");
	Generate(param, file, 0);
	fprintf(file, "[]");
	for (i = 0; i < ndims; i++) {
		fprintf(file, ", OBNC_LONGI int ");
		Generate(param, file, 0);
		fprintf(file, "len");
		if (i > 0) {
			fprintf(file, "%d", i);
		}
	}
}


static void GenerateFormalParameter(Trees_Node param, FILE *file)
{
	int kind;
	Trees_Node type;

	kind = Trees_Kind(param);
	type = Trees_Type(param);
	if (kind == TREES_VALUE_PARAM_KIND) {
		if (Types_IsArray(type) || Types_IsRecord(type)) {
			fprintf(file, "const ");
		}
		if (Types_IsRecord(type) || (type == declaredTypeIdent)) {
			fprintf(file, "struct ");
		}
		if (Types_IsOpenArray(type)) {
			GenerateOpenArrayParameter(param, file);
		} else {
			Generate(type, file, 0);
			fprintf(file, " ");
			if (Types_IsRecord(type) || (type == declaredTypeIdent)) {
				fprintf(file, "*");
			}
			Generate(param, file, 0);
		}
	} else {
		assert(kind == TREES_VAR_PARAM_KIND);
		if (type == declaredTypeIdent) {
			fprintf(file, "struct ");
		}
		if (Types_IsOpenArray(type)) {
			GenerateOpenArrayParameter(param, file);
		} else {
			Generate(type, file, 0);
			fprintf(file, " ");
			if (! Types_IsArray(type)) {
				fprintf(file, "*");
			}
			if (Types_IsPointer(type) && (type == declaredTypeIdent)) {
				fprintf(file, "*");
			}
			Generate(param, file, 0);
			if (Types_IsRecord(type)) {
				fprintf(file, ", const OBNC_Td *");
				Generate(param, file, 0);
				fprintf(file, "td");
			}
		}
	}
}


static void GenerateFormalParameterList(Trees_Node paramList, FILE *file)
{
	Trees_Node param;

	assert(paramList != NULL);

	do {
		param = Trees_Left(paramList);
		GenerateFormalParameter(param, file);
		if (Trees_Right(paramList) != NULL) {
			fprintf(file, ", ");
		}
		paramList = Trees_Right(paramList);
	} while (paramList != NULL);
}


void Generate_ProcedureHeading(Trees_Node procIdent)
{
	Trees_Node procType, resultType, paramList;

	PushProcedureDeclaration(procIdent);
	procedureDeclStart = ftell(cFile);
	fprintf(cFile, "\n");

	/*generate export status*/
	if (! Trees_Exported(procIdent)) {
		fprintf(cFile, "static ");
	}

	/*generate return type*/
	procType = Trees_Type(procIdent);
	resultType = Types_ResultType(procType);
	if (resultType != NULL) {
		Generate(resultType, cFile, 0);
		fprintf(cFile, " ");
	} else {
		fprintf(cFile, "void ");
	}

	/*generate function identifier*/
	Generate(procIdent, cFile, 0);

	/*generate parameter list*/
	fprintf(cFile, "(");
	paramList = Types_Parameters(procType);
	if (paramList != NULL) {
		GenerateFormalParameterList(paramList, cFile);
	} else {
		fprintf(cFile, "void");
	}
	fprintf(cFile, ")");

	if (Trees_Exported(procIdent)) {
		fprintf(hFile, "\n");
		GenerateObjectFileSymbolDefinitions(Trees_NewNode(TREES_NOSYM, procIdent, NULL), "", hFile, 0);
		CopyText(cFile, procedureDeclStart + 1, ftell(cFile) - procedureDeclStart, hFile);
		fprintf(hFile, ";\n");
	}

	fprintf(cFile, "\n{\n");
}


void Generate_ProcedureStatements(Trees_Node stmtSeq)
{
	fprintf(cFile, "\n");
	Generate(stmtSeq, cFile, 1);
}


void Generate_ReturnClause(Trees_Node exp)
{
	Trees_Node resultType;
	
	assert(procedureDeclStack != NULL);
	
	resultType = Types_ResultType(Trees_Type(procedureDeclStack->procIdent));

	Indent(cFile, 1);
	fprintf(cFile, "return ");
	if (CastNeeded(Trees_Type(exp), resultType)) {
		fprintf(cFile, "(");
		Generate(resultType, cFile, 0);
		fprintf(cFile, ") ");
	}
	Generate(exp, cFile, 0);
	fprintf(cFile, ";\n");
}


void Generate_ProcedureEnd(Trees_Node procIdent)
{
	fprintf(cFile, "}\n\n");
	PopProcedureDeclaration();
}


/*MODULE GENERATORS*/

static void GenerateInitCalls(int indent)
{
	Trees_Node current, moduleAndDirPath, module;

	current = importList;
	while (current != NULL) {
		moduleAndDirPath = Trees_Left(current);
		module = Trees_Left(moduleAndDirPath);
		Indent(cFile, indent);
		fprintf(cFile, "%s_Init();\n", Trees_Name(module));
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


void Generate_Open(const char moduleName[], int isEntryPoint)
{
	const char *template;

	inputModuleName = moduleName;
	isEntryPointModule = isEntryPoint;

	/*initialize header comment*/
	if (strcmp(CONFIG_VERSION, "") != 0) {
		template = "/*GENERATED BY OBNC %s*/";
		NEW_ARRAY(headerComment, strlen(template) + strlen(CONFIG_VERSION) + 1);
		sprintf(headerComment, "/*GENERATED BY OBNC %s*/", CONFIG_VERSION);
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
	sprintf(tempCFilepath, ".obnc/%s.c.%d", inputModuleName, getpid());
	cFile = Files_New(tempCFilepath);

	/*create temporary header file*/
	sprintf(tempHFilepath, ".obnc/%s.h.%d", inputModuleName, getpid());
	hFile = Files_New(tempHFilepath);

	atexit(DeleteTemporaryFiles);
}


void Generate_ModuleHeading(void)
{
	fprintf(cFile, "%s\n\n", headerComment);
	fprintf(cFile, "#include <obnc/OBNC.h>\n");
	if (! isEntryPointModule) {
		fprintf(cFile, "#include \"%s.h\"\n", inputModuleName);
	}

	fprintf(hFile, "%s\n\n", headerComment);
	fprintf(hFile, "#ifndef %s_h\n", inputModuleName);
	fprintf(hFile, "#define %s_h\n\n", inputModuleName);
	fprintf(hFile, "#include <obnc/OBNC.h>\n");
}


static int IsInstalledLibrary(const char *path)
{
	char *dotObncPath;
	const char *prefix = Config_Prefix();
	
	NEW_ARRAY(dotObncPath, strlen(path) + strlen("/.obnc") + 1);
	sprintf(dotObncPath, "%s/.obnc", path);
	return (strstr(path, prefix) == path) && (path[strlen(prefix)] == '/') && ! Files_Exists(dotObncPath);
}


static const char *RelativeInstalledLibraryPath(const char *path)
{
	const char *prefix = Config_Prefix();
	const char *libdir = Config_LibDir();
	const char *result, *tail;
	
	result = path;
	if (strstr(path, prefix) == path) {
		tail = result + strlen(prefix);
		if (tail[0] == '/') {
			tail++;
			if (strstr(tail, libdir) == tail) {
				tail += strlen(libdir);
				if (tail[0] == '/') {
					result = tail + 1;
				}
			}
		}	
	}
	return result;
}


void Generate_ImportList(Trees_Node list)
{
	static char hFileDir[PATH_MAX + 1];
	
	Trees_Node moduleAndDirPath, module, dirPathNode;
	const char *dirPath, *parentDirPrefix, *relativePath;

	importList = list;
	
	while (list != NULL) {
		moduleAndDirPath = Trees_Left(list);
		module = Trees_Left(moduleAndDirPath);
		dirPathNode = Trees_Right(moduleAndDirPath);
		dirPath = Trees_String(dirPathNode);
		if (IsInstalledLibrary(dirPath)) {
			relativePath = RelativeInstalledLibraryPath(dirPath);
			fprintf(cFile, "#include <%s/%s.h>\n", relativePath, Trees_Name(module));
			fprintf(hFile, "#include <%s/%s.h>\n", relativePath, Trees_Name(module));
		} else if (strcmp(dirPath, ".") == 0) {
			fprintf(cFile, "#include \"%s.h\"\n", Trees_Name(module));
			fprintf(hFile, "#include \"%s.h\"\n", Trees_Name(module));
		} else {		
			if ((dirPath[0] == '.') && (dirPath[1] == '/') && Files_Exists(".obnc")) {
				parentDirPrefix = ".";
			} else {
				parentDirPrefix = "";
			}
			sprintf(hFileDir, "%s/.obnc", dirPath);
			if (! Files_Exists(hFileDir)) {
				sprintf(hFileDir, "%s", dirPath);
			}
			fprintf(cFile, "#include \"%s%s/%s.h\"\n", parentDirPrefix, hFileDir, Trees_Name(module));
			fprintf(hFile, "#include \"%s%s/%s.h\"\n", parentDirPrefix, hFileDir, Trees_Name(module));
		}
		list = Trees_Right(list);
	}
}


void Generate_ModuleStatements(Trees_Node stmtSeq)
{
	static char initFuncName[FILENAME_MAX + 1];
	Trees_Node initFuncIdent;
	
	if (isEntryPointModule) {
		fprintf(cFile, "\nint main(int argc, char *argv[])\n");
		fprintf(cFile, "{\n");
		Indent(cFile, 1);
		fprintf(cFile, "OBNC_Initialize(argc, argv);\n");
		if (importList != NULL) {
			GenerateInitCalls(1);
		}
		Generate(stmtSeq, cFile, 1);
		Indent(cFile, 1);
		fprintf(cFile, "return 0;\n");
		fprintf(cFile, "}\n");
	} else {
		sprintf(initFuncName, "%s_Init", inputModuleName);
		fprintf(cFile, "\nvoid %s(void)\n", initFuncName);
		fprintf(cFile, "{\n");
		if ((importList != NULL) || (stmtSeq != NULL)) {
			Indent(cFile, 1);
			fprintf(cFile, "static int initialized = 0;\n\n");
			Indent(cFile, 1);
			fprintf(cFile, "if (! initialized) {\n");
			GenerateInitCalls(2);
			Generate(stmtSeq, cFile, 2);
			Indent(cFile, 2);
			fprintf(cFile, "initialized = 1;\n");
			Indent(cFile, 1);
			fprintf(cFile, "}\n");
		}
		fprintf(cFile, "}\n");

		fprintf(hFile, "\n");
		initFuncIdent = Trees_NewIdent(initFuncName);
		Trees_SetInternal(initFuncIdent);
		GenerateObjectFileSymbolDefinitions(Trees_NewNode(TREES_NOSYM, initFuncIdent, NULL), "", hFile, 0);
		fprintf(hFile, "void %s(void);\n", initFuncName);
	}
}


void Generate_ModuleEnd(void)
{
	fprintf(hFile, "\n#endif\n");
}


void Generate_Close(void)
{
	static char cFilepath[PATH_MAX];
	static char hFilepath[PATH_MAX];

	/*close temporary files*/
	Files_Close(cFile);
	Files_Close(hFile);
	
	/*move temporary C file to permanent C file*/
	sprintf(cFilepath, ".obnc/%s.c", inputModuleName);
	if (! Files_Exists(cFilepath) || Generated(cFilepath)) {
		Files_Move(tempCFilepath, cFilepath);
	} else {
		fprintf(stderr, "obnc-compile: error: C file generated by obnc-compile expected, will not overwrite: %s\n", cFilepath);
		exit(EXIT_FAILURE);
	}

	sprintf(hFilepath, ".obnc/%s.h", inputModuleName);
	if (isEntryPointModule) {
		/*delete generated header file*/
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
				GenerateOperator(node, file);
				break;
			case BECOMES:
				GenerateAssignment(node, file, indent);
				break;
			case CASE:
				GenerateCaseStatement(node, file, indent);
				break;
			case ELSE:
				Indent(file, indent);
				fprintf(file, "else {\n");
				Generate(Trees_Left(node), file, indent + 1);
				Indent(file, indent);
				fprintf(file, "}\n");
				break;
			case ELSIF:
				Indent(file, indent);
				fprintf(file, "else if (");
				Generate(Trees_Left(node), file, 0);
				fprintf(file, ") ");
				Generate(Trees_Right(node), file, indent);
				break;
			case FALSE:
				fprintf(file, "0");
				break;
			case FOR:
				GenerateForStatement(node, file, indent);
				break;
			case IDENT:
				GenerateIdent(node, file, indent);
				break;
			case IF:
				Indent(file, indent);
				fprintf(file, "if (");
				Generate(Trees_Left(node), file, 0);
				fprintf(file, ") ");
				Generate(Trees_Right(node), file, indent);
				break;
			case IN:
				fprintf(file, "OBNC_IN(");
				Generate(Trees_Left(node), file, indent);
				fprintf(file, ", ");
				Generate(Trees_Right(node), file, indent);
				fprintf(file, ")");
				break;
			case INTEGER:
				fprintf(file, "%" OBNC_INT_MOD "d", Trees_Integer(node));
				break;
			case IS:
				GenerateISExpression(Trees_Left(node), Trees_Right(node), file);
				break;
			case NIL:
				fprintf(file, "0");
				break;
			case POINTER:
				Generate(Trees_Left(node), file, indent);
				fprintf(file, " *");
				break;
			case REAL:
				GenerateReal(Trees_Real(node), file);
				break;
			case REPEAT:
				Indent(file, indent);
				fprintf(file, "do {\n");
				Generate(Trees_Left(node), file, indent + 1);
				Indent(file, indent);
				fprintf(file, "} while (! (");
				Generate(Trees_Right(node), file, 0);
				fprintf(file, "));\n");
				break;
			case STRING:
				GenerateString(Trees_String(node), file);
				break;
			case THEN:
				fprintf(file, "{\n");
				Generate(Trees_Left(node), file, indent + 1);
				Indent(file, indent);
				fprintf(file, "}\n");
				Generate(Trees_Right(node), file, indent);
				break;
			case TREES_NOSYM:
				Generate(Trees_Left(node), file, indent);
				Generate(Trees_Right(node), file, indent);
				break;
			case TREES_ABS_PROC:
				if (Types_IsInteger(Trees_Type(Trees_Left(node)))) {
					fprintf(file, "OBNC_ABS_INT(");
				} else {
					fprintf(file, "OBNC_ABS_FLT(");
				}
				Generate(Trees_Left(node), file, 0);
				fprintf(file, ")");
				break;
			case TREES_ASR_PROC:
				Indent(file, indent);
				fprintf(file, "OBNC_ASR(");
				Generate(Trees_Left(node), file, 0);
				fprintf(file, ", ");
				Generate(Trees_Right(node), file, 0);
				fprintf(file, ")");
				break;
			case TREES_ASSERT_PROC:
				GenerateAssert(node, file, indent);
				break;
			case TREES_BOOLEAN_TYPE:
				fprintf(file, "int");
				break;
			case TREES_BYTE_TYPE:
				fprintf(file, "unsigned char");
				break;
			case TREES_CHAR_CONSTANT:
				GenerateChar(Trees_Char(node), file);
				break;
			case TREES_CHAR_TYPE:
				fprintf(file, "char");
				break;
			case TREES_CHR_PROC:
				fprintf(file, "OBNC_CHR(");
				Generate(Trees_Left(node), file, 0);
				fprintf(file, ")");
				break;
			case TREES_DEC_PROC:
				if (Trees_Right(node) == NULL) {
					Indent(file, indent);
					fprintf(file, "OBNC_DEC(");
					Generate(Trees_Left(node), file, 0);
					fprintf(file, ");\n");
				} else {
					Indent(file, indent);
					fprintf(file, "OBNC_DEC_N(");
					Generate(Trees_Left(node), file, 0);
					fprintf(file, ", ");
					Generate(Trees_Right(node), file, 0);
					fprintf(file, ");\n");
				}
				break;
			case TREES_DESIGNATOR:
				GenerateDesignator(node, file);
				break;
			case TREES_EXCL_PROC:
				Indent(file, indent);
				fprintf(file, "OBNC_EXCL(");
				Generate(Trees_Left(node), file, 0);
				fprintf(file, ", ");
				Generate(Trees_Right(node), file, 0);
				fprintf(file, ");\n");
				break;
			case TREES_EXP_LIST:
				GenerateExpList(node, file);
				break;
			case TREES_FIELD_LIST_SEQUENCE:
				Generate(Trees_Left(node), file, indent);
				Generate(Trees_Right(node), file, indent);
				break;
			case TREES_FLOOR_PROC:
				fprintf(file, "OBNC_FLOOR(");
				Generate(Trees_Left(node), file, 0);
				fprintf(file, ")");
				break;
			case TREES_FLT_PROC:
				fprintf(file, "OBNC_FLT(");
				Generate(Trees_Left(node), file, 0);
				fprintf(file, ")");
				break;
			case TREES_INC_PROC:
				if (Trees_Right(node) == NULL) {
					Indent(file, indent);
					fprintf(file, "OBNC_INC(");
					Generate(Trees_Left(node), file, 0);
					fprintf(file, ");\n");
				} else {
					Indent(file, indent);
					fprintf(file, "OBNC_INC_N(");
					Generate(Trees_Left(node), file, 0);
					fprintf(file, ", ");
					Generate(Trees_Right(node), file, 0);
					fprintf(file, ");\n");
				}
				break;
			case TREES_INCL_PROC:
				Indent(file, indent);
				fprintf(file, "OBNC_INCL(");
				Generate(Trees_Left(node), file, 0);
				fprintf(file, ", ");
				Generate(Trees_Right(node), file, 0);
				fprintf(file, ");\n");
				break;
			case TREES_INTEGER_TYPE:
				fprintf(file, "OBNC_LONGI int");
				break;
			case TREES_LEN_PROC:
				{
					Trees_Node var;
					
					var = Trees_Left(node);
					GenerateArrayLength(var, Trees_Type(var), ArrayDimension(var), file);
				}
				break;
			case TREES_LSL_PROC:
				fprintf(file, "OBNC_LSL(");
				Generate(Trees_Left(node), file, 0);
				fprintf(file, ", ");
				Generate(Trees_Right(node), file, 0);
				fprintf(file, ")");
				break;
			case TREES_NEW_PROC:
				GenerateMemoryAllocation(Trees_Left(node), file, indent);
				break;
			case TREES_ODD_PROC:
				fprintf(file, "OBNC_ODD(");
				Generate(Trees_Left(node), file, 0);
				fprintf(file, ")");
				break;
			case TREES_ORD_PROC:
				fprintf(file, "OBNC_ORD(");
				if (Types_IsChar(Trees_Type(Trees_Left(node)))) {
					fprintf(file, "(unsigned char) ");
				}
				GenerateWithPrecedence(Trees_Left(node), file);
				fprintf(file, ")");
				break;
			case TREES_PACK_PROC:
				Indent(file, indent);
				if (ContainsProcedureCall(Trees_Left(node)) || ContainsProcedureCall(Trees_Right(node))) {
					fprintf(file, "OBNC_Pack(&(");
					Generate(Trees_Left(node), file, 0);
					fprintf(file, ")");
				} else {
					fprintf(file, "OBNC_PACK(");
					Generate(Trees_Left(node), file, 0);
				}
				fprintf(file, ", ");
				Generate(Trees_Right(node), file, 0);
				fprintf(file, ");\n");
				break;
			case TREES_PROCEDURE_CALL:
				GenerateProcedureCall(node, file, indent);
				break;
			case TREES_RANGE_SET:
				GenerateRangeSet(node, file);
				break;
			case TREES_REAL_TYPE:
				fprintf(file, "OBNC_LONGR double");
				break;
			case TREES_ROR_PROC:
				if (ContainsProcedureCall(Trees_Left(node)) || ContainsProcedureCall(Trees_Right(node))) {
					fprintf(file, "OBNC_Ror(");
				} else {
					fprintf(file, "OBNC_ROR(");
				}
				Generate(Trees_Left(node), file, 0);
				fprintf(file, ", ");
				Generate(Trees_Right(node), file, 0);
				fprintf(file, ")");
				break;
			case TREES_SET_CONSTANT:
				fprintf(file, "0x%" OBNC_INT_MOD "Xu", Trees_Set(node));
				break;
			case TREES_SET_TYPE:
				fprintf(file, "OBNC_LONGI unsigned int");
				break;
			case TREES_SINGLE_ELEMENT_SET:
				GenerateSingleElementSet(node, file);
				break;
			case TREES_STATEMENT_SEQUENCE:
				Generate(Trees_Left(node), file, indent);
				Generate(Trees_Right(node), file, indent);
				break;
			case TREES_UNPK_PROC:
				Indent(file, indent);
				if (ContainsProcedureCall(Trees_Left(node)) || ContainsProcedureCall(Trees_Right(node))) {
					fprintf(file, "OBNC_Unpk(&(");
					Generate(Trees_Left(node), file, 0);
					fprintf(file, "), &(");
					Generate(Trees_Right(node), file, 0);
					fprintf(file, "));\n");				
				} else {
					fprintf(file, "OBNC_UNPK(");
					Generate(Trees_Left(node), file, 0);
					fprintf(file, ", ");
					Generate(Trees_Right(node), file, 0);
					fprintf(file, ");\n");
				}
				break;
			case TRUE:
				fprintf(file, "1");
				break;
			case WHILE:
				GenerateWhileStatement(node, file, indent);
				break;
			default:
				fprintf(stderr, "obnc-compile: unknown symbol: %d\n", Trees_Symbol(node));
				assert(0);
		}
	}
}
