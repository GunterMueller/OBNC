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

#include "Range.h"
#include "Oberon.h"
#include <float.h>
#include <limits.h>
#include <stdlib.h>

#ifdef OBNC_CONFIG_USE_LONG_INT
	#define INTEGER_MIN LONG_MIN
	#define INTEGER_MAX LONG_MAX
#else
	#define INTEGER_MIN INT_MIN
	#define INTEGER_MAX INT_MAX
#endif

#ifdef OBNC_CONFIG_USE_LONG_REAL
	#define REAL_MAX LDBL_MAX
#else
	#define REAL_MAX DBL_MAX
#endif

#define SHIFT_COUNT_MAX ((int) (CHAR_BIT * sizeof (OBNC_LONGI int) - 1))
#define SET_ELEMENT_MAX ((int) (CHAR_BIT * sizeof (OBNC_LONGI unsigned int) - 1))

void Range_CheckIntSum(OBNC_LONGI int a, OBNC_LONGI int b)
{
	if ((b > 0) && (a > INTEGER_MAX - b)) {
		Oberon_PrintContext();
		fprintf(stderr, "warning: integer overflow: %" OBNC_INT_MOD "d + %" OBNC_INT_MOD "d > %" OBNC_INT_MOD "d\n", a, b, INTEGER_MAX);
	} else if ((b < 0) && (a < INTEGER_MIN - b)) {
		Oberon_PrintContext();
		fprintf(stderr, "warning: integer overflow: %" OBNC_INT_MOD "d + (%" OBNC_INT_MOD "d) < %" OBNC_INT_MOD "d\n", a, b, INTEGER_MIN);
	}
}


void Range_CheckIntDiff(OBNC_LONGI int a, OBNC_LONGI int b)
{
	if ((b < 0) && (a > INTEGER_MAX + b)) {
		Oberon_PrintContext();
		fprintf(stderr, "warning: integer overflow: %" OBNC_INT_MOD "d - (%" OBNC_INT_MOD "d) > %" OBNC_INT_MOD "d\n", a, b, INTEGER_MAX);
	} else if ((b > 0) && (a < INTEGER_MIN + b)) {
		Oberon_PrintContext();
		fprintf(stderr, "warning: integer overflow: %" OBNC_INT_MOD "d - %" OBNC_INT_MOD "d < %" OBNC_INT_MOD "d\n", a, b, INTEGER_MIN);
	}
}


void Range_CheckIntProd(OBNC_LONGI int a, OBNC_LONGI int b)
{
	if (b > 0) {
		if ((a > 0) && (a > INTEGER_MAX / b)) {
			Oberon_PrintContext();
			fprintf(stderr, "warning: integer overflow: %" OBNC_INT_MOD "d * %" OBNC_INT_MOD "d > %" OBNC_INT_MOD "d\n", a, b, INTEGER_MAX);
		} else if ((a < 0) && (a < INTEGER_MIN / b)) {
			Oberon_PrintContext();
			fprintf(stderr, "warning: integer overflow: (%" OBNC_INT_MOD "d) * %" OBNC_INT_MOD "d < %" OBNC_INT_MOD "d\n", a, b, INTEGER_MIN);
		}
	} else if (b < 0) {
		if ((a > 0) && (a < INTEGER_MIN / b)) {
			Oberon_PrintContext();
			fprintf(stderr, "warning: integer overflow: %" OBNC_INT_MOD "d * (%" OBNC_INT_MOD "d) < %" OBNC_INT_MOD "d\n", a, b, INTEGER_MIN);
		} else if ((a < 0) && (a < INTEGER_MAX / b)) {
			Oberon_PrintContext();
			fprintf(stderr, "warning: integer overflow: (%" OBNC_INT_MOD "d) * (%" OBNC_INT_MOD "d) > %" OBNC_INT_MOD "d\n", a, b, INTEGER_MAX);
		}
	}
}


void Range_CheckLSL(OBNC_LONGI int n)
{
	if (n < 0) {
		Oberon_PrintContext();
		fprintf(stderr, "warning: negative left shift count: %" OBNC_INT_MOD "d < 0\n", n);
	} else if (n > SHIFT_COUNT_MAX ) {
		Oberon_PrintContext();
		fprintf(stderr, "warning: left shift count exceeds maximum: %" OBNC_INT_MOD "d > %lu\n", n, (long unsigned int) SHIFT_COUNT_MAX);
	}
}


void Range_CheckASR(OBNC_LONGI int n)
{
	if (n < 0) {
		Oberon_PrintContext();
		fprintf(stderr, "warning: negative right shift count: %" OBNC_INT_MOD "d < 0\n", n);
	} else if (n > SHIFT_COUNT_MAX) {
		Oberon_PrintContext();
		fprintf(stderr, "warning: right shift count exceeds maximum: %" OBNC_INT_MOD "d > %lu\n", n, (long unsigned int) SHIFT_COUNT_MAX);
	}
}


void Range_CheckROR(OBNC_LONGI int n)
{
	if (n < 1) {
		Oberon_PrintContext();
		fprintf(stderr, "warning: non-positive rotation: %" OBNC_INT_MOD "d\n", n);
	} else if (n > SHIFT_COUNT_MAX) {
		Oberon_PrintContext();
		fprintf(stderr, "warning: rotation exceeds maximum: %" OBNC_INT_MOD "d > %lu\n", n, (long unsigned int) SHIFT_COUNT_MAX);
	}
}


