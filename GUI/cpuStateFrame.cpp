/**
 * The implementation of the CPUStateFrame class.
 */
#include "include\cpuStateFrame.h"
#include "include\emulationThread.h"
#include "..\core\include\cpu.h"
#include "..\core\include\core.h"
#include "..\core\include\defines.h"
#include <iostream>
#include <string.h>

// Enables debug cout statements for this file.
#define ENABLE_DEBUG_PRINTS true

CPUStateFrame::CPUStateFrame(Core *d_emuCore, EmulationThread *d_emuThread) : wxFrame(NULL, wxID_ANY, CPU_STATE_DISPLAY_TITLE, wxDefaultPosition, wxSize(CPU_STATE_DISPLAY_WIDTH, CPU_STATE_DISPLAY_HEIGHT), wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER)
{
    if (ENABLE_DEBUG_PRINTS)
        std::cout << "Starting: CPU State Frame" << std::endl;

    // Save passed pointers.
    emuCore = d_emuCore;
    emuThread = d_emuThread;

    // Defines the size of the view grids.
    int registerGridRowSize = 35;
    int registerGridColSize = 142;
    int flagGridRowSize = registerGridRowSize;
    int flagGridColSize = 71;

    // Create WxWidget elements.

    // Create the Register View Grid and set the high-level styling.
    registerViewGrid = new wxGrid(
        this,
        wxID_ANY,
        wxPoint(0, 0),
        wxSize(CPU_STATE_DISPLAY_WIDTH - 15, registerGridRowSize * 7));
    registerViewGrid->CreateGrid(7, 2);
    // Make the grid read-only.
    registerViewGrid->EnableDragGridSize(false);
    registerViewGrid->EnableEditing(false);
    // Disable column and row labels..
    registerViewGrid->HideColLabels();
    registerViewGrid->HideRowLabels();

    // Resize the rows and columns.
    registerViewGrid->SetColSize(0, registerGridColSize);
    registerViewGrid->SetColSize(1, registerGridColSize);
    for (int rowIndex = 0; rowIndex < 7; rowIndex++)
    {
        registerViewGrid->SetRowSize(rowIndex, registerGridRowSize);
    }

    // Set the font of each cell.
    wxFont categoryLabelFont = wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, true);
    wxFont regLabelFont = wxFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
    wxFont regValueFont = wxFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);
    // Category Labels.
    registerViewGrid->SetCellFont(0, 0, categoryLabelFont);
    registerViewGrid->SetCellFont(0, 1, categoryLabelFont);
    registerViewGrid->SetCellAlignment(0, 0, wxALIGN_CENTRE, wxALIGN_TOP);
    registerViewGrid->SetCellAlignment(0, 1, wxALIGN_CENTRE, wxALIGN_TOP);
    registerViewGrid->SetCellBackgroundColour(0, 0, *wxLIGHT_GREY);
    registerViewGrid->SetCellBackgroundColour(0, 1, *wxLIGHT_GREY);
    // Register labels and their value.
    for (int rowIndex = 1; rowIndex < 7; rowIndex++)
    {
        registerViewGrid->SetCellFont(rowIndex, 0, regLabelFont);
        registerViewGrid->SetCellFont(rowIndex, 1, regValueFont);
    }

    // Add column labels.
    registerViewGrid->SetCellValue(0, 0, "Register");
    registerViewGrid->SetCellValue(0, 1, "Value");

    // Add register labels.
    registerViewGrid->SetCellValue(1, 0, "AF");
    registerViewGrid->SetCellValue(2, 0, "BC");
    registerViewGrid->SetCellValue(3, 0, "DE");
    registerViewGrid->SetCellValue(4, 0, "HL");
    registerViewGrid->SetCellValue(5, 0, "PC");
    registerViewGrid->SetCellValue(6, 0, "SP");

    // Add the Register View Grid to the sizer.

    // Create the Flag View Grid and set the high-level styling.
    flagViewGrid = new wxGrid(
        this,
        wxID_ANY,
        wxPoint(0, (registerGridRowSize)*7),
        wxSize(CPU_STATE_DISPLAY_WIDTH - 15, flagGridColSize * 4));
    flagViewGrid->CreateGrid(2, 4);
    flagViewGrid->SetCellSize(0, 0, 1, 4);
    // Make the grid read-only.
    flagViewGrid->EnableDragGridSize(false);
    flagViewGrid->EnableEditing(false);
    // Disable column and row labels..
    flagViewGrid->HideColLabels();
    flagViewGrid->HideRowLabels();

    // Set the font and size of each cell.
    // Category Labels.
    flagViewGrid->SetCellFont(0, 0, categoryLabelFont);
    flagViewGrid->SetCellAlignment(0, 0, wxALIGN_CENTRE, wxALIGN_TOP);
    flagViewGrid->SetCellBackgroundColour(0, 0, *wxLIGHT_GREY);
    flagViewGrid->SetRowSize(0, flagGridRowSize);
    flagViewGrid->SetRowSize(1, flagGridRowSize);
    // Flag values.
    for (int colIndex = 0; colIndex < 4; colIndex++)
    {
        flagViewGrid->SetColSize(colIndex, flagGridColSize);
        flagViewGrid->SetCellFont(1, colIndex, regValueFont);
        flagViewGrid->SetCellAlignment(1, colIndex, wxALIGN_CENTRE, wxALIGN_TOP);
    }

    // Set the values.
    flagViewGrid->SetCellValue(0, 0, "Flag");
    flagViewGrid->SetCellValue(1, 0, "Z");
    flagViewGrid->SetCellValue(1, 1, "N");
    flagViewGrid->SetCellValue(1, 2, "H");
    flagViewGrid->SetCellValue(1, 3, "C");
}

