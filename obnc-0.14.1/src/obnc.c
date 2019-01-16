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

#include "Config.h"
#include "Error.h"
#include "Files.h"
#include "ModulePaths.h"
#include "Paths.h"
#include "StackTrace.h"
#include "Util.h"
#include <sys/stat.h> /*POSIX*/
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ModuleNode *ModuleList;

struct ModuleNode {
	char *module, *dir;
	int stale;
	ModuleList next;
};

static const char *executableFile;
static int verbosity;
static int crossCompilationEnabled;

static ModuleList NewModuleNode(const char module[], const char dir[], ModuleList next)
{
	ModuleList result;

	NEW(result);
	NEW_ARRAY(result->module, strlen(module) + 1);
	strcpy(result->module, module);
	NEW_ARRAY(result->dir, strlen(dir) + 1);
	strcpy(result->dir, dir);
	result->stale = 0;
	result->next = next;
	return result;
}


static ModuleList MatchingModuleNode(const char module[], const char dir[], ModuleList ls)
{
	while ((ls != NULL) && ! ((strcmp(ls->module, module) == 0) && (strcmp(ls->dir, dir) == 0))) {
		ls = ls->next;
	}
	return ls;
}


static const char *AsPrefix(const char dir[])
{
	return (strcmp(dir, ".") == 0)? "": Util_String("%s/", dir);
}


static const char *ObncCompilerPath(void)
{
	static const char *result;

	if (result == NULL) {
		result = Util_String("%s/bin/obnc-compile", Config_Prefix());
	}
	return result;
}


static void ReadLine(FILE *fp, char **line, int *done)
{
	int resultLen, ch, i;
	char *result;

	resultLen = 256;
	NEW_ARRAY(result, resultLen);
	ch = fgetc(fp);
	i = 0;
	while ((ch != EOF) && (ch != '\n')) {
		if (i >= resultLen - 1) {
			resultLen *= 2;
			RENEW_ARRAY(result, resultLen);
		}
		result[i] = ch;
		ch = fgetc(fp);
		i++;
	}
	if (! ferror(fp)) {
		assert(i < resultLen);
		result[i] = '\0';
		*line = result;
		*done = ! ((i == 0) && (ch == EOF));
	} else {
		Error_Handle(Util_String("reading line failed: %s", strerror(errno)));
	}
}


static void ReadLines(FILE *fp, char ***lines, int *linesLen)
{
	int resultLen, i, done;
	char *line;
	char **result;

	resultLen = 256;
	NEW_ARRAY(result, resultLen);
	i = 0;
	ReadLine(fp, &line, &done);
	while (done) {
		result[i] = line;
		i++;
		if (i >= resultLen) {
			resultLen *= 2;
			RENEW_ARRAY(result, resultLen);
		}
		ReadLine(fp, &line, &done);
	}
	*lines = result;
	*linesLen = i;
}


static void GetImportedModulesFromImpFile(const char impFile[], char ***importedModules, int *importedModulesLen)
{
	struct stat st;
	int error;
	FILE *fp;

	error = stat(impFile, &st); /*for empty files, stat is faster than fopen/fclose*/
	if (! error) {
		if (st.st_size > 0) {
			fp = Files_Old(impFile, FILES_READ);
			ReadLines(fp, importedModules, importedModulesLen);
			Files_Close(&fp);
		} else {
			*importedModules = NULL;
			*importedModulesLen = 0;
		}
	} else {
		Error_Handle(Util_String("getting file size of '%s' failed: %s", impFile, strerror(errno)));
	}
}


static void GetImportedModulesFromSourceFile(const char oberonFile[], char ***importedModules, int *importedModulesLen)
{
	const char *dir, *file;
	const char *command;
	FILE *fp;
	int status;

	dir = Paths_Dirname(oberonFile);
	file = Paths_Basename(oberonFile);
	command = Util_String("cd %s && %s -l %s", Paths_ShellArg(dir), Paths_ShellArg(ObncCompilerPath()), Paths_ShellArg(file));
	fp = popen(command, "r");
	if (fp != NULL) {
		ReadLines(fp, importedModules, importedModulesLen);
		status = pclose(fp);
		if (status != 0) {
			if (status < 0) {
				Error_Handle(Util_String("closing pipe failed: %s", strerror(errno)));
			} else {
				Error_Handle("");
			}
		}
	} else {
		Error_Handle(Util_String("getting imported modules failed: %s", strerror(errno)));
	}
}


