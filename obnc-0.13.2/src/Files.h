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

#ifndef FILES_H
#define FILES_H

#include <stdio.h>
#include <time.h>

/*file access modes*/
#define FILES_READ 0
#define FILES_WRITE 1

int Files_Exists(const char filename[]);

time_t Files_Timestamp(const char filename[]);

FILE *Files_New(const char filename[]);

FILE *Files_Old(const char filename[], int mode);

void Files_CreateDir(const char dirname[]);

void Files_Move(const char sourceFilename[], const char destFilename[]);

void Files_Remove(const char filename[]);

void Files_Close(FILE *file);

#endif
