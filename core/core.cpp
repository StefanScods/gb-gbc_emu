/*
The header implementation for the main emulator core.
*/
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

    //memory.write(0x100, INC_B);
    //memory.write(0x101, DEC_B);
}

Core::~Core() {
    cartridge.close();
    memory.destroy();
}

void Core::populateCpuStateBuffer(CPU_State* CPU_StateBuffer){
    // Calls the CPU's states `populateCpuStateBuffer`.
    cpu.populateCpuStateBuffer(CPU_StateBuffer);
}

void Core::updateCyclesPerFrame() {
    cyclesPerFrame = cpu.getClockSpeed() / TARGET_FPS;
}

void Core::runForFrame() {
    
    cycles cycleCounter = 0;

    //!!!  cycleCounter could be > cyclesPerFrame during last instuction 
    // CPU computation.
    while (debugState == CONTINUE && cycleCounter < cyclesPerFrame) {
        cycleCounter += cpu.fetchAndExecute();
    }
}




