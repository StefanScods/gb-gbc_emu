#include "GUI\include\app.h"
#include <wx/wx.h>

// Implement wxWidget Application.
wxIMPLEMENT_APP_NO_MAIN(App);
int main(int argc, char** argv)
{
	return wxEntry(argc, argv);
}




// 	wxPanel* panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

// 	m_pMenuBar = new wxMenuBar();
// 	// File Menu
// 	m_pFileMenu = new wxMenu();
// 	m_pFileMenu->Append(wxID_OPEN, _T("&Open"));
// 	m_pFileMenu->Append(wxID_SAVE, _T("&Save"));
// 	m_pFileMenu->AppendSeparator();
// 	m_pFileMenu->Append(wxID_EXIT, _T("&Quit"));
// 	m_pMenuBar->Append(m_pFileMenu, _T("&File"));
// 	// About menu
// 	m_pHelpMenu = new wxMenu();
// 	m_pHelpMenu->Append(wxID_ABOUT, _T("&About"));
// 	m_pMenuBar->Append(m_pHelpMenu, _T("&Help"));

// 	SetMenuBar(m_pMenuBar);