#ifndef MEMORY_H
#define MEMORY_H
/*
Memory class declaration for a GameBoy color emulator 

date: 2021-11-13
*/
#include "defines.h"
#include <fstream>

class Timer;
class CPU;
class IOController;
class PPU;

class Memory{
private:
    friend class PPU;
    friend class IOController;

    CPU* cpu = nullptr;
    IOController* ioController = nullptr;
    PPU* ppu = nullptr;

    // I/O Devices;
    Timer* DIVTimer;
    Timer* TIMATimer;

    //memory map memory chunks 
    byte* romBank0 = nullptr;
    byte* romBankn = nullptr;

    bool selectedVRAMBank = 0;
    byte* vRAMBank1 = nullptr;
    byte* vRAMBank2 = nullptr;

    byte* externalRAM = nullptr;

    byte* wRAM0 = nullptr;
    byte* wRAM1 = nullptr;

    byte* spriteAttributeTable = nullptr;

    byte* hRAM = nullptr;

    byte interruptEnableRegister;
public:
    /**
     * @brief Initializes the memory object. Dynamically allocates all blocks
     * of memory. Returns a bool indicating success.
     * 
     * @param d_cpu A pointer to the emulator's CPU.
     * @param d_ioController A pointer to the emulator's I/O controller.
     * @param d_ppu A pointer to the emulator's PPU.
     */
    bool init(CPU* d_cpu, IOController* d_ioController, PPU* d_ppu);

    /**
     * @brief Sets all blocks of memory to their 
     * inital values.
     */
    void setInitalValues();
    /**
     * @brief A helper function for `setInitalValues()`.
     * This function will zero all memory used by the emulator.
     */
    void zeroAllBlocksOfMemory();
    /**
     * @brief A helper function for `setInitalValues()`.
     * This function will write the initial values into
     * VRAM.
     */
    void initializeVRAM();

    /**
     * @brief Deallocates all dynamic memory.
     */
    bool destroy();

    //accessors + mutators //-> !no error checking is done in these functions for the sake of speed  -> !!!might wanna make safe versions
    void write(word address, byte d_data);
    const byte read(word address);

    byte* getBytePointer(word address); 

    void storeROMBank(int ROMBankNumber, std::ifstream* romFile);


    // Toggle between VRAM banks.
    void setvRAMBANK(bool vRAMBankNumber){selectedVRAMBank = vRAMBankNumber;}
    bool getSelectedvRAMBANK(){return selectedVRAMBank;}

};

#endif