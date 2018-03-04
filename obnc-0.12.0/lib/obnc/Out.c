/*Copyright (C) 2017, 2018 Karl Landstrom <karl@miasap.se>

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
