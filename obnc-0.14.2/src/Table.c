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

#include "Table.h"
#include "Config.h"
#include "Files.h"
#include "Maps.h"
#include "Trees.h"
#include "Types.h"
#include "Util.h"
#include "../lib/obnc/OBNC.h"
#include "y.tab.h"
#include <assert.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*symbol file symbols*/
#define IDENT_SYM 1
#define BOOLEAN_SYM 2
#define CHAR_SYM 3
#define INTEGER_SYM 4
#define REAL_SYM 5
#define STRING_SYM 6
#define SET_SYM 7
#define BOOLEAN_TYPE_SYM 8
#define CHAR_TYPE_SYM 9
#define INTEGER_TYPE_SYM 10
#define REAL_TYPE_SYM 11
#define BYTE_TYPE_SYM 12
#define SET_TYPE_SYM 13
#define ARRAY_SYM 14
#define RECORD_SYM 15
#define POINTER_SYM 16
#define PROCEDURE_SYM 17

/*symbol file identifier kinds*/
#define CONST_KIND 1
#define TYPE_KIND 2
#define VAR_KIND 3
#define PROCEDURE_KIND 4
#define FIELD_KIND 5
#define VALUE_PARAM_KIND 6
#define VAR_PARAM_KIND 7

typedef struct ScopeDesc *Scope;
struct ScopeDesc {
	Maps_Map symbols;
	Scope parent;
};

static const char *predeclaredNames[24];
static Trees_Node predeclaredNodes[LEN(predeclaredNames)];

static Scope globalScope, currentScope;
static const char *importFilename, *exportFilename;
static FILE *importFile, *exportFile;
static Maps_Map writtenSymbols;

void Table_Init(void)
{
	static const struct { const char *name; int kind, type; } predecIdents[] = {
		{"ABS", TREES_PROCEDURE_KIND, TREES_ABS_PROC},
		{"ASR", TREES_PROCEDURE_KIND, TREES_ASR_PROC},
		{"ASSERT", TREES_PROCEDURE_KIND, TREES_ASSERT_PROC},
		{"BOOLEAN", TREES_TYPE_KIND, TREES_BOOLEAN_TYPE},
		{"BYTE", TREES_TYPE_KIND, TREES_BYTE_TYPE},
		{"CHAR", TREES_TYPE_KIND, TREES_CHAR_TYPE},
		{"CHR", TREES_PROCEDURE_KIND, TREES_CHR_PROC},
		{"DEC", TREES_PROCEDURE_KIND, TREES_DEC_PROC},
		{"EXCL", TREES_PROCEDURE_KIND, TREES_EXCL_PROC},
		{"FLOOR", TREES_PROCEDURE_KIND, TREES_FLOOR_PROC},
		{"FLT", TREES_PROCEDURE_KIND, TREES_FLT_PROC},
		{"INC", TREES_PROCEDURE_KIND, TREES_INC_PROC},
		{"INCL", TREES_PROCEDURE_KIND, TREES_INCL_PROC},
		{"INTEGER", TREES_TYPE_KIND, TREES_INTEGER_TYPE},
		{"LEN", TREES_PROCEDURE_KIND, TREES_LEN_PROC},
		{"LSL", TREES_PROCEDURE_KIND, TREES_LSL_PROC},
		{"NEW", TREES_PROCEDURE_KIND, TREES_NEW_PROC},
		{"ODD", TREES_PROCEDURE_KIND, TREES_ODD_PROC},
		{"ORD", TREES_PROCEDURE_KIND, TREES_ORD_PROC},
		{"PACK", TREES_PROCEDURE_KIND, TREES_PACK_PROC},
		{"REAL", TREES_TYPE_KIND, TREES_REAL_TYPE},
		{"ROR", TREES_PROCEDURE_KIND, TREES_ROR_PROC},
		{"SET", TREES_TYPE_KIND, TREES_SET_TYPE},
		{"UNPK", TREES_PROCEDURE_KIND, TREES_UNPK_PROC}};

	int i;
	Trees_Node node;

	assert(LEN(predecIdents) == LEN(predeclaredNames));
	assert(LEN(predeclaredNodes) == LEN(predeclaredNames));

	for (i = 0; i < LEN(predeclaredNodes); i++) {
		predeclaredNames[i] = predecIdents[i].name;

		node = Trees_NewIdent(predecIdents[i].name);
		Trees_SetKind(predecIdents[i].kind, node);
		Trees_SetType(Trees_NewLeaf(predecIdents[i].type), node);
		predeclaredNodes[i] = node;
	}

	NEW(globalScope);
	globalScope->symbols = Maps_New();
	globalScope->parent = NULL;
	currentScope = globalScope;
}


