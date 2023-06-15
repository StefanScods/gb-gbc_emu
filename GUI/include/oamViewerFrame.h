/**
 * The header declaration for the OAMViewerFrame class.
 */
#ifndef OAMVIEWERFRAME_H
#define OAMVIEWERFRAME_H

#include <wx/wxprec.h>
#include "wx/thread.h"
#include "wx/dynarray.h"
#include "wx/numdlg.h"
#include "wx/progdlg.h"
#include "wx/rawbmp.h"
#include "../../core/include/defines.h"

class Core;
class EmulationThread;

/**
 * The main window GUI for displaying the sprite / object attribute memory loaded within VRAM.
 */
class OAMViewerFrame : public wxFrame{
public:
	OAMViewerFrame(Core *d_emuCore, EmulationThread* d_emuThread);
	~OAMViewerFrame();

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
};

#endif