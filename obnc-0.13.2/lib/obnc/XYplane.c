/*Copyright 2017, 2018 Karl Landstrom <karl@miasap.se>

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.*/

#include ".obnc/XYplane.h"
#include ".obnc/Input.h"
#include "../../src/Util.h"
#include <obnc/OBNC.h>
#include <SDL/SDL.h> /*SDL 1.2*/
#include <assert.h>
#include <stdio.h>

#define WITHIN_BOUNDS(x, y) (((OBNC_LONGI unsigned int) (x) < (OBNC_LONGI unsigned int) plane->w) && ((OBNC_LONGI unsigned int) (y) < (OBNC_LONGI unsigned int) plane->h))

#define PIXEL_PTR(x, y) (((Uint8 *) plane->pixels) + (plane->h - 1- (y)) * plane->w + (x))

static SDL_Surface *plane;

OBNC_LONGI int XYplane_X_ = 0;
OBNC_LONGI int XYplane_Y_ = 0;
OBNC_LONGI int XYplane_W_ = 0;
OBNC_LONGI int XYplane_H_ = 0;

static void CalculatePlaneSize(int useFullscreen, int *width, int *height)
{
	const SDL_VideoInfo *info;
	int nominalW;

	*width = 0;
	*height = 0;
	info = SDL_GetVideoInfo();
	if (info != NULL) {
		if (useFullscreen) {
			*width = info->current_w;
			*height = info->current_h;
		} else {
			if (info->current_h > info->current_w) {
				nominalW = info->current_w * 4 / 5;
			} else {
				nominalW = info->current_h * 4 * 4 / (5 * 3);
			}
			*width = nominalW - nominalW % 4;
			*height = *width * 3 / 4;
		}
	} else {
		fprintf(stderr, "XYplane.Open failed: cannot get video info\n");
	}
	assert(*width % 4 == 0); /*required for correct rendering*/
}


static void Open(int useFullscreen)
{
	int error, width, height;
	unsigned int flags;
	
	if (plane != NULL) {
		SDL_Quit();
	}
	error = SDL_Init(SDL_INIT_VIDEO);
	if (! error) {
		atexit(SDL_Quit);
		SDL_WM_SetCaption("XYplane - OBNC", NULL);
		SDL_EnableUNICODE(1);
		CalculatePlaneSize(useFullscreen, &width, &height);
		flags = SDL_HWSURFACE | SDL_DOUBLEBUF;
		if (useFullscreen) {
			flags |= SDL_FULLSCREEN;
		}
		plane = SDL_SetVideoMode(width, height, 8, flags);
		if (plane != NULL) {
			XYplane_W_ = plane->w;
			XYplane_H_ = plane->h;
			if (SDL_MUSTLOCK(plane)) {
				error = SDL_LockSurface(plane);
				if (error) {
					fprintf(stderr, "XYplane.Open failed: %s\n", SDL_GetError());
				}
			}
		} else {
			fprintf(stderr, "XYplane.Open failed: %s\n", SDL_GetError());
		}
	} else {
		fprintf(stderr, "XYplane.Open failed: %s\n", SDL_GetError());
	}
}


void XYplane_Open_(void)
{
	Open(0);
}


void XYplane_Clear_(void)
{
	if (plane != NULL) {
		if (SDL_MUSTLOCK(plane)) {
			SDL_LockSurface(plane);
			SDL_FillRect(plane, NULL, 0);
			SDL_UnlockSurface(plane);
		} else {
			SDL_FillRect(plane, NULL, 0);
		}
	}
}


void XYplane_Dot_(OBNC_LONGI int x, OBNC_LONGI int y, OBNC_LONGI int mode)
{
	static unsigned char color[] = {0, 0xff};

	if ((plane != NULL) && WITHIN_BOUNDS(x, y)) {
		*PIXEL_PTR(x, y) = color[mode & 1];
	}
}


int XYplane_IsDot_(OBNC_LONGI int x, OBNC_LONGI int y)
{
	return (plane != NULL) && WITHIN_BOUNDS(x, y) && (*PIXEL_PTR(x, y) == 0xff);
}


char XYplane_Key_(void)
{
	static const int ack = 0x06; /*Ctrl-f*/
	static const int esc = 0x1b;
	char result = 0;
	int fullscreen;
	
	if (plane != NULL) {
		if (SDL_MUSTLOCK(plane)) {
			SDL_LockSurface(plane);
			SDL_Flip(plane);
			SDL_UnlockSurface(plane);
		} else {
			SDL_Flip(plane);
		}
		if (Input_Available_() > 0) {
			Input_Read_(&result);
			fullscreen = (plane->flags & SDL_FULLSCREEN) != 0;
			if (result == ack) {
				Open(! fullscreen);
			} else if ((result == esc) && fullscreen) {
				Open(0);
			}
		}
		SDL_Delay(10);
	}
	return result;
}


void XYplane_Init(void)
{
	/*do nothing*/
}
