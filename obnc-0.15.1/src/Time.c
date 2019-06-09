/*Copyright (C) 2017, 2018, 2019 Karl Landstrom <karl@miasap.se>

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

#include "Time.h"
#include "Error.h"
#include "Util.h"
#ifdef _WIN32
	#include <windows.h>
#else
	#include <sys/time.h> /*POSIX*/
#endif
#include <errno.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32

int Time(void)
{
	return GetTickCount();
}

#else /*POSIX*/

#ifndef CLOCK_MONOTONIC
#define CLOCK_MONOTONIC 1

static int clock_gettime(int clock_id, struct timespec *result)
{
	struct timeval t;

	gettimeofday(&t, NULL);
	result->tv_sec = t.tv_sec;
	result->tv_nsec = t.tv_usec * 1000;
	return 0;
}

#endif

int Time(void)
{
	struct timespec now;
	int error;
	int result = -1;

	error = clock_gettime(CLOCK_MONOTONIC, &now);
	if (! error) {
		result = now.tv_sec * 1000 + now.tv_nsec / 1000000;
	} else {
		Error_Handle(Util_String("Time failed: clock_gettime: %s\n", strerror(errno)));
	}
	return result;
}

#endif
