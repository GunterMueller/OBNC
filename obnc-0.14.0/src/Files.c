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

#include "Files.h"
#include "Error.h"
#include "Util.h"
#include <sys/stat.h> /*POSIX*/
#include <sys/types.h> /*POSIX*/
#include <unistd.h> /*POSIX*/
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int initialized = 0;

void Files_Init(void)
{
	if (! initialized) {
		Error_Init();
		initialized = 1;
	}
}


int Files_Exists(const char filename[])
{
	int error;

	assert(initialized);
	assert(filename != NULL);

	error = access(filename, F_OK);
	return ! error;
}


time_t Files_Timestamp(const char filename[])
{
	struct stat buf;
	int error;
	time_t result = 0;

	assert(initialized);
	assert(filename != NULL);

	error = stat(filename, &buf);
	if (! error) {
		result = buf.st_mtime;
	} else {
		Error_Handle(Util_String("Cannot get timestamp of filename: %s: %s", filename, strerror(errno)));
	}
	return result;
}


FILE *Files_New(const char filename[])
{
	FILE *newFile;

	assert(initialized);
	assert(filename != NULL);

	newFile = fopen(filename, "w+");
	if (newFile == NULL) {
		Error_Handle(Util_String("Cannot open new file: %s: %s", filename, strerror(errno)));
	}

	assert(newFile != NULL);

	return newFile;
}


FILE *Files_Old(const char filename[], int mode)
{
	const char *fopenMode;
	FILE *oldFile;

	assert(initialized);
	assert(filename != NULL);
	assert((mode == FILES_READ) || (mode == FILES_WRITE) || (mode == FILES_APPEND));
	assert(Files_Exists(filename));

	if (mode == FILES_READ) {
		fopenMode = "r";
	} else if (mode == FILES_WRITE) {
		fopenMode = "w";
	} else {
		fopenMode = "a";
	}
	oldFile = fopen(filename, fopenMode);
	if (oldFile == NULL) {
		Error_Handle(Util_String("Cannot open old file: %s: %s", filename, strerror(errno)));
	}

	assert(oldFile != NULL);

	return oldFile;
}


void Files_Close(FILE **file)
{
	int error;

	assert(initialized);
	assert(file != NULL);

	if (*file != NULL) {
		error = fclose(*file);
		if (error) {
			Error_Handle(Util_String("Closing file failed"));
		}
		*file = NULL;
	}
}


void Files_Move(const char sourceFilename[], const char destFilename[])
{
	int error;

	assert(initialized);
	assert(sourceFilename != NULL);
	assert(destFilename != NULL);

#ifdef _WIN32
	if (Files_Exists(destFilename)) {
		Files_Remove(destFilename);
	}
#endif
	error = rename(sourceFilename, destFilename);
	if (error) {
		Error_Handle(Util_String("Cannot move file %s to %s: %s", sourceFilename, destFilename, strerror(errno)));
	}
}


void Files_Remove(const char filename[])
{
	int error;

	assert(initialized);
	assert(filename != NULL);

	error = remove(filename);
	if (error) {
		Error_Handle(Util_String("Cannot remove file: %s: %s", filename, strerror(errno)));
	}
}


void Files_CreateDir(const char dirname[])
{
	int error;

	assert(initialized);
	assert(dirname != NULL);
	assert(! Files_Exists(dirname));

#ifdef _WIN32
	error = mkdir(dirname);
#else
	error = mkdir(dirname, 0755);
#endif
	if (error) {
		Error_Handle(Util_String("Cannot create directory: %s: %s", dirname, strerror(errno)));
		exit(EXIT_FAILURE);
	}
}
