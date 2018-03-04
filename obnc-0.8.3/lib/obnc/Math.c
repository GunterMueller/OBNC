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

double Math_sqrt_(double x)
{
	return sqrt(x);
}


double Math_power_(double base, double exp)
{
	return pow(base, exp);
}


double Math_exp_(double x)
{
	return exp(x);
}


double Math_ln_(double x)
{
	return log(x);
}


double Math_log_(double x, double base)
{
	return log(x) / log(base);
}


double Math_round_(double x)
{
	return floor(x + 0.5);
}


double Math_sin_(double x)
{
	return sin(x);
}


double Math_cos_(double x)
{
	return cos(x);
}


double Math_tan_(double x)
{
	return tan(x);
}


double Math_arcsin_(double x)
{
	return asin(x);
}


double Math_arccos_(double x)
{
	return acos(x);
}


double Math_arctan_(double x)
{
	return atan(x);
}


double Math_arctan2_(double y, double x)
{
	return atan2(y, x);
}


double Math_sinh_(double x)
{
	return sinh(x);
}


double Math_cosh_(double x)
{
	return cosh(x);
}


double Math_tanh_(double x)
{
	return tanh(x);
}


double Math_arcsinh_(double x)
{
	/*asinh is not included in ANSI C math.h*/
	return log(x + sqrt(x * x + 1));
}


double Math_arccosh_(double x)
{
	/*acosh is not included in ANSI C math.h*/
	return log(x + sqrt(x * x - 1));
}


double Math_arctanh_(double x)
{
	/*atanh is not included in ANSI C math.h*/
	return (log(1 + x) - log(1 - x)) / 2;
}


void Math_Init(void)
{
	/*do nothing*/
}