CPUStateFrame::~CPUStateFrame()
{
    if (ENABLE_DEBUG_PRINTS)
        std::cout << "Exiting: CPU State Frame" << std::endl;
}

// Event Handlers.

void CPUStateFrame::handleEmulatorCoreUpdateEvent(wxCommandEvent &event)
{
    // Return early if not active.
    if (!this->IsShown())
        return;

    // Create a buffer to convert word values into hex notation strings.
    char valueBuffer[7];

    // Create a state object to populate.
    CPU_State currentCPUState;

    // Pull the current state of the CPU into the buffer.
    emuCore->populateCpuStateBuffer(&currentCPUState);

    // Update the display.

    // Registers.

    // Reg AF.
    convertWordToHexNotation(currentCPUState.reg_AF, valueBuffer);
    registerViewGrid->SetCellValue(1, 1, valueBuffer);
    // Reg BC.
    convertWordToHexNotation(currentCPUState.reg_BC, valueBuffer);
    registerViewGrid->SetCellValue(2, 1, valueBuffer);
    // Reg DE.
    convertWordToHexNotation(currentCPUState.reg_DE, valueBuffer);
    registerViewGrid->SetCellValue(3, 1, valueBuffer);
    // Reg HL.
    convertWordToHexNotation(currentCPUState.reg_HL, valueBuffer);
    registerViewGrid->SetCellValue(4, 1, valueBuffer);
    // Reg PC.
    convertWordToHexNotation(currentCPUState.PC, valueBuffer);
    registerViewGrid->SetCellValue(5, 1, valueBuffer);
    // Reg SP.
    convertWordToHexNotation(currentCPUState.SP, valueBuffer);
    registerViewGrid->SetCellValue(6, 1, valueBuffer);

    // Flags.
    // Get the value of the F register.
    byte regF = (byte)(currentCPUState.reg_AF & 0xFF);

    // Set the colour based on if the flag is high.

    // Flag Z.
    (readBit(regF, FLAG_Z) == 1) ? 
        flagViewGrid->SetCellTextColour(1, 0, *wxBLACK) : 
        flagViewGrid->SetCellTextColour(1, 0, *wxLIGHT_GREY);
    // Flag N.
    (readBit(regF, FLAG_N) == 1) ? 
        flagViewGrid->SetCellTextColour(1, 1, *wxBLACK) : 
        flagViewGrid->SetCellTextColour(1, 1, *wxLIGHT_GREY);
    // Flag H.
    (readBit(regF, FLAG_H) == 1) ? 
        flagViewGrid->SetCellTextColour(1, 2, *wxBLACK) : 
        flagViewGrid->SetCellTextColour(1, 2, *wxLIGHT_GREY);
    // Flag C.
    (readBit(regF, FLAG_C) == 1) ? 
        flagViewGrid->SetCellTextColour(1, 3, *wxBLACK) : 
        flagViewGrid->SetCellTextColour(1, 3, *wxLIGHT_GREY);

    // Force the grids to update.
    registerViewGrid->Refresh();
    flagViewGrid->Refresh();
}

void CPUStateFrame::OnCloseWindow(wxCloseEvent &event)
{
    this->Hide();
}