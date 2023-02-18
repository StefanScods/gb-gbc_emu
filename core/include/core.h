/*
The header declaration for the main emulator core.
*/
#ifndef CORE_H
#define CORE_H

#include <wx/wxprec.h>
#include "cpu.h"
#include "memory.h"
#include "register.h"
#include "defines.h"
#include "cartridge.h"
#include "../../GUI/include/app.h"

class Core{
private:
    CPU cpu;
    Memory memory;
    Cartridge cartridge;

    int debugState = CONTINUE;

    cycles cyclesPerFrame;

    // A flag indicating a ROM is loaded.
    bool loadedROM = false;


public:
    Core(int mode);

    // Runs the emulator core for one frame.
    void runForFrame();
    // Updates the target number of cycles per frame.
    void updateCyclesPerFrame();

    /**
     * @brief Populates a CPU_State struct with the current state of 
     * the CPU. This function can be used to pipe the current state 
     * of the CPU to other parts of the emulator application or to
     * serialize the CPU for save stating.
     * 
     * @param CPU_StateBuffer The buffer to populate. 
     */
    void populateCpuStateBuffer(CPU_State* CPU_StateBuffer);

    ~Core();
};

#endif