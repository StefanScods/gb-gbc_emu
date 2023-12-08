/*
The header declaration for the emulator's external timer.
*/
#ifndef TIMER_H
#define TIMER_H

#include "defines.h"
#include "sdlController.h"

class Timer {
private:
    byte data = 0xFF;
    cycles clockSpeed = 0;
    bool running = false;

    cycles cyclesPassed = 0;

public:

    // Accessor functions.
    byte read() {return data;}
    // Mutator functions.
    void setIncrementFrequency(cycles frequency);
    void setRunning(bool state);
    void resetTimer(byte resetValue);
    void cycle(bool cpuDoubleSpeed, voidFuncWithNoArguments overflowOperation = NULL);
};

#endif