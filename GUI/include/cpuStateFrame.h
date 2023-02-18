/**
 * The header declaration for the CPUStateFrame class.
 */
#ifndef CPUSTATEFRAME_H
#define CPUSTATEFRAME_H

#include <wx/wxprec.h>
#include "wx/thread.h"
#include "wx/dynarray.h"
#include "wx/numdlg.h"
#include "wx/progdlg.h"
#include "wx/rawbmp.h"
#include "wx/grid.h"

class Core;
class EmulationThread;

/**
 * The main window GUI for displaying the state of the emulated CPU.
 */
class CPUStateFrame : public wxFrame
{
public:
	CPUStateFrame(Core *d_emuCore, EmulationThread* d_emuThread);
	~CPUStateFrame();

	/**
	 * @brief The WxWidget's OnCloseWindow Event handler.
	 */
	void OnCloseWindow(wxCloseEvent& event);

	/**
	 * @brief The event handler for the `EMULATOR_CORE_UPDATE_EVENT` wxWidget event.
	 */
	void handleEmulatorCoreUpdateEvent(wxCommandEvent& event);


private:
	wxDECLARE_EVENT_TABLE();

	Core *emuCore = nullptr;
	EmulationThread* emuThread = nullptr;

	// WxWidget elements.
	wxGrid* registerViewGrid = nullptr;
	wxGrid* flagViewGrid = nullptr;
};

#endif