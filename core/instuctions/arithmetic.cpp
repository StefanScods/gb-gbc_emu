/*
a implementation file for the arithmetic instuction for the gb/gbc cpu

date: 2021-11-13
*/
#include "include\arithmetic.h"
#include "..\include\cpu.h"
#include <iostream>

cycles Arithmetic::inc_bc(CPU* cpu){
    cpu->reg_BC++;
    return INC_BC_CYCLES;
}

cycles  Arithmetic::inc_de(CPU* cpu){
    cpu->reg_DE++;
    return INC_DE_CYCLES;
}

cycles Arithmetic::inc_hl(CPU* cpu){
    cpu->reg_HL++;
    return INC_HL_CYCLES;
}

cycles  Arithmetic::inc_sp(CPU* cpu){
    cpu->SP++;
    return INC_SP_CYCLES;
}

cycles Arithmetic::dec_bc(CPU* cpu){
    cpu->reg_BC--;
    return DEC_BC_CYCLES;
}

cycles Arithmetic::dec_de(CPU* cpu){
    cpu->reg_DE--;
    return DEC_DE_CYCLES;
}

cycles Arithmetic::dec_hl(CPU* cpu){
    cpu->reg_HL--;
    return DEC_HL_CYCLES;
}
cycles Arithmetic::dec_sp(CPU* cpu){
    cpu->SP--;
    return DEC_SP_CYCLES;
}

cycles Arithmetic::inc_a(CPU* cpu){
    (*(cpu->A))++;

    //update flags 
    writeBit(*(cpu->F), FLAG_Z, *(cpu->A)==0);
    writeBit(*(cpu->F), FLAG_N, 0);
    writeBit(*(cpu->F), FLAG_H, (*(cpu->A) & 0xF) == 0);

    return INC_A_CYCLES;
}


cycles Arithmetic::inc_c(CPU* cpu){
    (*(cpu->C))++;

    //update flags 
    writeBit(*(cpu->F), FLAG_Z, *(cpu->C)==0);
    writeBit(*(cpu->F), FLAG_N, 0);
    writeBit(*(cpu->F), FLAG_H, (*(cpu->C) & 0xF) == 0);

    return INC_C_CYCLES;
}

cycles Arithmetic::inc_b(CPU* cpu){
    (*(cpu->B))++;

    //update flags 
    writeBit(*(cpu->F), FLAG_Z, *(cpu->B)==0);
    writeBit(*(cpu->F), FLAG_N, 0);
    writeBit(*(cpu->F), FLAG_H, (*(cpu->B) & 0xF) == 0);

    return INC_B_CYCLES;
}

cycles Arithmetic::inc_d(CPU* cpu){
    (*(cpu->D))++;

    //update flags 
    writeBit(*(cpu->F), FLAG_Z, *(cpu->D)==0);
    writeBit(*(cpu->F), FLAG_N, 0);
    writeBit(*(cpu->F), FLAG_H, (*(cpu->D) & 0xF) == 0);

    return INC_D_CYCLES;
}

cycles Arithmetic::inc_e(CPU* cpu){
    (*(cpu->E))++;

    //update flags 
    writeBit(*(cpu->F), FLAG_Z, *(cpu->E)==0);
    writeBit(*(cpu->F), FLAG_N, 0);
    writeBit(*(cpu->F), FLAG_H, (*(cpu->E) & 0xF) == 0);

    return INC_E_CYCLES;
}

cycles Arithmetic::inc_h(CPU* cpu){
    (*(cpu->H))++;

    //update flags 
    writeBit(*(cpu->F), FLAG_Z, *(cpu->H)==0);
    writeBit(*(cpu->F), FLAG_N, 0);
    writeBit(*(cpu->F), FLAG_H, (*(cpu->H) & 0xF) == 0);

    return INC_H_CYCLES;
}

cycles Arithmetic::inc_l(CPU* cpu){
    (*(cpu->L))++;

    //update flags 
    writeBit(*(cpu->F), FLAG_Z, *(cpu->L)==0);
    writeBit(*(cpu->F), FLAG_N, 0);
    writeBit(*(cpu->F), FLAG_H, (*(cpu->L) & 0xF) == 0);

    return INC_L_CYCLES;
}

cycles Arithmetic::dec_a(CPU* cpu){
    (*(cpu->A))--;

    //update flags 
    writeBit(*(cpu->F), FLAG_Z, *(cpu->A) == 0);
    writeBit(*(cpu->F), FLAG_N, 1);
    writeBit(*(cpu->F), FLAG_H, (*(cpu->A) & 0xF) == 0xF);

    return DEC_A_CYCLES;
}

cycles Arithmetic::dec_b(CPU* cpu){
    (*(cpu->B))--;

    //update flags 
    writeBit(*(cpu->F), FLAG_Z, *(cpu->B) == 0);
    writeBit(*(cpu->F), FLAG_N, 1);
    writeBit(*(cpu->F), FLAG_H, (*(cpu->B) & 0xF) == 0xF);

    return DEC_B_CYCLES;
}

cycles Arithmetic::dec_c(CPU* cpu){
    (*(cpu->C))--;

    //update flags 
    writeBit(*(cpu->F), FLAG_Z, *(cpu->C) == 0);
    writeBit(*(cpu->F), FLAG_N, 1);
    writeBit(*(cpu->F), FLAG_H, (*(cpu->C) & 0xF) == 0xF);

    return DEC_C_CYCLES;
}

cycles Arithmetic::dec_d(CPU* cpu){
    (*(cpu->D))--;

    //update flags 
    writeBit(*(cpu->F), FLAG_Z, *(cpu->D) == 0);
    writeBit(*(cpu->F), FLAG_N, 1);
    writeBit(*(cpu->F), FLAG_H, (*(cpu->D) & 0xF) == 0xF);

    return DEC_D_CYCLES;
}

cycles Arithmetic::dec_e(CPU* cpu){
    (*(cpu->E))--;

    //update flags 
    writeBit(*(cpu->F), FLAG_Z, *(cpu->E) == 0);
    writeBit(*(cpu->F), FLAG_N, 1);
    writeBit(*(cpu->F), FLAG_H, (*(cpu->E) & 0xF) == 0xF);

    return DEC_E_CYCLES;
}

