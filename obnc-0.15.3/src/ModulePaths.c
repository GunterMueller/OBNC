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

#include "ModulePaths.h"
#include "Config.h"
#include "Files.h"
#include "Util.h"
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
	#define PATH_SEPARATOR ';'
#else
	#define PATH_SEPARATOR ':'
#endif

static int initialized = 0;

void ModulePaths_Init(void)
{
	if (! initialized) {
		initialized = 1;
		Config_Init();
		Files_Init();
		Util_Init();
	}
}


static const char *AsPrefix(const char dir[])
{
	return (strcmp(dir, ".") == 0)? "": Util_String("%s/", dir);
}


const char *ModulePaths_SourceFile(const char module[], const char dir[])
{
	static const char *suffixes[] = {".obn", ".Mod", ".mod"};
	int i;
	const char *result;

	assert(initialized);

	i = -1;
	do {
		i++;
		result = Util_String("%s%s%s", AsPrefix(dir), module, suffixes[i % LEN(suffixes)]);
	} while ((i < LEN(suffixes)) && ! Files_Exists(result));

	return result;
}


static char *ModulePrefix(const char module[])
{
	char *result;
	int i;

	i = 0;
	while ((module[i] != '\0') && ! isupper(module[i])) {
		i++;
	}
	if ((i > 0) && isupper(module[i])) {
		NEW_ARRAY(result, i + 1);
		memcpy(result, module, i);
		result[i] = '\0';
	} else {
		result = NULL;
	}
	return result;
}


static const char *SearchDir(const char dir[], const char module[])
{
	const char *oberonFile = ModulePaths_SourceFile(module, dir);
	const char *localSymfile = Util_String("%s/.obnc/%s.sym", dir, module);
	const char *globalSymfile = Util_String("%s/%s.sym", dir, module);

	return (Files_Exists(oberonFile) || Files_Exists(localSymfile) || Files_Exists(globalSymfile))
		? dir
		: NULL;
}


static const char *SearchPath(const char dir[], const char module[])
{
	const char *result, *modulePrefix, *libDir;

	result = SearchDir(dir, module);
	if (result == NULL) {
		modulePrefix = ModulePrefix(module);
		if (modulePrefix != NULL) {
			libDir = Util_String("%s/%s", dir, modulePrefix);
			result = SearchDir(libDir, module);
		}
	}
	return result;
}


static void GetSearchPath(const char dir[], const char module[], int verbose, const char *result[])
{
	*result = SearchPath(dir, module);
	if (verbose && (*result == NULL)) {
		printf("%s\n", dir);
	}
}


static int CharCount(char ch, const char s[])
{
	int i = 0, result = 0;

	assert(s != NULL);

	while (s[i] != '\0') {
		if (s[i] == ch) {
			result++;
		}
		i++;
	}
	return result;
}


static char **Split(const char s[], char sep)
{
	char **result;
	int resultLen, i, pos, n;

	assert(s != NULL);

	resultLen = CharCount(sep, s) + 2;
	NEW_ARRAY(result, resultLen);

	pos = 0;
	for (i = 0; i < resultLen - 1; i++) {
		n = 0;
		while ((s[pos + n] != '\0') && (s[pos + n] != PATH_SEPARATOR)) {
			n++;
		}
		NEW_ARRAY(result[i], n + 1);
		memcpy(result[i], s + pos, n);
		result[i][n] = '\0';
		pos += n + 1;
	}
	result[resultLen - 1] = NULL;
	return result;
}


static char **CustomImportPaths(void)
{
	static char **result;
	const char *paths;

	if (result == NULL) {
		paths = getenv("OBNC_IMPORT_PATH");
		if (paths != NULL) {
			result = Split(paths, PATH_SEPARATOR);
		} else {
			NEW_ARRAY(result, 1);
			result[0] = NULL;
		}
	}
	return result;
}


const char *ModulePaths_Directory(const char module[], const char relativeDir[], int verbose)
{
	const char *result, *obncLibPath;
	char **paths;
	int i;

	assert(initialized);
	assert(module != NULL);
	assert(relativeDir != NULL);

	GetSearchPath(relativeDir, module, verbose, &result);
	if (result == NULL) {
		paths = CustomImportPaths();
		i = 0;
		while ((result == NULL) && (paths[i] != NULL)) {
			if (paths[i][0] != '\0') {
				GetSearchPath(paths[i], module, verbose, &result);
			}
			i++;
		}
		if (result == NULL) {
			obncLibPath = Util_String("%s/%s/obnc", Config_Prefix(), Config_LibDir());
			GetSearchPath(obncLibPath, module, verbose, &result);
		}
	}
	return result;
}
