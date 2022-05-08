#ifndef RAINBOW_H
#define RAINBOW_H

#include <SDL.h>

//colours
struct Rainbow {
	SDL_Color white = { 255, 255, 255 };
	SDL_Color black = { 0, 0, 0 };
	SDL_Color red = { 255, 0, 0 };
	SDL_Color green = { 0, 255, 0 };
	SDL_Color blue = { 0, 0, 255 };
};

#endif