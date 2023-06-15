/**
 * The header declaration for the PaletteViewerFrame class.
 */
#ifndef PALETTEVIEWERFRAME_H
#define PALETTEVIEWERFRAME_H

#include <wx/wxprec.h>
#include "wx/thread.h"
#include "wx/dynarray.h"
#include "wx/numdlg.h"
#include "wx/progdlg.h"
#include "wx/rawbmp.h"
#include "wx/sizer.h"
#include "wx/gbsizer.h"
#include "../../core/include/defines.h"
#include <string>

class Core;
class EmulationThread;

/**
 * The main window GUI for displaying the palettes loaded within VRAM.
 */
class PaletteViewerFrame : public wxFrame{
public:
	PaletteViewerFrame(Core *d_emuCore, EmulationThread* d_emuThread);
	~PaletteViewerFrame();

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

	/**
	 * @brief Generates all the wxWidget Elements for a group of swatches.
	 * 
	 * @param parentPanel The parent panel all elements will be a child of.
	 * @param parentSizer The Sizer which controls the position and size of all
	 * newly created elements.
	 * @param paletteTitle The string title to be displayed above all the palettes.
	 * @param paletteText The string label prefix which indicates which palette type
	 * this is. 
	 * @param labelElements An array to store all palette labels.
	 * @param swatchElements An array to store all swatch panel elements.
	 */
	void initSwatchDisplay(
		wxPanel* parentPanel, 
		wxGridBagSizer* parentSizer,
		std::string paletteTitle, 
		std::string paletteText, 
		wxStaticText** labelElements,
		wxPanel** swatchElements
	);

private:
	wxDECLARE_EVENT_TABLE();

    Core *emuCore = nullptr;
	EmulationThread* emuThread = nullptr;

	const int PALETTE_SWATCH_SIZE = 32;

	// wxWidget Elements.
	wxStaticText* objectPaletteLabels[NUMBER_OF_PALETTES] = { nullptr };
	wxPanel* objectPaletteSwatches[NUMBER_OF_PALETTES*SWATCHES_PER_PALETTE] = { nullptr };
	wxStaticText* backgroundPaletteLabels[NUMBER_OF_PALETTES] = { nullptr };
	wxPanel* backgroundPaletteSwatches[NUMBER_OF_PALETTES*SWATCHES_PER_PALETTE] = { nullptr };
};

#endif