int Table_LocallyDeclared(const char name[])
{
	return Maps_HasKey(name, currentScope->symbols);
}


void Table_Put(Trees_Node identNode)
{
	const char *name;

	assert(identNode != NULL);
	assert(Trees_Symbol(identNode) == IDENT);
	assert((Trees_Local(identNode) && Table_ScopeLocal())
		|| (! Trees_Local(identNode) && ! Table_ScopeLocal()));
	name = Trees_Name(identNode);
	assert(! Table_LocallyDeclared(name));

	Maps_Put(name, identNode, &(currentScope->symbols));
}


static int Cmp(const void *name, const void *namePtr)
{
	return strcmp((char *) name, * (char **) namePtr);
}


static Trees_Node PredeclaredIdent(const char name[])
{
	const char **namePtr;
	ptrdiff_t pos;
	Trees_Node result;

	namePtr = bsearch(name, predeclaredNames, LEN(predeclaredNames), sizeof predeclaredNames[0], Cmp);
	if (namePtr != NULL) {
		pos = namePtr - predeclaredNames;
		assert(pos >= 0);
		assert(pos < LEN(predeclaredNodes));
		result = predeclaredNodes[pos];
	} else {
		result = NULL;
	}
	return result;
}


Trees_Node Table_At(const char name[])
{
	void *result;

	assert(name != NULL);

	result = Maps_At(name, currentScope->symbols);
	if ((result == NULL) && (currentScope != globalScope)) {
		result = Maps_At(name, globalScope->symbols);
	}
	if (result == NULL) {
		result = PredeclaredIdent(name);
	}

	assert((result == NULL) || Trees_Symbol(result) == IDENT);

	return result;
}


void Table_OpenScope(void)
{
	Scope newScope;

	NEW(newScope);
	newScope->symbols = Maps_New();
	newScope->parent = currentScope;
	currentScope = newScope;

	assert(currentScope != globalScope);
}


void Table_CloseScope(void)
{
	assert(currentScope != globalScope);

	currentScope = currentScope->parent;
}


int Table_ScopeLocal(void)
{
	return currentScope != globalScope;
}


static void GetFilePosition(FILE *file, long int *line, long int *col)
{
	long int savedPos, pos;
	int ch;

	*line = 1;
	*col = 0;
	savedPos = ftell(file);
	if (savedPos >= 0) {
		rewind(file);
		pos = savedPos;
		ch = fgetc(file);
		while ((pos > 0) && (ch != EOF)) {
			if (ch == '\n') {
				++*line;
				*col = 0;
			} else {
				++*col;
			}
			pos--;
			ch = fgetc(file);
		}
		fseek(file, savedPos, SEEK_SET);
	}
	if (ferror(file)) {
		fprintf(stderr, "failed getting file position from %s: %s", importFilename, strerror(errno));
		exit(EXIT_FAILURE);
	}
}


static void PrintImportErrorPrefix(void)
{
	long int line, col;

	assert(importFile != NULL);
	GetFilePosition(importFile, &line, &col);
	fprintf(stderr, "%s:%ld:%ld: ", importFilename, line, col);
}


static int StringLength(FILE *file)
{
	long int savedPos;
	int ch, n;

	savedPos = ftell(file);
	fscanf(file, " ");
	n = -1;
	ch = fgetc(file);
	if (ch == '"') {
		do {
			ch = fgetc(file);
			n++;
		} while ((ch != EOF) && (ch != '\n') && (ch != '"'));
		if (ch != '"') {
			n = -1;
		}
	}
	fseek(file, savedPos, SEEK_SET);
	return n;
}


static int IsLeadingIdentChar(int ch)
{
	return ((ch >= 'A') && (ch <= 'Z')) || ((ch >= 'a') && (ch <= 'z'));
}


static int IsIdentChar(int ch)
{
	return IsLeadingIdentChar(ch) || ((ch >= '0') && (ch <= '9')) || (ch == '_');
}


static int IdentLength(FILE *file)
{
	long int savedPos;
	int ch, n;

	savedPos = ftell(file);
	fscanf(file, " ");
	n = 0;
	ch = fgetc(file);
	if (IsLeadingIdentChar(ch)) {
		do {
			n++;
			ch = fgetc(file);
		} while (IsIdentChar(ch));
		if (ch == '.') {
			n++;
			ch = fgetc(file);
			if (IsLeadingIdentChar(ch)) {
				do {
					n++;
					ch = fgetc(file);
				} while (IsIdentChar(ch));
			}
		}
	}
	fseek(file, savedPos, SEEK_SET);
	return n;
}


static void ReadSExp(int isRoot, FILE *file, Trees_Node *resultPtr);