void Range_CheckFLOOR(OBNC_LONGR double x)
{
	if (x < (OBNC_LONGR double) INT_MIN) {
		Oberon_PrintContext();
		fprintf(stderr, "warning: parameter in FLOOR too large for truncation to INTEGER: %" OBNC_REAL_MOD_W "E < %" OBNC_REAL_MOD_W "E\n", x, (OBNC_LONGR double) INT_MIN);
	} else if (x >= (OBNC_LONGR double) INT_MAX + 1.0) {
		Oberon_PrintContext();
		fprintf(stderr, "warning: parameter in FLOOR too large for truncation to INTEGER: %" OBNC_REAL_MOD_W "E >= %" OBNC_REAL_MOD_W "E\n", x, (OBNC_LONGR double) INT_MAX + 1.0);
	}
}


void Range_CheckCHR(OBNC_LONGI int n)
{
	if (n < 0) {
		Oberon_PrintContext();
		fprintf(stderr, "error: negative parameter in CHR: %" OBNC_INT_MOD "d\n", n);
		exit(EXIT_FAILURE);
	} else if (n > CHAR_MAX) {
		Oberon_PrintContext();
		fprintf(stderr, "warning: parameter in CHR too large for conversion: %" OBNC_INT_MOD "d > %d\n", n, CHAR_MAX);
	}
}


void Range_CheckRealSum(OBNC_LONGR double x, OBNC_LONGR double y)
{
	if ((y > 0.0) && (x > REAL_MAX - y)) {
		Oberon_PrintContext();
		fprintf(stderr, "warning: real number overflow: %" OBNC_REAL_MOD_W "G + %" OBNC_REAL_MOD_W "G > %" OBNC_REAL_MOD_W "G\n", x, y, REAL_MAX);
	} else if ((y < 0.0) && (x < -REAL_MAX - y)) {
		Oberon_PrintContext();
		fprintf(stderr, "warning: real number overflow: %" OBNC_REAL_MOD_W "G + (%" OBNC_REAL_MOD_W "G) < %" OBNC_REAL_MOD_W "G\n", x, y, -REAL_MAX);
	}
}


void Range_CheckRealDiff(OBNC_LONGR double x, OBNC_LONGR double y)
{
	if ((y < 0.0) && (x > REAL_MAX + y)) {
		Oberon_PrintContext();
		fprintf(stderr, "warning: real number overflow: %" OBNC_REAL_MOD_W "G - (%" OBNC_REAL_MOD_W "G) > %" OBNC_REAL_MOD_W "G\n", x, y, REAL_MAX);
	} else if ((y > 0.0) && (x < -REAL_MAX + y)) {
		Oberon_PrintContext();
		fprintf(stderr, "warning: real number overflow: %" OBNC_REAL_MOD_W "G - %" OBNC_REAL_MOD_W "G < %" OBNC_REAL_MOD_W "G\n", x, y, -REAL_MAX);
	}
}


void Range_CheckRealProd(OBNC_LONGR double x, OBNC_LONGR double y)
{
	if (y > 0.0) {
		if ((x > 0.0) && (x > REAL_MAX / y)) {
			Oberon_PrintContext();
			fprintf(stderr, "warning: real number overflow: %" OBNC_REAL_MOD_W "G * %" OBNC_REAL_MOD_W "G > %" OBNC_REAL_MOD_W "G\n", x, y, REAL_MAX);
		} else if ((x < 0.0) && (x < -REAL_MAX / y)) {
			Oberon_PrintContext();
			fprintf(stderr, "warning: real number overflow: (%" OBNC_REAL_MOD_W "G) * %" OBNC_REAL_MOD_W "G < %" OBNC_REAL_MOD_W "G\n", x, y, -REAL_MAX);
		}
	} else if (y < 0.0) {
		if ((x > 0.0) && (x < -REAL_MAX / y)) {
			Oberon_PrintContext();
			fprintf(stderr, "warning: real number overflow: %" OBNC_REAL_MOD_W "G * (%" OBNC_REAL_MOD_W "G) < %" OBNC_REAL_MOD_W "G\n", x, y, -REAL_MAX);
		} else if ((x < 0.0) && (x < REAL_MAX / y)) {
			Oberon_PrintContext();
			fprintf(stderr, "warning: real number overflow: (%" OBNC_REAL_MOD_W "G) * (%" OBNC_REAL_MOD_W "G) > %" OBNC_REAL_MOD_W "G\n", x, y, REAL_MAX);
		}
	}
}


void Range_CheckByte(OBNC_LONGI int n)
{
	if (n < 0) {
		Oberon_PrintContext();
		fprintf(stderr, "warning: negative byte overflow: %" OBNC_INT_MOD "d < 0\n", n);
	} else if (n > UCHAR_MAX) {
		Oberon_PrintContext();
		fprintf(stderr, "warning: byte overflow: %" OBNC_INT_MOD "d > %d\n", n, UCHAR_MAX);
	}
}


void Range_CheckSetElement(OBNC_LONGI int x)
{
	if (x < 0) {
		Oberon_PrintContext();
		fprintf(stderr, "error: negative set element: %" OBNC_INT_MOD "d\n", x);
		exit(EXIT_FAILURE);
	} else if (x > SET_ELEMENT_MAX) {
		Oberon_PrintContext();
		fprintf(stderr, "warning: set element exceededs maximum: %" OBNC_INT_MOD "d > %lu\n", x, (long unsigned int) SET_ELEMENT_MAX);
	}
}
