#include "include\core.h"
#include "include\cpu.h"
#include "include\memory.h"
#include "include\register.h"

#include <iostream>

#include "..\GUI\include\screen.h"


Core::Core(int mode) {

    debugState = mode;
    memory.init();
    cpu.bindMemory(&memory);
    cpu.init();

    updateCyclesPerFrame();

    if (mode != CONTINUE) {
        cpu.toggleVerbose();
    }

    memory.write(0x100, INC_B);
    //memory.write(0x101, DEC_B);

    loadSuccess = screen.init();
}

void Core::updateCyclesPerFrame() {
    cyclesPerFrame = cpu.getClockSpeed() / TARGET_FPS;
}

void Core::run() {
    
    cycles cycleCounter = 0;

    //application stuffs 
    while (loadSuccess && screen.running) {
        //start the frame counter 
        frameStart = SDL_GetTicks();

        //all input event handler: keyboard, window and mouse 
        screen.eventHandler();


        //run cpu 
        cycleCounter = 0; 
        switch (debugState) {

        case(CONTINUE): //!!!  cycleCounter could be > cyclesPerFrame during last instuction 
            while (debugState == CONTINUE && cycleCounter < cyclesPerFrame) {
                cycleCounter += cpu.fetchAndExecute();
            }
            break;

        case(STEP):
            if (screen.cpuStepButtonHeld) {
                if (screen.cpuStepButtonCounter % stepSpeedFactor == 0) {
                    cycleCounter += cpu.fetchAndExecute();
                    if (stepSpeedFactor>1) stepSpeedFactor--;
                }
                screen.cpuStepButtonCounter++;
            }
            else {
                stepSpeedFactor = 30;
            }
            break;

        default:
            break;
        }

        
        screen.mainloop(frameStart, &cpu);
    }
}

Core::~Core() {

    if(loadSuccess) screen.exit();
    memory.destroy();
}

