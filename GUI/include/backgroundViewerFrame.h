/**
 * The header declaration for the backgroundViewerFrame class.
 */
#ifndef BACKGROUNDVIEWERFRAME_H
#define BACKGROUNDVIEWERFRAME_H

#include <wx/wxprec.h>
#include <SDL.h>
#include "wx/thread.h"
#include "wx/dynarray.h"
#include "wx/numdlg.h"
#include "wx/progdlg.h"
#include "wx/rawbmp.h"
#include "../../core/include/defines.h"

class Core;
class App;
class EmulationThread;

/**
 * The main window GUI for displaying the constructed background loaded within VRAM.
 */
class BackgroundViewerFrame : public wxFrame{
public:
	BackgroundViewerFrame(App *d_appContext, Core *d_emuCore, EmulationThread* d_emuThread);
	~BackgroundViewerFrame();

    /**
	 * @brief The WxWidget's OnCloseWindow Event handler.
	 */
	void OnCloseWindow(wxCloseEvent& event);
    
	/**
	 * @brief The event handler for the `EMULATOR_CORE_UPDATE_EVENT` wxWidget event.
	 */
	void handleEmulatorCoreUpdateEvent(wxCommandEvent& event);

	/**
	 * @brief Sets up the SDL displays need for the event loop. 
	 * 
	 * @param mainDisplayPanel The wxWidget panel for the entire tile map. 
	 */
    void initializeSDL2(wxPanel* mainDisplayPanel);

	/**
	 * @brief The main SDL render loop for this frame.
	 */
    void updateSDLDisplays();

private:
	wxDECLARE_EVENT_TABLE();

	App *appContext = nullptr;
    Core *emuCore = nullptr;
	EmulationThread* emuThread = nullptr;

	SDL_Window *bgMapWindow = nullptr;
	SDL_Texture *bgMapTexture = nullptr;
    SDL_Renderer *bgMapRenderer = nullptr;

	// Window properties.
	const int BACKGROUND_MAP_WIDTH = BG_MAP_WIDTH_PIXELS;
	const int BACKGROUND_MAP_HEIGHT = BG_MAP_WIDTH_PIXELS;
};

#endif