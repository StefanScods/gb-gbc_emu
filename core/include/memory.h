#ifndef MEMORY_H
#define MEMORY_H
/*
Memory class declaration for a GameBoy color emulator 

date: 2021-11-13
*/
#include "defines.h"
#include <mutex> 

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
};











#endif