static void GetImportedFiles(const char module[], const char dir[], char ***importedFiles, int *importedFilesLen)
{
	const char *oberonFile, *impFile, *importedModule, *impDir;
	char **importedModules;
	int importedModulesLen, i;

	oberonFile = ModulePaths_SourceFile(module, dir);

	/*get imported modules*/
	impFile = Util_String("%s/.obnc/%s.imp", dir, module);
	if (! Files_Exists(impFile)) {
		impFile = Util_String("%s/%s.imp", dir, module);
	}
	if (Files_Exists(impFile) && (! Files_Exists(oberonFile) || (Files_Timestamp(impFile) >= Files_Timestamp(oberonFile)))) {
		GetImportedModulesFromImpFile(impFile, &importedModules, &importedModulesLen);
	} else if (Files_Exists(oberonFile)) {
		GetImportedModulesFromSourceFile(oberonFile, &importedModules, &importedModulesLen);
	} else {
		importedModulesLen = 0;
	}

	*importedFilesLen = importedModulesLen;
	if (*importedFilesLen > 0) {
		NEW_ARRAY(*importedFiles, *importedFilesLen);
		for (i = 0; i < *importedFilesLen; i++) {
			assert(i < importedModulesLen);
			importedModule = importedModules[i];
			impDir = ModulePaths_Directory(importedModule, dir);
			if (impDir != NULL) {
				(*importedFiles)[i] = Util_String("%s", ModulePaths_SourceFile(importedModule, impDir));
			} else {
				Error_Handle(Util_String("module imported by %s not found: %s", oberonFile, importedModules[i]));
			}
		}
	}
}


static void DetectImportCycle(const char importedModule[], const char moduleDir[], ModuleList nodePath)
{
	ModuleList p, q;
	const char *errorMsg;

	p = MatchingModuleNode(importedModule, moduleDir, nodePath);
	if (p != NULL) {
		errorMsg = Util_String("import cycle found: %s", ModulePaths_SourceFile(importedModule, moduleDir));
		q = nodePath;
		while (q != NULL) {
			errorMsg = Util_String("%s <- %s", errorMsg, ModulePaths_SourceFile(q->module, q->dir));
			q = q->next;
		}
		Error_Handle(errorMsg);
	}
}


static void CompileOberon(const char module[], const char dir[], int isEntryPoint)
{
	const char *outputDir, *inputFile, *symFile, *symBakFile, *entryPointOption, *command;
	int error;

	outputDir = Util_String("%s/.obnc", dir);

	/*backup current symbol file*/
	symFile = Util_String("%s/%s.sym", outputDir, module);
	symBakFile = Util_String("%s.bak", symFile);
	if (Files_Exists(symFile)) {
		Files_Move(symFile, symBakFile);
	} else {
		if (! Files_Exists(outputDir)) {
			Files_CreateDir(outputDir);
		}
	}

	entryPointOption = isEntryPoint? "-e": "";
	inputFile = Paths_Basename(ModulePaths_SourceFile(module, dir));
	if (strcmp(dir, ".") == 0) {
		command = Util_String("%s %s %s", Paths_ShellArg(ObncCompilerPath()), entryPointOption, Paths_ShellArg(inputFile));
	} else {
		command = Util_String("cd %s && %s %s %s", Paths_ShellArg(dir), Paths_ShellArg(ObncCompilerPath()), entryPointOption, Paths_ShellArg(inputFile));
	}
	if (verbosity == 2) {
		puts(command);
	}
	error = system(command);
	if (error) {
		Error_Handle("");
	}
}


