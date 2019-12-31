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

#ifndef RANGE_H
#define RANGE_H

#include "../lib/obnc/OBNC.h"

void Range_CheckIntSum(OBNC_INTEGER a, OBNC_INTEGER b);

void Range_CheckIntDiff(OBNC_INTEGER a, OBNC_INTEGER b);

void Range_CheckIntProd(OBNC_INTEGER a, OBNC_INTEGER b);

void Range_CheckLSL(OBNC_INTEGER n);

void Range_CheckASR(OBNC_INTEGER n);

void Range_CheckROR(OBNC_INTEGER n);

void Range_CheckBIT(OBNC_INTEGER n);

void Range_CheckFLOOR(OBNC_REAL x);

void Range_CheckCHR(OBNC_INTEGER n);

void Range_CheckRealSum(OBNC_REAL x, OBNC_REAL y);

void Range_CheckRealDiff(OBNC_REAL x, OBNC_REAL y);

void Range_CheckRealProd(OBNC_REAL x, OBNC_REAL y);

void Range_CheckByte(OBNC_INTEGER n);

void Range_CheckSetElement(OBNC_INTEGER x);

#endif
