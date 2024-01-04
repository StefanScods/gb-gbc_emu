/**
 * The header declaration for the BreakpointManagerFrame class along with it's components.
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
class ScrollableBreakpointPanel;

/**
 * An entry of the list of current breakpoints.
 */
class BreakPointElement : public wxWindow {
	friend class ScrollableBreakpointPanel;

private:
	word address;
	bool enabled = true;
	Core *emuCore = nullptr;

	wxStaticText* label = nullptr;
	wxBoxSizer* parentSizer = nullptr;
	wxButton* toggleEnableBreakpointButton = nullptr;

public:
    BreakPointElement(wxWindow* parent, wxWindowID id, word d_address, wxBoxSizer* d_parentSizer, Core *d_emuCore);
	void toggleEnabled(wxCommandEvent& event);
	void remove(wxCommandEvent& event);
};


/**
 * The list of current breakpoints container.
 */
class ScrollableBreakpointPanel : public wxScrolledWindow {
private:
	wxBoxSizer* gridSizer = nullptr;
	Core *emuCore = nullptr;

public:
    ScrollableBreakpointPanel(wxWindow* parent, wxWindowID id, Core *d_emuCore);
	void addBreakpoint(word address);
	void formatScrollbar();
};


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
    ScrollableBreakpointPanel *pcBreakpointGrid = nullptr;
	wxTextCtrl* addPCBreakpointTextBox = nullptr;
};

#endif