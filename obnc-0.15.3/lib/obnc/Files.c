/*Copyright (C) 2017, 2018, 2019 Karl Landstrom <karl@miasap.se>

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.*/

#include ".obnc/Files.h"
#include <obnc/OBNC.h>
#ifdef _WIN32
	#include <windows.h>
#else
	#include <unistd.h>
#endif
#include <sys/stat.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define LEN(arr) ((int) (sizeof (arr) / sizeof (arr)[0]))

typedef struct Handle *File;

struct Handle {
	Files__Handle_ base;
	FILE *file;
	char *name;
	int registered;
};

struct HeapHandle {
	const OBNC_Td *td;
	struct Handle fields;
};

const int Files__Handle_id;
const int *const Files__Handle_ids[1] = {&Files__Handle_id};
const OBNC_Td Files__Handle_td = {Files__Handle_ids, 1};

const int Files__Rider_id;
const int *const Files__Rider_ids[1] = {&Files__Rider_id};
const OBNC_Td Files__Rider_td = {Files__Rider_ids, 1};

static int FileExists(const char name[])
{
#ifdef _WIN32
	DWORD attr = GetFileAttributes(name);

	return (attr != INVALID_FILE_ATTRIBUTES &&
		! (attr & FILE_ATTRIBUTE_DIRECTORY));
#else
	return access(name, F_OK ) != -1;
#endif
}


static void CheckTermination(const char s[], OBNC_LONGI int sLen)
{
	OBNC_LONGI int i;

	i = 0;
	while ((i < sLen) && (s[i] != '\0')) {
		i++;
	}
	OBNC_C_ASSERT(i < sLen);
}


static File NewFile(FILE *file, const char name[], int registered)
{
	File result;
	size_t nameLen;

	OBNC_C_ASSERT(file != NULL);
	OBNC_C_ASSERT(name != NULL);

	OBNC_NEW(result, &Files__Handle_td, struct HeapHandle, OBNC_REGULAR_ALLOC);
	if (result != NULL) {
		result->file = file;
		nameLen = strlen(name) + 1;
		result->name = OBNC_Allocate(nameLen, OBNC_ATOMIC_NOINIT_ALLOC);
		if (result->name != NULL) {
			memcpy(result->name, name, nameLen);
			result->registered = registered;
		} else {
			result = NULL;
		}
	}
	return result;
}


Files__File_ Files__Old_(const char name[], OBNC_LONGI int nameLen)
{
	FILE *file;
	File result;

	OBNC_C_ASSERT(name != NULL);
	OBNC_C_ASSERT(nameLen >= 0);
	CheckTermination(name, nameLen);

	file = fopen(name, "r+b");
	if (file == NULL) {
		file = fopen(name, "rb");
		if ((file == NULL) && FileExists(name)) {
			file = fopen(name, "ab");
		}
	}
	if (file != NULL) {
		result = NewFile(file, name, 1);
	} else {
		result = NULL;
	}
	return (Files__File_) result;
}


Files__File_ Files__New_(const char name[], OBNC_LONGI int nameLen)
{
	FILE *file;
	File result;

	OBNC_C_ASSERT(name != NULL);
	OBNC_C_ASSERT(nameLen >= 0);
	CheckTermination(name, nameLen);

	file = tmpfile();
	if (file != NULL) {
		result = NewFile(file, name, 0);
	} else {
		result = NULL;
		fprintf(stderr, "Files.New failed: %s\n", strerror(errno));
	}
	return (Files__File_) result;
}


static void Copy(FILE *src, FILE *dst, const char dstName[], int *done)
{
	int ch;

	rewind(src);
	ch = fgetc(src);
	while (ch != EOF) {
		ch = fputc(ch, dst);
		if (ch != EOF) {
			ch = fgetc(src);
		}
	}
	*done = ! ferror(src) && ! ferror(dst);
	if (ferror(src) || ferror(dst)) {
		fprintf(stderr, "Files.Register failed: %s: %s\n", dstName, strerror(errno));
	}
}


