/**
 * The header declaration for the TileViewerFrame class.
 */
#ifndef TILEVEIWERFRAME_H
#define TILEVEIWERFRAME_H

#include <wx/wxprec.h>
#include "wx/thread.h"
#include "wx/dynarray.h"
#include "wx/numdlg.h"
#include "wx/progdlg.h"
#include "wx/rawbmp.h"
#include <SDL.h>
#include "../../core/include/defines.h"

class Core;
class EmulationThread;

/**
 * The main window GUI for displaying the tile loaded within VRAM.
 */
class TileViewerFrame : public wxFrame{
public:
	TileViewerFrame(Core *d_emuCore, EmulationThread* d_emuThread);
	~TileViewerFrame();

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
	 * @param zoomedTilePanel The wxWidget panel for the focused zoomed tile.
	 */
    void initializeSDL2(wxPanel* mainDisplayPanel, wxPanel* zoomedTilePanel);

	/**
	 * @brief The main SDL render loop for this frame.
	 */
    void updateSDLDisplays();

private:
	wxDECLARE_EVENT_TABLE();

    Core *emuCore = nullptr;
	EmulationThread* emuThread = nullptr;

    SDL_Window* mainTileMapWindow = nullptr;
	SDL_Texture* mainTileMapTexture = nullptr;
    SDL_Renderer* mainTileMapRenderer = nullptr;

	SDL_Window* zoomedTileWindow = nullptr;
	SDL_Texture* zoomedTileTexture = nullptr;
    SDL_Renderer* zoomedTileRenderer = nullptr;

	char selectedAddressText[18] = "Address: 0:0x0000";
	wxStaticText* selectedAddressTextElement = nullptr; 

	int lastSelectedTile = -1;
	bool lastSelectedBank = 0;
	
	// Tile map properties.
	const int TILE_SCALE_FACTOR = 3;
	const int ZOOM_TILE_SCALE_FACTOR = 25;
	const int TILE_SIZE = TILE_DIMENSION * TILE_SCALE_FACTOR;
	const int ZOOM_TILE_SIZE = TILE_DIMENSION * ZOOM_TILE_SCALE_FACTOR + 2;

	const int WIDTH_TILES = 32;
	const int HEIGHT_TILES = 24;
	const int HALF_WIDTH = WIDTH_TILES/2;
	const int TILE_MAP_WIDTH = TILE_SIZE*WIDTH_TILES + WIDTH_TILES + 2;
	const int TILE_MAP_HEIGHT = TILE_SIZE*HEIGHT_TILES + HEIGHT_TILES + 1;
};

#endif