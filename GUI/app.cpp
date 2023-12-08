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
#include "include\paletteViewerFrame.h"
#include "include\tileViewerFrame.h"
#include "include\oamViewerFrame.h"
#include "include\backgroundViewerFrame.h"

#include "include\emulationThread.h"

#include "..\core\include\cpu.h"
#include "..\core\include\defines.h"
#include "..\core\include\core.h"

// Enables debug cout statements for this file.
#define ENABLE_DEBUG_PRINTS true

// Custom WxWidget Events.
wxDEFINE_EVENT(EMULATOR_CORE_UPDATE_EVENT, wxCommandEvent);
wxDEFINE_EVENT(ON_CARTRIDGE_LOADED_EVENT, wxCommandEvent);

// WxWidget Event Tables.
wxBEGIN_EVENT_TABLE(MainWindowFrame, wxFrame)
	EVT_CLOSE(MainWindowFrame::OnCloseWindow)
	EVT_COMMAND(wxID_ANY, EMULATOR_CORE_UPDATE_EVENT, MainWindowFrame::handleEmulatorCoreUpdateEvent)

	EVT_MENU(wxID_EXIT, MainWindowFrame::OnMenuQuitButton)
	EVT_MENU(wxMenuIDs::OPEN_CPU_STATE_VIEW, MainWindowFrame::OnMenuOpenCPUStateViewButton)
	EVT_MENU(wxMenuIDs::OPEN_MEMORY_VIEWER_VIEW, MainWindowFrame::OnMenuOpenMemoryViewerViewButton)
	EVT_MENU(wxMenuIDs::OPEN_PALETTE_VIEWER_VIEW, MainWindowFrame::OnMenuOpenPaletteViewerViewButton)
	EVT_MENU(wxMenuIDs::OPEN_TILE_VIEWER_VIEW, MainWindowFrame::OnMenuOpenTileViewerViewButton)
	EVT_MENU(wxMenuIDs::OPEN_OAM_VIEWER_VIEW, MainWindowFrame::OnMenuOpenOAMViewerViewButton)
	EVT_MENU(wxMenuIDs::OPEN_BACKGROUND_VIEWER_VIEW, MainWindowFrame::OnMenuOpenBackgroundViewerViewButton)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(CPUStateFrame, wxFrame)
	EVT_CLOSE(CPUStateFrame::OnCloseWindow)
	EVT_COMMAND(wxID_ANY, EMULATOR_CORE_UPDATE_EVENT, CPUStateFrame::handleEmulatorCoreUpdateEvent)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(MemoryViewerFrame, wxFrame)
	EVT_CLOSE(MemoryViewerFrame::OnCloseWindow)
	EVT_COMMAND(wxID_ANY, EMULATOR_CORE_UPDATE_EVENT, MemoryViewerFrame::handleEmulatorCoreUpdateEvent)
	EVT_GRID_CELL_LEFT_CLICK(MemoryViewerFrame::OnGridClick)
	EVT_TEXT(wxMenuIDs::MEMORY_MAP_SEARCHBAR, MemoryViewerFrame::OnSearch)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(PaletteViewerFrame, wxFrame)
	EVT_CLOSE(PaletteViewerFrame::OnCloseWindow)
	EVT_COMMAND(wxID_ANY, EMULATOR_CORE_UPDATE_EVENT, PaletteViewerFrame::handleEmulatorCoreUpdateEvent)
	EVT_COMMAND(wxID_ANY, ON_CARTRIDGE_LOADED_EVENT, PaletteViewerFrame::handleOnCartridgeLoadedEvent)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(TileViewerFrame, wxFrame)
	EVT_CLOSE(TileViewerFrame::OnCloseWindow)
	EVT_COMMAND(wxID_ANY, EMULATOR_CORE_UPDATE_EVENT, TileViewerFrame::handleEmulatorCoreUpdateEvent)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(OAMViewerFrame, wxFrame)
	EVT_CLOSE(OAMViewerFrame::OnCloseWindow)
	EVT_COMMAND(wxID_ANY, EMULATOR_CORE_UPDATE_EVENT, OAMViewerFrame::handleEmulatorCoreUpdateEvent)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(BackgroundViewerFrame, wxFrame)
	EVT_CLOSE(BackgroundViewerFrame::OnCloseWindow)
	EVT_COMMAND(wxID_ANY, EMULATOR_CORE_UPDATE_EVENT, BackgroundViewerFrame::handleEmulatorCoreUpdateEvent)
wxEND_EVENT_TABLE()

void App::loadCartridge(std::string filepath){
	if(emuCore == nullptr){
		std::cerr << "Initialize the emulation core before opening a cartridge!" << std::endl;
		exit(1);
	}

	// Attempt to load the ROM file.
	if(!emuCore->loadROM(filepath)){
		exit(1);
	}

	// Start emulation.
	runningEmulator = true;
	// Begins running the emulator's fetch and execute loop.
	emuCore->continueEmulatorExecution();

	// On Success, send an event.
	sendCartridgeLoadedEvent();
}