static void ReadIdent(int isRoot, FILE *file, Trees_Node *resultPtr)
{
	char *name;
	int len, sfKind, kind, n, exported;

	fscanf(file, " ");
	len = IdentLength(file);
	NEW_ARRAY(name, len + 1);
	fgets(name, len + 1, file);

	*resultPtr = Trees_NewIdent(name);

	/*kind*/
	n = fscanf(file, "%d", &sfKind);
	if (n == 1) {
		switch (sfKind) {
			case CONST_KIND:
				kind = TREES_CONSTANT_KIND;
				break;
			case TYPE_KIND:
				kind = TREES_TYPE_KIND;
				break;
			case VAR_KIND:
				kind = TREES_VARIABLE_KIND;
				break;
			case PROCEDURE_KIND:
				kind = TREES_PROCEDURE_KIND;
				break;
			case FIELD_KIND:
				kind = TREES_FIELD_KIND;
				break;
			case VALUE_PARAM_KIND:
				kind = TREES_VALUE_PARAM_KIND;
				break;
			case VAR_PARAM_KIND:
				kind = TREES_VAR_PARAM_KIND;
				break;
			default:
				PrintImportErrorPrefix();
				fprintf(stderr, "error: invalid identifier kind: %d\n", sfKind);
				exit(EXIT_FAILURE);
		}
		Trees_SetKind(kind, *resultPtr);
	} else {
		PrintImportErrorPrefix();
		fprintf(stderr, "error: reading identifier kind failed\n");
		exit(EXIT_FAILURE);
	}

	/*exported flag*/
	if ((sfKind == TYPE_KIND) && isRoot) {
		n = fscanf(file, "%d", &exported);
		if (n == 1) {
			if (exported) {
				Trees_SetImported(*resultPtr);
			}
		} else {
			PrintImportErrorPrefix();
			fprintf(stderr, "error: reading export status failed\n");
			exit(EXIT_FAILURE);
		}
	} else if (sfKind == FIELD_KIND) {
		Trees_SetExported(*resultPtr);
	} else if ((sfKind != VALUE_PARAM_KIND) && (sfKind != VAR_PARAM_KIND)) {
		Trees_SetImported(*resultPtr);
	}

	/*value or type*/
	switch (sfKind) {
		case CONST_KIND:
			{
				Trees_Node constValue;

				ReadSExp(0, file, &constValue);
				Trees_SetValue(constValue, *resultPtr);
			}
			break;
		case TYPE_KIND:
			{
				Trees_Node type;

				if (isRoot) {
					ReadSExp(0, file, &type);
					Trees_SetType(type, *resultPtr);
				}
			}
			break;
		case VAR_KIND:
		case PROCEDURE_KIND:
		case FIELD_KIND:
		case VALUE_PARAM_KIND:
		case VAR_PARAM_KIND:
			{
				Trees_Node type;

				ReadSExp(0, file, &type);
				Trees_SetType(type, *resultPtr);
			}
			break;
		default:
			assert(0);
	}
}


static int AtEndOfList(FILE *file)
{
	int ch, result;

	ch = getc(file);
	result = ch == ')';
	ungetc(ch, file);
	return result;
}


static void ReadFieldList(FILE *file, Trees_Node *resultPtr)
{
	Trees_Node field;
	int ch;

	assert(resultPtr != NULL);

	*resultPtr = NULL;
	fscanf(file, " ");
	ch = fgetc(file);
	if (ch == '(') {
		while (! AtEndOfList(file)) {
			ReadSExp(0, file, &field);
			*resultPtr = Trees_NewNode(TREES_IDENT_LIST, field, *resultPtr);
			fscanf(file, " ");
		}
		Trees_ReverseList(resultPtr); /*correct order*/
		fscanf(file, " ");
		ch = fgetc(file);
		if (ch != ')') {
			PrintImportErrorPrefix();
			fprintf(stderr, "error: expected end of field list s-expression\n");
			exit(EXIT_FAILURE);
		}
		fscanf(file, " ");
	} else {
		PrintImportErrorPrefix();
		fprintf(stderr, "error: expected start of field list s-expression\n");
		exit(EXIT_FAILURE);
	}
}


