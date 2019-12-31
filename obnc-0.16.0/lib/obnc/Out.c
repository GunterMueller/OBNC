/*Copyright (C) 2017, 2018, 2019 Karl Landstrom <karl@miasap.se>

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.*/

#include ".obnc/Out.h"
#include <obnc/OBNC.h>
#include <stdio.h>

void Out__Open_(void)
{
	/*do nothing*/
}


void Out__Char_(char ch)
{
	putchar(ch);
}


void Out__String_(const char s[], OBNC_INTEGER sLen)
{
	OBNC_INTEGER i;

	i = 0;
	while ((i < sLen) && (s[i] != '\0')) {
		putchar(s[i]);
		i++;
	}
}


void Out__Int_(OBNC_INTEGER i, OBNC_INTEGER n)
{
	OBNC_WriteInt(i, n, stdout);
}


void Out__Hex_(OBNC_INTEGER i)
{
	OBNC_WriteHex((unsigned OBNC_INTEGER) i, stdout);
}


void Out__Real_(OBNC_REAL x, OBNC_INTEGER n)
{
	printf("%*" OBNC_REAL_MOD_W "E", (int) n, x);
}


void Out__Ln_(void)
{
	putchar('\n');
}


void Out__Init(void)
{
	/*do nothing*/
}
