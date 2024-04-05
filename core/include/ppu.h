#ifndef PPU_H
#define PPU_H
/**
 * Class declaration for a GameBoy Color PPU / LCD display. 
 */

#include "defines.h"
#include <vector>
#include <list>

class Memory;

class OAMEntry{
public:
    // Byte 0.
    byte yPos = 0;
    // Byte 1.
    byte xPos = 0;
    // Byte 2.
    byte tileIndex = 0;
    // Byte 3.
    bool priority = 0;
    bool yFlip = 0;
    bool xFlip = 0;
    bool dmgPalette = 0;
    byte colourPalette = 0;
    /**
     * @brief Reads 4 bytes and updates the entry.
     */
    void update(byte* bytes);
    
    /**
     * @brief Writes the current state to a state file.
     *
     * @param writeBuffer The buffer to write to.
    */
    void saveToState(byte*& writeBuffer);
    /**
     * @brief Load the current state from a state file.
     *
     * @param readBuffer The buffer to read from.
    */
    void loadFromState(byte*& readBuffer);
};

class PPU{
private:
    friend class Memory;
    friend class IOController;

    // A pointer to memory. 
    Memory* memory = nullptr;

    // The current mode of the PPU. This controls what the PPU is doing per cycle. 
    byte mode = 2;

    // Viewport.
    byte SCY = 0;
    byte SCX = 0;

    // Window Viewport.
    byte WY = 0;
    byte WX = 0;

    byte LYC = 0;
    byte STAT = 0;

    byte LCDC = 0;
    bool ppuEnable = true;
    word windowAreaStart = BGM0_DATA_START;
    bool windowEnable = false;
    word tileAreaStart = TILE0_DATA_START;
    word backgroundAreaStart = BGM0_DATA_START;
    bool doubleObjectSize = false;
    bool objectEnable = false;
    bool backgroundEnablePriority = false;

    // The current scanline being drawn.
    byte scanline = 0;

    // Arrays for holding colour data.
    byte objectColours[4*SWATCHES_PER_PALETTE*NUMBER_OF_PALETTES];
    byte backgroundColours[4*SWATCHES_PER_PALETTE*NUMBER_OF_PALETTES];

    // Pixel data for the current background layer.
    uint8_t* videoBufferBackgroundLayer = nullptr;
    // Pixel data for the current window layer.
    uint8_t* videoBufferWindowLayer = nullptr;
    // Pixel data for the current Object layer.
    uint8_t* videoBufferObjectLayer = nullptr;

    // Pixel data for the entire background map - this is only used to visualize the entire map with the map viewer.
    uint8_t* backgroundMap0 = nullptr;
    uint8_t* backgroundMap1 = nullptr;
    // Pixel data for each tile.
    uint8_t* tileMap = nullptr;
    // An intermediate array for fetching a single tile without a palette.
    uint8_t* nonColouredTile = nullptr;
    // Parsed Data for each object.
    OAMEntry* objectAttributeMemory = nullptr;
    std::list<int> objectsToRender;

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
     * @brief Zeros all dynamic memory.
     */
    void zeroAllBlocksOfMemory();

    /**
     * @brief Resets the PPU to inital values.
     */
    void reset();

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
     * @brief Renders the current scanline of BG VRAM to the video buffer.
     * 
     * @param CGBMode - A boolean indicating whether to interpret VRAM in
     * GameBoy vs GameBoy Colour modes. 
     */
    void renderBGMapScanline(bool CGBMode);

    /**
     * @brief Renders the current scanline of window VRAM to the video buffer.
     * 
     * @param CGBMode - A boolean indicating whether to interpret VRAM in
     * GameBoy vs GameBoy Colour modes. 
     */
    void renderWindowMapScanline(bool CGBMode);

    /**
     * @brief Renders the current scanline of Object VRAM to the video buffer.
     * 
     * @param CGBMode - A boolean indicating whether to interpret VRAM in
     * GameBoy vs GameBoy Colour modes. 
     */
    void renderObjectsScanline(bool CGBMode);

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
     * @param mapNum The map number to update. Should be one or zero.
     */
    void updateBackgroundMap(bool CGBMode, bool mapNum);

    // Accessors.

    /**
     * @brief A helper function for retrieving the generated background 
     * map. This returns an array of size `INT8_PER_BG_MAP`. This will already
     * have the colour palette and transformations applied such that no additional 
     * processing is needed to render the background.
     * 
     * @param mapNum The map number to return. Should be one or zero.
     */
    uint8_t* getBackgroundMap(bool mapNum){
        return mapNum ? backgroundMap1 : backgroundMap0;
    };

    uint8_t* getVideoBufferBG(){
        return videoBufferBackgroundLayer;
    };
    uint8_t* getVideoBufferObject(){
        return videoBufferObjectLayer;
    };
    uint8_t* getVideoBufferWindow(){
        return videoBufferWindowLayer;
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
     * @param data The data to write to the STAT register.
     */
    void writeToSTAT(byte data);

    /**
     * @brief Used to interface with the LCDC register.
     * 
     * @param data The data to write to the LCDC register.
     */
    void writeToLCDC(byte data);
    
    /**
     * @brief Parse VRAM and update relevant elements of the Object Attribute Table.
    */
    void updateOAM();

    /**
     * @brief Scan the OAM to determine which element to render this scanline.
    */
    void determineObjectToRender();

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

#endif;