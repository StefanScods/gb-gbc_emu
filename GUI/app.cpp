/**
 * The main application class implementation for the GameBoy color emulator.
 * Also defines WxWidgets event table.
 */
#include <iostream>
#include <string>
#include <SDL.h>

#include "include\app.h"
#include "include\mainWindowFrame.h"
#include "include\memoryViewerFrame.h"
#include "include\cpuStateFrame.h"
#include "include\emulationThread.h"
#include "..\core\include\cpu.h"
#include "..\core\include\defines.h"
#include "..\core\include\core.h"

// Enables debug cout statements for this file.
#define ENABLE_DEBUG_PRINTS true

// Custom WxWidget Events.
wxDEFINE_EVENT(EMULATOR_CORE_UPDATE_EVENT, wxCommandEvent);

// WxWidget Event Tables.
wxBEGIN_EVENT_TABLE(MainWindowFrame, wxFrame)
	EVT_CLOSE(MainWindowFrame::OnCloseWindow)
	EVT_COMMAND(wxID_ANY, EMULATOR_CORE_UPDATE_EVENT, MainWindowFrame::handleEmulatorCoreUpdateEvent)
	

	EVT_MENU(wxID_EXIT, MainWindowFrame::OnMenuQuitButton)

	EVT_MENU(wxMenuIDs::OPEN_CPU_STATE_VIEW, MainWindowFrame::OnMenuOpenCPUStateViewButton)
	EVT_MENU(wxMenuIDs::OPEN_MEMORY_VIEWER_VIEW, MainWindowFrame::OnMenuOpenMemoryViewerViewButton)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(CPUStateFrame, wxFrame)
	EVT_CLOSE(CPUStateFrame::OnCloseWindow)
	EVT_COMMAND(wxID_ANY, EMULATOR_CORE_UPDATE_EVENT, CPUStateFrame::handleEmulatorCoreUpdateEvent)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(MemoryViewerFrame, wxFrame)
	EVT_CLOSE(MemoryViewerFrame::OnCloseWindow)
	EVT_COMMAND(wxID_ANY, EMULATOR_CORE_UPDATE_EVENT, MemoryViewerFrame::handleEmulatorCoreUpdateEvent)
	EVT_GRID_CELL_LEFT_CLICK( MemoryViewerFrame::OnGridClick )
wxEND_EVENT_TABLE()

bool App::OnInit(){
	if (ENABLE_DEBUG_PRINTS)
		std::cout << "Starting: " << APP_TITLE << std::endl;

	// Initialize SDL2 subsystem.
	if(!initializeSDL2()){
        exit(1);
	}

	// Create the emulator core object.
	emuCore = new Core(PAUSE);

	// Create the main window frame.
	mainWindow = new MainWindowFrame(emuCore, this);
	// Create the main emulation thread.
	emuThread = new EmulationThread(this, emuCore, mainWindow->getDisplayPanel());

	// Finish main window set up.
	mainWindow->setEmuThread(emuThread);
	// Activate the main window frame and give it focus.
	mainWindow->Show(true);
	SetTopWindow(mainWindow);

	// Attempt to create a separate thread to run the emulator in.
	int returnCode = emuThread->Create();
	if (returnCode != wxTHREAD_NO_ERROR)
	{
		// If the emulation thread cannot be created display an error and exit.
		wxLogError("Failed to create thread main emulation thread!");
		exit(returnCode);
	}

	// Create the CPU state display.
	cpuStateFrame = new CPUStateFrame(emuCore, emuThread);
	cpuStateFrame->Hide();

	// Create the memory viewer displays.
	memoryViewerFrame = new MemoryViewerFrame(emuCore, emuThread);
	memoryViewerFrame->Hide();

	// Start emulation.
	runningEmulator = true;
	// Begin the emulation thread's execution.
	emuThread->Run();

	return true;
}

int App::OnExit()
{
	// Stop the emulator.
	runningEmulator = false;
	// Wait for the main thread to return (Finishes the current frame).
	emuThread->Wait();

	// Clean dynamic memory. WxWidgets handles its own memory.
	delete emuCore;
	delete emuThread;

	if (ENABLE_DEBUG_PRINTS)
		std::cout << "Exiting: " << APP_TITLE << std::endl;
	return 0;
}

void App::closeAllSideFrames(){
	if(cpuStateFrame != nullptr){
		cpuStateFrame->Destroy();
		cpuStateFrame = nullptr;
	}

	if(memoryViewerFrame != nullptr){
		memoryViewerFrame->Destroy();
		memoryViewerFrame = nullptr;
	}	
}

bool App::initializeSDL2(){

	// Initialize the main SDL2 subsystem.
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "Failed to initialize SDL2 with error: "<< SDL_GetError() << std::endl;
		return false;
	}

	// Everything initialized successfully.
	return true;
}

void App::sendEmulationCoreUpdateEvent()
{
	if (!runningEmulator) return;

	// Create an event.
	wxCommandEvent event(EMULATOR_CORE_UPDATE_EVENT);

	// Send the event to the child windows.
	wxPostEvent(this, event);
	
	if(mainWindow != nullptr && mainWindow->IsShown())
		wxPostEvent(mainWindow, event);

	if(cpuStateFrame != nullptr && cpuStateFrame->IsShown())
		wxPostEvent(cpuStateFrame, event);

	if(memoryViewerFrame != nullptr && memoryViewerFrame->IsShown())
		wxPostEvent(memoryViewerFrame, event);
}

void App::showCPUStateFrame(){ 
	if(cpuStateFrame!=nullptr) 	cpuStateFrame->Show(true);
}

void App::showMemoryViewerFrame(){ 
	if(memoryViewerFrame!=nullptr) 	memoryViewerFrame->Show(true);
}