void Files__Register_(Files__File_ file)
{
	File f;
	FILE *new;
	int done;

	OBNC_C_ASSERT(file != NULL);

	f = (File) file;
	if (! f->registered) {
		new = fopen(f->name, "w+b");
		if (new != NULL) {
			Copy(f->file, new, f->name, &done);
			if (done) {
				f->file = new;
				f->registered = 1;
			}
		} else {
			fprintf(stderr, "Files.Register failed: %s: %s\n", f->name, strerror(errno));
		}
	}
}


void Files__Close_(Files__File_ file)
{
	File f;
	int error;

	OBNC_C_ASSERT(file != NULL);

	f = (File) file;
	error = fflush(f->file);
	if (error) {
		fprintf(stderr, "Files.Close failed: %s: %s\n", f->name, strerror(errno));
	}
}


void Files__Purge_(Files__File_ file)
{
	File f;
	int error;

	OBNC_C_ASSERT(file != NULL);

	f = ((File) file);
	error = fclose(f->file);
	if (! error) {
		if (f->registered) {
			f->file = fopen(f->name, "w+b");
		} else {
			f->file = tmpfile();
		}
		if (f->file == NULL) {
			fprintf(stderr, "Files.Purge failed: %s: %s\n", f->name, strerror(errno));
		}
	} else {
		fprintf(stderr, "Files.Purge failed: %s: %s\n", f->name, strerror(errno));
	}
}


void Files__Delete_(const char name[], OBNC_LONGI int nameLen, OBNC_LONGI int *res)
{
	OBNC_C_ASSERT(name != NULL);
	OBNC_C_ASSERT(nameLen >= 0);
	CheckTermination(name, nameLen);

	*res = unlink(name);
	if (*res != 0) {
		fprintf(stderr, "Files.Delete failed: %s: %s\n", name, strerror(errno));
	}
}


void Files__Rename_(const char old[], OBNC_LONGI int oldLen, const char new[], OBNC_LONGI int newLen, OBNC_LONGI int *res)
{
	OBNC_C_ASSERT(old != NULL);
	OBNC_C_ASSERT(oldLen >= 0);
	CheckTermination(old, oldLen);
	OBNC_C_ASSERT(new != NULL);
	OBNC_C_ASSERT(newLen >= 0);
	CheckTermination(new, newLen);

	*res = rename(old, new);
	if (*res != 0) {
		fprintf(stderr, "Files.Rename failed: %s: %s\n", old, strerror(errno));
	}
}


OBNC_LONGI int Files__Length_(Files__File_ file)
{
	File f;
	long int result;
	int error;

	OBNC_C_ASSERT(file != NULL);

	f = (File) file;
	result = 0;
	error = fseek(f->file, 0, SEEK_END);
	if (! error) {
		result = ftell(f->file);
		if (result < 0) {
			fprintf(stderr, "Files.Length failed: %s: %s\n", f->name, strerror(errno));
		}
#ifndef OBNC_USE_LONG_INT
		else if (result > INT_MAX) {
			fprintf(stderr, "Files.Length failed: %s: length exceeds maximum value of INTEGER (%d)\n", f->name, INT_MAX);
		}
#endif
	} else {
		fprintf(stderr, "Files.Length failed: %s: %s\n", f->name, strerror(errno));
	}
	return (OBNC_LONGI int) result;
}