static char *UnquotedString(const char s[])
{
	int sLen;
	char *result;

	sLen = strlen(s);
	if ((sLen > 0)
			&& (((s[0] == '\'') && (s[sLen - 1] == '\''))
				|| ((s[0] == '"') && (s[sLen - 1] == '"')))) {
		result = Util_String("%s", s + 1);
		result[sLen - 2] = '\0';
	} else {
		result = Util_String("%s", s);
	}
	return result;
}


static void ReadEnvFile(const char filename[], char **keys[], char **values[], int *len)
{
	FILE *fp;
	char **lines;
	int linesLen, i;
	const char *p;
	char *key, *value;

	fp = Files_Old(filename, FILES_READ);
	ReadLines(fp, &lines, &linesLen);
	*len = linesLen;
	NEW_ARRAY(*keys, *len);
	NEW_ARRAY(*values, *len);
	for (i = 0; i < linesLen; i++) {
		p = strchr(lines[i], '=');
		if (p != NULL) {
			key = Util_String("%s", lines[i]);
			key[p - lines[i]] = '\0';
			value = UnquotedString(Util_String("%s", p + 1));
		} else {
			key = Util_String("%s", "");
			value = Util_String("%s", "");
		}
		(*keys)[i] = key;
		(*values)[i] = value;
	}
	Files_Close(&fp);
}


static void CompileC(const char module[], const char dir[])
{
	const char *inputFile, *outputFile, *envFile, *cc, *includePath, *globalCFlags, *moduleCFlags, *cFlags, *command;
	char **keys, **values;
	int len, i, error;

	inputFile = Util_String("%s%s.c", AsPrefix(dir), module);
	if (! Files_Exists(inputFile)) {
		inputFile = Util_String("%s.obnc/%s.c", AsPrefix(dir), module);
	}

	outputFile = Util_String("%s.obnc/%s.o", AsPrefix(dir), module);
	envFile = Util_String("%s/%s.env", dir, module);

	cc = getenv("CC");
	if ((cc == NULL) || (strcmp(cc, "") == 0)) {
		cc = "cc";
	}

	globalCFlags = getenv("CFLAGS");
	if (globalCFlags == NULL) {
		globalCFlags = "";
	}
	includePath = Util_String("%s/include", Config_Prefix());
	globalCFlags = Util_String("-I %s %s", Paths_ShellArg(includePath), globalCFlags);

	moduleCFlags = "";
	if (Files_Exists(envFile)) {
		ReadEnvFile(envFile, &keys, &values, &len);
		for (i = 0; i < len; i++) {
			if (strcmp(keys[i], "CC") == 0) {
				cc = Util_String("%s", values[i]);
			} else if (strcmp(keys[i], "CFLAGS") == 0) {
				moduleCFlags = Util_String("%s", values[i]);
			}
		}
	}

	cFlags = Util_String("%s %s", globalCFlags, moduleCFlags);

	command = Util_String("%s -c -o %s %s %s", cc, Paths_ShellArg(outputFile), cFlags, Paths_ShellArg(inputFile));
	if (verbosity == 2) {
		puts(command);
	}
	if ((strstr(cFlags, "OBNC_CONFIG_TARGET_EMB=") != NULL) && ! crossCompilationEnabled) {
		Error_Handle("OBNC_CONFIG_TARGET_EMB can only be used with option -x");
	}
	error = system(command);
	if (error) {
		Error_Handle("");
	}
}


static void Compile(const char module[], const char dir[], int oberonCompilationNeeded, int isEntryPoint)
{
	if (verbosity == 1) {
		printf("Compiling module %s\n", module);
	} else if (verbosity == 2) {
		printf("\nCompiling module %s:\n\n", module);
	}
	if (oberonCompilationNeeded) {
		CompileOberon(module, dir, isEntryPoint);
	}
	if (! crossCompilationEnabled) {
		CompileC(module, dir);
	}
}


