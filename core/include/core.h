#ifndef CORE_H
#define CORE_H
/*
the main emulator application core 

date: 2022-05-01
*/

#include <wx/wxprec.h>
#include "cpu.h"
#include "memory.h"
#include "register.h"
#include "defines.h"
#include "cartridge.h"
#include "../../GUI/include/app.h"
#include <chrono>

class Core {
private:

    CPU cpu;
    Memory memory;
    Cartridge cartridge;

    int debugState = CONTINUE;

    cycles cyclesPerFrame;
    long long targetFrameTime = FRAME_DELAY;

    bool loadedROM = false;


    //fps vars 
    long long frameTime;
    std::chrono::steady_clock::time_point frameStartTimer;
    std::chrono::steady_clock::time_point frameEndTimer;



public:

    //frontend access vars
    long long outputFrameTime;


    Core(int mode);
    //runs the emulator core for one frame
    void runForFrame();
    //updates the target number of cycles per frame 
    void updateCyclesPerFrame();
    ~Core();

};



#endif