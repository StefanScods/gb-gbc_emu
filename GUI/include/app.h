/**
 * Application class declaration for a GameBoy color emulator. This class acts
 * as the parent WxWidget application.
 */
#ifndef APP_H
#define APP_H

#include <wx/wxprec.h>
#include "wx/thread.h"
#include "wx/dynarray.h"
#include "wx/numdlg.h"
#include "wx/progdlg.h"
#include "wx/grid.h"
#include <string>

// Forward declarations.
class EmulationThread;
class MainWindowFrame;
class CPUStateFrame;
class Core;
class MemoryViewerFrame;
class TileViewerFrame;

// An Enum defining the wxIDs used for each of the custom menu bar actions.
enum wxMenuIDs {
	OPEN_CPU_STATE_VIEW = 1,
	OPEN_MEMORY_VIEWER_VIEW,
	OPEN_TILE_VIEWER_VIEW
};

const char APP_TITLE[] = "RedPandaEmu";
#define MAIN_WINDOW_WIDTH 600
#define MAIN_WINDOW_HEIGHT 480
#define MAIN_WINDOW_DISPLAY_PIXEL_DEPTH 24

const char CPU_STATE_DISPLAY_TITLE[] = "CPU State";
#define CPU_STATE_DISPLAY_WIDTH 300
#define CPU_STATE_DISPLAY_HEIGHT 355

const char MEMORY_VIEWER_DISPLAY_TITLE[] = "Memory Viewer";
#define MEMORY_VIEWER_DISPLAY_WIDTH 700
#define MEMORY_VIEWER_DISPLAY_HEIGHT 500

const char TILE_VIEWER_DISPLAY_TITLE[] = "Tile Viewer";

// The parent WxWidget application.
class App : public wxApp
{
public:
	virtual bool OnInit();
	virtual int OnExit();

	//void SendUpdateDisplayEvent();
	//void OnUpdateDisplayEvent(wxCommandEvent &event);

	/**
	 * @brief Gets the current running state of the emulation core.	
	 * 
	 * @return Returns a boolean indicating whether or not the emulator
	 * is currently running.
	 */
	bool getRunningEmulationState() { return runningEmulator; }
	/**
	 * @brief Sets the current running state of the emulation core.
	 * 
	 * @param state The running state to set the emulation core to.
	 */
	void setRunningEmulationState(bool state) { runningEmulator = state; }

	/**
	 * @brief Initialize the SDL2 subsystem.
	 * 
	 * Will output error messages to std::cerr upon failure.
	 * 
	 * @return Returns the success of SDL's initialization.
	 */
	bool initializeSDL2();

	/**
	 * @brief Closes all frames except the main emulator frame.
	 */
	void closeAllSideFrames();

	/**
	 * @brief Reveals the CPU state frame.
	 */
	void showCPUStateFrame();

	/**
	 * @brief Reveals the memory viewer frame.
	 */
	void showMemoryViewerFrame();

	/**
	 * @brief Reveals the tile viewer frame.
	 */
	void showTileViewerFrame();

	/**
	 * @brief Sends an 'EMULATOR_CORE_UPDATE_EVENT' to the rest of the application. This 
	 * event should be generated by the emulator core when it wishes the rest of the GUI
	 * to update.
	 */
	void sendEmulationCoreUpdateEvent();

private:
	// A boolean indicating whether the emulator is currently running.
	bool runningEmulator = false;

	EmulationThread *emuThread = nullptr;
	MainWindowFrame *mainWindow = nullptr;
	CPUStateFrame *cpuStateFrame = nullptr;
	MemoryViewerFrame *memoryViewerFrame = nullptr;
	TileViewerFrame *tileViewerFrame = nullptr;

	Core *emuCore = nullptr;
};

#endif