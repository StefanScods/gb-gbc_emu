#ifndef SCREEN_H
#define SCREEN_H
/*
screen class delcaration for a gameboy color emulator

date: 2022-01-06
*/

#include <SDL.h>
#include <string>

#include "..\..\core\include\defines.h"

class Screen {

private:
	//Main window
	SDL_Window* main_window;

	//Event handler
	SDL_Event event;

	//The surface contained by the window
	SDL_Surface* screenSurface = NULL;



	//window title
	std::string title = "red panda's are pretty cool imo ";

	//controls main loop
	bool running = false;

	//controls vSync
	bool enableVsync = false;

	int screen_width = SCREEN_WIDTH;
	int screen_height = SCREEN_HEIGHT;

	//fps vars
	Uint32 frameStart;
	Uint32 frameTime;

public:

	//constuctor -> calls init
	Screen();

	//initalization routine
	bool init();
	//exit routine
	void exit();

	//main screen function ->opens, starts and runs the window
	void mainloop();

	//handles all input events
	void eventHandler();
};


#endif