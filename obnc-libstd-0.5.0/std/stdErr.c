/*Copyright (C) 2017, 2018 Karl Landstrom <karl@miasap.se>

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.*/

#include ".obnc/stdErr.h"
#include <obnc/OBNC.h>
#include <stdio.h>

void stdErr_Char_(char ch)
{
	putc(ch, stderr);
}


void stdErr_String_(const char s[], int OBNC_LONGI sLen)
{
	int charCount;
	
	charCount = fprintf(stderr, "%.*s", (int) sLen, s);
	OBNC_IT(charCount, sLen); /*trap if not null-terminated*/
}


void stdErr_Int_(OBNC_LONGI int i, OBNC_LONGI int n)
{
	fprintf(stderr, "%*" OBNC_INT_MOD "d", (int) n, i);
}


void stdErr_Real_(OBNC_LONGR double x, OBNC_LONGI int n)
{
	fprintf(stderr, "%*" OBNC_REAL_MOD_W "E", (int) n, x);
}


void stdErr_Ln_(void)
{
	putc('\n', stderr);
}


void stdErr_Init(void)
{
	/*do nothing*/
}
