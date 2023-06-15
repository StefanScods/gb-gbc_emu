/**
 * The implementation of the BackgroundViewerFrame class.
 */
#include "include\backgroundViewerFrame.h"
#include "include\emulationThread.h"
#include "..\core\include\cpu.h"
#include "..\core\include\core.h"
#include "..\core\include\memory.h"

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
    if (!appContext->getRunningEmulationState() || !this->IsShown())
        return;

    Memory* memory = this->emuCore->getMemory();
    memory->acquireMutexLock();

     void* lockedPixels = nullptr;
    int pitch = NULL;
    SDL_LockTexture(bgMapTexture, NULL, &lockedPixels, &pitch);
    SDL_memcpy(lockedPixels, memory->getBackgroundMap(0), INT8_PER_BG_MAP);
    SDL_UnlockTexture(bgMapTexture);

    SDL_Rect textureRenderLocation;
    textureRenderLocation.x = 0;
    textureRenderLocation.y = 0;
    textureRenderLocation.w = BG_MAP_WIDTH_PIXELS;
    textureRenderLocation.h = BG_MAP_WIDTH_PIXELS;

    // Render the texture.
    SDL_RenderCopy(bgMapRenderer, bgMapTexture, NULL, &textureRenderLocation);

    memory->releaseMutexLock();

    // Update the display.
    SDL_RenderPresent(bgMapRenderer);
}