cycles Arithmetic::dec_h(CPU* cpu){
    (*(cpu->H))--;

    //update flags 
    writeBit(*(cpu->F), FLAG_Z, *(cpu->H) == 0);
    writeBit(*(cpu->F), FLAG_N, 1);
    writeBit(*(cpu->F), FLAG_H, (*(cpu->H) & 0xF) == 0xF);

    return DEC_H_CYCLES;
}

cycles Arithmetic::dec_l(CPU* cpu){
    (*(cpu->L))--;

    //update flags 
    writeBit(*(cpu->F), FLAG_Z, *(cpu->L) == 0);
    writeBit(*(cpu->F), FLAG_N, 1);
    writeBit(*(cpu->F), FLAG_H, (*(cpu->L) & 0xF) == 0xF);

    return DEC_L_CYCLES;
}

cycles Arithmetic::add_hl_bc(CPU* cpu){

    //do arithmetic in a larger buffer so we can check for overflow
    uint32_t result = (uint32_t) cpu->reg_HL.read() + (uint32_t) cpu->reg_BC.read(); 

    writeBit(*(cpu->F), FLAG_N, 0);
    writeBit(*(cpu->F), FLAG_C, (result > 0xFFFF)); //carry flag if result is greater than a 16 bit number 
    writeBit(*(cpu->F), FLAG_H, ((((cpu->reg_HL.read() & 0xFFF) + (cpu->reg_BC.read() & 0xFFF)) > 0xFFF))); //half carry flag if the 12th bit overflows -> !!! maybe find a better method 

    cpu->reg_HL = (word) (result & 0xFFFF); //take the bottom 16 bits 

    return ADD_HL_BC_CYCLES;
}



cycles Arithmetic::add_hl_de(CPU* cpu){
    //do arithmetic in a larger buffer so we can check for overflow
    uint32_t result = (uint32_t) cpu->reg_HL.read() + (uint32_t) cpu->reg_DE.read(); 

    writeBit(*(cpu->F), FLAG_N, 0);
    writeBit(*(cpu->F), FLAG_C, (result > 0xFFFF)); //carry flag if result is greater than a 16 bit number 
    writeBit(*(cpu->F), FLAG_H, ((((cpu->reg_HL.read() & 0xFFF) + (cpu->reg_DE.read() & 0xFFF)) > 0xFFF))); //half carry flag if the 12th bit overflows -> !!! maybe find a better method 

    cpu->reg_HL = (word) (result & 0xFFFF); //take the bottom 16 bits 

    return ADD_HL_DE_CYCLES;
}

cycles Arithmetic::add_hl_hl(CPU* cpu){
    //do arithmetic in a larger buffer so we can check for overflow
    uint32_t result = (uint32_t) cpu->reg_HL.read() + (uint32_t) cpu->reg_HL.read(); 

    writeBit(*(cpu->F), FLAG_N, 0);
    writeBit(*(cpu->F), FLAG_C, (result > 0xFFFF)); //carry flag if result is greater than a 16 bit number 
    writeBit(*(cpu->F), FLAG_H, ((((cpu->reg_HL.read() & 0xFFF) + (cpu->reg_HL.read() & 0xFFF)) > 0xFFF))); //half carry flag if the 12th bit overflows -> !!! maybe find a better method 

    cpu->reg_HL = (word) (result & 0xFFFF); //take the bottom 16 bits 

    return ADD_HL_HL_CYCLES;
}

cycles Arithmetic::add_hl_sp(CPU* cpu){
    //do arithmetic in a larger buffer so we can check for overflow
    uint32_t result = (uint32_t) cpu->reg_HL.read() + (uint32_t) cpu->SP.read(); 

    writeBit(*(cpu->F), FLAG_N, 0);
    writeBit(*(cpu->F), FLAG_C, (result > 0xFFFF)); //carry flag if result is greater than a 16 bit number 
    writeBit(*(cpu->F), FLAG_H, ((((cpu->reg_HL.read() & 0xFFF) + (cpu->SP.read() & 0xFFF)) > 0xFFF))); //half carry flag if the 12th bit overflows -> !!! maybe find a better method 

    cpu->reg_HL = (word) (result & 0xFFFF); //take the bottom 16 bits 

    return ADD_HL_SP_CYCLES;
}


 cycles Arithmetic::daa(CPU* cpu){ 
    byte correctionOffset = 0;

    if(readBit(*(cpu->F), FLAG_C) || (!readBit(*(cpu->F), FLAG_N) && (*(cpu->A)) > 0x99)){
        correctionOffset |= 0x60;
        writeBit(*(cpu->F), FLAG_C, 1);
    } 
    if(readBit(*(cpu->F), FLAG_H) || (!readBit(*(cpu->F), FLAG_N) && (*(cpu->A) & 0xF ) > 0x9 )){
        correctionOffset |= 0x6;
    }

    if(readBit(*(cpu->F), FLAG_N))  *(cpu->A)-=correctionOffset;
    else *(cpu->A)+=correctionOffset;

    writeBit(*(cpu->F), FLAG_Z,  *(cpu->A) == 0);
    writeBit(*(cpu->F), FLAG_H, 0);

    return DAA_CYCLES;
 }

     
cycles Arithmetic::inc_mhl(CPU* cpu){

    byte temp = cpu->memory->read( cpu->reg_HL.read());
    temp++;
    cpu->memory->write( cpu->reg_HL.read(), temp);

    //update flags 
    writeBit(*(cpu->F), FLAG_Z, temp == 0);
    writeBit(*(cpu->F), FLAG_N, 0);
    writeBit(*(cpu->F), FLAG_H, (temp & 0xF) == 0);

    return INC_mHL_CYCLES;
}

cycles Arithmetic::dec_mhl(CPU* cpu){

    byte temp = cpu->memory->read( cpu->reg_HL.read());
    temp--;
    cpu->memory->write( cpu->reg_HL.read(), temp);

    //update flags 
    writeBit(*(cpu->F), FLAG_Z, temp == 0);
    writeBit(*(cpu->F), FLAG_N, 1);
    writeBit(*(cpu->F), FLAG_H, (temp & 0xF) == 0xF);

    return DEC_mHL_CYCLES;
}

