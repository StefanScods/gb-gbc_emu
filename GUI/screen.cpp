/*
screen class implementation for a gameboy color emulator
->the physical application  

date: 2022-01-06
*/


#include "include\screen.h"
#include "..\core\include\cpu.h"
#include <iostream>
//#include "include\rainbow.h"



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
unsigned int drawText(SDL_Surface* surface, const char* text, TTF_Font* font, SDL_Color& colour, int x, int y) {

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

	return destLocation.w;
}

bool Screen::init() {
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0){
		std::cout << "ERROR: SDL2 could not initialize!: " << SDL_GetError() << std::endl;
		return false;
	}

	//loadingFonts 
	//Initialize SDL_ttf
	if (TTF_Init() == -1) {
		std::cout << "ERROR: SDL_ttf could not initialize!: " << TTF_GetError() << std::endl;
		return false;
	}

	cpuStateFontTitle = TTF_OpenFont("GUI/resources/fonts/PressStart2P-Regular.ttf", FONT_SIZE);
	if (cpuStateFontTitle == NULL) {
		std::cout << "ERROR: Failed to load ttf!:" << TTF_GetError() << std::endl;
		return false;
	}
	cpuStateFontMain = TTF_OpenFont("GUI/resources/fonts/PressStart2P-Regular.ttf", FONT_SIZE-4);
	if (cpuStateFontMain == NULL) {
		std::cout << "ERROR: Failed to load ttf!:" << TTF_GetError() << std::endl;
		return false;
	}

	//create windows

	//main screen
	main_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_SHOWN);
	if (main_window == NULL){	
		std::cout << "ERROR: Window could not be created!: " << SDL_GetError() << std::endl;
		return false;
	}	
	mainWindowID = SDL_GetWindowID(main_window);

	//cpu state screen
	//uses key words as reference to size the screen and the offsets required when rendering the text 
	SDL_Surface* textSurface = TTF_RenderText_Solid(cpuStateFontTitle, "Registers", colour.black);
	cpuStateScreenWidth = textSurface->w+CPUSTATE_WIDTH_OFFSET;
	cpuStateScreenLineHeight = textSurface->h;
	SDL_FreeSurface(textSurface);
	textSurface = TTF_RenderText_Solid(cpuStateFontMain, "0x0000", colour.black);
	regValueSpacing = cpuStateScreenWidth - 5 - textSurface->w;
	SDL_FreeSurface(textSurface);
	
	//creates the window for the cpu state screen 
	cpuStateWindow = SDL_CreateWindow("CPU State", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, cpuStateScreenWidth, cpuStateScreenLineHeight*13, SDL_WINDOW_HIDDEN);
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

	TTF_CloseFont(cpuStateFontTitle);
	TTF_CloseFont(cpuStateFontMain);

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


