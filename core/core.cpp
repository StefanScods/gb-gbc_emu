/*
The header implementation for the main emulator core.
*/
#include "include\core.h"
#include "include\cpu.h"
#include "include\memory.h"
#include "include\register.h"
#include "include\cartridge.h"

#include <iostream>
#include <wx/wxprec.h>
#include <algorithm>

Core::Core(ExecutionModes mode) {

    executionMode = mode;
    memory.init();
    cpu.bindMemory(&memory);
    cpu.init();
    if (!cartridge.open("C:/C++/gb-gbc_emu/testroms/tetris.gb")) {
        exit(1);
    }
    updateCyclesPerFrame();
}

Core::~Core() {
    cartridge.close();
    memory.destroy();
}

void Core::emulatorMain(){
    // Determine what state the core should be in.
    if( executionMode == STEP || executionMode == STEP_CPU){
        // Return back to PAUSE after stepping the core.
        executionMode = PAUSE;
    }
    else if( executionMode == PAUSE){
        if(holdingStepNextInstuctionButton){
            holdingStepNextInstuctionDelay--;
            // If there is no delay, set execution mode to STEP_CPU.
            if(holdingStepNextInstuctionDelay <= 0){
                executionMode = STEP_CPU;
                // Determine the next delay.
                if(holdingStepNextInstuctionDelayStage <= 4){
                    holdingStepNextInstuctionDelay = (TARGET_FPS * 2) / holdingStepNextInstuctionDelayStage;
                    holdingStepNextInstuctionDelayStage++;
                }
            }
        }
        else if(holdingStepNextFrameButton){
            holdingStepNextFrameDelay--;
            // If there is no delay, set execution mode to STEP.
            if(holdingStepNextFrameDelay <= 0){
                executionMode = STEP;
                // Determine the next delay.
                if(holdingStepNextFrameDelayStage <= 4){
                    holdingStepNextFrameDelay = (TARGET_FPS * 2) / holdingStepNextFrameDelayStage;
                    holdingStepNextFrameDelayStage++;
                }
            }
        }
    }

    // Determine what the core should do depending on the executionMode.
    switch(executionMode){
        case PAUSE:
            // Do nothing on PAUSE.
            break;
        case STEP_CPU:
             // Acquire the mutex lock protecting the memory.
            memory.acquireMutexLock();
            // Advance the core by a single instuction.
            cpu.fetchAndExecute();
            // Release the mutex lock protecting the memory.
            memory.releaseMutexLock();
         break;
        case STEP:
        case CONTINUE:
            // Acquire the mutex lock protecting the memory.
            memory.acquireMutexLock();
            // For both STEP and CONTINUE, advance the core by a single frame.
            runForFrame();
            // Release the mutex lock protecting the memory.
            memory.releaseMutexLock();
            break;
        default:
            break;
    }
}


void Core::populateCpuStateBuffer(CPU_State* CPU_StateBuffer){
    // Calls the CPU's states `populateCpuStateBuffer`.
    cpu.populateCpuStateBuffer(CPU_StateBuffer);
}

void Core::updateCyclesPerFrame() {
    cyclesPerFrame = cpu.getClockSpeed() / TARGET_FPS;
}

void Core::runForFrame() {
    //!!!  cycleCounter could be > cyclesPerFrame during last instuction 
    // CPU computation.
    cycles cycleCounter = 0;
    while (cycleCounter < cyclesPerFrame) {

        cycleCounter += cpu.fetchAndExecute();

        // Breakpoint logic.
        bool hitBreakpoint = std::find(CPUBreakpoints.begin(), CPUBreakpoints.end(), cpu.getPC()) != CPUBreakpoints.end();
        // If we hit a breakpoint, pause execution and break out of the execution loop.
        if(hitBreakpoint){
            executionMode = PAUSE;
            break;
        }
    }
}

/**
 * Button Callbacks.
 */

void Core::pauseEmulatorExecution(){
    executionMode = PAUSE;
}
void Core::continueEmulatorExecution(){
    executionMode = CONTINUE;
}
void Core::toggleEmulatorExecution(){
    // Any State -> <CONTINUE>. 
    if(executionMode != CONTINUE) return continueEmulatorExecution();
    // <CONTINUE> -> <PAUSE>
    pauseEmulatorExecution();
}
void Core::keyDownStepNextFrameButton(){
    holdingStepNextFrameButton = true;
}
void Core::keyUpStepNextFrameButton(){
    holdingStepNextFrameButton = false;
    holdingStepNextFrameDelay = 0;
    holdingStepNextFrameDelayStage = 1;
}
void Core::keyDownStepNextInstuctionButton(){
    holdingStepNextInstuctionButton = true;
}
void Core::keyUpStepNextInstuctionButton(){
    holdingStepNextInstuctionButton = false;
    holdingStepNextInstuctionDelay = 0;
    holdingStepNextInstuctionDelayStage = 1;
}
