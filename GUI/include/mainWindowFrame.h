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
class App;
class EmulationThread;

/**
 * The main window of GameBoy emulator's GUI.
 */
class MainWindowFrame : public wxFrame
{
public:
	MainWindowFrame(Core *d_emuCore, App* d_appContext);
	~MainWindowFrame();

	/**
	 * @brief The WxWidget's OnCloseWindow event handler.
	 */
	void OnCloseWindow(wxCloseEvent& event);
	/**
	 * @brief The WxWidget menu's quit button event handler.
	 */
	void OnMenuQuitButton(wxCommandEvent& event);

	/**
	 * @brief The WxWidget menu's open ROM button event handler.
	 */
	void OnMenuOpenROMButton(wxCommandEvent& event);
	/**
	 * @brief The WxWidget menu's open CPU state view button event handler.
	 */
	void OnMenuOpenCPUStateViewButton(wxCommandEvent& event);
	/**
	 * @brief The WxWidget menu's open memory viewer view button event handler.
	 */
	void OnMenuOpenMemoryViewerViewButton(wxCommandEvent& event);
	/**
	 * @brief The WxWidget menu's open palette viewer view button event handler.
	 */
	void OnMenuOpenPaletteViewerViewButton(wxCommandEvent& event);
	/**
	 * @brief The WxWidget menu's open tile viewer view button event handler.
	 */
	void OnMenuOpenTileViewerViewButton(wxCommandEvent& event);
	/**
	 * @brief The WxWidget menu's open OAM viewer view button event handler.
	 */
	void OnMenuOpenOAMViewerViewButton(wxCommandEvent& event);
	/**
	 * @brief The WxWidget menu's open background viewer view button event handler.
	 */
	void OnMenuOpenBackgroundViewerViewButton(wxCommandEvent& event);

	/**
	 * @brief The event handler for the `EMULATOR_CORE_UPDATE_EVENT` wxWidget event.
	 */
	void handleEmulatorCoreUpdateEvent(wxCommandEvent& event);

	/**
	 * @brief Helper function to resize the main display into a pixel perfect aspect ratio.
	 * 
	 * @param multiplier The resize multiplier scale.
	*/
	void pixelPerfectResizer(int multiplier);

	void handleTimes1SizeEvent(wxCommandEvent& event){pixelPerfectResizer(1);}
	void handleTimes2SizeEvent(wxCommandEvent& event){pixelPerfectResizer(2);}
	void handleTimes3SizeEvent(wxCommandEvent& event){pixelPerfectResizer(3);}
	void handleTimes4SizeEvent(wxCommandEvent& event){pixelPerfectResizer(4);}
	void handleTimes5SizeEvent(wxCommandEvent& event){pixelPerfectResizer(5);}

	/**
	 * @brief Gets the pointer to the displayPanel object.
	 * 
	 * @return wxPanel* (this.displayPanel).
	 */
	wxPanel* getDisplayPanel() { return displayPanel;}

	// Simple Mutators.
	void setEmuThread(EmulationThread* d_emuThread){emuThread = d_emuThread;}

private:
	wxDECLARE_EVENT_TABLE();

	Core *emuCore = nullptr;
	EmulationThread* emuThread = nullptr;
	App *appContext = nullptr;

	// WxWidget elements.

	// GB Display -> Used to generate an SDL window.
	wxPanel* displayPanel = nullptr;
	wxGridSizer* mainDisplaySizer = nullptr;

	// Menu bar elements.
	wxMenuBar* menuBar = nullptr;
	wxMenu* fileMenuLayout = nullptr;
	wxMenu* toolsMenuLayout = nullptr;
	wxMenu* displayMenuLayout = nullptr;
};

#endif