static void UpdateObjectFile(const char module[], const char dir[], int stale, int isEntryPoint)
{
	const char *oberonFile, *dirName, *symFile, *genCFile, *hFile, *dirFile, *objectFile, *envFile, *nonGenCFile;
	int dirFileUpToDate, done, oberonCompilationNeeded, cCompilationNeeded;
	FILE *fp;
	char *dirFileContent;

	oberonFile = ModulePaths_SourceFile(module, dir);
	dirName = Paths_Basename(dir);
	dirFile = Util_String("%s/.obnc/%s.dir", dir, module);
	symFile = Util_String("%s/.obnc/%s.sym", dir, module);
	genCFile = Util_String("%s/.obnc/%s.c", dir, module);
	nonGenCFile = Util_String("%s/%s.c", dir, module);
	hFile = Util_String("%s/.obnc/%s.h", dir, module);
	objectFile = Util_String("%s/.obnc/%s.o", dir, module);
	envFile = Util_String("%s/%s.env", dir, module);

	dirFileUpToDate = 0;
	if (Files_Exists(dirFile)) {
		fp = Files_Old(dirFile, FILES_READ);
		ReadLine(fp, &dirFileContent, &done);
		if (done && strcmp(dirFileContent, dirName) == 0) {
			dirFileUpToDate = 1;
		}
		Files_Close(&fp);
	}

	oberonCompilationNeeded = 0;
	if (stale
		|| ! Files_Exists(genCFile) || (Files_Timestamp(genCFile) < Files_Timestamp(oberonFile)
		|| (isEntryPoint && Files_Exists(symFile))
		|| (! isEntryPoint && (
			! Files_Exists(symFile) || (Files_Timestamp(symFile) < Files_Timestamp(oberonFile))
			|| ! Files_Exists(hFile) || (Files_Timestamp(hFile) < Files_Timestamp(oberonFile))
			|| ! dirFileUpToDate)))) {
		oberonCompilationNeeded = 1;
	}

	cCompilationNeeded = 0;
	if (! crossCompilationEnabled) {
		if (oberonCompilationNeeded
				|| ! Files_Exists(objectFile)
				|| (! Files_Exists(nonGenCFile) && (Files_Timestamp(objectFile) < Files_Timestamp(genCFile)))
				|| (Files_Exists(nonGenCFile) && (Files_Timestamp(objectFile) < Files_Timestamp(nonGenCFile)))
				|| (Files_Exists(envFile) && (Files_Timestamp(objectFile) < Files_Timestamp(envFile)))) {
			cCompilationNeeded = 1;
		}
	}

	if (oberonCompilationNeeded || cCompilationNeeded) {
		Compile(module, dir, oberonCompilationNeeded, isEntryPoint);
	}

	if (isEntryPoint) {
		if (Files_Exists(dirFile)) {
			Files_Remove(dirFile);
		}
	} else if (! dirFileUpToDate) {
		if (! Files_Exists(dirFile)) {
			fp = Files_New(dirFile);
		} else {
			fp = Files_Old(dirFile, FILES_WRITE);
		}
		fprintf(fp, "%s\n", dirName);
		Files_Close(&fp);
	}

}


static int FilesEqual(const char f1[], const char f2[])
{
	FILE *fp1, *fp2;
	int ch1, ch2;

	fp1 = Files_Old(f1, FILES_READ);
	fp2 = Files_Old(f2, FILES_READ);
	do {
		ch1 = getc(fp1);
		ch2 = getc(fp2);
	} while ((ch1 != EOF) && (ch1 == ch2));
	Files_Close(&fp1);
	Files_Close(&fp2);

	return (ch1 == EOF) && (ch2 == EOF);
}