cycles Arithmetic::add_a_a(CPU* cpu){
    word result = *(cpu->A) + *(cpu->A);

    bool carry = result > 0xFF;
    
    result = result & 0xFF;

    writeBit(*(cpu->F), FLAG_Z, result == 0);
    writeBit(*(cpu->F), FLAG_N, 0);
    writeBit(*(cpu->F), FLAG_C, carry);
    writeBit(*(cpu->F), FLAG_H, ((((*(cpu->A) & 0xF) + (*(cpu->A) & 0xF)) > 0xF)));

    *(cpu->A) = (byte) result;

    return ADD_A_A_CYCLES;
}

cycles Arithmetic::add_a_b(CPU* cpu){
    word result = *(cpu->A) + *(cpu->B);

    bool carry = result > 0xFF;
    
    result = result & 0xFF;

    writeBit(*(cpu->F), FLAG_Z, result == 0);
    writeBit(*(cpu->F), FLAG_N, 0);
    writeBit(*(cpu->F), FLAG_C, carry);
    writeBit(*(cpu->F), FLAG_H, ((((*(cpu->A) & 0xF) + (*(cpu->B) & 0xF)) > 0xF)));

    *(cpu->A) = (byte) result;

    return ADD_A_B_CYCLES;
}

cycles Arithmetic::add_a_c(CPU* cpu){
    word result = *(cpu->A) + *(cpu->C);

    bool carry = result > 0xFF;
    
    result = result & 0xFF;

    writeBit(*(cpu->F), FLAG_Z, result == 0);
    writeBit(*(cpu->F), FLAG_N, 0);
    writeBit(*(cpu->F), FLAG_C, carry);
    writeBit(*(cpu->F), FLAG_H, ((((*(cpu->A) & 0xF) + (*(cpu->C) & 0xF)) > 0xF)));

    *(cpu->A) = (byte) result;

    return ADD_A_C_CYCLES;
}

cycles Arithmetic::add_a_d(CPU* cpu){
    word result = *(cpu->A) + *(cpu->D);

    bool carry = result > 0xFF;
    
    result = result & 0xFF;

    writeBit(*(cpu->F), FLAG_Z, result == 0);
    writeBit(*(cpu->F), FLAG_N, 0);
    writeBit(*(cpu->F), FLAG_C, carry);
    writeBit(*(cpu->F), FLAG_H, ((((*(cpu->A) & 0xF) + (*(cpu->D) & 0xF)) > 0xF)));

    *(cpu->A) = (byte) result;

    return ADD_A_D_CYCLES;
}

cycles Arithmetic::add_a_e(CPU* cpu){
    word result = *(cpu->A) + *(cpu->E);

    bool carry = result > 0xFF;
    
    result = result & 0xFF;

    writeBit(*(cpu->F), FLAG_Z, result == 0);
    writeBit(*(cpu->F), FLAG_N, 0);
    writeBit(*(cpu->F), FLAG_C, carry);
    writeBit(*(cpu->F), FLAG_H, ((((*(cpu->A) & 0xF) + (*(cpu->E) & 0xF)) > 0xF)));

    *(cpu->A) = (byte) result;

    return ADD_A_E_CYCLES;
}

cycles Arithmetic::add_a_h(CPU* cpu){
    word result = *(cpu->A) + *(cpu->H);

    bool carry = result > 0xFF;
    
    result = result & 0xFF;

    writeBit(*(cpu->F), FLAG_Z, result == 0);
    writeBit(*(cpu->F), FLAG_N, 0);
    writeBit(*(cpu->F), FLAG_C, carry);
    writeBit(*(cpu->F), FLAG_H, ((((*(cpu->A) & 0xF) + (*(cpu->H) & 0xF)) > 0xF)));

    *(cpu->A) = (byte) result;

    return ADD_A_H_CYCLES;
}

cycles Arithmetic::add_a_l(CPU* cpu){
    word result = *(cpu->A) + *(cpu->L);

    bool carry = result > 0xFF;
    
    result = result & 0xFF;

    writeBit(*(cpu->F), FLAG_Z, result == 0);
    writeBit(*(cpu->F), FLAG_N, 0);
    writeBit(*(cpu->F), FLAG_C, carry);
    writeBit(*(cpu->F), FLAG_H, ((((*(cpu->A) & 0xF) + (*(cpu->L) & 0xF)) > 0xF)));

    *(cpu->A) = (byte) result;

    return ADD_A_L_CYCLES;
}

cycles Arithmetic::add_a_mhl(CPU* cpu){
    byte data = cpu->memory->read(cpu->reg_HL.read());
    word result = *(cpu->A) + data;

    bool carry = result > 0xFF;
    
    result = result & 0xFF;

    writeBit(*(cpu->F), FLAG_Z, result == 0);
    writeBit(*(cpu->F), FLAG_N, 0);
    writeBit(*(cpu->F), FLAG_C, carry);
    writeBit(*(cpu->F), FLAG_H, ((((*(cpu->A) & 0xF) + (data & 0xF)) > 0xF)));

    *(cpu->A) = (byte) result;

    return ADD_A_mHL_CYCLES;
}

cycles Arithmetic::adc_a_a(CPU* cpu){

    bool CY = readBit(*(cpu->F), FLAG_C);

    word result = *(cpu->A) + *(cpu->A) + CY;

    bool carry = result > 0xFF;
    
    result = result & 0xFF;

    writeBit(*(cpu->F), FLAG_Z, result == 0);
    writeBit(*(cpu->F), FLAG_N, 0);
    writeBit(*(cpu->F), FLAG_C, carry);
    writeBit(*(cpu->F), FLAG_H, ((((*(cpu->A) & 0xF) + (*(cpu->A) & 0xF) + CY) > 0xF)));

    *(cpu->A) = (byte) result;

    return ADC_A_A_CYCLES;
}

