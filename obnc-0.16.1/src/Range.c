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

#include "Range.h"
#include "Oberon.h"
#include <float.h>
#include <limits.h>
#include <stdlib.h>

#define SHIFT_COUNT_MAX ((int) (CHAR_BIT * sizeof (OBNC_INTEGER) - 1))
#define SET_ELEMENT_MAX ((int) (CHAR_BIT * sizeof (unsigned OBNC_INTEGER) - 1))

void Range_CheckIntSum(OBNC_INTEGER a, OBNC_INTEGER b)
{
	if ((b > 0) && (a > OBNC_INT_MAX - b)) {
		Oberon_PrintError("warning: integer overflow: %" OBNC_INT_MOD "d + %" OBNC_INT_MOD "d > %" OBNC_INT_MOD "d", a, b, (OBNC_INTEGER) OBNC_INT_MAX);
	} else if ((b < 0) && (a < OBNC_INT_MIN - b)) {
		Oberon_PrintError("warning: integer overflow: %" OBNC_INT_MOD "d + (%" OBNC_INT_MOD "d) < %" OBNC_INT_MOD "d", a, b, (OBNC_INTEGER) OBNC_INT_MIN);
	}
}


void Range_CheckIntDiff(OBNC_INTEGER a, OBNC_INTEGER b)
{
	if ((b < 0) && (a > OBNC_INT_MAX + b)) {
		Oberon_PrintError("warning: integer overflow: %" OBNC_INT_MOD "d - (%" OBNC_INT_MOD "d) > %" OBNC_INT_MOD "d", a, b, (OBNC_INTEGER) OBNC_INT_MAX);
	} else if ((b > 0) && (a < OBNC_INT_MIN + b)) {
		Oberon_PrintError("warning: integer overflow: %" OBNC_INT_MOD "d - %" OBNC_INT_MOD "d < %" OBNC_INT_MOD "d", a, b, (OBNC_INTEGER) OBNC_INT_MIN);
	}
}


void Range_CheckIntProd(OBNC_INTEGER a, OBNC_INTEGER b)
{
	if (b > 0) {
		if ((a > 0) && (a > OBNC_INT_MAX / b)) {
			Oberon_PrintError("warning: integer overflow: %" OBNC_INT_MOD "d * %" OBNC_INT_MOD "d > %" OBNC_INT_MOD "d", a, b, (OBNC_INTEGER) OBNC_INT_MAX);
		} else if ((a < 0) && (a < OBNC_INT_MIN / b)) {
			Oberon_PrintError("warning: integer overflow: (%" OBNC_INT_MOD "d) * %" OBNC_INT_MOD "d < %" OBNC_INT_MOD "d", a, b, (OBNC_INTEGER) OBNC_INT_MIN);
		}
	} else if (b < 0) {
		if ((a > 0) && (a < OBNC_INT_MIN / b)) {
			Oberon_PrintError("warning: integer overflow: %" OBNC_INT_MOD "d * (%" OBNC_INT_MOD "d) < %" OBNC_INT_MOD "d", a, b, (OBNC_INTEGER) OBNC_INT_MIN);
		} else if ((a < 0) && (a < OBNC_INT_MAX / b)) {
			Oberon_PrintError("warning: integer overflow: (%" OBNC_INT_MOD "d) * (%" OBNC_INT_MOD "d) > %" OBNC_INT_MOD "d", a, b, (OBNC_INTEGER) OBNC_INT_MAX);
		}
	}
}


void Range_CheckLSL(OBNC_INTEGER n)
{
	if (n < 0) {
		Oberon_PrintError("warning: negative left shift count: %" OBNC_INT_MOD "d < 0", n);
	} else if (n > SHIFT_COUNT_MAX ) {
		Oberon_PrintError("warning: left shift count exceeds maximum: %" OBNC_INT_MOD "d > %lu", n, (long unsigned int) SHIFT_COUNT_MAX);
	}
}


void Range_CheckASR(OBNC_INTEGER n)
{
	if (n < 0) {
		Oberon_PrintError("warning: negative right shift count: %" OBNC_INT_MOD "d < 0", n);
	} else if (n > SHIFT_COUNT_MAX) {
		Oberon_PrintError("warning: right shift count exceeds maximum: %" OBNC_INT_MOD "d > %lu", n, (long unsigned int) SHIFT_COUNT_MAX);
	}
}


void Range_CheckROR(OBNC_INTEGER n)
{
	if (n < 1) {
		Oberon_PrintError("warning: non-positive rotation: %" OBNC_INT_MOD "d", n);
	} else if (n > SHIFT_COUNT_MAX) {
		Oberon_PrintError("warning: rotation exceeds maximum: %" OBNC_INT_MOD "d > %lu", n, (long unsigned int) SHIFT_COUNT_MAX);
	}
}


void Range_CheckBIT(OBNC_INTEGER n)
{
	if (n < 0) {
		Oberon_PrintError("warning: negative bit position: %" OBNC_INT_MOD "d < 0", n);
	} else if (n > SHIFT_COUNT_MAX) {
		Oberon_PrintError("warning: bit position exceeds maximum: %" OBNC_INT_MOD "d > %lu", n, (long unsigned int) SHIFT_COUNT_MAX);
	}
}


