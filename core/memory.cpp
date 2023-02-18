/*
Memory class implmentation for a GameBoy color emulator 

date: 2021-11-13
*/
#include "include\memory.h"

#include <iostream>

bool Memory::init(){

    //dynamically alloc the memory arrays 
    romBank0 = new byte[ROMBANK0_END - ROMBANK0_START + 1];
    if (romBank0 == nullptr)  return false; //make sure it was successful 

    romBankn = new byte[ROMBANKN_END - ROMBANKN_START + 1];
    if (romBankn == nullptr)  return false; //make sure it was successful 

    vRAM = new byte[VRAM_END - VRAM_START + 1];
    if (vRAM == nullptr)  return false; //make sure it was successful 

    externalRAM = new byte[EXTERNALRAM_END - EXTERNALRAM_START + 1];
    if (externalRAM == nullptr)  return false; //make sure it was successful 

    wRAM0 = new byte[WRAMBANK0_END - WRAMBANK0_START + 1];
    if (wRAM0 == nullptr)  return false; //make sure it was successful 

    wRAM1 = new byte[WRAMBANK1_END - WRAMBANK1_START + 1];
    if (wRAM1 == nullptr)  return false; //make sure it was successful 

    spriteAttributeTable = new byte[OAM_END - OAM_START + 1];
    if (spriteAttributeTable == nullptr)  return false; //make sure it was successful 

    ioPorts = new byte[IOPORTS_END - IOPORTS_START + 1];
    if (ioPorts == nullptr)  return false; //make sure it was successful 

    hRAM = new byte[HRAM_END - HRAM_START + 1];
    if (hRAM == nullptr)  return false; //make sure it was successful 

    //zeros the memory 
    for(int address = 0; address< ROMBANK0_END - ROMBANK0_START + 1; ++address){
        romBank0[(word)address] = 0;
    }
    for (int address = 0; address < ROMBANKN_END - ROMBANKN_START + 1; ++address) {
        romBankn[(word)address] = 0;
    }
    for (int address = 0; address < VRAM_END - VRAM_START + 1; ++address) {
        vRAM[(word)address] = 0;
    }
    for (int address = 0; address < EXTERNALRAM_END - EXTERNALRAM_START + 1; ++address) {
        externalRAM[(word)address] = 0;
    }
    for (int address = 0; address < WRAMBANK0_END - WRAMBANK0_START + 1; ++address) {
        wRAM0[(word)address] = 0;
    }
    for (int address = 0; address < WRAMBANK1_END - WRAMBANK1_START + 1; ++address) {
        wRAM1[(word)address] = 0;
    }
    for (int address = 0; address < OAM_END - OAM_START + 1; ++address) {
        spriteAttributeTable[(word)address] = 0;
    }
    for (int address = 0; address < IOPORTS_END - IOPORTS_START + 1; ++address) {
        ioPorts[(word)address] = 0;
    }
    for (int address = 0; address < HRAM_END - HRAM_START + 1; ++address) {
        hRAM[(word)address] = 0;
    }

    interruptEnableRegister = 0;

    return true;
}


bool Memory::destroy(){
    if(romBank0 == nullptr)  return false;

    //unallocs the data 
    delete[] romBank0; 
    delete[] romBankn;
    delete[] vRAM;
    delete[] externalRAM;
    delete[] wRAM0;
    delete[] wRAM1;
    delete[] spriteAttributeTable;
    delete[] ioPorts;
    delete[] hRAM;

    return true;
}

