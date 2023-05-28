/*
Memory class implmentation for a GameBoy color emulator

date: 2021-11-13
*/
#include "include\memory.h"

#include <iostream>

bool Memory::init()
{
    dirtyVRAM = true;

    // Dynamically alloc the memory arrays and zero each memory.
    romBank0 = new byte[ROMBANK0_END - ROMBANK0_START + 1];
    if (romBank0 == nullptr)
        return false;
    for (int address = 0; address < ROMBANK0_END - ROMBANK0_START + 1; ++address)
    {
        romBank0[(word)address] = 0;
    }
    romBankn = new byte[ROMBANKN_END - ROMBANKN_START + 1];
    if (romBankn == nullptr)
        return false;
    for (int address = 0; address < ROMBANKN_END - ROMBANKN_START + 1; ++address)
    {
        romBankn[(word)address] = 0;
    }

    vRAMBank1 = new byte[VRAM_END - VRAM_START + 1];
    if (vRAMBank1 == nullptr)
        return false;
    vRAMBank2 = new byte[VRAM_END - VRAM_START + 1];
    if (vRAMBank2 == nullptr)
        return false;
    for (int address = 0; address < VRAM_END - VRAM_START + 1; ++address)
    {
        vRAMBank1[(word)address] = 0;
        vRAMBank2[(word)address] = 0;
    }

    externalRAM = new byte[EXTERNALRAM_END - EXTERNALRAM_START + 1];
    if (externalRAM == nullptr)
        return false;

    for (int address = 0; address < EXTERNALRAM_END - EXTERNALRAM_START + 1; ++address)
    {
        externalRAM[(word)address] = 0;
    }

    wRAM0 = new byte[WRAMBANK0_END - WRAMBANK0_START + 1];
    if (wRAM0 == nullptr)
        return false;
    for (int address = 0; address < WRAMBANK0_END - WRAMBANK0_START + 1; ++address)
    {
        wRAM0[(word)address] = 0;
    }
    wRAM1 = new byte[WRAMBANK1_END - WRAMBANK1_START + 1];
    if (wRAM1 == nullptr)
        return false;
    for (int address = 0; address < WRAMBANK1_END - WRAMBANK1_START + 1; ++address)
    {
        wRAM1[(word)address] = 0;
    }

    spriteAttributeTable = new byte[OAM_END - OAM_START + 1];
    if (spriteAttributeTable == nullptr)
        return false;
    for (int address = 0; address < OAM_END - OAM_START + 1; ++address)
    {
        spriteAttributeTable[(word)address] = 0;
    }
 
    ioPorts = new byte[IOPORTS_END - IOPORTS_START + 1];
    if (ioPorts == nullptr)
        return false;
    for (int address = 0; address < IOPORTS_END - IOPORTS_START + 1; ++address)
    {
        ioPorts[(word)address] = 0;
    }

    hRAM = new byte[HRAM_END - HRAM_START + 1];
    if (hRAM == nullptr)
        return false;
    for (int address = 0; address < HRAM_END - HRAM_START + 1; ++address)
    {
        hRAM[(word)address] = 0;
    }

    interruptEnableRegister = 0;

    // Create the tile map.
    tileMap = new uint8_t[PIXELS_PER_TILE * TILES_PER_BANK * 2];
    if (tileMap == nullptr)
        return false;
    colouredTile = new uint8_t[INT8_PER_TILE];
    if (colouredTile == nullptr)
        return false;

    return true;
}

bool Memory::destroy()
{
    if (romBank0 == nullptr)
        return false;

    // Deallocates the data.
    delete[] romBank0;
    delete[] romBankn;
    delete[] vRAMBank1;
    delete[] vRAMBank2;
    delete[] externalRAM;
    delete[] wRAM0;
    delete[] wRAM1;
    delete[] spriteAttributeTable;
    delete[] ioPorts;
    delete[] hRAM;

    delete[] tileMap;
    delete[] colouredTile;
 
    return true;
}