cycles Arithmetic::adc_a_b(CPU* cpu){

    bool CY = readBit(*(cpu->F), FLAG_C);

    word result = *(cpu->A) + *(cpu->B) + CY;

    bool carry = result > 0xFF;
    
    result = result & 0xFF;

    writeBit(*(cpu->F), FLAG_Z, result == 0);
    writeBit(*(cpu->F), FLAG_N, 0);
    writeBit(*(cpu->F), FLAG_C, carry);
    writeBit(*(cpu->F), FLAG_H, ((((*(cpu->A) & 0xF) + (*(cpu->B) & 0xF) + CY) > 0xF)));

    *(cpu->A) = (byte) result;

    return ADC_A_B_CYCLES;
}

cycles Arithmetic::adc_a_c(CPU* cpu){
    
    bool CY = readBit(*(cpu->F), FLAG_C);

    word result = *(cpu->A) + *(cpu->C) + CY;

    bool carry = result > 0xFF;
    
    result = result & 0xFF;

    writeBit(*(cpu->F), FLAG_Z, result == 0);
    writeBit(*(cpu->F), FLAG_N, 0);
    writeBit(*(cpu->F), FLAG_C, carry);
    writeBit(*(cpu->F), FLAG_H, ((((*(cpu->A) & 0xF) + (*(cpu->C) & 0xF) + CY) > 0xF)));

    *(cpu->A) = (byte) result;

    return ADC_A_C_CYCLES;
}

cycles Arithmetic::adc_a_d(CPU* cpu){
    
    bool CY = readBit(*(cpu->F), FLAG_C);

    word result = *(cpu->A) + *(cpu->D) + CY;

    bool carry = result > 0xFF;
    
    result = result & 0xFF;

    writeBit(*(cpu->F), FLAG_Z, result == 0);
    writeBit(*(cpu->F), FLAG_N, 0);
    writeBit(*(cpu->F), FLAG_C, carry);
    writeBit(*(cpu->F), FLAG_H, ((((*(cpu->A) & 0xF) + (*(cpu->D) & 0xF) + CY) > 0xF)));

    *(cpu->A) = (byte) result;

    return ADC_A_D_CYCLES;
}

cycles Arithmetic::adc_a_e(CPU* cpu){
    
    bool CY = readBit(*(cpu->F), FLAG_C);

    word result = *(cpu->A) + *(cpu->E) + CY;

    bool carry = result > 0xFF;
    
    result = result & 0xFF;

    writeBit(*(cpu->F), FLAG_Z, result == 0);
    writeBit(*(cpu->F), FLAG_N, 0);
    writeBit(*(cpu->F), FLAG_C, carry);
    writeBit(*(cpu->F), FLAG_H, ((((*(cpu->A) & 0xF) + (*(cpu->E) & 0xF) + CY) > 0xF)));

    *(cpu->A) = (byte) result;

    return ADC_A_E_CYCLES;
}

cycles Arithmetic::adc_a_h(CPU* cpu){
    
    bool CY = readBit(*(cpu->F), FLAG_C);

    word result = *(cpu->A) + *(cpu->H) + CY;

    bool carry = result > 0xFF;
    
    result = result & 0xFF;

    writeBit(*(cpu->F), FLAG_Z, result == 0);
    writeBit(*(cpu->F), FLAG_N, 0);
    writeBit(*(cpu->F), FLAG_C, carry);
    writeBit(*(cpu->F), FLAG_H, ((((*(cpu->A) & 0xF) + (*(cpu->H) & 0xF) + CY) > 0xF)));

    *(cpu->A) = (byte) result;

    return ADC_A_H_CYCLES;
}

cycles Arithmetic::adc_a_l(CPU* cpu){
    
    bool CY = readBit(*(cpu->F), FLAG_C);

    word result = *(cpu->A) + *(cpu->L) + CY;

    bool carry = result > 0xFF;
    
    result = result & 0xFF;

    writeBit(*(cpu->F), FLAG_Z, result == 0);
    writeBit(*(cpu->F), FLAG_N, 0);
    writeBit(*(cpu->F), FLAG_C, carry);
    writeBit(*(cpu->F), FLAG_H, ((((*(cpu->A) & 0xF) + (*(cpu->L) & 0xF) + CY) > 0xF)));

    *(cpu->A) = (byte) result;

    return ADC_A_L_CYCLES;
}

cycles Arithmetic::adc_a_mhl(CPU* cpu){
    
    bool CY = readBit(*(cpu->F), FLAG_C);

    byte data = cpu->memory->read(cpu->reg_HL.read());
    word result = *(cpu->A) + data + CY;

    bool carry = result > 0xFF;
    
    result = result & 0xFF;

    writeBit(*(cpu->F), FLAG_Z, result == 0);
    writeBit(*(cpu->F), FLAG_N, 0);
    writeBit(*(cpu->F), FLAG_C, carry);
    writeBit(*(cpu->F), FLAG_H, ((((*(cpu->A) & 0xF) + (data & 0xF) + CY) > 0xF)));

    *(cpu->A) = (byte) result;

    return ADC_A_mHL_CYCLES;
}

cycles Arithmetic::sub_a_a(CPU* cpu){

    *(cpu->A) = 0;
    *(cpu->F) = 0b11000000; //set Z=1,N=1,H=0,C=0

    return SUB_A_CYCLES;
}

cycles Arithmetic::sub_a_b(CPU* cpu){

    byte temp = *(cpu->A);

    *(cpu->A) = *(cpu->A) - *(cpu->B);

    writeBit(*(cpu->F), FLAG_Z, *(cpu->A) == 0);
    writeBit(*(cpu->F), FLAG_N, 1);
    writeBit(*(cpu->F), FLAG_H,  (temp & 0xF) < (*(cpu->B) & 0xF));
    writeBit(*(cpu->F), FLAG_C,  temp < *(cpu->B));

    return SUB_B_CYCLES;
}

