/*Copyright 2017, 2018 Karl Landstrom <karl@miasap.se>

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


void Out__String_(const char s[], OBNC_LONGI int sLen)
{
	OBNC_LONGI int i;

	i = 0;
	while ((i < sLen) && (s[i] != '\0')) {
		i++;
	}
	OBNC_IT(i, sLen);
	fputs(s, stdout);
}


void Out__Int_(OBNC_LONGI int i, OBNC_LONGI int n)
{
	OBNC_WriteInt(i, n, stdout);
}


void Out__Hex_(OBNC_LONGI int i)
{
	OBNC_WriteHex((OBNC_LONGI unsigned int) i, stdout);
}


void Out__Real_(OBNC_LONGR double x, OBNC_LONGI int n)
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
