/*
The class implementation for the emulator's PPU / LCD  display.
*/
#include "include/ppu.h"
#include "include/memory.h"
#include "include/register.h"
#include <set>

// Enables debug cout statements for this file.
#define ENABLE_DEBUG_PRINTS false

bool PPU::init(){
   // Create the background maps.
    backgroundMap0 = new uint8_t[INT8_PER_BG_MAP];
    if (backgroundMap0 == nullptr)
        return false;
    // Create the tile map.
    tileMap = new uint8_t[PIXELS_PER_TILE * TILES_PER_BANK * 2];
    if (tileMap == nullptr)
        return false;
    nonColouredTile = new uint8_t[INT8_PER_TILE];
    if (nonColouredTile == nullptr)
        return false;
    videoBufferBackgroundLayer = new uint8_t[INT8_PER_SCREEN];
    if (videoBufferBackgroundLayer == nullptr)
        return false;

    // All alloc successful.
    return true;
}

void PPU::destroy(){
    // Deallocates the data.
    delete[] backgroundMap0;
    delete[] tileMap;
    delete[] nonColouredTile;
}

void PPU::cycle(bool CGBMode){
    /**
     * The PPU renders to the LCD in rows called scan lines. There are 154 scanlines in the display, 
     * 144 visible, 10 in the "v-blank" zone. It takes the PPU 456 cycles to render a scanline.
     * 
     * For the visible scanlines the PPU moves through different modes, these modes indicate which
     * blocks of memory are accessible to the CPU.
     * 
     * Mode 0 - Nothing is blocked : ~ 204 cycles : Moves to mode 2 for visible scanlines and mode 1 for v-blanking.
     * Mode 1 - Nothing is blocked : ~ 4560 cycles : Moves to mode 1.
     * Mode 2 - OAM is blocked : ~ 80 cycles : Moves to mode 3.
     * Mode 3 - OAM and VRAM is blocked : ~ 172 cycles : Moves to mode 0.
     * 
     * 
     * Since VRAM and OAM do not update after they become blocked and the changes only need to be visible after the
     * scanline is rendered, we can interpret and process VRAM and OAM at the start of the mode where they become blocked.
     * From there, that state of OAM and VRAM data will be rendered to the scanline.
     */
    cyclesCounter++;
    switch(mode){
        case 0:
            // Start of Mode 0.
            if(cyclesCounter == 1){
            // End of Mode 0.
            } else if (cyclesCounter == MODE0_LEN){
                // Loop the cycles counter at a mode transition.
                cyclesCounter = 0;
                increaseScanline();
            }
            break;
        case 1:
            // Start of Mode 1.
            if(cyclesCounter == 1){
            // End of Mode 1.
            } else if (cyclesCounter == MODE1_LEN){
                // Loop the cycles counter at a mode transition.
                cyclesCounter = 0;
                increaseScanline();
            }
            break;
        case 2:
            // Start of Mode 2.
            if(cyclesCounter == 1){
            // End of Mode 2.
            } else if (cyclesCounter == MODE2_LEN){
                // Loop the cycles counter at a mode transition.
                cyclesCounter = 0;
                // Move to mode 3.
                mode = 3;
            }
            break;
         case 3:
            // Start of Mode 2.
            if(cyclesCounter == 1){
                updateTileMap();
                updatePalettes(CGBMode);
                renderCurrentScanlineVRAM(CGBMode);
            // End of Mode 2.
            } else if (cyclesCounter == MODE3_LEN){
                // Loop the cycles counter at a mode transition.
                cyclesCounter = 0;
                // Move to mode 0.
                mode = 0;
            }
            break;
        default:
            break;
    }
}

void PPU::increaseScanline(){
    // Increment the scanline and move to the next mode depending on which scanline we are on.
    scanline++;
    // V-BLANK.
    if(scanline > LAST_VISIBLE_SCANLINE){
        mode = 1;

        // Raise the VBlank Interrupt flag.
        if(scanline == LAST_VISIBLE_SCANLINE + 1){
            byte interruptFlags = memory->read(INTERRUPT_FLAG_REGISTER_ADDR);
            writeBit(interruptFlags, 0, true);
            memory->write(INTERRUPT_FLAG_REGISTER_ADDR, interruptFlags);
        }
       
        // Loop back to the visible scanlines.
        if(scanline == NUM_SCANLINES){
            scanline = 0;
            mode = 2;
        }
    }
    // Visible scanline.
    else{
        mode = 2;
    }

    // Update the LYC=LY Flag.
    writeBit(STAT, 2, scanline == LYC);
}