static void Traverse1(const char module[], const char dir[], ModuleList nodePath, int isRoot, ModuleList *discoveredModules)
{
	char **importedFiles;
	int stale, symFileChanged, importedFilesLen, i;
	const char *importedModule, *importedModuleDir, *oberonFile, *symFile, *symBakFile;
	ModuleList newNodePath, p, moduleNode;

	*discoveredModules = NewModuleNode(module, dir, *discoveredModules);

	/*traverse imported files*/
	stale = 0;
	GetImportedFiles(module, dir, &importedFiles, &importedFilesLen);
	for (i = 0; i < importedFilesLen; i++) {
		importedModule = Paths_SansSuffix(Paths_Basename(importedFiles[i]));
		importedModuleDir = Paths_Dirname(importedFiles[i]);
		DetectImportCycle(importedModule, importedModuleDir, nodePath);
		if (! MatchingModuleNode(importedModule, importedModuleDir, *discoveredModules)) {
			newNodePath = NewModuleNode(importedModule, importedModuleDir, nodePath);
			Traverse1(importedModule, importedModuleDir, newNodePath, 0, discoveredModules);
		}
		p = MatchingModuleNode(importedModule, importedModuleDir, *discoveredModules);
		assert(p != NULL);
		if (p->stale) {
			stale = 1;
		}
	}

	symFileChanged = 0;
	oberonFile = ModulePaths_SourceFile(module, dir);
	if (Files_Exists(oberonFile)) {
		UpdateObjectFile(module, dir, stale, isRoot);

		/*find out if the symbol file has changed*/
		symFile = Util_String("%s/.obnc/%s.sym", dir, module);
		symBakFile = Util_String("%s.bak", symFile);
		if (Files_Exists(symFile) && Files_Exists(symBakFile)) {
			if (! FilesEqual(symFile, symBakFile)) {
				symFileChanged = 1;
			}
			Files_Remove(symBakFile);
		}
	}
	moduleNode = MatchingModuleNode(module, dir, *discoveredModules);
	assert(moduleNode != NULL);
	moduleNode->stale = symFileChanged;
}


static void Traverse(const char oberonFile[], ModuleList *discoveredModules)
{
	const char *module = Paths_SansSuffix(Paths_Basename(oberonFile));
	const char *dir = Paths_Dirname(oberonFile);
	ModuleList nodePath = NewModuleNode(module, dir, NULL);

	Traverse1(module, dir, nodePath, 1, discoveredModules);
}


static const char *NewestFile(const char *filenames[], int filenamesLen)
{
	const char *result;
	int i;

	assert(filenamesLen > 0);
	result = filenames[0];
	for (i = 1; i < filenamesLen; i++) {
		if (Files_Timestamp(filenames[i]) > Files_Timestamp(result)) {
			result = filenames[i];
		}
	}
	return result;
}


static char *CCInputFile(const char module[], const char dir[])
{
	int found;
	char *result;

	found = 0;
	if (crossCompilationEnabled) {
		result = Util_String("%s%s.c", AsPrefix(dir), module);
		found = Files_Exists(result);
		if (! found) {
			result = Util_String("%s.obnc/%s.c", AsPrefix(dir), module);
			found = Files_Exists(result);
		}
	}
	if (! found) {
		result = Util_String("%s.obnc/%s.o", AsPrefix(dir), module);
		if (! Files_Exists(result)) {
			result = Util_String("%s%s.o", AsPrefix(dir), module);
			if (! Files_Exists(result)) {
				Error_Handle(Util_String("object file not found for module `%s' in directory `%s'", module, dir));
			}
		}
	}
	return result;
}


static void DeleteArg(const char arg[], char argList[])
{
	int argLen;
	char *p;

	assert(arg != NULL);
	assert(argList != NULL);

	argLen = strlen(arg);
	if (argLen > 0) {
		p = strstr(argList, arg);
		while (p != NULL) {
			if (((p == argList) || isspace((p - 1)[0]))
					&& (isspace((p + argLen)[0]) || ((p + argLen)[0] == '\0'))) {
				strcpy(p, p + argLen);
			} else {
				p += argLen;
			}
			p = strstr(p, arg);
		}
	}
}