void Range_CheckFLOOR(OBNC_REAL x)
{
	if (x < (OBNC_REAL) INT_MIN) {
		Oberon_PrintError("warning: parameter in FLOOR too large for truncation to INTEGER: %" OBNC_REAL_MOD_W "E < %" OBNC_REAL_MOD_W "E", x, (OBNC_REAL) INT_MIN);
	} else if (x >= (OBNC_REAL) INT_MAX + 1.0) {
		Oberon_PrintError("warning: parameter in FLOOR too large for truncation to INTEGER: %" OBNC_REAL_MOD_W "E >= %" OBNC_REAL_MOD_W "E", x, (OBNC_REAL) INT_MAX + 1.0);
	}
}


void Range_CheckCHR(OBNC_INTEGER n)
{
	if (n < 0) {
		Oberon_PrintError("warning: negative parameter in CHR: %" OBNC_INT_MOD "d", n);
	} else if (n > CHAR_MAX) {
		Oberon_PrintError("warning: parameter in CHR too large for conversion: %" OBNC_INT_MOD "d > %d", n, CHAR_MAX);
	}
}


void Range_CheckRealSum(OBNC_REAL x, OBNC_REAL y)
{
	if ((y > 0.0) && (x > OBNC_REAL_MAX - y)) {
		Oberon_PrintError("warning: real number overflow: %" OBNC_REAL_MOD_W "G + %" OBNC_REAL_MOD_W "G > %" OBNC_REAL_MOD_W "G", x, y, OBNC_REAL_MAX);
	} else if ((y < 0.0) && (x < -OBNC_REAL_MAX - y)) {
		Oberon_PrintError("warning: real number overflow: %" OBNC_REAL_MOD_W "G + (%" OBNC_REAL_MOD_W "G) < %" OBNC_REAL_MOD_W "G", x, y, -OBNC_REAL_MAX);
	}
}


void Range_CheckRealDiff(OBNC_REAL x, OBNC_REAL y)
{
	if ((y < 0.0) && (x > OBNC_REAL_MAX + y)) {
		Oberon_PrintError("warning: real number overflow: %" OBNC_REAL_MOD_W "G - (%" OBNC_REAL_MOD_W "G) > %" OBNC_REAL_MOD_W "G", x, y, OBNC_REAL_MAX);
	} else if ((y > 0.0) && (x < -OBNC_REAL_MAX + y)) {
		Oberon_PrintError("warning: real number overflow: %" OBNC_REAL_MOD_W "G - %" OBNC_REAL_MOD_W "G < %" OBNC_REAL_MOD_W "G", x, y, -OBNC_REAL_MAX);
	}
}


void Range_CheckRealProd(OBNC_REAL x, OBNC_REAL y)
{
	if (y > 0.0) {
		if ((x > 0.0) && (x > OBNC_REAL_MAX / y)) {
			Oberon_PrintError("warning: real number overflow: %" OBNC_REAL_MOD_W "G * %" OBNC_REAL_MOD_W "G > %" OBNC_REAL_MOD_W "G", x, y, OBNC_REAL_MAX);
		} else if ((x < 0.0) && (x < -OBNC_REAL_MAX / y)) {
			Oberon_PrintError("warning: real number overflow: (%" OBNC_REAL_MOD_W "G) * %" OBNC_REAL_MOD_W "G < %" OBNC_REAL_MOD_W "G", x, y, -OBNC_REAL_MAX);
		}
	} else if (y < 0.0) {
		if ((x > 0.0) && (x < -OBNC_REAL_MAX / y)) {
			Oberon_PrintError("warning: real number overflow: %" OBNC_REAL_MOD_W "G * (%" OBNC_REAL_MOD_W "G) < %" OBNC_REAL_MOD_W "G", x, y, -OBNC_REAL_MAX);
		} else if ((x < 0.0) && (x < OBNC_REAL_MAX / y)) {
			Oberon_PrintError("warning: real number overflow: (%" OBNC_REAL_MOD_W "G) * (%" OBNC_REAL_MOD_W "G) > %" OBNC_REAL_MOD_W "G", x, y, OBNC_REAL_MAX);
		}
	}
}


void Range_CheckByte(OBNC_INTEGER n)
{
	if (n < 0) {
		Oberon_PrintError("warning: value less than BYTE minimum: %" OBNC_INT_MOD "d < 0", n);
	} else if (n > UCHAR_MAX) {
		Oberon_PrintError("warning: BYTE maximum exceeded: %" OBNC_INT_MOD "d > %d", n, UCHAR_MAX);
	}
}


void Range_CheckSetElement(OBNC_INTEGER x)
{
	if (x < 0) {
		Oberon_PrintError("error: negative set element: %" OBNC_INT_MOD "d", x);
		exit(EXIT_FAILURE);
	} else if (x > SET_ELEMENT_MAX) {
		Oberon_PrintError("warning: set element exceededs maximum: %" OBNC_INT_MOD "d > %lu", x, (long unsigned int) SET_ELEMENT_MAX);
	}
}
