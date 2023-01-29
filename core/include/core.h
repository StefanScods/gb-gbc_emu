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
#include <chrono>

class Core
{
private:
    CPU cpu;
    Memory memory;
    Cartridge cartridge;

    int debugState = CONTINUE;

    cycles cyclesPerFrame;
    long long targetFrameTime = FRAME_DELAY;

    // A flag indicating a ROM is loaded.
    bool loadedROM = false;

    // Vars used in FPS logic.
    
    long long frameTime;
    std::chrono::steady_clock::time_point frameStartTimer;
    std::chrono::steady_clock::time_point frameEndTimer;

public:
    // Frontend access vars.
    long long outputFrameTime;

    Core(int mode);
    // Runs the emulator core for one frame.
    void runForFrame();
    // Updates the target number of cycles per frame.
    void updateCyclesPerFrame();
    ~Core();
};

#endif