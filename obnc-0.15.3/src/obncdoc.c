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
#include "Error.h"
#include "Files.h"
#include "Paths.h"
#include "Util.h"
#include <dirent.h> /*POSIX*/
#include <unistd.h> /*POSIX*/
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { int count; char **filenames; } Accumulator;

typedef void (*Applicator)(const char filename[], Accumulator *acc);

static void Apply(Applicator f, const char dirName[], Accumulator *acc) /*apply f on each file in directory dir with accumulator acc*/
{
	DIR *dir;
	struct dirent *file;
	const char *filename;
	int error;

	assert(f != NULL);
	assert(dirName != NULL);

	dir = opendir(dirName);
	if (dir != NULL) {
		file = readdir(dir);
		while (file != NULL) {
			if ((strcmp(file->d_name, ".") != 0) && (strcmp(file->d_name, "..") != 0)) {
				filename = file->d_name;
				f(filename, acc);
			}
			file = readdir(dir);
		}
		error = closedir(dir);
		if (error) {
			Error_Handle(Util_String("closing directory failed: %s", strerror(errno)));
		}
	} else {
		Error_Handle(Util_String("reading directory failed: %s", strerror(errno)));
	}
}


static void ExitFailure(const char msg[])
{
	assert(msg != NULL);

	if (strcmp(msg, "") != 0) {
		fprintf(stderr, "obncdoc: %s\n", msg);
	}
	fputs("obncdoc: generating documentation failed\n", stderr);
	exit(EXIT_FAILURE);
}


static void PrintHelp(void)
{
	puts("obncdoc - extract exported features from Oberon modules");
	putchar('\n');
	puts("usage:");
	puts("\tobncdoc [-h | -v]");
	putchar('\n');
	puts("\t-h\tdisplay help and exit");
	puts("\t-v\tdisplay version and exit");
}


static void PrintVersion(void)
{
	if (strcmp(CONFIG_VERSION, "") != 0) {
		printf("OBNC %s\n", CONFIG_VERSION);
	} else {
		puts("OBNC (unknown version)");
	}
}


static int EndsWith(const char pattern[], const char target[])
{
	size_t patternLen = strlen(pattern);
	size_t targetLen = strlen(target);

	return (patternLen <= targetLen) && (strcmp(target + targetLen - patternLen, pattern) == 0);
}


static int IsOberonFile(const char filename[])
{
	return EndsWith(".obn", filename) || EndsWith(".Mod", filename) || EndsWith(".mod", filename);
}


static const char *IndexTitle(void)
{
	return Util_String("Index of %s", Paths_Basename(Paths_CurrentDir()));
}


static void PrintHtmlHeader(const char title[], FILE *file)
{
	fputs("<!DOCTYPE html PUBLIC '-//W3C//DTD XHTML 1.0 Strict//EN' 'http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd'>\n", file);
	fputs("<html xmlns='http://www.w3.org/1999/xhtml' xml:lang='en' lang='en'>\n", file);
	fputs("	<head>\n", file);
	fputs("		<meta name='viewport' content='width=device-width, initial-scale=1.0' />\n", file);
	fputs("		<meta http-equiv='Content-Type' content='text/html; charset=utf-8' />\n", file);
	fprintf(file, "		<title>%s</title>\n", title);
	fprintf(file, "		<link rel='stylesheet' type='text/css' href='style.css' />\n");
	fputs("	</head>\n", file);
	fputs("	<body>\n", file);
}


static void PrintHtmlFooter(FILE *file)
{
	fputs("	</body>\n", file);
	fputs("</html>\n", file);
}


static void CreateHtmlDefinition(const char module[], const char inputFile[], const char outputFile[])
{
	const char *title, *script, *command;
	FILE *outFile;
	int error;

	title = Util_String("DEFINITION %s", module);

	if (Files_Exists(outputFile)) {
		outFile = Files_Old(outputFile, FILES_WRITE);
	} else {
		outFile = Files_New(outputFile);
	}
	PrintHtmlHeader(title, outFile);
	fprintf(outFile, "		<p><a href='index.html'>Index</a></p>\n");
	fputc('\n', outFile);
	fputs("		<pre>\n", outFile);
	Files_Close(&outFile);

	script = Util_String("%s/bin/obncdoc-markup", Config_Prefix());
	command = Util_String("awk -f %s < %s >> %s", Paths_ShellArg(script), Paths_ShellArg(inputFile), Paths_ShellArg(outputFile));
	error = system(command);
	if (! error) {
		outFile = Files_Old(outputFile, FILES_APPEND);
		fputs("</pre>\n", outFile);
		PrintHtmlFooter(outFile);
		Files_Close(&outFile);
	} else {
		Files_Remove(outputFile);
		Error_Handle("");
	}
}


static void CreateDefinition(const char filename[], Accumulator *acc)
{
	const char *oberonFile, *module, *textOutputFile, *htmlOutputFile, *scriptFile, *command;
	int error;

	assert(acc != NULL);

	if (IsOberonFile(filename)) {
		oberonFile = filename;
		module = Paths_SansSuffix(Paths_Basename(oberonFile));
		textOutputFile = Util_String("obncdoc/%s.def", module);
		htmlOutputFile = Util_String("obncdoc/%s.def.html", module);

		if (! Files_Exists(textOutputFile) || (Files_Timestamp(textOutputFile) < Files_Timestamp(oberonFile))) {
			scriptFile = Util_String("%s/bin/obncdoc-extract", Config_Prefix());
			command = Util_String("awk -f %s < %s > %s", Paths_ShellArg(scriptFile), oberonFile, textOutputFile);
			error = system(command);
			if (! error) {
				CreateHtmlDefinition(module, textOutputFile, htmlOutputFile);
			} else {
				Error_Handle("");
			}
		}
		acc->count++;
	}
}


