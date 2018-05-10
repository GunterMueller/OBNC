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
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

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


static char *String(int n, ...)
{
	int lengths[256];
	int resultLen, i, ofs;
	va_list args;
	char *result;

	assert(n < LEN(lengths));

	resultLen = 0;
	va_start(args, n);
	for (i = 0; i < n; i++) {
		lengths[i] = strlen(va_arg(args, char *));
		resultLen += lengths[i];
	}
	va_end(args);
	resultLen++;
	
	NEW_ARRAY(result, resultLen);
	result[0] = '\0';
	va_start(args, n);
	for (i = 0, ofs = 0; i < n; ofs += lengths[i], i++) {
		strcpy(result + ofs, va_arg(args, char *));
	}
	va_end(args);
	
	return result;
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
	const char *result;
	
	if ((strchr(s, '\'') != NULL) || (strchr(s, ' ') != NULL)) {
		result = String(3, "'", Replace("'", "'\"'\"'", Replace(" ", "\\ ", s)), "'");
	} else {
		result = s;
	}
	return result;
}


static char *Dirname(const char path[])
{
	/*NOTE: We need to copy the string twice since "The dirname() function may m\
odify the string pointed to by path, and may return a pointer to static storage\
 that may then be overwritten by a subsequent call to dirname()."*/
	assert(path != NULL);
	return String(1, dirname(String(1, path)));
}


static const char *Basename(const char path[])
{
	/*NOTE: We need to copy the string twice since "The basename() function may \
modify the string pointed to by path, and may return a pointer to internal stor\
age. The returned pointer might be invalidated or the storage might be overwrit\
ten by a subsequent call to basename()."*/
	assert(path != NULL);
	return String(1, basename(String(1, path)));
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
	return (strcmp(dir, ".") == 0)? "": String(2, dir, "/");
}


static const char *ObncCompilerPath(void)
{
	static const char *result;
	
	if (result == NULL) {
		result = String(2, Config_Prefix(), "/bin/obnc-compile");
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
	FILE *fp;
	
	fp = Files_Old(impFile, FILES_READ);
	ReadLines(fp, importedModules, importedModulesLen);
	Files_Close(fp);
}


static void GetImportedModulesFromSourceFile(const char oberonFile[], char ***importedModules, int *importedModulesLen)
{
	const char *dir, *file;
	const char *command;
	FILE *fp;
	int status;

	dir = Dirname(oberonFile);
	file = Basename(oberonFile);
	command = String(6, "cd ", ShellArg(dir), " && ", ShellArg(ObncCompilerPath()), " -l ", ShellArg(file));
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
	static const int impDirLen = PATH_MAX + 1;
	static char *impDir;
	const char *oberonFile, *impFile, *importedModule;
	char **importedModules;
	int importedModulesLen, i;
	
	oberonFile = String(4, dir, "/", module, ".obn");
	
	/*get imported modules*/
	impFile = String(4, dir, "/.obnc/", module, ".imp");
	if (! Files_Exists(impFile)) {
		impFile = String(4, dir, "/", module, ".imp");
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
		if (impDir == NULL) {
			NEW_ARRAY(impDir, impDirLen);
		}
		for (i = 0; i < *importedFilesLen; i++) {
			assert(i < importedModulesLen);
			importedModule = importedModules[i];
			Path_Get(importedModule, dir, impDir, impDirLen);
			if (impDir[0] != '\0') {
				(*importedFiles)[i] = String(3, AsPrefix(impDir), importedModule, ".obn");
			} else {
				fprintf(stderr, "obnc: Module imported by %s not found: %s\n", oberonFile, importedModules[i]);
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
		fprintf(stderr, "obnc: error: import cycle found: %s%s.obn", AsPrefix(moduleDir), importedModule);
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
	
	outputDir = String(2, dir, "/.obnc");
	
	/*backup current symbol file*/
	symFile = String(4, outputDir, "/", module, ".sym");
	symBakFile = String(2, symFile, ".bak");
	if (Files_Exists(symFile)) {
		Files_Move(symFile, symBakFile);
	} else {
		if (! Files_Exists(outputDir)) {
			Files_CreateDir(outputDir);
		}
	}

	entryPointOption = isEntryPoint? "-e": "";
	if (strcmp(dir, ".") == 0) {
		command = String(6, ShellArg(ObncCompilerPath()), " ", entryPointOption, " ", ShellArg(module), ".obn");
	} else {
		command = String(9, "cd ", ShellArg(dir), " && ", ShellArg(ObncCompilerPath()), " ", entryPointOption, " ", ShellArg(module), ".obn");
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
		result = String(1, s + 1);
		result[sLen - 2] = '\0';
	} else {
		result = String(1, s);
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
			key = String(1, lines[i]);
			key[p - lines[i]] = '\0';
			value = UnquotedString(String(1, p + 1));
		} else {
			key = String(1, "");
			value = String(1, "");
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
	
	inputFile = String(3, AsPrefix(dir), module, ".c");
	if (! Files_Exists(inputFile)) {
		inputFile = String(4, AsPrefix(dir), ".obnc/", module, ".c");
	}
	
	outputFile = String(4, AsPrefix(dir), ".obnc/", module, ".o");
	envFile = String(4, dir, "/", module, ".env");
	
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
				cc = String(1, values[i]);
			} else if (strcmp(keys[i], "CFLAGS") == 0) {
				moduleCFlags = String(1, values[i]);
			}
		}
	}
		
	command = String(9, cc, " -c -o ", ShellArg(outputFile), " ", globalCFlags, " ", moduleCFlags, " ", ShellArg(inputFile));
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

	oberonFile = String(4, dir, "/", module, ".obn");
	dirName = Basename(dir);
	dirFile = String(4, dir, "/.obnc/", module, ".dir");
	symFile = String(4, dir, "/.obnc/", module, ".sym");
	genCFile = String(4, dir, "/.obnc/", module, ".c");
	nonGenCFile = String(4, dir, "/", module, ".c");
	hFile = String(4, dir, "/.obnc/", module, ".h");
	objectFile = String(4, dir, "/.obnc/", module, ".o");
	envFile = nonGenCFile = String(4, dir, "/", module, ".env");

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
	oberonFile = String(4, dir, "/", module, ".obn");
	if (Files_Exists(oberonFile)) {
		UpdateObjectFile(module, dir, stale, isRoot);

		/*find out if the symbol file has changed*/
		symFile = String(4, dir, "/.obnc/", module, ".sym");
		symBakFile = String(2, symFile, ".bak");
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
	
	result = String(4, AsPrefix(dir), ".obnc/", module, ".o");
	if (! Files_Exists(result)) {
		result = String(3, AsPrefix(dir), module, ".o");
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
		envFile = String(4, envFileDir, "/", module, ".env");
		if (Files_Exists(envFile)) {
			ReadEnvFile(envFile, &keys, &values, &keysLen);
			for (j = 0; j < keysLen; j++) {
				if (strcmp(keys[j], "LDFLAGS") == 0) {
					ldFlags = String(3, ldFlags, " ", values[j]);
				} else if (strcmp(keys[j], "LDLIBS") == 0) {
					ldLibs = String(3, ldLibs, " ", values[j]);
				}
			}
		}
		objectFileArgs = String(3, objectFileArgs, " ", ShellArg(objectFiles[i]));
	}
		
	command = String(9, cc, " -o ", ShellArg(exeFile), " ", ldFlags, " ", objectFileArgs, " ", ldLibs);
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
	objectFiles[0] = String(4, Config_Prefix(), "/", Config_LibDir(), "/obnc/OBNC.o");
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
