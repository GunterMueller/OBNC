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

#include "Config.h"
#include "Oberon.h"
#include "StackTrace.h"
#include "Util.h"
#include "../lib/obnc/OBNC.h" /*needed by YYSTYPE in y.tab.h*/
#include "Trees.h" /*needed by YYSTYPE in y.tab.h*/
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


static void ExitInvalidCommand(void)
{
	fprintf(stderr, "obnc-compile: invalid command. Try 'obnc-compile -h' for more information.\n");
	exit(1);
}


static void PrintHelp(void)
{
	puts("obnc-compile - compile an Oberon module to C");
	puts("");
	puts("usage:");
	puts("\tobnc-compile [-e | -l] MODULE.obn");
	puts("\tobnc-compile (-h | -v)");
	puts("");
	puts("\t-e\tcreate entry point function (main)");
	puts("\t-h\tdisplay help and exit");
	puts("\t-l\tprint names of imported modules and exit");
	puts("\t-v\tdisplay version and exit");
}


static void PrintVersion(void)
{
	if (strcmp(CONFIG_VERSION, "") != 0) {
		printf("OBNC %s\n", CONFIG_VERSION);
	} else {
		puts("OBNC (unknown version)");
	}
}


int main(int argc, char *argv[])
{
	int i;
	int helpWanted = 0;
	int versionWanted = 0;
	int mode = OBERON_NORMAL_MODE;
	const char *inputFile = NULL;

	Util_Init();
	StackTrace_Init();

	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-h") == 0) {
			helpWanted = 1;
		} else if (strcmp(argv[i], "-v") == 0) {
			versionWanted = 1;
		} else if (strcmp(argv[i], "-e") == 0) {
			mode = OBERON_ENTRY_POINT_MODE;
		} else if (strcmp(argv[i], "-l") == 0) {
			mode = OBERON_IMPORT_LIST_MODE;
		} else if ((argv[i][0] != '-') && (inputFile == NULL) && HasFileSuffix(".obn", argv[i])) {
			inputFile = argv[i];
		} else {
			ExitInvalidCommand();
		}
	}

	if (helpWanted) {
		PrintHelp();
	} else if (versionWanted) {
		PrintVersion();
	} else if (inputFile != NULL) {
		Oberon_Parse(inputFile, mode);
	} else {
		ExitInvalidCommand();
	}
	
	return 0;
}
