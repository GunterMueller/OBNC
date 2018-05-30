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

#include "StackTrace.h"
#include "Util.h"
#include <execinfo.h> /*GNU specific*/
#include <signal.h> /*POSIX*/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static StackTrace_ContextPrinter printContext;

static void ScanFilenameAndOffset(const char line[], char **filename, char **offset, int *done)
{
	const char *leftParenPtr, *plusPtr, *rightParenPtr;
	ptrdiff_t exeFileLen, offsetLen;
	
	*filename = NULL;
	*offset = NULL;
	*done = 0;
	leftParenPtr = strrchr(line, '(');
	if (leftParenPtr != NULL) {
		/*scan filename*/
		exeFileLen = leftParenPtr - line + 1;
		assert(exeFileLen > 0);
		NEW_ARRAY(*filename, exeFileLen);
		memcpy(*filename, line, (size_t) exeFileLen - 1);
		(*filename)[exeFileLen - 1] = '\0';
		
		/*scan file offset*/
		plusPtr = strrchr(line, '+');
		if (plusPtr != NULL) {
			rightParenPtr = strrchr(line, ')');
			if (rightParenPtr != 0) {
				offsetLen = rightParenPtr - plusPtr - 1 + 1;
				assert(offsetLen > 0);
				NEW_ARRAY(*offset, offsetLen);
				memcpy(*offset, plusPtr + 1, (size_t) offsetLen - 1);
				(*offset)[offsetLen - 1] = '\0';
				*done = 1;
			}
		}
	}
}


static void PrintSourceFilePosition(const char binFilename[], const char offset[])
{
	const char *command;
	int error;

	command = Util_String("addr2line -f -e %s %s | grep -v '^?' | sed 's|^/|\t/|' >&2", binFilename, offset);
	error = system(command);
	if (error) {
		fprintf(stderr, "command to print stack trace failed: %s\n", command);
	}
}


static void PrintStackTrace(int signum)
{
	void *returnAddresses[STACKTRACE_MAXLEN];
	int count, lineNum;
	char **lines;
	int done;
	char *filename, *offset;

	if (printContext != NULL) {
		printContext();
		fprintf(stderr, "\n");
	}
	count = backtrace(returnAddresses, LEN(returnAddresses));
	lines = backtrace_symbols(returnAddresses, count);
	fprintf(stderr, "Fatal signal raised, stack trace:\n");
	for (lineNum = 0; lineNum < count; lineNum++) {
		ScanFilenameAndOffset(lines[lineNum], &filename, &offset, &done);
		if (done) {
			PrintSourceFilePosition(filename, offset);
		} else {
			fprintf(stderr, "warning: failed getting filename and offset from backtrace\n");
		}
	}
	free(lines);
}


void StackTrace_Init(StackTrace_ContextPrinter f)
{
	static int initialized = 0;
	static const int fatalSignals[] = {SIGABRT, SIGALRM, SIGFPE, SIGHUP, SIGILL, SIGINT, /*SIGKILL,*/ SIGPIPE, SIGQUIT, SIGSEGV, SIGTERM, SIGUSR1, SIGUSR2};
	int i;
	void (*prevHandler)(int signum);
	
	if (! initialized) {
		printContext = f;
		
		/*register signal handler for fatal signals*/
		for (i = 0; i < LEN(fatalSignals); i++) {
			prevHandler = signal(fatalSignals[i], PrintStackTrace);
			if (prevHandler == SIG_ERR) {
				fprintf(stderr, "warning: setting signal handler for PrintStackTrace failed: signal: %d\n", fatalSignals[i]);
			} /*else if (prevHandler != NULL) {
				fprintf(stderr, "replacing previous signal handler with PrintStackTrace\n");
			}*/
		}
	}
}
