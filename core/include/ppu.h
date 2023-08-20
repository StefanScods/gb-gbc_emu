#ifndef PPU_H
#define PPU_H
/**
 * Class declaration for a GameBoy Color PPU / LCD display. 
 */

#include "defines.h"
#include <vector>

class Memory;

class PPU{
private:
    friend class Memory;
    friend class IOController;

    // A pointer to memory. 
    Memory* memory = nullptr;

    // The current mode of the PPU. This controls what the PPU is doing per cycle. 
    int mode = 2;

    // Viewport.
    byte SCY = 0;
    byte SCX = 0;

    byte LYC = 0;
    byte STAT = 0;

    // The current scanline being drawn.
    int scanline = 0;

    // Arrays for holding colour data.
    byte objectColours[4*SWATCHES_PER_PALETTE*NUMBER_OF_PALETTES] = {0xFF};
    byte backgroundColours[4*SWATCHES_PER_PALETTE*NUMBER_OF_PALETTES] = {0xFF};

    // Pixel data for the current background layer.
    uint8_t* videoBufferBackgroundLayer = nullptr;
    // Pixel data for the entire background map - this is only used to visualize the entire map with the map viewer.
    uint8_t* backgroundMap0 = nullptr;
    // Pixel data for each tile.
    uint8_t* tileMap = nullptr;
    // An intermediate array for fetching a single tile without a palette.
    uint8_t* nonColouredTile = nullptr;

     // Keeps track of the "cycle currency" the PPU can spend.
    cycles cyclesCounter = 0;

public:
    byte getSCY(){return SCY;}
    byte getSCX(){return SCX;}
    /**
     * @brief Dynamically allocates all blocks
     * of memory. Returns a bool indicating success.
     */
    bool init();
    
    /**
     * @brief Deallocates all dynamic memory.
     */
    void destroy();

    /**
     * @brief Binds a memory module to the CPU. All future PPU operations are
     * performed on bound memory.
     * 
     * @param mem 
     */
    void bindMemory(Memory* mem) { memory = mem; }

    /**
     * @brief Perform any work need on this clock cycle. This will increase 
     * the PPU's internal cycle counter.
     * 
     * @param CGBMode - A boolean indicating whether to interpret VRAM in
     * GameBoy vs GameBoy Colour modes.  
     */
    void cycle(bool CGBMode);

    /**
     * @brief Renders the current scanline of VRAM to the video buffer.
     * 
     * @param CGBMode - A boolean indicating whether to interpret VRAM in
     * GameBoy vs GameBoy Colour modes. 
     */
    void renderCurrentScanlineVRAM(bool CGBMode);

    /**
     * @brief This function reinterprets the entire tile map.
     */
    void updateTileMap();
    /**
     * @brief Helper function for `updateTileMap()`. This
     * function updates the tile map for a single tile.
     * 
     * @param tileIndex - The tile index to update. This is computed
     * using: (<TILE_ADDRESS> - TILE_DATA_START) / BYTES_PER_TILE.
     * @param vRAMBank - The bank to update. This should 
     * be `1` or `0`.
     */
    void updateTile(int tileIndex, bool vRAMBank);

    /**
     * @brief This function reinterprets the object and background palettes.
     * 
     * @param CGBMode - A boolean indicating whether to interpret VRAM in
     * GameBoy vs GameBoy Colour modes.  
     */
    void updatePalettes(bool CGBMode);

    /**
     * @brief This function reinterprets background map data. This function
     * should only be called when running the background map viewer. 
     * 
     * @param CGBMode - A boolean indicating whether to interpret VRAM in 
     * GameBoy vs GameBoy Colour modes.  
     */
    void updateBackgroundMap(bool CGBMode);

    // Accessors.

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

    uint8_t* getVideoBufferBG(){
        return videoBufferBackgroundLayer;
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
     * @brief A helper function used to increase the scanline.
     */
    void increaseScanline();

    /**
     * @brief Used to interface with the STAT register.
     * 
     * @param data The data to write to the stat register.
     */
    void writeToSTAT(byte data);
};

#endif;