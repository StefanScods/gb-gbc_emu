#ifndef REGISTER_H
#define REGISTER_H
/*
Register class delcaration for a GameBoy color emulator -> 16 wide 

date: 2021-11-12 
*/
#include "defines.h"

class reg{

private:
    byte high;
    byte low;

public:

    //default constuctor -> sets word to 0x0000
    reg();

    //constuctor for full word 
    reg(word d_data);

    //constuctor for two bytes 
    reg(byte d_high, byte d_low);

    //overloaded ops

    //assigment from intermidiate data 
    reg& operator= (const word d_data);

    //assigment from intermidiate data -> effects lower byte 
    reg& operator= (const byte d_data);

    //increments the reg by one -> does not check for overflowing 
    reg& operator++();
    reg operator++(int d_data);

    //decrements the reg by one -> does not check for underflowing 
    reg& operator--();
    reg operator--(int d_data);

    //accessors 
    byte getHigh() { return high;}
    byte getLow()  { return low; }

    //returns a pointer to the high byte of the reg
    reg8 getHighPtr() { return &high;}
    //returns a pointer to the low byte of the reg
    reg8 getLowPtr()  { return &low; }

    //returns the word {high,low}
    word read();
};

//sets the bit of a given byte and position to a value 
static void writeBit(byte& data, int pos, bool value){
    if(value) data = data | (0x1<<pos); //set the bit to 1
    else data = data & ~(0x1<<pos); //set the bit to 0
    
}

//returns the bit of a given byte and position
static bool readBit(const byte& data, int pos){
    bool value = (data >> (pos)) & 0x1;
    return value;
}









#endif