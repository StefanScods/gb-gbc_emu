/**
 * The implementation of the wxWidget thread object which holds the emulator
 * core.
*/
#include <functional>
#include "include/emulationThread.h"
#include "../core/include/core.h"
#include "../core/include/joypad.h"
#include "../core/include/ppu.h"

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
	
	sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);
	if (sdlRenderer == NULL) {
		std::cerr << "Failed to create SDL renderer. Error: " << SDL_GetError() << std::endl;
		return false;
	}

	backgroundLayer = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_BGRA32, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (backgroundLayer == NULL) {
		std::cerr << "Failed to create SDL texture. Error: " << SDL_GetError() << std::endl;
		return false;
	}

	windowLayer = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_BGRA32, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (windowLayer == NULL) {
		std::cerr << "Failed to create SDL texture. Error: " << SDL_GetError() << std::endl;
		return false;
	}

	objectLayer = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_BGRA32, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (objectLayer == NULL) {
		std::cerr << "Failed to create SDL texture. Error: " << SDL_GetError() << std::endl;
		return false;
	}

	// Set alpha blend textures.
	SDL_SetRenderDrawBlendMode(sdlRenderer, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(windowLayer, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(objectLayer, SDL_BLENDMODE_BLEND);

	// Initialization was a success.
	return true;
}

void* EmulationThread::Entry()
{
	// Preform initialize routine.
	if(!initializeEmulator()){
		exit(1);
	}

	// Bind important keys.
	emuCore->controller.bindKeyUp(SDL_SCANCODE_HOME, std::bind(&Core::printCurrentState, emuCore));
	
	// Joypad Keys.
	// Up.
	emuCore->controller.bindKeyUp(SDL_SCANCODE_W, std::bind(&Joypad::releaseUp, emuCore->getJoypad()));
	emuCore->controller.bindKeyDown(SDL_SCANCODE_W, std::bind(&Joypad::pressUp, emuCore->getJoypad()));
	// Down.
	emuCore->controller.bindKeyUp(SDL_SCANCODE_S, std::bind(&Joypad::releaseDown, emuCore->getJoypad()));
	emuCore->controller.bindKeyDown(SDL_SCANCODE_S, std::bind(&Joypad::pressDown, emuCore->getJoypad()));
	// Left.
	emuCore->controller.bindKeyUp(SDL_SCANCODE_A, std::bind(&Joypad::releaseLeft, emuCore->getJoypad()));
	emuCore->controller.bindKeyDown(SDL_SCANCODE_A, std::bind(&Joypad::pressLeft, emuCore->getJoypad()));
	// Right.
	emuCore->controller.bindKeyUp(SDL_SCANCODE_D, std::bind(&Joypad::releaseRight, emuCore->getJoypad()));
	emuCore->controller.bindKeyDown(SDL_SCANCODE_D, std::bind(&Joypad::pressRight, emuCore->getJoypad()));
	// A.
	emuCore->controller.bindKeyUp(SDL_SCANCODE_RETURN, std::bind(&Joypad::releaseA, emuCore->getJoypad()));
	emuCore->controller.bindKeyDown(SDL_SCANCODE_RETURN, std::bind(&Joypad::pressA, emuCore->getJoypad()));
	// B.
	emuCore->controller.bindKeyUp(SDL_SCANCODE_RSHIFT, std::bind(&Joypad::releaseB, emuCore->getJoypad()));
	emuCore->controller.bindKeyDown(SDL_SCANCODE_RSHIFT, std::bind(&Joypad::pressB, emuCore->getJoypad()));
	// Start.
	emuCore->controller.bindKeyUp(SDL_SCANCODE_Q, std::bind(&Joypad::releaseSelect, emuCore->getJoypad()));
	emuCore->controller.bindKeyDown(SDL_SCANCODE_Q, std::bind(&Joypad::pressSelect, emuCore->getJoypad()));
	// Select.
	emuCore->controller.bindKeyUp(SDL_SCANCODE_E, std::bind(&Joypad::releaseStart, emuCore->getJoypad()));
	emuCore->controller.bindKeyDown(SDL_SCANCODE_E, std::bind(&Joypad::pressStart, emuCore->getJoypad()));

	// Main event loop.
	while (appContext->getRunningEmulationState())
	{
		// Get the start time of the frame.
		frameStartTime = SDL_GetTicks64();

		// Call the event handler.
		sdlEventHandler();

		// Runs the emulator.
		emuCore->emulatorMain();

		PPU* ppu = emuCore->getPPU();
		// Copy the entire background layer into a texture.
		void* lockedPixels = nullptr;
		int pitch = NULL;
		SDL_LockTexture(backgroundLayer, NULL, &lockedPixels, &pitch);
		SDL_memcpy(lockedPixels, ppu->getVideoBufferBG(), INT8_PER_SCREEN);
		SDL_UnlockTexture(backgroundLayer);

		SDL_LockTexture(windowLayer, NULL, &lockedPixels, &pitch);
		SDL_memcpy(lockedPixels, ppu->getVideoBufferWindow(), INT8_PER_SCREEN);
		SDL_UnlockTexture(windowLayer);

		SDL_LockTexture(objectLayer, NULL, &lockedPixels, &pitch);
		SDL_memcpy(lockedPixels, ppu->getVideoBufferObject(), INT8_PER_SCREEN);
		SDL_UnlockTexture(objectLayer);

		// Clear Screen.
		SDL_SetRenderDrawColor(sdlRenderer, 0xFF, 0x00, 0xFF, 0xFF);
		SDL_RenderClear(sdlRenderer);

		// Render the PPU layer textures.
		SDL_RenderCopy(sdlRenderer, backgroundLayer, NULL, NULL);
		SDL_RenderCopy(sdlRenderer, windowLayer, NULL, NULL);
		SDL_RenderCopy(sdlRenderer, objectLayer, NULL, NULL);

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
