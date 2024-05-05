#ifndef IOCONTROLLER_H
#define IOCONTROLLER_H
/**
 * Class declaration for a wrapper of all GameBoy Color's I/O devices / registers. 
 */

#include "defines.h"
#include "timer.h"
#include "joypad.h"
#include "dmaController.h"
#include "apu.h"

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
    APU* apu = nullptr;

    // Joypad.
    Joypad joypad;

    // DMA Controller.
    DMAController dmaController;
    HDMAController hdmaController;

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

    byte divAPUCounter = 0;

    bool KEY1SwitchArmed = false;

public:
    /**
     * @brief Fetches a pointer to the joypad.
    */
    Joypad* getJoypad(){return &joypad;}

    void init(CPU* d_cpu, PPU* d_ppu, APU* d_apu);
    void bindMemory(Memory* d_memory);
    void reset();
    void cycle(bool cpuDoubleSpeed);
  
    void TIMATimerOverflowLogic();
    byte read(word address);
    void write(word address, byte data );

    byte getDivAPUCounter(){return divAPUCounter;}

    /**
     * @brief Sends an HBlank event the various IO devices..
     */
    void receiveHBlank();

    /**
     * @brief Writes the current state to a state file.
     *
     * @param stateFile The file to write to.
    */
    void saveToState(std::ofstream & stateFile);
    /**
     * @brief Load the current state from a state file.
     *
     * @param stateFile The file to load from.
    */
    void loadFromState(std::ifstream & stateFile);
};



#endif