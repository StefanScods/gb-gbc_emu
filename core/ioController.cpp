/**
 * Class implementation for a wrapper of all GameBoy Color's I/O devices / registers. 
 */
#include "include/ioController.h"
#include "include/cpu.h"
#include "include/memory.h"
#include "include/ppu.h"
#include <functional>

// todo!!! turn off GBC regs if we are not running in GBC mode.

void IOController::init(CPU* d_cpu, PPU* d_ppu, APU* d_apu){
    cpu = d_cpu;
    ppu = d_ppu;
    apu = d_apu;

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
    hdmaController.reset();
}

void IOController::cycle(bool cpuDoubleSpeed){
    DIVTimer.cycle(cpuDoubleSpeed);
    TIMATimer.cycle(cpuDoubleSpeed, std::bind(&IOController::TIMATimerOverflowLogic, this));

    dmaController.cycle(cpuDoubleSpeed);
}

void IOController::bindMemory(Memory* d_memory){
    memory = d_memory;
    dmaController.bindMemory(d_memory);
    hdmaController.bindMemory(d_memory);
    joypad.setInterruptCallback(
        std::bind(&Memory::raiseJoypadInterrupt, d_memory)
    );
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
    // Special case for wave pattern RAM.
    if(address >= WAVEPATTERNRAME_START && address <= WAVEPATTERNRAME_END)
        return apu->readRAM(address);
    // All other I/O devices.
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
        // NR10 - Channel 1 sweep.
        case 0xFF10:
            return apu->readReg(1, 0);
        // NR11 - Channel 1 length timer & duty cycle.
        case 0xFF11:
            return apu->readReg(1, 1);
        // NR12 - Channel 1 volume & envelope.
        case 0xFF12:
            return apu->readReg(1, 2);
        // NR14 - Channel 1 period high & control.
        case 0xFF14:
            return apu->readReg(1, 4);
        // NR21 - Channel 2 length timer & duty cycle.
        case 0xFF16:
            return apu->readReg(2, 1);
        // NR22 - Channel 2 volume & envelope.
        case 0xFF17:
            return apu->readReg(2, 2);
        // NR24 - Channel 2 period high & control.
        case 0xFF19:
            return apu->readReg(2, 4);
        // NR30 - Channel 3 DAC enable.
        case 0xFF1A:
            return apu->readReg(3, 0);     
        // NR32 - Channel 3 output level.
        case 0xFF1C:
            return apu->readReg(3, 2);
        // NR34 - Channel 3 period high & control.
        case 0xFF1E:
            return apu->readReg(3, 4);
        // NR42 - Channel 4 volume & envelope.
        case 0xFF21:
            return apu->readReg(4, 2);
        // NR43 - Channel 4 frequency & randomness.
        case 0xFF22:
            return apu->readReg(4, 3);
        // NR44 - Channel 4 control.
        case 0xFF23:
            return apu->readReg(4, 4);
        // NR50 - Master volume & VIN panning.
        case 0xFF24:
            return apu->readReg(5, 0);
        // NR51 - Sound panning.
        case 0xFF25:
            return apu->readReg(5, 1);
        // NR52 - Audio master control.
        case 0xFF26:
            return apu->readReg(5, 2);
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
        // WY - Window Y position.
        case 0xFF4A:
            return ppu->WY;
        // WX - X position plus 7.
        case 0xFF4B:
            return ppu->WX;
        //  KEY1 - Prepare speed switch - CGB Mode Only.
        case 0xFF4D:{
            byte result = 0b01111110;
            writeBit(result, 0, KEY1SwitchArmed);
            writeBit(result, 7, cpu->getDoubleSpeedMode());
            return result;
        }
        //  HDMA5 -  VRAM DMA length/mode/start.
        case 0xFF55:
            return hdmaController.readStatus();
        //  BCPS/BGPI (CGB Mode only): Background color palette specification / Background palette index.
        case 0xFF68:
            return ppu->readFromBCPS();
        // BCPD/BGPD (CGB Mode only): Background color palette data / Background palette data.
        case 0xFF69:
            return ppu->readFromBCPDandOCPD(false);
        // OCPS/OBPI (CGB Mode only): OBJ color palette specification / OBJ palette index.
        case 0xFF6A:
            return ppu->readFromOCPS();
        // OCPD/OBPD (CGB Mode only): OBJ color palette data / OBJ palette data.
        case 0xFF6B:
            return ppu->readFromBCPDandOCPD(true);
        // SVBK: WRAM bank.
        case 0xFF70:
            return memory->readActiveWRAMBank();
        default:
            //std::cerr << "Error: I/O Device At 0x" << std::hex << address << std::dec <<" Not Yet Supported!" << std::endl;
            return 0;
        return 0;
    }
}
void IOController::write(word address, byte data){
    // Special case for wave pattern RAM.
    if(address >= WAVEPATTERNRAME_START && address <= WAVEPATTERNRAME_END)
        return apu->writeRAM(address, data);
    // All other I/O devices.
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
        // NR10 - Channel 1 sweep.
        case 0xFF10:
            apu->writeReg(1, 0, data);
            break;
        // NR11 - Channel 1 length timer & duty cycle.
        case 0xFF11:
            apu->writeReg(1, 1, data);
            break;
        // NR12 - Channel 1 volume & envelope.
        case 0xFF12:
            apu->writeReg(1, 2, data);
            break;
        // NR13 - Channel 1 period low.
        case 0xFF13:
            apu->writeReg(1, 3, data);
            break;
        // NR14 - Channel 1 period high & control.
        case 0xFF14:
            apu->writeReg(1, 4, data);
            break;
        // NR21 - Channel 2 length timer & duty cycle.
        case 0xFF16:
            apu->writeReg(2, 1, data);
            break;
        // NR22 - Channel 2 volume & envelope.
        case 0xFF17:
            apu->writeReg(2, 2, data);
            break;
        // NR23 - Channel 2 period low.
        case 0xFF18:
            apu->writeReg(2, 3, data);
            break;
        // NR24 - Channel 2 period high & control.
        case 0xFF19:
            apu->writeReg(2, 4, data);
            break;
        // NR30 - Channel 3 DAC enable.
        case 0xFF1A:
            apu->writeReg(3, 0, data);
            break;
        // NR31 - Channel 3 length timer.
        case 0xFF1B:
            apu->writeReg(3, 1, data);
            break;      
        // NR32 - Channel 3 output level.
        case 0xFF1C:
            apu->writeReg(3, 2, data);
            break;
        // NR33 - Channel 3 period low.
        case 0xFF1D:
            apu->writeReg(3, 3, data);
            break;
        // NR34 - Channel 3 period high & control.
        case 0xFF1E:
            apu->writeReg(3, 4, data);
            break;
        // NR41 - Channel 4 length timer.
        case 0xFF20:
            apu->writeReg(4, 1, data);
            break;
        // NR42 - Channel 4 volume & envelope.
        case 0xFF21:
            apu->writeReg(4, 2, data);
            break;
        // NR43 - Channel 4 frequency & randomness.
        case 0xFF22:
            apu->writeReg(4, 3, data);
            break;
        // NR44 - Channel 4 control.
        case 0xFF23:
            apu->writeReg(4, 4, data);
            break;
        // NR50 - Master volume & VIN panning.
        case 0xFF24:
            apu->writeReg(5, 0, data);
            break;
        // NR51 - Sound panning.
        case 0xFF25:
            apu->writeReg(5, 1, data);
            break;
        // NR52 - Audio master control.
        case 0xFF26:
            apu->writeReg(5, 2, data);
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
        // WY - Window Y position.
        case 0xFF4A:
            ppu->WY = data;
            break;
        // WX - X position plus 7.
        case 0xFF4B:
            ppu->WX = data;
            break;
        //  KEY1 - Prepare speed switch - CGB Mode Only.
        case 0xFF4D:
            KEY1SwitchArmed = readBit(data, 0);
            break;
        // VBK - VRAM bank.
        case 0xFF4F:
            memory->setActiveVRAMBank(readBit(data, 0));
            break;
        //  HDMA1 - VRAM DMA source (high).
        case 0xFF51:
            hdmaController.setSourceAddr(data, true);
            break;
        //  HDMA2 - VRAM DMA source (low).   
        case 0xFF52:
            hdmaController.setSourceAddr(data, false);
            break;
        //  HDMA3 - VRAM DMA destination (high).
        case 0xFF53:
            hdmaController.setTargetAddr(data, true);
            break;
        //  HDMA4 - VRAM DMA destination (low).
        case 0xFF54:
            hdmaController.setTargetAddr(data, false);
            break;
        //  HDMA5 -  VRAM DMA length/mode/start.
        case 0xFF55:
            hdmaController.setStatus(data);
            break;
        //  BCPS/BGPI (CGB Mode only): Background color palette specification / Background palette index.
        case 0xFF68:
            ppu->writeToBCPS(data);
            break;
        // BCPD/BGPD (CGB Mode only): Background color palette data / Background palette data.
        case 0xFF69:
            ppu->writeToBCPDandOCPD(data, false);
            break;
        // OCPS/OBPI (CGB Mode only): OBJ color palette specification / OBJ palette index.
        case 0xFF6A:
             ppu->writeToOCPS(data);
            break;
        // OCPD/OBPD (CGB Mode only): OBJ color palette data / OBJ palette data.
        case 0xFF6B:
            ppu->writeToBCPDandOCPD(data, true);
            break;
        // SVBK: WRAM bank.
        case 0xFF70:
            memory->setActiveWRAMBank(data);
            break;
        default:
            //std::cerr << "Error: I/O Device At 0x" << std::hex << address << std::dec <<" Not Yet Supported!" << std::endl;
            break;
    }
}

