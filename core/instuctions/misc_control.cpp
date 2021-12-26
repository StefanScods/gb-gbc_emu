/*
a implementaion file for the misc/control instuctions for the gb/gbc cpu

date: 2021-11-13
*/

#include "include\misc_control.h"
#include "..\include\cpu.h"
#include <iostream>

cycles MiscAndControl::notSupported(CPU* cpu){
    std::cout<< "ERROR: INSTUCTION NOT SUPPORTED!" << std::endl;
    return NOP_CYCLES;
}

cycles MiscAndControl::nop(CPU* cpu){
    return NOP_CYCLES;
}

//!!! IMPLEMENT THIS AHHHHHHHHHHHHHHHHHHHHH
cycles MiscAndControl::stop(CPU* cpu){
    notSupported(cpu);

    return STOP_CYCLES;
}

cycles MiscAndControl::di(CPU* cpu){
    cpu->ime = false;

    return DI_CYCLES;
}

cycles MiscAndControl::ei(CPU* cpu){
    cpu->ime = true;

    return EI_CYCLES;
}
