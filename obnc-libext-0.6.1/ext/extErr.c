/*Copyright (C) 2017, 2018, 2019 Karl Landstrom <karl@miasap.se>

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.*/

#include ".obnc/extErr.h"
#include <obnc/OBNC.h>
#include <stdio.h>

void extErr__Char_(char ch)
{
	putc(ch, stderr);
}


void extErr__String_(const char s[], int OBNC_LONGI sLen)
{
	fprintf(stderr, "%.*s", (int) sLen, s);
}


void extErr__Int_(OBNC_LONGI int i, OBNC_LONGI int n)
{
	fprintf(stderr, "%*" OBNC_INT_MOD "d", (int) n, i);
}


void extErr__Hex_(OBNC_LONGI int i)
{
	OBNC_WriteHex((OBNC_LONGI unsigned int) i, stderr);
}


void extErr__Real_(OBNC_LONGR double x, OBNC_LONGI int n)
{
	fprintf(stderr, "%*" OBNC_REAL_MOD_W "E", (int) n, x);
}


void extErr__Ln_(void)
{
	putc('\n', stderr);
}


void extErr__Init(void)
{
	/*do nothing*/
}