void PPU::renderCurrentScanlineVRAM(bool CGBMode){
    if(scanline > LAST_VISIBLE_SCANLINE) return;

    if(CGBMode){
        std::cerr << "Error: GameBoy Colour Mode is currently unsupported!" << std::endl;
        return;
    }

    // Get the map to render from.
    byte* startOfTileMapPointer = memory->getBytePointer(BGM0_DATA_START);
    // Get the palette to apply -> always BGP0 on non-CGB mode.
    byte* startOfPalette =  getPaletteColour(false, 0);
    // Determine the map data to render.
    int currY = (SCY + scanline) % BG_MAP_WIDTH_PIXELS;
    int mapY = currY / TILE_DIMENSION;
    int pixelY = currY % TILE_DIMENSION;
    // Loop over all pixels of the scanline.
    for(int i = 0; i < SCREEN_WIDTH; i++){
        int currX = (SCX + i) % BG_MAP_WIDTH_PIXELS;
        int mapX = currX / TILE_DIMENSION;
        int pixelX = currX % TILE_DIMENSION;
        int mapIndex = mapY * BG_MAP_WIDTH_TILES + mapX;
        // Get which tile to draw.
        byte tileIndex = *(startOfTileMapPointer + mapIndex);

        // Determine which colour to use.
        uint32_t pixelPositionInMap = tileIndex * PIXELS_PER_TILE + pixelY*TILE_DIMENSION + pixelX;
        byte* startOfSwatch = startOfPalette + (4 * tileMap[pixelPositionInMap]);

        // Calculate where each pixel is on the master array.
        uint32_t pixelPositionToRender = scanline * SCREEN_WIDTH * sizeof(uint32_t) + i * sizeof(uint32_t);
        videoBufferBackgroundLayer[pixelPositionToRender    ] = *(startOfSwatch + 2); // Blue.
        videoBufferBackgroundLayer[pixelPositionToRender + 1] = *(startOfSwatch + 1); // Green.
        videoBufferBackgroundLayer[pixelPositionToRender + 2] = *(startOfSwatch    ); // Red.
        videoBufferBackgroundLayer[pixelPositionToRender + 3] = *(startOfSwatch + 3); // Alpha.
    }
}

void PPU::updateTileMap(){
    // Loop over all the tiles marked dirty.
    std::set<int> dirtyTiles = memory->getDirtyTiles();
    std::set<int>::iterator itr;
    for(itr = dirtyTiles.begin(); itr != dirtyTiles.end(); itr++){
       
        // Update the tile marked as dirty.
        int bankNumber = (*itr) / TILES_PER_BANK;
        int tileIndex = (*itr) % TILES_PER_BANK;
        if (ENABLE_DEBUG_PRINTS)
            std::cout << "Updating Tile: " << tileIndex << std::endl;
        updateTile(tileIndex, bankNumber);
    }
     memory->clearDirtyTiles();
}

void PPU::updateTile(int tileIndex, bool vRAMBank){
    // Get a pointer to the VRAM bank.
    byte* VRAMPointer = nullptr;
    if(vRAMBank) VRAMPointer = memory->vRAMBank1;
    else VRAMPointer = memory->vRAMBank2;

    word tileAddress = tileIndex * BYTES_PER_TILE;

    // Loop over the 8 lines which make up a tile.
    for(int lineNumber = 0; lineNumber<TILE_DIMENSION; lineNumber++){
        // Read two bytes per line.
        byte VRAMTileData1 = *(VRAMPointer+(tileAddress+lineNumber*2));
        byte VRAMTileData2 = *(VRAMPointer+0x1+(tileAddress+lineNumber*2));

        // Loop over the 8 pixels per line.
        for(int pixelNumber = TILE_DIMENSION - 1; pixelNumber >= 0; pixelNumber--){

            // Compute the colour index.
            int colourIndex = ((VRAMTileData2 & 0b1) << 1) | (VRAMTileData1 & 0b1);
            int pixelPosition = PIXELS_PER_TILE*(tileIndex + vRAMBank*TILES_PER_BANK) + (lineNumber * TILE_DIMENSION + pixelNumber);
            tileMap[pixelPosition] = colourIndex;

            // Shift by 2 to get the next colour.
            VRAMTileData1 = VRAMTileData1 >> 1;
            VRAMTileData2 = VRAMTileData2 >> 1;
        }
    }
}

void PPU::updatePalettes(bool CGBMode){
    // Handle the monochrome mode.
    if(!CGBMode){
        // Read BGP at 0xFF47 and construct the colour.
        byte bgTileData = memory->read(0xFF47);
        for(int swatchIndex = 0; swatchIndex < SWATCHES_PER_PALETTE; swatchIndex++){
            const byte* selectedBrightness = &MONOCHROME_COLOURS[(bgTileData & 0x3) * 3];
            backgroundColours[swatchIndex * 4 + 3] = 0xFF;                         // Alpha.
            backgroundColours[swatchIndex * 4 + 2] = *(selectedBrightness + 2);    // Blue.
            backgroundColours[swatchIndex * 4 + 1] = *(selectedBrightness + 1);    // Green.
            backgroundColours[swatchIndex * 4 + 0] = *(selectedBrightness + 0);    // Red.
            bgTileData = bgTileData >> 2;
        }
        // Read OBP0 and 1 at 0xFF48/0xFF49 and construct the colour.
        for(int paletteIndex = 0; paletteIndex < NUMBER_OF_OBJECT_PALETTES_NON_COLOR; paletteIndex++){
            byte obTileData = memory->read(0xFF48 + paletteIndex);
            for(int swatchIndex = 0; swatchIndex < SWATCHES_PER_PALETTE; swatchIndex++){
                const byte* selectedBrightness = &MONOCHROME_COLOURS[(obTileData & 0x3) * 3];
                int offset = paletteIndex * SWATCHES_PER_PALETTE * 4;
                // Set the alpha of swatch0 to 0 (transparent).
                objectColours[offset + swatchIndex * 4 + 3] = swatchIndex ? 0xFF : 0x00;    // Alpha.
                objectColours[offset + swatchIndex * 4 + 2] = *(selectedBrightness + 2);    // Blue.
                objectColours[offset + swatchIndex * 4 + 1] = *(selectedBrightness + 1);    // Green.
                objectColours[offset + swatchIndex * 4 + 0] = *(selectedBrightness + 0);    // Red.
                obTileData = obTileData >> 2;
            }
        }
        return;
    }

    // Handle the GameBoy Colour Mode.
    std::cerr << "Error: GameBoy Colour Palettes are currently unsupported!" << std::endl;
}

