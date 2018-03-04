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

#include "Maps.h"
#include "Util.h"
#include <assert.h>
#include <stdio.h>

typedef struct { int value; } *BoxedInteger;

int count;

static void Count(const char key[], void *value, void *data)
{
	count++;
}


static void Increment(const char key[], void *value, void *data)
{
	((BoxedInteger) value)->value++;
}


int main(void)
{
	Maps_Map map;
	BoxedInteger boxedInteger;
	struct { const char *key; int value; } items[] = {{"foo", 1}, {"bar", 2}, {"baz", 3}};
	int i;

	map = Maps_New();
	assert(Maps_IsEmpty(map));
	
	/*insert items*/
	for (i = 0; i < LEN(items); i++) {
		NEW(boxedInteger);
		boxedInteger->value = items[i].value;
		Maps_Put(items[i].key, boxedInteger, &map);
	}
	assert(! Maps_IsEmpty(map));

	/*retrieve keys*/
	for (i = 0; i < LEN(items); i++) {
		assert(Maps_HasKey(items[i].key, map));
	}

	/*retrieve values*/
	for (i = 0; i < LEN(items); i++) {
		boxedInteger = Maps_At(items[i].key, map);
		assert(boxedInteger->value == items[i].value);
	}

	/*reinsert element*/
	NEW(boxedInteger);
	boxedInteger->value = 1;
	Maps_Put("foo", boxedInteger, &map);
	
	/*count elements*/
	count = 0;
	Maps_Apply(Count, map, NULL);
	assert(count == 3);

	/*increment all values by one*/
	Maps_Apply(Increment, map, NULL);
	for (i = 0; i < LEN(items); i++) {
		assert(Maps_HasKey(items[i].key, map));
		boxedInteger = Maps_At(items[i].key, map);
		assert(boxedInteger->value == items[i].value + 1);
	}
	
	return 0;
}
