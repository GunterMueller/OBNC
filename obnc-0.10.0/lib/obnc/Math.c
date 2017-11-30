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

#include ".obnc/Math.h"
#include <math.h>

OBNC_LONGR double Math_sqrt_(OBNC_LONGR double x)
{
	return sqrt(x);
}


OBNC_LONGR double Math_power_(OBNC_LONGR double base, OBNC_LONGR double exp)
{
	return pow(base, exp);
}


OBNC_LONGR double Math_exp_(OBNC_LONGR double x)
{
	return exp(x);
}


OBNC_LONGR double Math_ln_(OBNC_LONGR double x)
{
	return log(x);
}


OBNC_LONGR double Math_log_(OBNC_LONGR double x, OBNC_LONGR double base)
{
	return log(x) / log(base);
}


OBNC_LONGR double Math_round_(OBNC_LONGR double x)
{
	return floor(x + 0.5);
}


OBNC_LONGR double Math_sin_(OBNC_LONGR double x)
{
	return sin(x);
}


OBNC_LONGR double Math_cos_(OBNC_LONGR double x)
{
	return cos(x);
}


OBNC_LONGR double Math_tan_(OBNC_LONGR double x)
{
	return tan(x);
}


OBNC_LONGR double Math_arcsin_(OBNC_LONGR double x)
{
	return asin(x);
}


OBNC_LONGR double Math_arccos_(OBNC_LONGR double x)
{
	return acos(x);
}


OBNC_LONGR double Math_arctan_(OBNC_LONGR double x)
{
	return atan(x);
}


OBNC_LONGR double Math_arctan2_(OBNC_LONGR double y, OBNC_LONGR double x)
{
	return atan2(y, x);
}


OBNC_LONGR double Math_sinh_(OBNC_LONGR double x)
{
	return sinh(x);
}


OBNC_LONGR double Math_cosh_(OBNC_LONGR double x)
{
	return cosh(x);
}


OBNC_LONGR double Math_tanh_(OBNC_LONGR double x)
{
	return tanh(x);
}


OBNC_LONGR double Math_arcsinh_(OBNC_LONGR double x)
{
	/*asinh is not included in ANSI C math.h*/
	return log(x + sqrt(x * x + 1));
}


OBNC_LONGR double Math_arccosh_(OBNC_LONGR double x)
{
	/*acosh is not included in ANSI C math.h*/
	return log(x + sqrt(x * x - 1));
}


OBNC_LONGR double Math_arctanh_(OBNC_LONGR double x)
{
	/*atanh is not included in ANSI C math.h*/
	return (log(1 + x) - log(1 - x)) / 2;
}


void Math_Init(void)
{
	/*do nothing*/
}