void Files__GetDate_(Files__File_ file, OBNC_LONGI int *t, OBNC_LONGI int *d)
{
	File f;
	struct stat statResult;
	int error;
	struct tm *td;

	OBNC_C_ASSERT(file != NULL);
	OBNC_C_ASSERT(t != NULL);
	OBNC_C_ASSERT(d != NULL);

	f = (File) file;
	if (f->registered) {
		error = stat(f->name, &statResult);
		if (! error) {
			td = localtime(&(statResult.st_mtime));
			if (td != NULL) {
				*t = (td->tm_hour << 12) | (td->tm_min << 6) | td->tm_sec; /*59 < 2^6*/
				*d = ((1900 + td->tm_year) << 9) | ((td->tm_mon + 1) << 5) | td->tm_mday; /*31 < 2^5 and 12 < 2^4*/
			} else {
				fprintf(stderr, "Files.GetDate failed: %s: %s\n", f->name, strerror(errno));
			}
		} else {
			fprintf(stderr, "Files.GetDate failed: %s: %s\n", f->name, strerror(errno));
		}
	} else {
		*t = 0;
		*d = 0;
		fprintf(stderr, "Files.GetDate failed: %s: cannot get date of unregistered file\n", f->name);
	}
}


void Files__Set_(Files__Rider_ *r, const OBNC_Td *rTD, Files__File_ f, OBNC_LONGI int pos)
{
	OBNC_C_ASSERT(r != NULL);
	OBNC_C_ASSERT(f != NULL);
	OBNC_C_ASSERT(pos >= 0);
	OBNC_C_ASSERT(pos <= Files__Length_(f));

	r->eof_ = 0;
	r->base_ = f;
	r->pos_ = pos;
}


OBNC_LONGI int Files__Pos_(Files__Rider_ *r, const OBNC_Td *rTD)
{
	OBNC_C_ASSERT(r != NULL);
	OBNC_C_ASSERT(r->base_ != NULL);

	return r->pos_;
}


Files__File_ Files__Base_(Files__Rider_ *r, const OBNC_Td *rTD)
{
	OBNC_C_ASSERT(r != NULL);
	OBNC_C_ASSERT(r->base_ != NULL);

	return r->base_;
}


static void Position(const Files__Rider_ *r, FILE **fp)
{
	File f;
	int error;

	f = (File) (r->base_);
	*fp = f->file;
	error = fseek(*fp, r->pos_, SEEK_SET);
	if (error) {
		fprintf(stderr, "Positioning rider failed: %s: %s\n", f->name, strerror(errno));
		*fp = NULL;
	}
}


static const char *BaseName(const Files__Rider_ *r)
{
	return ((File) (r->base_))->name;
}


void Files__Read_(Files__Rider_ *r, const OBNC_Td *rTD, unsigned char *x)
{
	FILE *fp;
	int ch;

	OBNC_C_ASSERT(r != NULL);
	OBNC_C_ASSERT(r->base_ != NULL);
	OBNC_C_ASSERT(x != NULL);

	Position(r, &fp);
	if (fp != NULL) {
		ch = fgetc(fp);
		if (ch != EOF) {
			*x = (unsigned char) ch;
			r->pos_++;
		} else {
			if (feof(fp)) {
				r->eof_ = 1;
			} else if (ferror(fp)) {
				fprintf(stderr, "Files.Read failed: %s: %s\n", BaseName(r), strerror(errno));
			}
		}
	}
}


void Files__ReadInt_(Files__Rider_ *r, const OBNC_Td *rTD, OBNC_LONGI int *i)
{
	FILE *fp;
	size_t n;

	OBNC_C_ASSERT(r != NULL);
	OBNC_C_ASSERT(r->base_ != NULL);
	OBNC_C_ASSERT(i != NULL);

	Position(r, &fp);
	if (fp != NULL) {
		n = fread(i, sizeof (*i), 1, fp);
		r->pos_ += (OBNC_LONGI int) (n * sizeof (*i));
		if (feof(fp)) {
			r->eof_ = 1;
		} else if (ferror(fp)) {
			fprintf(stderr, "Files.ReadInt failed: %s: %s\n", BaseName(r), strerror(errno));
		}
	}
}


