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

#include "Range.h"
#include "Oberon.h"
#include <float.h>
#include <limits.h>
#include <stdlib.h>

#define SHIFT_COUNT_MAX ((int) (CHAR_BIT * sizeof (OBNC_LONGI int) - 1))
#define SET_ELEMENT_MAX ((int) (CHAR_BIT * sizeof (OBNC_LONGI unsigned int) - 1))

void Range_CheckIntSum(OBNC_LONGI int a, OBNC_LONGI int b)
{
	if ((b > 0) && (a > OBNC_INT_MAX - b)) {
		yyerror("warning: integer overflow: %" OBNC_INT_MOD "d + %" OBNC_INT_MOD "d > %" OBNC_INT_MOD "d", a, b, OBNC_INT_MAX);
	} else if ((b < 0) && (a < OBNC_INT_MIN - b)) {
		yyerror("warning: integer overflow: %" OBNC_INT_MOD "d + (%" OBNC_INT_MOD "d) < %" OBNC_INT_MOD "d", a, b, OBNC_INT_MIN);
	}
}


void Range_CheckIntDiff(OBNC_LONGI int a, OBNC_LONGI int b)
{
	if ((b < 0) && (a > OBNC_INT_MAX + b)) {
		yyerror("warning: integer overflow: %" OBNC_INT_MOD "d - (%" OBNC_INT_MOD "d) > %" OBNC_INT_MOD "d", a, b, OBNC_INT_MAX);
	} else if ((b > 0) && (a < OBNC_INT_MIN + b)) {
		yyerror("warning: integer overflow: %" OBNC_INT_MOD "d - %" OBNC_INT_MOD "d < %" OBNC_INT_MOD "d", a, b, OBNC_INT_MIN);
	}
}


void Range_CheckIntProd(OBNC_LONGI int a, OBNC_LONGI int b)
{
	if (b > 0) {
		if ((a > 0) && (a > OBNC_INT_MAX / b)) {
			yyerror("warning: integer overflow: %" OBNC_INT_MOD "d * %" OBNC_INT_MOD "d > %" OBNC_INT_MOD "d", a, b, OBNC_INT_MAX);
		} else if ((a < 0) && (a < OBNC_INT_MIN / b)) {
			yyerror("warning: integer overflow: (%" OBNC_INT_MOD "d) * %" OBNC_INT_MOD "d < %" OBNC_INT_MOD "d", a, b, OBNC_INT_MIN);
		}
	} else if (b < 0) {
		if ((a > 0) && (a < OBNC_INT_MIN / b)) {
			yyerror("warning: integer overflow: %" OBNC_INT_MOD "d * (%" OBNC_INT_MOD "d) < %" OBNC_INT_MOD "d", a, b, OBNC_INT_MIN);
		} else if ((a < 0) && (a < OBNC_INT_MAX / b)) {
			yyerror("warning: integer overflow: (%" OBNC_INT_MOD "d) * (%" OBNC_INT_MOD "d) > %" OBNC_INT_MOD "d", a, b, OBNC_INT_MAX);
		}
	}
}


void Range_CheckLSL(OBNC_LONGI int n)
{
	if (n < 0) {
		yyerror("warning: negative left shift count: %" OBNC_INT_MOD "d < 0", n);
	} else if (n > SHIFT_COUNT_MAX ) {
		yyerror("warning: left shift count exceeds maximum: %" OBNC_INT_MOD "d > %lu", n, (long unsigned int) SHIFT_COUNT_MAX);
	}
}


void Range_CheckASR(OBNC_LONGI int n)
{
	if (n < 0) {
		yyerror("warning: negative right shift count: %" OBNC_INT_MOD "d < 0", n);
	} else if (n > SHIFT_COUNT_MAX) {
		yyerror("warning: right shift count exceeds maximum: %" OBNC_INT_MOD "d > %lu", n, (long unsigned int) SHIFT_COUNT_MAX);
	}
}


void Range_CheckROR(OBNC_LONGI int n)
{
	if (n < 1) {
		yyerror("warning: non-positive rotation: %" OBNC_INT_MOD "d", n);
	} else if (n > SHIFT_COUNT_MAX) {
		yyerror("warning: rotation exceeds maximum: %" OBNC_INT_MOD "d > %lu", n, (long unsigned int) SHIFT_COUNT_MAX);
	}
}


void Range_CheckBIT(OBNC_LONGI int n)
{
	if (n < 0) {
		yyerror("warning: negative bit position: %" OBNC_INT_MOD "d < 0", n);
	} else if (n > SHIFT_COUNT_MAX) {
		yyerror("warning: bit position exceeds maximum: %" OBNC_INT_MOD "d > %lu", n, (long unsigned int) SHIFT_COUNT_MAX);
	}
}


