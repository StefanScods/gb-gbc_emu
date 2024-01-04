/**
 * The implementation of the BreakpointManagerFrame class.
 */
#include "include\breakpointManager.h"
#include "include\emulationThread.h"
#include "..\core\include\cpu.h"
#include "..\core\include\core.h"
#include "..\core\include\memory.h"
#include <algorithm>

// Enables debug cout statements for this file.
#define ENABLE_DEBUG_PRINTS false
wxFont guiFont = wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);
wxFont legendFont = wxFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);


BreakPointElement::BreakPointElement(wxWindow* parent, wxWindowID id, word d_address, wxBoxSizer* d_parentSizer, Core *d_emuCore) : wxWindow(parent, id){
    address = d_address;
    parentSizer = d_parentSizer;
    emuCore = d_emuCore;

    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

    SetBackgroundColour(wxColour(255,255,255));

    // Create a label for the address.
    char addressText[] = "0x0000";
    convertWordToHexNotation(address, addressText);
    label = new wxStaticText(this, wxID_ANY, addressText);
    label->SetFont(guiFont);
    label->SetMinSize(wxSize(200, -1));
    sizer->Add(label, 0, wxEXPAND | wxTOP | wxLEFT, 5);

    // Add buttons.
    toggleEnableBreakpointButton = new wxButton (this, wxID_ANY, "Disable", wxDefaultPosition, wxDefaultSize);
    toggleEnableBreakpointButton->SetFont(guiFont);
    toggleEnableBreakpointButton->Bind(wxEVT_BUTTON, &BreakPointElement::toggleEnabled, this);
    sizer->Add(toggleEnableBreakpointButton, 0, wxEXPAND);

    wxButton* removeBreakpointButton = new wxButton (this, wxID_ANY, "x", wxDefaultPosition, wxDefaultSize);
    removeBreakpointButton->Bind(wxEVT_BUTTON, &BreakPointElement::remove, this);
    removeBreakpointButton->SetFont(guiFont);
    sizer->Add(removeBreakpointButton, 0, wxEXPAND);

    SetSizer(sizer);
}

void BreakPointElement::toggleEnabled(wxCommandEvent& event){
    enabled = !enabled;

    std::vector<word>* enabledCPUBreakpoints = emuCore->getEnabledCPUBreakpoints();

    // Change colour.
    if(enabled){
        label->SetForegroundColour(wxColour(0,0,0));
        toggleEnableBreakpointButton->SetLabel("Disable");
        // Add the address to the enabled list if not already enabled.
        auto index = std::find(enabledCPUBreakpoints->begin(), enabledCPUBreakpoints->end(), address);
        if(index==enabledCPUBreakpoints->end()) enabledCPUBreakpoints->push_back(address);
    }
    else{
        label->SetForegroundColour(wxColour(190,190,190));
        toggleEnableBreakpointButton->SetLabel("Enable");
        // Remove the address to the enabled list if not already disabled.
        auto index = std::find(enabledCPUBreakpoints->begin(),enabledCPUBreakpoints->end(), address);
        if(index!=enabledCPUBreakpoints->end()) enabledCPUBreakpoints->erase(index);

    }
    label->Refresh();
}

void BreakPointElement::remove(wxCommandEvent& event){
    // Remove the breakpoint from the lists.
    std::vector<word>* CPUBreakpoints = emuCore->getCPUBreakpoints();
    auto index = std::find(CPUBreakpoints->begin(),CPUBreakpoints->end(), address);
    if(index!=CPUBreakpoints->end()) CPUBreakpoints->erase(index);
    std::vector<word>* enabledCPUBreakpoints = emuCore->getEnabledCPUBreakpoints();
    index = std::find(enabledCPUBreakpoints->begin(),enabledCPUBreakpoints->end(), address);
    if(index!=enabledCPUBreakpoints->end()) enabledCPUBreakpoints->erase(index);

    parentSizer->Detach(this);
    ((ScrollableBreakpointPanel*) GetParent())->formatScrollbar();
    Destroy();
}

ScrollableBreakpointPanel::ScrollableBreakpointPanel(wxWindow* parent, wxWindowID id, Core *d_emuCore) : wxScrolledWindow(parent, id){
    emuCore = d_emuCore;
    SetBackgroundColour(wxColour(255,255,255));
    // Create sizers.
    gridSizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(gridSizer);
}