void Memory::storeROMBank(int ROMBankNumber, std::ifstream *romFile)
{
    // Determine the amount of data to read and create a buffer.
    const int BANK_SIZE = ROMBANK0_END - ROMBANK0_START + 1;
    int bytesToRead = (BANK_SIZE) * sizeof(char);
    char *cartridgeDataBuffer = new char[bytesToRead];

    // Go to the start of the bank and perform the read.
    int bankStartLocation = ROMBankNumber*BANK_SIZE*sizeof(char);
    romFile->seekg(bankStartLocation);
    romFile->read(cartridgeDataBuffer, bytesToRead);

    byte *romBankPointer = (ROMBankNumber == 0) ? 
        getBytePointer(ROMBANK0_START) : getBytePointer(ROMBANKN_START);

    // Copy the data from the file stream into the correct address space.
    for (word i = 0; i < BANK_SIZE; i++)
    {
        romBankPointer[i] = cartridgeDataBuffer[i];
    }

    // Free the data buffer.
    delete[] cartridgeDataBuffer;
}

void Memory::write(word address, byte d_data)
{

    // Memory Map.

    // 0000-3FFF   16KB ROM Bank 00     (in cartridge, fixed at bank 00).
    if (address >= ROMBANK0_START && address <= ROMBANK0_END)
    {
        // romBank0[address - ROMBANK0_START] = d_data; //read only
    }

    // 4000-7FFF   16KB ROM Bank 01..NN (in cartridge, switchable bank number).
    else if (address >= ROMBANKN_START && address <= ROMBANKN_END)
    {
        // romBankn[address - ROMBANKN_START] = d_data; //read only
    }

    // 8000-9FFF   8KB Video RAM (VRAM) (switchable bank 0-1 in CGB Mode).
    else if (address >= VRAM_START && address <= VRAM_END)
    {
        dirtyVRAM = true;
        if(selectedVRAMBank) vRAMBank1[address - VRAM_START] = d_data;
        else vRAMBank2[address - VRAM_START] = d_data;
    }

    // A000-BFFF   8KB External RAM     (in cartridge, switchable bank, if any).
    else if (address >= EXTERNALRAM_START && address <= EXTERNALRAM_END)
    {
        externalRAM[address - EXTERNALRAM_START] = d_data;
    }

    // C000 - CFFF   4KB Work RAM Bank 0 (WRAM).
    else if (address >= WRAMBANK0_START && address <= WRAMBANK0_END)
    {
        wRAM0[address - WRAMBANK0_START] = d_data;
    }

    // D000-DFFF   4KB Work RAM Bank 1 (WRAM)  (switchable bank 1-7 in CGB Mode).
    else if (address >= WRAMBANK1_START && address <= WRAMBANK1_END)
    {
        wRAM1[address - WRAMBANK1_START] = d_data;
    }

    // E000-FDFF   Same as C000-DDFF (ECHO)    (typically not used).
    else if (address >= ECHO_START && address <= ECHO_END)
    {
        // Re-calls this function but offset to the actual block of memory.
        write(address - ECHO_START + WRAMBANK0_START, d_data);
    }

    // FE00-FE9F   Sprite Attribute Table (OAM).
    else if (address >= OAM_START && address <= OAM_END)
    {
        spriteAttributeTable[address - OAM_START] = d_data;
    }

    // FEA0-FEFF   Not Usable.
    else if (address >= NOTUSABLE_START && address <= NOTUSABLE_END)
    {
        //!!! maybe do an error check???
    }

    // FF00-FF7F   I/O Ports.
    else if (address >= IOPORTS_START && address <= IOPORTS_END)
    {
        ioPorts[address - IOPORTS_START] = d_data;
    }

    // FF80-FFFE   High RAM (HRAM).
    else if (address >= HRAM_START && address <= HRAM_END)
    {
        hRAM[address - HRAM_START] = d_data;
    }

    // FFFF        Interrupt Enable Register.
    else if (address == INTERRUPT_ENABLE_REGISTER_ADDR)
    {
        interruptEnableRegister = d_data;
    }
}