void Range_CheckFLOOR(OBNC_LONGR double x)
{
	if (x < (OBNC_LONGR double) INT_MIN) {
		yyerror("warning: parameter in FLOOR too large for truncation to INTEGER: %" OBNC_REAL_MOD_W "E < %" OBNC_REAL_MOD_W "E", x, (OBNC_LONGR double) INT_MIN);
	} else if (x >= (OBNC_LONGR double) INT_MAX + 1.0) {
		yyerror("warning: parameter in FLOOR too large for truncation to INTEGER: %" OBNC_REAL_MOD_W "E >= %" OBNC_REAL_MOD_W "E", x, (OBNC_LONGR double) INT_MAX + 1.0);
	}
}


void Range_CheckCHR(OBNC_LONGI int n)
{
	if (n < 0) {
		yyerror("warning: negative parameter in CHR: %" OBNC_INT_MOD "d", n);
	} else if (n > CHAR_MAX) {
		yyerror("warning: parameter in CHR too large for conversion: %" OBNC_INT_MOD "d > %d", n, CHAR_MAX);
	}
}


void Range_CheckRealSum(OBNC_LONGR double x, OBNC_LONGR double y)
{
	if ((y > 0.0) && (x > OBNC_REAL_MAX - y)) {
		yyerror("warning: real number overflow: %" OBNC_REAL_MOD_W "G + %" OBNC_REAL_MOD_W "G > %" OBNC_REAL_MOD_W "G", x, y, OBNC_REAL_MAX);
	} else if ((y < 0.0) && (x < -OBNC_REAL_MAX - y)) {
		yyerror("warning: real number overflow: %" OBNC_REAL_MOD_W "G + (%" OBNC_REAL_MOD_W "G) < %" OBNC_REAL_MOD_W "G", x, y, -OBNC_REAL_MAX);
	}
}


void Range_CheckRealDiff(OBNC_LONGR double x, OBNC_LONGR double y)
{
	if ((y < 0.0) && (x > OBNC_REAL_MAX + y)) {
		yyerror("warning: real number overflow: %" OBNC_REAL_MOD_W "G - (%" OBNC_REAL_MOD_W "G) > %" OBNC_REAL_MOD_W "G", x, y, OBNC_REAL_MAX);
	} else if ((y > 0.0) && (x < -OBNC_REAL_MAX + y)) {
		yyerror("warning: real number overflow: %" OBNC_REAL_MOD_W "G - %" OBNC_REAL_MOD_W "G < %" OBNC_REAL_MOD_W "G", x, y, -OBNC_REAL_MAX);
	}
}


void Range_CheckRealProd(OBNC_LONGR double x, OBNC_LONGR double y)
{
	if (y > 0.0) {
		if ((x > 0.0) && (x > OBNC_REAL_MAX / y)) {
			yyerror("warning: real number overflow: %" OBNC_REAL_MOD_W "G * %" OBNC_REAL_MOD_W "G > %" OBNC_REAL_MOD_W "G", x, y, OBNC_REAL_MAX);
		} else if ((x < 0.0) && (x < -OBNC_REAL_MAX / y)) {
			yyerror("warning: real number overflow: (%" OBNC_REAL_MOD_W "G) * %" OBNC_REAL_MOD_W "G < %" OBNC_REAL_MOD_W "G", x, y, -OBNC_REAL_MAX);
		}
	} else if (y < 0.0) {
		if ((x > 0.0) && (x < -OBNC_REAL_MAX / y)) {
			yyerror("warning: real number overflow: %" OBNC_REAL_MOD_W "G * (%" OBNC_REAL_MOD_W "G) < %" OBNC_REAL_MOD_W "G", x, y, -OBNC_REAL_MAX);
		} else if ((x < 0.0) && (x < OBNC_REAL_MAX / y)) {
			yyerror("warning: real number overflow: (%" OBNC_REAL_MOD_W "G) * (%" OBNC_REAL_MOD_W "G) > %" OBNC_REAL_MOD_W "G", x, y, OBNC_REAL_MAX);
		}
	}
}


void Range_CheckByte(OBNC_LONGI int n)
{
	if (n < 0) {
		yyerror("warning: value less than BYTE minimum: %" OBNC_INT_MOD "d < 0", n);
	} else if (n > UCHAR_MAX) {
		yyerror("warning: BYTE maximum exceeded: %" OBNC_INT_MOD "d > %d", n, UCHAR_MAX);
	}
}


void Range_CheckSetElement(OBNC_LONGI int x)
{
	if (x < 0) {
		yyerror("error: negative set element: %" OBNC_INT_MOD "d", x);
		exit(EXIT_FAILURE);
	} else if (x > SET_ELEMENT_MAX) {
		yyerror("warning: set element exceededs maximum: %" OBNC_INT_MOD "d > %lu", x, (long unsigned int) SET_ELEMENT_MAX);
	}
}