void PPU::updateBackgroundMap(bool CGBMode){
    if(CGBMode){
        std::cerr << "Error: GameBoy Colour Mode is currently unsupported!" << std::endl;
        return;
    }

    // Handle the monochrome mode.
    byte* startOfTileMapPointer = memory->getBytePointer(BGM0_DATA_START);
    for(int mapIndex = 0; mapIndex < BGM0_DATA_END - BGM0_DATA_START + 1; mapIndex++){
        // Get which tile to draw.
        byte tileIndex = *(startOfTileMapPointer + mapIndex);
        int mapX = mapIndex % BG_MAP_WIDTH_TILES;
        int mapY = mapIndex / BG_MAP_WIDTH_TILES;
        // Get the palette to apply -> always BGP0 on non-CGB mode.
        byte* startOfPalette =  getPaletteColour(false, 0);
        // Loop over the tile data and render the tile with the background palette applied.
        for(int lineNumber = 0; lineNumber<TILE_DIMENSION; lineNumber++){
            for(int pixelNumber = 0; pixelNumber<TILE_DIMENSION; pixelNumber++){
                // Determine which colour index to use.
                uint32_t pixelPositionInMap = tileIndex * PIXELS_PER_TILE + lineNumber*TILE_DIMENSION + pixelNumber;
                byte* startOfSwatch = startOfPalette + (4 * tileMap[pixelPositionInMap]);
                // Calculate where each pixel is on the master array.
                uint32_t pixelPositionInBGM0 = pixelNumber * sizeof(uint32_t) + mapX * TILE_PITCH + lineNumber * BG_MAP_PITCH + mapY * BG_MAP_PITCH * TILE_DIMENSION;
                backgroundMap0[pixelPositionInBGM0    ] = *(startOfSwatch + 2); // Blue.
                backgroundMap0[pixelPositionInBGM0 + 1] = *(startOfSwatch + 1); // Green.
                backgroundMap0[pixelPositionInBGM0 + 2] = *(startOfSwatch    ); // Red.
                backgroundMap0[pixelPositionInBGM0 + 3] = *(startOfSwatch + 3); // Alpha.
            }
        }
    }
}

byte* PPU::getPaletteColour(boolean objectPalette, int index){
    if(objectPalette) return &objectColours[4*SWATCHES_PER_PALETTE*index];
    return &backgroundColours[4*SWATCHES_PER_PALETTE*index];;
}
uint8_t* PPU::getTileDataWithoutPalette(int tileIndex, bool bankNumber){
    for(int lineNumber = 0; lineNumber<TILE_DIMENSION; lineNumber++){
        for(int pixelNumber = 0; pixelNumber<TILE_DIMENSION; pixelNumber++){
        
            // Determine which colour index to use.
            uint32_t pixelPositionInMap = bankNumber*TILES_PER_BANK*PIXELS_PER_TILE + tileIndex*PIXELS_PER_TILE + lineNumber*TILE_DIMENSION + pixelNumber;
            // Apply a monochrome colour mapping.
            int8_t colourToRender = 255 - 85 * tileMap[pixelPositionInMap];
            
            // Update the one pixel's colour.
            uint32_t pixelPositionInColouredTile = lineNumber * TILE_PITCH  + pixelNumber * sizeof(uint32_t);
            nonColouredTile[pixelPositionInColouredTile    ] = colourToRender; // Blue.
            nonColouredTile[pixelPositionInColouredTile + 1] = colourToRender; // Green.
            nonColouredTile[pixelPositionInColouredTile + 2] = colourToRender; // Red.
            nonColouredTile[pixelPositionInColouredTile + 3] = 0xFF; // Alpha.
        }
    }

    // Return the completed tile.
    return nonColouredTile;
}

void PPU::writeToSTAT(byte data){
    // Bit 7 is unused.
    // Only bits 3-6 are writeable.
    data = data & 0b01111000;
    // Copy the read-only bits of STAT into data.
    data = data | (STAT & 0b10000111);
    STAT = data;
}
