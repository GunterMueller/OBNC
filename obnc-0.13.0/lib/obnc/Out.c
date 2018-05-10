/*Copyright 2017, 2018 Karl Landstrom <karl@miasap.se>

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.*/

#include ".obnc/Out.h"
#include <obnc/OBNC.h>
#include <stdio.h>

void Out_Open_(void)
{
	/*do nothing*/
}


void Out_Char_(char ch)
{
	putchar(ch);
}


void Out_String_(const char s[], OBNC_LONGI int sLen)
{
	OBNC_LONGI int i;
	
	i = 0;
	while ((i < sLen) && (s[i] != '\0')) {
		i++;
	}
	OBNC_IT(i, sLen);
	fputs(s, stdout);
}


void Out_Int_(OBNC_LONGI int i, OBNC_LONGI int n)
{
	OBNC_WriteInt(i, n, stdout);
}


void Out_Real_(OBNC_LONGR double x, OBNC_LONGI int n)
{
	printf("%*" OBNC_REAL_MOD_W "E", (int) n, x);
}


void Out_Ln_(void)
{
	putchar('\n');
}


void Out_Init(void)
{
	/*do nothing*/
}
