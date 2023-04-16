/**
 * The header declaration for the MemoryViewerFrame class.
 */
#ifndef MEMORYVEIWERFRAME_H
#define MEMORYVEIWERFRAME_H

#include <wx/wxprec.h>
#include "wx/thread.h"
#include "wx/dynarray.h"
#include "wx/numdlg.h"
#include "wx/progdlg.h"
#include "wx/rawbmp.h"
#include "wx/grid.h"
#include "wx/gbsizer.h"
#include "../../core/include/defines.h"

class Core;
class EmulationThread;

/**
 * The main window GUI for displaying the state of the different blocks of memory.
 */
class MemoryViewerFrame : public wxFrame
{
public:
	MemoryViewerFrame(Core *d_emuCore, EmulationThread* d_emuThread);
	~MemoryViewerFrame();

	/**
	 * @brief The WxWidget's OnCloseWindow Event handler.
	 */
	void OnCloseWindow(wxCloseEvent& event);

	/**
	 * @brief The event handler for the `EMULATOR_CORE_UPDATE_EVENT` wxWidget event.
	 */
	void handleEmulatorCoreUpdateEvent(wxCommandEvent& event);

	/**
	 * @brief The WxWidget's OnGridClick Event handler.
	 */
	void OnGridClick(wxGridEvent& event);

	/**
	 * @brief In order to better render the memory viewer grid,
	 * blocks of memory are given their own unique colour. This
	 * function takes in an address and returns which colour the 
	 * corrsponding cell should be. 
	 * 
	 * @param address The memory address which the cell represents.
	 * This is calculated with address =  (row * 16) + col.
	 * 
	 * @returns wxColour
	 */
	wxColour getMemoryViewerGridCellColour(word address);

private:
	wxDECLARE_EVENT_TABLE();

	// Display 0x0 -> 0xF in each row. 
    const int MEMORY_VIEW_GRID_COLS = 0xF + 1;
    // Get the number of rows needed to display each byte of memory.
    const int MEMORY_VIEW_GRID_ROWS = MEM_SIZE / MEMORY_VIEW_GRID_COLS;

	Core *emuCore = nullptr;
	EmulationThread* emuThread = nullptr;

	// The address currently selected.
	word selectedAddress = 0;
	char selectedAddressText[16] = "Address: 0x0000";

	// WxWidget elements.
	wxPanel* topControlPanel = nullptr;
    wxPanel* memoryViewPanel = nullptr;
    wxPanel* bottomControlPanel = nullptr;
	wxGrid* memoryViewGrid = nullptr;
	wxStaticText* selectedAddressTextElement = nullptr;
};

#endif
