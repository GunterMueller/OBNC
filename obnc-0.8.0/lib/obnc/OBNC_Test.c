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

#include "OBNC_.h"
#include "../../src/Util.h"
#include <assert.h>
#include <math.h>

static void TestABS(void)
{
	assert(OBNC_ABS_INT(-1) == 1);
	assert(OBNC_ABS_INT(0) == 0);
	assert(OBNC_ABS_INT(1) == 1);
	
	assert(OBNC_ABS_FLT(-1.0) == 1.0);
	assert(OBNC_ABS_FLT(0.0) == 0.0);
	assert(OBNC_ABS_FLT(1.0) == 1.0);
}


static void TestODD(void)
{
	assert(! OBNC_ODD(-2));
	assert(OBNC_ODD(-1));
	assert(! OBNC_ODD(0));
	assert(OBNC_ODD(1));
	assert(! OBNC_ODD(2));
}


static void TestLSL(void)
{
	assert(OBNC_LSL(0, 0) == 0);
	assert(OBNC_LSL(0, 1) == 0);
	assert(OBNC_LSL(1, 0) == 1);
	assert(OBNC_LSL(1, 1) == 2);
}


static void TestASR(void)
{
	assert(OBNC_ASR(0, 0) == 0);
	assert(OBNC_ASR(0, 1) == 0);
	assert(OBNC_ASR(1, 0) == 1);
	assert(OBNC_ASR(1, 1) == 0);
	assert(OBNC_ASR(~0, 1) == ~0);
}


static void TestROR(void)
{
	assert(OBNC_ROR(0, 1) == 0);
	assert(OBNC_ROR(2, 1) == 1);
	assert(OBNC_ROR(1, 1) == 1 << (sizeof (int) * 8 - 1));
}


static void TestFLOOR(void)
{
	assert(OBNC_FLOOR(-1.5) == -2);
	assert(OBNC_FLOOR(0.0) == 0);
	assert(OBNC_FLOOR(1.5) == 1);
}


static void TestFLT(void)
{
	assert(OBNC_FLT(-1) == -1.0);
	assert(OBNC_FLT(0) == 0.0);
	assert(OBNC_FLT(1) == 1.0);
}


static void TestORD(void)
{
	assert(OBNC_ORD('\0') == 0);
	assert(OBNC_ORD('\1') == 1);
}


static void TestCHR(void)
{
	assert(OBNC_CHR(0) == '\0');
	assert(OBNC_CHR(1) == '\1');
}


static void TestINC(void)
{
	int x;

	x = 0;
	OBNC_INC(x);
	assert(x == 1);

	x = 0;
	OBNC_INC_N(x, 10);
	assert(x == 10);
}


static void TestDEC(void)
{
	int x;

	x = 0;
	OBNC_DEC(x);
	assert(x == -1);

	x = 0;
	OBNC_DEC_N(x, 10);
	assert(x == -10);
}


static void TestINCL(void)
{
	int A;

	A = 0;
	OBNC_INCL(A, 0);
	assert(A == 1);
}


static void TestEXCL(void)
{
	int A;

	A = 1;
	OBNC_EXCL(A, 0);
	assert(A == 0);
}


static void TestNEW(void)
{
	struct { int *typeID; int x; } *v;

	OBNC_NEW0(v);
	assert(v != NULL);
	v->x = 1;
	assert(v->x == 1);
}


static void TestASSERT(void)
{
	OBNC_ASSERT(1, "", -1);
	/*can't test failure here*/
}


static void TestPACK(void)
{
	const double eps = 0.01;
	double x;

	x = 1.0;
	OBNC_PACK(x, 2);
	assert(fabs(x - 4.0) < eps);
}


static void TestUNPK(void)
{
	double x;
	int n;

	x = 4.0;
	OBNC_UNPK(x, n);
	assert(x >= 1.0);
	assert(x < 2.0);
	assert(n == 2);
}


static void TestCMP(void)
{
	char s[4];
	
	strcpy(s, "foo");
	assert(OBNC_CMP(s, LEN(s),  "foo", LEN("foo")) == 0);
	assert(OBNC_CMP(s, LEN(s), "fool", LEN("fool")) < 0);
	assert(OBNC_CMP(s, LEN(s), "fo", LEN("fo")) > 0);
}


int main(void)
{
	Util_Init();
	TestABS();
	TestODD();
	TestLSL();
	TestASR();
	TestROR();
	TestFLOOR();
	TestFLT();
	TestORD();
	TestCHR();
	TestINC();
	TestDEC();
	TestINCL();
	TestEXCL();
	TestNEW();
	TestASSERT();
	TestPACK();
	TestUNPK();
	TestCMP();

	return 0;
}
