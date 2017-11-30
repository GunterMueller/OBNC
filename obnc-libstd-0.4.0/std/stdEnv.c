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

#include ".obnc/stdEnv.h"
#include <obnc/OBNC.h>
#include <stdlib.h>

static void AssertNullTermination(const char s[], int sLen)
{
	int i;
	
	i = 0;
	while ((i < sLen) && (s[i] != '\0')) {
		i++;
	}
	OBNC_IT(i, sLen);
}


void stdEnv_Get_(const char name[], OBNC_LONGI int nameLen, char value[], OBNC_LONGI int valueLen, OBNC_LONGI int *res)
{
	const char *source;
	OBNC_LONGI int sourceStrLen;
	
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


void stdEnv_Init(void)
{
}