static void CreateAllDefinitions(void)
{
	Accumulator acc;

	if (! Files_Exists("obncdoc")) {
		Files_CreateDir("obncdoc");
	}
	acc.count = 0;
	Apply(CreateDefinition, ".", &acc);
	if (acc.count == 0) {
		Error_Handle("no oberon files");
	}
}


static void DeleteOrphanedDefinition(const char filename[], Accumulator *acc)
{
	const char *module;

	(void) acc; /*unused*/
	if (strstr(filename, ".def") != NULL) {
		module = Paths_SansSuffix(Paths_Basename(filename));
		if (! Files_Exists(Util_String("%s.obn", module))
				&& ! Files_Exists(Util_String("%s.Mod", module))
				&& ! Files_Exists(Util_String("%s.mod", module))) {
			Files_Remove(Util_String("obncdoc/%s", filename));
		}
	}
}


static void DeleteAllOrphanedDefinitions(void)
{
	Apply(DeleteOrphanedDefinition, "obncdoc", NULL);
}


static void CountDefinition(const char filename[], Accumulator *acc)
{
	assert(filename != NULL);
	assert(acc != NULL);

	if (EndsWith(".def", filename)) {
		acc->count++;
	}
}


static void AddDefinition(const char filename[], Accumulator *acc)
{
	if (EndsWith(".def", filename)) {
		acc->filenames[acc->count] = Util_String("%s", filename);
		acc->count++;
	}
}


static int StringComparison(const void *a, const void *b)
{
	const char *s1 = *(const char **) a;
	const char *s2 = *(const char **) b;

	return strcmp(s1, s2);
}


static int DefFileEmpty(const char filename[])
{
	FILE *f;
	int ch, newlineCount;

	f = Files_Old(filename, FILES_READ);
	newlineCount = 0;
	ch = fgetc(f);
	while ((ch != EOF) && (newlineCount <= 2)) {
		if (ch == '\n') {
			newlineCount++;
		}
		ch = fgetc(f);
	}
	Files_Close(&f);
	return newlineCount == 2;
}


static void CreateIndex(void)
{
	const char *indexFilename = "obncdoc/index.html", *defFilename, *module;
	Accumulator acc;
	FILE *indexFile;
	int filenamesLen, i;

	/*count definition files*/
	acc.count = 0;
	Apply(CountDefinition, "obncdoc", &acc);

	/*add definition files to string array*/
	filenamesLen = acc.count;
	NEW_ARRAY(acc.filenames, filenamesLen);
	acc.count = 0;
	Apply(AddDefinition, "obncdoc", &acc);
	assert(acc.count == filenamesLen);

	/*sort definition files*/
	qsort(acc.filenames, filenamesLen, sizeof (char *), StringComparison);

	if (Files_Exists(indexFilename)) {
		indexFile = Files_Old(indexFilename, FILES_WRITE);
	} else {
		indexFile = Files_New(indexFilename);
	}
	PrintHtmlHeader(IndexTitle(), indexFile);
	fputs("		<p><a href='../index.html'>Index</a></p>\n", indexFile);
	fputc('\n', indexFile);
	fputs("		<pre>\n", indexFile);
	for (i = 0; i < filenamesLen; i++) {
		defFilename = acc.filenames[i];
		if (! DefFileEmpty(Util_String("obncdoc/%s", defFilename))) {
			module = Paths_SansSuffix(Paths_Basename(defFilename));
			fprintf(indexFile, "DEFINITION <a href='%s.def.html'>%s</a>\n", module, module);
		}
	}
	fputs("		</pre>\n", indexFile);
	PrintHtmlFooter(indexFile);
	Files_Close(&indexFile);
}


static void CreateCss(void)
{
	const char *cssFile, *command;
	int error;

	if (! Files_Exists("obncdoc/style.css")) {
		cssFile = Util_String("%s/share/obnc/style.css", Config_Prefix());
#ifdef _WIN32
		command = Util_String("copy %s obncdoc > NUL", Paths_ShellArg(Util_Replace("/", "\\", cssFile)));
#else
		command = Util_String("cp %s obncdoc", Paths_ShellArg(cssFile));
#endif
		error = system(command);
		if (error) {
			Error_Handle("");
		}
	}
}


static void CreateOutputFiles(void)
{
	CreateAllDefinitions();
	DeleteAllOrphanedDefinitions();
	CreateIndex();
	CreateCss();
}


int main(int argc, char *argv[])
{
	int i, helpWanted = 0, versionWanted = 0;
	const char *arg;

	Config_Init();
	Error_Init();
	Files_Init();
	Util_Init();
	Error_SetHandler(ExitFailure);

	for (i = 1; i < argc; i++) {
		arg = argv[i];
		if (strcmp(arg, "-h") == 0) {
			helpWanted = 1;
		} else if (strcmp(arg, "-v") == 0) {
			versionWanted = 1;
		} else {
			Error_Handle("Invalid command. Try 'obncdoc -h' for more information.");
		}
	}
	if (helpWanted) {
		PrintHelp();
	} else if (versionWanted) {
		PrintVersion();
	} else {
		CreateOutputFiles();
	}
	return 0;
}