static void ReadRecord(FILE *file, Trees_Node *resultPtr)
{
	Trees_Node baseType, fieldListSeq, fieldList;
	int ch;

	ReadSExp(0, file, &baseType);

	fieldListSeq = NULL;
	fscanf(file, " ");
	ch = fgetc(file);
	if (ch == '(') {
		while (! AtEndOfList(file)) {
			ReadFieldList(file, &fieldList);
			fieldListSeq = Trees_NewNode(TREES_FIELD_LIST_SEQUENCE, fieldList, fieldListSeq);
		}
		Trees_ReverseList(&fieldListSeq); /*correct order*/
		fscanf(file, " ");
		ch = fgetc(file);
		if (ch != ')') {
			PrintImportErrorPrefix();
			fprintf(stderr, "error: expected end of field list sequence s-expression\n");
			exit(EXIT_FAILURE);
		}
	} else {
		PrintImportErrorPrefix();
		fprintf(stderr, "error: expected start of field list sequence s-expression\n");
		exit(EXIT_FAILURE);
	}

	*resultPtr = Types_NewRecord(baseType, fieldListSeq);
}


static void ReadSymbol(int symbol, int isRoot, FILE *file, Trees_Node *resultPtr)
{
	assert(resultPtr != NULL);

	*resultPtr = NULL;
	switch (symbol) {
		case IDENT_SYM:
			ReadIdent(isRoot, file, resultPtr);
			break;
		case BOOLEAN_SYM:
			{
				int b, n;

				n = fscanf(file, "%d", &b);
				if (n == 1) {
					*resultPtr = Trees_NewBoolean(b);
				} else {
					PrintImportErrorPrefix();
					fprintf(stderr, "error: reading boolean value failed\n");
					exit(EXIT_FAILURE);
				}
			}
			break;
		case CHAR_SYM:
			{
				int i, n;

				n = fscanf(file, "%d", &i);
				if (n == 1) {
					if (i < CHAR_MIN) {
						PrintImportErrorPrefix();
						fprintf(stderr, "warning: character constant out of range: %d < %d", i, CHAR_MIN);
					} else if (i > CHAR_MAX) {
						PrintImportErrorPrefix();
						fprintf(stderr, "warning: character constant out of range: %d > %d", i, CHAR_MAX);
					}
					*resultPtr = Trees_NewChar((char) i);
				} else {
					PrintImportErrorPrefix();
					fprintf(stderr, "error: reading character constant failed\n");
					exit(EXIT_FAILURE);
				}
			}
			break;
		case INTEGER_SYM:
			{
				OBNC_LONGI int i, n;

				n = fscanf(file, "%" OBNC_INT_MOD "d", &i);
				if (n == 1) {
					*resultPtr = Trees_NewInteger(i);
				} else {
					PrintImportErrorPrefix();
					fprintf(stderr, "error: reading integer failed\n");
					exit(EXIT_FAILURE);
				}
			}
			break;
		case REAL_SYM:
			{
				OBNC_LONGR double x;
				int n;

				n = fscanf(file, "%" OBNC_REAL_MOD_R "f", &x);
				if (n == 1) {
					*resultPtr = Trees_NewReal(x);
				} else {
					PrintImportErrorPrefix();
					fprintf(stderr, "error: reading real number failed\n");
					exit(EXIT_FAILURE);
				}
			}
			break;
		case STRING_SYM:
			{
				int done, len, ch;
				char *s;

				done = 0;
				len = StringLength(file);
				NEW_ARRAY(s, len + 1);
				fscanf(file, " ");
				ch = fgetc(file);
				if (ch == '"') {
					fgets(s, len + 1, file);
					ch = fgetc(file);
					if (ch == '"') {
						*resultPtr = Trees_NewString(s);
						done = 1;
					}
				}
				if (! done) {
					PrintImportErrorPrefix();
					fprintf(stderr, "error: reading string failed\n");
					exit(EXIT_FAILURE);
				}
			}
			break;
		case SET_SYM:
			{
				OBNC_LONGI unsigned int u;
				int n;

				n = fscanf(file, "%" OBNC_INT_MOD "u", &u);
				if (n == 1) {
					*resultPtr = Trees_NewSet(u);
				} else {
					PrintImportErrorPrefix();
					fprintf(stderr, "error: reading set constant failed\n");
					exit(EXIT_FAILURE);
				}
			}
			break;
		case BOOLEAN_TYPE_SYM:
			*resultPtr = Trees_NewNode(TREES_BOOLEAN_TYPE, NULL, NULL);
			break;
		case CHAR_TYPE_SYM:
			*resultPtr = Trees_NewNode(TREES_CHAR_TYPE, NULL, NULL);
			break;
		case INTEGER_TYPE_SYM:
			*resultPtr = Trees_NewNode(TREES_INTEGER_TYPE, NULL, NULL);
			break;
		case REAL_TYPE_SYM:
			*resultPtr = Trees_NewNode(TREES_REAL_TYPE, NULL, NULL);
			break;
		case BYTE_TYPE_SYM:
			*resultPtr = Trees_NewNode(TREES_BYTE_TYPE, NULL, NULL);
			break;
		case SET_TYPE_SYM:
			*resultPtr = Trees_NewNode(TREES_SET_TYPE, NULL, NULL);
			break;
		case ARRAY_SYM:
			{
				Trees_Node length, elemType;

				ReadSExp(0, file, &length);
				if (length != NULL) {
					if (Trees_Symbol(length) == INTEGER) {
						if (Trees_Integer(length) < 0) {
							PrintImportErrorPrefix();
							fprintf(stderr, "error: negative array length: %" OBNC_INT_MOD "d\n", Trees_Integer(length));
							exit(EXIT_FAILURE);
						}
					} else {
						PrintImportErrorPrefix();
						fprintf(stderr, "error: non-integer array length read\n");
						exit(EXIT_FAILURE);
					}
				}
				ReadSExp(0, file, &elemType);
				*resultPtr = Types_NewArray(length, elemType);
			}
			break;
		case RECORD_SYM:
			ReadRecord(file, resultPtr);
			break;
		case POINTER_SYM:
			{
				Trees_Node ptrBaseType;

				ReadSExp(0, file, &ptrBaseType);
				*resultPtr = Types_NewPointer(ptrBaseType);
			}
			break;
		case PROCEDURE_SYM:
			{
				Trees_Node resultType, par, params;
				int ch;

				ReadSExp(0, file, &resultType);

				params = NULL;
				fscanf(file, " ");
				ch = fgetc(file);
				if (ch == '(') {
					while (! AtEndOfList(file)) {
						ReadSExp(0, file, &par);
						params = Trees_NewNode(TREES_IDENT_LIST, par, params);
					}
					Trees_ReverseList(&params); /*correct order*/
					fscanf(file, " ");
					ch = fgetc(file);
					if (ch != ')') {
						PrintImportErrorPrefix();
						fprintf(stderr, "error: expected end of parameter list s-expression\n");
						exit(EXIT_FAILURE);
					}
				} else {
					PrintImportErrorPrefix();
					fprintf(stderr, "error: expected start of parameter list s-expression\n");
					exit(EXIT_FAILURE);
				}
				*resultPtr = Types_NewProcedure(params, resultType);
			}
			break;
		default:
			assert(0);
	}

	assert(*resultPtr != NULL);
}


