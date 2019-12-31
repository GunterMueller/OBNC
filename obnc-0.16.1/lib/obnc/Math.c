/*Copyright (C) 2017, 2018, 2019 Karl Landstrom <karl@miasap.se>

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.*/

#include ".obnc/Math.h"
#include <obnc/OBNC.h>
#include <math.h>

#if OBNC_CONFIG_C_REAL_TYPE == OBNC_CONFIG_FLOAT
	#define S(func) func ## f
#elif OBNC_CONFIG_C_REAL_TYPE == OBNC_CONFIG_DOUBLE
	#define S(func) func
#elif OBNC_CONFIG_C_REAL_TYPE == OBNC_CONFIG_LONG_DOUBLE
	#define S(func) func ## l
#endif

OBNC_REAL Math__sqrt_(OBNC_REAL x)
{
	return S(sqrt)(x);
}


OBNC_REAL Math__power_(OBNC_REAL base, OBNC_REAL exp)
{
	return S(pow)(base, exp);
}


OBNC_REAL Math__exp_(OBNC_REAL x)
{
	return S(exp)(x);
}


OBNC_REAL Math__ln_(OBNC_REAL x)
{
	return S(log)(x);
}


OBNC_REAL Math__log_(OBNC_REAL x, OBNC_REAL base)
{
	return S(log)(x) / S(log)(base);
}


OBNC_REAL Math__round_(OBNC_REAL x)
{
	return S(floor)(x + 0.5);
}


OBNC_REAL Math__sin_(OBNC_REAL x)
{
	return S(sin)(x);
}


OBNC_REAL Math__cos_(OBNC_REAL x)
{
	return S(cos)(x);
}


OBNC_REAL Math__tan_(OBNC_REAL x)
{
	return S(tan)(x);
}


OBNC_REAL Math__arcsin_(OBNC_REAL x)
{
	return S(asin)(x);
}


OBNC_REAL Math__arccos_(OBNC_REAL x)
{
	return S(acos)(x);
}


OBNC_REAL Math__arctan_(OBNC_REAL x)
{
	return S(atan)(x);
}


OBNC_REAL Math__arctan2_(OBNC_REAL y, OBNC_REAL x)
{
	return S(atan2)(y, x);
}


OBNC_REAL Math__sinh_(OBNC_REAL x)
{
	return S(sinh)(x);
}


OBNC_REAL Math__cosh_(OBNC_REAL x)
{
	return S(cosh)(x);
}


OBNC_REAL Math__tanh_(OBNC_REAL x)
{
	return S(tanh)(x);
}


OBNC_REAL Math__arcsinh_(OBNC_REAL x)
{
#ifdef _WIN32
	return S(log)(x + S(sqrt)(x * x + 1.0));
#else
	return S(asinh)(x);
#endif
}


OBNC_REAL Math__arccosh_(OBNC_REAL x)
{
#ifdef _WIN32
	return S(log)(x + S(sqrt)(x * x - 1.0));
#else
	return S(acosh)(x);
#endif
}


OBNC_REAL Math__arctanh_(OBNC_REAL x)
{
#ifdef _WIN32
	return 0.5 * S(log)((1.0 + x) / (1.0 - x));
#else
	return S(atanh)(x);
#endif
}


void Math__Init(void)
{
	/*do nothing*/
}
