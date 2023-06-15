/**
 * The implementation of the PaletteViewerFrame class.
 */
#include "include\paletteViewerFrame.h"
#include "include\emulationThread.h"
#include "..\core\include\cpu.h"
#include "..\core\include\core.h"
#include "..\core\include\memory.h"

// Enables debug cout statements for this file.
#define ENABLE_DEBUG_PRINTS false

PaletteViewerFrame::PaletteViewerFrame(Core *d_emuCore, EmulationThread *d_emuThread) : wxFrame(NULL, wxID_ANY, PALETTE_VIEWER_DISPLAY_TITLE, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER)
{
    if (ENABLE_DEBUG_PRINTS)
        std::cout << "Starting: Palette Viewer Frame" << std::endl;

    // Save passed pointers.
    emuCore = d_emuCore;
    emuThread = d_emuThread;

    // Create the root sizer.
    wxFlexGridSizer* rootSizer = new wxFlexGridSizer(2, 0 , 0);
    rootSizer->AddGrowableCol(0, 1);
    rootSizer->AddGrowableCol(1, 1);
    // Create sub-sizers.
    wxGridBagSizer* objectPaletteSizer = new wxGridBagSizer(5, 10);
    wxGridBagSizer* backgroundPaletteSizer = new wxGridBagSizer(5, 10);

    // Create the Root Panels 
    SetBackgroundColour(wxColour((unsigned long) 0xFFFFFF));
    wxPanel* objectPalettesPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    objectPalettesPanel->SetBackgroundColour(wxColour((unsigned long) 0xFFFFFF));
    rootSizer->Add(objectPalettesPanel, 0, wxEXPAND | wxALL, 10);
    wxPanel* backgroundPalettesPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    backgroundPalettesPanel->SetBackgroundColour(wxColour((unsigned long) 0xFFFFFF));
    rootSizer->Add(backgroundPalettesPanel, 0, wxEXPAND | wxALL, 10);

    // Create the content of the frame.
    initSwatchDisplay(
        objectPalettesPanel, objectPaletteSizer, "Object Palettes", "OBP", objectPaletteLabels, objectPaletteSwatches
    );
    initSwatchDisplay(
        backgroundPalettesPanel, backgroundPaletteSizer, "Background Palettes", "BGP", backgroundPaletteLabels, backgroundPaletteSwatches
    );

    // Size the parent frame.
    objectPalettesPanel->SetSizerAndFit(objectPaletteSizer);
    backgroundPalettesPanel->SetSizerAndFit(backgroundPaletteSizer);
    SetSizerAndFit(rootSizer);
}

PaletteViewerFrame::~PaletteViewerFrame()
{
    if (ENABLE_DEBUG_PRINTS)
        std::cout << "Exiting: Palette Viewer Frame" << std::endl;
}

