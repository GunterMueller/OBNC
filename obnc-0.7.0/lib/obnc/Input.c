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

#include "Input.h"
#include "../../src/Util.h"
#include <SDL/SDL.h>
#include <termios.h> /*POSIX*/
#include <unistd.h> /*POSIX*/
#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>

int Input_TimeUnit_ = (int) CLOCKS_PER_SEC;

static int IsAscii(const SDL_Event *event)
{
	Uint16 inputChar;
	int isAsciiChar, result;

	if (event->type == SDL_KEYDOWN) {
		inputChar = event->key.keysym.unicode;
		isAsciiChar = (inputChar & 0xFF80) == 0;
		result = (inputChar != '\0') && isAsciiChar;
	} else {
		result = 0;
	}
	return result;
}


int Input_Available_(void)
{
	SDL_Event events[10];
	int count, result, i;

	result = 0;
	if (SDL_GetVideoSurface() != NULL) {
		SDL_PumpEvents();
		/*search the event queue for key down events*/
		count = SDL_PeepEvents(events, LEN(events), SDL_PEEKEVENT, SDL_EVENTMASK(SDL_KEYDOWN));
		if (count >= 0) {
			for (i = 0; i < count; i++) {
				if (IsAscii(&(events[i]))) {
					result++;
				}
			}
			if (result == 0) {
				/*remove events from the queue to allow new ones to enter*/
				count = SDL_PeepEvents(events, LEN(events), SDL_GETEVENT, -1);
				for (i = 0; i < count; i++) {
					if (events[i].type == SDL_QUIT) {
						exit(EXIT_SUCCESS);
					}
				}
			}
		} else {
			fprintf(stderr, "SDL_PeepEvents: %s\n", SDL_GetError());
		}
	} else {
		fprintf(stderr, "No display surface\n");
	}
	return result;
}


static void ReadGUI(char *ch)
{
	SDL_Event event;
	int done;

	do {
		done = SDL_WaitEvent(&event);
	} while (! done || ! IsAscii(&event));
	if (done) {
		*ch = event.key.keysym.unicode;
	}
}


static void ReadCLI(char *ch)
{
	struct termios savedState, newState;
	int error, inputChar, isAscii;

	*ch = '\0';
	error = tcgetattr(STDIN_FILENO, &savedState);
	if (! error) {
		newState = savedState;
		newState.c_lflag &= ~(ECHO | ICANON);
		newState.c_cc[VMIN] = 1;
		error = tcsetattr(STDIN_FILENO, TCSANOW, &newState);
		if (! error) {
			do {
				inputChar = getchar();
				isAscii = (inputChar >= 0) && (inputChar < 128);
			} while (! isAscii);
			*ch = inputChar;
			error = tcsetattr(STDIN_FILENO, TCSANOW, &savedState);
			if (error) {
				fprintf(stderr, "tcsetattr: %s\n", strerror(errno));
			}
		} else {
			fprintf(stderr, "tcsetattr: %s\n", strerror(errno));
		}
	} else {
		fprintf(stderr, "tcgetattr: %s\n", strerror(errno));
	}
}


void Input_Read_(char *ch)
{
	if (SDL_GetVideoSurface() != NULL) {
		ReadGUI(ch);
	} else {
		ReadCLI(ch);
	}
}


void Input_Mouse_(unsigned int *keys, int *x, int *y)
{
	SDL_Surface *display;
	Uint8 buttons;
	int leftPressed, middlePressed, rightPressed;

	display = SDL_GetVideoSurface();
	if (display != NULL) {
		SDL_PumpEvents();
		buttons = SDL_GetMouseState(x, y);
		*y = display->h - 1 - *y;

		leftPressed = (buttons & SDL_BUTTON_LMASK) != 0;
		middlePressed = (buttons & SDL_BUTTON_MMASK) != 0;
		rightPressed = (buttons & SDL_BUTTON_RMASK) != 0;

		*keys = (unsigned int) (leftPressed << 2) | (middlePressed << 1) | rightPressed;
	} else {
		fprintf(stderr, "No display surface\n");
		exit(EXIT_FAILURE);
	}
}


void Input_SetMouseLimits_(int w, int h)
{
	fprintf(stderr, "Function Input_SetMouseLimits is not implemented\n");
}


int Input_Time_(void)
{
	return (int) clock();
}


void Input_Init(void)
{
	/*do nothing*/
}
