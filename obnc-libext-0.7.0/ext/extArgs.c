/*Copyright (C) 2017, 2018, 2019 Karl Landstrom <karl@miasap.se>

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.*/

#include ".obnc/extArgs.h"
#include <obnc/OBNC.h>
#include <assert.h>
#include <string.h>

OBNC_INTEGER extArgs__count_;

void extArgs__Get_(OBNC_INTEGER n, char arg[], OBNC_INTEGER argLen, OBNC_INTEGER *res)
{
	const char *srcArg;
	OBNC_INTEGER srcArgStrLen;

	assert(n >= 0);
	assert(n < extArgs__count_);
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


void extArgs__Init(void)
{
	static int initialized = 0;

	if (! initialized) {
		extArgs__count_ = Max(OBNC_argc - 1, 0);
		initialized = 1;
	}
}
