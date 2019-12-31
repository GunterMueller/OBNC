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

#include "Files.h"
#include "StackTrace.h"
#include "Table.h"
#include "Trees.h"
#include "Util.h"
#include "../lib/obnc/OBNC.h" /*needed by YYSTYPE in y.tab.h*/
#include "y.tab.h"
#include <unistd.h> /*POSIX*/
#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

static const char *symfilename;

static void DeleteSymbolFile(void)
{
	int error;

	if (strcmp(symfilename, "") != 0) {
		error = remove(symfilename);
		if (error) {
			perror("error: remove failed: ");
			exit(EXIT_FAILURE);
		}
	}
}


static void Test(void)
{
	Trees_Node symbol, value, result;

	Table_Init();
	assert(! Table_ScopeLocal());

	symbol = Trees_NewIdent("a");
	Trees_SetKind(TREES_CONSTANT_KIND, symbol);
	Trees_SetValue(Trees_NewInteger(37), symbol);
	Trees_SetExported(symbol);
	Table_Put(symbol);

	symbol = Trees_NewIdent("X");
	Trees_SetKind(TREES_PROCEDURE_KIND, symbol);
	Trees_SetExported(symbol);
	Table_Put(symbol);

	Table_OpenScope();
	assert(Table_ScopeLocal());
	Table_CloseScope();

	symbol = Trees_NewIdent("Y");
	Trees_SetKind(TREES_PROCEDURE_KIND, symbol);
	Table_Put(symbol);
	assert(Table_LocallyDeclared("Y"));

	Table_OpenScope();
		assert(! Table_LocallyDeclared("Y"));

		symbol = Trees_NewIdent("X");
		Trees_SetKind(TREES_VARIABLE_KIND, symbol);
		Trees_SetLocal(symbol);
		Table_Put(symbol);

		result = Table_At("X");
		assert(result != NULL);
		assert(Trees_Kind(result) == TREES_VARIABLE_KIND);
		assert(! Trees_Exported(result));

		Table_OpenScope();
			result = Table_At("X"); /*shall return global object*/
			assert(result != NULL);
			assert(Trees_Kind(result) == TREES_PROCEDURE_KIND);
			assert(Trees_Exported(result));
		Table_CloseScope();
	Table_CloseScope();

	result = Table_At("X");
	assert(result != NULL);
	assert(Trees_Kind(result) == TREES_PROCEDURE_KIND);
	assert(Trees_Exported(result));

	result = Table_At("foo");
	assert(result == NULL);

	/*export symbols*/
	Table_Export(symfilename);

	/*clear table*/
	Table_Init();

	/*import symbols*/
	symbol = Trees_NewIdent("Test");
	Trees_SetKind(TREES_QUALIFIER_KIND, symbol);
	Table_Put(symbol);
	Table_Import(symfilename, "Test", "Test");

	result = Table_At("Test.a");
	assert(result != NULL);
	assert(Trees_Kind(result) == TREES_CONSTANT_KIND);
	value = Trees_Value(result);
	assert(Trees_Symbol(value) == INTEGER);
	assert(Trees_Integer(value) == 37);

	result = Table_At("Test.X");
	assert(result != NULL);
	assert(Trees_Kind(result) == TREES_PROCEDURE_KIND);
}


int main(void)
{
	int error;
	const char *tmpdir;

	Files_Init();
	Util_Init();
	StackTrace_Init(NULL);

	tmpdir = getenv("TMPDIR");
	if (tmpdir == NULL) {
		tmpdir = "/tmp";
	}
	symfilename = Util_String("%s/TableTest.%d", tmpdir, getpid());

	error = atexit(DeleteSymbolFile);
	if (error) {
		fprintf(stderr, "error: atexit failed with error: %d\n", error);
		exit(EXIT_FAILURE);
	}

	Test();
	return 0;
}
