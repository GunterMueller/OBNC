/*Copyright (C) 2017, 2018, 2019 Karl Landstrom <karl@miasap.se>

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.*/

#include "OBNC.h"
#if ! OBNC_CONFIG_TARGET_EMB
	#include <gc/gc.h>
#endif
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define UNUSED(x) (void) (x)

int OBNC_argc;
char **OBNC_argv;
OBNC_TrapHandler OBNC_handleTrap;

void OBNC_ExitTrap(void)
{
#if OBNC_CONFIG_TARGET_EMB
	while (1);
#else
	exit(EXIT_FAILURE);
#endif
}


static void ExitTrapWithMessage(OBNC_INTEGER exception, const char file[], OBNC_INTEGER fileLen, OBNC_INTEGER line)
{
	UNUSED(fileLen);
	fputs("Exception (E", stderr);
	OBNC_WriteInt(exception, 0, stderr);
	putc(')', stderr);
#if ! OBNC_CONFIG_TARGET_EMB
	fputs(": ", stderr);
	switch (exception) {
		case OBNC_ARRAY_ASSIGNMENT_EXCEPTION:
			fputs("destination array too short for assignment", stderr);
			break;
		case OBNC_ARRAY_INDEX_EXCEPTION:
			fputs("array index out of bounds", stderr);
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
		case OBNC_ASSERT_STATEMENT_EXCEPTION:
			fputs("assertion failure", stderr);
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
	OBNC_ExitTrap();
}


void OBNC_Init(int argc, char *argv[])
{
	OBNC_argc = argc;
	OBNC_argv = argv;
	OBNC_handleTrap = ExitTrapWithMessage;
#if ! OBNC_CONFIG_TARGET_EMB
	GC_INIT();
#endif
}


void *OBNC_Allocate(size_t size, int kind)
{
	void *result = NULL;

#if ! OBNC_CONFIG_TARGET_EMB
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
			OBNC_C_ASSERT(0);
	}
#else
	OBNC_C_ASSERT(0);
#endif
	return result;
}


OBNC_INTEGER OBNC_It1(OBNC_INTEGER i, OBNC_INTEGER n, const char file[], int line)
{
	if ((i < 0) || (i >= n)) {
		OBNC_handleTrap(OBNC_ARRAY_INDEX_EXCEPTION, file, strlen(file) + 1, line);
	}
	return i;
}


static OBNC_INTEGER Abs(OBNC_INTEGER x)
{
	return (x >= 0)? x: -x;
}


void OBNC_WriteInt(OBNC_INTEGER x, OBNC_INTEGER n, FILE *f)
{
	int neg, i;
	char buf[(CHAR_BIT * sizeof (OBNC_INTEGER) / 3) + 3];

	neg = x < 0;
	i = 0;
	do {
		OBNC_C_ASSERT(i < (int) (sizeof (buf) / sizeof (buf[0])));
		buf[i] = '0' + Abs(x % 10);
		x = x / 10;
		i++;
	} while (x != 0);
	while (n > i + !! neg) {
		putc(' ', f);
		n--;
	}
	if (neg) {
		putc('-', f);
	}
	do {
		i--;
		putc(buf[i], f);
	} while (i > 0);
}


void OBNC_WriteHex(unsigned OBNC_INTEGER n, FILE *f)
{
	unsigned OBNC_INTEGER d;
	char digits[2 * sizeof n];
	int i;

	putc(' ', f);
	for (i = 0; i < (int) sizeof (digits); i++) {
		d = n % 16;
		digits[i] = (d >= 10)? 'A' + d - 10: '0' + d;
		n = n / 16;
	}
	for (i = (int) sizeof (digits) - 1; i >= 0; i--) {
		putc(digits[i], f);
	}
}


void OBNC_Exit(int status)
{
#if OBNC_CONFIG_TARGET_EMB
	while (1);
#else
	exit(status);
#endif
}


int OBNC_Cmp(const char s[], OBNC_INTEGER sLen, const char t[], OBNC_INTEGER tLen)
{
	return strncmp(s, t, (sLen < tLen)? sLen: tLen);
}


OBNC_INTEGER OBNC_Div(OBNC_INTEGER x, OBNC_INTEGER y)
{
	return (x >= 0)? x / y: (x - OBNC_Mod(x, y)) / y;
}


OBNC_INTEGER OBNC_Mod(OBNC_INTEGER x, OBNC_INTEGER y)
{
	return (x >= 0)? x % y: ((x % y) + y) % y;
}


unsigned OBNC_INTEGER OBNC_Range(OBNC_INTEGER m, OBNC_INTEGER n)
{
	return (m <= n)?
		(unsigned OBNC_INTEGER) ((((unsigned OBNC_INTEGER) -2) << n) ^ (((unsigned OBNC_INTEGER) -1) << m)):
		(unsigned OBNC_INTEGER) 0x0u;
}


OBNC_INTEGER OBNC_Ror(OBNC_INTEGER x, OBNC_INTEGER n)
{
	return (OBNC_INTEGER) (((unsigned OBNC_INTEGER) x >> n) | ((unsigned OBNC_INTEGER) x << (((unsigned OBNC_INTEGER) sizeof (OBNC_INTEGER) << 3) - (size_t) n)));
}


void OBNC_Pack(OBNC_REAL *x, OBNC_INTEGER n)
{
	*x = OBNC_REAL_SUFFIX(ldexp)(*x, (int) n);
}


void OBNC_Unpk(OBNC_REAL *x, OBNC_INTEGER *n)
{
	int t;

	*x = OBNC_REAL_SUFFIX(frexp)(*x, &t);
	*n = (OBNC_INTEGER) t;
	*x += *x;
	(*n)--;
}
