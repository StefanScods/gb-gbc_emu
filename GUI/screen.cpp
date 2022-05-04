/*
screen class implementation for a gameboy color emulator
->the physical application  

date: 2022-01-06
*/


#include "include\screen.h"
#include "..\core\include\cpu.h"
#include <iostream>



/*!!! how to directly edit surface pixels 
SDL_LockSurface(cpuStateSurface);
Uint8 bytesperPixel = cpuStateSurface->format->BytesPerPixel;
Uint8* pixelArray = (Uint8*) cpuStateSurface->pixels;
int lenOfRow = cpuStateSurface->pitch;
std::cout << (int)bytesperPixel << std::endl;
for (int x = 0; x < 100;x++) {
	for (int y = 0; y < 100; y++) {
		pixelArray[y * lenOfRow + x * bytesperPixel] = 255; //BRGA blue is +0 -> alpha is +3
	}
}
SDL_UnlockSurface(cpuStateSurface);
*/


//!!! todo maybe upgrade to a hardware text rendering -> sdl_screens are purely cpu rendering 
void drawText(SDL_Surface* surface, const char* text, TTF_Font* font, SDL_Color& colour, int x, int y) {

	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, colour); //renders the text

	//specifies the position on the desination surface 
	SDL_Rect destLocation;
	destLocation.x = x;
	destLocation.y = y;
	destLocation.w = textSurface->w;
	destLocation.h = textSurface->h;

	//draws the text to the surface 
	SDL_BlitSurface(textSurface, NULL, surface, &destLocation);
	SDL_FreeSurface(textSurface);
}

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

	//get hardware renderers
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

	SDL_DestroyWindow(main_window);
	SDL_DestroyWindow(cpuStateWindow);
	main_window = NULL;
	mainWindowSurface = NULL;
	cpuStateWindow = NULL;
	cpuStateSurface = NULL;

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

	for (int i = 0; i < 100; i++) {
		drawText(cpuStateSurface, "hello world", cpuStateFont, white, (i*31)%SCREEN_WIDTH, (i * 37) % SCREEN_HEIGHT);
	}

}

void Screen::mainloop(Uint32& frameStart, CPU* cpu) {

	//main screen
	SDL_FillRect(mainWindowSurface, NULL, 0); //clears the surface
	uiLogic();
	SDL_UpdateWindowSurface(main_window); //update display

	//cpu state window 
	if (showCPUState) {
		SDL_FillRect(cpuStateSurface, NULL, 0); //clears the surface
		renderCpuState(cpu);
		SDL_UpdateWindowSurface(cpuStateWindow); //update display
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