static void CreateExecutable(const char *inputFiles[], int inputFilesLen)
{
	int embedded, keysLen, i, j, error;
	char **keys, **values;
	char *ldLibs;
	const char *cc, *cFlags, *includePath, *ldFlags, *inputFileArgs, *module, *envFileDir, *envFile, *command;

	cc = getenv("CC");
	if ((cc == NULL) || (strcmp(cc, "") == 0)) {
		cc = "cc";
	}

	embedded = 0;
	if (crossCompilationEnabled) {
		cFlags = getenv("CFLAGS");
		if (cFlags != NULL) {
			embedded = strstr(cFlags, "OBNC_CONFIG_TARGET_EMB=1") != NULL;
		} else {
			cFlags = "";
		}
		includePath = Util_String("%s/include", Config_Prefix());
		cFlags = Util_String("-I %s %s", Paths_ShellArg(includePath), cFlags);
	} else {
		cFlags = "";
	}
	ldFlags = getenv("LDFLAGS");
	if (ldFlags == NULL) {
		ldFlags = "";
	}
	ldLibs = getenv("LDLIBS");
	if (ldLibs == NULL) {
		ldLibs = Util_String("%s", "");
	}
	inputFileArgs = "";
	for (i = 0; i < inputFilesLen; i++) {
		module = Paths_SansSuffix(Paths_Basename(inputFiles[i]));
		envFileDir = Paths_Dirname(inputFiles[i]);
		if (strcmp(Paths_Basename(envFileDir), ".obnc") == 0) {
			envFileDir = Paths_Dirname(envFileDir);
		}
		envFile = Util_String("%s/%s.env", envFileDir, module);
		if (Files_Exists(envFile)) {
			ReadEnvFile(envFile, &keys, &values, &keysLen);
			for (j = 0; j < keysLen; j++) {
				if (strcmp(keys[j], "LDFLAGS") == 0) {
					ldFlags = Util_String("%s %s", ldFlags, values[j]);
				} else if (strcmp(keys[j], "LDLIBS") == 0) {
					ldLibs = Util_String("%s %s", ldLibs, values[j]);
				}
			}
		}
		inputFileArgs = Util_String("%s %s", inputFileArgs, Paths_ShellArg(inputFiles[i]));
	}
	if (embedded) {
		DeleteArg("-lgc", ldLibs);
		DeleteArg("-lm", ldLibs);
	}

	command = Util_String("%s -o %s %s %s %s %s", cc, Paths_ShellArg(executableFile), cFlags, ldFlags, inputFileArgs, ldLibs);
	if (verbosity == 1) {
		printf("Creating executable %s\n", executableFile);
	} else if (verbosity == 2) {
		printf("\nCreating executable %s:\n\n%s\n", executableFile, command);
	}
	error = system(command);
	if (error) {
		Error_Handle("");
	}
}


static void Build(const char oberonFile[])
{
	ModuleList discoveredModules, p;
	const char *coreLibFile, *newestCCModule;
	int ccInputFilesLen, i;
	const char **ccInputFiles;

	discoveredModules = NULL;
	Traverse(oberonFile, &discoveredModules);

	ccInputFilesLen = 1;
	p = discoveredModules;
	while (p != NULL) {
		ccInputFilesLen++;
		p = p->next;
	}
	assert(ccInputFilesLen >= 2);

	if (crossCompilationEnabled) {
		coreLibFile = Util_String("%s/%s/obnc/OBNC.c", Config_Prefix(), Config_LibDir());
		if (! Files_Exists(coreLibFile)) {
			coreLibFile = Util_String("%s/%s/obnc/OBNC.o", Config_Prefix(), Config_LibDir());
		}
	} else {
		coreLibFile = Util_String("%s/%s/obnc/OBNC.o", Config_Prefix(), Config_LibDir());
	}

	NEW_ARRAY(ccInputFiles, ccInputFilesLen);
	ccInputFiles[0] = coreLibFile;
	i = 1;
	p = discoveredModules;
	while (p != NULL) {
		ccInputFiles[i] = CCInputFile(p->module, p->dir);
		i++;
		p = p->next;
	}

	newestCCModule = NewestFile(ccInputFiles, ccInputFilesLen);
	if (! Files_Exists(executableFile) || (Files_Timestamp(executableFile) < Files_Timestamp(newestCCModule))) {
		CreateExecutable(ccInputFiles, ccInputFilesLen);
	} else {
		printf("%s is up to date\n", executableFile);
	}
}