const byte Memory::read(word address)
{
    // Memory Map.

    // 0000-3FFF   16KB ROM Bank 00     (in cartridge, fixed at bank 00).
    if (address >= ROMBANK0_START && address <= ROMBANK0_END)
    {
        return romBank0[address - ROMBANK0_START];
    }

    // 4000-7FFF   16KB ROM Bank 01..NN (in cartridge, switchable bank number).
    else if (address >= ROMBANKN_START && address <= ROMBANKN_END)
    {
        return romBankn[address - ROMBANKN_START];
    }

    // 8000-9FFF   8KB Video RAM (VRAM) (switchable bank 0-1 in CGB Mode).
    else if (address >= VRAM_START && address <= VRAM_END)
    {
        if(selectedVRAMBank) return vRAMBank1[address - VRAM_START];
        else return vRAMBank2[address - VRAM_START];
    }

    // A000-BFFF   8KB External RAM     (in cartridge, switchable bank, if any),
    else if (address >= EXTERNALRAM_START && address <= EXTERNALRAM_END)
    {
        return externalRAM[address - EXTERNALRAM_START];
    }

    // C000 - CFFF   4KB Work RAM Bank 0 (WRAM).
    else if (address >= WRAMBANK0_START && address <= WRAMBANK0_END)
    {
        return wRAM0[address - WRAMBANK0_START];
    }

    // D000-DFFF   4KB Work RAM Bank 1 (WRAM)  (switchable bank 1-7 in CGB Mode).
    else if (address >= WRAMBANK1_START && address <= WRAMBANK1_END)
    {
        return wRAM1[address - WRAMBANK1_START];
    }

    // E000-FDFF   Same as C000-DDFF (ECHO)    (typically not used).
    else if (address >= ECHO_START && address <= ECHO_END)
    {
        // Re-calls this function but offset to the actual block of memory.
        return read(address - ECHO_START + WRAMBANK0_START);
    }

    // FE00-FE9F   Sprite Attribute Table (OAM).
    else if (address >= OAM_START && address <= OAM_END)
    {
        return spriteAttributeTable[address - OAM_START];
    }

    // FEA0-FEFF   Not Usable.
    else if (address >= NOTUSABLE_START && address <= NOTUSABLE_END)
    {
        // !!! maybe do an error check???
        return 0;
    }

    // FF00-FF7F   I/O Ports.
    else if (address >= IOPORTS_START && address <= IOPORTS_END)
    {
        return ioPorts[address - IOPORTS_START];
    }

    // FF80-FFFE   High RAM (HRAM).
    else if (address >= HRAM_START && address <= HRAM_END)
    {
        return hRAM[address - HRAM_START];
    }

    // FFFF        Interrupt Enable Register.
    else if (address == INTERRUPT_ENABLE_REGISTER_ADDR)
    {
        return interruptEnableRegister;
    }
}

