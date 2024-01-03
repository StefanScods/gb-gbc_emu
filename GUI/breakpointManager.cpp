/**
 * The implementation of the BreakpointManagerFrame class.
 */
#include "include\breakpointManager.h"
#include "include\emulationThread.h"
#include "..\core\include\cpu.h"
#include "..\core\include\core.h"
#include "..\core\include\memory.h"

// Enables debug cout statements for this file.
#define ENABLE_DEBUG_PRINTS false
wxFont guiFont = wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);
wxFont legendFont = wxFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);

BreakpointManagerFrame::BreakpointManagerFrame(Core *d_emuCore, EmulationThread *d_emuThread) : wxFrame(NULL, wxID_ANY, BREAKPOINT_MANAGER_TITLE, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER){
    if (ENABLE_DEBUG_PRINTS)
        std::cout << "Starting: OAM Viewer Frame" << std::endl;

    // Save passed pointers.
    emuCore = d_emuCore;
    emuThread = d_emuThread;

    // Font and style constants.
    SetBackgroundColour(wxColour(255,255,255));

    // Create sizers.
    wxBoxSizer* parentSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* topControlPanelSizer = new wxBoxSizer(wxHORIZONTAL);
    pcGridSizer = new wxBoxSizer(wxVERTICAL);

    // PC Control Panel elements.
    wxStaticText* label = new wxStaticText(this, wxID_ANY, "Program Counter:");
    label->SetFont(legendFont);
    topControlPanelSizer->Add(label, 0, wxEXPAND | wxTOP, 4);
    wxTextCtrl* addPCBreakpointTextBox = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
    addPCBreakpointTextBox->SetFont(guiFont);
    addPCBreakpointTextBox->SetToolTip("PC Value");
    topControlPanelSizer->Add(addPCBreakpointTextBox, 0, wxEXPAND | wxLEFT, 60);
    wxButton * addPCBreakpointButton = new wxButton (this, wxID_ANY, "+", wxDefaultPosition, wxDefaultSize);
    addPCBreakpointButton->SetFont(guiFont);
    topControlPanelSizer->Add(addPCBreakpointButton, 0, wxEXPAND);
    addPCBreakpointButton->Bind(wxEVT_BUTTON, &BreakpointManagerFrame::handleAddPCBreakpointButton, this);

    // PC breakpoints grid.
    pcBreakpointGrid = new wxScrolledWindow(this,
            wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL);
    pcBreakpointGrid->SetMaxSize(wxSize(-1, 300));
    pcBreakpointGrid->SetMinSize(wxSize(-1, 300));
    pcBreakpointGrid->SetSizer(pcGridSizer);

    // Format the frame.
    parentSizer->Add(topControlPanelSizer, 0, wxEXPAND | wxALL, 5);
    parentSizer->Add(pcBreakpointGrid, 0, wxEXPAND);
    SetSizerAndFit(parentSizer);
}

BreakpointManagerFrame::~BreakpointManagerFrame(){
    if (ENABLE_DEBUG_PRINTS)
        std::cout << "Exiting: OAM Viewer Frame" << std::endl;
}

// Event Handlers.
void BreakpointManagerFrame::OnCloseWindow(wxCloseEvent &event){
    this->Hide();
}

void BreakpointManagerFrame::handleEmulatorCoreUpdateEvent(wxCommandEvent &event){
    // Return early if not active.
    if (!this->IsShown())
        return;
}

void BreakpointManagerFrame::handleAddPCBreakpointButton(wxCommandEvent& event){
    wxStaticText* element = new wxStaticText(pcBreakpointGrid, wxID_ANY, "0x0000");
    pcGridSizer->Add(element, 0, wxEXPAND | wxALL, 5);
    pcGridSizer->Layout();    
}