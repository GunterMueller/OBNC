/*Copyright (C) 2017, 2018, 2019 Karl Landstrom <karl@miasap.se>

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.*/

#ifndef OBNC_H
#define OBNC_H

#include "OBNCConfig.h"
#include <limits.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if OBNC_CONFIG_TARGET_EMB
	#define OBNC_CFILE ""
	#define OBNC_OBNFILE ""
#else
	#define OBNC_CFILE __FILE__
	#define OBNC_OBNFILE OBERON_SOURCE_FILENAME
#endif

/*Run-time exceptions*/

#define OBNC_ARRAY_ASSIGNMENT_EXCEPTION 1
#define OBNC_ARRAY_INDEX_EXCEPTION 2
#define OBNC_POINTER_DEREFERENCE_EXCEPTION 3
#define OBNC_PROCEDURE_CALL_EXCEPTION 4
#define OBNC_RECORD_ASSIGNMENT_EXCEPTION 5
#define OBNC_TYPE_GUARD_EXCEPTION 6
#define OBNC_CASE_EXP_MATCH_EXCEPTION 7
#define OBNC_ASSERT_STATEMENT_EXCEPTION 8

/*Memory allocation kinds*/

#define OBNC_REGULAR_ALLOC 0
#define OBNC_ATOMIC_ALLOC 1
#define OBNC_ATOMIC_NOINIT_ALLOC 2

/*Properties of basic type INTEGER (a set is an unsigned OBNC_INTEGER)*/

#if OBNC_CONFIG_C_INT_TYPE == OBNC_CONFIG_SHORT
	#define OBNC_INTEGER short
	#define OBNC_INT_MOD "h"
	#define OBNC_INT_MIN SHRT_MIN
	#define OBNC_INT_MAX SHRT_MAX
	#define OBNC_UINT_MAX USHRT_MAX
	#define OBNC_INT_PREFIX(func) func
#elif OBNC_CONFIG_C_INT_TYPE == OBNC_CONFIG_INT
	#define OBNC_INTEGER int
	#define OBNC_LONGI /*obsolescent*/
	#define OBNC_INT_MOD ""
	#define OBNC_INT_MIN INT_MIN
	#define OBNC_INT_MAX INT_MAX
	#define OBNC_UINT_MAX UINT_MAX
	#define OBNC_INT_PREFIX(func) func
#elif OBNC_CONFIG_C_INT_TYPE == OBNC_CONFIG_LONG
	#define OBNC_INTEGER long
	#define OBNC_LONGI long /*obsolescent*/
	#define OBNC_INT_MOD "l"
	#define OBNC_INT_MIN LONG_MIN
	#define OBNC_INT_MAX LONG_MAX
	#define OBNC_UINT_MAX ULONG_MAX
	#define OBNC_INT_PREFIX(func) l ## func
#elif OBNC_CONFIG_C_INT_TYPE == OBNC_CONFIG_LONG_LONG
	#define OBNC_INTEGER long long
	#define OBNC_LONGI long long /*obsolescent*/
	#ifdef _WIN32
		#define OBNC_INT_MOD "I64"
	#else
		#define OBNC_INT_MOD "ll"
	#endif
	#define OBNC_INT_MIN LLONG_MIN
	#define OBNC_INT_MAX LLONG_MAX
	#define OBNC_UINT_MAX ULLONG_MAX
	#define OBNC_INT_PREFIX(func) ll ## func
#else
	#error Invalid value of OBNC_CONFIG_C_INT_TYPE
#endif

#ifdef OBNC_CONFIG_USE_LONG_INT
	#error OBNC_CONFIG_USE_LONG_INT is obsolete, use OBNC_CONFIG_C_INT_TYPE instead
#endif

/*Properties of basic type REAL*/

#if OBNC_CONFIG_C_REAL_TYPE == OBNC_CONFIG_FLOAT
	#define OBNC_REAL float
	#define OBNC_REAL_MOD_R ""
	#define OBNC_REAL_MOD_W ""
	#define OBNC_REAL_MIN FLT_MIN
	#define OBNC_REAL_MAX FLT_MAX
	#define OBNC_REAL_PREFIX(func) f ## func
	#define OBNC_REAL_SUFFIX(func) func ## f