//!!! todo maybe add code to only update the surfaces if the cpu has updated since last render 
void Screen::renderCpuState(CPU* cpu) {

	//get data to display 
	cpu->populateCpuStateBuffer(regValues, flagValues);
	//primes the rendering buffer 
	bufferToRender[0] = '0';
	bufferToRender[1] = 'x';
	bufferToRender[6] = '\0';

	//set up the hardware renderer 
	SDL_SetRenderDrawColor(cpuStateRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(cpuStateRenderer);

	SDL_FillRect(cpuStateSurface, NULL, SDL_MapRGB(cpuStateSurface->format, 255, 255, 255)); //clears the surface

	unsigned int verticalOffset = 5;
	unsigned int temp1, temp2, temp3, flagSpacing;

	//renders register related info 
	drawText(cpuStateSurface, "Registers", cpuStateFontTitle, colour.black, 5, verticalOffset);
	verticalOffset += cpuStateScreenLineHeight;
	temp1 = verticalOffset+1;
	verticalOffset += 10;

	drawText(cpuStateSurface, "AF:", cpuStateFontMain, colour.black, 5, verticalOffset);
	displayHex(regValues[0], bufferToRender + 2);
	drawText(cpuStateSurface, bufferToRender, cpuStateFontMain, colour.black, regValueSpacing, verticalOffset);
	verticalOffset += cpuStateScreenLineHeight;

	drawText(cpuStateSurface, "BC:", cpuStateFontMain, colour.black, 5, verticalOffset);
	displayHex(regValues[1], bufferToRender + 2);
	drawText(cpuStateSurface, bufferToRender, cpuStateFontMain, colour.black, regValueSpacing, verticalOffset);
	verticalOffset += cpuStateScreenLineHeight;

	drawText(cpuStateSurface, "DE:", cpuStateFontMain, colour.black, 5, verticalOffset);
	displayHex(regValues[2], bufferToRender + 2);
	drawText(cpuStateSurface, bufferToRender, cpuStateFontMain, colour.black, regValueSpacing, verticalOffset);
	verticalOffset += cpuStateScreenLineHeight;

	drawText(cpuStateSurface, "HL:", cpuStateFontMain, colour.black, 5, verticalOffset);
	displayHex(regValues[3], bufferToRender + 2);
	drawText(cpuStateSurface, bufferToRender, cpuStateFontMain, colour.black, regValueSpacing, verticalOffset);
	verticalOffset += cpuStateScreenLineHeight*2;

	drawText(cpuStateSurface, "SP:", cpuStateFontMain, colour.black, 5, verticalOffset);
	displayHex(regValues[4], bufferToRender + 2);
	drawText(cpuStateSurface, bufferToRender, cpuStateFontMain, colour.black, regValueSpacing, verticalOffset);
	verticalOffset += cpuStateScreenLineHeight;

	drawText(cpuStateSurface, "HL:", cpuStateFontMain, colour.black, 5, verticalOffset);
	displayHex(regValues[5], bufferToRender + 2);
	drawText(cpuStateSurface, bufferToRender, cpuStateFontMain, colour.black, regValueSpacing, verticalOffset);
	verticalOffset += cpuStateScreenLineHeight*2;

	//renders flag related info 
	temp3 = drawText(cpuStateSurface, "Flags", cpuStateFontTitle, colour.black, 5, verticalOffset);
	verticalOffset += cpuStateScreenLineHeight;
	temp2 = verticalOffset+1;
	verticalOffset += 10;

	flagSpacing = drawText(cpuStateSurface, "Z", cpuStateFontMain, colour.black, 5, verticalOffset);
	flagSpacing = (cpuStateScreenWidth - 10 - flagSpacing)/3; //divide by 3 to generate horizontal offsets for the flag text 

	drawText(cpuStateSurface, "N", cpuStateFontMain, colour.black, 5 + flagSpacing, verticalOffset);
	drawText(cpuStateSurface, "H", cpuStateFontMain, colour.black, 5 + flagSpacing*2, verticalOffset);
	drawText(cpuStateSurface, "C", cpuStateFontMain, colour.black, 5 + flagSpacing*3, verticalOffset);
	verticalOffset += cpuStateScreenLineHeight;

	bufferToRender[1] = '\0';
	bufferToRender[0] = flagValues[0];
	drawText(cpuStateSurface, bufferToRender, cpuStateFontMain, colour.black, 5, verticalOffset);
	bufferToRender[0] = flagValues[1];
	drawText(cpuStateSurface, bufferToRender, cpuStateFontMain, colour.black, 5 + flagSpacing, verticalOffset);
	bufferToRender[0] = flagValues[2];
	drawText(cpuStateSurface, bufferToRender, cpuStateFontMain, colour.black, 5 + flagSpacing*2, verticalOffset);
	bufferToRender[0] = flagValues[3];
	drawText(cpuStateSurface, bufferToRender, cpuStateFontMain, colour.black, 5 + flagSpacing*3, verticalOffset);

	//Render screen surface 
	SDL_Texture* texture = SDL_CreateTextureFromSurface(cpuStateRenderer, cpuStateSurface);
	SDL_RenderCopy(cpuStateRenderer, texture, NULL, NULL);

	//draw some lines using the hardware renderer
	SDL_RenderDrawLine(cpuStateRenderer, 5, temp1, cpuStateScreenWidth - CPUSTATE_WIDTH_OFFSET, temp1);
	SDL_RenderDrawLine(cpuStateRenderer, 5, temp1+1, cpuStateScreenWidth - CPUSTATE_WIDTH_OFFSET, temp1+1);
	SDL_RenderDrawLine(cpuStateRenderer, 5, temp2, temp3+5, temp2);
	SDL_RenderDrawLine(cpuStateRenderer, 5, temp2+1, temp3+5, temp2+1);

	//update display
	SDL_RenderPresent(cpuStateRenderer);

	//cleanup
	SDL_DestroyTexture(texture);
}

void Screen::mainloop(Uint32& frameStart, CPU* cpu) {

	//main screen
	SDL_FillRect(mainWindowSurface, NULL, 0); //clears the surface
	uiLogic();
	SDL_UpdateWindowSurface(main_window); //update display

	//cpu state window 
	if (showCPUState) {
		renderCpuState(cpu);
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