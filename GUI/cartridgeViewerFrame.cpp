/**
 * The implementation of the CartridgeViewerFrame class.
 */
#include "include\cartridgeViewerFrame.h"
#include "include\emulationThread.h"
#include "..\core\include\cpu.h"
#include "..\core\include\core.h"
#include "..\core\include\memory.h"

// Enables debug cout statements for this file.
#define ENABLE_DEBUG_PRINTS false

CartridgeViewerFrame::CartridgeViewerFrame(App *d_appContextCore, Core *d_emuCore, EmulationThread *d_emuThread) : wxFrame(NULL, wxID_ANY,  CARTRIDGE_VIEWER_DISPLAY_TITLE, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER)
{
    if (ENABLE_DEBUG_PRINTS)
        std::cout << "Starting: Cartridge Viewer Frame" << std::endl;

    // Save passed pointers.
    emuCore = d_emuCore;
    emuThread = d_emuThread;
    appContext = d_appContextCore;
        
    SetBackgroundColour(wxColour((unsigned long) 0xFFFFFF));

    // Fonts.
    wxFont categoryLabelFont = wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
    wxFont guiFont = wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);

    // Set up ROM info table.
    romInfoGridSizer = new wxBoxSizer(wxVERTICAL);
    std::string categoryNames[TABLE_ENTRIES] = {
        "File Name: ",
        "ROM Title: ",
        "ROM Version: ",
        "Licensee: ",
        "Cartridge Type: ",
        "ROM Size: ",
        "RAM Size: ",
        "Overseas Only: ",
        "GameBoy Color Only: ",
        "Manufacturer Code: ",
        "Licensee Code: "
    };
    for(int i = 0; i < TABLE_ENTRIES; i++){
        tableEntrySizers[i] = new wxBoxSizer(wxHORIZONTAL);
        tableEntryPanels[i] = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);

        romInfoGridSizer->Add(tableEntryPanels[i]);

        wxStaticText* categoryElement = new wxStaticText(tableEntryPanels[i], wxID_ANY, categoryNames[i], wxDefaultPosition, wxDefaultSize);
        categoryElement->SetFont(categoryLabelFont);

        tableEntries[i] = new wxStaticText(tableEntryPanels[i], wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
        tableEntries[i]->SetFont(guiFont);
      
        tableEntrySizers[i]->Add(categoryElement, 0, wxEXPAND | wxLEFT, 5);
        tableEntrySizers[i]->Add(tableEntries[i], 0, wxEXPAND | wxRIGHT, 10);

        tableEntryPanels[i]->SetSizerAndFit(tableEntrySizers[i]);
    }
    // Hide it.
    changeVisiblityOfSizer(romInfoGridSizer, true);

    // Set up a message if no ROM is loaded.
    noROMSizer = new wxBoxSizer(wxVERTICAL);
    noROMMessage = new wxStaticText(this, wxID_ANY, "No ROM currently loaded!", wxDefaultPosition, wxDefaultSize);
    noROMMessage->SetFont(guiFont);
    noROMSizer->Add(noROMMessage, 0, wxEXPAND | wxALL, 35);
    SetSizerAndFit(noROMSizer);
}

CartridgeViewerFrame::~CartridgeViewerFrame()
{
    if (ENABLE_DEBUG_PRINTS)
        std::cout << "Exiting: Cartridge Viewer Frame" << std::endl;
}

// Event Handlers.
void CartridgeViewerFrame::OnCloseWindow(wxCloseEvent &event)
{
    this->Hide();
}

void CartridgeViewerFrame::handleOnCartridgeLoadedEvent(wxCommandEvent& event){
    if (ENABLE_DEBUG_PRINTS)
        std::cout << "Cartridge Viewer Frame Responding to Cartridge Load" << std::endl;

    Cartridge* cartridge = emuCore->getCartridge();
        
    if(!cartridge->isROMLoaded()){
        changeVisiblityOfSizer(romInfoGridSizer, true);
        noROMMessage->Show(true);
        return;
    } 
    // Update visiblity of elements.
    noROMMessage->Hide();
    changeVisiblityOfSizer(romInfoGridSizer, false);
 
    // Populate the table.
    tableEntries[0]->SetLabel(appContext->getCurrentlyLoadedFilePath());
    tableEntries[1]->SetLabel(cartridge->getROMName());
    tableEntries[2]->SetLabel(cartridge->getVersionNumberText());
    tableEntries[3]->SetLabel(cartridge->getLicensee());
    tableEntries[4]->SetLabel(cartridge->getMemoryControllerName());
    tableEntries[5]->SetLabel(cartridge->getROMSizeText());
    tableEntries[6]->SetLabel(cartridge->getRAMSizeText());
    tableEntries[7]->SetLabel(cartridge->getOverseasOnlyText());
    tableEntries[8]->SetLabel(cartridge->getGameBoyOnlyText());
    tableEntries[9]->SetLabel(cartridge->getManufacturerCode());
    tableEntries[10]->SetLabel(cartridge->getLicenseeCode());

    // Re-gen the layout.
    for(int i = 0; i < TABLE_ENTRIES; i++){
        tableEntrySizers[i]->Layout();
        tableEntryPanels[i]->SetSizerAndFit(tableEntrySizers[i]);
    }
    romInfoGridSizer->Layout();
    SetSizerAndFit(romInfoGridSizer);
}

void CartridgeViewerFrame::changeVisiblityOfSizer(wxSizer* sizer, bool hide){
    for (int i = 0; i < sizer->GetItemCount(); ++i) {
        // Fetch the item.
        wxSizerItem* item = sizer->GetItem(i);
        if (!item || !item->GetWindow()) continue;
        // Change visiblity.
        hide ? item->GetWindow()->Hide() : item->GetWindow()->Show();
    }
}

void CartridgeViewerFrame::handleEmulatorCoreUpdateEvent(wxCommandEvent &event)
{
    // Return early if not active.
    if (!this->IsShown())
        return;

    
}
