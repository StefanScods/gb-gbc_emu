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

    // Arrays for holding colour data.
    byte objectColours[4*SWATCHES_PER_PALETTE*NUMBER_OF_PALETTES] = {0xFF};
    byte backgroundColours[4*SWATCHES_PER_PALETTE*NUMBER_OF_PALETTES] = {0xFF};

    // Pixel data for the entire background map.
    uint8_t* backgroundMap0 = nullptr;
    // Pixel data for each tile.
    uint8_t* tileMap = nullptr;
    // An intermediate array for fetching a single tile without a palette.
    uint8_t* nonColouredTile = nullptr;

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
     * the selected tile. This array will have a black and white
     * palette applied so no additional processing is required between
     * this output and the `SDL_memcpy()` call in the tile map viewer.
     * 
     * @param tileIndex - The tile index to retrieve. This is computed
     * using: (<TILE_ADDRESS> - TILE_DATA_START) / BYTES_PER_TILE.
     * @param vRAMBank - The bank to retrieve. This should 
     * be `1` or `0`.
     * @return `uint8_t*`
     */
    uint8_t* getTileDataWithoutPalette(int tileIndex, bool bankNumber);

    /**
     * @brief A helper function for when the I/O memory map is written too.
     * Calls one of many functions to perform I/O address specific actions.
     * 
     * @param address - The I/O address to update.
     */
    void handleIOUpdate(word address, byte data);


    // A boolean indicating that the VRAM has been updated since last rendered.
    bool dirtyVRAM = true;
    // A boolean indicating that the colour palettes has been updated since last rendered.
    bool dirtyPalettes = true;
    /**
     * @brief Performs any additional processing needed to handle changes to VRAM data.
     * This includes:
     * - Updating the tile map.
     * 
     * If `dirtyVRAM` is set to false, this function is a no-op.
     * 
     * @param CGBMode A boolean which is true if the GameBoy Colour algorithm must be used.
     */
    void handleDirtyVRAM(bool CGBMode);
     /**
     * @brief Performs any additional processing needed to handle changes to palette data.
     * 
     * If `dirtyPalettes` is set to false, this function is a no-op.
     * 
     * @param CGBMode A boolean which is true if the GameBoy Colour algorithm must be used.
     */
    void handleDirtyPalettes(bool CGBMode);
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

    /**
     * @brief A helper function for retrieving the generated background 
     * map. This returns an array of size `INT8_PER_BG_MAP`. This will already
     * have the colour palette and transformations applied such that no additional 
     * processing is needed to render the background.
     * 
     * @param mapNumber The map to return. Can be either zero or one.
     */
    uint8_t* getBackgroundMap(bool mapNumber){
        if(!mapNumber) return backgroundMap0;
    };

    /**
     * @brief Get the Palette Colour at the passed index;
     * 
     * @param objectPalette A boolean to determine which palette
     * to retrieve. if true, an object palette is returned, else 
     * a background palette.
     * @param index The index (0-7) of the desired palette to 
     * retrieve. 
     * @return byte* 
     */
    byte* getPaletteColour(boolean objectPalette, int index);
};











#endif