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

#ifndef OBNC_H
#define OBNC_H

#include "OBNCConfig.h"
#include <limits.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Run-time exceptions*/

#define OBNC_ARRAY_ASSIGNMENT_EXCEPTION 0
#define OBNC_ARRAY_INDEX_EXCEPTION 1
#define OBNC_ORDER_RELATION_EXCEPTION 2
#define OBNC_POINTER_DEREFERENCE_EXCEPTION 3
#define OBNC_PROCEDURE_CALL_EXCEPTION 4
#define OBNC_RECORD_ASSIGNMENT_EXCEPTION 5
#define OBNC_TYPE_GUARD_EXCEPTION 6
#define OBNC_CASE_EXP_MATCH_EXCEPTION 7

/*Memory allocation kinds*/

#define OBNC_REGULAR_ALLOC 0
#define OBNC_ATOMIC_ALLOC 1
#define OBNC_ATOMIC_NOINIT_ALLOC 2

/*Size of type INTEGER and SET*/

#ifdef OBNC_CONFIG_USE_LONG_INT
	#define OBNC_LONGI long
	#define OBNC_INT_MOD "l"
#else
	#define OBNC_LONGI
	#define OBNC_INT_MOD ""
#endif

/*Size of type REAL*/

#ifdef OBNC_CONFIG_USE_LONG_REAL
	#define OBNC_LONGR long
	#define OBNC_REAL_MOD_R "L"
	#define OBNC_REAL_MOD_W "L"
#else
	#define OBNC_LONGR
	#define OBNC_REAL_MOD_R "l"
	#define OBNC_REAL_MOD_W ""
#endif

/*Predefined function procedures*/

#ifdef OBNC_CONFIG_USE_LONG_INT
	#define OBNC_ABS_INT(x) labs(x)
#else
	#define OBNC_ABS_INT(x) abs(x)
#endif
#ifdef OBNC_CONFIG_USE_LONG_REAL
	#define OBNC_ABS_FLT(x) fabsl(x)
#else
	#define OBNC_ABS_FLT(x) fabs(x)
#endif
#define OBNC_ODD(x) (((x) & 1) == 1)
#define OBNC_LSL(x, n) ((OBNC_LONGI int) (x) << (n))
#define OBNC_ASR(x, n) ((OBNC_LONGI int) (x) >> (n))
#define OBNC_ROR(x, n) (((OBNC_LONGI unsigned int) (x) >> (n)) | ((OBNC_LONGI unsigned int) (x) << ((sizeof (OBNC_LONGI int) << 3) - (n))))

/*Type conversions*/

#ifdef OBNC_CONFIG_USE_LONG_REAL
	#define OBNC_FLOOR(x) ((OBNC_LONGI int) floorl(x))
#else
	#define OBNC_FLOOR(x) ((OBNC_LONGI int) floor(x))
#endif
#define OBNC_FLT(x) ((OBNC_LONGR double) (x))
#define OBNC_ORD(x) ((OBNC_LONGI int) (x))
#define OBNC_CHR(x) ((char) (x))

/*Predefined proper procedures*/

#define OBNC_INC(v) (v)++
#define OBNC_INC_N(v, n) (v) += (n)

#define OBNC_DEC(v) (v)--
#define OBNC_DEC_N(v, n) (v) -= (n)

#define OBNC_INCL(v, x) (v) |= (1 << (x))
#define OBNC_EXCL(v, x) (v) &= ~(1 << (x))

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
		fprintf(stderr, "Assertion failed at line %d in file %s\n", (line), (oberonFile)); \
		abort(); \
	}

#ifdef OBNC_CONFIG_USE_LONG_REAL
	#define OBNC_PACK(x, n) (x) = ldexpl(x, n)
#else
	#define OBNC_PACK(x, n) (x) = ldexp(x, n)
#endif

#ifdef OBNC_CONFIG_USE_LONG_INT
	#define OBNC_UNPK(x, n) OBNC_Unpk(&(x), &(n))
#elif defined(OBNC_CONFIG_USE_LONG_REAL)
	#define OBNC_UNPK(x, n) (x) = frexpl(x, &(n)); (x) += (x); (n)--
#else
	#define OBNC_UNPK(x, n) (x) = frexp(x, &(n)); (x) += (x); (n)--
#endif

/*Type descriptor accessor*/

#define OBNC_TD(ptr, heapType) (*(const OBNC_Td **) ((char *) (ptr) - offsetof (heapType, fields)))

/*Operators*/

#define OBNC_CMP(str1, len1, str2, len2) OBNC_Cmp((str1), (len1), (str2), (len2), __FILE__, __LINE__)