byte *Memory::getBytePointer(word address)
{

    // Memory Map.

    // 0000-3FFF   16KB ROM Bank 00     (in cartridge, fixed at bank 00).
    if (address >= ROMBANK0_START && address <= ROMBANK0_END)
    {
        return romBank0 + address - ROMBANK0_START;
    }

    // 4000-7FFF   16KB ROM Bank 01..NN (in cartridge, switchable bank number).
    else if (address >= ROMBANKN_START && address <= ROMBANKN_END)
    {
        return romBankn + address - ROMBANKN_START;
    }

    // 8000-9FFF   8KB Video RAM (VRAM) (switchable bank 0-1 in CGB Mode).
    else if (address >= VRAM_START && address <= VRAM_END)
    {   
        if(selectedVRAMBank) return vRAMBank1 + address - VRAM_START;
        else return vRAMBank2 + address - VRAM_START;
    }

    // A000-BFFF   8KB External RAM     (in cartridge, switchable bank, if any).
    else if (address >= EXTERNALRAM_START && address <= EXTERNALRAM_END)
    {
        return externalRAM + address - EXTERNALRAM_START;
    }

    // C000 - CFFF   4KB Work RAM Bank 0 (WRAM).
    else if (address >= WRAMBANK0_START && address <= WRAMBANK0_END)
    {
        return wRAM0 + address - WRAMBANK0_START;
    }

    // D000-DFFF   4KB Work RAM Bank 1 (WRAM)  (switchable bank 1-7 in CGB Mode).
    else if (address >= WRAMBANK1_START && address <= WRAMBANK1_END)
    {
        return wRAM1 + address - WRAMBANK1_START;
    }

    // E000-FDFF   Same as C000-DDFF (ECHO)    (typically not used).
    else if (address >= ECHO_START && address <= ECHO_END)
    {
        // Re-calls this function but offset to the actual block of memory
        return getBytePointer(address - ECHO_START + WRAMBANK0_START);
    }

    // FE00-FE9F   Sprite Attribute Table (OAM).
    else if (address >= OAM_START && address <= OAM_END)
    {
        return spriteAttributeTable + address - OAM_START;
    }

    // FEA0-FEFF   Not Usable.
    else if (address >= NOTUSABLE_START && address <= NOTUSABLE_END)
    {
        //!!! maybe do an error check???
        return 0;
    }

    // FF00-FF7F   I/O Ports.
    else if (address >= IOPORTS_START && address <= IOPORTS_END)
    {
        return ioPorts + address - IOPORTS_START;
    }

    // FF80-FFFE   High RAM (HRAM).
    else if (address >= HRAM_START && address <= HRAM_END)
    {
        return hRAM + address - HRAM_START;
    }

    // FFFF        Interrupt Enable Register.
    else if (address == INTERRUPT_ENABLE_REGISTER_ADDR)
    {
        return &interruptEnableRegister;
    }
}

void Memory::updateTile(int tileIndex, bool vRAMBank){
    // Get a pointer to the VRAM bank.
    byte* VRAMPointer = nullptr;
    if(vRAMBank) VRAMPointer = vRAMBank1;
    else VRAMPointer = vRAMBank2;

    word tileAddress = tileIndex * BYTES_PER_TILE;

    // Loop over the 8 lines which make up a tile.
    for(int lineNumber = 0; lineNumber<TILE_DIMENSION; lineNumber++){
        // Read two bytes per line.
        byte VRAMTileData1 = *(VRAMPointer+(tileAddress+lineNumber*2));
        byte VRAMTileData2 = *(VRAMPointer+0x11+(tileAddress+lineNumber*2));

        // Loop over the 8 pixels per line.
        for(int pixelNumber = TILE_DIMENSION-1; pixelNumber >= 0; pixelNumber--){

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

void Memory::handleDirtyVRAM(){
    // Return early if there is nothing to do.
    if(!dirtyVRAM) return;

    // Update Tile Map.
    // Loop over all tiles in both banks of VRAM.
    for(int bankNumber = 0; bankNumber <= 1; bankNumber++){
        for(int tileIndex = 0; tileIndex < TILES_PER_BANK; tileIndex++){
            updateTile(tileIndex, bankNumber);
        }
    }

    dirtyVRAM = false;
}

uint8_t* Memory::getTileWithPalette(int tileIndex, bool bankNumber){
    for(int lineNumber = 0; lineNumber<TILE_DIMENSION; lineNumber++){
        for(int pixelNumber = 0; pixelNumber<TILE_DIMENSION; pixelNumber++){
        
            // Determine which colour to use.
            uint32_t pixelPositionInMap = bankNumber*TILES_PER_BANK*PIXELS_PER_TILE + tileIndex*PIXELS_PER_TILE + lineNumber*TILE_DIMENSION + pixelNumber;
            int8_t colourToRender = 255 - 85 * tileMap[pixelPositionInMap];
            
            // Update the one pixel's colour.
            uint32_t pixelPositionInColouredTile = lineNumber * TILE_PITCH  + pixelNumber * sizeof(uint32_t);
            colouredTile[pixelPositionInColouredTile    ] = colourToRender; // Blue.
            colouredTile[pixelPositionInColouredTile + 1] = colourToRender; // Green.
            colouredTile[pixelPositionInColouredTile + 2] = colourToRender; // Red.
            colouredTile[pixelPositionInColouredTile + 3] = 0xFF; // Alpha.
        }
    }

    // Return the completed tile.
    return colouredTile;
}