void Files__ReadReal_(Files__Rider_ *r, const OBNC_Td *rTD, OBNC_LONGR double *x)
{
	FILE *fp;
	size_t n;

	OBNC_C_ASSERT(r != NULL);
	OBNC_C_ASSERT(r->base_ != NULL);
	OBNC_C_ASSERT(x != NULL);

	Position(r, &fp);
	if (fp != NULL) {
		n = fread(x, sizeof (*x), 1, fp);
		r->pos_ += (OBNC_LONGI int) (n * sizeof (*x));
		if (feof(fp)) {
			r->eof_ = 1;
		} else if (ferror(fp)) {
			fprintf(stderr, "Files.ReadReal failed: %s: %s\n", BaseName(r), strerror(errno));
		}
	}
}


void Files__ReadNum_(Files__Rider_ *r, const OBNC_Td *rTD, OBNC_LONGI int *x)
{
	FILE *fp;
	OBNC_LONGI int s, n;
	int ch;

	OBNC_C_ASSERT(r != NULL);
	OBNC_C_ASSERT(r->base_ != NULL);
	OBNC_C_ASSERT(x != NULL);

	Position(r, &fp);
	if (fp != NULL) {
		s = 0;
		n = 0;
		ch = fgetc(fp);
		while (ch >= 128) {
			r->pos_++;
			n += (ch - 128) << s;
			s += 7;
			ch = fgetc(fp);
		}
		if (ch != EOF) {
			r->pos_++;
			*x = n + ((OBNC_MOD(ch, 64) - OBNC_DIV(ch, 64) * 64) << s);
		} else {
			if (feof(fp)) {
				r->eof_ = 1;
			} else if (ferror(fp)) {
				fprintf(stderr, "Files.ReadNum failed: %s: %s\n", BaseName(r), strerror(errno));
			}
		}
	}
}


void Files__ReadString_(Files__Rider_ *r, const OBNC_Td *rTD, char s[], OBNC_LONGI int sLen)
{
	FILE *fp;
	int ch;
	OBNC_LONGI int i;

	OBNC_C_ASSERT(r != NULL);
	OBNC_C_ASSERT(r->base_ != NULL);
	OBNC_C_ASSERT(s != NULL);
	OBNC_C_ASSERT(sLen >= 0);

	Position(r, &fp);
	if (fp != NULL) {
		ch = fgetc(fp);
		i = 0;
		while ((ch != EOF) && (ch != '\0') && (i < sLen - 1)) {
			s[i] = (char) ch;
			ch = fgetc(fp);
			i++;
		}
		if (ch != EOF) {
			if (ch == '\0') {
				s[i] = '\0';
			} else { /*string doesn't fit*/
				s[0] = '\0';
			}
			r->pos_ += i + 1;
		} else {
			if (feof(fp)) {
				r->eof_ = 1;
			} else if (ferror(fp)) {
				fprintf(stderr, "Files.ReadString failed: %s: %s\n", BaseName(r), strerror(errno));
			}
		}
	}
}


void Files__ReadSet_(Files__Rider_ *r, const OBNC_Td *rTD, OBNC_LONGI unsigned int *s)
{
	FILE *fp;
	size_t n;

	OBNC_C_ASSERT(r != NULL);
	OBNC_C_ASSERT(r->base_ != NULL);
	OBNC_C_ASSERT(s != NULL);

	Position(r, &fp);
	if (fp != NULL) {
		n = fread(s, sizeof (*s), 1, fp);
		r->pos_ += (OBNC_LONGI int) (n * sizeof (*s));
		if (feof(fp)) {
			r->eof_ = 1;
		} else if (ferror(fp)) {
			fprintf(stderr, "Files.ReadSet failed: %s: %s\n", BaseName(r), strerror(errno));
		}
	}
}


