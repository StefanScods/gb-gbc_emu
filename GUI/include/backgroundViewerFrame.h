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
	 * @param mainDisplayPanel0 The wxWidget panel for the entire tile map 0.
	 * @param mainDisplayPanel0 The wxWidget panel for the entire tile map 1. 
	 */
    void initializeSDL2(wxPanel* mainDisplayPanel0, wxPanel* mainDisplayPanel1);

	/**
	 * @brief The main SDL render loop for this frame.
	 */
    void updateSDLDisplays();

	/**
	 * @brief Helper function which draws on the SDL display.
	 * 
	 * @param bgMapRenderer The renderer to draw with.
	 * @param bgMapTexture The texture to draw on.
	 * @param offsetX The viewport's X offset. 
	 * @param offsetY The viewport's Y offset. 
	*/
	void drawDisplay(SDL_Renderer* bgMapRenderer, SDL_Texture* bgMapTexture, int offsetX, int offsetY);

private:
	wxDECLARE_EVENT_TABLE();

	App *appContext = nullptr;
    Core *emuCore = nullptr;
	EmulationThread* emuThread = nullptr;

	SDL_Window *bgMapWindow0 = nullptr;
	SDL_Texture *bgMapTexture0 = nullptr;
    SDL_Renderer *bgMapRenderer0 = nullptr;

	SDL_Window *bgMapWindow1 = nullptr;
	SDL_Texture *bgMapTexture1 = nullptr;
    SDL_Renderer *bgMapRenderer1 = nullptr;

	char selectedAddressText[16] = "Address: 0x0000";
	wxStaticText* selectedAddressTextElement = nullptr; 
	wxStaticText *selectedMapTextElement = nullptr;
	wxStaticText *selectedWindowTextElement = nullptr;
	wxStaticText *enableWindowTextElement = nullptr;
	wxStaticText *selectedTileDataTextElement = nullptr;
	wxBoxSizer* statusbarSizer = nullptr;



	// Window properties.
	const int BACKGROUND_MAP_SCALE = 2;
	const int BACKGROUND_MAP_WIDTH = BG_MAP_WIDTH_PIXELS * BACKGROUND_MAP_SCALE;
	const int BACKGROUND_MAP_HEIGHT = BG_MAP_WIDTH_PIXELS * BACKGROUND_MAP_SCALE;
};

#endif