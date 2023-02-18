#ifndef MEMORY_H
#define MEMORY_H
/*
Memory class delcaration for a GameBoy color emulator 

date: 2021-11-13
*/
#include "defines.h"



class Memory{
private:

    //memory map memory chunks 
    byte* romBank0 = nullptr;
    byte* romBankn = nullptr;

    byte* vRAM = nullptr;

    byte* externalRAM = nullptr;

    byte* wRAM0 = nullptr;
    byte* wRAM1 = nullptr;

    byte* spriteAttributeTable = nullptr;

    byte* ioPorts = nullptr;

    byte* hRAM = nullptr;

    byte interruptEnableRegister;



public:
    //initalizes the memory class -> zeros the memeory 
    bool init();

    //destorys the memory class and cleans up its data 
    bool destroy();

    //accessors + mutators //-> !no error checking is done in these functions for the sake of speed  -> !!!might wanna make safe versions
    void write(word address, byte d_data);
    const byte read(word address);

    byte* getBytePointer(word address); 
};











#endif