#ifndef SCREEN_H
#define SCREEN_H
/*
frame and application class delcarations for a gameboy color emulator
->the physical application -> all gui features 

date: 2022-01-06
*/

#include <wx/wxprec.h>
#include "wx/thread.h"
#include "wx/dynarray.h"
#include "wx/numdlg.h"
#include "wx/progdlg.h"

#include <string>
#include "app.h"
#include "..\..\core\include\defines.h"

class Core;
class EmulationThread;




class Frame : public wxFrame {
public:
	Frame(Core* emuCore);
	~Frame();

	void OnUpdateDisplayEvent(wxCommandEvent& event);

private:
	wxPanel* fpsPanel = nullptr;
	wxStaticText* fpsText = nullptr;
	Core* emuCore = nullptr;
	wxDECLARE_EVENT_TABLE();
};


//wxWidget Application class 
class App : public wxApp{
public:
	virtual bool OnInit();
	virtual int OnExit();

	void SendUpdateDisplayEvent();
	void OnUpdateDisplayEvent(wxCommandEvent& event);

	bool getRunningEmulationState() { return runningEmulator; }
	void setRunningEmulationState(bool state) { runningEmulator = state; }

private:

	bool runningEmulator = false;

	EmulationThread* emuThread = nullptr;
	Core* emuCore = nullptr;
	Frame* frame1 = nullptr;
};

class EmulationThread : public wxThread
{
public:
	EmulationThread(App* d_appContext, Core* d_emuCore);
	virtual ~EmulationThread();
	// thread execution starts here
	virtual void* Entry();

public:
	App* appContext = nullptr;
	Core* emuCore = nullptr;
};

/*
class Screen {


private:
	//Main window
	SDL_Window* main_window;
	//The surface contained by the window
	SDL_Surface* mainWindowSurface = NULL;
	SDL_Renderer* mainWindowRenderer;
	int mainWindowID;

	//window title
	std::string title = "red panda's are pretty cool imo ";
	
	//size vars for the mainWindow 
	int screen_width = SCREEN_WIDTH;
	int screen_height = SCREEN_HEIGHT;

	//controls vSync
	bool enableVsync = false;

	//Event handler
	SDL_Event event;

	//smaller screen used to display the internal cpu state 
	SDL_Window* cpuStateWindow;
	SDL_Surface* cpuStateSurface = NULL;
	SDL_Renderer* cpuStateRenderer;
	int cpuStateWindowID;

	//cpustate vars to help with rendering 
	unsigned int cpuStateScreenWidth; //determined by font size to best fit the cpu state info 
	unsigned int cpuStateScreenLineHeight;
	unsigned int regValueSpacing;

	//smaller screen used to display the cartridge details  
	SDL_Window* cartridgeDetailsWindow;
	SDL_Surface* cartridgeDetailsSurface = NULL;
	SDL_Renderer* cartridgeDetailsRenderer;
	int cartridgeDetailsWindowID;


	//cartridge details vars to help with rendering 
	unsigned int cartridgeDetailsScreenWidth;//determined by font size to best fit the cpu state info 
	unsigned int cartridgeDetailsScreenLineHeight;

	//keycodes
	SDL_Keycode showCPUStateScreenKey = SDLK_F11;
	SDL_Keycode showCartridgeDetailsKey = SDLK_F10;
	SDL_Keycode stepCPUKey = SDLK_F1;

	//button logic vars 
	int showCPUStateButton = 0;
	int showCartridgeDetailsButton = 0;

	//fps vars
	Uint32 frameTime;

	//fonts
	TTF_Font* systemFontTitle = NULL;
	TTF_Font* systemFontMain = NULL;

	Rainbow colour;

public:

	//controls main loop
	bool running = false;

	bool cpuStepButtonHeld = false;

	bool showCPUState = false;
	bool showCartridgeDetails = false;

	uint16_t cpuStepButtonCounter = 0;

	//constuctor -> calls init
	//Screen();

	//initalization routine
	bool init();
	//exit routine
	void exit();

	//main graphics and ui routine 
	void mainloop(Uint32& frameStart, CPU* cpu, Cartridge* cartridge);

	//runs any logic directly involved in the application interface 
	void uiLogic();

	//populates the cpuStateSurface with graphics 
	void renderCpuState(CPU* cpu);

	//populates the cartridgeDetailsSureface with graphics 
	void renderCartridgeDetails(Cartridge* cartridge);

	//handles all input events
	void eventHandler();
};

//helper function to draw text onto a surface -> returns the width of the rendered text 
 unsigned int drawText(SDL_Surface* surface, const char* text, TTF_Font* font, SDL_Color& colour, int x, int y);
 */
#endif