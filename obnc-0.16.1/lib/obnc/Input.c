/*Copyright (C) 2017, 2018, 2019 Karl Landstrom <karl@miasap.se>

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.*/

#include ".obnc/Input.h"
#include ".obnc/Input0.h"
#include <obnc/OBNC.h>
#include <SDL/SDL.h>
#include <stdio.h>

#define LEN(arr) ((int) (sizeof (arr) / sizeof (arr)[0]))

OBNC_INTEGER Input__TimeUnit_;

static OBNC_INTEGER mouseLimitWidth = OBNC_INT_MAX;
static OBNC_INTEGER mouseLimitHeight = OBNC_INT_MAX;

static int IsNonModKey(const SDL_Event *event)
{
	return (event->type == SDL_KEYDOWN) && (event->key.keysym.unicode != '\0');
}


OBNC_INTEGER Input__Available_(void)
{
	SDL_Event events[10];
	int result, count, i;

	result = 0;
	if (SDL_GetVideoSurface() != NULL) {
		SDL_PumpEvents();
		/*search the event queue for key down events*/
		count = SDL_PeepEvents(events, LEN(events), SDL_PEEKEVENT, SDL_EVENTMASK(SDL_KEYDOWN));
		if (count > 0) {
			for (i = 0; i < count; i++) {
				if (IsNonModKey(&(events[i]))) {
					result++;
				}
			}
		} else if (count == 0) {
			/*remove events from the queue to allow new ones to enter*/
			count = SDL_PeepEvents(events, LEN(events), SDL_GETEVENT, (Uint32) -1);
			for (i = 0; i < count; i++) {
				if (events[i].type == SDL_QUIT) {
					OBNC_Exit(EXIT_SUCCESS);
				}
			}
		} else {
			fprintf(stderr, "Input.Available failed: SDL_PeepEvents: %s\n", SDL_GetError());
		}
	} else {
		fprintf(stderr, "Input.Available failed: No display surface\n");
	}
	return result;
}


void Input__Read_(char *ch)
{
	SDL_Event event;
	int done;

	*ch = '\0';
	if (SDL_GetVideoSurface() != NULL) {
		do {
			done = SDL_WaitEvent(&event);
		} while (! done || ! IsNonModKey(&event));
		if (done) {
			*ch = (char) event.key.keysym.unicode;
		}
	} else {
		fprintf(stderr, "Input.Read failed: No display surface\n");
	}
}


void Input__Mouse_(unsigned OBNC_INTEGER *keys, OBNC_INTEGER *x, OBNC_INTEGER *y)
{
	SDL_Surface *display;
	int x0, y0;
	Uint8 buttons;
	unsigned int leftPressed, middlePressed, rightPressed;

	display = SDL_GetVideoSurface();
	if (display != NULL) {
		SDL_PumpEvents();
		buttons = SDL_GetMouseState(&x0, &y0);
		*x = x0;
		*y = y0;
		*y = display->h - 1 - *y;
		if (*x >= mouseLimitWidth) {
			*x = mouseLimitWidth - 1;
		}
		if (*y >= mouseLimitHeight) {
			*y = mouseLimitHeight - 1;
		}
		leftPressed = (buttons & SDL_BUTTON_LMASK) != 0;
		middlePressed = (buttons & SDL_BUTTON_MMASK) != 0;
		rightPressed = (buttons & SDL_BUTTON_RMASK) != 0;

		*keys = (unsigned OBNC_INTEGER) (leftPressed << 2) | (middlePressed << 1) | rightPressed;
	} else {
		fprintf(stderr, "Input.Mouse failed: No display surface\n");
		OBNC_Exit(EXIT_FAILURE);
	}
}


void Input__SetMouseLimits_(OBNC_INTEGER w, OBNC_INTEGER h)
{
	OBNC_C_ASSERT(w > 0);
	OBNC_C_ASSERT(h > 0);

	mouseLimitWidth = w;
	mouseLimitHeight = h;
}


OBNC_INTEGER Input__Time_(void)
{
	return Input0__Time_();
}


void Input__Init(void)
{
	static int initialized = 0;

	if (! initialized) {
		Input0__Init();
		Input__TimeUnit_ = Input0__TimeUnit_;
		initialized = 1;
	}
}
