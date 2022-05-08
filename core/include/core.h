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

    bool loadSuccess = false;

    //fps vars
    Uint32 frameStart;

public:

    Core(int mode);
    ~Core();

};



#endif