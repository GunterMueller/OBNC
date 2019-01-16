/*Copyright 2017, 2018 Karl Landstrom <karl@miasap.se>

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.*/

#include ".obnc/Input0.h"
#include <obnc/OBNC.h>
#ifdef _WIN32
	#include <conio.h>
	#include <windows.h>
#else
	#include <termios.h> /*POSIX*/
	#include <unistd.h> /*POSIX*/
#endif
#include <fcntl.h> /*POSIX*/
#include <sys/time.h> /*POSIX*/
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>

#define LEN(arr) ((int) (sizeof (arr) / sizeof (arr)[0]))

#ifdef _WIN32

/*----- Windows Implementation*/

OBNC_LONGI int Input0__TimeUnit_ = 1000;

OBNC_LONGI int Input0__Available_(void)
{
	HANDLE inputHandle;
	INPUT_RECORD events[256];
	DWORD eventCount;
	BOOL peeked;
	DWORD result = 0;

	inputHandle = GetStdHandle(STD_INPUT_HANDLE);
	if ((inputHandle != NULL) && (inputHandle != INVALID_HANDLE_VALUE)) {
		peeked = PeekConsoleInput(inputHandle, events, LEN(events), &eventCount);
		if(peeked) {
			result = eventCount;
		} else {
			fprintf(stderr, "Input0.Available failed: %s\n", strerror(errno));
		}
	} else if (inputHandle == INVALID_HANDLE_VALUE) {
		fprintf(stderr, "Input0.Available failed with error code %lu\n", GetLastError());
	} else if (inputHandle == NULL) {
		fprintf(stderr, "Input0.Available failed\n");
	} else {
		assert(0);
	}
	return result;
}


void Input0__Read_(char *ch)
{
	*ch = getch();
}


OBNC_LONGI int Input0__Time_(void)
{
	return GetTickCount();
}

#else

/*----- POSIX Implementation*/

OBNC_LONGI int Input0__TimeUnit_ = (sizeof (OBNC_LONGI int) < 8)? 1000: 1000000000;

/*Keyboard buffer data structure*/

static char queue[256];
static int queueFront, queueCount;

static void Enqueue(char ch)
{
	if (queueCount < LEN(queue)) {
		queue[(queueFront + queueCount) % LEN(queue)] = ch;
		queueCount++;
	}
}


static void Dequeue(char *ch)
{
	assert(queueCount > 0);
	*ch = queue[queueFront];
	queueFront = (queueFront + 1) % LEN(queue);
	queueCount--;
}


OBNC_LONGI int Input0__Available_(void)
{
	int result, error, oldf, status, ch;
	struct termios oldt, newt;

	result = 0;
	error = tcgetattr(STDIN_FILENO, &oldt);
	if (! error) {
		newt = oldt;
		newt.c_lflag &= ~(ICANON | ECHO);
		error = tcsetattr(STDIN_FILENO, TCSANOW, &newt);
		if (! error) {
			oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
			status = fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
			if (status != -1) {
				ch = getchar();
				while (ch != EOF) {
					Enqueue(ch);
					ch = getchar();
				}
				status = fcntl(STDIN_FILENO, F_SETFL, oldf);
				if (status != -1) {
					error = tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
					if (! error) {
						result = queueCount;
					} else {
						fprintf(stderr, "Input0.Available failed: tcsetattr: %s\n", strerror(errno));
					}
				} else {
					fprintf(stderr, "Input0.Available failed: fcntl: %s\n", strerror(errno));
				}
			} else {
				fprintf(stderr, "Input0.Available failed: fcntl: %s\n", strerror(errno));
			}
		} else {
			fprintf(stderr, "Input0.Available failed: tcsetattr: %s\n", strerror(errno));
		}
	} else {
		fprintf(stderr, "Input0.Available failed: tcgetattr: %s\n", strerror(errno));
	}
	return result;
}


void Input0__Read_(char *ch)
{
	struct termios oldt, newt;
	int error, inputChar;

	if (queueCount > 0) {
		Dequeue(ch);
	} else {
		*ch = '\0';
		error = tcgetattr(STDIN_FILENO, &oldt);
		if (! error) {
			newt = oldt;
			newt.c_lflag &= (tcflag_t) ~(ECHO | ICANON);
			newt.c_cc[VMIN] = 1;
			error = tcsetattr(STDIN_FILENO, TCSANOW, &newt);
			if (! error) {
				inputChar = getchar();
				error = tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
				if (! error) {
					if (inputChar != EOF) {
						*ch = (char) inputChar;
					}
				} else {
					fprintf(stderr, "Input0.Read failed: tcsetattr: %s\n", strerror(errno));
				}
			} else {
				fprintf(stderr, "Input0.Read failed: tcsetattr: %s\n", strerror(errno));
			}
		} else {
			fprintf(stderr, "Input0.Read failed: tcgetattr: %s\n", strerror(errno));
		}
	}
}


#ifndef CLOCK_MONOTONIC
#define CLOCK_MONOTONIC 1

static int clock_gettime(int clock_id, struct timespec *result)
{
	struct timeval t;
	int status;

	status = gettimeofday(&t, NULL);
	OBNC_C_ASSERT(status == 0);
	result->tv_sec = t.tv_sec;
	result->tv_nsec = t.tv_usec * 1000;
	return status;
}

#endif

OBNC_LONGI int Input0__Time_(void)
{
	struct timespec now;
	int error;
	OBNC_LONGI int result = -1;

	error = clock_gettime(CLOCK_MONOTONIC, &now);
	if (! error) {
		result = now.tv_sec * Input0__TimeUnit_ + now.tv_nsec / (1000000000 / Input0__TimeUnit_);
	} else {
		fprintf(stderr, "Input0.Time failed: clock_gettime: %s\n", strerror(errno));
	}
	return result;
}

#endif /*defined _WIN32*/

void Input0__Init(void)
{
}
