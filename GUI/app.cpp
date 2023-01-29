/*
The main application class implementation for the gameboy color emulator.
Also defines WxWidgets event table.
*/
#include <iostream>
#include <string>
#include <SDL.h>

#include "include\app.h"
#include "include\mainWindowFrame.h"
#include "include\emulationThread.h"
#include "..\core\include\cpu.h"
#include "..\core\include\defines.h"
#include "..\core\include\core.h"

// Enables debug cout statements for this file.
#define ENABLE_DEBUG_PRINTS true

// Custom WxWidget Events.
wxDEFINE_EVENT(EMULATOR_CORE_UPDATE_EVENT, wxCommandEvent);

// WxWidget Event Table.

wxBEGIN_EVENT_TABLE(MainWindowFrame, wxFrame)
	EVT_COMMAND(wxID_ANY, EMULATOR_CORE_UPDATE_EVENT, MainWindowFrame::handleEmulatorCoreUpdateEvent)
wxEND_EVENT_TABLE()

bool App::OnInit()
{
	if (ENABLE_DEBUG_PRINTS)
		std::cout << "Starting: " << APP_TITLE << std::endl;

	// Initialize SDL2 subsystem.
	if(!initializeSDL2()){
        exit(1);
	}

	// Create the emulator core object.
	emuCore = new Core(CONTINUE);

	// Create the main window frame.
	mainWindow = new MainWindowFrame(emuCore);
	// Create the main emulation thread.
	emuThread = new EmulationThread(this, emuCore, mainWindow->getDisplayPanel());
	mainWindow->setEmuThread(emuThread);

	// Attempt to create a sepreate thread to run the emulator in.
	int returnCode = emuThread->Create();
	if (returnCode != wxTHREAD_NO_ERROR)
	{
		// If the emulation thread cannot be created display an error and exit.
		wxLogError("Failed to create thread main emulation thread!");
		exit(returnCode);
	}

	// Activate the main window frame and give it focus.
	mainWindow->Show(true);
	SetTopWindow(mainWindow);

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
	if (!runningEmulator)
		return;

	// Create an event.
	wxCommandEvent event(EMULATOR_CORE_UPDATE_EVENT);

	// Send the event to the child windows.
	wxPostEvent(this, event);
	wxPostEvent(mainWindow, event);
}
