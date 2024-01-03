/**
 * The header declaration for the BreakpointManagerFrame class.
 */
#ifndef BREAKPOINTMANAGERFRAME_H
#define BREAKPOINTMANAGERFRAME_H

#include <wx/wxprec.h>
#include "wx/thread.h"
#include "wx/dynarray.h"
#include "wx/numdlg.h"
#include "wx/progdlg.h"
#include "wx/rawbmp.h"
#include "wx/grid.h"
#include "../../core/include/defines.h"

class Core;
class EmulationThread;

/**
 * The main window GUI for managing the emulator's current breakpoints.
 */
class BreakpointManagerFrame : public wxFrame{
public:
	BreakpointManagerFrame(Core *d_emuCore, EmulationThread* d_emuThread);
	~BreakpointManagerFrame();

    /**
	 * @brief The WxWidget's OnCloseWindow Event handler.
	 */
	void OnCloseWindow(wxCloseEvent& event);
    
	/**
	 * @brief The event handler for the `EMULATOR_CORE_UPDATE_EVENT` wxWidget event.
	 */
	void handleEmulatorCoreUpdateEvent(wxCommandEvent& event);

	/**
	 * @brief The event handler for the add PC breakpoint button.
	 */
    void handleAddPCBreakpointButton(wxCommandEvent& event);


private:
	wxDECLARE_EVENT_TABLE();

    Core *emuCore = nullptr;
	EmulationThread* emuThread = nullptr;

    // wxWidget elements.
    wxScrolledWindow *pcBreakpointGrid = nullptr;
    wxBoxSizer* pcGridSizer = nullptr;
};

#endif