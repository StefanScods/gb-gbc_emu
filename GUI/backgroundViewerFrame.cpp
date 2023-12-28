/**
 * The implementation of the BackgroundViewerFrame class.
 */
#include "include\backgroundViewerFrame.h"
#include "include\emulationThread.h"
#include "..\core\include\cpu.h"
#include "..\core\include\core.h"
#include "..\core\include\ppu.h"

// Enables debug cout statements for this file.
#define ENABLE_DEBUG_PRINTS true

BackgroundViewerFrame::BackgroundViewerFrame(App *d_appContext, Core *d_emuCore, EmulationThread *d_emuThread) : wxFrame(NULL, wxID_ANY, BACKGROUND_VIEWER_DISPLAY_TITLE, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER)
{
    if (ENABLE_DEBUG_PRINTS)
        std::cout << "Starting: Background Map Viewer Frame" << std::endl;

    // Save passed pointers.
    appContext = d_appContext;
    emuCore = d_emuCore;
    emuThread = d_emuThread;

    SetBackgroundColour(wxColour(255,255,255));

    // Create a sizer to maintain the desired size of the main display.
	wxBoxSizer* mainDisplaySizer  = new wxBoxSizer(wxHORIZONTAL);

    // Create the main display and add it to the sizer.
    wxPanel* mainDisplayPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(BACKGROUND_MAP_WIDTH, BACKGROUND_MAP_HEIGHT));
    mainDisplaySizer->Add(mainDisplayPanel, 0, wxEXPAND);

	// Render the sizer.
	SetSizerAndFit(mainDisplaySizer);

    // Perform some display initiation and early renders.
    initializeSDL2(mainDisplayPanel);
}

BackgroundViewerFrame::~BackgroundViewerFrame()
{
    if (ENABLE_DEBUG_PRINTS)
        std::cout << "Exiting: Background Map Viewer Frame" << std::endl;

    // Clean SDL2.
    SDL_DestroyTexture(bgMapTexture);
    bgMapTexture = nullptr;
    SDL_DestroyRenderer(bgMapRenderer);
    bgMapRenderer = nullptr;
	SDL_DestroyWindow(bgMapWindow);
	bgMapWindow = nullptr;
}

// Event Handlers.
void BackgroundViewerFrame::OnCloseWindow(wxCloseEvent &event)
{
    this->Hide();
}

void BackgroundViewerFrame::handleEmulatorCoreUpdateEvent(wxCommandEvent &event)
{
    // Return early if not active.
    if (!this->IsShown())
        return;
}

/**
 * @brief Sets up the SDL displays need for the event loop. 
 * 
 * @param mainDisplayPanel The wxWidget panel for the entire tile map. 
 */
void BackgroundViewerFrame::initializeSDL2(wxPanel* mainDisplayPanel){
    // Create SDL2 objects.
    bgMapWindow = SDL_CreateWindowFrom((void*)mainDisplayPanel->GetHandle());
	SDL_SetWindowSize(bgMapWindow, BACKGROUND_MAP_WIDTH, BACKGROUND_MAP_HEIGHT);
    bgMapRenderer = SDL_CreateRenderer(bgMapWindow, -1, SDL_RENDERER_ACCELERATED);
    bgMapTexture = SDL_CreateTexture(bgMapRenderer, SDL_PIXELFORMAT_BGRA32, SDL_TEXTUREACCESS_STREAMING, BG_MAP_WIDTH_PIXELS, BG_MAP_WIDTH_PIXELS);
}

/**
 * @brief The main SDL render loop for this frame.
 */