#elif OBNC_CONFIG_C_REAL_TYPE == OBNC_CONFIG_DOUBLE
	#define OBNC_REAL double
	#define OBNC_LONGR /*obsolescent*/
	#define OBNC_REAL_MOD_R "l"
	#define OBNC_REAL_MOD_W ""
	#define OBNC_REAL_MIN DBL_MIN
	#define OBNC_REAL_MAX DBL_MAX
	#define OBNC_REAL_PREFIX(func) func
	#define OBNC_REAL_SUFFIX(func) func
#elif OBNC_CONFIG_C_REAL_TYPE == OBNC_CONFIG_LONG_DOUBLE
	#define OBNC_REAL long double
	#define OBNC_LONGR long /*obsolescent*/
	#define OBNC_REAL_MOD_R "L"
	#define OBNC_REAL_MOD_W "L"
	#define OBNC_REAL_MIN LDBL_MIN
	#define OBNC_REAL_MAX LDBL_MAX
	#define OBNC_REAL_PREFIX(func) l ## func
	#define OBNC_REAL_SUFFIX(func) func ## l
#else
	#error Invalid value of OBNC_CONFIG_C_REAL_TYPE
#endif

#ifdef OBNC_CONFIG_USE_LONG_REAL
	#error OBNC_CONFIG_USE_LONG_REAL is obsolete, use OBNC_CONFIG_C_REAL_TYPE instead
#endif

/*Predefined function procedures*/

#define OBNC_ABS_INT(x) OBNC_INT_PREFIX(abs)(x)
#define OBNC_ABS_FLT(x) OBNC_REAL_SUFFIX(fabs)(x)
#define OBNC_ODD(x) (((x) & 1) == 1)
#define OBNC_LSL(x, n) ((OBNC_INTEGER) (x) << (n))
#define OBNC_ASR(x, n) ((OBNC_INTEGER) (x) >> (n))
#define OBNC_ROR(x, n) ((OBNC_INTEGER) (((unsigned OBNC_INTEGER) (x) >> (n)) | ((unsigned OBNC_INTEGER) (x) << ((sizeof (OBNC_INTEGER) << 3) - (size_t) (n)))))

/*Type conversions*/

#define OBNC_FLOOR(x) ((OBNC_INTEGER) OBNC_REAL_SUFFIX(floor)(x))
#define OBNC_FLT(x) ((OBNC_REAL) (x))
#define OBNC_ORD(x) ((OBNC_INTEGER) (x))
#define OBNC_CHR(x) ((char) (x))

/*Predefined proper procedures*/

#define OBNC_INC(v) (v)++
#define OBNC_INC_N(v, n) (v) += (n)

#define OBNC_DEC(v) (v)--
#define OBNC_DEC_N(v, n) (v) -= (n)

#define OBNC_INCL(v, x) (v) |= (1 << (x))
#define OBNC_EXCL(v, x) (v) &= ~((unsigned OBNC_INTEGER) 1 << (x))

#define OBNC_NEW(v, vtd, vHeapType, allocKind) \
	{ \
		vHeapType *p = OBNC_Allocate(sizeof *p, (allocKind)); \
		if (p != NULL) { \
			p->td = (vtd); \
			(v) = &p->fields; \
		} else { \
			(v) = NULL; \
		}\
	}

#define OBNC_NEW_ANON(v, allocKind) (v) = OBNC_Allocate(sizeof *(v), (allocKind))

