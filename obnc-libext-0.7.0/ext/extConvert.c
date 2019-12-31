/*Copyright (C) 2017, 2018, 2019 Karl Landstrom <karl@miasap.se>

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.*/

#include ".obnc/extConvert.h"
#include <obnc/OBNC.h>
#include <float.h>
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


void extConvert__IntToString_(OBNC_INTEGER i, char s[], OBNC_INTEGER sLen, int *done)
{
	int strLen, n;

	strLen = Length(i);
	if (strLen < sLen) {
		n = sprintf(s, "%" OBNC_INT_MOD "d", i);
		*done = n == strLen;
	} else {
		*done = 0;
	}
}


void extConvert__RealToString_(OBNC_REAL x, char s[], OBNC_INTEGER sLen, int *done)
{
	char buffer[LDBL_DIG + 9]; /*LDBL_DIG + strlen("-") + strlen(".") + strlen("e+9999") + 1*/
	int n;

	*done = 0;
	n = sprintf(buffer, "%" OBNC_REAL_MOD_W "E", x);
	if (n < sLen) {
		strcpy(s, buffer);
		*done = 1;
	}
}


static void CheckTermination(const char s[], OBNC_INTEGER sLen)
{
	OBNC_INTEGER i;

	i = 0;
	while ((i < sLen) && (s[i] != '\0')) {
		i++;
	}
	OBNC_C_ASSERT(i < sLen);
}


void extConvert__StringToInt_(const char s[], OBNC_INTEGER sLen, OBNC_INTEGER *i, int *done)
{
	unsigned OBNC_INTEGER j;
	int n;
	char ch;

	CheckTermination(s, sLen);

	/*TODO: check for overflow by using strtol*/
	n = sscanf(s, " %" OBNC_INT_MOD "x%c", &j, &ch);
    if ((n == 2) && (ch == 'H')) {
    		*i = j;
		*done = 1;
	} else {
		n = sscanf(s, " %" OBNC_INT_MOD "d", i);
		*done = n == 1;
	}
}


void extConvert__StringToReal_(const char s[], OBNC_INTEGER sLen, OBNC_REAL *x, int *done)
{
	int n;

	CheckTermination(s, sLen);

	n = sscanf(s, " %" OBNC_REAL_MOD_R "f", x);
	*done = n == 1;
}


void extConvert__Init(void)
{
}