void ScrollableBreakpointPanel::addBreakpoint(word address){
    // Add the breakpoint to the array.
    std::vector<word>* CPUBreakpoints = emuCore->getCPUBreakpoints();
    auto index = std::find(CPUBreakpoints->begin(),CPUBreakpoints->end(), address);
    if(index==CPUBreakpoints->end()) CPUBreakpoints->push_back(address);
    else return;
    std::vector<word>* enabledCPUBreakpoints = emuCore->getEnabledCPUBreakpoints();
    index = std::find(enabledCPUBreakpoints->begin(),enabledCPUBreakpoints->end(), address);
    if(index==enabledCPUBreakpoints->end()) enabledCPUBreakpoints->push_back(address);

    // Create a label for the address.
    BreakPointElement* element = new BreakPointElement(this, wxID_ANY, address, gridSizer, emuCore);

    // Try to insert in order.
    bool added = false;
    int itemCount = gridSizer->GetItemCount();
    for(int i = 0; i < itemCount; i++){
        // Fetch breakpoint item.
        wxSizerItem* item = gridSizer->GetItem(i);
        if(!item->GetWindow()->IsKindOf(wxCLASSINFO(BreakPointElement))) continue;

        // Find the place to insert.
        if( ((BreakPointElement*) (item->GetWindow()))->address <= address) continue;

        // Insert the element.
        gridSizer->Insert(i, element, 0, wxEXPAND | wxLEFT | wxBOTTOM, 5);
        added = true;
        break;
    }

    // Add at the end of the list case.
    if(!added) gridSizer->Add(element, 0, wxEXPAND | wxLEFT | wxBOTTOM, 5);

    // Add scrollbar if the grid becomes large enough.
    formatScrollbar();
}

void ScrollableBreakpointPanel::formatScrollbar(){
    this->FitInside(); 
    this->SetScrollRate(0, 10);
}

BreakpointManagerFrame::BreakpointManagerFrame(Core *d_emuCore, EmulationThread *d_emuThread) : wxFrame(NULL, wxID_ANY, BREAKPOINT_MANAGER_TITLE, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER){
    if (ENABLE_DEBUG_PRINTS)
        std::cout << "Starting: Breakpoint Manager Frame" << std::endl;

    // Save passed pointers.
    emuCore = d_emuCore;
    emuThread = d_emuThread;

    // Font and style constants.
    SetBackgroundColour(wxColour(255,255,255));

    // Create sizers.
    wxBoxSizer* parentSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* topControlPanelSizer = new wxBoxSizer(wxHORIZONTAL);
    

    // PC Control Panel elements.
    wxStaticText* label = new wxStaticText(this, wxID_ANY, "Program Counter:");
    label->SetFont(legendFont);
    topControlPanelSizer->Add(label, 0, wxEXPAND | wxTOP, 4);
    addPCBreakpointTextBox = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
    addPCBreakpointTextBox->SetFont(guiFont);
    addPCBreakpointTextBox->SetToolTip("PC Value");
    topControlPanelSizer->Add(addPCBreakpointTextBox, 0, wxEXPAND | wxLEFT, 60);
    wxButton * addPCBreakpointButton = new wxButton (this, wxID_ANY, "+", wxDefaultPosition, wxDefaultSize);
    addPCBreakpointButton->SetFont(guiFont);
    topControlPanelSizer->Add(addPCBreakpointButton, 0, wxEXPAND);
    addPCBreakpointButton->Bind(wxEVT_BUTTON, &BreakpointManagerFrame::handleAddPCBreakpointButton, this);

    // PC breakpoints grid.
    pcBreakpointGrid = new ScrollableBreakpointPanel(this, wxID_ANY, emuCore);
    pcBreakpointGrid->SetMaxSize(wxSize(-1, 300));
    pcBreakpointGrid->SetMinSize(wxSize(-1, 300));
 
    // Format the frame.
    parentSizer->Add(topControlPanelSizer, 0, wxEXPAND | wxALL, 5);
    parentSizer->Add(pcBreakpointGrid, 0, wxEXPAND);
    SetSizerAndFit(parentSizer);

    // Add inital breakpoints.
    std::vector<word>* enabledCPUBreakpoints = emuCore->getEnabledCPUBreakpoints();
    for(auto itr = enabledCPUBreakpoints->begin(); itr != enabledCPUBreakpoints->end(); itr++){
        pcBreakpointGrid->addBreakpoint(*itr);
    }
}

BreakpointManagerFrame::~BreakpointManagerFrame(){
    if (ENABLE_DEBUG_PRINTS)
        std::cout << "Exiting: Breakpoint Manager Frame" << std::endl;
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
    // Fetch the breakpoint to add.
    std::string currentValue = std::string(addPCBreakpointTextBox->GetValue().mb_str());
    // Remove the "0x" if it exists.
    if(currentValue.length() >= 2 && currentValue[0] == '0' && (currentValue[1] == 'x' || currentValue[1] == 'X')){
        currentValue = currentValue.substr(2, currentValue.length()-1);
    }
    // Return early if the value is the wrong length.
    if(currentValue.length() <= 0 || currentValue.length() > 4) return;

    // Try and convert the value into an integer.
    int convertedValue = -1;
    try {
        convertedValue = std::stoi(currentValue, 0, 16);
    } catch (...) {}
    if(convertedValue == -1) return;

    addPCBreakpointTextBox->Clear();
    pcBreakpointGrid->addBreakpoint(convertedValue);
}