void Files__ReadBool_(Files__Rider_ *r, const OBNC_Td *rTD, int *b)
{
	FILE *fp;
	int ch;

	OBNC_C_ASSERT(r != NULL);
	OBNC_C_ASSERT(r->base_ != NULL);
	OBNC_C_ASSERT(b != NULL);

	Position(r, &fp);
	if (fp != NULL) {
		ch = fgetc(fp);
		if (ch != EOF) {
			*b = ch;
			r->pos_++;
		} else {
			if (feof(fp)) {
				r->eof_ = 1;
			} if (ferror(fp)) {
				fprintf(stderr, "Files.ReadBool failed: %s: %s\n", BaseName(r), strerror(errno));
			}
		}
	}
}


static OBNC_LONGI int Min(OBNC_LONGI int a, OBNC_LONGI int b)
{
	return (a < b)? a: b;
}


void Files__ReadBytes_(Files__Rider_ *r, const OBNC_Td *rTD, unsigned char buf[], OBNC_LONGI int bufLen, OBNC_LONGI int n)
{
	FILE *fp;
	size_t nRead;

	OBNC_C_ASSERT(r != NULL);
	OBNC_C_ASSERT(r->base_ != NULL);
	OBNC_C_ASSERT(buf != NULL);
	OBNC_C_ASSERT(bufLen >= 0);
	OBNC_C_ASSERT(n >= 0);

	Position(r, &fp);
	if (fp != NULL) {
		nRead = fread(buf, sizeof buf[0], (size_t) Min(n, bufLen), fp);
		r->pos_ += (OBNC_LONGI int) (nRead * sizeof buf[0]);
		r->res_ = n - (OBNC_LONGI int) nRead;
		if (feof(fp)) {
			r->eof_ = 1;
		} else if (ferror(fp)) {
			fprintf(stderr, "Files.ReadBytes failed: %s: %s\n", BaseName(r), strerror(errno));
		}
	}
}


void Files__Write_(Files__Rider_ *r, const OBNC_Td *rTD, unsigned char x)
{
	FILE *fp;
	int res;

	OBNC_C_ASSERT(r != NULL);
	OBNC_C_ASSERT(r->base_ != NULL);

	Position(r, &fp);
	if (fp != NULL) {
		res = fputc(x, fp);
		if (res != EOF) {
			r->pos_++;
		} else {
			fprintf(stderr, "Files.Write failed: %s: %s\n", BaseName(r), strerror(errno));
		}
	}
}


void Files__WriteInt_(Files__Rider_ *r, const OBNC_Td *rTD, OBNC_LONGI int i)
{
	FILE *fp;
	size_t n;

	OBNC_C_ASSERT(r != NULL);
	OBNC_C_ASSERT(r->base_ != NULL);

	Position(r, &fp);
	if (fp != NULL) {
		n = fwrite(&i, sizeof i, 1, fp);
		r->pos_ += (OBNC_LONGI int) (n * sizeof i);
		if (ferror(fp)) {
			fprintf(stderr, "Files.WriteInt failed: %s: %s\n", BaseName(r), strerror(errno));
		}
	}
}


void Files__WriteReal_(Files__Rider_ *r, const OBNC_Td *rTD, OBNC_LONGR double x)
{
	FILE *fp;
	size_t n;

	OBNC_C_ASSERT(r != NULL);
	OBNC_C_ASSERT(r->base_ != NULL);

	Position(r, &fp);
	if (fp != NULL) {
		n = fwrite(&x, sizeof x, 1, fp);
		r->pos_ += (OBNC_LONGI int) (n * sizeof x);
		if (ferror(fp)) {
			fprintf(stderr, "Files.WriteReal failed: %s: %s\n", BaseName(r), strerror(errno));
		}
	}
}


