/**
 * The header declaration for the CartridgeViewerFrame class.
 */
#ifndef CARTRIDGEVIEWERFRAME_H
#define CARTRIDGEVIEWERFRAME_H

#include <wx/wxprec.h>
#include "wx/thread.h"
#include "wx/dynarray.h"
#include "wx/numdlg.h"
#include "wx/progdlg.h"
#include "wx/rawbmp.h"
#include "wx/sizer.h"
#include "../../core/include/defines.h"

class Core;
class EmulationThread;
class App;

#define TABLE_ENTRIES 11

/**
 * The main window GUI for displaying info about the currently loaded cartridge.
 */
class CartridgeViewerFrame : public wxFrame{
public:
	CartridgeViewerFrame(App *d_appContextCore, Core* d_emuCore, EmulationThread* d_emuThread);
	~CartridgeViewerFrame();

    /**
	 * @brief The WxWidget's OnCloseWindow Event handler.
	 */
	void OnCloseWindow(wxCloseEvent& event);
    
	/**
	 * @brief The event handler for the `EMULATOR_CORE_UPDATE_EVENT` wxWidget event.
	 */
	void handleEmulatorCoreUpdateEvent(wxCommandEvent& event);

    /**
	 * @brief The event handler for the `ON_CARTRIDGE_LOADED_EVENT` wxWidget event.
	 */
	void handleOnCartridgeLoadedEvent(wxCommandEvent& event);

    void changeVisiblityOfSizer(wxSizer* sizer, bool hide);


private:
	wxDECLARE_EVENT_TABLE();

    Core *emuCore = nullptr;
    App *appContext = nullptr;
	EmulationThread* emuThread = nullptr;

    // WxWidget Elements.
    wxStaticText* tableEntries[TABLE_ENTRIES];
    wxBoxSizer* tableEntrySizers[TABLE_ENTRIES];
    wxPanel* tableEntryPanels[TABLE_ENTRIES];
    wxBoxSizer* romInfoGridSizer = nullptr; 
    wxBoxSizer* noROMSizer = nullptr; 
    wxStaticText* noROMMessage = nullptr;
};

#endif