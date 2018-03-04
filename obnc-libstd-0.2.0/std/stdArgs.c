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

#include ".obnc/stdArgs.h"
#include "OBNC_.h"
#include <assert.h>
#include <string.h>

int stdArgs_count_;

void stdArgs_Get_(int n, char arg[], int argLen, int *res)
{
	const char *srcArg;
	int srcArgStrLen;
	
	assert(n >= 0);
	assert(n < stdArgs_count_);
	assert(arg != NULL);
	assert(argLen >= 0);

	srcArg = OBNC_argv[n + 1];
	srcArgStrLen = strlen(srcArg);
	if (argLen > srcArgStrLen) {
		memcpy(arg, srcArg, srcArgStrLen + 1);
		*res = 0;
	} else {
		memcpy(arg, srcArg, argLen - 1);
		arg[argLen - 1] = '\0';
		*res = srcArgStrLen + 1 - argLen;
	}	
}


static int Max(int a, int b)
{
	return (a > b)? a: b;
}


void stdArgs_Init(void)
{
	static int initialized = 0;

	if (! initialized) {
		stdArgs_count_ = Max(OBNC_argc - 1, 0);
		initialized = 1;
	}
}