void Memory::write(word address, byte d_data){

    //Memory Map

    //0000-3FFF   16KB ROM Bank 00     (in cartridge, fixed at bank 00)
    if (address >= ROMBANK0_START && address <= ROMBANK0_END) {
        //romBank0[address - ROMBANK0_START] = d_data; //read only 
    }

    //4000-7FFF   16KB ROM Bank 01..NN (in cartridge, switchable bank number)
    else if (address >= ROMBANKN_START && address <= ROMBANKN_END) {
        //romBankn[address - ROMBANKN_START] = d_data; //read only 
    }

    //8000-9FFF   8KB Video RAM (VRAM) (switchable bank 0-1 in CGB Mode)
    else if (address >= VRAM_START && address <= VRAM_END) {
        vRAM[address - VRAM_START] = d_data;
    }

    //A000-BFFF   8KB External RAM     (in cartridge, switchable bank, if any)
    else if (address >= EXTERNALRAM_START && address <= EXTERNALRAM_END) {
        externalRAM[address - EXTERNALRAM_START] = d_data;
    }
    
    //C000 - CFFF   4KB Work RAM Bank 0 (WRAM)
    else if (address >= WRAMBANK0_START && address <= WRAMBANK0_END) {
        wRAM0[address - WRAMBANK0_START] = d_data;
    }

    //D000-DFFF   4KB Work RAM Bank 1 (WRAM)  (switchable bank 1-7 in CGB Mode)
    else if (address >= WRAMBANK1_START && address <= WRAMBANK1_END) {
        wRAM1[address - WRAMBANK1_START] = d_data;
    }

    //E000-FDFF   Same as C000-DDFF (ECHO)    (typically not used)
    else if (address >= ECHO_START && address <= ECHO_END) {
        //re calls the function but offset to the actual block of memory 
        write(address - ECHO_START + WRAMBANK0_START, d_data);
    }

    //FE00-FE9F   Sprite Attribute Table (OAM)
    else if (address >= OAM_START && address <= OAM_END) {
        spriteAttributeTable[address - OAM_START] = d_data;
    }

    //FEA0-FEFF   Not Usable
    else if (address >= NOTUSABLE_START && address <= NOTUSABLE_END) {
        //!!! maybe do an error check???
    }

    //FF00-FF7F   I/O Ports
    else if (address >= IOPORTS_START && address <= IOPORTS_END) {
        ioPorts[address - IOPORTS_START] = d_data;
    }

    //FF80-FFFE   High RAM (HRAM)
    else if (address >= HRAM_START && address <= HRAM_END) {
        hRAM[address - HRAM_START] = d_data;
    }
    //FFFF        Interrupt Enable Register

    else if (address == INTERUPT_ENABLE_REGISTER_ADDR) {
        interruptEnableRegister = d_data;
    }
}

const byte Memory::read(word address){
    //Memory Map

    //0000-3FFF   16KB ROM Bank 00     (in cartridge, fixed at bank 00)
    if (address >= ROMBANK0_START && address <= ROMBANK0_END) {
        return romBank0[address - ROMBANK0_START];
    }

    //4000-7FFF   16KB ROM Bank 01..NN (in cartridge, switchable bank number)
    else if (address >= ROMBANKN_START && address <= ROMBANKN_END) {
        return romBankn[address - ROMBANKN_START];
    }

    //8000-9FFF   8KB Video RAM (VRAM) (switchable bank 0-1 in CGB Mode)
    else if (address >= VRAM_START && address <= VRAM_END) {
        return vRAM[address - VRAM_START];
    }

    //A000-BFFF   8KB External RAM     (in cartridge, switchable bank, if any)
    else if (address >= EXTERNALRAM_START && address <= EXTERNALRAM_END) {
        return externalRAM[address - EXTERNALRAM_START];
    }

    //C000 - CFFF   4KB Work RAM Bank 0 (WRAM)
    else if (address >= WRAMBANK0_START && address <= WRAMBANK0_END) {
        return wRAM0[address - WRAMBANK0_START];
    }

    //D000-DFFF   4KB Work RAM Bank 1 (WRAM)  (switchable bank 1-7 in CGB Mode)
    else if (address >= WRAMBANK1_START && address <= WRAMBANK1_END) {
        return wRAM1[address - WRAMBANK1_START];
    }

    //E000-FDFF   Same as C000-DDFF (ECHO)    (typically not used)
    else if (address >= ECHO_START && address <= ECHO_END) {
        //re calls the function but offset to the actual block of memory 
        return read(address - ECHO_START + WRAMBANK0_START);
    }

    //FE00-FE9F   Sprite Attribute Table (OAM)
    else if (address >= OAM_START && address <= OAM_END) {
        return spriteAttributeTable[address - OAM_START];
    }

    //FEA0-FEFF   Not Usable
    else if (address >= NOTUSABLE_START && address <= NOTUSABLE_END) {
        //!!! maybe do an error check???
        return 0;
    }

    //FF00-FF7F   I/O Ports
    else if (address >= IOPORTS_START && address <= IOPORTS_END) {
        return ioPorts[address - IOPORTS_START];
    }

    //FF80-FFFE   High RAM (HRAM)
    else if (address >= HRAM_START && address <= HRAM_END) {
        return hRAM[address - HRAM_START];
    }
    //FFFF        Interrupt Enable Register

    else if (address == INTERUPT_ENABLE_REGISTER_ADDR) {
        return interruptEnableRegister;
    }
}

