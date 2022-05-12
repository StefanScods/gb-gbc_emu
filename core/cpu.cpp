/*
CPU class implementation for a gameboy color emulator 

date: 2021-11-12 
*/


#include "include\cpu.h"
#include <iostream>

//according to http://bgb.bircd.org/pandocs.htm#powerupsequence
void CPU::init(){

    if (memory == nullptr) {
        std::cout << "Error: Must Bind Memory to CPU First!" << std::endl;
        return;
    }

    reg_AF = (word)0x01B0;
    reg_BC = (word)0x0013;
    reg_DE = (word)0x00D8;
    reg_HL = (word)0x014D;

    PC = (word)0x0100;
    SP = (word)0xFFFE;

    memory->write(0xFF05, 0x00); //TIMA
    memory->write(0xFF06, 0x00); //TMA
    memory->write(0xFF07, 0x00); //TAC
    memory->write(0xFF10, 0x80); //NR10
    memory->write(0xFF11, 0xBF); //NR11
    memory->write(0xFF12, 0xF3); //NR12
    memory->write(0xFF14, 0xBF); //NR14
    memory->write(0xFF16, 0x3F); //NR21
    memory->write(0xFF17, 0x00); //NR22
    memory->write(0xFF19, 0xBF); //NR24
    memory->write(0xFF1A, 0x7F); //NR30
    memory->write(0xFF1B, 0xFF); //NR31
    memory->write(0xFF1C, 0x9F); //NR32
    memory->write(0xFF1E, 0xBF); //NR33
    memory->write(0xFF20, 0xFF); //NR41
    memory->write(0xFF21, 0x00); //NR42
    memory->write(0xFF22, 0x00); //NR43
    memory->write(0xFF23, 0xBF); //NR30
    memory->write(0xFF24, 0x77); //NR50
    memory->write(0xFF25, 0xF3); //NR51
    memory->write(0xFF26, 0xF1); //NR52
    memory->write(0xFF40, 0x91); //LCDC
    memory->write(0xFF42, 0x00); //SCY
    memory->write(0xFF43, 0x00); //SCX
    memory->write(0xFF45, 0x00); //LYC
    memory->write(0xFF47, 0xFC); //BGP
    memory->write(0xFF48, 0xFF); //OBP0
    memory->write(0xFF49, 0xFF); //OBP1
    memory->write(0xFF4A, 0x00); //WY
    memory->write(0xFF4B, 0x00); //WX
    memory->write(0xFFFF, 0x00); //IE
}

cycles CPU::fetchAndExecute(){
    //read the next instuction and increment pc 
    byte nextOpcode = memory->read(PC.read());

    Instruction nextInsruction = instructionSet.set[nextOpcode];
    PC++;

    if(verbose) std::cout << nextInsruction.opcode << std::endl;
    
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

        if (verbose) std::cout << nextInsruction.opcode << std::endl;
     
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

cycles CPU::getClockSpeed() {
    return clockSpeed;
}

void CPU::populateCpuStateBuffer(word* regs, char* flags){

    regs[0] = reg_AF.read();
    regs[1] = reg_BC.read();
    regs[2] = reg_DE.read();
    regs[3] = reg_HL.read();
    regs[4] = SP.read();
    regs[5] = PC.read();

    readBit(*(F), FLAG_Z) ? flags[0] = '1' : flags[0] = '0';
    readBit(*(F), FLAG_N) ? flags[1] = '1' : flags[1] = '0';
    readBit(*(F), FLAG_H) ? flags[2] = '1' : flags[2] = '0';
    readBit(*(F), FLAG_C) ? flags[3] = '1' : flags[3] = '0';
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

void displayHex(word value, char* output) {

    output[3] = toHex[value % 16];
    value = value >> 4;
    output[2] = toHex[value % 16];
    value = value >> 4;
    output[1] = toHex[value % 16];
    value = value >> 4;
    output[0] = toHex[value % 16];
}