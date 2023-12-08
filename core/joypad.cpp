/*
The class implementation for the GB's joypad I/O device.
*/
#include "include/joypad.h"
#include "include/register.h"

byte Joypad::read(){
    bool selectButton = !readBit(data, 5);
    bool selectDirection = !readBit(data, 4);

    if(selectButton){
        writeBit(data, 0, !a);
        writeBit(data, 1, !b);
        writeBit(data, 2, !select);
        writeBit(data, 3, !start);
    } else if (selectDirection) {
        writeBit(data, 0, !right);
        writeBit(data, 1, !left);
        writeBit(data, 2, !up);
        writeBit(data, 3, !down);
    } else {
        // Make all inputs high.
        data = (data & 0xF0) | 0xF;
    }

    return data;
}

void Joypad::write(byte d_data){
    data = (0x30 & d_data) | 0b11001111;
}

void Joypad::reset(){
    a = true;
    b = true;
    start = true;
    select = true;

    up = true;
    down = true;
    left = true;
    right = true;

    data = 0xFF;
}