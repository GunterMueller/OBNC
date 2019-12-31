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

#include "Files.h"
#include "lex.yy.h"
#include "Oberon.h"
#include "Trees.h" /*symbol type in y.tab.h needs tree node declaration*/
#include "Util.h"
#include "y.tab.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

static FILE *inputFile;

static struct { int token; const char *value; } expectedOutput[] = {
	{IDENT, "x"},
	{IDENT, "scan"},
	{IDENT, "Oberon"},
	{IDENT, "GetSymbol"},
	{IDENT, "firstLetter"},
	{INTEGER, "32767"},
	{INTEGER, "256"},
	{REAL, "340282346638528859811704183484516925440.0"},
	{REAL, "4.567E+6"},
	{REAL, "4.567E-6"},
	{REAL, "179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.0"},
	{INTEGER, "1"},
	{DOTDOT, ""},
	{INTEGER, "10"},
	{STRING, ""},
	{STRING, ""},
	{STRING, "*"},
	{STRING, "*"},
	{STRING, "Don't worry!"},
	{'+', ""},
	{'-', ""},
	{'*', ""},
	{'/', ""},
	{'~', ""},
	{'&', ""},
	{'.', ""},
	{',', ""},
	{';', ""},
	{'|', ""},
	{'(', ""},
	{'[', ""},
	{'{', ""},
	{BECOMES, ""},
	{'^', ""},
	{'=', ""},
	{'#', ""},
	{'<', ""},
	{'>', ""},
	{LE, ""},
	{GE, ""},
	{DOTDOT, ""},
	{':', ""},
	{')', ""},
	{']', ""},
	{'}', ""},
	{ARRAY, ""},
	{BEGIN_, ""},
	{BY, ""},
	{CASE, ""},
	{CONST, ""},
	{DIV, ""},
	{DO, ""},
	{ELSE, ""},
	{ELSIF, ""},
	{END, ""},
	{FALSE, ""},
	{FOR, ""},
	{IF, ""},
	{IMPORT, ""},
	{IN, ""},
	{IS, ""},
	{MOD, ""},
	{MODULE, ""},
	{NIL, ""},
	{OF, ""},
	{OR, ""},
	{POINTER, ""},
	{PROCEDURE, ""},
	{RECORD, ""},
	{REPEAT, ""},
	{RETURN, ""},
	{THEN, ""},
	{TO, ""},
	{TRUE, ""},
	{TYPE, ""},
	{UNTIL, ""},
	{VAR, ""},
	{WHILE, ""}
};

static void CompareTokens(int token, int i)
{
	int expectedToken;
	const char *expectedValue;
	union {
		OBNC_INTEGER integer;
		OBNC_REAL real;
	} value;

	assert((i >= 0) && (i < LEN(expectedOutput)));

	expectedToken = expectedOutput[i].token;
	expectedValue = expectedOutput[i].value;

	/*compare token IDs*/
	assert(token == expectedToken);

	/*compare semantic values*/
	switch (token) {
		case IDENT:
			assert(strcmp(yylval.ident, expectedValue) == 0);
			break;
		case INTEGER:
			sscanf(expectedValue, "%" OBNC_INT_MOD "d", &value.integer);
			assert(yylval.integer == value.integer);
			break;
		case REAL:
			sscanf(expectedValue, "%" OBNC_REAL_MOD_R "f", &value.real);
			assert(yylval.real == value.real);
			break;
		case STRING:
			assert(strcmp(yylval.string, expectedValue) == 0);
			break;
	}
}


static void TestYYLex(void)
{
	int token, i;

	token = yylex();
	i = 0;
	while (token > 0) {
		CompareTokens(token, i);
		token = yylex();
		i++;
	}
	assert(i == LEN(expectedOutput));
}


int main(void)
{
	const char *inputFilename = "../tests/scanner/tokens.txt";
	int exitStatus;

	Files_Init();
	Oberon_Init();
	Trees_Init();
	Util_Init();
	inputFile = Files_Old(inputFilename, FILES_READ);
	assert(inputFile != NULL);
	yyin = inputFile;
	TestYYLex();
	Files_Close(&inputFile);
	exitStatus = EXIT_SUCCESS;
	return exitStatus;
}
