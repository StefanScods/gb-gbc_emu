/*
screen class implementation for a gameboy color emulator

date: 2022-01-06
*/


#include "include\screen.h"
#include <iostream>

Screen::Screen() {
	//load sdl
	if (!init()) {
		std::cout << "ERROR: Failed to init SLD2: " << SDL_GetError() << std::endl;
	}
	else {
		std::cout << "SLD2 init successful" << std::endl;
	}
}

bool Screen::init() {
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Create window
		main_window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (main_window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Get window surface
			screenSurface = SDL_GetWindowSurface(main_window);
		}



	}


	//Create renderer for window
	gRenderer = SDL_CreateRenderer(main_window, -1, SDL_RENDERER_ACCELERATED);

	running = true;
	return success;
}

void Screen::exit() {
	//Destroy window
	SDL_DestroyWindow(main_window);
	main_window = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

void Screen::eventHandler() {
	//gets the current event state
	while (SDL_PollEvent(&event)) {
		switch (event.type) {

		//QUIT BUTTON
		case(SDL_QUIT):
			running = false;
			break;

		//window events such as resizing 
		case (SDL_WINDOWEVENT):
			switch (event.window.event) {

			case(SDL_WINDOWEVENT_MAXIMIZED):
				break;

			case(SDL_WINDOWEVENT_RESIZED):
				break;

			default:
				break;
			}
			break;

		case(SDL_KEYDOWN):
			break;

		case(SDL_KEYUP):
			break;

		case (SDL_MOUSEMOTION):
			break;

		default:
			break;
		}
	}
}

void Screen::mainloop() {

	double FPS;

	while (running) {

		//start the frame counter 
		frameStart = SDL_GetTicks();

		//!!! clear renderer




		//all input event handler: keyboard, window and mouse 
		eventHandler();

	

		//update display
		SDL_UpdateWindowSurface(main_window);

	

		frameTime = SDL_GetTicks() - frameStart;

		//frame cap
		if (FRAME_DELAY > frameTime) {
			SDL_Delay(FRAME_DELAY - frameTime);
		}

		FPS = 1000.0 / (SDL_GetTicks() - frameStart);
		SDL_SetWindowTitle(main_window, (title + std::to_string(FPS)).c_str()); //display fps on the title bar
	}
}

//std::cout << "Frame Budget:" << (SDL_GetTicks() - frameStart) * 6.0 << "%" << std::endl;