cycles Arithmetic::sub_a_c(CPU* cpu){

    byte temp = *(cpu->A);

    *(cpu->A) = *(cpu->A) - *(cpu->C);

    writeBit(*(cpu->F), FLAG_Z, *(cpu->A) == 0);
    writeBit(*(cpu->F), FLAG_N, 1);
    writeBit(*(cpu->F), FLAG_H,  (temp & 0xF) < (*(cpu->C) & 0xF));
    writeBit(*(cpu->F), FLAG_C,  temp < *(cpu->C));

    return SUB_C_CYCLES;
}
cycles Arithmetic::sub_a_d(CPU* cpu){

    byte temp = *(cpu->A);

    *(cpu->A) = *(cpu->A) - *(cpu->D);

    writeBit(*(cpu->F), FLAG_Z, *(cpu->A) == 0);
    writeBit(*(cpu->F), FLAG_N, 1);
    writeBit(*(cpu->F), FLAG_H,  (temp & 0xF) < (*(cpu->D) & 0xF));
    writeBit(*(cpu->F), FLAG_C,  temp < *(cpu->D));

    return SUB_D_CYCLES;
}
cycles Arithmetic::sub_a_e(CPU* cpu){

    byte temp = *(cpu->A);

    *(cpu->A) = *(cpu->A) - *(cpu->E);

    writeBit(*(cpu->F), FLAG_Z, *(cpu->A) == 0);
    writeBit(*(cpu->F), FLAG_N, 1);
    writeBit(*(cpu->F), FLAG_H,  (temp & 0xF) < (*(cpu->E) & 0xF));
    writeBit(*(cpu->F), FLAG_C,  temp < *(cpu->E));

    return SUB_E_CYCLES;
}
cycles Arithmetic::sub_a_h(CPU* cpu){

    byte temp = *(cpu->A);

    *(cpu->A) = *(cpu->A) - *(cpu->H);

    writeBit(*(cpu->F), FLAG_Z, *(cpu->A) == 0);
    writeBit(*(cpu->F), FLAG_N, 1);
    writeBit(*(cpu->F), FLAG_H,  (temp & 0xF) < (*(cpu->H) & 0xF));
    writeBit(*(cpu->F), FLAG_C,  temp < *(cpu->H));

    return SUB_H_CYCLES;
}
cycles Arithmetic::sub_a_l(CPU* cpu){

    byte temp = *(cpu->A);

    *(cpu->A) = *(cpu->A) - *(cpu->L);

    writeBit(*(cpu->F), FLAG_Z, *(cpu->A) == 0);
    writeBit(*(cpu->F), FLAG_N, 1);
    writeBit(*(cpu->F), FLAG_H,  (temp & 0xF) < (*(cpu->L) & 0xF));
    writeBit(*(cpu->F), FLAG_C,  temp < *(cpu->L));

    return SUB_L_CYCLES;
}

cycles Arithmetic::sub_a_mhl(CPU* cpu){

    byte temp = *(cpu->A);
    byte data = cpu->memory->read(cpu->reg_HL.read());

    *(cpu->A) = *(cpu->A) - data;

    writeBit(*(cpu->F), FLAG_Z, *(cpu->A) == 0);
    writeBit(*(cpu->F), FLAG_N, 1);
    writeBit(*(cpu->F), FLAG_H,  (temp & 0xF) < (data & 0xF));
    writeBit(*(cpu->F), FLAG_C,  temp < data);

    return SUB_mHL_CYCLES;
}


cycles Arithmetic::sbc_a_a(CPU* cpu){

    bool CY = readBit(*(cpu->F), FLAG_C);
    *(cpu->A) = 0 - CY;
   
    writeBit(*(cpu->F), FLAG_Z, !CY);
    writeBit(*(cpu->F), FLAG_N, 1);
    writeBit(*(cpu->F), FLAG_H, CY);
    writeBit(*(cpu->F), FLAG_C, CY);

    return SBC_A_A_CYCLES;
}

cycles Arithmetic::sbc_a_b(CPU* cpu){

    bool CY = readBit(*(cpu->F), FLAG_C);    
    byte temp = *(cpu->A);

    *(cpu->A) = *(cpu->A) - *(cpu->B) - CY;

    writeBit(*(cpu->F), FLAG_Z, *(cpu->A) == 0);
    writeBit(*(cpu->F), FLAG_N, 1);
    writeBit(*(cpu->F), FLAG_H,  (temp & 0xF) < (*(cpu->B) & 0xF) + CY);
    writeBit(*(cpu->F), FLAG_C,  temp < *(cpu->B) + CY);

    return SBC_A_B_CYCLES;
}

cycles Arithmetic::sbc_a_c(CPU* cpu){

    bool CY = readBit(*(cpu->F), FLAG_C);    
    byte temp = *(cpu->A);

    *(cpu->A) = *(cpu->A) - *(cpu->C) - CY;

    writeBit(*(cpu->F), FLAG_Z, *(cpu->A) == 0);
    writeBit(*(cpu->F), FLAG_N, 1);
    writeBit(*(cpu->F), FLAG_H,  (temp & 0xF) < (*(cpu->C) & 0xF) + CY);
    writeBit(*(cpu->F), FLAG_C,  temp < *(cpu->C) + CY);

    return SBC_A_C_CYCLES;
}
cycles Arithmetic::sbc_a_d(CPU* cpu){

    bool CY = readBit(*(cpu->F), FLAG_C); 
    byte temp = *(cpu->A);

    *(cpu->A) = *(cpu->A) - *(cpu->D) - CY;

    writeBit(*(cpu->F), FLAG_Z, *(cpu->A) == 0);
    writeBit(*(cpu->F), FLAG_N, 1);
    writeBit(*(cpu->F), FLAG_H,  (temp & 0xF) < (*(cpu->D) & 0xF) + CY);
    writeBit(*(cpu->F), FLAG_C,  temp < *(cpu->D) + CY);

    return SBC_A_D_CYCLES;
}
cycles Arithmetic::sbc_a_e(CPU* cpu){

    bool CY = readBit(*(cpu->F), FLAG_C); 
    byte temp = *(cpu->A);

    *(cpu->A) = *(cpu->A) - *(cpu->E) - CY;

    writeBit(*(cpu->F), FLAG_Z, *(cpu->A) == 0);
    writeBit(*(cpu->F), FLAG_N, 1);
    writeBit(*(cpu->F), FLAG_H,  (temp & 0xF) < (*(cpu->E) & 0xF) + CY);
    writeBit(*(cpu->F), FLAG_C,  temp < *(cpu->E) + CY);

    return SBC_A_E_CYCLES;
}
cycles Arithmetic::sbc_a_h(CPU* cpu){

    bool CY = readBit(*(cpu->F), FLAG_C); 
    byte temp = *(cpu->A);

    *(cpu->A) = *(cpu->A) - *(cpu->H) - CY;

    writeBit(*(cpu->F), FLAG_Z, *(cpu->A) == 0);
    writeBit(*(cpu->F), FLAG_N, 1);
    writeBit(*(cpu->F), FLAG_H,  (temp & 0xF) < (*(cpu->H) & 0xF) + CY);
    writeBit(*(cpu->F), FLAG_C,  temp < *(cpu->H )+ CY);

    return SBC_A_H_CYCLES;
}
cycles Arithmetic::sbc_a_l(CPU* cpu){

    bool CY = readBit(*(cpu->F), FLAG_C); 
    byte temp = *(cpu->A);

    *(cpu->A) = *(cpu->A) - *(cpu->L) - CY;

    writeBit(*(cpu->F), FLAG_Z, *(cpu->A) == 0);
    writeBit(*(cpu->F), FLAG_N, 1);
    writeBit(*(cpu->F), FLAG_H,  (temp & 0xF) < (*(cpu->L) & 0xF) + CY);
    writeBit(*(cpu->F), FLAG_C,  temp < *(cpu->L)+ CY);

    return SBC_A_L_CYCLES;
}

