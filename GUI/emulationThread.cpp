/**
 * The implementation of the wxWidget thread object which holds the emulator
 * core.
*/
#include <functional>
#include "include/emulationThread.h"
#include "../core/include/core.h"

// Enables debug cout statements for this file.
#define ENABLE_DEBUG_PRINTS true

EmulationThread::EmulationThread(App *d_appContext, Core *d_emuCore, wxPanel *d_displayPanel) : wxThread(wxTHREAD_JOINABLE)
{
	if (ENABLE_DEBUG_PRINTS)
		std::cout << "Starting: Main Emulation Thread" << std::endl;

	// Save input pointers.
	appContext = d_appContext;
	emuCore = d_emuCore;
	displayPanel = d_displayPanel;
}

EmulationThread::~EmulationThread()
{
	if (ENABLE_DEBUG_PRINTS)
		std::cout << "Exiting: Main Emulation Thread" << std::endl;
}

bool EmulationThread::initializeEmulator(){

	// Create an SDL display from a wxWidget frame panel.
	if(displayPanel == nullptr){
		std::cerr << "WxWidget frame's display panel is a nullptr. Cannot create SDL display." << std::endl;
		return false;
	}

	sdlWindow = SDL_CreateWindowFrom((void*)displayPanel->GetHandle());
	if (sdlWindow == NULL) {
		std::cerr << "Failed to create SDL window from display panel. Error: " << SDL_GetError() << std::endl;
		return false;
	}
	SDL_SetWindowSize(sdlWindow, MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT);
	
	sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED);
	if (sdlRenderer == NULL) {
		std::cerr << "Failed to create SDL renderer. Error: " << SDL_GetError() << std::endl;
		return false;
	}

	// Initialization was a success.
	return true;
}

void* EmulationThread::Entry()
{
	// Preform initialize routine.
	if(!initializeEmulator()){
		exit(1);
	}

	int x = 0;
	int y = 0;

	// Bind important keys.
	emuCore->controller.bindKeyUp(SDL_SCANCODE_HOME, std::bind(&Core::printCurrentState, emuCore));
	emuCore->controller.bindKeyUp(SDL_SCANCODE_PAUSE, std::bind(&Core::toggleEmulatorExecution, emuCore));

	// Step next frame binds to the key "PAGE UP"
	emuCore->controller.bindKeyDown(SDL_SCANCODE_PAGEUP, std::bind(&Core::keyDownStepNextFrameButton, emuCore));
	emuCore->controller.bindKeyUp(SDL_SCANCODE_PAGEUP, std::bind(&Core::keyUpStepNextFrameButton, emuCore));
	// Step next instuction binds to the key "PAGE UP"
	emuCore->controller.bindKeyDown(SDL_SCANCODE_PAGEDOWN, std::bind(&Core::keyDownStepNextInstuctionButton, emuCore));
	emuCore->controller.bindKeyUp(SDL_SCANCODE_PAGEDOWN, std::bind(&Core::keyUpStepNextInstuctionButton, emuCore));

	// Main event loop.
	while (appContext->getRunningEmulationState())
	{
		// Get the start time of the frame.
		frameStartTime = SDL_GetTicks64();

		// Call the event handler.
		sdlEventHandler();

		// Runs the emulator.
		emuCore->emulatorMain();

		x++;
		y++;

		// Clear Screen.
		SDL_SetRenderDrawColor(sdlRenderer, 0xFF, 0x00, 0xFF, 0xFF);
		SDL_RenderClear(sdlRenderer);

		// Draw a red line.
		SDL_SetRenderDrawColor(sdlRenderer, 0xFF, 0x0, 0x0, 0xFF);
		SDL_RenderDrawLine(sdlRenderer, 0, 0, x % MAIN_WINDOW_WIDTH, y % MAIN_WINDOW_HEIGHT);

		// Update screen.
		SDL_RenderPresent(sdlRenderer);

		// Call any other render event.
		for(int callbackIndex = 0; callbackIndex < additionalRenderFunctions.size(); callbackIndex++){
			additionalRenderFunctions[callbackIndex]();
		}

		// Tell rest of the GUI to update.
		appContext->sendEmulationCoreUpdateEvent();

		// Place a cap on the framerate.
		updateAndGetFrameDelta();	
		if(targetFrameTime > frameDelta){
			SDL_Delay(targetFrameTime - frameDelta);
		}

		// Calculate the framerate.
		updateAndGetFrameDelta();
		// Prevent a division by zero error.
		if(frameDelta == 0){
			currentFPS = 0;
			continue;
		}
		currentFPS = ((double) MILLISECOND_FACTOR)/frameDelta;
	};

	// Destroys the SDL window.
	SDL_DestroyRenderer(sdlRenderer);
	SDL_DestroyWindow(sdlWindow);
	sdlRenderer = nullptr;
	sdlWindow = nullptr;



	// Casts success as a wxThread::ExitCode
	return (wxThread::ExitCode)true;
}

void EmulationThread::sdlEventHandler(){
	// Loop over all SDL2 events.
    while( SDL_PollEvent( &sdlEvent ) ){
		switch( sdlEvent.type ){
			// Keyboard events. 
			case SDL_KEYDOWN:
				emuCore->controller.keyDown(sdlEvent);
				break;

			case SDL_KEYUP:
				emuCore->controller.keyUp(sdlEvent);
				break;
			default:
				break;
		}


	}
}

Uint64 EmulationThread::updateAndGetFrameDelta(){
	frameEndTime = SDL_GetTicks64();
	frameDelta = frameEndTime - frameStartTime;
	return frameDelta;
}
