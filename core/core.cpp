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

// An array holding all current breakpoints. Simply add the desired value of PC to this register to pause execution.
std::vector<word> CPUBreakpoints = {
    // 0x64FD
};

Core::Core(ExecutionModes mode) {

    executionMode = mode;
    ppu.init();
    ioController.init(&cpu, &ppu);
    memory.init(&cpu, &ioController, &ppu);

    cpu.bindMemory(&memory);
    ppu.bindMemory(&memory);
    ioController.bindMemory(&memory);

    // Set CPU and Memory to default state.
    cpu.setInitalValues();
    memory.setInitalValues();

    // Start running I/O devices.
    ioController.start();
}

Core::~Core() {
    cartridge.close();
    memory.destroy();
}

bool Core::loadROM(std::string filePath){
    bool success = cartridge.open(filePath.c_str(), this);
    return success;
}

void Core::emulatorMain(){
    // Determine what the core should do depending on the executionMode.
    switch(executionMode){
        case PAUSE:
            // Do nothing on PAUSE.
            break;
        case STEP_CPU:
            runForFrame(true);
         break;
        case STEP:
        case CONTINUE:
            runForFrame(false);
            break;
        default:
            break;
    }

    // Keep the emulator paused if not in CONTINUE mode;
    if(executionMode != CONTINUE) executionMode = PAUSE;
}

void Core::populateCpuStateBuffer(CPU_State* CPU_StateBuffer){
    // Calls the CPU's states `populateCpuStateBuffer`.
    cpu.populateCpuStateBuffer(CPU_StateBuffer);
}


void Core::runForFrame(bool breakOnCPU) {
   acquireMutexLock();
    cycles cycleCounter = 0;
    while (cycleCounter < CYCLES_PER_FRAME) {
        // Run hardware.
        cycles cpuWork = cpu.cycle();
        ppu.cycle(emulatingGBColour);
        ioController.cycle(cpu.getDoubleSpeedMode());
        handleInterrupts();

        if(std::find(CPUBreakpoints.begin(),CPUBreakpoints.end(), cpu.getPC()) != CPUBreakpoints.end()){
            breakOnCPU = true;
            pauseEmulatorExecution();
        }
  
        // Stop execution if the CPU finished an instuction. 
        if(breakOnCPU && cpuWork) break;
        cycleCounter ++;
    }
    releaseMutexLock();
}

/**
 * Button Callbacks.
 */
void Core::stepNextFrameButton(){
    executionMode = STEP;
}
void Core::stepNextInstuctionButton(){
    executionMode = STEP_CPU;
}
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

void Core::handleInterrupts(){
    byte interruptEnableMask = memory.read(INTERRUPT_ENABLE_REGISTER_ADDR);
    byte interruptFlags = memory.read(INTERRUPT_FLAG_REGISTER_ADDR);

    // Calculate the interrupts that are both high and enabled.
    byte validInterrupts = interruptEnableMask & interruptFlags;

    // Turn off low power mode if there is an interrupt pending.
    if(validInterrupts != 0) cpu.setLowPowerMode(false);
    
    // Return early if the CPU disabled all interrupts.
    if(!cpu.getMasterInterruptEnabledFlag()) return;

    // V-Blank.
    if (readBit(validInterrupts, 0)) {
        // Clear the interrupt.
        writeBit(interruptFlags, 0, 0);
        memory.write(INTERRUPT_FLAG_REGISTER_ADDR, interruptFlags);
        // Force the CPU to invoke its handler.
        return cpu.setActiveInterruptHandler(0x0040);
    // LCD Stat.
    } else if  (readBit(validInterrupts, 1)) {
        // Clear the interrupt.
        writeBit(interruptFlags, 1, 0);
        memory.write(INTERRUPT_FLAG_REGISTER_ADDR, interruptFlags);
        // Force the CPU to invoke its handler.
        return cpu.setActiveInterruptHandler(0x0048);
    // Timer.
    } else if  (readBit(validInterrupts, 2)) {
        // Clear the interrupt.
        writeBit(interruptFlags, 2, 0);
        memory.write(INTERRUPT_FLAG_REGISTER_ADDR, interruptFlags);
        // Force the CPU to invoke its handler.
       return cpu.setActiveInterruptHandler(0x0050);
     // Serial.   
    } else if  (readBit(validInterrupts, 3)) {
        // Clear the interrupt.
        writeBit(interruptFlags, 3, 0);
        memory.write(INTERRUPT_FLAG_REGISTER_ADDR, interruptFlags);
        // Force the CPU to invoke its handler.
        return cpu.setActiveInterruptHandler(0x0058);
    // Joypad. 
    } else if  (readBit(validInterrupts, 4)) {
        // Clear the interrupt.
        writeBit(interruptFlags, 4, 0);
        memory.write(INTERRUPT_FLAG_REGISTER_ADDR, interruptFlags);
        // Force the CPU to invoke its handler.
        return cpu.setActiveInterruptHandler(0x0060);
    }
}