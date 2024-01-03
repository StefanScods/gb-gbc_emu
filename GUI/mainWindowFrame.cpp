/**
 * The implementation of the MainWindowFrame class.
 */
#include "include\mainWindowFrame.h"
#include "include\app.h"
#include "include\emulationThread.h"
#include "..\core\include\core.h"
#include "..\core\include\defines.h"
#include <wx/filedlg.h>
#include <iostream>
#include <string.h>
#include <sstream> 

// Enables debug cout statements for this file.
#define ENABLE_DEBUG_PRINTS false

MainWindowFrame::MainWindowFrame(Core *d_emuCore, App* d_appContext ) : wxFrame(NULL, wxID_ANY, APP_TITLE, wxDefaultPosition, wxSize(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT))
{
	if (ENABLE_DEBUG_PRINTS)
		std::cout << "Starting: Main Window Frame" << std::endl;

	// Save passed pointers.
	emuCore = d_emuCore;
	appContext = d_appContext;

	this->SetBackgroundColour(wxColour(0x0, 0x0, 0x0, 0xFF));

	// Construct the top menubar.
	menuBar = new wxMenuBar();
	// File Menu
	fileMenuLayout = new wxMenu();
	fileMenuLayout->Append(wxMenuIDs::OPEN_ROM, _T("&Open ROM\tCtrl+O"));
	fileMenuLayout->Append(wxMenuIDs::CLOSE_ROM, _T("&Close ROM"));
	fileMenuLayout->Append(wxMenuIDs::RESET_GAMEBOY, _T("&Reset\tCtrl+R"));
	fileMenuLayout->AppendSeparator();
	fileMenuLayout->Append(wxID_EXIT, _T("&Quit"));
	menuBar->Append(fileMenuLayout, _T("&File"));

	// Tool Menu.
	toolsMenuLayout = new wxMenu();
	toolsMenuLayout->Append(wxMenuIDs::OPEN_CARTRIDGE_VIEWER_VIEW, _T("&Open Cartridge Info"));
	toolsMenuLayout->Append(wxMenuIDs::OPEN_CPU_STATE_VIEW, _T("&Open CPU State View"));
	toolsMenuLayout->Append(wxMenuIDs::OPEN_MEMORY_VIEWER_VIEW, _T("&Open Memory Viewer"));
	toolsMenuLayout->Append(wxMenuIDs::OPEN_PALETTE_VIEWER_VIEW, _T("&Open Palette Viewer"));
	toolsMenuLayout->Append(wxMenuIDs::OPEN_TILE_VIEWER_VIEW, _T("&Open Tile Viewer"));
	toolsMenuLayout->Append(wxMenuIDs::OPEN_OAM_VIEWER_VIEW, _T("&Open OAM Viewer"));
	toolsMenuLayout->Append(wxMenuIDs::OPEN_BACKGROUND_VIEWER_VIEW, _T("&Open Background Viewer"));
	menuBar->Append(toolsMenuLayout, _T("&Tools"));

	// Display Menu.
	displayMenuLayout = new wxMenu();
	// Add pixel perfect resolutions.
	for(int i = 1; i <= 5; i++){
		std::string label =
			"&x" + 
			std::to_string(i) + 
			" (" + 
			std::to_string(SCREEN_WIDTH*i) + 
			"x" + 
			std::to_string(SCREEN_HEIGHT*i) + 
			")";
		displayMenuLayout->Append(wxMenuIDs::DISPLAY_SIZE_1 - 1 + i, label.c_str());
	}
	menuBar->Append(displayMenuLayout, _T("&Display"));

	// Debug Menu.
	debugMenuLayout = new wxMenu();
	debugMenuLayout->Append(wxMenuIDs::CONTINUE_EMULATION_MENU, _T("&Continue\tF9"));
	debugMenuLayout->Append(wxMenuIDs::PAUSE_EMULATION_MENU, _T("&Pause\tF10"));
	debugMenuLayout->Append(wxMenuIDs::STEP_FRAME_MENU, _T("&Step Frame\tF8"));
	debugMenuLayout->Append(wxMenuIDs::STEP_CPU_MENU, _T("&Step CPU\tF7"));
	debugMenuLayout->AppendSeparator();
	debugMenuLayout->Append(wxMenuIDs::OPEN_BREAKPOINT_MANAGER, _T("&Breakpoints\tCtrl+B"));
	menuBar->Append(debugMenuLayout, _T("&Debug"));

	// Render the top menu bar.
	SetMenuBar(menuBar);

	// Create a sizer to maintain the desired size of the main display.
	mainDisplaySizer = new wxGridSizer(1, 1, 0, 0);

	// Create a panel used as the emulator's display.
	displayPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT), wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER);
	mainDisplaySizer->Add(displayPanel, 0, wxSHAPED | wxALIGN_CENTER);

	// Render the sizer.
	SetSizer(mainDisplaySizer);
	// Set inital size of screen to be 3x the default res.
	pixelPerfectResizer(3);
}