cycles Arithmetic::sbc_a_mhl(CPU* cpu){

    bool CY = readBit(*(cpu->F), FLAG_C); 
    byte temp = *(cpu->A);
    byte data = cpu->memory->read(cpu->reg_HL.read());

    *(cpu->A) = *(cpu->A) - data - CY;

    writeBit(*(cpu->F), FLAG_Z, *(cpu->A) == 0);
    writeBit(*(cpu->F), FLAG_N, 1);
    writeBit(*(cpu->F), FLAG_H,  (temp & 0xF) < (data & 0xF) + CY);
    writeBit(*(cpu->F), FLAG_C,  temp < data + CY);

    return SBC_A_mHL_CYCLES;
}

cycles Arithmetic::and_a_a(CPU* cpu){
    *(cpu->F) = 0b00100000; //set N=0, H=1, C=0
    writeBit(*(cpu->F), FLAG_Z, *(cpu->A) == 0);

    return AND_A_CYCLES;
}

cycles Arithmetic::and_a_b(CPU* cpu){

    *(cpu->A) = *(cpu->A) & *(cpu->B);

    *(cpu->F) = 0b00100000; //set N=0, H=1, C=0
    writeBit(*(cpu->F), FLAG_Z, *(cpu->A) == 0);

    return AND_B_CYCLES;
}

cycles Arithmetic::and_a_c(CPU* cpu){

    *(cpu->A) = *(cpu->A) & *(cpu->C);

    *(cpu->F) = 0b00100000; //set N=0, H=1, C=0
    writeBit(*(cpu->F), FLAG_Z, *(cpu->A) == 0);

    return AND_C_CYCLES;
}

cycles Arithmetic::and_a_d(CPU* cpu){

    *(cpu->A) = *(cpu->A) & *(cpu->D);

    *(cpu->F) = 0b00100000; //set N=0, H=1, C=0
    writeBit(*(cpu->F), FLAG_Z, *(cpu->A) == 0);

    return AND_D_CYCLES;
}

cycles Arithmetic::and_a_e(CPU* cpu){

    *(cpu->A) = *(cpu->A) & *(cpu->E);

    *(cpu->F) = 0b00100000; //set N=0, H=1, C=0
    writeBit(*(cpu->F), FLAG_Z, *(cpu->A) == 0);

    return AND_E_CYCLES;
}

cycles Arithmetic::and_a_h(CPU* cpu){

    *(cpu->A) = *(cpu->A) & *(cpu->H);

    *(cpu->F) = 0b00100000; //set N=0, H=1, C=0
    writeBit(*(cpu->F), FLAG_Z, *(cpu->A) == 0);

    return AND_H_CYCLES;
}

cycles Arithmetic::and_a_l(CPU* cpu){

    *(cpu->A) = *(cpu->A) & *(cpu->L);

    *(cpu->F) = 0b00100000; //set N=0, H=1, C=0
    writeBit(*(cpu->F), FLAG_Z, *(cpu->A) == 0);

    return AND_L_CYCLES;
}

cycles Arithmetic::and_a_mhl(CPU* cpu){

    byte data = cpu->memory->read(cpu->reg_HL.read());
    *(cpu->A) = *(cpu->A) & data;

    *(cpu->F) = 0b00100000; //set N=0, H=1, C=0
    writeBit(*(cpu->F), FLAG_Z, *(cpu->A) == 0);

    return AND_mHL_CYCLES;
}

cycles Arithmetic::xor_a_a(CPU* cpu){
    *(cpu->A) = *(cpu->A) ^ *(cpu->A);

    *(cpu->F) = 0; //set all flags to 0 
    writeBit(*(cpu->F), FLAG_Z, *(cpu->A) == 0);

    return XOR_A_CYCLES;
}

cycles Arithmetic::xor_a_b(CPU* cpu){
    *(cpu->A) = *(cpu->A) ^ *(cpu->B);

    *(cpu->F) = 0; //set all flags to 0 
    writeBit(*(cpu->F), FLAG_Z, *(cpu->A) == 0);

    return XOR_B_CYCLES;
}

cycles Arithmetic::xor_a_c(CPU* cpu){
    *(cpu->A) = *(cpu->A) ^ *(cpu->C);

    *(cpu->F) = 0; //set all flags to 0 
    writeBit(*(cpu->F), FLAG_Z, *(cpu->A) == 0);

    return XOR_C_CYCLES;
}

cycles Arithmetic::xor_a_d(CPU* cpu){
    *(cpu->A) = *(cpu->A) ^ *(cpu->D);

    *(cpu->F) = 0; //set all flags to 0 
    writeBit(*(cpu->F), FLAG_Z, *(cpu->A) == 0);

    return XOR_D_CYCLES;
}

cycles Arithmetic::xor_a_e(CPU* cpu){
    *(cpu->A) = *(cpu->A) ^ *(cpu->E);

    *(cpu->F) = 0; //set all flags to 0 
    writeBit(*(cpu->F), FLAG_Z, *(cpu->A) == 0);

    return XOR_E_CYCLES;
}

