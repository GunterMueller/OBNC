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

#include ".obnc/Out.h"
#include "OBNC_.h"
#include <stdio.h>

void Out_Open_(void)
{
	/*do nothing*/
}


void Out_Char_(char ch)
{
	putchar(ch);
}


void Out_String_(const char s[], int sLen)
{
	int charCount;
	
	charCount = printf("%.*s", sLen, s);
	OBNC_IT(charCount, sLen); /*trap if not null-terminated*/
}


void Out_Int_(int i, int n)
{
	printf("%*d", n, i);
}


void Out_Real_(double x, int n)
{
	printf("%*E", n, x);
}


void Out_Ln_(void)
{
	putchar('\n');
}


void Out_Init(void)
{
	/*do nothing*/
}
