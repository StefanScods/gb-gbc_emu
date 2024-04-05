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

void Timer::cycle(bool cpuDoubleSpeed, voidFuncWithNoArguments overflowOperation){
    // Return early if not running.
    if(!running || clockSpeed == 0) return;
    cyclesPassed ++;
    if(cpuDoubleSpeed) cyclesPassed ++;

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
    cyclesPassed = 0;
    data = resetValue;
}

void Timer::saveToState(byte*& writeBuffer){
    std::memcpy(writeBuffer, &data, sizeof(byte)); writeBuffer+=sizeof(byte);
    std::memcpy(writeBuffer, &clockSpeed, sizeof(cycles)); writeBuffer+=sizeof(cycles);
    std::memcpy(writeBuffer, &cyclesPassed, sizeof(cycles)); writeBuffer+=sizeof(cycles);
    std::memcpy(writeBuffer, &running, sizeof(bool)); writeBuffer+=sizeof(bool);
}

void Timer::loadFromState(byte*& readBuffer){
    std::memcpy(&data, readBuffer, sizeof(byte)); readBuffer+=sizeof(byte);
    std::memcpy(&clockSpeed, readBuffer, sizeof(cycles)); readBuffer+=sizeof(cycles);
    std::memcpy(&cyclesPassed, readBuffer, sizeof(cycles)); readBuffer+=sizeof(cycles);
    std::memcpy(&running, readBuffer, sizeof(bool)); readBuffer+=sizeof(bool);
}