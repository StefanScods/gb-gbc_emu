/**
 * The implementation of the TileViewerFrame class.
 */
#include "include\tileViewerFrame.h"
#include "include\emulationThread.h"
#include "..\core\include\cpu.h"
#include "..\core\include\core.h"
#include "..\core\include\memory.h"

#include "..\core\include\defines.h"
#include <iostream>
#include <string.h>
#include <math.h>
#include <algorithm>

// Enables debug cout statements for this file.
#define ENABLE_DEBUG_PRINTS false

TileViewerFrame::TileViewerFrame(Core *d_emuCore, EmulationThread *d_emuThread) : wxFrame(NULL, wxID_ANY, TILE_VIEWER_DISPLAY_TITLE, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER)
{
    if (ENABLE_DEBUG_PRINTS)
        std::cout << "Starting: Tile Viewer Frame" << std::endl;

    // Save passed pointers.
    emuCore = d_emuCore;
    emuThread = d_emuThread;

    // Create a sizer to maintain the desired size of the main display.
	wxBoxSizer* mainDisplaySizer  = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* panelSizer  = new wxBoxSizer(wxVERTICAL);

    // Create the two displays.
    wxPanel* mainDisplayPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(TILE_MAP_WIDTH, TILE_MAP_HEIGHT));
    mainDisplaySizer->Add(mainDisplayPanel, 0, wxEXPAND);
    mainDisplaySizer->Add(panelSizer, 1, wxEXPAND | wxLEFT, 20);
    wxPanel* selectedTilePanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(ZOOM_TILE_SIZE, ZOOM_TILE_SIZE));
    panelSizer->Add(selectedTilePanel, 0);

    // Create the selected address text.
    selectedAddressTextElement = new wxStaticText(this, wxID_ANY, "Address:", wxDefaultPosition, wxSize(ZOOM_TILE_SIZE, 20));
    wxFont guiFont = wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);
    selectedAddressTextElement->SetFont(guiFont);
    panelSizer->Add(selectedAddressTextElement, 1, wxEXPAND | wxTOP, 10);

	// Render the sizer.
	SetSizerAndFit(mainDisplaySizer);

    // Perform some display initiation and early renders.
    initializeSDL2(mainDisplayPanel, selectedTilePanel);
}

TileViewerFrame::~TileViewerFrame()
{
    if (ENABLE_DEBUG_PRINTS)
        std::cout << "Exiting: Tile Viewer Frame" << std::endl;
}

// Event Handlers.
void TileViewerFrame::OnCloseWindow(wxCloseEvent &event)
{
    this->Hide();
    // Reset selection.
    int lastSelectedTile = -1;
	bool lastSelectedBank = 0;
    selectedAddressTextElement->SetLabelText("Address:");
}

void TileViewerFrame::handleEmulatorCoreUpdateEvent(wxCommandEvent &event)
{
    // Return early if not active.
    if (!this->IsShown())
        return;
}

