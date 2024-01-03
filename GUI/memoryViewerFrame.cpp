/**
 * The implementation of the MemoryViewerFrame class.
 */
#include "include\memoryViewerFrame.h"
#include "include\emulationThread.h"
#include "..\core\include\cpu.h"
#include "..\core\include\core.h"
#include "..\core\include\defines.h"
#include <iostream>
#include <string.h>
#include <math.h>
#include <algorithm>

// Enables debug cout statements for this file.
#define ENABLE_DEBUG_PRINTS false

MemoryViewerFrame::MemoryViewerFrame(Core *d_emuCore, EmulationThread *d_emuThread) : wxFrame(NULL, wxID_ANY, MEMORY_VIEWER_DISPLAY_TITLE, wxDefaultPosition, wxSize(MEMORY_VIEWER_DISPLAY_WIDTH, MEMORY_VIEWER_DISPLAY_HEIGHT), wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER)
{
    if (ENABLE_DEBUG_PRINTS)
        std::cout << "Starting: Memory Viewer Frame" << std::endl;

    // Save passed pointers.
    emuCore = d_emuCore;
    emuThread = d_emuThread;

    // Font and style constants.
    SetBackgroundColour(wxColour(255,255,255));
    wxFont guiFont = wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);
    wxFont legendFont = wxFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
    wxFont valueFont = wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);

    // Create the parent sizer object.
    wxBoxSizer* parentSizer = new wxBoxSizer(wxVERTICAL);

    // Create top level panels and add them to the sizer.
    topControlPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    parentSizer->Add(topControlPanel, 0, wxEXPAND | wxTOP | wxBOTTOM, 5);
    memoryViewPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    parentSizer->Add(memoryViewPanel, 0, wxEXPAND | wxTOP, 2);
    bottomControlPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    parentSizer->Add(bottomControlPanel, 0, wxEXPAND | wxTOP | wxBOTTOM, 5);

    // Create the top control panel.
    wxBoxSizer* topControlPanelSizer  = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* searchPanelSizer  = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* displayTypeSizer  = new wxBoxSizer(wxHORIZONTAL);

    // Memory Search.
    wxStaticText* label = new wxStaticText(topControlPanel, wxID_ANY, "Search Memory Address:");
    label->SetFont(guiFont);
    searchPanelSizer->Add(label, 0, wxEXPAND | wxTOP, 3);
    wxSearchCtrl* memorySearchBar = new wxSearchCtrl(topControlPanel, wxMenuIDs::MEMORY_MAP_SEARCHBAR, wxEmptyString, wxDefaultPosition, wxDefaultSize);
    memorySearchBar->SetFont(valueFont);
    searchPanelSizer->Add(memorySearchBar, 0, wxEXPAND | wxLEFT, 5);

    // Display notation drop down.
    label = new wxStaticText(topControlPanel, wxID_ANY, "Selected Notation:");
    label->SetFont(guiFont);
    displayTypeSizer->Add(label, 0, wxEXPAND | wxTOP, 3);
    selectedNotationDropDown = new wxComboBox(topControlPanel, wxID_ANY, NOTATION_CHOICES[0], wxDefaultPosition, wxDefaultSize, 3, NOTATION_CHOICES);
    selectedNotationDropDown->SetFont(valueFont);
    selectedNotationDropDown->SelectNone();
    displayTypeSizer->Add(selectedNotationDropDown, 0, wxLEFT, 5);

    topControlPanelSizer->Add(searchPanelSizer, 1, wxALIGN_CENTER | wxLEFT, 5);
    topControlPanelSizer->Add(displayTypeSizer, 1, wxALIGN_CENTER | wxLEFT, 20);
    topControlPanel->SetSizer(topControlPanelSizer);

    // Create the main memory viewer grid.
    memoryViewGrid = new wxGrid( memoryViewPanel,
                -1,
                wxPoint( 0, 0 ),
                wxSize( MEMORY_VIEWER_DISPLAY_WIDTH, MEMORY_VIEWER_DISPLAY_HEIGHT/5*4) );
    
    // Calculate grid dimensions.
    const int RIGHT_MARGIN = 17;
    const int COLUMN_LEGEND_WIDTH = (MEMORY_VIEWER_DISPLAY_WIDTH - RIGHT_MARGIN)/(MEMORY_VIEW_GRID_COLS+1) + 50;
    const int COLUMN_WIDTH = (MEMORY_VIEWER_DISPLAY_WIDTH - COLUMN_LEGEND_WIDTH - RIGHT_MARGIN) /(MEMORY_VIEW_GRID_COLS);
    const int ROW_HEIGHT = 20;

    // Create the grid. 
    memoryViewGrid->CreateGrid(MEMORY_VIEW_GRID_ROWS, MEMORY_VIEW_GRID_COLS );
    // Make the grid read-only.
    memoryViewGrid->EnableDragGridSize(false);
    memoryViewGrid->EnableDragRowSize(false);
    memoryViewGrid->EnableDragColSize(false);
    memoryViewGrid->EnableEditing(false);

    memoryViewGrid->EnableGridLines(false);

    // Set the grid's legend style and size.
    memoryViewGrid->SetLabelFont(legendFont);
    memoryViewGrid->SetRowLabelSize(COLUMN_LEGEND_WIDTH);
    memoryViewGrid->SetColLabelSize(ROW_HEIGHT);
    char columnLegendLabel[4] = "0x0";
    char rowLegendLabel[7] = "0x0000";

    // Grid cells style/size and populate the grid.
    for (int colNum = 0; colNum < MEMORY_VIEW_GRID_COLS; colNum++){
        // Set the column size.
        memoryViewGrid->SetColSize(colNum, COLUMN_WIDTH);
        // Set the column legend to the hex value it represents.
        columnLegendLabel[2] = toHex[colNum % 16];
        memoryViewGrid->SetColLabelValue(colNum, columnLegendLabel);
        for (int rowNum = 0; rowNum < MEMORY_VIEW_GRID_ROWS; rowNum++){
            // On the first iteration ...
            if(!colNum){
                // Set the row size.
                memoryViewGrid->SetRowSize(rowNum, ROW_HEIGHT);
                // Set row style.
                memoryViewGrid->SetRowLabelAlignment(wxALIGN_RIGHT, wxALIGN_CENTRE);
                // Set the row legend to the memory address it represents.
                convertWordToHexNotation(rowNum*MEMORY_VIEW_GRID_COLS, rowLegendLabel);
                memoryViewGrid->SetRowLabelValue(rowNum, rowLegendLabel);
            }
            word address = rowNum * MEMORY_VIEW_GRID_COLS + colNum;
            // Set placeholder data.
            memoryViewGrid->SetCellValue(rowNum, colNum, "00");
            // Cell style.
            memoryViewGrid->SetCellFont(rowNum, colNum,valueFont);
            memoryViewGrid->SetCellAlignment(rowNum, colNum, wxALIGN_CENTRE, wxALIGN_CENTRE);
            memoryViewGrid->SetCellBackgroundColour(rowNum, colNum, getMemoryViewerGridCellColour(address));
        }
    }

    // Create the bottom control panel.
    wxBoxSizer* bottomControlPanelSizer = new wxBoxSizer(wxHORIZONTAL);
    bottomControlPanel->SetSizer(bottomControlPanelSizer);

    // Create the text displaying the currently selected address.
    selectedAddressTextElement = new wxStaticText(bottomControlPanel, wxID_ANY, selectedAddressText, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_VERTICAL);
    selectedAddressTextElement->SetFont(guiFont);
    bottomControlPanelSizer->Add(selectedAddressTextElement, 1, wxLEFT | wxALIGN_CENTER_VERTICAL, 10);
    
    // Create the text displaying the currently selected data.
    selectedDataTextElement = new wxStaticText(bottomControlPanel, wxID_ANY, selectedDataText, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_VERTICAL);
    selectedDataTextElement->SetFont(valueFont);
    bottomControlPanelSizer->Add(selectedDataTextElement, 1, wxALIGN_CENTER_VERTICAL); 

    bottomControlPanelSizer->Add(new wxStaticText(bottomControlPanel, wxID_ANY, " ", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_VERTICAL), 1, wxALIGN_CENTER_VERTICAL);  
    // Set Main Sizer.
    this->SetSizerAndFit(parentSizer);

    memorySearchBar->SetFocus();
}

