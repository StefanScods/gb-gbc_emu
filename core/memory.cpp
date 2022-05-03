/*
Memory class implmentation for a gameboy color emulator 

date: 2021-11-13
*/
#include "include\memory.h"

#include <iostream>

bool Memory::init(){

    data = new byte[MEM_SIZE]; //dynamically alloc the memory array 

    if(data==nullptr)  return false; //make sure it was successful 

    //zeros the memory 
    for(int address = 0; address<MEM_SIZE; ++address){
        data[ (word) address] = 0;
    }

    return true;
}


bool Memory::destroy(){
    if(data==nullptr)  return false;

    delete[] data; //unallocs the data 
    return true;
}


void Memory::write(word address, byte d_data){
    data[address] = d_data;
}

const byte Memory::read(word address){
    return data[address];
}

byte* Memory::getBytePointer(word address){
    return data+address;
}