void Files__WriteNum_(Files__Rider_ *r, const OBNC_Td *rTD, OBNC_LONGI int x)
{
	FILE *fp;
	int i;
	char buf[CHAR_BIT * sizeof x]; /* 10^x = 2^n implies x < n */
	size_t n;

	OBNC_C_ASSERT(r != NULL);
	OBNC_C_ASSERT(r->base_ != NULL);

	i = 0;
	while ((x < -64) || (x > 63)) {
		OBNC_C_ASSERT(i < LEN(buf));
		buf[i] = (char) (OBNC_MOD(x, 128) + 128);
		x = OBNC_DIV(x, 128);
		i++;
	}
	OBNC_C_ASSERT(i < LEN(buf));
	buf[i] = (char) OBNC_MOD(x, 128);

	Position(r, &fp);
	if (fp != NULL) {
		n = fwrite(buf, sizeof buf[0], (size_t) i + 1, fp);
		r->pos_ += (OBNC_LONGI int) (n * sizeof buf[0]);
		if (ferror(fp)) {
			fprintf(stderr, "Files.WriteNum failed: %s: %s\n", BaseName(r), strerror(errno));
		}

	}
}


void Files__WriteString_(Files__Rider_ *r, const OBNC_Td *rTD, const char s[], OBNC_LONGI int sLen)
{
	FILE *fp;
	size_t n;

	OBNC_C_ASSERT(r != NULL);
	OBNC_C_ASSERT(r->base_ != NULL);
	OBNC_C_ASSERT(s != NULL);
	OBNC_C_ASSERT(sLen >= 0);
	CheckTermination(s, sLen);

	Position(r, &fp);
	if (fp != NULL) {
		n = fwrite(s, sizeof s[0], strlen(s) + 1, fp);
		r->pos_ += (OBNC_LONGI int) (n * sizeof s[0]);
		if (ferror(fp)) {
			fprintf(stderr, "Files.WriteString failed: %s: %s\n", BaseName(r), strerror(errno));
		}
	}
}


void Files__WriteSet_(Files__Rider_ *r, const OBNC_Td *rTD, OBNC_LONGI unsigned int s)
{
	FILE *fp;
	size_t n;

	OBNC_C_ASSERT(r != NULL);
	OBNC_C_ASSERT(r->base_ != NULL);

	Position(r, &fp);
	if (fp != NULL) {
		n = fwrite(&s, sizeof s, 1, fp);
		r->pos_ += (OBNC_LONGI int) (n * sizeof s);
		if (ferror(fp)) {
			fprintf(stderr, "Files.WriteSet failed: %s: %s\n", BaseName(r), strerror(errno));
		}
	}
}


void Files__WriteBool_(Files__Rider_ *r, const OBNC_Td *rTD, int b)
{
	FILE *fp;
	int res;

	OBNC_C_ASSERT(r != NULL);
	OBNC_C_ASSERT(r->base_ != NULL);

	Position(r, &fp);
	if (fp != NULL) {
		res = fputc(!! b, fp);
		if (res != EOF) {
			r->pos_++;
		} else {
			fprintf(stderr, "Files.WriteBool failed: %s: %s\n", BaseName(r), strerror(errno));
		}
	}
}


void Files__WriteBytes_(Files__Rider_ *r, const OBNC_Td *rTD, unsigned char buf[], OBNC_LONGI int bufLen, OBNC_LONGI int n)
{
	FILE *fp;
	size_t nWritten;

	OBNC_C_ASSERT(r != NULL);
	OBNC_C_ASSERT(r->base_ != NULL);
	OBNC_C_ASSERT(buf != NULL);
	OBNC_C_ASSERT(bufLen >= 0);
	OBNC_C_ASSERT(n >= 0);
	OBNC_C_ASSERT(n <= bufLen);

	Position(r, &fp);
	if (fp != NULL) {
		nWritten = fwrite(buf, sizeof buf[0], (size_t) n, fp);
		r->pos_ += (OBNC_LONGI int) (nWritten * sizeof buf[0]);
		r->res_ = n - (OBNC_LONGI int) nWritten;
		if (ferror(fp)) {
			fprintf(stderr, "Files.WriteBytes failed: %s: %s\n", BaseName(r), strerror(errno));
		}
	}
}


void Files__Init(void)
{
}
