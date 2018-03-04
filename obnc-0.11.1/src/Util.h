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

#ifndef UTIL_H
#define UTIL_H

#include <gc/gc.h>
#include <stdio.h>
#include <stdlib.h>

#define LEN(arr) ((int) (sizeof (arr) / sizeof (arr)[0]))

#define NEW_ARRAY(ptr, n) \
	{ \
		(ptr) = GC_MALLOC((n) * sizeof (ptr)[0]); \
		if ((ptr) == NULL) { \
			fprintf(stderr, "error: Memory exhausted\n"); \
			exit(EXIT_FAILURE); \
		} \
	}

#define NEW(ptr) NEW_ARRAY((ptr), 1)

void Util_Init(void);

#endif
