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

#include ".obnc/Files.h"
#include "OBNC_.h"
#include "../../src/Util.h"
#include <unistd.h>
#include <sys/stat.h>
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct Handle *File;

struct Handle {
	Files_Handle_ base;
	FILE *file;
	char *name;
	int registered;
};

struct HeapHandle {
	const OBNC_Td *td;
	struct Handle fields;
};

const int Files_Handle_id;
const int *const Files_Handle_ids[1] = {&Files_Handle_id};
const OBNC_Td Files_Handle_td = {Files_Handle_ids, 1};

const int Files_Rider_id;
const int *const Files_Rider_ids[1] = {&Files_Rider_id};
const OBNC_Td Files_Rider_td = {Files_Rider_ids, 1};

static int FileExists(const char name[])
{
	return access(name, F_OK ) != -1;
}


static void CheckTermination(const char s[], int sLen)
{
	int i;
	
	i = 0;
	while ((i < sLen) && (s[i] != '\0')) {
		i++;
	}
	OBNC_IT(i, sLen);
}


static File NewFile(FILE *file, const char name[], int registered)
{
	File result;
	int nameLen;
	
	assert(file != NULL);
	assert(name != NULL);
	
	OBNC_NEW(result, &Files_Handle_td, struct HeapHandle);
	if (result != NULL) {
		result->file = file;
		nameLen = strlen(name) + 1;
		NEW_ARRAY(result->name, nameLen);
		if (result->name != NULL) {
			memcpy(result->name, name, nameLen);
			result->registered = registered;
		} else {
			result = NULL;
		}
	}
	return result;
}


Files_File_ Files_Old_(const char name[], int nameLen)
{
	FILE *file;
	File result;

	assert(name != NULL);
	assert(nameLen >= 0);
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
	return (Files_File_) result;
}


