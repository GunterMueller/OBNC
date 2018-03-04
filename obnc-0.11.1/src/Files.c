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
#include <sys/stat.h> /*POSIX*/
#include <sys/types.h> /*POSIX*/
#include <unistd.h> /*POSIX*/
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int Files_Exists(const char filename[])
{
	int error;

	assert(filename != NULL);

	error = access(filename, F_OK);
	return ! error;
}


FILE *Files_New(const char filename[])
{
	FILE *newFile;

	assert(filename != NULL);

	newFile = fopen(filename, "w+");
	if (newFile == NULL) {
		fprintf(stderr, "obnc-compile: error: Cannot open new file: %s: %s\n", filename, strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	assert(newFile != NULL);
	
	return newFile;
}


FILE *Files_Old(const char filename[], int mode)
{
	const char *fopenMode;
	FILE *oldFile;

	assert(filename != NULL);
	assert((mode == FILES_READ) || (mode == FILES_WRITE));
	assert(Files_Exists(filename));

	if (mode == FILES_READ) {
		fopenMode = "r";
	} else {
		fopenMode = "w";
	}
	oldFile = fopen(filename, fopenMode);
	if (oldFile == NULL) {
		fprintf(stderr, "obnc-compile: error: Cannot open old file: %s: %s\n", filename, strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	assert(oldFile != NULL);
	
	return oldFile;
}


void Files_Close(FILE *file)
{
	int error;

	error = fclose(file);
	if (error) {
		fprintf(stderr, "obnc-compile: error: Closing file failed");
		exit(EXIT_FAILURE);
	}
}


void Files_Move(const char sourceFilename[], const char destFilename[])
{
	int error;
	
	assert(sourceFilename != NULL);
	assert(destFilename != NULL);
	
	error = rename(sourceFilename, destFilename);
	if (error) {
		fprintf(stderr, "obnc-compile: error: Cannot move file %s to %s: %s\n", sourceFilename, destFilename, strerror(errno));
		exit(EXIT_FAILURE);	
	}
}


void Files_Remove(const char filename[])
{
	int error;

	assert(filename != NULL);

	error = remove(filename);
	if (error) {
		fprintf(stderr, "obnc-compile: error: Cannot remove file: %s: %s\n", filename, strerror(errno));
		exit(EXIT_FAILURE);
	}
}


void Files_CreateDir(const char dirname[])
{
	const mode_t accessMode = 0755;
	int error;

	assert(dirname != NULL);
	assert(! Files_Exists(dirname));

	error = mkdir(dirname, accessMode);
	if (error) {
		fprintf(stderr, "obnc-compile: error: Cannot create directory: %s: %s\n", dirname, strerror(errno));
		exit(EXIT_FAILURE);
	}
}
