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

#ifndef ARGS_H
#define ARGS_H

#define ARGS_NORMAL_MODE 0
#define ARGS_ENTRY_POINT_MODE 1
#define ARGS_IMPORT_LIST_MODE 2
#define ARGS_MODE_COUNT 3

void Args_Init(const char filename[], int mode);

const char *Args_Filename(void);

const char *Args_ModuleName(void);

int Args_Mode(void);

#endif
