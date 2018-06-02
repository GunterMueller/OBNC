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
#include "Files.h"
#include "Path.h"
#include "StackTrace.h"
#include "Util.h"
#include <libgen.h> /*POSIX*/
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

static int verbosity;

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


static const char *Replace(const char old[], const char new[], const char s[])
{
	const int newLength = strlen(new);
	char *t;
	int count, i, j, tLen;
	const char *p, *result;
	
	result = s;
	count = 0;
	p = strstr(s, old);
	if (p != NULL) {
		do {
			count++;
			p = strstr(p + 1, old);
		} while (p != NULL);
		
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


static const char *ShellArg(const char s[])
{
	int i;
	const char *result;

	i = 0;
	while ((s[i] != '\0') && ! isspace(s[i])) {
		i++;
	}
	if (s[i] == '\0') {
		result = s;
	} else {
		result = Util_String("'%s'", Replace("'", "'\"'\"'", s));
	}
	return result;
}


static char *Dirname(const char path[])
{
	/*NOTE: We need to copy the string twice since "The dirname() function may modify the string pointed to by path, and may return a pointer to static storage that may then be overwritten by a subsequent call to dirname()."*/
	assert(path != NULL);
	return Util_String("%s", dirname(Util_String("%s", path)));
}


static const char *Basename(const char path[])
{
	/*NOTE: We need to copy the string twice since "The basename() function may modify the string pointed to by path, and may return a pointer to internal storage. The returned pointer might be invalidated or the storage might be overwritten by a subsequent call to basename()."*/
	assert(path != NULL);
	return Util_String("%s", basename(Util_String("%s", path)));
}


static const char *SansExtension(const char filename[])
{
	char *result, *ext;
	
	NEW_ARRAY(result, strlen(filename) + 1);
	strcpy(result, filename);
	ext = strrchr(result, '.');
	if (ext != NULL) {
		*ext = '\0';		
	}
	return result;
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


static void ExitFailure(void)
{
	fputs("obnc: build process failed\n", stderr);
	exit(EXIT_FAILURE);
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
		fprintf(stderr, "obnc: reading line failed: %s\n", strerror(errno));
		ExitFailure();
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
			Files_Close(fp);
		} else {
			*importedModules = NULL;
			*importedModulesLen = 0;
		}
	} else {
		fprintf(stderr, "obnc: getting file size of '%s' failed: %s\n", impFile, strerror(errno));
		ExitFailure();
	}
}


static void GetImportedModulesFromSourceFile(const char oberonFile[], char ***importedModules, int *importedModulesLen)
{
	const char *dir, *file;
	const char *command;
	FILE *fp;
	int status;

	dir = Dirname(oberonFile);
	file = Basename(oberonFile);
	command = Util_String("cd %s && %s -l %s", ShellArg(dir), ShellArg(ObncCompilerPath()), ShellArg(file));
	fp = popen(command, "r");
	if (fp != NULL) {
		ReadLines(fp, importedModules, importedModulesLen);
		status = pclose(fp);
		if (status < 0) {
			fprintf(stderr, "obnc: closing pipe failed: %s\n", strerror(errno));
			ExitFailure();
		}
	} else {
		fprintf(stderr, "obnc: getting imported modules failed: %s\n", strerror(errno));
		ExitFailure();
	}
}


static void GetImportedFiles(const char module[], const char dir[], char ***importedFiles, int *importedFilesLen)
{
	const char *oberonFile, *impFile, *importedModule, *impDir;
	char **importedModules;
	int importedModulesLen, i;
	
	oberonFile = Util_String("%s%s.obn", AsPrefix(dir), module);
	
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
			impDir = Path_ModuleDir(importedModule, dir);
			if (impDir != NULL) {
				(*importedFiles)[i] = Util_String("%s%s.obn", AsPrefix(impDir), importedModule);
			} else {
				fprintf(stderr, "obnc: module imported by %s not found: %s\n", oberonFile, importedModules[i]);
				ExitFailure();
			}
		}
	}
}


static void DetectImportCycle(const char importedModule[], const char moduleDir[], ModuleList nodePath)
{
	ModuleList p, q;
	
	p = MatchingModuleNode(importedModule, moduleDir, nodePath);
	if (p != NULL) {
		fprintf(stderr, "obnc: import cycle found: %s%s.obn", AsPrefix(moduleDir), importedModule);
		q = nodePath;
		while (q != NULL) {
			fprintf(stderr, " <- %s%s.obn", AsPrefix(q->dir), q->module);
			q = q->next;
		}
		fputc('\n', stderr);
		ExitFailure();		
	}	
}