bool App::OnInit()
{
	if (ENABLE_DEBUG_PRINTS)
		std::cout << "Starting: " << APP_TITLE << std::endl;

	// Initialize SDL2 subsystem.
	if (!initializeSDL2())
	{
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

	// Create the VRAM viewer displays.
	paletteViewerFrame = new PaletteViewerFrame(emuCore, emuThread);
	paletteViewerFrame->Hide();
	tileViewerFrame = new TileViewerFrame(this, emuCore, emuThread);
	tileViewerFrame->Hide();
	oamViewerFrame = new OAMViewerFrame(emuCore, emuThread);
	oamViewerFrame->Hide();
	backgroundViewerFrame = new BackgroundViewerFrame(this, emuCore, emuThread);
	backgroundViewerFrame->Hide();

	// Add additional render events to the SDL render loop. 
	emuThread->addAdditionalRenderEvent(
		std::bind(&TileViewerFrame::updateSDLDisplays, tileViewerFrame)
	);
	emuThread->addAdditionalRenderEvent(
		std::bind(&BackgroundViewerFrame::updateSDLDisplays, backgroundViewerFrame)
	);

	emuThread->SetPriority(wxPRIORITY_MAX);
	// Begin the emulation thread's execution.
	emuThread->Run();

	// Load the ROM file.
	loadCartridge("D:/C++/gb-gbc_emu/testroms/Tetris.gb");

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

void App::closeAllSideFrames()
{
	if (cpuStateFrame != nullptr){
		cpuStateFrame->Destroy();
		cpuStateFrame = nullptr;
	}
	if (memoryViewerFrame != nullptr){
		memoryViewerFrame->Destroy();
		memoryViewerFrame = nullptr;
	}
	if (paletteViewerFrame != nullptr){
		paletteViewerFrame->Destroy();
		paletteViewerFrame = nullptr;
	}	
	if (tileViewerFrame != nullptr){
		tileViewerFrame->Destroy();
		tileViewerFrame = nullptr;
	}	
	if (oamViewerFrame != nullptr){
		oamViewerFrame->Destroy();
		oamViewerFrame = nullptr;
	}
	if (backgroundViewerFrame != nullptr){
		backgroundViewerFrame->Destroy();
		backgroundViewerFrame = nullptr;
	}
}

bool App::initializeSDL2()
{

	// Initialize the main SDL2 subsystem.
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "Failed to initialize SDL2 with error: " << SDL_GetError() << std::endl;
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

	if (mainWindow != nullptr && mainWindow->IsShown())
		wxPostEvent(mainWindow, event);
	if (cpuStateFrame != nullptr && cpuStateFrame->IsShown())
		wxPostEvent(cpuStateFrame, event);
	if (memoryViewerFrame != nullptr && memoryViewerFrame->IsShown())
		wxPostEvent(memoryViewerFrame, event);
	if (paletteViewerFrame != nullptr && paletteViewerFrame->IsShown())
		wxPostEvent(paletteViewerFrame, event);
	if (tileViewerFrame != nullptr && tileViewerFrame->IsShown())
		wxPostEvent(tileViewerFrame, event);
	if (oamViewerFrame != nullptr && oamViewerFrame->IsShown())
		wxPostEvent(oamViewerFrame, event);
	if (backgroundViewerFrame != nullptr && backgroundViewerFrame->IsShown())
		wxPostEvent(backgroundViewerFrame, event);
}

void App::sendCartridgeLoadedEvent(){
	if (!runningEmulator)
		return;

	// Create an event.
	wxCommandEvent event(ON_CARTRIDGE_LOADED_EVENT);


	// Send the event to the child windows.
	wxPostEvent(this, event);
	if (paletteViewerFrame != nullptr)
		wxPostEvent(paletteViewerFrame, event);
}

void App::showCPUStateFrame()
{
	if (cpuStateFrame != nullptr)
		cpuStateFrame->Show(true);
}
void App::showMemoryViewerFrame()
{
	if (memoryViewerFrame != nullptr)
		memoryViewerFrame->Show(true);
}
void App::showPaletteViewerFrame()
{
	if (paletteViewerFrame != nullptr)
		paletteViewerFrame->Show(true);
}
void App::showTileViewerFrame()
{
	if (tileViewerFrame != nullptr)
		tileViewerFrame->Show(true);
}
void App::showOAMViewerFrame()
{
	if (oamViewerFrame != nullptr)
		oamViewerFrame->Show(true);
}
void App::showBackgroundViewerFrame()
{
	if (backgroundViewerFrame != nullptr)
		backgroundViewerFrame->Show(true);
}