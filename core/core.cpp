#include "include\core.h"
#include "include\cpu.h"
#include "include\memory.h"
#include "include\register.h"
#include "include\cartridge.h"

#include <iostream>
#include <wx/wxprec.h>

Core::Core(int mode) {

    debugState = mode;
    memory.init();
    cpu.bindMemory(&memory);
    cpu.init();
    if (!cartridge.open("C:/C++/gb-gbc_emu/testroms/tetris.gb")) {
        exit(1);
    }

    updateCyclesPerFrame();

    if (mode != CONTINUE) {
        cpu.toggleVerbose();
    }

    memory.write(0x100, INC_B);
    //memory.write(0x101, DEC_B);
}

void Core::updateCyclesPerFrame() {
    cyclesPerFrame = cpu.getClockSpeed() / TARGET_FPS;
}


void Core::runForFrame() {
    frameStartTimer = std::chrono::steady_clock::now();
    
    cycles cycleCounter = 0;

    //!!!  cycleCounter could be > cyclesPerFrame during last instuction 
    //cpu computation
    while (debugState == CONTINUE && cycleCounter < cyclesPerFrame) {
        cycleCounter += cpu.fetchAndExecute();
    }
 
    //fps cap
    long long sleepTime = targetFrameTime;
    frameEndTimer = std::chrono::steady_clock::now();
    frameTime = std::chrono::duration_cast<std::chrono::nanoseconds> (frameEndTimer - frameStartTimer).count();
    sleepTime = sleepTime - frameTime;
    if (sleepTime > 0) {
        //sleep if we are faster than the target fps
        debug::spinSleep(sleepTime);
    }

    //update the frame time for the frontend counter
    frameEndTimer = std::chrono::steady_clock::now();
    outputFrameTime = std::chrono::duration_cast<std::chrono::nanoseconds> (frameEndTimer - frameStartTimer).count();
}

Core::~Core() {
    cartridge.close();
    memory.destroy();
}

