/*Copyright (C) 2017, 2018, 2019 Karl Landstrom <karl@miasap.se>

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.*/

#include ".obnc/extEnv.h"
#include <obnc/OBNC.h>
#include <stdlib.h>

static void AssertNullTermination(const char s[], int sLen)
{
	int i;

	i = 0;
	while ((i < sLen) && (s[i] != '\0')) {
		i++;
	}
	OBNC_C_ASSERT(i < sLen);
}


void extEnv__Get_(const char name[], OBNC_INTEGER nameLen, char value[], OBNC_INTEGER valueLen, OBNC_INTEGER *res)
{
	const char *source;
	OBNC_INTEGER sourceStrLen;

	AssertNullTermination(name, nameLen);
	source = getenv(name);
	if (source != NULL) {
		sourceStrLen = strlen(source);
		if (sourceStrLen < valueLen) {
			memcpy(value, source, sourceStrLen);
			value[sourceStrLen] = '\0';
			*res = 0;
		} else {
			memcpy(value, source, valueLen - 1);
			value[valueLen - 1] = '\0';
			*res = sourceStrLen - (valueLen - 1);
		}
	} else {
		value[0] = '\0';
		*res = -1;
	}
}


void extEnv__Init(void)
{
}