static void ReadSExp(int isRoot, FILE *file, Trees_Node *resultPtr)
{
	int symbol, n, ch;

	assert(resultPtr != NULL);

	*resultPtr = NULL;
	n = fscanf(file, " ");
	if ((n != EOF) && ! feof(file)) {
		ch = fgetc(file);
		if (ch == '(') {
			ch = fgetc(file);
			if (ch != ')') {
				ungetc(ch, file);
				n = fscanf(file, "%d", &symbol);
				if (n == 1) {
					ReadSymbol(symbol, isRoot, file, resultPtr);
					ch = fgetc(file);
					if (ch != ')') {
						PrintImportErrorPrefix();
						fprintf(stderr, "error: expected end of s-expression, read '%c'\n", ch);
						exit(EXIT_FAILURE);
					}
				} else {
					PrintImportErrorPrefix();
					fprintf(stderr, "error: failed reading symbol\n");
					exit(EXIT_FAILURE);
				}
			}
		} else {
			PrintImportErrorPrefix();
			fprintf(stderr, "error: expected start of s-expression, read '%c'\n", ch);
			exit(EXIT_FAILURE);
		}
	} else if (feof(file)) {
		PrintImportErrorPrefix();
		fprintf(stderr, "error: unexpected end of file\n");
		exit(EXIT_FAILURE);
	} else if (ferror(file)) {
		PrintImportErrorPrefix();
		fprintf(stderr, "error: failed reading expression: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
}


static void ResolveTypesRec(Trees_Node node, Maps_Map symfileEntries)
{
	Trees_Node type, typeDef;

	if (node != NULL) {
		type = NULL;
		switch (Trees_Symbol(node)) {
			case IDENT:
				switch (Trees_Kind(node)) {
					case TREES_TYPE_KIND:
					case TREES_VARIABLE_KIND:
					case TREES_FIELD_KIND:
					case TREES_VALUE_PARAM_KIND:
					case TREES_VAR_PARAM_KIND:
						type = Trees_Type(node);
						break;
				}
				break;
			case ARRAY:
				type = Types_ElementType(node);
				break;
			case RECORD:
				type = Types_RecordBaseType(node);
				break;
			case POINTER:
				type = Types_PointerBaseType(node);
				break;
			case PROCEDURE:
				type = Types_ResultType(node);
				break;
		}

		if ((type != NULL) && (Trees_Symbol(type) == IDENT)) {
			typeDef = Maps_At(Trees_Name(type), symfileEntries);
			if (typeDef != NULL) {
				switch (Trees_Symbol(node)) {
					case IDENT:
						Trees_SetType(typeDef, node);
						break;
					case ARRAY:
						Types_SetElementType(typeDef, node);
						break;
					case RECORD:
						Types_SetRecordBaseType(typeDef, node);
						ResolveTypesRec(Types_Fields(node), symfileEntries);
						break;
					case POINTER:
						Types_SetPointerBaseType(typeDef, node);
						break;
					case PROCEDURE:
						Types_SetResultType(typeDef, node);
						ResolveTypesRec(Types_Parameters(node), symfileEntries);
						break;
					default:
						assert(0);
				}
			} else {
				fprintf(stderr, "missing type name in symbol file: %s\n", Trees_Name(type));
				exit(EXIT_FAILURE);
			}
		} else if (Trees_Symbol(node) == IDENT) {
			ResolveTypesRec(Trees_Type(node), symfileEntries);
		} else {
			ResolveTypesRec(Trees_Left(node), symfileEntries);
			ResolveTypesRec(Trees_Right(node), symfileEntries);
		}
	}
}


static void ResolveTypes(const char identName[], void *identNode, void *symbolFileEntriesMap)
{
	Trees_Node ident;
	Maps_Map symbolFileEntries;

	(void) identName; /*prevent "unused" warning*/
	ident = identNode;
	symbolFileEntries = symbolFileEntriesMap;
	ResolveTypesRec(ident, symbolFileEntries);
}


static char *QualifiedName(const char qualifier[], const char name[])
{
	size_t resultLen;
	char *result;

	resultLen = strlen(qualifier) + strlen(".") + strlen(name) + strlen("\0");
	NEW_ARRAY(result, resultLen);
	strcpy(result, qualifier);
	strcat(result, ".");
	strcat(result, name);
	return result;
}


static const char *importModule, *importQualifier;

static void SetQualifiers(const char identName[], void *identNode, void *data)
{
	const char *name;
	int isReExportedType;

	(void) identName; /*prevent "unused" warning*/
	(void) data; /*prevent "unused" warning*/
	name = Trees_Name(identNode);
	isReExportedType = strchr(name, '.') != NULL;
	if (! isReExportedType) {
		Trees_SetName(QualifiedName(importQualifier, name), identNode);
		Trees_SetUnaliasedName(QualifiedName(importModule, name), identNode);
	}
}


void Table_Import(const char filename[], const char module[], const char qualifier[])
{
	int ch, n;
	Maps_Map symbolFileEntries;
	Trees_Node ident, importEntries, p;

	importFilename = filename;
	importFile = Files_Old(filename, FILES_READ);

	/*skip version line*/
	do {
		ch = fgetc(importFile);
	} while ((ch != EOF) && (ch != '\n'));

	/*read entries*/
	symbolFileEntries = Maps_New();
	importEntries = NULL;
	n = fscanf(importFile, " ");
	while ((n != EOF) && ! feof(importFile)) {
		ReadSExp(1, importFile, &ident);
		if (ident != NULL) {
			Maps_Put(Trees_Name(ident), ident, &symbolFileEntries);
			if (Trees_Imported(ident)) {
				importEntries = Trees_NewNode(TREES_NOSYM, ident, importEntries);
			}
		} else {
			fprintf(stderr, "unexpected null entry in symbol file\n");
			exit(EXIT_FAILURE);
		}
		n = fscanf(importFile, " ");
	}

	/*resolve types*/
	Maps_Apply(ResolveTypes, symbolFileEntries, symbolFileEntries);

	/*qualify identifiers*/
	importModule = module;
	importQualifier = qualifier;
	Maps_Apply(SetQualifiers, symbolFileEntries, NULL);

	/*import*/
	p = importEntries;
	while (p != NULL) {
		ident = Trees_Left(p);
		Table_Put(ident);
		p = Trees_Right(p);
	}

	Files_Close(&importFile);
	importFile = NULL;
	importFilename = NULL;
}


void Table_ImportSystem(const char qualifier[])
{
	static const struct { const char *name; int type; } procs[] = {
		{"ADR", TREES_ADR_PROC},
		{"SIZE", TREES_SIZE_PROC},
		{"BIT", TREES_BIT_PROC},
		{"GET", TREES_GET_PROC},
		{"PUT", TREES_PUT_PROC},
		{"COPY", TREES_COPY_PROC},
		{"VAL", TREES_VAL_PROC}};
	const char *name;
	int i;
	Trees_Node ident;

	for (i = 0; i < LEN(procs); i++) {
		name = Util_String("%s.%s", qualifier, procs[i].name);
		ident = Trees_NewIdent(name);
		Trees_SetKind(TREES_PROCEDURE_KIND, ident);
		Trees_SetType(Trees_NewLeaf(procs[i].type), ident);
		Table_Put(ident);
	}
}


static int SFKind(Trees_Node ident)
{
	int result = -1;

	switch (Trees_Kind(ident)) {
		case TREES_CONSTANT_KIND:
			result = CONST_KIND;
			break;
		case TREES_TYPE_KIND:
			result = TYPE_KIND;
			break;
		case TREES_VARIABLE_KIND:
			result = VAR_KIND;
			break;
		case TREES_PROCEDURE_KIND:
			result = PROCEDURE_KIND;
			break;
		case TREES_FIELD_KIND:
			result = FIELD_KIND;
			break;
		case TREES_VALUE_PARAM_KIND:
			result = VALUE_PARAM_KIND;
			break;
		case TREES_VAR_PARAM_KIND:
			result = VAR_PARAM_KIND;
			break;
		default:
			assert(0);
			break;
	}
	return result;
}


static void Write(Trees_Node node, int isRoot, FILE *file, Maps_Map *indirectlyExportedTypes)
{
	if (node == NULL) {
		fprintf(file, "()");
	} else {
		switch (Trees_Symbol(node)) {
			case IDENT:
				fprintf(file, "(%d %s %d", IDENT_SYM, Trees_UnaliasedName(node), SFKind(node));
				if ((SFKind(node) == TYPE_KIND) && isRoot) {
					fprintf(file, " %d", Trees_Exported(node));
				}
				switch (SFKind(node)) {
					case CONST_KIND:
							fprintf(file, " ");
							Write(Trees_Value(node), 0, file, indirectlyExportedTypes);
						break;
					case TYPE_KIND:
						if (isRoot) {
							fprintf(file, " ");
							Write(Trees_Type(node), 0, file, indirectlyExportedTypes);
						} else if (! Trees_Exported(node)) {
							Maps_Put(Trees_Name(node), node, indirectlyExportedTypes);
						}
						break;
					case VAR_KIND:
					case PROCEDURE_KIND:
					case FIELD_KIND:
					case VALUE_PARAM_KIND:
					case VAR_PARAM_KIND:
						fprintf(file, " ");
						Write(Trees_Type(node), 0, file, indirectlyExportedTypes);
						break;
					default:
						assert(0);
				}
				fprintf(file, ")");
				break;
			case FALSE:
				fprintf(file, "(%d 0)", BOOLEAN_SYM);
				break;
			case TRUE:
				fprintf(file, "(%d 1)", BOOLEAN_SYM);
				break;
			case TREES_CHAR_CONSTANT:
				fprintf(file, "(%d %d)", CHAR_SYM, Trees_Char(node));
				break;
			case INTEGER:
				fprintf(file, "(%d %" OBNC_INT_MOD "d)", INTEGER_SYM, Trees_Integer(node));
				break;
			case REAL:
				fprintf(file, "(%d %.*" OBNC_REAL_MOD_W "g)", REAL_SYM, DBL_DIG, Trees_Real(node));
				break;
			case STRING:
				fprintf(file, "(%d \"%s\")", STRING_SYM, Trees_String(node));
				break;
			case TREES_SET_CONSTANT:
				fprintf(file, "(%d %" OBNC_INT_MOD "u)", SET_SYM, Trees_Set(node));
				break;
			case TREES_BOOLEAN_TYPE:
				fprintf(file, "(%d)", BOOLEAN_TYPE_SYM);
				break;
			case TREES_CHAR_TYPE:
				fprintf(file, "(%d)", CHAR_TYPE_SYM);
				break;
			case TREES_INTEGER_TYPE:
				fprintf(file, "(%d)", INTEGER_TYPE_SYM);
				break;
			case TREES_REAL_TYPE:
				fprintf(file, "(%d)", REAL_TYPE_SYM);
				break;
			case TREES_BYTE_TYPE:
				fprintf(file, "(%d)", BYTE_TYPE_SYM);
				break;
			case TREES_SET_TYPE:
				fprintf(file, "(%d)", SET_TYPE_SYM);
				break;
			case ARRAY:
				fprintf(file, "(%d ", ARRAY_SYM);
				Write(Types_ArrayLength(node), 0, file, indirectlyExportedTypes);
				fprintf(file, " ");
				Write(Types_ElementType(node), 0, file, indirectlyExportedTypes);
				fprintf(file, ")");
				break;
			case RECORD:
				{
					Trees_Node fieldListSeq, fieldList, field;
					int fieldListSeqEmpty, fieldListEmpty;

					fprintf(file, "(%d ", RECORD_SYM);
					Write(Types_RecordBaseType(node), 0, file, indirectlyExportedTypes);
					fieldListSeq = Types_Fields(node);
					fprintf(file, " (");
					fieldListSeqEmpty = 1;
					while (fieldListSeq != NULL) {
						assert(Trees_Symbol(fieldListSeq) == TREES_FIELD_LIST_SEQUENCE);
						fieldList = Trees_Left(fieldListSeq);
						fieldListEmpty = 1;
						while (fieldList != NULL) {
							assert(Trees_Symbol(fieldList) == TREES_IDENT_LIST);
							field = Trees_Left(fieldList);
							if (Trees_Exported(field)) {
								if (fieldListEmpty) {
									if (! fieldListSeqEmpty) {
										fprintf(file, " ");
									}
									fprintf(file, "(");
								} else {
									fprintf(file, " ");
								}
								Write(field, 0, file, indirectlyExportedTypes);
								fieldListSeqEmpty = 0;
								fieldListEmpty = 0;
							}
							fieldList = Trees_Right(fieldList);
						}
						if (! fieldListEmpty) {
							fprintf(file, ")");
						}
						fieldListSeq = Trees_Right(fieldListSeq);
					}
					fprintf(file, "))");
				}
				break;
			case POINTER:
				fprintf(file, "(%d ", POINTER_SYM);
				Write(Types_PointerBaseType(node), 0, file, indirectlyExportedTypes);
				fprintf(file, ")");
				break;
			case PROCEDURE:
				{
					Trees_Node params = Types_Parameters(node);

					fprintf(file, "(%d ", PROCEDURE_SYM);
					Write(Types_ResultType(node), 0, file, indirectlyExportedTypes);
					fprintf(file, " (");
					while (params != NULL) {
						Write(Trees_Left(params), 0, file, indirectlyExportedTypes);
						if (Trees_Right(params) != NULL) {
							fprintf(file, " ");
						}
						params = Trees_Right(params);
					}
					fprintf(file, "))");
				}
				break;
			default:
				assert(0);
		}
	}
}


static void WriteExportedSymbol(const char identName[], void *identNode, void *indirectlyExportedTypesMapPtr)
{
	Trees_Node ident = identNode;
	Maps_Map *indirectlyExportedTypes = indirectlyExportedTypesMapPtr;

	(void) identName; /*prevent "unused" warning*/
	assert(Trees_Symbol(ident) == IDENT);
	if (Trees_Exported(ident)) {
		assert(indirectlyExportedTypesMapPtr != NULL);
		Write(ident, 1, exportFile, indirectlyExportedTypes);
		Maps_Put(Trees_Name(ident), NULL, &writtenSymbols);
		fputc('\n', exportFile);
	}
}


static void WriteSymbol(const char identName[], void *identNode, void *indirectlyExportedTypesMapPtr)
{
	Trees_Node ident = identNode;
	Maps_Map *indirectlyExportedTypes = indirectlyExportedTypesMapPtr;

	(void) identName; /*prevent "unused" warning*/
	assert(Trees_Symbol(ident) == IDENT);
	if (! Maps_HasKey(Trees_Name(ident), writtenSymbols)) {
		Write(ident, 1, exportFile, indirectlyExportedTypes);
		Maps_Put(Trees_Name(ident), NULL, &writtenSymbols);
		fputc('\n', exportFile);
	}
}


void Table_Export(const char filename[])
{
	Maps_Map indirectlyExportedTypes, nextIndirectlyExportedTypes;
	int i;

	assert(filename != NULL);

	exportFilename = filename;
	exportFile = Files_New(filename);
	if (strcmp(CONFIG_VERSION, "") != 0) {
		fprintf(exportFile, "%s", CONFIG_VERSION);
	}
	fputc('\n', exportFile);

	writtenSymbols = Maps_New();

	indirectlyExportedTypes = Maps_New();
	Maps_Apply(WriteExportedSymbol, globalScope->symbols, &indirectlyExportedTypes);
	i = 0;
	while (! Maps_IsEmpty(indirectlyExportedTypes) && (i < 10)) {
		nextIndirectlyExportedTypes = Maps_New();
		Maps_Apply(WriteSymbol, indirectlyExportedTypes, &nextIndirectlyExportedTypes);
		indirectlyExportedTypes = nextIndirectlyExportedTypes;
		i++;
	}
	if (i < 10) {
		Files_Close(&exportFile);
		exportFile = NULL;
		exportFilename = NULL;
	} else {
		fprintf(stderr, "too many levels of indirectly exported types when exporting symbols to %s\n", exportFilename);
		exit(EXIT_FAILURE);
	}
}
