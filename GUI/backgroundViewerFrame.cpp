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
	wxBoxSizer* mainDisplaySizer  = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* bgMapsSizer  = new wxBoxSizer(wxHORIZONTAL);
    statusbarSizer  = new wxBoxSizer(wxHORIZONTAL);

    // Create the main displays and add it to the sizer.
    wxPanel* mainDisplayPanel0 = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(BACKGROUND_MAP_WIDTH, BACKGROUND_MAP_HEIGHT));
    bgMapsSizer->Add(mainDisplayPanel0, 0, wxEXPAND);

    wxPanel* mainDisplayPanel1 = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(BACKGROUND_MAP_WIDTH, BACKGROUND_MAP_HEIGHT));
    bgMapsSizer->Add(mainDisplayPanel1, 0, wxEXPAND | wxLEFT, 3);

    wxFont guiFont = wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);
    // Add text elements.
    selectedAddressTextElement = new wxStaticText(this, wxID_ANY, selectedAddressText, wxDefaultPosition, wxDefaultSize);
    selectedAddressTextElement->SetFont(guiFont);
    statusbarSizer->Add(selectedAddressTextElement, 0, wxEXPAND | wxLEFT, 10);

    selectedMapTextElement = new wxStaticText(this, wxID_ANY, "Current Background Map: 00000", wxDefaultPosition, wxDefaultSize);
    selectedMapTextElement->SetFont(guiFont);
    statusbarSizer->Add(selectedMapTextElement, 0, wxEXPAND | wxLEFT, 25);

    selectedWindowTextElement = new wxStaticText(this, wxID_ANY, "Current Window Map: 00000", wxDefaultPosition, wxDefaultSize);
    selectedWindowTextElement->SetFont(guiFont);
    statusbarSizer->Add(selectedWindowTextElement, 0, wxEXPAND | wxLEFT, 25);
        
    enableWindowTextElement = new wxStaticText(this, wxID_ANY, "Window Enabled: 0000", wxDefaultPosition, wxDefaultSize);
    enableWindowTextElement->SetFont(guiFont);
    statusbarSizer->Add(enableWindowTextElement, 0, wxEXPAND | wxLEFT, 25);

    selectedTileDataTextElement = new wxStaticText(this, wxID_ANY, "Current Tile Data: ", wxDefaultPosition, wxDefaultSize);
    selectedTileDataTextElement->SetFont(guiFont);
    statusbarSizer->Add(selectedTileDataTextElement, 0, wxEXPAND | wxLEFT, 25);

    mainDisplaySizer->Add(bgMapsSizer, 0, wxEXPAND);
    mainDisplaySizer->Add(statusbarSizer, 0, wxEXPAND);
  
	// Render the sizer.
	SetSizerAndFit(mainDisplaySizer);

    // Perform some display initiation and early renders.
    initializeSDL2(mainDisplayPanel0, mainDisplayPanel1);
}

