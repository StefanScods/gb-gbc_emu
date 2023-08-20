/*
The class implementation for the emulator's external timer.
*/

#include "include/timer.h"

void Timer::setIncrementFrequency(cycles frequency){
    clockSpeed = frequency;
}
void Timer::setRunning(bool state){
    running = state;
}

void Timer::cycle(voidFuncWithNoArguments overflowOperation){
    // Return early if not running.
    if(!running) return;
    cyclesPassed ++;

    // Increment the register at the desired frequency.
    if(cyclesPassed >= clockSpeed){
        data += 1;
        cyclesPassed = cyclesPassed % clockSpeed;
        // Perform the overflow action on overflow.
        if(data == 0 && overflowOperation!=NULL){
            overflowOperation();
        }

    }
}

void Timer::resetTimer(byte resetValue){
    // Return early if not running.
    if(!running) return;
    
    cyclesPassed = 0;
    data = resetValue;
}