void PaletteViewerFrame::initSwatchDisplay(
    wxPanel* parentPanel, 
    wxGridBagSizer* parentSizer,
    std::string paletteTitle, 
    std::string paletteText, 
    wxStaticText** labelElements,
    wxPanel** swatchElements
){
    // Fonts.
    wxFont categoryLabelFont = wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
    wxFont guiFont = wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);

    // Create the tile section.
    wxStaticText* titleTextElement = new wxStaticText(parentPanel, wxID_ANY, paletteTitle, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
    titleTextElement->SetFont(categoryLabelFont);
    parentSizer->Add(titleTextElement, {0, 0}, {1, 5}, wxEXPAND);
    for(int i = 0; i < SWATCHES_PER_PALETTE; i++){
        wxStaticText* columnLegend = new wxStaticText(parentPanel, wxID_ANY, std::to_string(i), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL | wxALIGN_BOTTOM);
        columnLegend->SetFont(guiFont);
        parentSizer->Add(columnLegend, {1, i + 1}, {1, 1}, wxEXPAND);
    }

    // Create each of the palette rows.
    for(int paletteIndex = 0; paletteIndex < NUMBER_OF_PALETTES; paletteIndex++){
        wxStaticText* paletteLabelText = new wxStaticText(parentPanel, wxID_ANY,  paletteText + std::to_string(paletteIndex) + ":", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
        paletteLabelText->SetFont(guiFont);
        parentSizer->Add(paletteLabelText, {paletteIndex + 2, 0}, {1, 1}, wxEXPAND);
        labelElements[paletteIndex] = paletteLabelText;
        
        // Create panels for each of the 4 colour swatches.
        for(int colourIndex = 0; colourIndex < SWATCHES_PER_PALETTE; colourIndex++){
            wxPanel* paletteSwatchPanel = new wxPanel(parentPanel, wxID_ANY, wxDefaultPosition, wxSize(PALETTE_SWATCH_SIZE, PALETTE_SWATCH_SIZE), wxSIMPLE_BORDER);
            paletteSwatchPanel->SetBackgroundColour(wxColour(0xFFFFFF));
            parentSizer->Add(paletteSwatchPanel, {paletteIndex + 2, colourIndex + 1}, {1, 1}, wxEXPAND);
            swatchElements[paletteIndex * SWATCHES_PER_PALETTE + colourIndex] = paletteSwatchPanel;
        }
    }
}

// Event Handlers.
void PaletteViewerFrame::OnCloseWindow(wxCloseEvent &event)
{
    this->Hide();
}

void PaletteViewerFrame::handleOnCartridgeLoadedEvent(wxCommandEvent& event){
    if(!emuCore->getCGBMode()){
        // Hide irrelevant palettes.
        for(int paletteIndex = NUMBER_OF_OBJECT_PALETTES_NON_COLOR; paletteIndex < NUMBER_OF_PALETTES;  paletteIndex++){
            objectPaletteLabels[paletteIndex]->Hide();
            for(int colourIndex = 0; colourIndex < SWATCHES_PER_PALETTE; colourIndex++){
                objectPaletteSwatches[paletteIndex * SWATCHES_PER_PALETTE + colourIndex]->Hide();
            }
        }
        for(int paletteIndex = NUMBER_OF_BACKGROUND_PALETTES_NON_COLOR; paletteIndex < NUMBER_OF_PALETTES;  paletteIndex++){
            backgroundPaletteLabels[paletteIndex]->Hide();
            for(int colourIndex = 0; colourIndex < SWATCHES_PER_PALETTE; colourIndex++){
                backgroundPaletteSwatches[paletteIndex * SWATCHES_PER_PALETTE + colourIndex]->Hide();
            }
        }
    }
    else{
        // Show all palettes.
        for(int paletteIndex = 0; paletteIndex < NUMBER_OF_PALETTES;  paletteIndex++){
            objectPaletteLabels[paletteIndex]->Show();
            for(int colourIndex = 0; colourIndex < SWATCHES_PER_PALETTE; colourIndex++){
                objectPaletteSwatches[paletteIndex * SWATCHES_PER_PALETTE + colourIndex]->Show();
            }
        }
        for(int paletteIndex = 0; paletteIndex < NUMBER_OF_PALETTES;  paletteIndex++){
            backgroundPaletteLabels[paletteIndex]->Show();
            for(int colourIndex = 0; colourIndex < SWATCHES_PER_PALETTE; colourIndex++){
                backgroundPaletteSwatches[paletteIndex * SWATCHES_PER_PALETTE + colourIndex]->Show();
            }
        }
    }
}

void PaletteViewerFrame::handleEmulatorCoreUpdateEvent(wxCommandEvent &event)
{
    // Return early if not active.
    if (!this->IsShown())
        return;

    Memory* memory = emuCore->getMemory();
    memory->acquireMutexLock();

    // Background Palettes.
    for(int paletteIndex = 0; paletteIndex < NUMBER_OF_PALETTES; paletteIndex++){
        // Skip hidden palettes.
        if(!backgroundPaletteSwatches[paletteIndex*SWATCHES_PER_PALETTE]->IsShown()) continue;

        // Update the palette
        byte* startOfPalette =  memory->getPaletteColour(false, paletteIndex);
        for(int swatchIndex = 0; swatchIndex < SWATCHES_PER_PALETTE; swatchIndex++){    
            backgroundPaletteSwatches[paletteIndex*SWATCHES_PER_PALETTE + swatchIndex]->SetBackgroundColour(
                wxColour(*startOfPalette, *(startOfPalette+1),  *(startOfPalette+2)));
            backgroundPaletteSwatches[paletteIndex*SWATCHES_PER_PALETTE + swatchIndex]->Refresh();
            startOfPalette+=4;
        }
    }

    // Object Palettes.
    for(int paletteIndex = 0; paletteIndex < NUMBER_OF_PALETTES; paletteIndex++){
        // Skip hidden palettes.
        if(!objectPaletteSwatches[paletteIndex*SWATCHES_PER_PALETTE]->IsShown()) continue;

        // Update the palette
        byte* startOfPalette =  memory->getPaletteColour(true, paletteIndex) + 4;
        // Skip the first of the object swatches as it is transparent.
        for(int swatchIndex = 1; swatchIndex < SWATCHES_PER_PALETTE; swatchIndex++){    
            objectPaletteSwatches[paletteIndex*SWATCHES_PER_PALETTE + swatchIndex]->SetBackgroundColour(
                wxColour(*startOfPalette , *(startOfPalette+1),  *(startOfPalette+2)));
            objectPaletteSwatches[paletteIndex*SWATCHES_PER_PALETTE + swatchIndex]->Refresh();
            startOfPalette+=4;
        }
    }

    memory->releaseMutexLock();
}

