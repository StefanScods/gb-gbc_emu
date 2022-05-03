/*
screen class implementation for a gameboy color emulator
->the physical application  

date: 2022-01-06
*/


#include "include\screen.h"
#include "..\core\include\cpu.h"
#include <iostream>


bool Screen::init() {
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0){
		std::cout << "ERROR: SDL2 could not initialize!: " << SDL_GetError() << std::endl;
		return false;
	}

	//create windows
	main_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_SHOWN);
	if (main_window == NULL){	
		std::cout << "ERROR: Window could not be created!: " << SDL_GetError() << std::endl;
		return false;
	}	
	mainWindowID = SDL_GetWindowID(main_window);

	cpuStateWindow = SDL_CreateWindow("CPU State", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_HIDDEN);
	if (cpuStateWindow == NULL) {
		std::cout << "ERROR: Window could not be created!: " << SDL_GetError() << std::endl;
		return false;
	}
	cpuStateWindowID = SDL_GetWindowID(cpuStateWindow);

	//Get window surfaces
	mainWindowSurface = SDL_GetWindowSurface(main_window);
	if (mainWindowSurface == NULL){
		std::cout << "ERROR: Window surface could not be created!: " << SDL_GetError() << std::endl;
		return false;
	}

	cpuStateSurface = SDL_GetWindowSurface(cpuStateWindow);
	if (cpuStateSurface == NULL) {
		std::cout << "ERROR: Window surface could not be created!: " << SDL_GetError() << std::endl;
		return false;
	}

	//Create renderer for windows
	mainWindowRenderer = SDL_CreateRenderer(main_window, -1, SDL_RENDERER_ACCELERATED);
	if (mainWindowRenderer == NULL) {
		std::cout << "ERROR: Renderer could not be created!: " << SDL_GetError() << std::endl;;
		return false;
	}
	cpuStateRenderer = SDL_CreateRenderer(cpuStateWindow, -1, SDL_RENDERER_ACCELERATED);
	if (cpuStateRenderer == NULL) {
		std::cout << "ERROR: Renderer could not be created!: " << SDL_GetError() << std::endl;
		return false;
	}

	//loadingFonts 

	//Initialize SDL_ttf
	if (TTF_Init() == -1){
		std::cout << "ERROR: SDL_ttf could not initialize!: " << TTF_GetError() << std::endl;
		return false;
	}

	cpuStateFont = TTF_OpenFont("GUI/resources/fonts/framd.ttf", 32);
	if (cpuStateFont == NULL){
		std::cout << "ERROR: Failed to load ttf!:" <<  TTF_GetError() << std::endl;
		return false;
	}

	running = true;
	return true;
}

void Screen::exit() {

	SDL_FreeSurface(mainWindowSurface);
	SDL_FreeSurface(cpuStateSurface);
	mainWindowSurface = NULL;
	cpuStateSurface = NULL;

	SDL_DestroyWindow(main_window);
	SDL_DestroyWindow(cpuStateWindow);
	main_window = NULL;
	cpuStateWindow = NULL;

	SDL_DestroyRenderer(mainWindowRenderer);
	SDL_DestroyRenderer(cpuStateRenderer);
	mainWindowRenderer = NULL;
	cpuStateRenderer = NULL;

	TTF_CloseFont(cpuStateFont);
	cpuStateFont = NULL;

	//Quit SDL subsystems
	TTF_Quit();
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

			case(SDL_WINDOWEVENT_CLOSE):

				if (event.window.windowID == mainWindowID) {
					running = false;
				}
				else if (event.window.windowID == cpuStateWindowID) {
					SDL_HideWindow(cpuStateWindow);
					showCPUState = false;
				}
				break;

			default:
				break;
			}

		case(SDL_KEYDOWN):

			if (event.key.keysym.sym == showCPUStateScreenKey && showCPUStateButton <= FIRST_PRESSED) {
				showCPUStateButton += 1;
			}

			break;

		case(SDL_KEYUP):

			if (event.key.keysym.sym == showCPUStateScreenKey){
				showCPUStateButton = NOT_PRESSED;
			}

			break;

		case (SDL_MOUSEMOTION):
			break;

		default:
			break;
		}

	}
}

void  Screen::uiLogic() {


	//toggles the cpu state window 
	if (showCPUStateButton == FIRST_PRESSED) {
		showCPUState = !showCPUState;
		showCPUState ? SDL_ShowWindow(cpuStateWindow) : SDL_HideWindow(cpuStateWindow);
		showCPUStateButton = HOLD; //stop the key from toggling again next frame until released 
	}
}


void Screen::renderCpuState(CPU* cpu) {

	//Initialize renderer 
	SDL_RenderClear(cpuStateRenderer);
	SDL_SetRenderDrawColor(cpuStateRenderer, 0xFF, 0xFF, 0xFF, 0xFF);



}

void Screen::mainloop(Uint32& frameStart, CPU* cpu) {

	uiLogic();

	//update display
	SDL_UpdateWindowSurface(main_window);

	if (showCPUState) {
		renderCpuState(cpu);
		SDL_UpdateWindowSurface(cpuStateWindow);
	}

	frameTime = SDL_GetTicks() - frameStart;

	//frame cap
	if (FRAME_DELAY > frameTime) {
		SDL_Delay(FRAME_DELAY - frameTime);
	}

	double FPS = 1000.0 / (SDL_GetTicks() - frameStart);
	SDL_SetWindowTitle(main_window, (title + std::to_string(FPS)).c_str()); //display fps on the title bar


	//debug::loudScream();
}

//std::cout << "Frame Budget:" << (SDL_GetTicks() - frameStart) * 6.0 << "%" << std::endl;