void TileViewerFrame::initializeSDL2(wxPanel* mainDisplayPanel, wxPanel* zoomedTilePanel){
    mainTileMapWindow = SDL_CreateWindowFrom((void*)mainDisplayPanel->GetHandle());
	SDL_SetWindowSize(mainTileMapWindow, TILE_MAP_WIDTH, TILE_MAP_HEIGHT);
    mainTileMapRenderer = SDL_CreateRenderer(mainTileMapWindow, -1, SDL_RENDERER_ACCELERATED);
    mainTileMapTexture = SDL_CreateTexture(mainTileMapRenderer, SDL_PIXELFORMAT_BGRA32, SDL_TEXTUREACCESS_STREAMING, TILE_DIMENSION, TILE_DIMENSION);

    zoomedTileWindow = SDL_CreateWindowFrom((void*)zoomedTilePanel->GetHandle());
	SDL_SetWindowSize(zoomedTileWindow,ZOOM_TILE_SIZE, ZOOM_TILE_SIZE);
    zoomedTileMapRenderer = SDL_CreateRenderer(zoomedTileWindow, -1, SDL_RENDERER_ACCELERATED);
    zoomedTileMapTexture = SDL_CreateTexture(zoomedTileMapRenderer, SDL_PIXELFORMAT_BGRA32, SDL_TEXTUREACCESS_STREAMING, TILE_DIMENSION, TILE_DIMENSION);

    // Draw grid in black.
    SDL_SetRenderDrawColor(mainTileMapRenderer, 0x00, 0x00, 0x00, 0xFF);
    // Vertical lines.
    int lineOffset = 0;
    for(int lineX = 0; lineX<WIDTH_TILES+2; lineX++){
        SDL_RenderDrawLine(mainTileMapRenderer, lineOffset, 0, lineOffset, TILE_MAP_HEIGHT);
        lineOffset += (TILE_SIZE + 1);

        // Add a line of three thickness.
        if(lineX == 15 || lineX == 16){
            lineOffset -= TILE_SIZE;
        }
    }
    // Horizontal lines.
    lineOffset = 0;
    for(int lineY = 0; lineY<HEIGHT_TILES; lineY++){
        SDL_RenderDrawLine(mainTileMapRenderer, 0, lineOffset, TILE_MAP_WIDTH, lineOffset);
        lineOffset += (TILE_SIZE+1);
    }
    SDL_RenderPresent(mainTileMapRenderer);

    // Clear the Zoomed in tile.
    SDL_SetRenderDrawColor(zoomedTileMapRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(zoomedTileMapRenderer);
    // Draw a black boarder.
    SDL_SetRenderDrawColor(zoomedTileMapRenderer, 0x00, 0x00, 0x0, 0xFF);
    SDL_RenderDrawLine(zoomedTileMapRenderer, 0, 0, 0, ZOOM_TILE_SIZE);
    SDL_RenderDrawLine(zoomedTileMapRenderer, ZOOM_TILE_SIZE-1, 0, ZOOM_TILE_SIZE-1, ZOOM_TILE_SIZE);
    SDL_RenderDrawLine(zoomedTileMapRenderer, 0, 0, ZOOM_TILE_SIZE, 0);
    SDL_RenderDrawLine(zoomedTileMapRenderer, 0, ZOOM_TILE_SIZE-1, ZOOM_TILE_SIZE, ZOOM_TILE_SIZE-1);
    // Update screen
    SDL_RenderPresent(zoomedTileMapRenderer);
}

void TileViewerFrame::updateSDLDisplays(){
    // Return early if not active.
    if (!this->IsShown())
        return;

    Memory* memory = this->emuCore->getMemory();

    // Generate the mouse position relative to the top of the context of this frame.
    const wxPoint mouseLocation = ScreenToClient(::wxGetMousePosition());
    int selectedTile = -1;
    int selectedBank = -1;
    // Check if the mouse is-bounds.
    if(mouseLocation.x >= 0 && mouseLocation.x < TILE_MAP_WIDTH && mouseLocation.y >= 0 && mouseLocation.y < TILE_MAP_HEIGHT){
 
        int offset = mouseLocation.x >= (TILE_SIZE+1)*HALF_WIDTH + 1 ? -1 : 0;

        // Convert the mouse into a tile index.
        int tileX = (mouseLocation.x + offset) / (TILE_SIZE+1);
        int tileY = (mouseLocation.y) / (TILE_SIZE+1);
        selectedBank = tileX / HALF_WIDTH;
        selectedTile = tileY * HALF_WIDTH + tileX % HALF_WIDTH;
    }

    // Loop over both banks of VRAM.
    for(int bankNumber = 0; bankNumber <= 1; bankNumber++){
        // Loop over the 8 lines of the tile.
        for(int tileIndex = 0; tileIndex < TILES_PER_BANK; tileIndex++){  

            // Determine the coordinates to render at.
            SDL_Rect textureRenderLocation;
            textureRenderLocation.x = (TILE_SIZE+1)*(tileIndex % HALF_WIDTH) + bankNumber * ((TILE_SIZE+1) * HALF_WIDTH + 1) + 1;
            textureRenderLocation.y = (TILE_SIZE+1)*(tileIndex / HALF_WIDTH) + 1;
            textureRenderLocation.w = TILE_SIZE;
            textureRenderLocation.h = TILE_SIZE;

            void* lockedPixels = nullptr;
            int pitch = NULL;
            SDL_LockTexture(mainTileMapTexture, NULL, &lockedPixels, &pitch);
            SDL_memcpy(lockedPixels, memory->getTileWithPalette(tileIndex, bankNumber), INT8_PER_TILE);
            SDL_UnlockTexture(mainTileMapTexture);

            // Render the texture.
            SDL_RenderCopy(mainTileMapRenderer, mainTileMapTexture, NULL, &textureRenderLocation);


            // Render the zoomed in tile.
            if(tileIndex == selectedTile && selectedBank == bankNumber){
                // Determine the coordinates to render at.
                textureRenderLocation.x = 1;
                textureRenderLocation.y = 1;
                textureRenderLocation.w = ZOOM_TILE_SIZE - 2;
                textureRenderLocation.h = ZOOM_TILE_SIZE - 2;

                void* lockedPixels = nullptr;
                int pitch = NULL;
                SDL_LockTexture(zoomedTileMapTexture, NULL, &lockedPixels, &pitch);
                SDL_memcpy(lockedPixels, memory->getTileWithPalette(tileIndex, bankNumber), INT8_PER_TILE);
                SDL_UnlockTexture(zoomedTileMapTexture);

                // Render the texture.
                SDL_RenderCopy(zoomedTileMapRenderer, zoomedTileMapTexture, NULL, &textureRenderLocation);

                // Update the side panel messages.
                convertWordToHexNotation(TILE_DATA_START + tileIndex * BYTES_PER_TILE, selectedAddressText+11);
                selectedAddressText[9] = bankNumber ? '1' : '0';
                selectedAddressTextElement->SetLabelText(selectedAddressText);
            }
        }
    }

    // Update the display.
    SDL_RenderPresent(zoomedTileMapRenderer);
    SDL_RenderPresent(mainTileMapRenderer);
}
