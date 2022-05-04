/*
CPU class implementation for a gameboy color emulator 

date: 2021-11-12 
*/


#include "include\cpu.h"
#include <iostream>


//!!! NOT ACCUATE EMU -> UPDATE 
void CPU::init(){
    PC = (word) 0x0100;
    SP = (word) 0xFF;
}

cycles CPU::fetchAndExecute(){
    //read the next instuction and increment pc 
    byte nextOpcode = memory->read(PC.read());

    Instruction nextInsruction = instructionSet.set[nextOpcode];
    PC++;

    std::cout << nextInsruction.opcode << std::endl;
    
    //if the instuction has immediate data that also needs to be read from memory
    if(nextInsruction.length>=2){
        //perform another mem read
        parsedData = memory->read(PC.read());
        PC++;

        if(nextInsruction.length==3){
            //perform a third and final mem read
            parsedData = parsedData | memory->read(PC.read()) << 8; //constructs a word from the two bytes of parsed data 
            PC++;
        }
    }

    //execute the instuction and return number of cycles 
    return nextInsruction.execute(this);
}

int CPU::executeCycles(cycles numCycles){

    cycles cyclesRemaining = numCycles;

    //runs the cpu in a loop for the desired amount of cycles 
    while(cyclesRemaining>0){

        //read the next instuction and increment pc 
        byte nextOpcode = memory->read(PC.read());
        Instruction nextInsruction = instructionSet.set[nextOpcode];

        if(cyclesRemaining<nextInsruction.cycleCount) break;

        PC++;

        std::cout << nextInsruction.opcode << std::endl;
     
        //if the instuction has immediate data that also needs to be read from memory
        if(nextInsruction.length>=2){
            //perform another mem read
            parsedData = memory->read(PC.read());
            PC++;

            if(nextInsruction.length==3){
                //perform a third and final mem read
                parsedData = parsedData | memory->read(PC.read()) << 8; //constructs a word from the two bytes of parsed data 
                PC++;
            }
        }

        //decrement the cycles at the end of the instuction -> !!! not rly accurate but i think its fine  
        cyclesRemaining-=nextInsruction.execute(this);
    }
    
    return numCycles - cyclesRemaining;
}

void CPU::outputState(){

    std::cout << std::hex;
    std::cout << "========================" << std::endl;
    std::cout <<  "AF: 0x"  << (int) reg_AF.read() << std::endl;
    std::cout <<  "BC: 0x" << (int) reg_BC.read() << std::endl;
    std::cout <<  "DE: 0x"  << (int) reg_DE.read() << std::endl;
    std::cout <<  "HL: 0x"  << (int) reg_HL.read() << std::endl;
    std::cout <<  "\nSP: 0x"  << (int) SP.read() << std::endl;
    std::cout <<  "PC: 0x" << (int) PC.read() << std::endl;

    std::cout << "FLAGS: Z: " << readBit(*(F), FLAG_Z) <<" N: "<< readBit(*(F), FLAG_N) <<" H: " << readBit(*(F), FLAG_H)<<" C: "  << readBit(*(F), FLAG_C) <<std::endl;
    std::cout << "========================" << std::endl;

    std::cout << std::dec;
}

void popHelper(CPU* cpu, reg& dest){
    
    cpu->SP++; //pop the data off the stack
    word data = cpu->memory->read(cpu->SP.read());
    cpu->SP++;
    data = data | (cpu->memory->read(cpu->SP.read()))<<8;
    
    dest = data; //load reg with the popped data 
    //std::cout << data <<std::endl;
}

void pushHelper(CPU* cpu, reg& source){

    word data = source.read();
    //std::cout << data <<std::endl;

    cpu->memory->write(cpu->SP.read(), data >> 8 );
    cpu->SP--;
    cpu->memory->write(cpu->SP.read(), data & 0xFF);
    cpu->SP--; //push the data onto the stack
}