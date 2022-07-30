/*
frame and application class class implementation for a gameboy color emulator

date: 2022-01-06
*/

#include "include\app.h"
#include "..\core\include\cpu.h"
#include "..\core\include\defines.h"
#include "..\core\include\core.h"
#include <iostream>
#include <string> 

//#include "include\rainbow.h"


wxDEFINE_EVENT(EMU_UPDATE_DISPLAY_READY, wxCommandEvent);

wxBEGIN_EVENT_TABLE(Frame, wxFrame)
	EVT_COMMAND(wxID_ANY, EMU_UPDATE_DISPLAY_READY, Frame::OnUpdateDisplayEvent)
wxEND_EVENT_TABLE()



bool App::OnInit() {
	std::cout << "Starting: " << APP_TITLE << std::endl;
	emuCore = new Core(CONTINUE);
	frame1 = new Frame(emuCore);

	emuThread = new EmulationThread(this, emuCore);
	if (emuThread->Create() != wxTHREAD_NO_ERROR) {
		wxLogError("Can't create thread!");
	}

	frame1->Show(true);
	SetTopWindow(frame1);

	//start emulation 
	runningEmulator = true;
	emuThread->Run();

	return true;
}

int App::OnExit(){
	runningEmulator = false;
	emuThread->Wait();

	delete emuThread;
	delete emuCore;

	std::cout << "Exiting: " << APP_TITLE << std::endl;
	return 0;
}

void App::SendUpdateDisplayEvent() {
	if (!runningEmulator) return;

	wxCommandEvent event(EMU_UPDATE_DISPLAY_READY);
	wxPostEvent(this, event);
	wxPostEvent(frame1, event);

}


Frame::Frame(Core* d_emuCore):wxFrame(NULL, wxID_ANY, APP_TITLE, wxDefaultPosition, wxSize(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT)){
	emuCore = d_emuCore;
	
	fpsPanel = new wxPanel(this, wxID_ANY);
	fpsText = new wxStaticText(fpsPanel, wxID_ANY, "0");
}

Frame::~Frame() {

	delete fpsText;
	delete fpsPanel;
}

EmulationThread::EmulationThread(App* d_appContext, Core* d_emuCore) :wxThread(wxTHREAD_JOINABLE) {
	std::cout << "Starting Thread" << std::endl;
	appContext = d_appContext;
	emuCore = d_emuCore;
}

EmulationThread::~EmulationThread() {
	std::cout << "Exiting Thread" << std::endl;
}

void* EmulationThread::Entry() {
	while (appContext->getRunningEmulationState()) {
		emuCore->runForFrame();
		appContext->SendUpdateDisplayEvent();
	}

	return (wxThread::ExitCode)0;
}

void Frame::OnUpdateDisplayEvent(wxCommandEvent& event) {
	fpsText->SetLabel(std::to_string(NANOSECOND_FACTOR/emuCore->outputFrameTime));
}
























/*!!! how to directly edit surface pixels 
SDL_LockSurface(cpuStateSurface);
Uint8 bytesperPixel = cpuStateSurface->format->BytesPerPixel;
Uint8* pixelArray = (Uint8*) cpuStateSurface->pixels;
int lenOfRow = cpuStateSurface->pitch;
std::cout << (int)bytesperPixel << std::endl;
for (int x = 0; x < 100;x++) {
	for (int y = 0; y < 100; y++) {
		pixelArray[y * lenOfRow + x * bytesperPixel] = 255; //BRGA blue is +0 -> alpha is +3
	}
}
SDL_UnlockSurface(cpuStateSurface);
*/



