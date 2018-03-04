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

#include "Args.h"
#include "Config.h"
#include "lex.yy.h"
#include "Oberon.h"
#include "StackTrace.h"
#include "Trees.h" /*symbol type in y.tab.h needs tree node declaration*/
#include "Util.h"
#include "y.tab.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>

static int HasFileSuffix(const char suffix[], const char filename[])
{
	int suffixLen = strlen(suffix);
	int filenameLen = strlen(filename);

	return (filenameLen > suffixLen) && (strcmp(filename + filenameLen - suffixLen, suffix) == 0);
}


static void GetArguments(int argc, char *argv[], const char **inputFile, int *mode, int *commandValid)
{
	int i;
	const char *arg;
	
	*inputFile = NULL;
	*mode = ARGS_NORMAL_MODE;
	*commandValid = 1;
	i = 1;
	while ((i < argc) && *commandValid) {
		arg = argv[i];
		if ((strcmp(arg, "-e") == 0) && (i == 1)) {
			*mode = ARGS_ENTRY_POINT_MODE;
		} else if ((strcmp(arg, "-l") == 0) && (i == 1)) {
			*mode = ARGS_IMPORT_LIST_MODE;
		} else if (arg[0] == '-') {
			*commandValid = 0;
		} else if (HasFileSuffix(".obn", arg) && (*inputFile == NULL) && (i <= 2)) {
			*inputFile = arg;
		} else {
			*commandValid = 0;
		}
		i++;
	}
	if (*inputFile == NULL) {
		*commandValid = 0;
	}
}


int main(int argc, char *argv[])
{
	int mode, commandValid, error;
	const char *inputFile;

	Util_Init();
	StackTrace_Init();
	
	error = 0;
	if ((argc == 2) && (strcmp(argv[1], "-v") == 0)) {
		if (strcmp(CONFIG_VERSION, "") != 0) {
			printf("OBNC %s\n", CONFIG_VERSION);
		} else {
			puts("OBNC (unknown version)");
		}
	} else {
		error = 1;
		GetArguments(argc, argv, &inputFile, &mode, &commandValid);
		if (commandValid) {
			yyin = fopen(inputFile, "r");
			if (yyin != NULL) {
				Args_Init(inputFile, mode);
				Oberon_Init();
				error = yyparse();
			} else {
				fprintf(stderr, "%s: error: cannot open file: %s: %s\n", argv[0], inputFile, strerror(errno));
			}
		} else {
			fprintf(stderr, "usage:\n");
			fprintf(stderr, "\tobnc-compile [-e | -l] MODULE.obn\n");
			fprintf(stderr, "\tobnc-compile -v\n");
		}
	}
	return error;
}
