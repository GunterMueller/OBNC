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
#include "Util.h"
#include <assert.h>
#include <ctype.h>
#include <string.h>

static const char *filename;
static char *moduleName;
static int mode;

static void InitModuleName(const char filename[])
{
	int startPos, endPos, moduleNameLen, i;

	endPos = strlen(filename);
	do {
		endPos--;
	} while ((endPos >= 0) && (filename[endPos] != '.'));
	assert(endPos > 0);
	assert(filename[endPos] == '.');

	startPos = endPos - 1;
	while ((startPos >= 0) && isalnum(filename[startPos])) {
		startPos--;
	}
	if ((startPos < 0) || ! isalnum(filename[startPos])) {
		startPos++;
	}
	
	moduleNameLen = endPos - startPos;
	NEW_ARRAY(moduleName, moduleNameLen);
	for (i = 0; i < moduleNameLen; i++) {
		moduleName[i] = filename[startPos + i];
	}
}


void Args_Init(const char f[], int m)
{
	static int initialized = 0;
	
	assert(! initialized);
	
	filename = f;
	InitModuleName(f);
	mode = m;
	initialized = 1;
}


const char *Args_Filename(void)
{
	return filename;
}


const char *Args_ModuleName(void)
{
	return moduleName;
}


int Args_Mode(void)
{
	return mode;
}
