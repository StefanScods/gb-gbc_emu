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

    const int BOTTOM_CONTROL_PANEL_HEIGHT = 30;
    const int TOP_CONTROL_PANEL_HEIGHT = MEMORY_VIEWER_DISPLAY_HEIGHT/5-BOTTOM_CONTROL_PANEL_HEIGHT;

    // Create the parent sizer object.
    wxFlexGridSizer* parentSizer = new wxFlexGridSizer(3, 1, 0, 0);

    // Create top level panels and add them to the sizer.
    topControlPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(MEMORY_VIEWER_DISPLAY_WIDTH, TOP_CONTROL_PANEL_HEIGHT));
    topControlPanel->SetBackgroundColour(wxColour(255,255,255));
    parentSizer->Add(topControlPanel, 1, wxEXPAND);
    memoryViewPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(MEMORY_VIEWER_DISPLAY_WIDTH, MEMORY_VIEWER_DISPLAY_HEIGHT/5*4));
    memoryViewPanel->SetBackgroundColour(wxColour(255,255,255));
    parentSizer->Add(memoryViewPanel, 1, wxEXPAND);
    bottomControlPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(MEMORY_VIEWER_DISPLAY_WIDTH, BOTTOM_CONTROL_PANEL_HEIGHT));
    bottomControlPanel->SetBackgroundColour(wxColour(255,255,255));
    parentSizer->Add(bottomControlPanel, 1, wxEXPAND);


    // Create the top control panel elements.
    // const wxString CHOICES[] = {"hex", "decimal"};
    // wxComboBox * valueDisplayDropDown = new wxComboBox(topControlPanel, wxID_ANY, "hex", wxDefaultPosition, wxSize(MEMORY_VIEWER_DISPLAY_WIDTH, TOP_CONTROL_PANEL_HEIGHT), 2, CHOICES);

    // Create the main memory viewer grid.
    memoryViewGrid = new wxGrid( memoryViewPanel,
                -1,
                wxPoint( 0, 0 ),
                wxSize( MEMORY_VIEWER_DISPLAY_WIDTH, MEMORY_VIEWER_DISPLAY_HEIGHT/5*4) );
    
    // Font and style constants.
    wxFont guiFont = wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);
    wxFont legendFont = wxFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
    wxFont valueFont = wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);

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
    wxFlexGridSizer* bottomControlPanelSizer = new wxFlexGridSizer(1, 1, 0, 0);
    wxPanel* selectedAddressPanel = new wxPanel(bottomControlPanel, wxID_ANY, wxDefaultPosition, wxSize(MEMORY_VIEWER_DISPLAY_WIDTH*6/7, BOTTOM_CONTROL_PANEL_HEIGHT));
    bottomControlPanelSizer->Add(selectedAddressPanel, 1, wxEXPAND);

    // Create the text displaying the currently selected address.
    selectedAddressTextElement = new wxStaticText(selectedAddressPanel, wxID_ANY, "Address: 0x0000", wxPoint(10,5), wxSize(MEMORY_VIEWER_DISPLAY_WIDTH*6/7, BOTTOM_CONTROL_PANEL_HEIGHT), wxALIGN_CENTER_VERTICAL);
    selectedAddressTextElement->SetFont(guiFont);

    // Set Sizers.
    this->SetSizerAndFit(parentSizer);
    bottomControlPanel->SetSizerAndFit(bottomControlPanelSizer);
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
    Memory* memory = this->emuCore->getMemory();
    memory->acquireMutexLock();

    // Determine the row range to render. 
    int scrollbarPosition = (int) round((float) memoryViewGrid->GetScrollPos(wxVERTICAL) * 3.0 / 4.0);
    int rowStart = std::max(scrollbarPosition-2, 0); 
    int rowEnd = std::min(scrollbarPosition+20, MEMORY_VIEW_GRID_ROWS); 

    // Loop over the memory values to update.
    for (int colNum = 0; colNum < MEMORY_VIEW_GRID_COLS; colNum++){
        for (int rowNum = rowStart; rowNum < rowEnd; rowNum++){
            // Read the memory address.
            word address = rowNum * MEMORY_VIEW_GRID_COLS + colNum;
            byte data = memory->read(address);
            // Convert the data to hex notation.
            char textToRender[3] = "00";
            textToRender[1] = toHex[data % 16];
            data = data >> 4;
            textToRender[0] = toHex[data % 16];
            // Update the cell data.
            memoryViewGrid->SetCellValue(rowNum, colNum, textToRender);
        }
    }

    // Release the lock.
    memory->releaseMutexLock();
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