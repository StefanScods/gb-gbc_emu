/*
a implementation file for the bit instuctions for the gb/gbc cpu

date: 2021-11-13
*/
#include "include\bit.h"
#include "..\include\cpu.h"
#include <iostream>


cycles Bit::rlca(CPU* cpu){

    byte bit7 = readBit(*(cpu->A), 7);
    *(cpu->A) = (*(cpu->A) << 1) | bit7; //rotates to the left 

    writeBit(*(cpu->F), FLAG_Z, 0);
    writeBit(*(cpu->F), FLAG_N, 0);
    writeBit(*(cpu->F), FLAG_H, 0); 
    writeBit(*(cpu->F), FLAG_C, bit7); 

    return RLCA_CYCLES;
}

cycles Bit::rrca(CPU* cpu){

    byte bit0 = readBit(*(cpu->A), 0);

    *(cpu->A) = (*(cpu->A) >> 1) | (bit0<<7); //rotates to the right 

    writeBit(*(cpu->F), FLAG_Z, 0);
    writeBit(*(cpu->F), FLAG_N, 0);
    writeBit(*(cpu->F), FLAG_H, 0); 
    writeBit(*(cpu->F), FLAG_C, bit0); 

    return RRCA_CYCLES;
}

cycles Bit::rla(CPU* cpu){

    byte bit7 = readBit(*(cpu->A), 7);
    byte oldCarry = readBit(*(cpu->F), FLAG_C);
    *(cpu->A) = (*(cpu->A) << 1) | oldCarry; //rotates to the left 

    writeBit(*(cpu->F), FLAG_Z, 0);
    writeBit(*(cpu->F), FLAG_N, 0);
    writeBit(*(cpu->F), FLAG_H, 0); 
    writeBit(*(cpu->F), FLAG_C, bit7); 

    return RLA_CYCLES;
}

cycles Bit::rra(CPU* cpu){

    byte oldCarry = readBit(*(cpu->F), FLAG_C);
    byte bit0 = readBit(*(cpu->A), 0);

    *(cpu->A) = (*(cpu->A) >> 1) | (oldCarry<<7); //rotates to the right 

    writeBit(*(cpu->F), FLAG_Z, 0);
    writeBit(*(cpu->F), FLAG_N, 0);
    writeBit(*(cpu->F), FLAG_H, 0); 
    writeBit(*(cpu->F), FLAG_C, bit0); 

    return RRA_CYCLES;
}


cycles Bit::cpl(CPU* cpu){

    *(cpu->A) = ~(*(cpu->A));

    writeBit(*(cpu->F), FLAG_N, 1);
    writeBit(*(cpu->F), FLAG_H, 1);
    return CPL_CYCLES;
}

cycles Bit::scf(CPU* cpu){
    writeBit(*(cpu->F), FLAG_N, 0);
    writeBit(*(cpu->F), FLAG_H, 0);
    writeBit(*(cpu->F), FLAG_C, 1);
    
    return SCF_CYCLES;
}


cycles Bit::ccf(CPU* cpu){

    writeBit(*(cpu->F), FLAG_N, 0);
    writeBit(*(cpu->F), FLAG_H, 0);
    writeBit(*(cpu->F), FLAG_C, !(readBit(*(cpu->F), FLAG_C)));
    
    return CCF_CYCLES;
}

