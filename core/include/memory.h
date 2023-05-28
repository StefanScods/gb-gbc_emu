#ifndef MEMORY_H
#define MEMORY_H
/*
Memory class declaration for a GameBoy color emulator 

date: 2021-11-13
*/
#include "defines.h"
#include <mutex> 
#include <fstream>

class Memory{
private:
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

    byte* ioPorts = nullptr;

    byte* hRAM = nullptr;

    byte interruptEnableRegister;

    // Pixel data for each tile.
    uint8_t* tileMap = nullptr;
    // An intermediate array for applying a palate.
    uint8_t* colouredTile = nullptr;

    std::mutex mtx;
public:
    //initializes the memory class -> zeros the memory 
    bool init();

    //destroys the memory class and cleans up its data 
    bool destroy();

    //accessors + mutators //-> !no error checking is done in these functions for the sake of speed  -> !!!might wanna make safe versions
    void write(word address, byte d_data);
    const byte read(word address);

    byte* getBytePointer(word address); 

    /**
     * @brief Acquires the mutex lock which protects the memory sub-system.
     */
    void acquireMutexLock(){ mtx.lock();}
    /**
     * @brief Releases the mutex lock which protects the memory sub-system.
     */
    void releaseMutexLock(){ mtx.unlock();}

    void storeROMBank(int ROMBankNumber, std::ifstream* romFile);

    /**
     * @brief A helper function for generating tiles to render.
     * This returns a uint8_t array of size `INT8_PER_TILE` with 
     * the selected tile. This array will have the correct colour 
     * palette applied so no additional processing is required between
     * this output and the `SDL_memcpy()` call.
     * 
     * @param tileIndex - The tile index to retrieve. This is computed
     * using: (<TILE_ADDRESS> - TILE_DATA_START) / BYTES_PER_TILE.
     * @param vRAMBank - The bank to retrieve. This should 
     * be `1` or `0`.
     * @return `uint8_t*`
     */
    uint8_t* getTileWithPalette(int tileIndex, bool bankNumber);

    // A boolean indicating that the VRAM has been updated since last rendered.
    bool dirtyVRAM = true;
    /**
     * @brief Performs any additional processing needed to handle changes to VRAM data.
     * This includes:
     * - Updating the tile map.
     * 
     * If `dirtyVRAM` is set to false, this function is a no-op.
     */
    void handleDirtyVRAM();
    /**
     * @brief Helper function for `handleDirtyVRAM()`. This
     * function updates the tile map for a single tile.
     * 
     * @param tileIndex - The tile index to update. This is computed
     * using: (<TILE_ADDRESS> - TILE_DATA_START) / BYTES_PER_TILE.
     * @param vRAMBank - The bank to update. This should 
     * be `1` or `0`.
     */
    void updateTile(int tileIndex, bool vRAMBank);

    // Toggle between VRAM banks.
    void setvRAMBANK(bool vRAMBankNumber){selectedVRAMBank = vRAMBankNumber;}
    bool getSelectedvRAMBANK(){return selectedVRAMBank;}
};











#endif