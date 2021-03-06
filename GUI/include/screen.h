#ifndef SCREEN_H
#define SCREEN_H
/*
screen class delcaration for a gameboy color emulator
->the physical application 

date: 2022-01-06
*/

#include <SDL.h>
#include "SDL_ttf.h"
#include <string>

#include "..\..\core\include\cpu.h"
#include "..\..\core\include\defines.h"
#include "rainbow.h"



class Screen {

private:
	//Main window
	SDL_Window* main_window;
	//The surface contained by the window
	SDL_Surface* mainWindowSurface = NULL;
	SDL_Renderer* mainWindowRenderer;
	int mainWindowID;

	//window title
	std::string title = "red panda's are pretty cool imo ";
	
	//size vars for the mainWindow 
	int screen_width = SCREEN_WIDTH;
	int screen_height = SCREEN_HEIGHT;

	//controls vSync
	bool enableVsync = false;

	//Event handler
	SDL_Event event;

	//smaller screen used to display the internal cpu state 
	SDL_Window* cpuStateWindow;
	SDL_Surface* cpuStateSurface = NULL;
	SDL_Renderer* cpuStateRenderer;
	int cpuStateWindowID;

	//cpustate vars to help with rendering 
	unsigned int cpuStateScreenWidth; //determined by font size to best fit the cpu state info 
	unsigned int cpuStateScreenLineHeight;
	unsigned int regValueSpacing;
	word regValues[6];
	char flagValues[4];
	char bufferToRender[7];


	//keycodes
	SDL_Keycode showCPUStateScreenKey = SDLK_F11;
	SDL_Keycode stepCPUKey = SDLK_F1;

	//button logic vars 
	int showCPUStateButton = 0;

	//fps vars
	Uint32 frameTime;

	//fonts
	TTF_Font* cpuStateFontTitle = NULL;
	TTF_Font* cpuStateFontMain = NULL;

	Rainbow colour;

public:

	//controls main loop
	bool running = false;
	bool cpuStepButtonHeld = false;
	bool showCPUState = false;

	uint16_t cpuStepButtonCounter = 0;

	//constuctor -> calls init
	//Screen();

	//initalization routine
	bool init();
	//exit routine
	void exit();

	//main graphics and ui routine 
	void mainloop(Uint32& frameStart, CPU* cpu);

	//runs any logic directly involved in the application interface 
	void uiLogic();

	//populates the cpuStateSurface with graphics 
	void renderCpuState(CPU* cpu);

	//handles all input events
	void eventHandler();
};

//helper function to draw text onto a surface -> returns the width of the rendered text 
 unsigned int drawText(SDL_Surface* surface, const char* text, TTF_Font* font, SDL_Color& colour, int x, int y);

#endif