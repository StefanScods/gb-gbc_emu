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
#include "include\cartridgeViewerFrame.h"
#include "include\breakpointManager.h"

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
	EVT_MENU(wxMenuIDs::OPEN_ROM, MainWindowFrame::OnMenuOpenROMButton)
	EVT_MENU(wxMenuIDs::CLOSE_ROM, MainWindowFrame::OnMenuCloseROMButton)
	EVT_MENU(wxMenuIDs::RESET_GAMEBOY, MainWindowFrame::OnMenuResetGameBoyButton)
	
	EVT_MENU(wxMenuIDs::OPEN_CPU_STATE_VIEW, MainWindowFrame::OnMenuOpenCPUStateViewButton)
	EVT_MENU(wxMenuIDs::OPEN_MEMORY_VIEWER_VIEW, MainWindowFrame::OnMenuOpenMemoryViewerViewButton)
	EVT_MENU(wxMenuIDs::OPEN_PALETTE_VIEWER_VIEW, MainWindowFrame::OnMenuOpenPaletteViewerViewButton)
	EVT_MENU(wxMenuIDs::OPEN_TILE_VIEWER_VIEW, MainWindowFrame::OnMenuOpenTileViewerViewButton)
	EVT_MENU(wxMenuIDs::OPEN_OAM_VIEWER_VIEW, MainWindowFrame::OnMenuOpenOAMViewerViewButton)
	EVT_MENU(wxMenuIDs::OPEN_BACKGROUND_VIEWER_VIEW, MainWindowFrame::OnMenuOpenBackgroundViewerViewButton)
	EVT_MENU(wxMenuIDs::OPEN_CARTRIDGE_VIEWER_VIEW, MainWindowFrame::OnMenuOpenCartridgeViewerViewButton)
	
	EVT_MENU(wxMenuIDs::DISPLAY_SIZE_1, MainWindowFrame::handleTimes1SizeEvent)
	EVT_MENU(wxMenuIDs::DISPLAY_SIZE_2, MainWindowFrame::handleTimes2SizeEvent)
	EVT_MENU(wxMenuIDs::DISPLAY_SIZE_3, MainWindowFrame::handleTimes3SizeEvent)
	EVT_MENU(wxMenuIDs::DISPLAY_SIZE_4, MainWindowFrame::handleTimes4SizeEvent)
	EVT_MENU(wxMenuIDs::DISPLAY_SIZE_5, MainWindowFrame::handleTimes5SizeEvent)

	EVT_MENU(wxMenuIDs::PAUSE_EMULATION_MENU, MainWindowFrame::OnMenuPauseButton)
	EVT_MENU(wxMenuIDs::CONTINUE_EMULATION_MENU, MainWindowFrame::OnMenuContinueButton)
	EVT_MENU(wxMenuIDs::STEP_FRAME_MENU, MainWindowFrame::OnMenuStepFrameButton)
	EVT_MENU(wxMenuIDs::STEP_CPU_MENU, MainWindowFrame::OnMenuStepCPUButton)
	EVT_MENU(wxMenuIDs::OPEN_BREAKPOINT_MANAGER, MainWindowFrame::OnMenuOpenBreakpointManagerViewButton)
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

wxBEGIN_EVENT_TABLE(CartridgeViewerFrame, wxFrame)
	EVT_CLOSE(CartridgeViewerFrame::OnCloseWindow)
	EVT_COMMAND(wxID_ANY, EMULATOR_CORE_UPDATE_EVENT, CartridgeViewerFrame::handleEmulatorCoreUpdateEvent)
	EVT_COMMAND(wxID_ANY, ON_CARTRIDGE_LOADED_EVENT, CartridgeViewerFrame::handleOnCartridgeLoadedEvent)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(BreakpointManagerFrame, wxFrame)
	EVT_CLOSE(BreakpointManagerFrame::OnCloseWindow)
	EVT_COMMAND(wxID_ANY, EMULATOR_CORE_UPDATE_EVENT, BreakpointManagerFrame::handleEmulatorCoreUpdateEvent)
wxEND_EVENT_TABLE()

void App::loadCartridge(std::string filepath){
	if(emuCore == nullptr){
		std::cerr << "Initialize the emulation core before opening a cartridge!" << std::endl;
		exit(1);
	}
	currentlyLoadedFilePath = "";
	emuCore->pauseEmulatorExecution();
	emuCore->acquireMutexLock();
	// Reset the emulator.
	emuCore->resetCore();
	// Attempt to load the ROM file.
	LoadCartridgeReturnCodes returnCode = emuCore->loadROM(filepath);

	// Begins running the emulator's fetch and execute loop.
	if(returnCode == SUCCESS) {
		emuCore->continueEmulatorExecution();
		currentlyLoadedFilePath = filepath;
	}

	// Send an event to the rest of the app to indicate a change of cartridge.
	sendCartridgeLoadedEvent();
	emuCore->releaseMutexLock();
}

void App::clearCurrentlyLoadedFilePath(){
	currentlyLoadedFilePath = "";
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

	// Set the app icon.
	wxIcon icon;
	icon.CopyFromBitmap(wxICON(IDI_ICON1));

	// Create the emulator core object.
	emuCore = new Core(PAUSE);

	// Create the main window frame.
	mainWindow = new MainWindowFrame(emuCore, this);
	mainWindow->SetIcon(icon);
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
	cartridgeViewerFrame = new CartridgeViewerFrame(this, emuCore, emuThread);
	cartridgeViewerFrame->Hide();
	breakpointManagerFrame = new BreakpointManagerFrame(emuCore, emuThread);
	breakpointManagerFrame->Hide();

	// Add additional render events to the SDL render loop. 
	emuThread->addAdditionalRenderEvent(
		std::bind(&TileViewerFrame::updateSDLDisplays, tileViewerFrame)
	);
	emuThread->addAdditionalRenderEvent(
		std::bind(&BackgroundViewerFrame::updateSDLDisplays, backgroundViewerFrame)
	);
	// Begin the emulation thread's execution.
	emuThread->SetPriority(wxPRIORITY_MAX);
	emuThread->Run();

	return runningEmulator;
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
	if (cartridgeViewerFrame != nullptr){
		cartridgeViewerFrame->Destroy();
		cartridgeViewerFrame = nullptr;
	}
	if (breakpointManagerFrame != nullptr){
		breakpointManagerFrame->Destroy();
		breakpointManagerFrame = nullptr;
	}
}

bool App::initializeSDL2(){
	// Initialize the main SDL2 subsystem.
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "Failed to initialize SDL2 with error: " << SDL_GetError() << std::endl;
		return false;
	}

	// Everything initialized successfully.
	return true;
}

void App::sendEmulationCoreUpdateEvent(){
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
	if (cartridgeViewerFrame != nullptr && cartridgeViewerFrame->IsShown())
		wxPostEvent(cartridgeViewerFrame, event);
	if (breakpointManagerFrame != nullptr && breakpointManagerFrame->IsShown())
		wxPostEvent(breakpointManagerFrame, event);
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
	if (cartridgeViewerFrame != nullptr)
		wxPostEvent(cartridgeViewerFrame, event);
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
void App::showCartridgeViewerFrame()
{
	if (cartridgeViewerFrame != nullptr)
		cartridgeViewerFrame->Show(true);
}
void App::showBreakpointManagerFrame()
{
	if (breakpointManagerFrame != nullptr)
		breakpointManagerFrame->Show(true);
}