MemoryViewerFrame::~MemoryViewerFrame()
{
    if (ENABLE_DEBUG_PRINTS)
        std::cout << "Exiting: Memory Viewer Frame" << std::endl;
}

// Event Handlers.

void MemoryViewerFrame::handleEmulatorCoreUpdateEvent(wxCommandEvent &event)
{
    // Return early if not active.
    if (!this->IsShown())
        return;

    // Acquire the mutex lock which protects the memory. 
    Memory* memory = emuCore->getMemory();
    emuCore->acquireMutexLock();

    // Determine the row range to render. 
    int scrollbarPosition = (int) round((float) memoryViewGrid->GetScrollPos(wxVERTICAL) * 3.0 / 4.0);
    int rowStart = std::max(scrollbarPosition-2, 0); 
    int rowEnd = std::min(scrollbarPosition+20, MEMORY_VIEW_GRID_ROWS); 

    // Loop over the memory values to update.
    int selectedNotation = selectedNotationDropDown->GetCurrentSelection();
    for (int colNum = 0; colNum < MEMORY_VIEW_GRID_COLS; colNum++){
        for (int rowNum = rowStart; rowNum < rowEnd; rowNum++){
            // Read the memory address.
            word address = rowNum * MEMORY_VIEW_GRID_COLS + colNum;
            byte data = memory->read(address);

            wxString textToRender = "";

            switch(selectedNotation){
                // Decimal.
                case 1:
                    textToRender = std::to_string(data);
                    break;
                // ASCII.
                case 2:
                    textToRender = (char) data;
                    break;
                 // Hex.
                case 0:
                default:
                    char temp[3] = "00";
                    temp[1] = toHex[data % 16];
                    data = data >> 4;
                    temp[0] = toHex[data % 16];
                    textToRender = temp;
                    break;
            }
        
            // Update the cell data.
            memoryViewGrid->SetCellValue(rowNum, colNum, textToRender);
        }
    }

    // Update the bottom control panel.
    byte data = memory->read(selectedAddress);
    convertWordToBinaryNotation(data, selectedDataText+6);
    selectedDataTextElement->SetLabelText(selectedDataText);

    // Release the lock.
    emuCore->releaseMutexLock();
}

