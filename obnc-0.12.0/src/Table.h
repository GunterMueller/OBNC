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

#ifndef TABLE_H
#define TABLE_H

#include "Trees.h"
#include <stdio.h>

void Table_Init(void);

void Table_Put(Trees_Node identNode);

Trees_Node Table_At(const char name[]);

int Table_LocallyDeclared(const char name[]);

void Table_OpenScope(void);

void Table_CloseScope(void);

int Table_ScopeLocal(void);

void Table_Import(const char filename[], const char module[], const char qualifier[]);

void Table_ImportSystem(const char qualifier[]);

void Table_Export(const char filename[]);

#endif
