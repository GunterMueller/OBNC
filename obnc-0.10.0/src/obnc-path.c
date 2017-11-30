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
#include "Path.h"
#include "Util.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void ExitInvalidCommand(void)
{
	fprintf(stderr, "obnc-path: invalid command. Try 'obnc-path -h' for more information.\n");
	exit(1);
}


static void PrintHelp(void)
{
	puts("obnc-path - print directory path for Oberon module");
	puts("");
	puts("usage:");
	puts("\tobnc-path MODULE");
	puts("\tobnc-path (-h | -v)");
	puts("");
	puts("\t-h\tdisplay help and exit");
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


static void PrintPath(const char module[])
{
	char dir[PATH_MAX + 1];
	
	Path_Get(module, dir, LEN(dir));
	if (strcmp(dir, "") != 0) {
		puts(dir);
	} else {
		fprintf(stderr, "obnc-path: %s.obn or %s.sym not found\n", module, module);
		exit(1);
	}
}


int main(int argc, char *argv[])
{
	int i;
	int helpWanted = 0;
	int versionWanted = 0;
	const char *module = NULL;

	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-h") == 0) {
			helpWanted = 1;
		} else if (strcmp(argv[i], "-v") == 0) {
			versionWanted = 1;
		} else if ((argv[i][0] != '-') && (module == NULL)) {
			module = argv[i];
		} else {
			ExitInvalidCommand();
		}
	}
	
	if (helpWanted) {
		PrintHelp();
	} else if (versionWanted) {
		PrintVersion();
	} else if (module != NULL) {
		PrintPath(module);
	} else {
		ExitInvalidCommand();
	}
	
	return 0;
}
