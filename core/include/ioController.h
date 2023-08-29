#ifndef IOCONTROLLER_H
#define IOCONTROLLER_H
/**
 * Class declaration for a wrapper of all GameBoy Color's I/O devices / registers. 
 */

#include "defines.h"
#include "timer.h"
#include "joypad.h"

class CPU;
class PPU;
class Memory;

class IOController{
private:
    friend class Memory;
    friend class PPU;

    CPU* cpu = nullptr;
    PPU* ppu = nullptr;
    Memory* memory = nullptr;

    // Joypad.
    Joypad joypad;

    // Timers.
    Timer DIVTimer;
    Timer TIMATimer;
    byte TMA = 0;
    byte TAC = 0;

    // Palettes.
    byte BGP = 0;
    byte OBP0 = 0;
    byte OBP1 = 0;

    byte IF = 0xFF;

public:

    void init(CPU* d_cpu, PPU* d_ppu);
    void bindMemory(Memory* d_memory);
    void start();
    void cycle();
  
    void TIMATimerOverflowLogic();
    byte read(word address);
    void write(word address, byte data );
};



#endif