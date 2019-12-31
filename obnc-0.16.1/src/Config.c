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

#include "Config.h"
#include "Util.h"
#include <stdlib.h>
#include <string.h>

static int initialized = 0;

void Config_Init(void)
{
	if (! initialized) {
		initialized = 1;
		Util_Init();
	}
}


const char *Config_Prefix(void)
{
	static const char *prefix;

	assert(initialized);

	if (prefix == NULL) {
		prefix = getenv("OBNC_PREFIX");
		if (prefix == NULL) {
			prefix = CONFIG_DEFAULT_PREFIX;
		}
	}
	return prefix;
}


const char *Config_LibDir(void)
{
	static const char *libdir;

	assert(initialized);

	if (libdir == NULL) {
		libdir = getenv("OBNC_LIBDIR");
		if (libdir == NULL) {
			libdir = CONFIG_DEFAULT_LIBDIR;
		}
	}
	return libdir;
}
