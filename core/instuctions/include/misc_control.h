#ifndef MISCCONTROL_H
#define MISCCONTROL_H

/*
a header file for the misc/control instuctions for the gb/gbc cpu

date: 2021-11-13
*/

#include "..\..\include\defines.h"

class CPU; //forward declaration

class MiscAndControl{
    
public:
    //a temp function for not supported opperations 
    cycles notSupported(CPU* cpu);

    /*
    NOP
    No Operation.
    Cycles: 1
    Bytes: 1
    Flags: None affected.*/
    cycles nop(CPU* cpu);

    /*
    STOP
    Enter CPU very low power mode. Also used to switch between double and normal speed CPU modes in GBC.
    Cycles: -
    Bytes: 2
    Flags: None affected.*/
    cycles stop(CPU* cpu);

    /*DI
    Disable Interrupts by clearing the IME flag.
    Cycles: 1
    Bytes: 1
    Flags: None affected.*/
    cycles di(CPU* cpu);

    /*EI
    Enable Interrupts by setting the IME flag. The flag is only set after the instruction following EI.
    Cycles: 1
    Bytes: 1
    Flags: None affected.*/
    cycles ei(CPU* cpu);

};





#endif 