static void PrintHelp(void)
{
	puts("obnc - build an executable for an Oberon module\n");
	puts("usage:");
	puts("\tobnc [-o OUTFILE] [-v | -V] [-x] INFILE");
	puts("\tobnc [-h | -v]\n");
	puts("\t-o\tuse pathname OUTFILE for generated executable");
	puts("\t-v\tlog compiled modules or display version and exit");
	puts("\t-V\tlog compiler and linker commands");
	puts("\t-x\tcompile and link C files in one command (for cross compilation)");
	puts("\t-h\tdisplay help and exit");
	puts("");
	puts("\tINFILE is expected to end with .obn, .Mod or .mod");
}


static void PrintVersion(void)
{
	if (strcmp(CONFIG_VERSION, "") != 0) {
		printf("OBNC %s\n", CONFIG_VERSION);
	} else {
		puts("OBNC (unknown version)");
	}
}


static void ExitInvalidCommand(const char msg[])
{
	assert(msg != NULL);

	if (strcmp(msg, "") != 0) {
		fprintf(stderr, "obnc: %s", msg);
	}
	fprintf(stderr, ". Try 'obnc -h' for more information.\n");
	exit(EXIT_FAILURE);
}


static void ExitFailure(const char msg[])
{
	assert(msg != NULL);

	if (strcmp(msg, "") != 0) {
		fprintf(stderr, "obnc: %s\n", msg);
	}
	fprintf(stderr, "obnc: build process failed\n");
	exit(EXIT_FAILURE);
}


int main(int argc, char *argv[])
{
	int i, hSet = 0, vSet = 0, VSet = 0;
	const char *arg, *inputFile = NULL, *fileSuffix;

	Error_Init();
	Files_Init();
	ModulePaths_Init();
	Util_Init();
	StackTrace_Init(NULL);

	Error_SetHandler(ExitInvalidCommand);

	i = 1;
	while (i < argc) {
		arg = argv[i];
		if (strcmp(arg, "-h") == 0) {
			hSet = 1;
		} else if (strcmp(arg, "-o") == 0) {
			if ((i < argc - 1) && (argv[i + 1][0] != '-')) {
				executableFile = argv[i + 1];
				i++;
			} else {
				Error_Handle("output file parameter expected for option -o");
			}
		} else if (strcmp(arg, "-v") == 0) {
			vSet = 1;
		} else if (strcmp(arg, "-V") == 0) {
			VSet = 1;
		} else if (strcmp(arg, "-x") == 0) {
			crossCompilationEnabled = 1;
		} else if (arg[0] == '-') {
			Error_Handle(Util_String("invalid option: `%s'", arg));
		} else if (inputFile == NULL) {
			fileSuffix = strrchr(arg, '.');
			if ((fileSuffix != NULL)
					&& ((strcmp(fileSuffix, ".obn") == 0)
						|| (strcmp(fileSuffix, ".Mod") == 0)
						|| (strcmp(fileSuffix, ".mod") == 0))) {
				if (Files_Exists(arg)) {
					inputFile = arg;
				} else {
					Error_Handle(Util_String("no such file or directory: %s", arg));
				}
			} else {
				Error_Handle(Util_String("invalid or missing file suffix for input file: %s", arg));
			}
		} else {
			Error_Handle("only one input file expected");
		}
		i++;
	}

	if (hSet) {
		PrintHelp();
	} else if (vSet && (inputFile == NULL)) {
		PrintVersion();
	} else if (inputFile != NULL) {
		Error_SetHandler(ExitFailure);
		if (executableFile == NULL) {
#ifdef _WIN32
			executableFile = Util_String("%s.exe", Paths_SansSuffix(Paths_Basename(inputFile)));
#else
			executableFile = Paths_SansSuffix(Paths_Basename(inputFile));
#endif
		}
		if (VSet) {
			verbosity = 2;
		} else if (vSet) {
			verbosity = 1;
		}
		Build(inputFile);
	} else {
		Error_Handle("no input file");
	}
	return 0;
}
