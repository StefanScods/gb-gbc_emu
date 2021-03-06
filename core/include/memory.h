#ifndef MEMORY_H
#define MEMORY_H
/*
Memory class delcaration for a gameboy color emulator 

date: 2021-11-13
*/
#include "defines.h"



class Memory{
private:

    byte* data = nullptr;



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