cycles Arithmetic::xor_a_h(CPU* cpu)
{
    *(cpu->A) = *(cpu->A) ^ *(cpu->H);

    *(cpu->F) = 0; //set all flags to 0 
    writeBit(*(cpu->F), FLAG_Z, *(cpu->A) == 0);

    return XOR_H_CYCLES;
}

cycles Arithmetic::xor_a_l(CPU* cpu){
    *(cpu->A) = *(cpu->A) ^ *(cpu->L);

    *(cpu->F) = 0; //set all flags to 0 
    writeBit(*(cpu->F), FLAG_Z, *(cpu->A) == 0);

    return XOR_L_CYCLES;
}

cycles Arithmetic::xor_a_mhl(CPU* cpu){

    byte data = cpu->memory->read(cpu->reg_HL.read());
    *(cpu->A) = *(cpu->A) ^ data;

    *(cpu->F) = 0; //set all flags to 0 
    writeBit(*(cpu->F), FLAG_Z, *(cpu->A) == 0);

    return XOR_mHL_CYCLES;
}

cycles Arithmetic::or_a_a(CPU* cpu){
    *(cpu->A) = *(cpu->A) | *(cpu->A);

    *(cpu->F) = 0; //set all flags to 0 
    writeBit(*(cpu->F), FLAG_Z, *(cpu->A) == 0);

    return OR_A_CYCLES;
}

cycles Arithmetic::or_a_b(CPU* cpu){
    *(cpu->A) = *(cpu->A) | *(cpu->B);

    *(cpu->F) = 0; //set all flags to 0 
    writeBit(*(cpu->F), FLAG_Z, *(cpu->A) == 0);

    return OR_B_CYCLES;
}

cycles Arithmetic::or_a_c(CPU* cpu){
    *(cpu->A) = *(cpu->A) | *(cpu->C);

    *(cpu->F) = 0; //set all flags to 0 
    writeBit(*(cpu->F), FLAG_Z, *(cpu->A) == 0);

    return OR_C_CYCLES;
}

cycles Arithmetic::or_a_d(CPU* cpu){
    *(cpu->A) = *(cpu->A) | *(cpu->D);

    *(cpu->F) = 0; //set all flags to 0 
    writeBit(*(cpu->F), FLAG_Z, *(cpu->A) == 0);

    return OR_D_CYCLES;
}

cycles Arithmetic::or_a_e(CPU* cpu){
    *(cpu->A) = *(cpu->A) | *(cpu->E);

    *(cpu->F) = 0; //set all flags to 0 
    writeBit(*(cpu->F), FLAG_Z, *(cpu->A) == 0);

    return OR_E_CYCLES;
}

cycles Arithmetic::or_a_h(CPU* cpu)
{
    *(cpu->A) = *(cpu->A) | *(cpu->H);

    *(cpu->F) = 0; //set all flags to 0 
    writeBit(*(cpu->F), FLAG_Z, *(cpu->A) == 0);

    return OR_H_CYCLES;
}

cycles Arithmetic::or_a_l(CPU* cpu){
    *(cpu->A) = *(cpu->A) | *(cpu->L);

    *(cpu->F) = 0; //set all flags to 0 
    writeBit(*(cpu->F), FLAG_Z, *(cpu->A) == 0);

    return OR_L_CYCLES;
}

cycles Arithmetic::or_a_mhl(CPU* cpu){

    byte data = cpu->memory->read(cpu->reg_HL.read());
    *(cpu->A) = *(cpu->A) | data;

    *(cpu->F) = 0; //set all flags to 0 
    writeBit(*(cpu->F), FLAG_Z, *(cpu->A) == 0);

    return OR_mHL_CYCLES;
}

cycles Arithmetic::cp_a_a(CPU* cpu){

    *(cpu->F) = 0b11000000; //set Z=1,N=1,H=0,C=0
    return CP_A_CYCLES;
}

cycles Arithmetic::cp_a_b(CPU* cpu){

    byte temp = *(cpu->A) - *(cpu->B);

    writeBit(*(cpu->F), FLAG_Z, temp == 0);
    writeBit(*(cpu->F), FLAG_N, 1);
    writeBit(*(cpu->F), FLAG_H, (*(cpu->A) & 0xF) < (*(cpu->B) & 0xF));
    writeBit(*(cpu->F), FLAG_C, *(cpu->A) < *(cpu->B));

    return CP_B_CYCLES;
}

cycles Arithmetic::cp_a_c(CPU* cpu){

    byte temp = *(cpu->A) - *(cpu->C);

    writeBit(*(cpu->F), FLAG_Z, temp == 0);
    writeBit(*(cpu->F), FLAG_N, 1);
    writeBit(*(cpu->F), FLAG_H, (*(cpu->A) & 0xF) < (*(cpu->C) & 0xF));
    writeBit(*(cpu->F), FLAG_C, *(cpu->A) < *(cpu->C));

    return CP_C_CYCLES;
}
cycles Arithmetic::cp_a_d(CPU* cpu){

    byte temp = *(cpu->A) - *(cpu->D);

    writeBit(*(cpu->F), FLAG_Z, temp == 0);
    writeBit(*(cpu->F), FLAG_N, 1);
    writeBit(*(cpu->F), FLAG_H, (*(cpu->A) & 0xF) < (*(cpu->D) & 0xF));
    writeBit(*(cpu->F), FLAG_C, *(cpu->A) < *(cpu->D));

    return CP_D_CYCLES;
}
cycles Arithmetic::cp_a_e(CPU* cpu){

    byte temp = *(cpu->A) - *(cpu->E);

    writeBit(*(cpu->F), FLAG_Z, temp == 0);
    writeBit(*(cpu->F), FLAG_N, 1);
    writeBit(*(cpu->F), FLAG_H, (*(cpu->A) & 0xF) < (*(cpu->E) & 0xF));
    writeBit(*(cpu->F), FLAG_C, *(cpu->A) < *(cpu->E));

    return CP_E_CYCLES;
}
cycles Arithmetic::cp_a_h(CPU* cpu){

    byte temp = *(cpu->A) - *(cpu->H);

    writeBit(*(cpu->F), FLAG_Z, temp == 0);
    writeBit(*(cpu->F), FLAG_N, 1);
    writeBit(*(cpu->F), FLAG_H, (*(cpu->A) & 0xF) < (*(cpu->H) & 0xF));
    writeBit(*(cpu->F), FLAG_C, *(cpu->A) < *(cpu->H));

    return CP_H_CYCLES;
}
cycles Arithmetic::cp_a_l(CPU* cpu){

    byte temp = *(cpu->A) - *(cpu->L);

    writeBit(*(cpu->F), FLAG_Z, temp == 0);
    writeBit(*(cpu->F), FLAG_N, 1);
    writeBit(*(cpu->F), FLAG_H, (*(cpu->A) & 0xF) < (*(cpu->L) & 0xF));
    writeBit(*(cpu->F), FLAG_C, *(cpu->A) < *(cpu->L));

    return CP_L_CYCLES;
}

