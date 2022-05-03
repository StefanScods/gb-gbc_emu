#include "include\core.h"
#include "include\cpu.h"
#include "include\memory.h"
#include "include\register.h"

#include <iostream>

#include "..\GUI\include\screen.h"



Core::Core(int mode) {

    cpu.init();
    memory.init();
    cpu.bindMemory(&memory);

    memory.write(0x100, INC_B);
    memory.write(0x101, DEC_B);

    cpu.fetchAndExecute();
    cpu.fetchAndExecute();
    cpu.outputState();


    loadSuccess = screen.init();


    //application stuffs 
    while (loadSuccess && screen.running) {
        //start the frame counter 
        frameStart = SDL_GetTicks();

        //all input event handler: keyboard, window and mouse 
        screen.eventHandler();
        screen.mainloop(frameStart, &cpu);
    }

}

Core::~Core() {

    if(loadSuccess) screen.exit();
    memory.destroy();
}

