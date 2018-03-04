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

%option always-interactive

%{
#include "Trees.h" /*symbol type in y.tab.h needs tree node declaration*/
#include "Util.h"
#include "y.tab.h"
#include "yyerror.h"
#include <assert.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int KeywordToken(const char word[]);

%}

WORD [A-Za-z][A-Za-z0-9]*

INTEGER [0-9]+|[0-9][0-9A-F]*H

REAL [0-9]+"."[0-9]*(E[+-]?[0-9]+)?

QUOTED-STRING \"[^"\n]*\"

ORDINAL-STRING [0-9][0-9A-F]*X

%%

[ \t]+

\n {
	yylineno++;
}

":=" return BECOMES;

".." return DOTDOT;

"<=" return LE;

">=" return GE;

[][*+/&~.,;|({^:)}=#<>-] return yytext[0];

{WORD} {
	int token;
	char *lexeme;

	token = KeywordToken(yytext);
	if (token < 0) {
		token = IDENT;
		NEW_ARRAY(lexeme, yyleng + 1);
		strcpy(lexeme, yytext);
		yylval.ident = lexeme;
	}
	return token;
}

{INTEGER}/".."? {
	int base;
	long lexeme;

	base = (yytext[yyleng - 1] == 'H')? 16: 10;
	errno = 0;
	lexeme = strtol(yytext, NULL, base);
	if ((errno != 0) || (lexeme > INT_MAX)) {
		yyerror("warning: %s: %s > %d", strerror(ERANGE), yytext, INT_MAX);
	}
	yylval.integer = (int) lexeme;
	return INTEGER;
}

{REAL} {
	errno = 0;
	yylval.real = strtod(yytext, NULL);
	if (errno != 0) {
		yyerror("warning: %s: %s > %G", strerror(ERANGE), yytext, DBL_MAX);
	}
	return REAL;
}

{QUOTED-STRING} {
	int lexemeLen;
	char *lexeme;

	lexemeLen = yyleng - 1;
	NEW_ARRAY(lexeme, lexemeLen);
	memcpy(lexeme, yytext + 1, lexemeLen - 1);
	lexeme[lexemeLen - 1] = '\0';
	yylval.string = lexeme;
	return STRING;
}

{ORDINAL-STRING} {
	long ordinalNumber;
	char *lexeme;
	
	if (strcmp(yytext, "0X") == 0) {
		ordinalNumber = 0;
	} else {
		errno = 0;
		ordinalNumber = strtol(yytext, NULL, 16);
		if ((errno != 0) || (ordinalNumber > UCHAR_MAX)) {
			yyerror("warning: %s: %s > 0%XX", strerror(ERANGE), yytext, UCHAR_MAX);
		}
	}
	NEW_ARRAY(lexeme, 2);
	lexeme[0] = (char) ordinalNumber;
	lexeme[1] = '\0';
	yylval.string = lexeme;
	return STRING;
}

"(*" {
	int level, ch;
	
	level = 1;
	do {
		ch = input();
		switch (ch) {
			case '(':
				ch = input();
				if (ch == '*') {
					level++;
				} else {
					unput(ch);
				}
				break;
			case '*':
				ch = input();
				if (ch == ')') {
					level--;
				} else {
					unput(ch);
				}
				break;
			case '\n':
				yylineno++;
				break;
		}
	} while ((level > 0) && (ch != EOF));

	if (level > 0) {
		yyerror("error: unterminated comment");
		exit(EXIT_FAILURE);
	}
}

. {
	yyerror("error: unexpected character: %s", yytext);
	return -1;
}

%%

static int Cmp(const void *word, const void *keywordPtr)
{
	return strcmp((char *) word, * (char **) keywordPtr);
}


static int KeywordToken(const char word[])
{
	static const char *keywords[] = {"ARRAY", "BEGIN", "BY", "CASE", "CONST", "DIV", "DO", "ELSE", "ELSIF", "END", "FALSE", "FOR", "IF", "IMPORT", "IN", "IS", "MOD", "MODULE", "NIL", "OF", "OR", "POINTER", "PROCEDURE", "RECORD", "REPEAT", "RETURN", "THEN", "TO", "TRUE", "TYPE", "UNTIL", "VAR", "WHILE"};

	static int keywordTokens[] = {ARRAY, BEGIN_, BY, CASE, CONST, DIV, DO, ELSE, ELSIF, END, FALSE, FOR, IF, IMPORT, IN, IS, MOD, MODULE, NIL, OF, OR, POINTER, PROCEDURE, RECORD, REPEAT, RETURN, THEN, TO, TRUE, TYPE, UNTIL, VAR, WHILE};

	const char **keywordPtr;
	int pos, token;

	keywordPtr = bsearch(word, keywords, LEN(keywords), sizeof keywords[0], Cmp);
	if (keywordPtr != NULL) {
		pos = keywordPtr - keywords;
		assert(pos >= 0);
		assert(pos < LEN(keywordTokens));
		token = keywordTokens[pos];
	} else {
		token = -1;
	}
	return token;
}


int yywrap(void)
{
	const int done = 1;

	return done;
}