cycles Arithmetic::cp_a_mhl(CPU* cpu){

    byte data = cpu->memory->read(cpu->reg_HL.read());
    byte temp = *(cpu->A) - data;

    writeBit(*(cpu->F), FLAG_Z, temp == 0);
    writeBit(*(cpu->F), FLAG_N, 1);
    writeBit(*(cpu->F), FLAG_H, (*(cpu->A) & 0xF) < (data  & 0xF));
    writeBit(*(cpu->F), FLAG_C, *(cpu->A) < data);

    return CP_mHL_CYCLES;
}

cycles Arithmetic::add_a_d8(CPU* cpu){

    word result = *(cpu->A) + cpu->parsedData;

    bool carry = result > 0xFF;
    
    result = result & 0xFF;

    writeBit(*(cpu->F), FLAG_Z, result == 0);
    writeBit(*(cpu->F), FLAG_N, 0);
    writeBit(*(cpu->F), FLAG_C, carry);
    writeBit(*(cpu->F), FLAG_H, ((((*(cpu->A) & 0xF) + (cpu->parsedData & 0xF)) > 0xF)));

    *(cpu->A) = (byte) result;

    return ADD_A_d8_CYCLES;
}

cycles Arithmetic::adc_a_d8(CPU* cpu){

    bool CY = readBit(*(cpu->F), FLAG_C);

    word result = *(cpu->A) + cpu->parsedData + CY;

    bool carry = result > 0xFF;
    
    result = result & 0xFF;

    writeBit(*(cpu->F), FLAG_Z, result == 0);
    writeBit(*(cpu->F), FLAG_N, 0);
    writeBit(*(cpu->F), FLAG_C, carry);
    writeBit(*(cpu->F), FLAG_H, ((((*(cpu->A) & 0xF) + (cpu->parsedData & 0xF) + CY) > 0xF)));

    *(cpu->A) = (byte) result;

    return ADC_A_d8_CYCLES;
}

cycles Arithmetic::and_a_d8(CPU* cpu){

    *(cpu->A) = *(cpu->A) & cpu->parsedData;

    *(cpu->F) = 0b00100000; //set N=0, H=1, C=0
    writeBit(*(cpu->F), FLAG_Z, *(cpu->A) == 0);

    return AND_d8_CYCLES;
}

cycles Arithmetic::xor_a_d8(CPU* cpu){

    *(cpu->A) = *(cpu->A) ^ cpu->parsedData;

    *(cpu->F) = 0; //set all flags to 0 
    writeBit(*(cpu->F), FLAG_Z, *(cpu->A) == 0);

    return XOR_d8_CYCLES;
}

cycles Arithmetic::sbc_a_d8(CPU* cpu){

    bool CY = readBit(*(cpu->F), FLAG_C); 
    byte temp = *(cpu->A);

    *(cpu->A) = *(cpu->A) - cpu->parsedData - CY;

    writeBit(*(cpu->F), FLAG_Z, *(cpu->A) == 0);
    writeBit(*(cpu->F), FLAG_N, 1);
    writeBit(*(cpu->F), FLAG_H,  (temp & 0xF) < (cpu->parsedData & 0xF) + CY);
    writeBit(*(cpu->F), FLAG_C,  temp < cpu->parsedData + CY);

    return SBC_A_d8_CYCLES;
}

cycles Arithmetic::sub_a_d8(CPU* cpu){

    byte temp = *(cpu->A);

    *(cpu->A) = *(cpu->A) - cpu->parsedData;

    writeBit(*(cpu->F), FLAG_Z, *(cpu->A) == 0);
    writeBit(*(cpu->F), FLAG_N, 1);
    writeBit(*(cpu->F), FLAG_H,  (temp & 0xF) < (cpu->parsedData & 0xF));
    writeBit(*(cpu->F), FLAG_C,  temp < cpu->parsedData);

    return SUB_d8_CYCLES;
}

cycles Arithmetic::or_a_d8(CPU* cpu){

    *(cpu->A) = *(cpu->A) | cpu->parsedData;

    *(cpu->F) = 0; //set all flags to 0 
    writeBit(*(cpu->F), FLAG_Z, *(cpu->A) == 0);

    return OR_d8_CYCLES;
}

cycles Arithmetic::cp_a_d8(CPU* cpu){

    byte temp = *(cpu->A) - cpu->parsedData;

    writeBit(*(cpu->F), FLAG_Z, temp == 0);
    writeBit(*(cpu->F), FLAG_N, 1);
    writeBit(*(cpu->F), FLAG_H, (*(cpu->A) & 0xF) < (cpu->parsedData & 0xF));
    writeBit(*(cpu->F), FLAG_C, *(cpu->A) < cpu->parsedData);

    return CP_d8_CYCLES;
}

cycles Arithmetic::add_sp_r8(CPU* cpu){

    signedByte imm8 = (signedByte) cpu->parsedData;

    writeBit(*(cpu->F), FLAG_Z, 0);
    writeBit(*(cpu->F), FLAG_N, 0);
    writeBit(*(cpu->F), FLAG_C, (( cpu->SP.read() & 0xFF) + (imm8 & 0xFF)) > 0xFF);
    writeBit(*(cpu->F), FLAG_H, (((cpu->SP.read() & 0xF ) + (imm8 & 0xF )) > 0xF));

    cpu->SP = (word)(cpu->SP.read() + imm8);
    return ADD_SP_r8_CYCLES;
}