byte* Memory::getBytePointer(word address){

    //Memory Map

    //0000-3FFF   16KB ROM Bank 00     (in cartridge, fixed at bank 00)
    if (address >= ROMBANK0_START && address <= ROMBANK0_END) {
        return romBank0 + address - ROMBANK0_START;
    }

    //4000-7FFF   16KB ROM Bank 01..NN (in cartridge, switchable bank number)
    else if (address >= ROMBANKN_START && address <= ROMBANKN_END) {
        return romBankn + address - ROMBANKN_START;
    }

    //8000-9FFF   8KB Video RAM (VRAM) (switchable bank 0-1 in CGB Mode)
    else if (address >= VRAM_START && address <= VRAM_END) {
        return vRAM + address - VRAM_START;
    }

    //A000-BFFF   8KB External RAM     (in cartridge, switchable bank, if any)
    else if (address >= EXTERNALRAM_START && address <= EXTERNALRAM_END) {
        return externalRAM + address - EXTERNALRAM_START;
    }

    //C000 - CFFF   4KB Work RAM Bank 0 (WRAM)
    else if (address >= WRAMBANK0_START && address <= WRAMBANK0_END) {
        return wRAM0 + address - WRAMBANK0_START;
    }

    //D000-DFFF   4KB Work RAM Bank 1 (WRAM)  (switchable bank 1-7 in CGB Mode)
    else if (address >= WRAMBANK1_START && address <= WRAMBANK1_END) {
        return wRAM1 + address - WRAMBANK1_START;
    }

    //E000-FDFF   Same as C000-DDFF (ECHO)    (typically not used)
    else if (address >= ECHO_START && address <= ECHO_END) {
        //re calls the function but offset to the actual block of memory 
        return getBytePointer(address - ECHO_START + WRAMBANK0_START);
    }

    //FE00-FE9F   Sprite Attribute Table (OAM)
    else if (address >= OAM_START && address <= OAM_END) {
        return spriteAttributeTable + address - OAM_START;
    }

    //FEA0-FEFF   Not Usable
    else if (address >= NOTUSABLE_START && address <= NOTUSABLE_END) {
        //!!! maybe do an error check???
        return 0;
    }

    //FF00-FF7F   I/O Ports
    else if (address >= IOPORTS_START && address <= IOPORTS_END) {
        return ioPorts + address - IOPORTS_START;
    }

    //FF80-FFFE   High RAM (HRAM)
    else if (address >= HRAM_START && address <= HRAM_END) {
        return hRAM + address - HRAM_START;
    }

    //FFFF        Interrupt Enable Register
    else if (address == INTERUPT_ENABLE_REGISTER_ADDR) {
        return &interruptEnableRegister;
    }
}