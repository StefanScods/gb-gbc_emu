#ifndef IOCONTROLLER_H
#define IOCONTROLLER_H
/**
 * Class declaration for a wrapper of all GameBoy Color's I/O devices / registers. 
 */

#include "defines.h"
#include "timer.h"
#include "joypad.h"
#include "dmaController.h"

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

    // DMA Controller.
    DMAController dmaController;

    // Timers.
    Timer DIVTimer;
    Timer TIMATimer;
    byte TMA = 0xFF;
    byte TAC = 0xFF;

    // Palettes.
    byte BGP = 0xFF;
    byte OBP0 = 0xFF;
    byte OBP1 = 0xFF;

    byte IF = 0xFF;

public:
    /**
     * @brief Fetches a pointer to the joypad.
    */
    Joypad* getJoypad(){return &joypad;}

    void init(CPU* d_cpu, PPU* d_ppu);
    void bindMemory(Memory* d_memory);
    void reset();
    void cycle(bool cpuDoubleSpeed);
  
    void TIMATimerOverflowLogic();
    byte read(word address);
    void write(word address, byte data );
};



#endif