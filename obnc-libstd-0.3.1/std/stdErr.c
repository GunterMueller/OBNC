/*Copyright 2017 Karl Landstrom <karl@miasap.se>

This file is part of obnc-libstd.

obnc-libstd is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

obnc-libstd is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with obnc-libstd.  If not, see <http://www.gnu.org/licenses/>.*/

#include ".obnc/stdErr.h"
#include <stdio.h>

void stdErr_Char_(char ch)
{
	putc(ch, stderr);
}


void stdErr_String_(const char s[], int sLen)
{
	int charCount;
	
	charCount = fprintf(stderr, "%.*s", sLen, s);
	OBNC_IT(charCount, sLen); /*trap if not null-terminated*/
}


void stdErr_Int_(int i, int n)
{
	fprintf(stderr, "%*d", n, i);
}


void stdErr_Real_(double x, int n)
{
	fprintf(stderr, "%*E", n, x);
}


void stdErr_Ln_(void)
{
	putc('\n', stderr);
}


void stdErr_Init(void)
{
	/*do nothing*/
}
