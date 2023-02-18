/**
 * The implementation of the MainWindowFrame class.
 */
#include "include\mainWindowFrame.h"
#include "include\app.h"
#include "include\emulationThread.h"
#include "..\core\include\core.h"
#include "..\core\include\defines.h"
#include <iostream>
#include <string.h>

// Enables debug cout statements for this file.
#define ENABLE_DEBUG_PRINTS true

MainWindowFrame::MainWindowFrame(Core *d_emuCore, App* d_appContext ) : wxFrame(NULL, wxID_ANY, APP_TITLE, wxDefaultPosition, wxSize(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT))
{
	if (ENABLE_DEBUG_PRINTS)
		std::cout << "Starting: Main Window Frame" << std::endl;

	// Save passed pointers.
	emuCore = d_emuCore;
	appContext = d_appContext;

	this->SetBackgroundColour(wxColour(0x0, 0x0, 0x0, 0xFF));

	// Create a sizer to maintain the desired size of the main display.
	wxGridSizer* mainDisplaySizer  = new wxGridSizer(1, 1, 0, 0);

	// Create a panel used as the emulator's display.
	displayPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT),  wxALIGN_TOP || wxALIGN_LEFT || wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER);
	mainDisplaySizer->Add(displayPanel, 0);

	// Render the sizer.
	SetSizer(mainDisplaySizer);

	// Construct the top menubar.
	menuBar = new wxMenuBar();
	// File Menu
	fileMenuLayout = new wxMenu();
	fileMenuLayout->Append(wxID_EXIT, _T("&Quit"));
	menuBar->Append(fileMenuLayout, _T("&File"));

	// Tool Menu.
	toolsMenuLayout = new wxMenu();
	toolsMenuLayout->Append(wxMenuIDs::OPEN_CPU_STATE_VIEW, _T("&Open CPU State View"));
	menuBar->Append(toolsMenuLayout, _T("&Tools"));

	// Render the top menu bar.
	SetMenuBar(menuBar);

}

MainWindowFrame::~MainWindowFrame()
{
	if (ENABLE_DEBUG_PRINTS)
		std::cout << "Exiting: Main Window Frame" << std::endl;

}

// Event Handlers.

void MainWindowFrame::handleEmulatorCoreUpdateEvent(wxCommandEvent& event){
	// Change the title to reflect the current FPS.
	this->SetTitle(std::string(APP_TITLE) + " - " + std::to_string(emuThread->getCurrentFPS()));
}

void MainWindowFrame::OnCloseWindow(wxCloseEvent& event){
	// Close all other frames open in the application.
	appContext->closeAllSideFrames();
	// Close this frame.
	event.Skip();
	// All frames have been closed. Application will now exit.
}

void MainWindowFrame::OnMenuQuitButton(wxCommandEvent& event){
	Close(true);
}

void  MainWindowFrame::OnMenuOpenCPUStateViewButton(wxCommandEvent& event){
	appContext->showCPUStateFrame();
}