BackgroundViewerFrame::~BackgroundViewerFrame()
{
    if (ENABLE_DEBUG_PRINTS)
        std::cout << "Exiting: Background Map Viewer Frame" << std::endl;

    // Clean SDL2.
    SDL_DestroyTexture(bgMapTexture0);
    bgMapTexture0 = nullptr;
    SDL_DestroyRenderer(bgMapRenderer0);
    bgMapRenderer0 = nullptr;
	SDL_DestroyWindow(bgMapWindow0);
	bgMapWindow0 = nullptr;
    SDL_DestroyTexture(bgMapTexture1);
    bgMapTexture1 = nullptr;
    SDL_DestroyRenderer(bgMapRenderer1);
    bgMapRenderer1 = nullptr;
	SDL_DestroyWindow(bgMapWindow1);
	bgMapWindow1 = nullptr;
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

void BackgroundViewerFrame::initializeSDL2(wxPanel* mainDisplayPanel0, wxPanel* mainDisplayPanel1){
    // Create SDL2 objects.
    bgMapWindow0 = SDL_CreateWindowFrom((void*)mainDisplayPanel0->GetHandle());
	SDL_SetWindowSize(bgMapWindow0, BACKGROUND_MAP_WIDTH, BACKGROUND_MAP_HEIGHT);
    bgMapRenderer0 = SDL_CreateRenderer(bgMapWindow0, -1, SDL_RENDERER_ACCELERATED);
    bgMapTexture0 = SDL_CreateTexture(bgMapRenderer0, SDL_PIXELFORMAT_BGRA32, SDL_TEXTUREACCESS_STREAMING, BG_MAP_WIDTH_PIXELS, BG_MAP_WIDTH_PIXELS);

    bgMapWindow1 = SDL_CreateWindowFrom((void*)mainDisplayPanel1->GetHandle());
	SDL_SetWindowSize(bgMapWindow1, BACKGROUND_MAP_WIDTH, BACKGROUND_MAP_HEIGHT);
    bgMapRenderer1 = SDL_CreateRenderer(bgMapWindow1, -1, SDL_RENDERER_ACCELERATED);
    bgMapTexture1 = SDL_CreateTexture(bgMapRenderer1, SDL_PIXELFORMAT_BGRA32, SDL_TEXTUREACCESS_STREAMING, BG_MAP_WIDTH_PIXELS, BG_MAP_WIDTH_PIXELS);
}

void BackgroundViewerFrame::updateSDLDisplays(){
    // Return early if not active.
    if (!appContext->getRunningEmulationState() || !IsShown())
        return;

    emuCore->acquireMutexLock();
    PPU* ppu = emuCore->getPPU();
    // Render the entire background map.
    ppu->updateBackgroundMap(emuCore->getCGBMode(), 0);
    ppu->updateBackgroundMap(emuCore->getCGBMode(), 1);
    // Copy the entire bg map into a texture.
    void* lockedPixels = nullptr;
    int pitch = NULL;
    SDL_LockTexture(bgMapTexture0, NULL, &lockedPixels, &pitch);
    SDL_memcpy(lockedPixels, ppu->getBackgroundMap(0), INT8_PER_BG_MAP);
    SDL_UnlockTexture(bgMapTexture0);
    SDL_LockTexture(bgMapTexture1, NULL, &lockedPixels, &pitch);
    SDL_memcpy(lockedPixels, ppu->getBackgroundMap(1), INT8_PER_BG_MAP);
    SDL_UnlockTexture(bgMapTexture1);

    // Show the current map which is being rendered.
    std::string mapName = readBit(emuCore->getMemory()->read(0xFF40), 3) ? "BGM 1" : "BGM 0";
    mapName = "Current Background Map: " + mapName;
    selectedMapTextElement->SetLabel(mapName);

    mapName = readBit(emuCore->getMemory()->read(0xFF40), 6) ? "BGM 1" : "BGM 0";
    mapName = "Current Window Map: " + mapName;
    selectedWindowTextElement->SetLabel(mapName);

    mapName = readBit(emuCore->getMemory()->read(0xFF40), 5) ? "True" : "False";
    mapName = "Window Enabled: " + mapName;
    enableWindowTextElement->SetLabel(mapName);

    mapName = readBit(emuCore->getMemory()->read(0xFF40), 4) ? "Tile Set 1" : "Tile Set 0";
    mapName = "Current Tile Data: " + mapName;
    selectedTileDataTextElement->SetLabel(mapName);
    
    // Get the viewport offsets and compute the view regions.
    int offsetY = ppu->getSCY();
    int offsetX = ppu->getSCX();
    emuCore->releaseMutexLock();

    // Generate the mouse position relative to the top of the context of this frame.
    const wxPoint mouseLocation = ScreenToClient(::wxGetMousePosition());
    int selectedTile = -1;
    int selectedBank = 0;

    // Check if the mouse is-bounds.
    if(mouseLocation.x >= 0 && mouseLocation.x < GetSize().GetWidth() && mouseLocation.y >= 0 && mouseLocation.y < GetSize().GetHeight() ){
        int offset = mouseLocation.x >= BACKGROUND_MAP_WIDTH + 1 ? -3 : 0;
        const int TILE_DIMS = (BG_MAP_WIDTH_PIXELS / TILE_DIMENSION);
        // Convert the mouse into a tile index.
        int tileX = (mouseLocation.x + offset) / (TILE_DIMENSION*BACKGROUND_MAP_SCALE);
        int tileY = (mouseLocation.y) / (TILE_DIMENSION*BACKGROUND_MAP_SCALE);

        // Handle the second map.
        if(tileX >= TILE_DIMS){
            selectedBank = 1;
            tileX = tileX % TILE_DIMS;
        }

        // Calculate the hovered tile.
        if(tileY < TILE_DIMS) selectedTile = tileY * TILE_DIMS + tileX;
    }

    // Update the selected address display.
    if(selectedTile != -1){
        word address = (selectedBank ? BGM1_DATA_START : BGM0_DATA_START) + selectedTile;
        convertWordToHexNotation(address, selectedAddressText+9);
        selectedAddressTextElement->SetLabelText(selectedAddressText);
    }

    // Draw rest of the display.
    drawDisplay(bgMapRenderer0, bgMapTexture0, offsetX, offsetY);
    drawDisplay(bgMapRenderer1, bgMapTexture1, offsetX, offsetY);
}

void BackgroundViewerFrame::drawDisplay(SDL_Renderer* bgMapRenderer, SDL_Texture* bgMapTexture, int offsetX, int offsetY){
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