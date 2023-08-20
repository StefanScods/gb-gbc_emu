/*
Memory class implementation for a GameBoy color emulator

date: 2021-11-13
*/
#include "include\memory.h"
#include "include\timer.h"
#include "include\cpu.h"
#include "include\ioController.h"
#include "include\ppu.h"

#include <iostream>

bool Memory::init(CPU* d_cpu, IOController* d_ioController, PPU* d_ppu)
{
    cpu = d_cpu;
    ioController = d_ioController;
    ppu = d_ppu;

    // Dynamically alloc the memory arrays.
    romBank0 = new byte[ROMBANK0_END - ROMBANK0_START + 1];
    if (romBank0 == nullptr)
        return false;
    romBankn = new byte[ROMBANKN_END - ROMBANKN_START + 1];
    if (romBankn == nullptr)
        return false;
    vRAMBank1 = new byte[VRAM_END - VRAM_START + 1];
    if (vRAMBank1 == nullptr)
        return false;
    vRAMBank2 = new byte[VRAM_END - VRAM_START + 1];
    if (vRAMBank2 == nullptr)
        return false;
    externalRAM = new byte[EXTERNALRAM_END - EXTERNALRAM_START + 1];
    if (externalRAM == nullptr)
        return false;
    wRAM0 = new byte[WRAMBANK0_END - WRAMBANK0_START + 1];
    if (wRAM0 == nullptr)
        return false;
    wRAM1 = new byte[WRAMBANK1_END - WRAMBANK1_START + 1];
    if (wRAM1 == nullptr)
        return false;
    spriteAttributeTable = new byte[OAM_END - OAM_START + 1];
    if (spriteAttributeTable == nullptr)
        return false;
    hRAM = new byte[HRAM_END - HRAM_START + 1];
    if (hRAM == nullptr)
        return false;

    // All allocs successful.
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
    delete[] hRAM;

    return true;
}

