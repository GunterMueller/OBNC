/*Copyright (C) 2017 Karl Landstrom <karl@miasap.se>

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

#include ".obnc/In.h"
#include "../../src/Util.h"
#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

int In_Done_;
static int inputConsumed;

void In_Open_(void)
{
	if (! inputConsumed) {
		/*do nothing*/
	} else {
		fprintf(stderr, "cannot rewind stdin\n");
		exit(EXIT_FAILURE);
	}
}


void In_Char_(char *ch)
{
	int d;

	d = getchar();
	*ch = (char) d;
	In_Done_ = d != EOF;
	if (In_Done_) {
		inputConsumed = 1;
	}
}


void In_Int_(int *i)
{
	int scanCount;

	scanCount = scanf("%d", i);
	In_Done_ = scanCount == 1;
	if (In_Done_) {
		inputConsumed = 1;
	}
}


void In_Real_(double *x)
{
	int scanCount;

	scanCount = scanf("%lf", x);
	In_Done_ = scanCount == 1;
	if (In_Done_) {
		inputConsumed = 1;
	}
}


void In_String_(char str[], int strLen)
{
	int n, ch, i;

	In_Done_ = 0;
	n = 0;
	do {
		ch = getchar();
		n++;
	} while (isspace(ch));
	if (ch == '"') {
		i = 0;
		ch = getchar();
		n++;
		while ((ch != EOF) && (ch != '"') && (ch != '\n') && (i < strLen)) {
			str[i] = ch;
			i++;
			ch = getchar();
			n++;
		}
		if ((ch == '"') && (i < strLen)) {
			str[i] = '\0';
			In_Done_ = 1;
		} else {
			str[0] = '\0';
		}
	} else {
		if (ch != EOF) {
			ch = ungetc(ch, stdin);
			if (ch != EOF) {
				n--;
			} else {
				fprintf(stderr, "ungetc failed while reading string\n");
			}
		}
	}
	if (ch == EOF) {
		n--;
	}
	if (n > 0) {
		inputConsumed = 1;
	}
}


void In_Name_(char name[], int nameLen)
{
	int n, ch, i;

	In_Done_ = 0;
	n = 0;
	do {
		ch = getchar();
		n++;
	} while (isspace(ch));
	if (ch != EOF) {
		i = 0;
		while ((i < nameLen) && isgraph(ch)) {
			name[i] = ch;
			i++;
			ch = getchar();
			n++;
		}
		if (i < nameLen) {
			name[i] = '\0';
			In_Done_ = ! isgraph(ch);
		} else {
			name[0] = '\0';
		}
	}
	if (ch == EOF) {
		n--;
	}
	if (n > 0) {
		inputConsumed = 1;
	}
}


void In_Init(void)
{
	In_Done_ = 0;
	inputConsumed = 0;
}