MainWindowFrame::~MainWindowFrame()
{
	if (ENABLE_DEBUG_PRINTS)
		std::cout << "Exiting: Main Window Frame" << std::endl;

}

void MainWindowFrame::pixelPerfectResizer(int multiplier) {
	displayPanel->SetMinSize(
		wxSize(
			SCREEN_WIDTH  * multiplier,
			SCREEN_HEIGHT * multiplier
		)
	);
	mainDisplaySizer->Layout();
	Fit();
}

// Event Handlers.

void MainWindowFrame::handleEmulatorCoreUpdateEvent(wxCommandEvent& event){
	Cartridge* cartridge = emuCore->getCartridge();
	std::string cartName = "";
	// If there is a cartridge loaded, show the name in the application title.
	if(cartridge->isROMLoaded()){
		cartName = " - ";
		cartName += cartridge->getROMName();
	}
	// Change the title to reflect the current FPS and loaded title.
	this->SetTitle(std::string(APP_TITLE) + cartName + " - " + std::to_string(emuThread->getCurrentFPS()));
}

void MainWindowFrame::OnCloseWindow(wxCloseEvent& event){
	// Close all other frames open in the application.
	appContext->setRunningEmulationState(false);
	appContext->closeAllSideFrames();
	// Close this frame.
	event.Skip();
	// All frames have been closed. Application will now exit.
}

void MainWindowFrame::OnMenuQuitButton(wxCommandEvent& event){
	Close(true);
}

void MainWindowFrame::OnMenuOpenROMButton(wxCommandEvent& event){
	wxFileDialog openFileDialog(this, _("Select ROM"), "", "",
                       "GameBoy / GameBoy Color (*.gb,*.gbc)|*.gb;*.gbc|All Files|*", wxFD_FILE_MUST_EXIST);

	// Wait for the user to select a file or cancel.
	if (openFileDialog.ShowModal() == wxID_CANCEL) return;

	// Load the selected file.
	std::string filePath(openFileDialog.GetPath().c_str());
	appContext->loadCartridge(filePath);
}
void MainWindowFrame::OnMenuCloseROMButton(wxCommandEvent& event){
	emuCore->acquireMutexLock();
	emuCore->pauseEmulatorExecution();
	appContext->clearCurrentlyLoadedFilePath();
	emuCore->resetCore();
	emuCore->releaseMutexLock();
}
void MainWindowFrame::OnMenuResetGameBoyButton(wxCommandEvent& event){
	appContext->loadCartridge(appContext->getCurrentlyLoadedFilePath());
}

void MainWindowFrame::OnMenuOpenCPUStateViewButton(wxCommandEvent& event){
	appContext->showCPUStateFrame();
}
void MainWindowFrame::OnMenuOpenMemoryViewerViewButton(wxCommandEvent& event){
	appContext->showMemoryViewerFrame();
}
void MainWindowFrame::OnMenuOpenPaletteViewerViewButton(wxCommandEvent& event){
	appContext->showPaletteViewerFrame();
}
void MainWindowFrame::OnMenuOpenTileViewerViewButton(wxCommandEvent& event){
	appContext->showTileViewerFrame();
}
void MainWindowFrame::OnMenuOpenOAMViewerViewButton(wxCommandEvent& event){
	appContext->showOAMViewerFrame();
}
void MainWindowFrame::OnMenuOpenBackgroundViewerViewButton(wxCommandEvent& event){
	appContext->showBackgroundViewerFrame();
}
void MainWindowFrame::OnMenuOpenCartridgeViewerViewButton(wxCommandEvent& event){
	appContext->showCartridgeViewerFrame();
}
void MainWindowFrame::OnMenuOpenBreakpointManagerViewButton(wxCommandEvent& event){
	appContext->showBreakpointManagerFrame();
}
void MainWindowFrame::OnMenuPauseButton(wxCommandEvent& event){
	emuCore->pauseEmulatorExecution();
}
void MainWindowFrame::OnMenuContinueButton(wxCommandEvent& event){
	emuCore->continueEmulatorExecution();
}
void MainWindowFrame::OnMenuStepFrameButton(wxCommandEvent& event){
	emuCore->stepNextFrameButton();
}
void MainWindowFrame::OnMenuStepCPUButton(wxCommandEvent& event){
	emuCore->stepNextInstuctionButton();
}