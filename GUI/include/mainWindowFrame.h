/**
 * The header declaration for the MainWindowFrame class.
 */
#ifndef MAINWINDOWFRAME_H
#define MAINWINDOWFRAME_H

#include <wx/wxprec.h>
#include "wx/thread.h"
#include "wx/dynarray.h"
#include "wx/numdlg.h"
#include "wx/progdlg.h"
#include "wx/rawbmp.h"

class Core;
class EmulationThread;

/**
 * The main window of gameboy color emulator's GUI.
 */
class MainWindowFrame : public wxFrame
{
public:
	MainWindowFrame(Core *d_emuCore);
	~MainWindowFrame();

	/**
	 * @brief Gets the pointer to the displayPanel object.
	 * 
	 * @return wxPanel* (this.displayPanel).
	 */
	wxPanel* getDisplayPanel() { return displayPanel;}


	/**
	 * @brief The event handler for the `EMULATOR_CORE_UPDATE_EVENT` wxWidget event.
	 */
	void handleEmulatorCoreUpdateEvent(wxCommandEvent& event);

	// Simple Mutators.
	void setEmuThread(EmulationThread* d_emuThread){emuThread = d_emuThread;}

private:
	wxDECLARE_EVENT_TABLE();

	Core *emuCore = nullptr;
	EmulationThread* emuThread = nullptr;

	// WxWidget elements.

	// GB Display -> Used to generate an SDL window.
	wxPanel* displayPanel = nullptr;
};

#endif