/*
The implementation of the MainWindowFrame class.
*/
#include "include\mainWindowFrame.h"
#include "include\emulationThread.h"
#include "..\core\include\core.h"
#include "..\core\include\defines.h"
#include <iostream>
#include <string.h>

// Enables debug cout statements for this file.
#define ENABLE_DEBUG_PRINTS false

MainWindowFrame::MainWindowFrame(Core *d_emuCore) : wxFrame(NULL, wxID_ANY, APP_TITLE, wxDefaultPosition, wxSize(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT))
{
	if (ENABLE_DEBUG_PRINTS)
		std::cout << "Starting: Main Window Frame" << std::endl;

	// Save passed pointers.
	emuCore = d_emuCore;

	// Create a sizer to maintain the desired size of the main display.
	wxGridSizer* mainDisplaySizer  = new wxGridSizer(1, 1, 0, 0);

	// Create a panel used as the emulator's display.
	displayPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT),  wxALIGN_TOP || wxALIGN_LEFT || wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER);
	mainDisplaySizer->Add(displayPanel, 0);

	// // Create the CPU State Display. 
	// cpuStateDisplay = new CpuStateDisplayFrame(emuCore);
	// cpuStateDisplay->Show(true);

	// Render the sizer.
	SetSizer(mainDisplaySizer);
}

MainWindowFrame::~MainWindowFrame()
{
	if (ENABLE_DEBUG_PRINTS)
		std::cout << "Exiting: Main Window Frame" << std::endl;

}


void MainWindowFrame::handleEmulatorCoreUpdateEvent(wxCommandEvent& event){
	if (ENABLE_DEBUG_PRINTS)
		std::cout << "Event: Main Window Frame responding to \'EMULATOR_CORE_UPDATE_EVENT\'." << std::endl;
	
	// Change the title to reflect the current FPS.
	this->SetTitle(std::string(APP_TITLE) + " - " + std::to_string(emuThread->getCurrentFPS()));
}