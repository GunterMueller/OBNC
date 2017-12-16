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

#include "Path.h"
#include "Config.h"
#include "Files.h"
#include "Util.h"
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void Append(const char extra[], char s[], int sLen)
{
	int sStrLen, extraStrLen;
	
	sStrLen = strlen(s);
	extraStrLen = strlen(extra);
	if (sStrLen + extraStrLen < sLen) {
		strcpy(s + sStrLen, extra);
	} else {
		fprintf(stderr, "target array too short for concatenation (length %d): %s\n", sLen, s);
		fprintf(stderr, "source string: \"%s\"\n", extra);
		exit(EXIT_FAILURE);
	}
}


static void GetModulePrefix(const char module[], char prefix[], int prefixLen)
{
	int i;
	
	i = 0;
	while (((module[i] >= 'a') && (module[i] <= 'z')) || ((module[i] >= '0') && (module[i] <= '9'))) {
		assert(i < prefixLen);
		prefix[i] = module[i];
		i++;
	}
	if ((module[i] >= 'A') && (module[i] <= 'Z')) {
		prefix[i] = '\0';
	} else {
		prefix[0] = '\0';
	}
}


static void HandlePath(const char module[], int level, char dirPath[], int dirPathLen, int *found)
{
	char symfilePath[PATH_MAX + 1], modulePrefix[FILENAME_MAX + 1];

	*found = 0;
	symfilePath[0] = '\0';
	Append(dirPath, symfilePath, LEN(symfilePath));
	Append("/.obnc/", symfilePath, LEN(symfilePath));
	Append(module, symfilePath, LEN(symfilePath));
	Append(".sym", symfilePath, LEN(symfilePath));
	*found = Files_Exists(symfilePath);
	if (! *found) {
		symfilePath[0] = '\0';
		Append(dirPath, symfilePath, LEN(symfilePath));
		Append("/", symfilePath, LEN(symfilePath));
		Append(module, symfilePath, LEN(symfilePath));
		Append(".sym", symfilePath, LEN(symfilePath));
		*found = Files_Exists(symfilePath);
		if (! *found) {
			symfilePath[0] = '\0';
			Append(dirPath, symfilePath, LEN(symfilePath));
			Append("/", symfilePath, LEN(symfilePath));
			Append(module, symfilePath, LEN(symfilePath));
			Append(".obn", symfilePath, LEN(symfilePath));
			*found = Files_Exists(symfilePath);
			if (! *found & (level == 0)) {
				GetModulePrefix(module, modulePrefix, LEN(modulePrefix));
				Append("/", dirPath, dirPathLen);
				Append(modulePrefix, dirPath, dirPathLen);
				HandlePath(module, 1, dirPath, dirPathLen, found);
			}
		}
	}
}


void Path_Get(const char module[], char dirPath[], int dirPathLen)
{
	const char *impPaths;
	int found, pathStart, pathEnd;
	
	assert(module != NULL);
	assert(dirPath != NULL);
	assert(dirPathLen > 0);
	
	dirPath[0] = '\0';
	Append(".", dirPath, dirPathLen);
	
	/*search current directory*/
	HandlePath(module, 0, dirPath, dirPathLen, &found);
	if (! found) {
		/*search OBNC_IMPORT_PATH*/
		impPaths = getenv("OBNC_IMPORT_PATH");
		if (impPaths != NULL) {
			pathStart = 0;
			do {
				pathEnd = pathStart;
				while ((impPaths[pathEnd] != '\0') && (impPaths[pathEnd] != ':')) {
					pathEnd++;
				}
				if (pathEnd > pathStart) {
					memcpy(dirPath, impPaths + pathStart, pathEnd - pathStart);
					dirPath[pathEnd - pathStart] = '\0';
					HandlePath(module, 0, dirPath, dirPathLen, &found);
				}
				pathStart = pathEnd + 1;
			} while (! found && (impPaths[pathEnd] != '\0'));
		}
		if (! found) {
			/*search install path*/
			dirPath[0] = '\0';
			Append(Config_Prefix(), dirPath, dirPathLen);
			Append("/", dirPath, dirPathLen);
			Append(Config_LibDir(), dirPath, dirPathLen);
			Append("/obnc", dirPath, dirPathLen);
			HandlePath(module, 0, dirPath, dirPathLen, &found);
			if (! found) {
				dirPath[0] = '\0';
			}
		}
	}
}
