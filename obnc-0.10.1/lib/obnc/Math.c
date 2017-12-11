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
#include <obnc/OBNC.h>
#include <math.h>

OBNC_LONGR double Math_sqrt_(OBNC_LONGR double x)
{
#ifdef OBNC_USE_LONG_REAL
	return sqrtl(x);
#else
	return sqrt(x);
#endif
}


OBNC_LONGR double Math_power_(OBNC_LONGR double base, OBNC_LONGR double exp)
{
#ifdef OBNC_USE_LONG_REAL
	return powl(base, exp);
#else
	return pow(base, exp);
#endif
}


OBNC_LONGR double Math_exp_(OBNC_LONGR double x)
{
#ifdef OBNC_USE_LONG_REAL
	return expl(x);
#else
	return exp(x);
#endif
}


OBNC_LONGR double Math_ln_(OBNC_LONGR double x)
{
#ifdef OBNC_USE_LONG_REAL
	return logl(x);
#else
	return log(x);
#endif
}


OBNC_LONGR double Math_log_(OBNC_LONGR double x, OBNC_LONGR double base)
{
#ifdef OBNC_USE_LONG_REAL
	return logl(x) / logl(base);
#else
	return log(x) / log(base);
#endif
}


OBNC_LONGR double Math_round_(OBNC_LONGR double x)
{
#ifdef OBNC_USE_LONG_REAL
	return floorl(x + 0.5);
#else
	return floor(x + 0.5);
#endif
}


OBNC_LONGR double Math_sin_(OBNC_LONGR double x)
{
#ifdef OBNC_USE_LONG_REAL
	return sinl(x);
#else
	return sin(x);
#endif
}


OBNC_LONGR double Math_cos_(OBNC_LONGR double x)
{
#ifdef OBNC_USE_LONG_REAL
	return cosl(x);
#else
	return cos(x);
#endif
}


OBNC_LONGR double Math_tan_(OBNC_LONGR double x)
{
#ifdef OBNC_USE_LONG_REAL
	return tanl(x);
#else
	return tan(x);
#endif
}


OBNC_LONGR double Math_arcsin_(OBNC_LONGR double x)
{
#ifdef OBNC_USE_LONG_REAL
	return asinl(x);
#else
	return asin(x);
#endif
}


OBNC_LONGR double Math_arccos_(OBNC_LONGR double x)
{
#ifdef OBNC_USE_LONG_REAL
	return acosll(x);
#else
	return acos(x);
#endif
}


OBNC_LONGR double Math_arctan_(OBNC_LONGR double x)
{
#ifdef OBNC_USE_LONG_REAL
	return atanl(x);
#else
	return atan(x);
#endif
}


OBNC_LONGR double Math_arctan2_(OBNC_LONGR double y, OBNC_LONGR double x)
{
#ifdef OBNC_USE_LONG_REAL
	return atan2l(y, x);
#else
	return atan2(y, x);
#endif
}


OBNC_LONGR double Math_sinh_(OBNC_LONGR double x)
{
#ifdef OBNC_USE_LONG_REAL
	return sinhl(x);
#else
	return sinh(x);
#endif
}


OBNC_LONGR double Math_cosh_(OBNC_LONGR double x)
{
#ifdef OBNC_USE_LONG_REAL
	return coshl(x);
#else
	return cosh(x);
#endif
}


OBNC_LONGR double Math_tanh_(OBNC_LONGR double x)
{
#ifdef OBNC_USE_LONG_REAL
	return tanhl(x);
#else
	return tanh(x);
#endif
}


OBNC_LONGR double Math_arcsinh_(OBNC_LONGR double x)
{
#ifdef OBNC_USE_LONG_REAL
	return asinhl(x);
#else
	return asinh(x);
#endif
}


OBNC_LONGR double Math_arccosh_(OBNC_LONGR double x)
{
#ifdef OBNC_USE_LONG_REAL
	return acoshl(x);
#else
	return acosh(x);
#endif
}


OBNC_LONGR double Math_arctanh_(OBNC_LONGR double x)
{
#ifdef OBNC_USE_LONG_REAL
	return atanhl(x);
#else
	return atanh(x);
#endif
}


void Math_Init(void)
{
	/*do nothing*/
}
