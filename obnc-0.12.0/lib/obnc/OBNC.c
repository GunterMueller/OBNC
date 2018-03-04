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
#include <limits.h>
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


void OBNC_WriteInt(OBNC_LONGI int x, OBNC_LONGI int n, FILE *f)
{
	OBNC_LONGI int xabs, i;
	char buf[(CHAR_BIT * sizeof (OBNC_LONGI int) / 3) + 3];

	xabs = abs(x);
	i = 0;
	do {
		assert(i < sizeof (buf) / sizeof (buf[0]));
		buf[i] = xabs % 10;
		xabs = xabs / 10;
		i++;
	} while (xabs != 0);
	while (n > i + ((x < 0)? 1: 0)) {
		putc(' ', f);
		n--;
	}
	if (x < 0) {
		putc('-', f);
	}
	do {
		i--;
		putc('0' + buf[i], f);
	} while (i > 0);
}


void OBNC_Abort(int exception, const char file[], int line)
{
	fputs("Exception (E", stderr);
	OBNC_WriteInt(exception, 0, stderr);
	putc(')', stderr);
#ifndef OBNC_CONFIG_USE_SHORT_MSG
	fputs(": ", stderr);
	switch (exception) {
		case OBNC_ARRAY_ASSIGNMENT_EXCEPTION:
			fputs("destination array too short for assignment", stderr);
			break;
		case OBNC_ARRAY_INDEX_EXCEPTION:
			fputs("array index out of bounds", stderr);
			break;
		case OBNC_ORDER_RELATION_EXCEPTION:
			fputs("non-terminated character sequence in order relation", stderr);
			break;
		case OBNC_POINTER_DEREFERENCE_EXCEPTION:
			fputs("nil pointer dereference", stderr);
			break;
		case OBNC_PROCEDURE_CALL_EXCEPTION:
			fputs("nil procedure variable call", stderr);
			break;
		case OBNC_RECORD_ASSIGNMENT_EXCEPTION:
			fputs("source in assignment is not an extension of target", stderr);
			break;
		case OBNC_TYPE_GUARD_EXCEPTION:
			fputs("type guard failure", stderr);
			break;
		case OBNC_CASE_EXP_MATCH_EXCEPTION:
			fputs("unmatched expression in case statement", stderr);
			break;
		default:
			assert(0);
	}
	fputs(" (", stderr);
	fputs(file, stderr);
	fputs(":", stderr);
	OBNC_WriteInt(line, 0, stderr);
	fputs(")", stderr);
#endif
	putc('\n', stderr);
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