/*


bool Screen::init() {
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "ERROR: SDL2 could not initialize!: " << SDL_GetError() << std::endl;
		return false;
	}

	main_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_SHOWN);
	if (main_window == NULL) {
		std::cout << "ERROR: Window could not be created!: " << SDL_GetError() << std::endl;
		return false;
	}
	mainWindowID = SDL_GetWindowID(main_window);


	mainWindowSurface = SDL_GetWindowSurface(main_window);
	if (mainWindowSurface == NULL) {
		std::cout << "ERROR: Window surface could not be created!: " << SDL_GetError() << std::endl;
		return false;
	}

	//get hardware renderers
	mainWindowRenderer = SDL_CreateRenderer(main_window, -1, SDL_RENDERER_ACCELERATED);
	if (mainWindowRenderer == NULL) {
		std::cout << "ERROR: Renderer could not be created!: " << SDL_GetError() << std::endl;;
		return false;
	}

	running = true;
	return true;
}


void Screen::exit() {



	SDL_DestroyWindow(main_window);
	main_window = NULL;
	mainWindowSurface = NULL;

	SDL_DestroyRenderer(mainWindowRenderer);
	mainWindowRenderer = NULL;

	SDL_Quit();
}

void Screen::eventHandler() {
	//gets the current event state
	while (SDL_PollEvent(&event)) {


		switch (event.type) {

		//quit button  
		case(SDL_QUIT):
			running = false;
			break;

			//window events such as resizing 
		case (SDL_WINDOWEVENT):
			switch (event.window.event) {

			case(SDL_WINDOWEVENT_MAXIMIZED):
				break;

			case(SDL_WINDOWEVENT_RESIZED):
				break;

			case(SDL_WINDOWEVENT_CLOSE):

				if (event.window.windowID == mainWindowID) {
					running = false;
				}
				break;

			default:
				break;
			}

		case(SDL_KEYDOWN):
			break;

		case(SDL_KEYUP):
			break;

		case (SDL_MOUSEMOTION):
			break;

		default:
			break;
		}

	}
}


void Screen::mainloop(Uint32& frameStart, CPU* cpu, Cartridge* cartridge) {


	//main screen
	SDL_FillRect(mainWindowSurface, NULL, 0); //clears the surface
	SDL_UpdateWindowSurface(main_window); //update display



	//render ui
	SDL_RenderClear(mainWindowRenderer);
	SDL_RenderPresent(mainWindowRenderer);

	frameTime = SDL_GetTicks() - frameStart;

	//frame cap
	if (FRAME_DELAY > frameTime) {
		SDL_Delay(FRAME_DELAY - frameTime);
	}

	double FPS = 1000.0 / (SDL_GetTicks() - frameStart);
	SDL_SetWindowTitle(main_window, (title + std::to_string(FPS)).c_str()); //display fps on the title bar


	//debug::loudScream();
}

//std::cout << "Frame Budget:" << (SDL_GetTicks() - frameStart) * 6.0 << "%" << std::endl;


//!!! todo maybe upgrade to a hardware text rendering -> sdl_screens are purely cpu rendering 
unsigned int drawText(SDL_Surface* surface, const char* text, TTF_Font* font, SDL_Color& colour, int x, int y) {

	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, colour); //renders the text

	//specifies the position on the desination surface 
	SDL_Rect destLocation;
	destLocation.x = x;
	destLocation.y = y;
	destLocation.w = textSurface->w;
	destLocation.h = textSurface->h;

	//draws the text to the surface 
	SDL_BlitSurface(textSurface, NULL, surface, &destLocation);
	SDL_FreeSurface(textSurface);

	return destLocation.w;
}

unsigned int drawText_rightAlign(SDL_Surface* surface, const char* text, TTF_Font* font, SDL_Color& colour, int x, int y) {

	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, colour); //renders the text

	//specifies the position on the desination surface 
	SDL_Rect destLocation;
	destLocation.x = x-textSurface->w;
	destLocation.y = y;
	destLocation.w = textSurface->w;
	destLocation.h = textSurface->h;

	//draws the text to the surface 
	SDL_BlitSurface(textSurface, NULL, surface, &destLocation);
	SDL_FreeSurface(textSurface);

	return destLocation.w;
}



bool Screen::init() {
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0){
		std::cout << "ERROR: SDL2 could not initialize!: " << SDL_GetError() << std::endl;
		return false;
	}

	//loadingFonts 
	//Initialize SDL_ttf
	if (TTF_Init() == -1) {
		std::cout << "ERROR: SDL_ttf could not initialize!: " << TTF_GetError() << std::endl;
		return false;
	}

	systemFontTitle = TTF_OpenFont("GUI/resources/fonts/PressStart2P-Regular.ttf", TITLE_FONT_SIZE);
	if (systemFontTitle == NULL) {
		std::cout << "ERROR: Failed to load ttf!:" << TTF_GetError() << std::endl;
		return false;
	}
	systemFontMain = TTF_OpenFont("GUI/resources/fonts/PressStart2P-Regular.ttf", MAIN_FONT_SIZE);
	if (systemFontMain == NULL) {
		std::cout << "ERROR: Failed to load ttf!:" << TTF_GetError() << std::endl;
		return false;
	}

	//create windows

	//main screen
	main_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_SHOWN);
	if (main_window == NULL){	
		std::cout << "ERROR: Window could not be created!: " << SDL_GetError() << std::endl;
		return false;
	}	
	mainWindowID = SDL_GetWindowID(main_window);

	//cpu state screen
	//uses key words as reference to size the screen and the offsets required when rendering the text 
	SDL_Surface* textSurface = TTF_RenderText_Solid(systemFontTitle, "Registers", colour.black);
	cpuStateScreenWidth = textSurface->w+CPUSTATE_WIDTH_OFFSET;
	SDL_FreeSurface(textSurface);

	textSurface = TTF_RenderText_Solid(systemFontMain, "0x0000", colour.black);
	cpuStateScreenLineHeight = textSurface->h;
	regValueSpacing = cpuStateScreenWidth - 5 - textSurface->w;
	SDL_FreeSurface(textSurface);
	
	//creates the window for the cpu state screen 
	cpuStateWindow = SDL_CreateWindow("CPU State", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, cpuStateScreenWidth, cpuStateScreenLineHeight*13.5, SDL_WINDOW_HIDDEN);
	if (cpuStateWindow == NULL) {
		std::cout << "ERROR: Window could not be created!: " << SDL_GetError() << std::endl;
		return false;
	}
	cpuStateWindowID = SDL_GetWindowID(cpuStateWindow);

	//carttidge details screen

	//creates the window for the cartridge details screen 
	//uses key words as reference to size the screen and the offsets required when rendering the text 
	textSurface = TTF_RenderText_Solid(systemFontTitle, "Cartridge Details", colour.black);
	cartridgeDetailsScreenWidth = textSurface->w + CARTRIDGEDETAILS_WIDTH_OFFSET;
	SDL_FreeSurface(textSurface);

	textSurface = TTF_RenderText_Solid(systemFontMain, "Cartridge Details", colour.black);
	cartridgeDetailsScreenLineHeight = textSurface->h;
	SDL_FreeSurface(textSurface);

	cartridgeDetailsWindow = SDL_CreateWindow("Cartridge Details", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, cartridgeDetailsScreenWidth, cartridgeDetailsScreenLineHeight*20, SDL_WINDOW_HIDDEN);
	if (cartridgeDetailsWindow == NULL) {
		std::cout << "ERROR: Window could not be created!: " << SDL_GetError() << std::endl;
		return false;
	}
	cartridgeDetailsWindowID = SDL_GetWindowID(cartridgeDetailsWindow);

	//Get window surfaces
	mainWindowSurface = SDL_GetWindowSurface(main_window);
	if (mainWindowSurface == NULL){
		std::cout << "ERROR: Window surface could not be created!: " << SDL_GetError() << std::endl;
		return false;
	}

	cpuStateSurface = SDL_GetWindowSurface(cpuStateWindow);
	if (cpuStateSurface == NULL) {
		std::cout << "ERROR: Window surface could not be created!: " << SDL_GetError() << std::endl;
		return false;
	}

	cartridgeDetailsSurface = SDL_GetWindowSurface(cartridgeDetailsWindow);
	if (cartridgeDetailsSurface == NULL) {
		std::cout << "ERROR: Window surface could not be created!: " << SDL_GetError() << std::endl;
		return false;
	}

	//get hardware renderers
	mainWindowRenderer = SDL_CreateRenderer(main_window, -1, SDL_RENDERER_ACCELERATED);
	if (mainWindowRenderer == NULL) {
		std::cout << "ERROR: Renderer could not be created!: " << SDL_GetError() << std::endl;;
		return false;
	}
	cpuStateRenderer = SDL_CreateRenderer(cpuStateWindow, -1, SDL_RENDERER_ACCELERATED);
	if (cpuStateRenderer == NULL) {
		std::cout << "ERROR: Renderer could not be created!: " << SDL_GetError() << std::endl;
		return false;
	}
	cartridgeDetailsRenderer = SDL_CreateRenderer(cartridgeDetailsWindow, -1, SDL_RENDERER_ACCELERATED);
	if (cartridgeDetailsRenderer == NULL) {
		std::cout << "ERROR: Renderer could not be created!: " << SDL_GetError() << std::endl;
		return false;
	}

	running = true;
	return true;
}

void Screen::exit() {

	SDL_DestroyWindow(main_window);
	SDL_DestroyWindow(cpuStateWindow);
	SDL_DestroyWindow(cartridgeDetailsWindow);
	main_window = NULL;
	mainWindowSurface = NULL;
	cpuStateWindow = NULL;
	cpuStateSurface = NULL;
	cartridgeDetailsWindow = NULL;
	cartridgeDetailsSurface = NULL;

	SDL_DestroyRenderer(mainWindowRenderer);
	SDL_DestroyRenderer(cpuStateRenderer);
	SDL_DestroyRenderer(cartridgeDetailsRenderer);
	mainWindowRenderer = NULL;
	cpuStateRenderer = NULL;
	cartridgeDetailsRenderer = NULL;

	TTF_CloseFont(systemFontTitle);
	TTF_CloseFont(systemFontMain);

	//Quit SDL subsystems
	TTF_Quit();
	SDL_Quit();
}

void Screen::eventHandler() {
	//gets the current event state
	while (SDL_PollEvent(&event)) {

		switch (event.type) {

			//QUIT BUTTON

		case(SDL_QUIT):
			running = false;
			break;

			//window events such as resizing 
		case (SDL_WINDOWEVENT):
			switch (event.window.event) {

			case(SDL_WINDOWEVENT_MAXIMIZED):
				break;

			case(SDL_WINDOWEVENT_RESIZED):
				break;

			case(SDL_WINDOWEVENT_CLOSE):

				if (event.window.windowID == mainWindowID) {
					running = false;
				}
				else if (event.window.windowID == cpuStateWindowID) {
					SDL_HideWindow(cpuStateWindow);
					showCPUState = false;
				}
				else if (event.window.windowID == cartridgeDetailsWindowID) {
					SDL_HideWindow(cartridgeDetailsWindow);
					showCartridgeDetails = false;
				}
				break;

			default:
				break;
			}

		case(SDL_KEYDOWN):

			if (event.key.keysym.sym == showCPUStateScreenKey && showCPUStateButton <= FIRST_PRESSED) {
				showCPUStateButton += 1;
			}

			else if (event.key.keysym.sym == showCartridgeDetailsKey && showCartridgeDetailsButton <= FIRST_PRESSED) {
				showCartridgeDetailsButton += 1;
			}

			else if (event.key.keysym.sym == stepCPUKey) {
				cpuStepButtonHeld = true;
			}

			break;

		case(SDL_KEYUP):

			if (event.key.keysym.sym == showCPUStateScreenKey){
				showCPUStateButton = NOT_PRESSED;
			}

			else if (event.key.keysym.sym == showCartridgeDetailsKey) {
				showCartridgeDetailsButton = NOT_PRESSED;
			}

			else if (event.key.keysym.sym == stepCPUKey) {
				cpuStepButtonHeld = false;
				cpuStepButtonCounter = 0;
			}
			break;

		case (SDL_MOUSEMOTION):
			break;

		default:
			break;
		}

	}
}

void  Screen::uiLogic() {

	//toggles the cpu state window 
	if (showCPUStateButton == FIRST_PRESSED) {
		showCPUState = !showCPUState;
		showCPUState ? SDL_ShowWindow(cpuStateWindow) : SDL_HideWindow(cpuStateWindow);
		showCPUStateButton = HOLD; //stop the key from toggling again next frame until released 
	}

	//toggles the cpu state window 
	if (showCartridgeDetailsButton == FIRST_PRESSED) {
		showCartridgeDetails = !showCartridgeDetails;
		showCartridgeDetails ? SDL_ShowWindow(cartridgeDetailsWindow) : SDL_HideWindow(cartridgeDetailsWindow);
		showCartridgeDetailsButton = HOLD; //stop the key from toggling again next frame until released 
	}
}

void Screen::renderCartridgeDetails(Cartridge* cartridge) {
	//set up the hardware renderer 
	SDL_SetRenderDrawColor(cartridgeDetailsRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(cartridgeDetailsRenderer);
	SDL_FillRect(cartridgeDetailsSurface, NULL, SDL_MapRGB(cartridgeDetailsSurface->format, 255, 255, 255)); //clears the surface

	unsigned int verticalOffset = CARTRIDGEDETAILS_MARGIN;
	unsigned int temp1, titleLineLength;

	//renders register related info 
	titleLineLength = drawText(cartridgeDetailsSurface, "Cartridge Details", systemFontTitle, colour.black, CARTRIDGEDETAILS_MARGIN, verticalOffset);
	verticalOffset += cartridgeDetailsScreenLineHeight;
	temp1 = verticalOffset + 1;
	verticalOffset += 10;



	//Render screen surface 
	SDL_Texture* texture = SDL_CreateTextureFromSurface(cartridgeDetailsRenderer, cartridgeDetailsSurface);
	SDL_RenderCopy(cartridgeDetailsRenderer, texture, NULL, NULL);

	//draw some lines using the hardware renderer
	SDL_RenderDrawLine(cartridgeDetailsRenderer, CARTRIDGEDETAILS_MARGIN, temp1 + 1, CARTRIDGEDETAILS_MARGIN + titleLineLength, temp1 + 1);
	SDL_RenderDrawLine(cartridgeDetailsRenderer, CARTRIDGEDETAILS_MARGIN, temp1 + 2, CARTRIDGEDETAILS_MARGIN + titleLineLength, temp1 + 2);

	//update display
	SDL_RenderPresent(cartridgeDetailsRenderer);

	//cleanup
	SDL_DestroyTexture(texture);
}



//!!! todo maybe add code to only update the surfaces if the cpu has updated since last render 
void Screen::renderCpuState(CPU* cpu){

	word regValues[6];
	char flagValues[4];
	char bufferToRender[7];

	//get data to display 
	cpu->populateCpuStateBuffer(regValues, flagValues);
	//primes the rendering buffer 
	bufferToRender[0] = '0';
	bufferToRender[1] = 'x';
	bufferToRender[6] = '\0';

	//set up the hardware renderer 
	SDL_SetRenderDrawColor(cpuStateRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(cpuStateRenderer);

	SDL_FillRect(cpuStateSurface, NULL, SDL_MapRGB(cpuStateSurface->format, 255, 255, 255)); //clears the surface

	unsigned int verticalOffset = CPUSTATE_MARGIN;
	unsigned int temp1, temp2, temp3, flagSpacing, titleLineLength;

	//renders register related info 
	titleLineLength = drawText(cpuStateSurface, "Registers", systemFontTitle, colour.black, CPUSTATE_MARGIN, verticalOffset);
	verticalOffset += cpuStateScreenLineHeight;
	temp1 = verticalOffset+1;
	verticalOffset += 10;

	drawText(cpuStateSurface, "AF:", systemFontMain, colour.black, CPUSTATE_MARGIN, verticalOffset);
	displayHex(regValues[0], bufferToRender + 2);
	drawText(cpuStateSurface, bufferToRender, systemFontMain, colour.black, regValueSpacing, verticalOffset);
	verticalOffset += cpuStateScreenLineHeight;

	drawText(cpuStateSurface, "BC:", systemFontMain, colour.black, CPUSTATE_MARGIN, verticalOffset);
	displayHex(regValues[1], bufferToRender + 2);
	drawText(cpuStateSurface, bufferToRender, systemFontMain, colour.black, regValueSpacing, verticalOffset);
	verticalOffset += cpuStateScreenLineHeight;

	drawText(cpuStateSurface, "DE:", systemFontMain, colour.black, CPUSTATE_MARGIN, verticalOffset);
	displayHex(regValues[2], bufferToRender + 2);
	drawText(cpuStateSurface, bufferToRender, systemFontMain, colour.black, regValueSpacing, verticalOffset);
	verticalOffset += cpuStateScreenLineHeight;

	drawText(cpuStateSurface, "HL:", systemFontMain, colour.black, CPUSTATE_MARGIN, verticalOffset);
	displayHex(regValues[3], bufferToRender + 2);
	drawText(cpuStateSurface, bufferToRender, systemFontMain, colour.black, regValueSpacing, verticalOffset);
	verticalOffset += cpuStateScreenLineHeight*2;

	drawText(cpuStateSurface, "SP:", systemFontMain, colour.black, CPUSTATE_MARGIN, verticalOffset);
	displayHex(regValues[4], bufferToRender + 2);
	drawText(cpuStateSurface, bufferToRender, systemFontMain, colour.black, regValueSpacing, verticalOffset);
	verticalOffset += cpuStateScreenLineHeight;

	drawText(cpuStateSurface, "PC:", systemFontMain, colour.black, CPUSTATE_MARGIN, verticalOffset);
	displayHex(regValues[5], bufferToRender + 2);
	drawText(cpuStateSurface, bufferToRender, systemFontMain, colour.black, regValueSpacing, verticalOffset);
	verticalOffset += cpuStateScreenLineHeight*2;

	//renders flag related info 
	temp3 = drawText(cpuStateSurface, "Flags", systemFontTitle, colour.black, CPUSTATE_MARGIN, verticalOffset);
	verticalOffset += cpuStateScreenLineHeight;
	temp2 = verticalOffset+1;
	verticalOffset += 10;

	flagSpacing = drawText(cpuStateSurface, "Z", systemFontMain, colour.black, CPUSTATE_MARGIN, verticalOffset);
	flagSpacing = (cpuStateScreenWidth - 10 - flagSpacing)/3; //divide by 3 to generate horizontal offsets for the flag text 

	drawText(cpuStateSurface, "N", systemFontMain, colour.black, CPUSTATE_MARGIN + flagSpacing, verticalOffset);
	drawText(cpuStateSurface, "H", systemFontMain, colour.black, CPUSTATE_MARGIN + flagSpacing*2, verticalOffset);
	drawText(cpuStateSurface, "C", systemFontMain, colour.black, CPUSTATE_MARGIN + flagSpacing*3, verticalOffset);
	verticalOffset += cpuStateScreenLineHeight;

	bufferToRender[1] = '\0';
	bufferToRender[0] = flagValues[0];
	drawText(cpuStateSurface, bufferToRender, systemFontMain, colour.black, CPUSTATE_MARGIN, verticalOffset);
	bufferToRender[0] = flagValues[1];
	drawText(cpuStateSurface, bufferToRender, systemFontMain, colour.black, CPUSTATE_MARGIN + flagSpacing, verticalOffset);
	bufferToRender[0] = flagValues[2];
	drawText(cpuStateSurface, bufferToRender, systemFontMain, colour.black, CPUSTATE_MARGIN + flagSpacing*2, verticalOffset);
	bufferToRender[0] = flagValues[3];
	drawText(cpuStateSurface, bufferToRender, systemFontMain, colour.black, CPUSTATE_MARGIN + flagSpacing*3, verticalOffset);

	//Render screen surface 
	SDL_Texture* texture = SDL_CreateTextureFromSurface(cpuStateRenderer, cpuStateSurface);
	SDL_RenderCopy(cpuStateRenderer, texture, NULL, NULL);

	//draw some lines using the hardware renderer
	SDL_RenderDrawLine(cpuStateRenderer, CPUSTATE_MARGIN, temp1+1, CPUSTATE_MARGIN + titleLineLength, temp1+1);
	SDL_RenderDrawLine(cpuStateRenderer, CPUSTATE_MARGIN, temp1+2, CPUSTATE_MARGIN + titleLineLength, temp1+2);
	SDL_RenderDrawLine(cpuStateRenderer, CPUSTATE_MARGIN, temp2+1, temp3+5, temp2+1);
	SDL_RenderDrawLine(cpuStateRenderer, CPUSTATE_MARGIN, temp2+2, temp3+5, temp2+2);

	//update display
	SDL_RenderPresent(cpuStateRenderer);

	//cleanup
	SDL_DestroyTexture(texture);
}

void Screen::mainloop(Uint32& frameStart, CPU* cpu, Cartridge* cartridge) {


	//main screen
	SDL_FillRect(mainWindowSurface, NULL, 0); //clears the surface
	uiLogic();
	SDL_UpdateWindowSurface(main_window); //update display


	//cpu state window 
	if (showCPUState) {
		renderCpuState(cpu);
	}

	//cartridge details window 
	if (showCartridgeDetails) {
		renderCartridgeDetails(cartridge);
	}

	frameTime = SDL_GetTicks() - frameStart;

	//frame cap
	if (FRAME_DELAY > frameTime) {
		SDL_Delay(FRAME_DELAY - frameTime);
	}

	double FPS = 1000.0 / (SDL_GetTicks() - frameStart);
	SDL_SetWindowTitle(main_window, (title + std::to_string(FPS)).c_str()); //display fps on the title bar


	//debug::loudScream();
}

//std::cout << "Frame Budget:" << (SDL_GetTicks() - frameStart) * 6.0 << "%" << std::endl;*/