void Memory::setInitalValues(){
    zeroAllBlocksOfMemory();
    // Update I/O values. 
    write(0xFF05, 0x00); // TIMA
    write(0xFF06, 0x00); // TMA
    write(0xFF07, 0x00); // TAC
    write(0xFF10, 0x80); // NR10
    write(0xFF11, 0xBF); // NR11
    write(0xFF12, 0xF3); // NR12
    write(0xFF14, 0xBF); // NR14
    write(0xFF16, 0x3F); // NR21
    write(0xFF17, 0x00); // NR22
    write(0xFF19, 0xBF); // NR24
    write(0xFF1A, 0x7F); // NR30
    write(0xFF1B, 0xFF); // NR31
    write(0xFF1C, 0x9F); // NR32
    write(0xFF1E, 0xBF); // NR33
    write(0xFF20, 0xFF); // NR41
    write(0xFF21, 0x00); // NR42
    write(0xFF22, 0x00); // NR43
    write(0xFF23, 0xBF); // NR30
    write(0xFF24, 0x77); // NR50
    write(0xFF25, 0xF3); // NR51
    write(0xFF26, 0xF1); // NR52
    write(0xFF40, 0x91); // LCDC
    write(0xFF42, 0xD0); // SCY
    write(0xFF43, 0x00); // SCX
    write(0xFF45, 0x00); // LYC
    write(0xFF47, 0xFC); // BGP
    write(0xFF48, 0xFF); // OBP0
    write(0xFF49, 0xFF); // OBP1
    write(0xFF4A, 0x00); // WY
    write(0xFF4B, 0x00); // WX
    write(0xFFFF, 0x00); // IE
    initializeVRAM();
}
void Memory::zeroAllBlocksOfMemory(){
    for (int address = 0; address < ROMBANK0_END - ROMBANK0_START + 1; ++address)
        romBank0[(word)address] = 0;

    for (int address = 0; address < ROMBANKN_END - ROMBANKN_START + 1; ++address)
        romBankn[(word)address] = 0;
    for (int address = 0; address < VRAM_END - VRAM_START + 1; ++address){
        vRAMBank1[(word)address] = 0;
        vRAMBank2[(word)address] = 0;
    }
    for (int address = 0; address < EXTERNALRAM_END - EXTERNALRAM_START + 1; ++address)
        externalRAM[(word)address] = 0;
    for (int address = 0; address < WRAMBANK0_END - WRAMBANK0_START + 1; ++address)
        wRAM0[(word)address] = 0;
    for (int address = 0; address < WRAMBANK1_END - WRAMBANK1_START + 1; ++address)
        wRAM1[(word)address] = 0;
    for (int address = 0; address < OAM_END - OAM_START + 1; ++address)
        spriteAttributeTable[(word)address] = 0;
    for (int address = 0; address < HRAM_END - HRAM_START + 1; ++address)
        hRAM[(word)address] = 0;
    interruptEnableRegister = 0;
}
void Memory::initializeVRAM(){   
    // Overwrite the first 416 bytes of VRAM with the Nintendo logo Tile Data. 
    const int TILE_VALUES_TO_EDIT = 416;
    const byte INITAL_TILE_STATE[TILE_VALUES_TO_EDIT] =
        { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
          0xF0, 0x00, 0xF0, 0x00, 0xFC, 0x00, 0xFC, 0x00, 0xFC, 0x00, 0xFC, 0x00, 0xF3, 0x00, 0xF3, 0x00, 
          0x3C, 0x00, 0x3C, 0x00, 0x3C, 0x00, 0x3C, 0x00, 0x3C, 0x00, 0x3C, 0x00, 0x3C, 0x00, 0x3C, 0x00, 
          0xF0, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF3, 0x00, 0xF3, 0x00, 
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCF, 0x00, 0xCF, 0x00, 
          0x00, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x0F, 0x00, 0x3F, 0x00, 0x3F, 0x00, 0x0F, 0x00, 0x0F, 0x00, 
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0x0F, 0x00, 0x0F, 0x00, 
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x00, 0xF0, 0x00, 
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF3, 0x00, 0xF3, 0x00, 
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0xC0, 0x00, 
          0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0xFF, 0x00, 0xFF, 0x00, 
          0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC3, 0x00, 0xC3, 0x00, 
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x00, 0xFC, 0x00, 
          0xF3, 0x00, 0xF3, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0xF0, 0x00, 
          0x3C, 0x00, 0x3C, 0x00, 0xFC, 0x00, 0xFC, 0x00, 0xFC, 0x00, 0xFC, 0x00, 0x3C, 0x00, 0x3C, 0x00, 
          0xF3, 0x00, 0xF3, 0x00, 0xF3, 0x00, 0xF3, 0x00, 0xF3, 0x00, 0xF3, 0x00, 0xF3, 0x00, 0xF3, 0x00,
          0xF3, 0x00, 0xF3, 0x00, 0xC3, 0x00, 0xC3, 0x00, 0xC3, 0x00, 0xC3, 0x00, 0xC3, 0x00, 0xC3, 0x00,
          0xCF, 0x00, 0xCF, 0x00, 0xCF, 0x00, 0xCF, 0x00, 0xCF, 0x00, 0xCF, 0x00, 0xCF, 0x00, 0xCF, 0x00,
          0x3C, 0x00, 0x3C, 0x00, 0x3F, 0x00, 0x3F, 0x00, 0x3C, 0x00, 0x3C, 0x00, 0x0F, 0x00, 0x0F, 0x00, 
          0x3C, 0x00, 0x3C, 0x00, 0xFC, 0x00, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x00, 0xFC, 0x00, 
          0xFC, 0x00, 0xFC, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0xF0, 0x00, 
          0xF3, 0x00, 0xF3, 0x00, 0xF3, 0x00, 0xF3, 0x00, 0xF3, 0x00, 0xF3, 0x00, 0xF0, 0x00, 0xF0, 0x00, 
          0xC3, 0x00, 0xC3, 0x00, 0xC3, 0x00, 0xC3, 0x00, 0xC3, 0x00, 0xC3, 0x00, 0xFF, 0x00, 0xFF, 0x00, 
          0xCF, 0x00, 0xCF, 0x00, 0xCF, 0x00, 0xCF, 0x00, 0xCF, 0x00, 0xCF, 0x00, 0xC3, 0x00, 0xC3, 0x00, 
          0x0F, 0x00, 0x0F, 0x00, 0x0F, 0x00, 0x0F, 0x00, 0x0F, 0x00, 0x0F, 0x00, 0xFC, 0x00, 0xFC, 0x00,
          0x3C, 0x00, 0x42, 0x00, 0xB9, 0x00, 0xA5, 0x00, 0xB9, 0x00, 0xA5, 0x00, 0x42, 0x00, 0x3C, 0x00 };
    for(word i = 0; i<TILE_VALUES_TO_EDIT; i++){
        write(VRAM_START+i, INITAL_TILE_STATE[i]);
    }

    // Overwrite the next 48 bytes after 0x9900 of VRAM with the Nintendo logo Background Map Data. 
    const int MAP_VALUES_TO_EDIT = 48;
    const byte INITAL_MAP_STATE[MAP_VALUES_TO_EDIT] = 
        { 0x00, 0X00, 0X00, 0X00, 0X01, 0X02, 0X03, 0X04, 0X05, 0X06, 0X07, 0X08, 0X09, 0X0A, 0X0B, 0X0C, 
          0X19, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 
          0X00, 0X00, 0X00, 0X00, 0X0D, 0X0E, 0X0F, 0X10, 0X11, 0X12, 0X13, 0X14, 0X15, 0X16, 0X17, 0X18 };
    for(word i = 0; i<MAP_VALUES_TO_EDIT; i++){
        write(0x9900+i, INITAL_MAP_STATE[i]);
    }
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
    if (address >= ROMBANK0_START && address <= ROMBANK0_END){
        // Read Only.
    }

    // 4000-7FFF   16KB ROM Bank 01..NN (in cartridge, switchable bank number).
    else if (address >= ROMBANKN_START && address <= ROMBANKN_END){
        // Read Only.
    }

    // 8000-9FFF   8KB Video RAM (VRAM) (switchable bank 0-1 in CGB Mode).
    else if (address >= VRAM_START && address <= VRAM_END)
    {
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
        ioController->write(address, d_data);
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
        return ioController->read(address);
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
        return nullptr;
    }

    // FF00-FF7F   I/O Ports.
    else if (address >= IOPORTS_START && address <= IOPORTS_END)
    {   
        return nullptr;
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
