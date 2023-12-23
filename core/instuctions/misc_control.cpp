/*
An implementation file for the misc/control instuctions for the gb/gbc cpu

date: 2021-11-13
*/

#include "include\misc_control.h"
#include "..\include\cpu.h"
#include <iostream>

cycles MiscAndControl::notSupported(CPU* cpu){
    std::cout<< "ERROR: INSTUCTION NOT SUPPORTED!" << std::endl;
    cpu->printCurrentState();
    return NOP_CYCLES;
}

cycles MiscAndControl::nop(CPU* cpu){
    return NOP_CYCLES;
}

//!!! IMPLEMENT THIS AHHHHHHHHHHHHHHHHHHHHH
cycles MiscAndControl::stop(CPU* cpu){
    std::cout << "STOP" << std::endl;
    bool KEY1SwitchArmed = readBit(cpu->memory->read(0xFF4D), 0);
    if(KEY1SwitchArmed) cpu->toggleDoubleSpeedMode();
    else cpu->setLowPowerMode(true);
    return STOP_CYCLES;
}

cycles MiscAndControl::halt(CPU* cpu){
    cpu->setLowPowerMode(true);
    cpu->setDoubleReadBug(true);

    return HALT_CYCLES;
}

cycles MiscAndControl::di(CPU* cpu){
    cpu->disableInterrupts();

    return DI_CYCLES;
}

cycles MiscAndControl::ei(CPU* cpu){
    cpu->enableInterrupts();

    return EI_CYCLES;
}
