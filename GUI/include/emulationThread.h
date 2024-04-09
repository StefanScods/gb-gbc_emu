/**
 * The header declaration of the wxWidget thread object which holds the emulator
 * core.
 */
#ifndef EMULATIONTHREAD_H
#define EMULATIONTHREAD_H

#include <wx/wxprec.h>
#include "wx/thread.h"
#include "wx/dynarray.h"
#include "wx/numdlg.h"
#include "wx/progdlg.h"
#include <SDL.h>
#include <vector>

#include "../../core/include/defines.h"
#include "../../core/include/sdlController.h"

// Forward declarations.
class App;
class Core;

/**
 * @brief The main thread for running the emulator. This class handles the 
 * running of the emulation core as well as rendering the main display.
 */
class EmulationThread : public wxThread
{
public:
    EmulationThread(App *d_appContext, Core *d_emuCore,  wxPanel* d_displayPanel);
    virtual ~EmulationThread();

    /**
     * @brief Defines the entry point of thread execution.
     */
    virtual void* Entry();

	/**
	 * @brief Initializes the emulation thread.
	 * 
	 * Will output error messages to std::cerr upon failure.
	 * 
	 * @return bool - Returns the success of emulation's initialization.
	 */
    bool initializeEmulator();

	/**
	 * @brief Defines SDL2's event handler. This function can be used
     * to respond to keyboard events.
	 * 
	 * @return void.
	 */
    void sdlEventHandler();

    /**
     * @brief Helper function which updates the current frame delta.
     * Note: This function does not update the `frameStartTime`, only the
     * `frameEndTime` and `frameDelta`.
     * 
     * @return Uint64 - The updated value of `frameDelta`.
     */
    Uint64 updateAndGetFrameDelta();

    // Simple Accessors.
    double getCurrentFPS(){return currentFPS;}

    /**
     * @brief Adds a new rendering event to the event loop.
     * 
     * @param frameRenderCallback The additional render function to call.
     */
    void addAdditionalRenderEvent(voidFuncWithNoArguments frameRenderCallback){
        additionalRenderFunctions.push_back(frameRenderCallback);
    }

private:
    App *appContext = nullptr;
    Core *emuCore = nullptr;
    // GB Display -> Used to generate an SDL window.
    wxPanel* displayPanel = nullptr;
    // SDL2 window vars.
    SDL_Window* sdlWindow = nullptr;
    SDL_Renderer* sdlRenderer = nullptr;
    SDL_Texture* lcdDisplay = nullptr;
    // SDL2 events.
    SDL_Event sdlEvent;

    // FPS calculation vars.
    Uint64 frameStartTime = 0;
    Uint64 frameEndTime = 0;
    Uint64 frameDelta = 0;
    Uint64 targetFrameTime = FRAME_DELAY;
    double currentFPS = 0;

    // An array of additional render functions to call during the event loop.
    std::vector<voidFuncWithNoArguments> additionalRenderFunctions;
};

#endif