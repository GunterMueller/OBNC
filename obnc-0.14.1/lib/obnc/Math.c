/*Copyright 2017, 2018 Karl Landstrom <karl@miasap.se>

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.*/

#include ".obnc/Math.h"
#include <obnc/OBNC.h>
#include <math.h>

OBNC_LONGR double Math__sqrt_(OBNC_LONGR double x)
{
#if OBNC_CONFIG_USE_LONG_REAL
	return sqrtl(x);
#else
	return sqrt(x);
#endif
}


OBNC_LONGR double Math__power_(OBNC_LONGR double base, OBNC_LONGR double exp)
{
#if OBNC_CONFIG_USE_LONG_REAL
	return powl(base, exp);
#else
	return pow(base, exp);
#endif
}


OBNC_LONGR double Math__exp_(OBNC_LONGR double x)
{
#if OBNC_CONFIG_USE_LONG_REAL
	return expl(x);
#else
	return exp(x);
#endif
}


OBNC_LONGR double Math__ln_(OBNC_LONGR double x)
{
#if OBNC_CONFIG_USE_LONG_REAL
	return logl(x);
#else
	return log(x);
#endif
}


OBNC_LONGR double Math__log_(OBNC_LONGR double x, OBNC_LONGR double base)
{
#if OBNC_CONFIG_USE_LONG_REAL
	return logl(x) / logl(base);
#else
	return log(x) / log(base);
#endif
}


OBNC_LONGR double Math__round_(OBNC_LONGR double x)
{
#if OBNC_CONFIG_USE_LONG_REAL
	return floorl(x + 0.5);
#else
	return floor(x + 0.5);
#endif
}


OBNC_LONGR double Math__sin_(OBNC_LONGR double x)
{
#if OBNC_CONFIG_USE_LONG_REAL
	return sinl(x);
#else
	return sin(x);
#endif
}


OBNC_LONGR double Math__cos_(OBNC_LONGR double x)
{
#if OBNC_CONFIG_USE_LONG_REAL
	return cosl(x);
#else
	return cos(x);
#endif
}


OBNC_LONGR double Math__tan_(OBNC_LONGR double x)
{
#if OBNC_CONFIG_USE_LONG_REAL
	return tanl(x);
#else
	return tan(x);
#endif
}


OBNC_LONGR double Math__arcsin_(OBNC_LONGR double x)
{
#if OBNC_CONFIG_USE_LONG_REAL
	return asinl(x);
#else
	return asin(x);
#endif
}


OBNC_LONGR double Math__arccos_(OBNC_LONGR double x)
{
#if OBNC_CONFIG_USE_LONG_REAL
	return acosl(x);
#else
	return acos(x);
#endif
}


OBNC_LONGR double Math__arctan_(OBNC_LONGR double x)
{
#if OBNC_CONFIG_USE_LONG_REAL
	return atanl(x);
#else
	return atan(x);
#endif
}


OBNC_LONGR double Math__arctan2_(OBNC_LONGR double y, OBNC_LONGR double x)
{
#if OBNC_CONFIG_USE_LONG_REAL
	return atan2l(y, x);
#else
	return atan2(y, x);
#endif
}


OBNC_LONGR double Math__sinh_(OBNC_LONGR double x)
{
#if OBNC_CONFIG_USE_LONG_REAL
	return sinhl(x);
#else
	return sinh(x);
#endif
}


OBNC_LONGR double Math__cosh_(OBNC_LONGR double x)
{
#if OBNC_CONFIG_USE_LONG_REAL
	return coshl(x);
#else
	return cosh(x);
#endif
}


OBNC_LONGR double Math__tanh_(OBNC_LONGR double x)
{
#if OBNC_CONFIG_USE_LONG_REAL
	return tanhl(x);
#else
	return tanh(x);
#endif
}


OBNC_LONGR double Math__arcsinh_(OBNC_LONGR double x)
{
#if OBNC_CONFIG_USE_LONG_REAL
	return asinhl(x);
#else
	return asinh(x);
#endif
}


OBNC_LONGR double Math__arccosh_(OBNC_LONGR double x)
{
#if OBNC_CONFIG_USE_LONG_REAL
	return acoshl(x);
#else
	return acosh(x);
#endif
}


OBNC_LONGR double Math__arctanh_(OBNC_LONGR double x)
{
#if OBNC_CONFIG_USE_LONG_REAL
	return atanhl(x);
#else
	return atanh(x);
#endif
}


void Math__Init(void)
{
	/*do nothing*/
}
