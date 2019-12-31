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

#include "Config.h"
#include "Error.h"
#include "ModulePaths.h"
#include "Paths.h"
#include "Util.h"
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void PrintHelp(void)
{
	puts("obnc-path - print directory path for Oberon module");
	puts("");
	puts("usage:");
	puts("\tobnc-path [-v] MODULE");
	puts("\tobnc-path (-h | -v)");
	puts("");
	puts("\t-h\tdisplay help and exit");
	puts("\t-v\tprint each inspected directory, or display version and exit");
}


static void PrintVersion(void)
{
	if (strcmp(CONFIG_VERSION, "") != 0) {
		printf("OBNC %s\n", CONFIG_VERSION);
	} else {
		puts("OBNC (unknown version)");
	}
}


static void PrintPath(const char module[], int verbose)
{
	const char *dirPath;

	dirPath = ModulePaths_Directory(module, ".", verbose);
	if (dirPath != NULL) {
		puts(Paths_ShellArg(dirPath));
	} else {
		Error_Handle(Util_String("module not found: %s", module));
	}
}


static void ExitInvalidCommand(const char msg[])
{
	assert(msg != NULL);

	if (strcmp(msg, "") != 0) {
		fprintf(stderr, "obnc-path: %s", msg);
	}
	fprintf(stderr, ". Try 'obnc-path -h' for more information.\n");
	exit(EXIT_FAILURE);
}


static void ExitFailure(const char msg[])
{
	assert(msg != NULL);

	if (strcmp(msg, "") != 0) {
		fprintf(stderr, "obnc-path: %s\n", msg);
	}
	exit(EXIT_FAILURE);
}


int main(int argc, char *argv[])
{
	int i;
	int helpWanted = 0;
	int vSet = 0;
	const char *module = NULL;

	Config_Init();
	Error_Init();
	ModulePaths_Init();
	Util_Init();
	Error_SetHandler(ExitInvalidCommand);
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-h") == 0) {
			helpWanted = 1;
		} else if (strcmp(argv[i], "-v") == 0) {
			vSet= 1;
		} else if ((argv[i][0] != '-') && (module == NULL)) {
			module = argv[i];
		} else {
			Error_Handle("invalid command");
		}
	}

	if (helpWanted) {
		PrintHelp();
	} else if (module != NULL) {
		if (strchr(module, '.') == NULL) {
			Error_SetHandler(ExitFailure);
			PrintPath(module, vSet);
		} else {
			Error_Handle("module name cannot contain '.'");
		}
	} else if (vSet) {
		PrintVersion();
	} else {
		Error_Handle("invalid command");
	}

	return 0;
}