static void CompileOberon(const char module[], const char dir[], int isEntryPoint)
{
	const char *outputDir, *symFile, *symBakFile, *entryPointOption, *command;
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
	if (strcmp(dir, ".") == 0) {
		command = Util_String("%s %s %s.obn", ShellArg(ObncCompilerPath()), entryPointOption, ShellArg(module));
	} else {
		command = Util_String("cd %s && %s %s %s.obn", ShellArg(dir), ShellArg(ObncCompilerPath()), entryPointOption, ShellArg(module));
	}
	if (verbosity == 2) {
		puts(command);
	}
	error = system(command);
	if (error) {
		ExitFailure();
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
	Files_Close(fp);
}


static void CompileC(const char module[], const char dir[])
{
	const char *inputFile, *outputFile, *envFile, *cc, *globalCFlags, *moduleCFlags, *command;
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
		
	command = Util_String("%s -c -o %s %s %s %s", cc, ShellArg(outputFile), globalCFlags, moduleCFlags, ShellArg(inputFile));
	if (verbosity == 2) {
		puts(command);
	}
	error = system(command);
	if (error) {
		ExitFailure();
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
	CompileC(module, dir);
}


static void UpdateObjectFile(const char module[], const char dir[], int stale, int isEntryPoint)
{
	const char *oberonFile, *dirName, *symFile, *genCFile, *hFile, *dirFile, *objectFile, *envFile, *nonGenCFile;
	int dirFileUpToDate, done, oberonCompilationNeeded, cCompilationNeeded;
	FILE *fp;
	char *dirFileContent;

	oberonFile = Util_String("%s/%s.obn", dir, module);
	dirName = Basename(dir);
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
		Files_Close(fp);
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
	if (oberonCompilationNeeded
			|| ! Files_Exists(objectFile)
			|| (! Files_Exists(nonGenCFile) && (Files_Timestamp(objectFile) < Files_Timestamp(genCFile)))
			|| (Files_Exists(nonGenCFile) && (Files_Timestamp(objectFile) < Files_Timestamp(nonGenCFile)))
			|| (Files_Exists(envFile) && (Files_Timestamp(objectFile) < Files_Timestamp(envFile)))) {
		cCompilationNeeded = 1;
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
		Files_Close(fp);
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
	Files_Close(fp1);
	Files_Close(fp2);

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
		importedModule = SansExtension(Basename(importedFiles[i]));
		importedModuleDir = Dirname(importedFiles[i]);
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
	oberonFile = Util_String("%s/%s.obn", dir, module);
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
	const char *module = SansExtension(Basename(oberonFile));
	const char *dir = Dirname(oberonFile);
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


static char *ObjectFile(const char module[], const char dir[])
{
	char *result;
	
	result = Util_String("%s.obnc/%s.o", AsPrefix(dir), module);
	if (! Files_Exists(result)) {
		result = Util_String("%s%s.o", AsPrefix(dir), module);
		if (! Files_Exists(result)) {
			fprintf(stderr, "obnc: object file not found for module `%s' in directory `%s'\n", module, dir);
			ExitFailure();
		}
	}
	return result;
}


static void Link(const char *objectFiles[], int objectFilesLen, const char exeFile[])
{
	int keysLen, i, j, error;
	char **keys, **values;
	const char *cc, *ldFlags, *ldLibs, *objectFileArgs, *module, *envFileDir, *envFile, *command;

	cc = getenv("CC");
	if ((cc == NULL) || (strcmp(cc, "") == 0)) {
		cc = "cc";
	}
	
	ldFlags = getenv("LDFLAGS");
	if (ldFlags == NULL) {
		ldFlags = "";
	}
	ldLibs = getenv("LDLIBS");
	if (ldLibs == NULL) {
		ldLibs = "";
	}
	objectFileArgs = "";
	for (i = 0; i < objectFilesLen; i++) {
		module = SansExtension(Basename(objectFiles[i]));
		envFileDir = Dirname(objectFiles[i]);
		if (strcmp(Basename(envFileDir), ".obnc") == 0) {
			envFileDir = Dirname(envFileDir);
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
		objectFileArgs = Util_String("%s %s", objectFileArgs, ShellArg(objectFiles[i]));
	}
		
	command = Util_String("%s -o %s %s %s %s", cc, ShellArg(exeFile), ldFlags, objectFileArgs, ldLibs);
	if (verbosity == 1) {
		puts("Linking modules");
	} else if  (verbosity == 2) {
		printf("\nLinking modules:\n\n%s\n", command);
	}
	error = system(command);
	if (error) {
		ExitFailure();
	}
}


static void Build(const char oberonFile[])
{
	ModuleList discoveredModules, p;
	const char *newestObjectFile, *exeFile;
	int objectFilesLen, i;
	const char **objectFiles;
	
	discoveredModules = NULL;
	Traverse(oberonFile, &discoveredModules);
	
	objectFilesLen = 1;
	p = discoveredModules;
	while (p != NULL) {
		objectFilesLen++;
		p = p->next;
	}
	assert(objectFilesLen >= 2);
	
	NEW_ARRAY(objectFiles, objectFilesLen);
	objectFiles[0] = Util_String("%s/%s/obnc/OBNC.o", Config_Prefix(), Config_LibDir());
	i = 1;
	p = discoveredModules;
	while (p != NULL) {
		objectFiles[i] = ObjectFile(p->module, p->dir);
		i++;
		p = p->next;
	}
	
	newestObjectFile = NewestFile(objectFiles, objectFilesLen);
	exeFile = SansExtension(oberonFile);
	if (! Files_Exists(exeFile) || (Files_Timestamp(exeFile) < Files_Timestamp(newestObjectFile))) {
		Link(objectFiles, objectFilesLen, exeFile);
	} else {
		printf("%s is up to date\n", exeFile);
	}
}


static void ExitInvalidCommand(void)
{
	fputs(". Try 'obnc -h' for more information.\n", stderr);
	exit(EXIT_FAILURE);
}


static void PrintHelp(void)
{
	puts("obnc - build an executable for an Oberon module\n");
	puts("usage:");
	puts("\tobnc [-v | -V] MODULE.obn");
	puts("\tobnc [-h | -v]\n");
	puts("\t-v\tlog compiled modules or display version and exit");
	puts("\t-V\tlog compiler and linker commands");
	puts("\t-h\tdisplay help and exit");
}


static void PrintVersion(void)
{
	if (strcmp(CONFIG_VERSION, "") != 0) {
		printf("OBNC %s\n", CONFIG_VERSION);
	} else {
		puts("OBNC (unknown version)");
	}
}


int main(int argc, char *argv[])
{
	int i, hSet = 0, vSet = 0, VSet = 0, suffixValid;
	const char *arg, *inputFile = NULL;

	Util_Init();
	StackTrace_Init(NULL);

	for (i = 1; i < argc; i++) {
		arg = argv[i];
		if (strcmp(arg, "-h") == 0) {
			hSet = 1;
		} else if (strcmp(arg, "-v") == 0) {
			vSet = 1;
		} else if (strcmp(arg, "-V") == 0) {
			VSet = 1;
		} else if (arg[0] == '-') {
			fprintf(stderr, "obnc: invalid option: `%s'", arg);
			ExitInvalidCommand();
		} else if (inputFile == NULL) {
			suffixValid = strstr(arg, ".obn") == arg + strlen(arg) - strlen(".obn");
			if (suffixValid) {
				if (Files_Exists(arg)) {
					inputFile = arg;
				} else {
					fprintf(stderr, "obnc: no such file or directory: %s\n", arg);
					exit(EXIT_FAILURE);
				}
			} else {
				fputs("obnc: file extension .obn expected", stderr);
				ExitInvalidCommand();
			}
		} else {
			fputs("obnc: only one input file expected", stderr);
			ExitInvalidCommand();		
		}
	}	
	if (hSet) {
		PrintHelp();
	} else if (vSet && (inputFile == NULL)) {
		PrintVersion();
	} else if (inputFile != NULL) {
		if (VSet) {
			verbosity = 2;
		} else if (vSet) {
			verbosity = 1;
		}
		Build(inputFile);
	} else {
		fputs("obnc: no input file", stderr);
		ExitInvalidCommand();
	}
	return 0;
}