#define OBNC_IS(td, typeID, extLevel) (((extLevel) < (td)->nids) && ((td)->ids[extLevel] == (typeID)))

#define OBNC_DIV(x, y) (((x) >= 0)? (x) / (y): ((x) - OBNC_MOD(x, y)) / (y))

#define OBNC_MOD(x, y) (((x) >= 0)? (x) % (y): (((x) % (y)) + (y)) % (y))

#define OBNC_RANGE(m, n) (((m) <= (n))? (((OBNC_LONGI unsigned int) -2) << (n)) ^ (((OBNC_LONGI unsigned int) -1) << (m)): 0x0u)

#define OBNC_IN(x, A) ((((OBNC_LONGI unsigned int) 1) << (x)) & (A))

/*Structured assignments*/

#define OBNC_COPY_ARRAY(src, dst, n) memcpy(dst, src, (n) * sizeof (src)[0])

/*Traps*/

#define OBNC_IT(index, length) \
	(((OBNC_LONGI unsigned int) (index) < (OBNC_LONGI int) (length)) \
		? (index) \
		: (OBNC_Abort(OBNC_ARRAY_INDEX_EXCEPTION, __FILE__, __LINE__), (index)))
		
#define OBNC_IT1(index, length) (OBNC_It1((index), (length), __FILE__, __LINE__))

#define OBNC_RTT(recPtr, td, typeID, extLevel) \
	(OBNC_IS((td), (typeID), (extLevel)) \
		? (recPtr) \
		: (OBNC_Abort(OBNC_TYPE_GUARD_EXCEPTION, __FILE__, __LINE__), (recPtr)))

#define OBNC_PTT(ptrPtr, td, typeID, extLevel) \
	(((*(ptrPtr) != NULL) && OBNC_IS((td), (typeID), (extLevel))) \
		? (ptrPtr) \
		: (OBNC_Abort(OBNC_TYPE_GUARD_EXCEPTION, __FILE__, __LINE__), (ptrPtr)))

#define OBNC_AAT(sourceLen, targetLen) \
	if (sourceLen > targetLen) { \
		OBNC_Abort(OBNC_ARRAY_ASSIGNMENT_EXCEPTION, __FILE__, __LINE__); \
	}

#define OBNC_RAT(srcTD, dstTD) \
	if (! (((srcTD)->nids >= (dstTD)->nids) \
			&& ((srcTD)->ids[(dstTD)->nids - 1] == (dstTD)->ids[(dstTD)->nids - 1]))) { \
		OBNC_Abort(OBNC_RECORD_ASSIGNMENT_EXCEPTION, __FILE__, __LINE__); \
	}

#define OBNC_PT(ptr) (((ptr) != NULL)? (ptr): (OBNC_Abort(OBNC_POINTER_DEREFERENCE_EXCEPTION, __FILE__, __LINE__), (ptr)))

#define OBNC_PCT(ptr) (((ptr) != NULL)? (ptr): (OBNC_Abort(OBNC_PROCEDURE_CALL_EXCEPTION, __FILE__, __LINE__), (ptr)))

#define OBNC_CT OBNC_Abort(OBNC_CASE_EXP_MATCH_EXCEPTION, __FILE__, __LINE__)

typedef struct {
	const int *const *ids; /*basetype IDs*/
	const int nids; /*length of ids*/
} OBNC_Td;

extern int OBNC_argc;
extern char **OBNC_argv;

void OBNC_Initialize(int argc, char *argv[]);

int OBNC_Cmp(const char s[], OBNC_LONGI int sLen, const char t[], OBNC_LONGI int tLen, const char file[], int line);

void *OBNC_Allocate(size_t size, int kind);

int OBNC_It1(OBNC_LONGI int index, OBNC_LONGI int length, const char file[], int line);

void OBNC_Abort(int exception, const char file[], int line);

/*Functions used instead of the corresponding macros when a parameter contains a function call, which must not be evaluated more than once*/

OBNC_LONGI int OBNC_Div(OBNC_LONGI int x, OBNC_LONGI int y);

OBNC_LONGI int OBNC_Mod(OBNC_LONGI int x, OBNC_LONGI int y);

OBNC_LONGI int OBNC_Range(OBNC_LONGI int m, OBNC_LONGI int n);

OBNC_LONGI int OBNC_Ror(OBNC_LONGI int x, OBNC_LONGI int n);

void OBNC_Pack(OBNC_LONGR double *x, OBNC_LONGI int n);

void OBNC_Unpk(OBNC_LONGR double *x, OBNC_LONGI int *n);

#endif