void IOController::receiveHBlank(){
    hdmaController.transferChunk();
}

void IOController::saveToState(std::ofstream & stateFile){
    int bytesToWrite = sizeof(byte)*6 + sizeof(bool);
    bytesToWrite += sizeof(bool)*3 + sizeof(cycles) + sizeof(word)*3 + sizeof(byte); // DMA size.
    bytesToWrite += (sizeof(bool) + sizeof(cycles)*2 + sizeof(byte))*2; // Timer size.
    byte* writeBuffer = new byte[
        bytesToWrite
    ];
    byte* writeBufferStart = writeBuffer;

    // Do not save the joypad state.

    // DMA.
    dmaController.saveToState(writeBuffer);
    hdmaController.saveToState(writeBuffer);

    // Timers.
    std::memcpy(writeBuffer, &TMA, sizeof(byte)); writeBuffer+=sizeof(byte);
    std::memcpy(writeBuffer, &TAC, sizeof(byte)); writeBuffer+=sizeof(byte);
    DIVTimer.saveToState(writeBuffer);
    TIMATimer.saveToState(writeBuffer);

    // Palettes.
    std::memcpy(writeBuffer, &BGP, sizeof(byte)); writeBuffer+=sizeof(byte);
    std::memcpy(writeBuffer, &OBP0, sizeof(byte)); writeBuffer+=sizeof(byte);
    std::memcpy(writeBuffer, &OBP1, sizeof(byte)); writeBuffer+=sizeof(byte);

    // Other.
    std::memcpy(writeBuffer, &IF, sizeof(byte)); writeBuffer+=sizeof(byte);
    std::memcpy(writeBuffer, &KEY1SwitchArmed, sizeof(bool)); writeBuffer+=sizeof(bool);

    // Write out the data.
    stateFile.write((char*)writeBufferStart, bytesToWrite);
    delete[] writeBufferStart;
}