#define OBNC_ASSERT(b, oberonFile, line) \
	if (! (b)) { \
		if (strcmp(#b, "0") == 0) { \
			OBNC_Exit(EXIT_FAILURE); \
		} else { \
			OBNC_handleTrap(OBNC_ASSERT_STATEMENT_EXCEPTION, (oberonFile), strlen(oberonFile) + 1, (line)); \
			OBNC_ExitTrap(); \
		} \
	}

#define OBNC_C_ASSERT(b) \
	if (! (b)) { \
		OBNC_handleTrap(OBNC_ASSERT_STATEMENT_EXCEPTION, OBNC_CFILE, sizeof OBNC_CFILE, __LINE__); \
		OBNC_ExitTrap(); \
	}

#define OBNC_PACK(x, n) (x) = OBNC_REAL_SUFFIX(ldexp)(x, n)

#if OBNC_CONFIG_C_INT_TYPE != OBNC_CONFIG_INT
	#define OBNC_UNPK(x, n) OBNC_Unpk(&(x), &(n))
#else
	#define OBNC_UNPK(x, n) (x) = OBNC_REAL_SUFFIX(frexp)(x, &(n)); (x) += (x); (n)--
#endif

/*SYSTEM procedures*/

#ifndef OBNC_ADR
	#define OBNC_ADR(v) ((OBNC_INTEGER) &(v))
#endif

#ifndef OBNC_SIZE
	#define OBNC_SIZE(T) ((OBNC_INTEGER) sizeof (T))
#endif

#ifndef OBNC_BIT
	#define OBNC_BIT(a, n) (*(volatile OBNC_INTEGER *) (a) & ((OBNC_INTEGER) 1 << (n)))
#endif

#ifndef OBNC_GET
	#define OBNC_GET(a, v, T) (v) = *(volatile T *) (a)
#endif

#ifndef OBNC_PUT
	#define OBNC_PUT(a, x, T) *(volatile T *) (a) = (x)
#endif

#ifndef OBNC_COPY
	#define OBNC_COPY(src, dst, n) \
		{ \
			int i; \
			for (i = 0; i < (n); i++) { \
				((volatile OBNC_INTEGER *) (dst))[i] = ((volatile OBNC_INTEGER *) (src))[i]; \
			} \
		}
#endif

#ifndef OBNC_VAL
	#define OBNC_VAL(T, n) ((T) (n))
#endif

/*Type descriptor accessor*/

#define OBNC_TD(ptr, heapType) (*(const OBNC_Td **) ((char *) (ptr) - offsetof (heapType, fields)))

/*Operators*/

#define OBNC_CMP(arr1, len1, arr2, len2) (strncmp((arr1), (arr2), ((len1) < (len2))? (len1): (len2)))

#define OBNC_IS(var, td, typeID, extLevel) (((var) != NULL) && ((extLevel) < (td)->nids) && ((td)->ids[extLevel] == (typeID)))

#define OBNC_DIV(x, y) (((x) >= 0)? (x) / (y): ((x) - OBNC_MOD(x, y)) / (y))

#define OBNC_MOD(x, y) (((x) >= 0)? (x) % (y): (((x) % (y)) + (y)) % (y))

#define OBNC_RANGE(m, n) \
	(((m) <= (n))? \
		(unsigned OBNC_INTEGER) ((((unsigned OBNC_INTEGER) -2) << (n)) ^ (((unsigned OBNC_INTEGER) -1)) << (m)): \
		(unsigned OBNC_INTEGER) 0x0u)

#define OBNC_IN(x, A) ((int) ((((unsigned OBNC_INTEGER) 1) << (x)) & (A)))

/*Structured assignments*/

#define OBNC_COPY_ARRAY(src, dst, n) memcpy(dst, src, (size_t) (n) * sizeof (src)[0])

/*Traps*/

#define OBNC_IT(index, length, line) \
	(((unsigned OBNC_INTEGER) (index) < (unsigned OBNC_INTEGER) (length)) \
		? (index) \
		: (OBNC_handleTrap(OBNC_ARRAY_INDEX_EXCEPTION, OBNC_OBNFILE, sizeof OBNC_OBNFILE, (line)), OBNC_ExitTrap(), (index)))

#define OBNC_IT1(index, length, line) (OBNC_It1((index), (length), OBNC_OBNFILE, (line)))

#define OBNC_RTT(recPtr, td, typeID, extLevel, line) \
	(OBNC_IS((recPtr), (td), (typeID), (extLevel)) \
		? (recPtr) \
		: (OBNC_handleTrap(OBNC_TYPE_GUARD_EXCEPTION, OBNC_OBNFILE, sizeof OBNC_OBNFILE, (line)), OBNC_ExitTrap(), (recPtr)))

#define OBNC_PTT(ptrPtr, td, typeID, extLevel, line) \
	((OBNC_IS(*(ptrPtr), (td), (typeID), (extLevel))) \
		? (ptrPtr) \
		: (OBNC_handleTrap(OBNC_TYPE_GUARD_EXCEPTION, OBNC_OBNFILE, sizeof OBNC_OBNFILE, (line)), OBNC_ExitTrap(), (ptrPtr)))

#define OBNC_AAT(sourceLen, targetLen, line) \
	if (sourceLen > targetLen) { \
		OBNC_handleTrap(OBNC_ARRAY_ASSIGNMENT_EXCEPTION, OBNC_OBNFILE, sizeof OBNC_OBNFILE, (line)); \
		OBNC_ExitTrap(); \
	}

#define OBNC_RAT(srcTD, dstTD, line) \
	if (! (((srcTD)->nids >= (dstTD)->nids) \
			&& ((srcTD)->ids[(dstTD)->nids - 1] == (dstTD)->ids[(dstTD)->nids - 1]))) { \
		OBNC_handleTrap(OBNC_RECORD_ASSIGNMENT_EXCEPTION, OBNC_OBNFILE, sizeof OBNC_OBNFILE, (line)); \
		OBNC_ExitTrap(); \
	}

#define OBNC_PT(ptr, line) \
	(((ptr) != NULL)? \
		(ptr): \
		(OBNC_handleTrap(OBNC_POINTER_DEREFERENCE_EXCEPTION, OBNC_OBNFILE, sizeof OBNC_OBNFILE, (line)), OBNC_ExitTrap(), (ptr)))

#define OBNC_PCT(ptr, line) \
	(((ptr) != NULL)? \
		(ptr): \
		(OBNC_handleTrap(OBNC_PROCEDURE_CALL_EXCEPTION, OBNC_OBNFILE, sizeof OBNC_OBNFILE, (line)), OBNC_ExitTrap(), (ptr)))

#define OBNC_CT(line) \
	OBNC_handleTrap(OBNC_CASE_EXP_MATCH_EXCEPTION, OBNC_OBNFILE, sizeof OBNC_OBNFILE, (line)); \
	OBNC_ExitTrap()

typedef struct {
	const int *const *ids; /*basetype IDs*/
	const int nids; /*length of ids*/
} OBNC_Td;

typedef void (*OBNC_TrapHandler)(OBNC_INTEGER exception, const char file[], OBNC_INTEGER fileLen, OBNC_INTEGER line);

extern int OBNC_argc;
extern char **OBNC_argv;
extern OBNC_TrapHandler OBNC_handleTrap;

void OBNC_Init(int argc, char *argv[]);

void OBNC_ExitTrap(void);

void *OBNC_Allocate(size_t size, int kind);

void OBNC_Exit(int status);

/*Functions used instead of the corresponding macros when a parameter contains a function call, which must not be evaluated more than once*/

int OBNC_Cmp(const char s[], OBNC_INTEGER sLen, const char t[], OBNC_INTEGER tLen);

OBNC_INTEGER OBNC_Div(OBNC_INTEGER x, OBNC_INTEGER y);

OBNC_INTEGER OBNC_It1(OBNC_INTEGER index, OBNC_INTEGER length, const char file[], int line);

OBNC_INTEGER OBNC_Mod(OBNC_INTEGER x, OBNC_INTEGER y);

unsigned OBNC_INTEGER OBNC_Range(OBNC_INTEGER m, OBNC_INTEGER n);

OBNC_INTEGER OBNC_Ror(OBNC_INTEGER x, OBNC_INTEGER n);

void OBNC_Pack(OBNC_REAL *x, OBNC_INTEGER n);

void OBNC_Unpk(OBNC_REAL *x, OBNC_INTEGER *n);

/*On small systems fprintf takes up too much memory*/

void OBNC_WriteInt(OBNC_INTEGER x, OBNC_INTEGER n, FILE *f);

void OBNC_WriteHex(unsigned OBNC_INTEGER x, FILE *f);

#endif
