/*Copyright (C) 2017, 2018 Karl Landstrom <karl@miasap.se>

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

#include "Util.h"
#include <gc/gc.h>
#include <limits.h>
#include <stdarg.h>
#include <string.h>

void Util_Init(void)
{
	static int initialized = 0;
	
	if (! initialized) {
		GC_INIT();
		initialized = 1;
	}
}


char *Util_String(const char format[], ...)
{
	va_list args;
	int resultLen, i;
	const char *strArg;
	char *result;

	resultLen = strlen(format) + 1;
	va_start(args, format);
	i = 0;
	while (format[i] != '\0') {
		if (format[i] == '%') {
			switch (format[i + 1]) {
				case 's':
					strArg = va_arg(args, char *);
					if (strArg != NULL) {
						resultLen += strlen(strArg);
					}
					break;
				case 'd':
					resultLen += CHAR_BIT * sizeof (long int) / 3 + 3;
					break;
				case '%':
					break;
				default:
					fprintf(stderr, "error: non-supported format specifier: %c\n", format[i + 1]);
					exit(EXIT_FAILURE);
			}
		}
		i++;
	}
	va_end(args);

	result = GC_MALLOC_ATOMIC(resultLen);
	if (result != NULL) {
		va_start(args, format);
		vsprintf(result, format, args);
		va_end(args);
	} else {
		fprintf(stderr, "memory exausted\n");
		exit(1);
	}
	return result;
}