void IOController::loadFromState(std::ifstream & stateFile){
    int bytesToRead = sizeof(byte)*6 + sizeof(bool);
    bytesToRead += sizeof(bool)*3 + sizeof(cycles) + sizeof(word)*3 + sizeof(byte); // DMA size.
    bytesToRead += (sizeof(bool) + sizeof(cycles)*2 + sizeof(byte))*2; // Timer size.
    byte* readBuffer = new byte[
        bytesToRead
    ];
    byte* readBufferStart = readBuffer;
    stateFile.read((char*)readBufferStart, bytesToRead);

    // Reset control joypad on load.
    joypad.reset();

    // DMA.
    dmaController.loadFromState(readBuffer);
    hdmaController.loadFromState(readBuffer);

    // Timers.
    std::memcpy(&TMA, readBuffer, sizeof(byte)); readBuffer+=sizeof(byte);
    std::memcpy(&TAC, readBuffer, sizeof(byte)); readBuffer+=sizeof(byte);
    DIVTimer.loadFromState(readBuffer);
    TIMATimer.loadFromState(readBuffer);

    // Palettes.
    std::memcpy(&BGP, readBuffer, sizeof(byte)); readBuffer+=sizeof(byte);
    std::memcpy(&OBP0, readBuffer, sizeof(byte)); readBuffer+=sizeof(byte);
    std::memcpy(&OBP1, readBuffer, sizeof(byte)); readBuffer+=sizeof(byte);

    // Other.
    std::memcpy(&IF, readBuffer, sizeof(byte)); readBuffer+=sizeof(byte);
    std::memcpy(&KEY1SwitchArmed, readBuffer, sizeof(bool)); readBuffer+=sizeof(bool);

    delete[] readBufferStart;
}