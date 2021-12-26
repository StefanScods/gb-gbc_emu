/*
Register class implementation for a gameboy color emulator -> 16 wide 

date: 2021-11-12 
*/

#include "include\register.h"
#include <iostream>

reg::reg(){
    high = 0;
    low = 0;
} 

reg::reg(word d_data){
    low = d_data & 0XFF;
    high = d_data >> 8;
}

reg::reg(byte d_high, byte d_low){
    high = d_high;
    low = d_low;
}

word reg::read(){
    word output = high << 8;
    output = output | low; 

    return output;
}

reg& reg::operator= (const word d_data){
    low = d_data & 0XFF;
    high = d_data >> 8;

    return *this;
}

reg& reg::operator= (const byte d_data){

    std::cout << "ERROR: byte assigment to reg16" <<std::endl;

    low = d_data;
    return *this;
}

reg& reg::operator++(){

    low++;
    if(low == 0) high++; //carry the bit between high and low 

    return *this;
}

reg reg::operator++(int)
{
   reg temp = *this;
   ++*this;
   return temp;
}

reg& reg::operator--(){

    if(low == 0){  //borrow
        high--;
        low = 0xFF;
    }
    else  low--;
            
    return *this;
}

reg reg::operator--(int)
{
   reg temp = *this;
   --*this;
   return temp;
}