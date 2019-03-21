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

#include "Util.h"
#include <gc/gc.h>
#include <assert.h>
#include <limits.h>
#include <stdarg.h>
#include <string.h>

static int initialized = 0;

void Util_Init(void)
{
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

	assert(initialized);

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
				case 'l':
					if (format[i + 2] == 'd') {
						i++;
						/*fall through*/
					} else {
						fprintf(stderr, "error: non-supported format specifier: %c\n", format[i + 1]);
						exit(EXIT_FAILURE);
					}
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
		exit(EXIT_FAILURE);
	}
	return result;
}


const char *Util_Replace(const char old[], const char new[], const char s[])
{
	char *t;
	int newLength, count, i, j, tLen;
	const char *p, *result;

	result = s;
	count = 0;
	p = strstr(s, old);
	if (p != NULL) {
		do {
			count++;
			p = strstr(p + 1, old);
		} while (p != NULL);
		newLength = strlen(new);
		tLen = strlen(s) + count * newLength + 1;
		NEW_ARRAY(t, tLen);
		i = 0;
		j = 0;
		while (s[i] != '\0') {
			if (strstr(s + i, old) == s + i) {
				strcpy(t + j, new);
				j += newLength;
			} else {
				t[j] = s[i];
				j++;
			}
			i++;
		}
		t[j] = '\0';
		result = t;
	}
	return result;
}
