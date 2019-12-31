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
#include "Oberon.h"
#include "StackTrace.h"
#include "Util.h"
#include "../lib/obnc/OBNC.h" /*needed by YYSTYPE in y.tab.h*/
#include "Trees.h" /*needed by YYSTYPE in y.tab.h*/
#include "y.tab.h"
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

static void PrintHelp(void)
{
	puts("obnc-compile - compile an Oberon module to C");
	puts("");
	puts("usage:");
	puts("\tobnc-compile [-e | -l] INFILE");
	puts("\tobnc-compile (-h | -v)");
	puts("");
	puts("\t-e\tcreate entry point function (main)");
	puts("\t-h\tdisplay help and exit");
	puts("\t-l\tprint names of imported modules and exit");
	puts("\t-v\tdisplay version and exit");
	puts("");
	puts("\tINFILE is expected to end with .obn, .Mod or .mod");
}


static void PrintVersion(void)
{
	if (strcmp(CONFIG_VERSION, "") != 0) {
		printf("OBNC %s\n", CONFIG_VERSION);
	} else {
		puts("OBNC (unknown version)");
	}
}


static void PrintContext(void)
{
	Oberon_PrintError("%s", "");
}


static void ExitInvalidCommand(const char msg[])
{
	assert(msg != NULL);

	if (strcmp(msg, "") != 0) {
		fprintf(stderr, "obnc-compile: %s\n", msg);
	}
	fprintf(stderr, "obnc-compile: invalid command. Try 'obnc-compile -h' for more information.\n");
	exit(EXIT_FAILURE);
}


static void ExitFailure(const char msg[])
{
	assert(msg != NULL);

	if (strcmp(msg, "") != 0) {
		fprintf(stderr, "obnc-compile: %s\n", msg);
	}
	fprintf(stderr, "obnc-compile: compilation failed\n");
	exit(EXIT_FAILURE);
}


int main(int argc, char *argv[])
{
	int i;
	int helpWanted = 0;
	int versionWanted = 0;
	int mode = OBERON_NORMAL_MODE;
	const char *arg, *inputFile = NULL, *fileSuffix;

	Error_Init();
	Oberon_Init();
	Util_Init();
	StackTrace_Init(PrintContext);

	Error_SetHandler(ExitInvalidCommand);

	for (i = 1; i < argc; i++) {
		arg = argv[i];
		if (strcmp(arg, "-h") == 0) {
			helpWanted = 1;
		} else if (strcmp(arg, "-v") == 0) {
			versionWanted = 1;
		} else if (strcmp(arg, "-e") == 0) {
			mode = OBERON_ENTRY_POINT_MODE;
		} else if (strcmp(arg, "-l") == 0) {
			mode = OBERON_IMPORT_LIST_MODE;
		} else if ((arg[0] != '-') && (inputFile == NULL)) {
			fileSuffix = strrchr(arg, '.');
			if ((fileSuffix != NULL)
					&& ((strcmp(fileSuffix, ".obn") == 0)
						|| (strcmp(fileSuffix, ".Mod") == 0)
						|| (strcmp(fileSuffix, ".mod") == 0))) {
				inputFile = arg;
			} else {
				Error_Handle(Util_String("missing or invalid filename extension: %s", arg));
			}
		} else {
			Error_Handle("");
		}
	}

	if (helpWanted) {
		PrintHelp();
	} else if (versionWanted) {
		PrintVersion();
	} else if (inputFile != NULL) {
		Error_SetHandler(ExitFailure);
		Oberon_Parse(inputFile, mode);
	} else {
		Error_Handle("");
	}

	return 0;
}
