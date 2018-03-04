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

#include ".obnc/XYplane.h"
#include ".obnc/Input.h"
#include "../../src/Util.h"
#include <SDL/SDL.h> /*SDL 1.2*/
#include <assert.h>
#include <stdio.h>

static const Uint8 colorPerMode[2] = {0x0, 0xff};

int XYplane_X_ = 0;
int XYplane_Y_ = 0;
int XYplane_W_ = 0;
int XYplane_H_ = 0;

void XYplane_Open_(void)
{
	SDL_Surface *screen;
	int error;

	screen = SDL_GetVideoSurface();
	if (screen != NULL) {
		SDL_Quit();
	}
	error = SDL_Init(SDL_INIT_VIDEO);
	if (! error) {
		atexit(SDL_Quit);
		SDL_WM_SetCaption("XYplane", NULL);
		SDL_EnableUNICODE(1);
		screen = SDL_SetVideoMode(640, 480, 8, SDL_HWSURFACE | SDL_DOUBLEBUF);
		if (screen != NULL) {
			XYplane_W_ = screen->w;
			XYplane_H_ = screen->h;
		} else {
			fprintf(stderr, "Could not set the video mode: %s\n", SDL_GetError());
		}
	} else {
		fprintf(stderr, "Could not initialize the drawing plane: %s\n", SDL_GetError());
	}
}


void XYplane_Clear_(void)
{
	SDL_Surface *screen;
	int error;

	screen = SDL_GetVideoSurface();
	if (screen != NULL) {
		error = SDL_FillRect(screen, NULL, colorPerMode[XYplane_erase_]);
		if (error) {
			fprintf(stderr, "Could not clear the drawing plane: %s\n", SDL_GetError());
		}
	} else {
		fprintf(stderr, "No drawing plane to clear\n");
	}
}


static int WithinBounds(int x, int y, const SDL_Surface *screen)
{
	return (x >= 0) && (x < screen->w) && (y >= 0) && (y < screen->h);
}


static Uint8 *PixelPtr(int x, int y, const SDL_Surface *screen)
{
	return ((Uint8 *) screen->pixels) + (screen->h - 1- y) * screen->w + x;
}


void XYplane_Dot_(int x, int y, int mode)
{
	SDL_Surface *screen;
	int error;

	assert(mode >= 0);
	assert(mode < LEN(colorPerMode));

	screen = SDL_GetVideoSurface();
	if (screen != NULL) {
		if (WithinBounds(x, y, screen)) {
			if (SDL_MUSTLOCK(screen)) {
				error = SDL_LockSurface(screen);
				if (error) {
					fprintf(stderr, "Could not dot on the plane: %s\n", SDL_GetError());
				}
			}
			*PixelPtr(x, y, screen) = colorPerMode[mode];
			if (SDL_MUSTLOCK(screen)) {
				SDL_UnlockSurface(screen);
			}
		}
	} else {
		fprintf(stderr, "No drawing plane to dot on\n");
	}
}


int XYplane_IsDot_(int x, int y)
{
	SDL_Surface *screen;
	int result, error;

	result = 0;
	screen = SDL_GetVideoSurface();
	if (screen != NULL) {
		if (WithinBounds(x, y, screen)) {
			if (SDL_MUSTLOCK(screen)) {
				error = SDL_LockSurface(screen);
				if (error) {
					fprintf(stderr, "Could not access the drawing plane: %s\n", SDL_GetError());
				}
			}
			result = *(PixelPtr(x, y, screen)) == colorPerMode[XYplane_draw_];
			if (SDL_MUSTLOCK(screen)) {
				SDL_UnlockSurface(screen);
			}
		}
	} else {
		fprintf(stderr, "No drawing plane to inspect\n");
	}
	return result;
}


char XYplane_Key_(void)
{
	char result;
	SDL_Surface *screen;

	result = '\0';
	screen = SDL_GetVideoSurface();
	if (screen != NULL) {
		SDL_Flip(screen);
		if (Input_Available_() > 0) {
			Input_Read_(&result);
		}
		SDL_Delay(10);
	} else {
		fprintf(stderr, "No drawing plane available\n");
	}
	return result;
}


void XYplane_Init(void)
{
	/*do nothing*/
}
