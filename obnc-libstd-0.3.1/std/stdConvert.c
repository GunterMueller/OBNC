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

#include ".obnc/stdConvert.h"
#include <stdlib.h>
#include <string.h>

static int Length(int i)
{
	int n;
	
	if (i == 0) {
		n = 1;
	} else {
		n = 0;
		if (i < 0) {
			n++;
		}
		while (i != 0) {
			i = i / 10;
			n++;
		}
	}
	return n;
}


void stdConvert_IntToString_(int i, char s[], int sLen, int *done)
{
	int strLen, n;
	
	strLen = Length(i);
	if (strLen < sLen) {
		n = sprintf(s, "%d", i);
		*done = n == strLen;
	} else {
		*done = 0;
	}
}


void stdConvert_RealToString_(double x, char s[], int sLen, int *done)
{
	int strLen, n;
	
	if (x >= 0.0) {
		strLen = strlen("x.xxxxxx+Exx");
	} else {
		strLen = strlen("-x.xxxxxx+Exx");
	}
	if (strLen < sLen) {
		n = sprintf(s, "%E", x);
		*done = n == strLen;	
	} else {
		*done = 0;	
	}
}


static void CheckTermination(const char s[], int sLen)
{
	int i;
	
	i = 0;
	while ((i < sLen) && (s[i] != '\0')) {
		i++;
	}
	OBNC_IT(i, sLen);
}


void stdConvert_StringToInt_(const char s[], int sLen, int *i, int *done)
{
	int n;
	char ch;
	
	CheckTermination(s, sLen);
	
	/*TODO: check for overflow by using strtol*/
	n = sscanf(s, " %x%c", i, &ch);
    if ((n == 2) & (ch == 'H')) {
		*done = 1;
	} else {
		n = sscanf(s, " %d", i);
		*done = n == 1;
	}
}


void stdConvert_StringToReal_(const char s[], int sLen, double *x, int *done)
{
	int n;
	
	CheckTermination(s, sLen);

	n = sscanf(s, " %lf", x);
	*done = n == 1;
}


void stdConvert_Init(void)
{
}
