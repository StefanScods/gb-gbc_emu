/**
 * Class implementation for a wrapper of all GameBoy Color's I/O devices / registers. 
 */
#include "include/ioController.h"
#include "include/cpu.h"
#include "include/memory.h"
#include "include/ppu.h"
#include <functional>

void IOController::init(CPU* d_cpu, PPU* d_ppu){
    cpu = d_cpu;
    ppu = d_ppu;
    // Create the DIV timer.
    DIVTimer.setIncrementFrequency(cpu->getClockSpeed() / 256);
}

void IOController::start(){
    DIVTimer.setRunning(true);
}

void IOController::cycle(){
    DIVTimer.cycle();
    TIMATimer.cycle(std::bind(&IOController::TIMATimerOverflowLogic, this));
}

void IOController::bindMemory(Memory* d_memory){
    memory = d_memory;
}

void IOController::TIMATimerOverflowLogic(){
    // Populate the timer with the TMA-Timer modulo value.
    TIMATimer.resetTimer(memory->read(0xFF06));
    // Raise a timer interrupt by setting bit two of the IF register.
    byte value = memory->read(0xFF0F);
    writeBit(value, 2, true);
    memory->write(0xFF0F, value);
}

byte IOController::read(word address){
    switch(address){
        // DIV: Divider register.
        case 0xFF04:
            return DIVTimer.read();
        // TIMA: Timer counter.
        case 0xFF05:
           return TIMATimer.read();
         // TMA: Timer modulo.
        case 0xFF06:
            return TMA;
        // TAC: Timer control
        case 0xFF07:
            return TAC;
        // STAT: LCD status.
        case 0xFF41:
            //return ppu->STAT;  
        // SCY: Viewport Y.
        case 0xFF42:
            return ppu->SCY;
        // SCX: Viewport X.
        case 0xFF43:
            return ppu->SCX;
        // LY: LCD Y coordinate [read-only].
        case 0xFF44:
            //return ppu->scanline;
        // LYC: LY compare.
        case 0xFF45:
            return ppu->LYC;
            break;
        // BGP - BG Palette Data (R/W) - Non CGB Mode Only.
        case 0xFF47:
            return BGP ;
        // OBP0 - Object Palette 0 Data (R/W) - Non CGB Mode Only.
        case 0xFF48:
            return OBP0;
        // OBP1 - Object Palette 1 Data (R/W) - Non CGB Mode Only.
        case 0xFF49:
            return OBP1;
        default:
            //std::cerr << "Error: I/O Device At 0x" << std::hex << address << std::dec <<" Not Yet Supported!" << std::endl;
            return 0;
        return 0;
    }
}
void IOController::write(word address, byte data){
    switch(address){
        // DIV: Divider register.
        case 0xFF04:
            DIVTimer.resetTimer(0);
            break;
        // TIMA: Timer counter.
        case 0xFF05:
           TIMATimer.resetTimer(0);
           break;
         // TMA: Timer modulo.
        case 0xFF06:
            TMA = data;
            break;
        // TAC: Timer control
        case 0xFF07: {
            TAC = data;
            // Bit 2 defines the timer enable.
            bool enable = (TAC & 0b100) >> 2;
            TIMATimer.setRunning(enable);

            // Lower 2 bits define which factor to divide the CPU clock speed.
            byte speedIndex = (TAC & 0b11);
            int clockSpeedFactor = 1;
            switch (speedIndex){
                case 0:
                    clockSpeedFactor = 1024;
                    break;
                case 1:
                    clockSpeedFactor = 16;
                    break;
                case 2:
                    clockSpeedFactor = 64;
                    break;
                case 3:
                    clockSpeedFactor = 256;
                    break;
                default:
                    break;
                }

            TIMATimer.setIncrementFrequency(cpu->getClockSpeed() / clockSpeedFactor); 
            break;}
        // STAT: LCD status.
        case 0xFF41:
            ppu->writeToSTAT(data); 
            break;
        // SCY: Viewport Y.
        case 0xFF42:
            ppu->SCY = data;
            break;
        // SCX: Viewport X.
        case 0xFF43:
            ppu->SCX = data;
            break;
        // LY: LCD Y coordinate [read-only].
        case 0xFF44:
            break;
        // LYC: LY compare.
        case 0xFF45:
            ppu->LYC = data;
            break;
        // BGP - BG Palette Data (R/W) - Non CGB Mode Only.
        case 0xFF47:
            BGP = data;
            break;
        // OBP0 - Object Palette 0 Data (R/W) - Non CGB Mode Only.
        case 0xFF48:
            OBP0 = data;
            break;
        // OBP1 - Object Palette 1 Data (R/W) - Non CGB Mode Only.
        case 0xFF49:
            OBP1 = data;
            break;
        default:
            //std::cerr << "Error: I/O Device At 0x" << std::hex << address << std::dec <<" Not Yet Supported!" << std::endl;
            break;
    }
}