void BackgroundViewerFrame::updateSDLDisplays(){
    // Return early if not active.
    if (!appContext->getRunningEmulationState() || !IsShown())
        return;

   
    emuCore->acquireMutexLock();
    PPU* ppu = emuCore->getPPU();
    // Render the entire background map.
    ppu->updateBackgroundMap(emuCore->getCGBMode());
    // Copy the entire bg map into a texture.
    void* lockedPixels = nullptr;
    int pitch = NULL;
    SDL_LockTexture(bgMapTexture, NULL, &lockedPixels, &pitch);
    SDL_memcpy(lockedPixels, ppu->getBackgroundMap(), INT8_PER_BG_MAP);
    SDL_UnlockTexture(bgMapTexture);

    // Get the viewport offsets and compute the view regions.
    int offsetY = ppu->getSCY();
    int offsetX = ppu->getSCX();
    emuCore->releaseMutexLock();

    // Get display wrap-around amounts.
    int xOverFlow = (offsetX+SCREEN_WIDTH > BG_MAP_WIDTH_PIXELS) ? 
                        offsetX+SCREEN_WIDTH - BG_MAP_WIDTH_PIXELS : 0;
    int yOverFlow = (offsetY+SCREEN_HEIGHT > BG_MAP_WIDTH_PIXELS) ? 
                        offsetY+SCREEN_HEIGHT - BG_MAP_WIDTH_PIXELS : 0;

    // Render the background map texture.
    SDL_Rect textureRenderLocation;
    textureRenderLocation.x = 0;
    textureRenderLocation.y = 0;
    textureRenderLocation.w = BACKGROUND_MAP_WIDTH;
    textureRenderLocation.h = BACKGROUND_MAP_HEIGHT;
    SDL_RenderCopy(bgMapRenderer, bgMapTexture, NULL, &textureRenderLocation);

    // Draw the tile grid,
    SDL_SetRenderDrawColor(bgMapRenderer, 0, 0, 0, 0xA0);
    for(int i = 1; i < BG_MAP_WIDTH_TILES; i++){
        int offset = i*TILE_DIMENSION*BACKGROUND_MAP_SCALE - 1;
        // Vertical lines.
        SDL_RenderDrawLine(bgMapRenderer, offset, 0, offset, BACKGROUND_MAP_HEIGHT);
        // Horizontal lines.
        SDL_RenderDrawLine(bgMapRenderer, 0, offset, BACKGROUND_MAP_HEIGHT, offset);
    }

    // Draw the viewport with line wrapping.
    SDL_SetRenderDrawColor(bgMapRenderer, 0, 0, 0, 0xE0);
    // Top Line.
    SDL_RenderDrawLine(bgMapRenderer, offsetX*BACKGROUND_MAP_SCALE, offsetY*BACKGROUND_MAP_SCALE, (offsetX+SCREEN_WIDTH)*BACKGROUND_MAP_SCALE,  offsetY*BACKGROUND_MAP_SCALE);
    SDL_RenderDrawLine(bgMapRenderer, offsetX*BACKGROUND_MAP_SCALE, offsetY*BACKGROUND_MAP_SCALE+1, (offsetX+SCREEN_WIDTH)*BACKGROUND_MAP_SCALE,  offsetY*BACKGROUND_MAP_SCALE + 1);
    if(xOverFlow > 0){
        SDL_RenderDrawLine(bgMapRenderer, 0, offsetY*BACKGROUND_MAP_SCALE, xOverFlow*BACKGROUND_MAP_SCALE,  offsetY*BACKGROUND_MAP_SCALE);
        SDL_RenderDrawLine(bgMapRenderer, 0, offsetY*BACKGROUND_MAP_SCALE+1, xOverFlow*BACKGROUND_MAP_SCALE,  offsetY*BACKGROUND_MAP_SCALE + 1);
    }
    // Left Line.
    SDL_RenderDrawLine(bgMapRenderer, offsetX*BACKGROUND_MAP_SCALE, offsetY*BACKGROUND_MAP_SCALE, offsetX*BACKGROUND_MAP_SCALE,  (offsetY+SCREEN_HEIGHT)*BACKGROUND_MAP_SCALE);
    SDL_RenderDrawLine(bgMapRenderer, offsetX*BACKGROUND_MAP_SCALE+1, offsetY*BACKGROUND_MAP_SCALE, offsetX*BACKGROUND_MAP_SCALE+1,  (offsetY+SCREEN_HEIGHT)*BACKGROUND_MAP_SCALE);
    if(yOverFlow > 0){
        SDL_RenderDrawLine(bgMapRenderer, offsetX*BACKGROUND_MAP_SCALE, 0, offsetX*BACKGROUND_MAP_SCALE,  yOverFlow*BACKGROUND_MAP_SCALE);
        SDL_RenderDrawLine(bgMapRenderer, offsetX*BACKGROUND_MAP_SCALE+1, 0, offsetX*BACKGROUND_MAP_SCALE+1,  yOverFlow*BACKGROUND_MAP_SCALE);
    }
    // Bottom Line.
    int yPos = yOverFlow == 0 ? (offsetY+SCREEN_HEIGHT) : yOverFlow;
    SDL_RenderDrawLine(bgMapRenderer, offsetX*BACKGROUND_MAP_SCALE, yPos*BACKGROUND_MAP_SCALE, (offsetX+SCREEN_WIDTH)*BACKGROUND_MAP_SCALE, yPos*BACKGROUND_MAP_SCALE);
    SDL_RenderDrawLine(bgMapRenderer, offsetX*BACKGROUND_MAP_SCALE, yPos*BACKGROUND_MAP_SCALE+1, (offsetX+SCREEN_WIDTH)*BACKGROUND_MAP_SCALE,  yPos*BACKGROUND_MAP_SCALE + 1);
    if(xOverFlow > 0){
        SDL_RenderDrawLine(bgMapRenderer, 0, yPos*BACKGROUND_MAP_SCALE, xOverFlow*BACKGROUND_MAP_SCALE,  yPos*BACKGROUND_MAP_SCALE);
        SDL_RenderDrawLine(bgMapRenderer, 0, yPos*BACKGROUND_MAP_SCALE+1, xOverFlow*BACKGROUND_MAP_SCALE,  yPos*BACKGROUND_MAP_SCALE + 1);
    }
    // Right Line.
    int xPos = xOverFlow == 0 ? (offsetX+SCREEN_WIDTH) : xOverFlow;
    SDL_RenderDrawLine(bgMapRenderer, xPos*BACKGROUND_MAP_SCALE, offsetY*BACKGROUND_MAP_SCALE, xPos*BACKGROUND_MAP_SCALE,  (offsetY+SCREEN_HEIGHT)*BACKGROUND_MAP_SCALE);
    SDL_RenderDrawLine(bgMapRenderer, xPos*BACKGROUND_MAP_SCALE+1, offsetY*BACKGROUND_MAP_SCALE, xPos*BACKGROUND_MAP_SCALE+1,  (offsetY+SCREEN_HEIGHT)*BACKGROUND_MAP_SCALE);
    if(yOverFlow > 0){
        SDL_RenderDrawLine(bgMapRenderer, xPos*BACKGROUND_MAP_SCALE, 0, xPos*BACKGROUND_MAP_SCALE,  yOverFlow*BACKGROUND_MAP_SCALE);
        SDL_RenderDrawLine(bgMapRenderer, xPos*BACKGROUND_MAP_SCALE+1, 0, xPos*BACKGROUND_MAP_SCALE+1,  yOverFlow*BACKGROUND_MAP_SCALE);
    }

    // Update the display.
    SDL_RenderPresent(bgMapRenderer);
}