cycles Bit::prefix_cb(CPU* cpu){

    cycles usedCycles = NORMAL_CYCLES;

    byte opCode = cpu->memory->read(cpu->PC.read());
    cpu->PC++;

    byte insType = (opCode & 0xF8)>>3; // extract the instuction type 
    byte regNumber  = (opCode & 0x7); // extract the regs
    byte bitNumber = (opCode & 0x38)>>3; //extract the bit number

    reg8 regOperand; 

    switch(regNumber){
        case 0x0:
            std::cout << "Reg: B" << std::endl;
            regOperand = cpu->B;
            break;

        case 0x1:
            std::cout << "Reg: C" << std::endl;
            regOperand = cpu->C;
            break;

        case 0x2:
            std::cout << "Reg: D" << std::endl;
            regOperand = cpu->D;
            break;

        case 0x3:
            std::cout << "Reg: E" << std::endl;
            regOperand = cpu->E;
            break;
        
        case 0x4:
            std::cout << "Reg: H" << std::endl;
            regOperand = cpu->H;
            break;

        case 0x5: 
            std::cout << "Reg: L" << std::endl;
            regOperand = cpu->L;
            break;

        case 0x6:
            std::cout << "Reg: (" << cpu->reg_HL.read() << ")" << std::endl;
            regOperand = cpu->memory->getBytePointer(cpu->reg_HL.read());
            usedCycles += mHL_CYCLES;
            break;

        case 0x7:
            std::cout << "Reg: A" << std::endl;
            regOperand = cpu->A;
            break;

        default:
            regOperand = cpu->B;
            break;
    }
       
    if(insType ==  RLC){
        std::cout << "Ins: RLC" << std::endl; 

        byte bit7 = readBit(*(regOperand), 7);
        *(regOperand) = (*(regOperand) << 1) | bit7; //rotates to the left 

        writeBit(*(cpu->F), FLAG_Z, (*regOperand) == 0);
        writeBit(*(cpu->F), FLAG_N, 0);
        writeBit(*(cpu->F), FLAG_H, 0); 
        writeBit(*(cpu->F), FLAG_C, bit7); 
    }

    else if(insType ==  RRC){
        std::cout << "Ins: RRC" << std::endl; 

        byte bit0 = readBit(*(regOperand), 0);

        *(regOperand) = (*(regOperand) >> 1) | (bit0<<7); //rotates to the right 

        writeBit(*(cpu->F), FLAG_Z, *(regOperand) == 0);
        writeBit(*(cpu->F), FLAG_N, 0);
        writeBit(*(cpu->F), FLAG_H, 0); 
        writeBit(*(cpu->F), FLAG_C, bit0); 
    }
    
    else if(insType ==  RL){
        std::cout << "Ins: RL" << std::endl; 

        byte bit7 = readBit(*(regOperand), 7);
        byte oldCarry = readBit(*(cpu->F), FLAG_C);
        *(regOperand) = (*(regOperand) << 1) | oldCarry; //rotates to the left 

        writeBit(*(cpu->F), FLAG_Z, *(regOperand) == 0);
        writeBit(*(cpu->F), FLAG_N, 0);
        writeBit(*(cpu->F), FLAG_H, 0); 
        writeBit(*(cpu->F), FLAG_C, bit7); 
    }

    else if(insType ==  RR){
        std::cout << "Ins: RR" << std::endl; 

        byte oldCarry = readBit(*(cpu->F), FLAG_C);
        byte bit0 = readBit(*(regOperand), 0);

        *(regOperand) = (*(regOperand) >> 1) | (oldCarry<<7); //rotates to the right 

        writeBit(*(cpu->F), FLAG_Z, *(regOperand) == 0);
        writeBit(*(cpu->F), FLAG_N, 0);
        writeBit(*(cpu->F), FLAG_H, 0); 
        writeBit(*(cpu->F), FLAG_C, bit0); 
    }

    else if(insType ==  SLA){
        std::cout << "Ins: SLA" << std::endl;
               
        byte bit7 = readBit(*(regOperand), 7);
        *(regOperand) = (*(regOperand) << 1); //shifts to the left 

        writeBit(*(cpu->F), FLAG_Z, (*regOperand) == 0);
        writeBit(*(cpu->F), FLAG_N, 0);
        writeBit(*(cpu->F), FLAG_H, 0); 
        writeBit(*(cpu->F), FLAG_C, bit7);  
    }

    else if(insType ==  SRA){
        std::cout << "Ins: SRA" << std::endl; 

        byte bit0 = readBit(*(regOperand), 0);
        byte bit7 = readBit(*(regOperand), 7);

        std::cout << bit7 <<std::endl;

        *(regOperand) = (*(regOperand) >> 1) | (bit7 << 7); //airithmetic shifts to the right 

        writeBit(*(cpu->F), FLAG_Z, *(regOperand) == 0);
        writeBit(*(cpu->F), FLAG_N, 0);
        writeBit(*(cpu->F), FLAG_H, 0); 
        writeBit(*(cpu->F), FLAG_C, bit0); 
    }

    else if(insType ==  SWAP){
        std::cout << "Ins: SWAP" << std::endl;

        byte temp  =  *(regOperand) >> 4;
        *(regOperand) = *(regOperand) << 4 | temp; //swaps the top and bottom nybbles 

        writeBit(*(cpu->F), FLAG_Z, *(regOperand) == 0);
        writeBit(*(cpu->F), FLAG_N, 0);
        writeBit(*(cpu->F), FLAG_H, 0); 
        writeBit(*(cpu->F), FLAG_C, 0); 
    }

    else if(insType ==  SRL){
        std::cout << "Ins: SRL" << std::endl; 

        byte bit0 = readBit(*(regOperand), 0);

        *(regOperand) = (*(regOperand) >> 1); //shifts to the right 

        writeBit(*(cpu->F), FLAG_Z, *(regOperand) == 0);
        writeBit(*(cpu->F), FLAG_N, 0);
        writeBit(*(cpu->F), FLAG_H, 0); 
        writeBit(*(cpu->F), FLAG_C, bit0); 
    }

    else if(insType >=  BIT && insType < RES){
        std::cout << "Bit: " << (int) bitNumber << std::endl;
        std::cout << "Ins: BIT" << std::endl; 


        writeBit(*(cpu->F), FLAG_Z, readBit(*(regOperand), bitNumber));
        writeBit(*(cpu->F), FLAG_N, 0);
        writeBit(*(cpu->F), FLAG_H, 1); 
    }

    else if(insType >=  RES && insType < SET){
        std::cout << "Bit: " << (int) bitNumber << std::endl;
        std::cout << "Ins: RES" << std::endl; 


          writeBit(*(regOperand), bitNumber, 0);
    }

    else{
        std::cout << "Bit: " << (int) bitNumber << std::endl;
        std::cout << "Ins: SET" << std::endl;  

        writeBit(*(regOperand), bitNumber, 1);
    }

    return usedCycles;
}