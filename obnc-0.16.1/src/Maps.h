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

#ifndef MAPS_H
#define MAPS_H

typedef struct Maps_MapDesc *Maps_Map;
typedef void (*Maps_Applicator)(const char key[], void *value, void *data);

void Maps_Init(void);

Maps_Map Maps_New(void);

int Maps_IsEmpty(Maps_Map map);

void Maps_Put(const char key[], void *value, Maps_Map *map);

int Maps_HasKey(const char key[], Maps_Map map);

void *Maps_At(const char key[], Maps_Map map);

void Maps_Apply(Maps_Applicator f, Maps_Map map, void *data);

#endif
