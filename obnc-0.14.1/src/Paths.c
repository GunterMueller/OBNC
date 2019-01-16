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

#include "Paths.h"
#include "Util.h"
#include <libgen.h> /*POSIX*/
#include <unistd.h> /*POSIX*/
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <string.h>

int Paths_Absolute(const char path[])
{
	assert(path != NULL);
#ifdef _WIN32
	return (path[0] == '\\') || ((strlen(path) >= 2) && isalpha(path[0]) && (path[1] == ':'));
#else
	return path[0] == '/';
#endif
}


char *Paths_Dirname(const char path[])
{
	/*NOTE: We need to copy the string twice since "The dirname() function may modify the string pointed to by path, and may return a pointer to static storage that may then be overwritten by a subsequent call to dirname()."*/
	assert(path != NULL);

	return Util_String("%s", dirname(Util_String("%s", path)));
}


char *Paths_Basename(const char path[])
{
	/*NOTE: We need to copy the string twice since "The basename() function may modify the string pointed to by path, and may return a pointer to internal storage. The returned pointer might be invalidated or the storage might be overwritten by a subsequent call to basename()."*/
	assert(path != NULL);

	return Util_String("%s", basename(Util_String("%s", path)));
}


char *Paths_SansSuffix(const char path[])
{
	char *baseName, *suffix;

	baseName = Paths_Basename(path);
	suffix = strchr(baseName, '.');
	if (suffix != NULL) {
		suffix[0] = '\0';
	}
	return baseName;
}


const char *Paths_ShellArg(const char s[])
{
	int i;
	const char *result;

	i = 0;
	while ((s[i] != '\0') && ! isspace(s[i])) {
		i++;
	}
	if (s[i] == '\0') {
		result = s;
	} else {
#ifdef _WIN32
		/*cmd.com doesn't accept single quotes*/
		result = Util_String("\"%s\"", s);
#else
		result = Util_String("'%s'", Util_Replace("'", "'\"'\"'", s));
#endif
	}
#ifdef _WIN32
	/*use Windows directory separator in paths*/
	result = Util_Replace("/", "\\", result);
#endif
	return result;
}


char *Paths_CurrentDir(void)
{
	static char *result = NULL;
	static char dir[PATH_MAX + 1];
	const char *p;

	if (result == NULL) {
		p = getcwd(dir, sizeof dir);
		if (p != NULL) {
			result = Util_String("%s", dir);
		} else {
			fprintf(stderr, "error: cannot get current directory: %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
	}
	assert(result != NULL);
	return result;
}
