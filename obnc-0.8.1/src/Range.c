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
#include "yyerror.h"
#include <float.h>
#include <limits.h>
#include <stdlib.h>

#define SHIFT_COUNT_MAX (CHAR_BIT * sizeof (int) - 1)
#define SET_ELEMENT_MAX (CHAR_BIT * sizeof (unsigned int) - 1)

void Range_CheckIntSum(int a, int b)
{
	if ((b > 0) && (a > INT_MAX - b)) {
		yyerror("warning: integer overflow: %d + %d > %d", a, b, INT_MAX);
	} else if ((b < 0) && (a < INT_MIN - b)) {
		yyerror("warning: integer overflow: %d + (%d) < %d", a, b, INT_MIN);
	}
}


void Range_CheckIntDiff(int a, int b)
{
	if ((b < 0) && (a > INT_MAX + b)) {
		yyerror("warning: integer overflow: %d - (%d) > %d", a, b, INT_MAX);
	} else if ((b > 0) && (a < INT_MIN + b)) {
		yyerror("warning: integer overflow: %d - %d < %d", a, b, INT_MIN);
	}
}


void Range_CheckIntProd(int a, int b)
{
	if (b > 0) {
		if ((a > 0) && (a > INT_MAX / b)) {
			yyerror("warning: integer overflow: %d * %d > %d", a, b, INT_MAX);
		} else if ((a < 0) && (a < INT_MIN / b)) {
			yyerror("warning: integer overflow: (%d) * %d < %d", a, b, INT_MIN);
		}
	} else if (b < 0) {
		if ((a > 0) && (a < INT_MIN / b)) {
			yyerror("warning: integer overflow: %d * (%d) < %d", a, b, INT_MIN);
		} else if ((a < 0) && (a < INT_MAX / b)) {
			yyerror("warning: integer overflow: (%d) * (%d) > %d", a, b, INT_MAX);
		}
	}
}


void Range_CheckLSL(int n)
{
	if (n < 0) {
		yyerror("warning: negative left shift count: %d < 0", n);
	} else if (n > SHIFT_COUNT_MAX ) {
		yyerror("warning: left shift count exceeds maximum: %d > %d", n, SHIFT_COUNT_MAX);
	}
}


void Range_CheckASR(int n)
{
	if (n < 0) {
		yyerror("warning: negative right shift count: %d < 0", n);
	} else if (n > SHIFT_COUNT_MAX) {
		yyerror("warning: right shift count exceeds maximum: %d > %d", n, SHIFT_COUNT_MAX);
	}
}


void Range_CheckROR(int n)
{
	if (n < 1) {
		yyerror("warning: non-positive rotation: %d < 1", n);
	} else if (n > SHIFT_COUNT_MAX) {
		yyerror("warning: rotation exceeds maximum: %d > %d", n, SHIFT_COUNT_MAX);
	}
}


void Range_CheckFLOOR(double x)
{
	if (x < (double) INT_MIN) {
		yyerror("warning: parameter in FLOOR too large for truncation to INTEGER: %E < %E", x, (double) INT_MIN);
	} else if (x >= (double) INT_MAX + 1.0) {
		yyerror("warning: parameter in FLOOR too large for truncation to INTEGER: %E >= %E", x, (double) INT_MAX + 1.0);
	}
}


void Range_CheckCHR(int n)
{
	if (n < 0) {
		yyerror("error: negative parameter in CHR: %d", n);
		exit(EXIT_FAILURE);
	} else if (n > CHAR_MAX) {
		yyerror("warning: parameter in CHR too large for conversion: %d > %d", n, CHAR_MAX);
	}
}


void Range_CheckRealSum(double x, double y)
{
	if ((y > 0.0) && (x > DBL_MAX - y)) {
		yyerror("warning: real number overflow: %G + %G > %G", x, y, DBL_MAX);
	} else if ((y < 0.0) && (x < -DBL_MAX - y)) {
		yyerror("warning: real number overflow: %G + (%G) < %G", x, y, -DBL_MAX);
	}
}


void Range_CheckRealDiff(double x, double y)
{
	if ((y < 0.0) && (x > DBL_MAX + y)) {
		yyerror("warning: real number overflow: %G - (%G) > %G", x, y, DBL_MAX);
	} else if ((y > 0.0) && (x < -DBL_MAX + y)) {
		yyerror("warning: real number overflow: %G - %G < %G", x, y, -DBL_MAX);
	}
}


void Range_CheckRealProd(double x, double y)
{
	if (y > 0.0) {
		if ((x > 0.0) && (x > DBL_MAX / y)) {
			yyerror("warning: real number overflow: %G * %G > %G", x, y, DBL_MAX);
		} else if ((x < 0.0) && (x < -DBL_MAX / y)) {
			yyerror("warning: real number overflow: (%G) * %G < %G", x, y, -DBL_MAX);
		}
	} else if (y < 0.0) {
		if ((x > 0.0) && (x < -DBL_MAX / y)) {
			yyerror("warning: real number overflow: %G * (%G) < %G", x, y, -DBL_MAX);
		} else if ((x < 0.0) && (x < DBL_MAX / y)) {
			yyerror("warning: real number overflow: (%G) * (%G) > %G", x, y, DBL_MAX);
		}
	}
}


void Range_CheckByte(int n)
{
	if (n < 0) {
		yyerror("warning: negative byte overflow: %d < 0", n);
	} else if (n > UCHAR_MAX) {
		yyerror("warning: byte overflow: %d > %d", n, UCHAR_MAX);
	}
}


void Range_CheckSetElement(int x)
{
	if (x < 0) {
		yyerror("error: negative set element: %d", x);
		exit(EXIT_FAILURE);
	} else if (x > SET_ELEMENT_MAX) {
		yyerror("warning: set element exceededs maximum: %d > %d", x, SET_ELEMENT_MAX);
	}
}