Files_File_ Files_New_(const char name[], int nameLen)
{
	FILE *file;
	File result;
	
	assert(name != NULL);
	assert(nameLen >= 0);
	CheckTermination(name, nameLen);

	file = tmpfile();
	if (file != NULL) {
		result = NewFile(file, name, 0);
	} else {
		result = NULL;
		fprintf(stderr, "Files.New failed: %s\n", strerror(errno));
	}
	return (Files_File_) result;
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


void Files_Register_(Files_File_ file)
{
	File f;
	FILE *new;
	int done;
	
	OBNC_PT(file);

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


void Files_Close_(Files_File_ file)
{
	File f;
	int error;
	
	OBNC_PT(file);

	f = (File) file;
	error = fflush(f->file);
	if (error) {
		fprintf(stderr, "Files.Close failed: %s: %s\n", f->name, strerror(errno));
	}
}


void Files_Purge_(Files_File_ file)
{
	File f;
	int error;
	
	OBNC_PT(file);
	
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


void Files_Delete_(const char name[], int nameLen, int *res)
{
	assert(name != NULL);
	assert(nameLen >= 0);
	CheckTermination(name, nameLen);
	
	*res = unlink(name);
	if (*res != 0) {
		fprintf(stderr, "Files.Delete failed: %s: %s\n", name, strerror(errno));
	}
}


void Files_Rename_(const char old[], int oldLen, const char new[], int newLen, int *res)
{
	assert(old != NULL);
	assert(oldLen >= 0);
	CheckTermination(old, oldLen);
	assert(new != NULL);
	assert(newLen >= 0);	
	CheckTermination(new, newLen);
	
	*res = rename(old, new);
	if (*res != 0) {
		fprintf(stderr, "Failed renaming file: %s: %s\n", old, strerror(errno));
	}
}


int Files_Length_(Files_File_ file)
{
	File f;
	long int result;
	int error;
	
	OBNC_PT(file);
	
	f = (File) file;
	result = 0;
	error = fseek(f->file, 0, SEEK_END);
	if (! error) {
		result = ftell(f->file);
		if (result < 0) {
			fprintf(stderr, "Files.Length failed: %s: %s\n", f->name, strerror(errno));
		} else if (result > INT_MAX) {
			fprintf(stderr, "Files.Length failed: %s: length exceeds maximum value of INTEGER (%d)\n", f->name, INT_MAX);
		}
	} else {
		fprintf(stderr, "Files.Length failed: %s: %s\n", f->name, strerror(errno));
	}
	return result;
}


void Files_GetDate_(Files_File_ file, int *t, int *d)
{
	File f;
	struct stat statResult;
	int error;
	struct tm *td;

	OBNC_PT(file);
	assert(t != NULL);
	assert(d != NULL);

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


void Files_Set_(Files_Rider_ *r, const OBNC_Td *rTD, Files_File_ f, int pos)
{
	assert(r != NULL);
	OBNC_PT(f);
	assert(pos >= 0);
	assert(pos <= Files_Length_(f));

	r->eof_ = 0;
	r->base_ = f;
	r->pos_ = pos;
}


int Files_Pos_(Files_Rider_ *r, const OBNC_Td *rTD)
{
	assert(r != NULL);
	OBNC_PT(r->base_);
	
	return r->pos_;
}


Files_File_ Files_Base_(Files_Rider_ *r, const OBNC_Td *rTD)
{
	assert(r != NULL);
	OBNC_PT(r->base_);

	return r->base_;
}


static void Position(const Files_Rider_ *r, FILE **fp)
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


static const char *BaseName(const Files_Rider_ *r)
{
	return ((File) (r->base_))->name;
}


void Files_Read_(Files_Rider_ *r, const OBNC_Td *rTD, unsigned char *x)
{
	FILE *fp;
	int ch;
	
	assert(r != NULL);
	OBNC_PT(r->base_);
	assert(x != NULL);
	
	Position(r, &fp);
	if (fp != NULL) {
		ch = fgetc(fp);
		if (ch != EOF) {
			*x = ch;
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


void Files_ReadInt_(Files_Rider_ *r, const OBNC_Td *rTD, int *i)
{
	FILE *fp;
	size_t n;
	
	assert(r != NULL);
	OBNC_PT(r->base_);
	assert(i != NULL);
	
	Position(r, &fp);
	if (fp != NULL) {
		n = fread(i, sizeof (*i), 1, fp);
		r->pos_ += n * sizeof (*i);
		if (feof(fp)) {
			r->eof_ = 1;
		} else if (ferror(fp)) {
			fprintf(stderr, "Files.ReadInt failed: %s: %s\n", BaseName(r), strerror(errno));
		}
	}
}


void Files_ReadReal_(Files_Rider_ *r, const OBNC_Td *rTD, double *x)
{
	FILE *fp;
	size_t n;
	
	assert(r != NULL);
	OBNC_PT(r->base_);
	assert(x != NULL);
	
	Position(r, &fp);
	if (fp != NULL) {
		n = fread(x, sizeof (*x), 1, fp);
		r->pos_ += n * sizeof (*x);
		if (feof(fp)) {
			r->eof_ = 1;		
		} else if (ferror(fp)) {
			fprintf(stderr, "Files.ReadReal failed: %s: %s\n", BaseName(r), strerror(errno));
		}
	}
}


static int Mod(int x, int y)
{
	return (x >= 0)? x % y: (x % y + y) % y;
}


static int Div(int x, int y)
{
	return (x >= 0)? x / y: (x - Mod(x, y)) / y;
}


void Files_ReadNum_(Files_Rider_ *r, const OBNC_Td *rTD, int *x)
{
	FILE *fp;
	int s, n;
	int ch;
	
	assert(r != NULL);
	OBNC_PT(r->base_);
	assert(x != NULL);
	
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
			*x = n + ((Mod(ch, 64) - Div(ch, 64) * 64) << s);
		} else {
			if (feof(fp)) {
				r->eof_ = 1;
			} else if (ferror(fp)) {
				fprintf(stderr, "Files.Read failed: %s: %s\n", BaseName(r), strerror(errno));
			}
		}
	}
}


void Files_ReadString_(Files_Rider_ *r, const OBNC_Td *rTD, char s[], int sLen)
{
	FILE *fp;
	int ch, i;
	
	assert(r != NULL);
	OBNC_PT(r->base_);
	assert(s != NULL);
	assert(sLen >= 0);
	
	Position(r, &fp);
	if (fp != NULL) {
		ch = fgetc(fp);
		i = 0;
		while ((ch != EOF) && (ch != '\0') && (i < sLen - 1)) {
			s[i] = ch;
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


void Files_ReadSet_(Files_Rider_ *r, const OBNC_Td *rTD, unsigned int *s)
{
	FILE *fp;
	size_t n;
	
	assert(r != NULL);
	OBNC_PT(r->base_);
	assert(s != NULL);
	
	Position(r, &fp);
	if (fp != NULL) {
		n = fread(s, sizeof (*s), 1, fp);
		r->pos_ += n * sizeof (*s);
		if (feof(fp)) {
			r->eof_ = 1;
		} else if (ferror(fp)) {
			fprintf(stderr, "Files.ReadSet failed: %s: %s\n", BaseName(r), strerror(errno));
		}
	}
}


void Files_ReadBool_(Files_Rider_ *r, const OBNC_Td *rTD, int *b)
{
	FILE *fp;
	int ch;
	
	assert(r != NULL);
	OBNC_PT(r->base_);
	assert(b != NULL);
	
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


static int Min(int a, int b)
{
	return (a < b)? a: b;
}


void Files_ReadBytes_(Files_Rider_ *r, const OBNC_Td *rTD, unsigned char buf[], int bufLen, int n)
{
	FILE *fp;
	size_t nRead;
	
	assert(r != NULL);
	OBNC_PT(r->base_);
	assert(buf != NULL);
	assert(bufLen >= 0);
	assert(n >= 0);
	
	Position(r, &fp);
	if (fp != NULL) {
		nRead = fread(buf, sizeof buf[0], Min(n, bufLen), fp);
		r->pos_ += nRead * sizeof buf[0];
		r->res_ = n - nRead;
		if (feof(fp)) {
			r->eof_ = 1;
		} else if (ferror(fp)) {
			fprintf(stderr, "Files.ReadBytes failed: %s: %s\n", BaseName(r), strerror(errno));
		}
	}
}


void Files_Write_(Files_Rider_ *r, const OBNC_Td *rTD, unsigned char x)
{
	FILE *fp;
	int res;
	
	assert(r != NULL);
	OBNC_PT(r->base_);
	
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


void Files_WriteInt_(Files_Rider_ *r, const OBNC_Td *rTD, int i)
{
	FILE *fp;
	size_t n;
	
	assert(r != NULL);
	OBNC_PT(r->base_);
	
	Position(r, &fp);
	if (fp != NULL) {
		n = fwrite(&i, sizeof i, 1, fp);
		r->pos_ += n * sizeof i;
		if (ferror(fp)) {
			fprintf(stderr, "Files.WriteInt failed: %s: %s\n", BaseName(r), strerror(errno));
		}
	}
}


void Files_WriteReal_(Files_Rider_ *r, const OBNC_Td *rTD, double x)
{
	FILE *fp;
	size_t n;
	
	assert(r != NULL);
	OBNC_PT(r->base_);
	
	Position(r, &fp);
	if (fp != NULL) {
		n = fwrite(&x, sizeof x, 1, fp);
		r->pos_ += n * sizeof x;
		if (ferror(fp)) {
			fprintf(stderr, "Files.WriteReal failed: %s: %s\n", BaseName(r), strerror(errno));
		}
	}
}


void Files_WriteNum_(Files_Rider_ *r, const OBNC_Td *rTD, int x)
{
	FILE *fp;
	int i;
	char buf[CHAR_BIT * sizeof x]; /* 10^x = 2^n implies x < n */
	size_t n;
	
	assert(r != NULL);
	OBNC_PT(r->base_);
	
	i = 0;
	while ((x < -64) || (x > 63)) {
		assert(i < LEN(buf));
		buf[i] = Mod(x, 128) + 128;
		x = Div(x, 128);
		i++;
	}
	assert(i < LEN(buf));
	buf[i] = Mod(x, 128);
		
	Position(r, &fp);
	if (fp != NULL) {
		n = fwrite(buf, sizeof buf[0], i + 1, fp);
		r->pos_ += n * sizeof buf[0];
		if (ferror(fp)) {
			fprintf(stderr, "Files.WriteNum failed: %s: %s\n", BaseName(r), strerror(errno));
		}

	}
}


void Files_WriteString_(Files_Rider_ *r, const OBNC_Td *rTD, const char s[], int sLen)
{
	FILE *fp;
	size_t n;
	
	assert(r != NULL);
	OBNC_PT(r->base_);
	assert(s != NULL);
	assert(sLen >= 0);
	CheckTermination(s, sLen);
	
	Position(r, &fp);
	if (fp != NULL) {
		n = fwrite(s, sizeof s[0], strlen(s) + 1, fp);
		r->pos_ += n * sizeof s[0];
		if (ferror(fp)) {
			fprintf(stderr, "Files.WriteString failed: %s: %s\n", BaseName(r), strerror(errno));
		}
	}
}


void Files_WriteSet_(Files_Rider_ *r, const OBNC_Td *rTD, unsigned int s)
{
	FILE *fp;
	size_t n;
	
	assert(r != NULL);
	OBNC_PT(r->base_);
	
	Position(r, &fp);
	if (fp != NULL) {
		n = fwrite(&s, sizeof s, 1, fp);
		r->pos_ += n * sizeof s;
		if (ferror(fp)) {
			fprintf(stderr, "Files.WriteSet failed: %s: %s\n", BaseName(r), strerror(errno));
		}
	}
}


void Files_WriteBool_(Files_Rider_ *r, const OBNC_Td *rTD, int b)
{
	FILE *fp;
	int res;
	
	assert(r != NULL);
	OBNC_PT(r->base_);
	
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


void Files_WriteBytes_(Files_Rider_ *r, const OBNC_Td *rTD, unsigned char buf[], int bufLen, int n)
{
	FILE *fp;
	int nWritten;
	
	assert(r != NULL);
	OBNC_PT(r->base_);
	assert(buf != NULL);
	assert(bufLen >= 0);
	assert(n >= 0);
	assert(n <= bufLen);
	
	Position(r, &fp);
	if (fp != NULL) {
		nWritten = fwrite(buf, sizeof buf[0], n, fp);
		r->pos_ += nWritten * sizeof buf[0];
		r->res_ = n - nWritten;
		if (ferror(fp)) {
			fprintf(stderr, "Files.WriteBytes failed: %s: %s\n", BaseName(r), strerror(errno));
		}
	}
}


void Files_Init(void)
{
}
