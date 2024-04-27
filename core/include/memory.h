#ifndef MEMORY_H
#define MEMORY_H
/*
Memory class declaration for a GameBoy color emulator 

date: 2021-11-13
*/
#include "defines.h"
#include <fstream>
#include <set>

class Timer;
class CPU;
class IOController;
class PPU;
class Cartridge;

class Memory{
private:
    friend class PPU;
    friend class IOController;

    memoryControllerWriteFunctionTemplate memoryControllerWrite = NULL;
    memoryControllerReadFunctionTemplate memoryControllerRead = NULL;
    memoryControllerSaveToStateFunctionTemplate memoryControllerSaveToState = NULL;
    memoryControllerLoadFromStateFunctionTemplate memoryControllerLoadFromState = NULL;

    CPU* cpu = nullptr;
    IOController* ioController = nullptr;
    PPU* ppu = nullptr;
    Cartridge* cartridge = nullptr;

    // Memory map memory chunks.
    bool selectedVRAMBank = 0;
    byte* vRAMBank1 = nullptr;
    byte* vRAMBank2 = nullptr;
    std::set<int> dirtyTiles;

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
     * @param d_cartridge A pointer to the emulator's cartridge.
     */
    bool init(CPU* d_cpu, IOController* d_ioController, PPU* d_ppu, Cartridge* d_cartridge);

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

    /**
     * @brief Updated the dirty VRAM trackers depending on the passed address.
     *
     * @param address The address of affected VRAM.
     * @param d_selectedVRAMBank The bank of VRAM to target.
     */
    void updateDirtyVRAM(word address, bool d_selectedVRAMBank); 
    const std::set<int> & getDirtyTiles(){ return dirtyTiles; }
    void clearDirtyTiles(){ dirtyTiles.clear(); }

    // Accessors + Mutators -> the main data bus / memory map.
    void write(word address, byte d_data);
    const byte read(word address);
    byte* getBytePointer(word address); 

    /**
     * @brief Sets the memory controller function. To be used when updating the
     * type of cartridge.
     * 
     * @param writeFunction The memory controller found on the cartridge which handles writes.
     * @param readFunction The memory controller found on the cartridge which handles reads.
     * @param saveStateFunction Helper function to handle saving to states.
     * @param loadStateFunction Helper function to handle loading from states.
    */
    void setMemoryController(
        memoryControllerWriteFunctionTemplate writeFunction, 
        memoryControllerReadFunctionTemplate readFunction,
        memoryControllerSaveToStateFunctionTemplate saveStateFunction,
        memoryControllerLoadFromStateFunctionTemplate loadStateFunction

    ){
        memoryControllerWrite = writeFunction;
        memoryControllerRead = readFunction;
        memoryControllerSaveToState = saveStateFunction;
        memoryControllerLoadFromState = loadStateFunction;
    }

    /**
     * @brief Updates the VRAM bank currently used.
     *
     * @param vramBank The bank to switch to.
    */
    void setActiveVRAMBank(bool vramBank){selectedVRAMBank = vramBank;}
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
    /**
     * @brief Raises the joypad interrupt flag.
     */
    void raiseJoypadInterrupt(); 

    /**
     * @brief Sends an HBlank event to the IO controller.
     */
    void sendHBlankToIO();
};

#endif