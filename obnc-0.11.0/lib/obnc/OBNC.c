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

#include "OBNC.h"
#include <gc/gc.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int OBNC_argc;
char **OBNC_argv;

void OBNC_Initialize(int argc, char *argv[])
{
	OBNC_argc = argc;
	OBNC_argv = argv;
	GC_INIT();
}


int OBNC_Cmp(const char s[], OBNC_LONGI int sLen, const char t[], OBNC_LONGI int tLen, const char file[], int line)
{
	int minLen, i;
	
	minLen = (sLen < tLen)? sLen: tLen;
	i = 0;
	while ((i < minLen) && (s[i] != '\0') && (s[i] == t[i])) {
		i++;
	}
	if ((i < 0) || (i >= minLen)) {
		OBNC_Abort(OBNC_ORDER_RELATION_EXCEPTION, file, line);
	}
	
	return (unsigned char) s[i] - (unsigned char) t[i];
}


void *OBNC_Allocate(size_t size, int kind)
{
	void *result = NULL;
	
	switch (kind) {
		case OBNC_REGULAR_ALLOC:
			result = GC_MALLOC(size); /*initializes memory to zero like calloc*/
			break;
		case OBNC_ATOMIC_ALLOC:
			result = GC_MALLOC_ATOMIC(size);
			if (result != NULL) {
				memset(result, 0, size);
			}
			break;
		case OBNC_ATOMIC_NOINIT_ALLOC:
			result = GC_MALLOC_ATOMIC(size); /*no initialization*/
			break;
		default:
			assert(0);
	}	
	return result;
}


int OBNC_It1(OBNC_LONGI int i, OBNC_LONGI int n, const char file[], int line)
{
	if ((i < 0) || (i >= n)) {
		OBNC_Abort(OBNC_ARRAY_INDEX_EXCEPTION, file, line);
	}
	return i;
}


void OBNC_Abort(int exception, const char file[], int line)
{
	switch (exception) {
		case OBNC_ARRAY_ASSIGNMENT_EXCEPTION:
			fprintf(stderr, "exception: destination array too short for assignment (%s:%d)\n", file, line);
			break;
		case OBNC_ARRAY_INDEX_EXCEPTION:
			fprintf(stderr, "exception: array index out of bounds (%s:%d)\n", file, line);
			break;
		case OBNC_ORDER_RELATION_EXCEPTION:
			fprintf(stderr, "exception: non-terminated character array in order relation (%s:%d)\n", file, line);
			break;
		case OBNC_POINTER_DEREFERENCE_EXCEPTION:
			fprintf(stderr, "exception: nil pointer dereference (%s:%d)\n", file, line);
			break;
		case OBNC_PROCEDURE_CALL_EXCEPTION:
			fprintf(stderr, "exception: nil procedure variable call (%s:%d)\n", file, line);
			break;
		case OBNC_RECORD_ASSIGNMENT_EXCEPTION:
			fprintf(stderr, "exception: source in assignment is not an extension of target (%s:%d)\n", file, line);
			break;
		case OBNC_TYPE_GUARD_EXCEPTION:
			fprintf(stderr, "exception: type guard failure (%s:%d)\n", file, line);
			break;
		case OBNC_CASE_EXP_MATCH_EXCEPTION:
			fprintf(stderr, "exception: unmatched expression in case statement (%s:%d)\n", file, line);
			break;
		default:
			assert(0);
	}
	abort();
}


OBNC_LONGI int OBNC_Div(OBNC_LONGI int x, OBNC_LONGI int y)
{
	return (x >= 0)? x / y: (x - OBNC_Mod(x, y)) / y;
}


OBNC_LONGI int OBNC_Mod(OBNC_LONGI int x, OBNC_LONGI int y)
{
	return (x >= 0)? x % y: ((x % y) + y) % y;
}


OBNC_LONGI int OBNC_Range(OBNC_LONGI int m, OBNC_LONGI int n)
{
	return (m <= n)
		? (((OBNC_LONGI unsigned int) -2) << n) ^ (((OBNC_LONGI unsigned int) -1) << m)
		: 0x0u;
}


OBNC_LONGI int OBNC_Ror(OBNC_LONGI int x, OBNC_LONGI int n)
{
	return ((OBNC_LONGI unsigned int) x >> n) | ((OBNC_LONGI unsigned int) x << ((sizeof (OBNC_LONGI int) << 3) - n));
}


void OBNC_Pack(OBNC_LONGR double *x, OBNC_LONGI int n)
{
#ifdef OBNC_CONFIG_USE_LONG_REAL
	*x = ldexpl(*x, n);
#else
	*x = ldexp(*x, n);
#endif
}


void OBNC_Unpk(OBNC_LONGR double *x, OBNC_LONGI int *n)
{
#ifdef OBNC_CONFIG_USE_LONG_INT
	int t;
	#ifdef OBNC_CONFIG_USE_LONG_REAL
		*x = frexpl(*x, &t);
	#else
		*x = frexp(*x, &t);
	#endif
	*n = t;
	*x += *x;
	(*n)--;
#else
	#ifdef OBNC_CONFIG_USE_LONG_REAL
		*x = frexpl(*x, n);
	#else
		*x = frexp(*x, n);
	#endif
	*x += *x;
	(*n)--;
#endif
}
