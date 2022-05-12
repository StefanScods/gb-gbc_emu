#ifndef CORE_H
#define CORE_H
/*
the main emulator application core 

date: 2022-05-01
*/

#include "cpu.h"
#include "memory.h"
#include "register.h"
#include "defines.h"

#include "..\..\GUI\include\screen.h"


class Core {
private:

    Screen screen;
    CPU cpu;
    Memory memory;
    uint64_t frameCounter;
    uint8_t stepSpeedFactor; 

    int debugState = CONTINUE;

    cycles cyclesPerFrame;

    bool loadSuccess = false;

    //fps vars
    Uint32 frameStart;

public:

    Core(int mode);
    //runs the application 
    void run();
    //updates the target number of cycles per frame 
    void updateCyclesPerFrame();
    ~Core();

};



#endif