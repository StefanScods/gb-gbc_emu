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

    reset();
}

void IOController::reset(){
    // Init Register Values.
    TMA = 0xFF;
    // TAC.
    write(0xFF07, 0);
    BGP = 0xFF;
    OBP0 = 0xFF;
    OBP1 = 0xFF;
    IF = 0xFF;
    KEY1SwitchArmed = false;
    // Init timers.
    TIMATimer.resetTimer(0);
    DIVTimer.resetTimer(0);
    DIVTimer.setIncrementFrequency(cpu->getClockSpeed() / 256);
    DIVTimer.setRunning(true);
    
    joypad.reset();
    dmaController.reset();
}

void IOController::cycle(bool cpuDoubleSpeed){
    DIVTimer.cycle(cpuDoubleSpeed);
    TIMATimer.cycle(cpuDoubleSpeed, std::bind(&IOController::TIMATimerOverflowLogic, this));

    dmaController.cycle(cpuDoubleSpeed);
}

void IOController::bindMemory(Memory* d_memory){
    memory = d_memory;
    dmaController.bindMemory(d_memory);
}

void IOController::TIMATimerOverflowLogic(){
    // Populate the timer with the TMA-Timer modulo value.
    TIMATimer.resetTimer(memory->read(0xFF06));
    // Raise a timer interrupt by setting bit two of the IF register.
    byte value = memory->read(INTERRUPT_FLAG_REGISTER_ADDR);
    writeBit(value, 2, true);
    memory->write(INTERRUPT_FLAG_REGISTER_ADDR, value);
}

byte IOController::read(word address){
    switch(address){
        // P1/JOYP: Joypad (R/W).
        case 0xFF00:
            return joypad.read();
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
        // IF - Interrupt Flag
        case 0xFF0F:
            return IF;
        // LCDC: LCD control
        case 0xFF40:
            return ppu->LCDC;  
        // STAT: LCD status.
        case 0xFF41:
            return ppu->STAT;  
        // SCY: Viewport Y.
        case 0xFF42:
            return ppu->SCY;
        // SCX: Viewport X.
        case 0xFF43:
            return ppu->SCX;
        // LY: LCD Y coordinate [read-only].
        case 0xFF44:
            return ppu->scanline;
        // LYC: LY compare.
        case 0xFF45:
            return ppu->LYC;
        // BGP - BG Palette Data (R/W) - Non CGB Mode Only.
        case 0xFF47:
            return BGP ;
        // OBP0 - Object Palette 0 Data (R/W) - Non CGB Mode Only.
        case 0xFF48:
            return OBP0;
        // OBP1 - Object Palette 1 Data (R/W) - Non CGB Mode Only.
        case 0xFF49:
            return OBP1;
        //  KEY1 - Prepare speed switch - CGB Mode Only.
        case 0xFF4D:{
            byte result = 0b01111110;
            writeBit(result, 0, KEY1SwitchArmed);
            writeBit(result, 7, cpu->getDoubleSpeedMode());
            return result;
        }
        default:
            //std::cerr << "Error: I/O Device At 0x" << std::hex << address << std::dec <<" Not Yet Supported!" << std::endl;
            return 0;
        return 0;
    }
}
void IOController::write(word address, byte data){
    switch(address){
        // P1/JOYP: Joypad (R/W).
        case 0xFF00:
            return joypad.write(data);
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
            TAC = data | 0b11111000;
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

            TIMATimer.setIncrementFrequency(clockSpeedFactor); 
            break;}
        // IF - Interrupt Flag
        case 0xFF0F:
            IF = data;
            break;
        // LCDC: LCD control
        case 0xFF40:
            ppu->writeToLCDC(data); 
            break;
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
        // DMA: OAM DMA source address & start.
        case 0xFF46: {
            word targetAddress = ((word) data) << 8;
            dmaController.startTransfer(targetAddress);
            break;}
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
        //  KEY1 - Prepare speed switch - CGB Mode Only.
        case 0xFF4D:
            KEY1SwitchArmed = readBit(data, 0);
            break;
        default:
            //std::cerr << "Error: I/O Device At 0x" << std::hex << address << std::dec <<" Not Yet Supported!" << std::endl;
            break;
    }
}