void MemoryViewerFrame::OnCloseWindow(wxCloseEvent &event)
{
    this->Hide();
}

void MemoryViewerFrame::OnGridClick(wxGridEvent& event){
    int selectedCol = event.GetCol();
    int selectedRow = event.GetRow();

    selectedAddress = selectedRow * MEMORY_VIEW_GRID_COLS + selectedCol;

    convertWordToHexNotation(selectedAddress, selectedAddressText+9);
    selectedAddressTextElement->SetLabelText(selectedAddressText);

    event.Skip();
}

void MemoryViewerFrame::OnSearch(wxCommandEvent& event){
    std::string searchFilter = std::string(event.GetString().mb_str());


    // Remove the "0x" if it exists.
    if(searchFilter.length() >= 2 && searchFilter[0] == '0' && (searchFilter[1] == 'x' || searchFilter[1] == 'X')){
        searchFilter = searchFilter.substr(2, searchFilter.length()-1);
    }
    // Return early if the filter is the wrong length.
    if(searchFilter.length() <= 0 || searchFilter.length() > 4) return;

    // Try and convert the filter into an integer.
    int convertedFilter = -1;
    try {
        convertedFilter = std::stoi(searchFilter, 0, 16);
    } catch (...) {}
    if(convertedFilter == -1) return;

    memoryViewGrid->MakeCellVisible(convertedFilter / 16, convertedFilter % 16);

    event.Skip();
}

wxColour MemoryViewerFrame::getMemoryViewerGridCellColour(word address){
    // Memory Map.

    // 0000-3FFF   16KB ROM Bank 00     (in cartridge, fixed at bank 00).
    if (address >= ROMBANK0_START && address <= ROMBANK0_END) {
        return wxColour(255, 204, 204);
    }
    // 4000-7FFF   16KB ROM Bank 01..NN (in cartridge, switchable bank number).
    else if (address >= ROMBANKN_START && address <= ROMBANKN_END) {
        return wxColour(255, 211, 204);
    }
    // 8000-9FFF   8KB Video RAM (VRAM) (switchable bank 0-1 in CGB Mode).
    else if (address >= VRAM_START && address <= VRAM_END) {
        return wxColour(255, 235, 204);
    }
    // A000-BFFF   8KB External RAM     (in cartridge, switchable bank, if any),
    else if (address >= EXTERNALRAM_START && address <= EXTERNALRAM_END) {
        return wxColour(255, 255, 204);
    }
    // C000 - CFFF   4KB Work RAM Bank 0 (WRAM).
    else if (address >= WRAMBANK0_START && address <= WRAMBANK0_END) {
        return wxColour(215, 255, 204);
    }
    // D000-DFFF   4KB Work RAM Bank 1 (WRAM)  (switchable bank 1-7 in CGB Mode).
    else if (address >= WRAMBANK1_START && address <= WRAMBANK1_END) {
        return wxColour(204, 255, 205);
    }
    // E000-FDFF   Same as C000-DDFF (ECHO)    (typically not used).
    else if (address >= ECHO_START && address <= ECHO_END) {
        // Re-calls this function but offset to the actual block of memory.
        return getMemoryViewerGridCellColour(address - ECHO_START + WRAMBANK0_START);
    }
    // FE00-FE9F   Sprite Attribute Table (OAM).
    else if (address >= OAM_START && address <= OAM_END) {
        return wxColour(204, 255, 253);
    }
    // FEA0-FEFF   Not Usable.
    else if (address >= NOTUSABLE_START && address <= NOTUSABLE_END) {
        return wxColour(179, 179, 179);
    }
    // FF00-FF7F   I/O Ports.
    else if (address >= IOPORTS_START && address <= IOPORTS_END) {
        return wxColour(204, 213, 255);
    }
    // FF80-FFFE   High RAM (HRAM).
    else if (address >= HRAM_START && address <= HRAM_END) {
        return wxColour(225, 204, 255);
    }
    // FFFF        Interrupt Enable Register.
    else if (address == INTERRUPT_ENABLE_REGISTER_ADDR) {
        return wxColour(255, 204, 252);
    }

    // Default case - unreachable.
    return wxColour(179, 179, 179);
}