/**
 * The implementation of the OAMViewerFrame class.
 */
#include "include\oamViewerFrame.h"
#include "include\emulationThread.h"
#include "..\core\include\cpu.h"
#include "..\core\include\core.h"
#include "..\core\include\memory.h"

// Enables debug cout statements for this file.
#define ENABLE_DEBUG_PRINTS false

OAMViewerFrame::OAMViewerFrame(Core *d_emuCore, EmulationThread *d_emuThread) : wxFrame(NULL, wxID_ANY, OAM_VIEWER_DISPLAY_TITLE, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER)
{
    if (ENABLE_DEBUG_PRINTS)
        std::cout << "Starting: OAM Viewer Frame" << std::endl;

    // Save passed pointers.
    emuCore = d_emuCore;
    emuThread = d_emuThread;
}

OAMViewerFrame::~OAMViewerFrame()
{
    if (ENABLE_DEBUG_PRINTS)
        std::cout << "Exiting: OAM Viewer Frame" << std::endl;
}

// Event Handlers.
void OAMViewerFrame::OnCloseWindow(wxCloseEvent &event)
{
    this->Hide();
}

void OAMViewerFrame::handleEmulatorCoreUpdateEvent(wxCommandEvent &event)
{
    // Return early if not active.
    if (!this->IsShown())
        return;
}
