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

#include "Maps.h"
#include "Util.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct Maps_MapDesc {
	char *key;
	void *value;
	Maps_Map next;
};

Maps_Map Maps_New(void)
{
	return NULL;
}


int Maps_IsEmpty(Maps_Map map)
{
	return map == NULL;
}


void Maps_Put(const char key[], void *value, Maps_Map *map)
{
	Maps_Map node;

	assert(key != NULL);
	assert(map != NULL);

	NEW(node);
	NEW_ARRAY(node->key, strlen(key) + 1);
	strcpy(node->key, key);
	node->value = value;
	node->next = *map;
	*map = node;
}


int Maps_HasKey(const char key[], Maps_Map map)
{
	assert(key != NULL);

	while ((map != NULL) && (strcmp(map->key, key) != 0)) {
		map = map->next;
	}
	return map != NULL;
}


void *Maps_At(const char key[], Maps_Map map)
{
	void *result;

	assert(key != NULL);

	while ((map != NULL) && (strcmp(map->key, key) != 0)) {
		map = map->next;
	}
	if (map != NULL) {
		result = map->value;
	} else {
		result = NULL;
	}
	return result;
}


static Maps_Map DeletedDuplicates(Maps_Map map)
{
	Maps_Map result = Maps_New();

	while (map != NULL) {
		if (! Maps_HasKey(map->key, result)) {
			Maps_Put(map->key, map->value, &result);
		}
		map = map->next;
	}
	return result;
}


void Maps_Apply(Maps_Applicator f, Maps_Map map, void *data)
{
	map = DeletedDuplicates(map);
	while (map != NULL) {
		f(map->key, map->value, data);
		map = map->next;
	}
}
