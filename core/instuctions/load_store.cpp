/*
a implementation file for the load and store instuctions for the gb/gbc cpu

date: 2021-11-13
*/
#include "include\load_store.h"
#include "..\include\cpu.h"




cycles LoadAndStore::ld_bc_d16(CPU* cpu){
    cpu->reg_BC = cpu->parsedData;
    return LD_BC_d16_CYCLES;
}

cycles LoadAndStore::ld_de_d16(CPU* cpu){
    cpu->reg_DE = cpu->parsedData;
    return LD_DE_d16_CYCLES;
}

cycles LoadAndStore::ld_hl_d16(CPU* cpu){
    cpu->reg_HL = cpu->parsedData;
    return LD_HL_d16_CYCLES;
}

cycles LoadAndStore::ld_sp_d16(CPU* cpu){
    cpu->SP = cpu->parsedData;
    return LD_SP_d16_CYCLES;
}

cycles LoadAndStore::ld_mbc_a(CPU* cpu){ 
    cpu->memory->write(cpu->reg_BC.read(), *(cpu->A));
    return LD_mBC_A_CYCLES;
}

    
cycles LoadAndStore::ld_mde_a(CPU* cpu){
    cpu->memory->write(cpu->reg_DE.read(), *(cpu->A));
    return LD_mDE_A_CYCLES;
}

cycles LoadAndStore::ld_mhlp_a(CPU* cpu){
    cpu->memory->write(cpu->reg_HL.read(), *(cpu->A));
    cpu->reg_HL++;
    return LD_mHLp_A_CYCLES;
}
    
cycles LoadAndStore::ld_mhls_a(CPU* cpu){
    cpu->memory->write(cpu->reg_HL.read(), *(cpu->A));
    cpu->reg_HL--;
    return LD_mHLs_A_CYCLES;
}
    
cycles LoadAndStore::ld_a_d8(CPU* cpu){
    *(cpu->A) = (byte) cpu->parsedData;
    return LD_A_d8_CYCLES;
}

cycles LoadAndStore::ld_b_d8(CPU* cpu){
    *(cpu->B) = (byte) cpu->parsedData;
    return LD_B_d8_CYCLES;
}

cycles LoadAndStore::ld_c_d8(CPU* cpu){
    *(cpu->C) = (byte) cpu->parsedData;
    return LD_C_d8_CYCLES;
}

cycles LoadAndStore::ld_d_d8(CPU* cpu){
    *(cpu->D) = (byte) cpu->parsedData;
    return LD_D_d8_CYCLES;
}

cycles LoadAndStore::ld_e_d8(CPU* cpu){
    *(cpu->E) = (byte) cpu->parsedData;
    return LD_E_d8_CYCLES;
}

cycles LoadAndStore::ld_h_d8(CPU* cpu){
    *(cpu->H) = (byte) cpu->parsedData;
    return LD_H_d8_CYCLES;
}

cycles LoadAndStore::ld_l_d8(CPU* cpu){
    *(cpu->L) = (byte) cpu->parsedData;
    return LD_L_d8_CYCLES;
}

cycles LoadAndStore::ld_ma16_sp(CPU* cpu){

    word dataToWrite = cpu->SP.read();
    word addressToWrite = cpu->parsedData;

    cpu->memory->write(addressToWrite, dataToWrite & 0xFF); //bottom byte
    cpu->memory->write(addressToWrite+1, dataToWrite >> 8 ); //top byte 

    return LD_mA16_SP_CYCLES;
}

cycles LoadAndStore::ldh_ma8_a(CPU* cpu){
     
    byte dataToWrite = *(cpu->A);
    word addressToWrite = cpu->parsedData | 0xFF00;
    cpu->memory->write(addressToWrite, dataToWrite); 

    return LDH_ma8_A_CYCLES;
}

cycles LoadAndStore::ldh_a_ma8(CPU* cpu){

    word addressToRead = cpu->parsedData | 0xFF00;

    *(cpu->A) = cpu->memory->read(addressToRead); 
    return LDH_A_ma8_CYCLES;
}
    
cycles LoadAndStore::ld_a_mc(CPU* cpu){

    word addressToRead = (*(cpu->C)) | 0xFF00;

    *(cpu->A) = cpu->memory->read(addressToRead); 

    return LD_A_mC_CYCLES;
}

cycles LoadAndStore::ld_mc_a(CPU* cpu){

    byte dataToWrite = *(cpu->A);
    word addressToWrite = (*(cpu->C)) | 0xFF00;
    cpu->memory->write(addressToWrite, dataToWrite); 

    return LD_mC_A_CYCLES;
}

cycles LoadAndStore::ld_a_mbc(CPU* cpu){
    *(cpu->A) = cpu->memory->read(cpu->reg_BC.read());
    return LD_A_mBC_CYCLES;
}

cycles LoadAndStore::ld_a_mde(CPU* cpu){
    *(cpu->A) = cpu->memory->read(cpu->reg_DE.read());
    return LD_A_mDE_CYCLES;
}

cycles LoadAndStore::ld_a_mhlp(CPU* cpu){
    *(cpu->A) = cpu->memory->read(cpu->reg_HL.read());
    cpu->reg_HL++;
    return LD_A_mHLp_CYCLES;
}

cycles LoadAndStore::ld_a_mhls(CPU* cpu){
    *(cpu->A) = cpu->memory->read(cpu->reg_HL.read());  
    cpu->reg_HL--;
    return LD_A_mHLs_CYCLES;
}

cycles LoadAndStore::ld_mhl_d8(CPU* cpu){
    cpu->memory->write(cpu->reg_HL.read(), (byte) cpu->parsedData);
    return LD_mHL_d8_CYCLES;
}

cycles LoadAndStore::ld_a_a(CPU* cpu){
    *(cpu->A) = *(cpu->A);
    return LD_A_A_CYCLES;
}

cycles LoadAndStore::ld_a_b(CPU* cpu){
    *(cpu->A) = *(cpu->B);
    return LD_A_B_CYCLES;
}

cycles LoadAndStore::ld_a_c(CPU* cpu){
    *(cpu->A) = *(cpu->C);
    return LD_A_C_CYCLES;
}

cycles LoadAndStore::ld_a_d(CPU* cpu){
    *(cpu->A) = *(cpu->D);
    return LD_A_D_CYCLES;
}

cycles LoadAndStore::ld_a_e(CPU* cpu){
    *(cpu->A) = *(cpu->E);
    return LD_A_E_CYCLES;
}

cycles LoadAndStore::ld_a_h(CPU* cpu){
    *(cpu->A) = *(cpu->H);
    return LD_A_H_CYCLES;
}

cycles LoadAndStore::ld_a_l(CPU* cpu){
    *(cpu->A) = *(cpu->L);
    return LD_A_L_CYCLES;
}

cycles LoadAndStore::ld_b_a(CPU* cpu){
    *(cpu->B) = *(cpu->A);
    return LD_B_A_CYCLES;
}

cycles LoadAndStore::ld_b_b(CPU* cpu){
    *(cpu->B) = *(cpu->B);
    return LD_B_B_CYCLES;
}

cycles LoadAndStore::ld_b_c(CPU* cpu){
    *(cpu->B) = *(cpu->C);
    return LD_B_C_CYCLES;
}

cycles LoadAndStore::ld_b_d(CPU* cpu){
    *(cpu->B) = *(cpu->D);
    return LD_B_D_CYCLES;
}

cycles LoadAndStore::ld_b_e(CPU* cpu){
    *(cpu->B) = *(cpu->E);
    return LD_B_E_CYCLES;
}

cycles LoadAndStore::ld_b_h(CPU* cpu){
    *(cpu->B) = *(cpu->H);
    return LD_B_H_CYCLES;
}

cycles LoadAndStore::ld_b_l(CPU* cpu){
    *(cpu->B) = *(cpu->L);
    return LD_B_L_CYCLES;
}

cycles LoadAndStore::ld_c_a(CPU* cpu){
    *(cpu->C) = *(cpu->A);
    return LD_C_A_CYCLES;
}

cycles LoadAndStore::ld_c_b(CPU* cpu){
    *(cpu->C) = *(cpu->B);
    return LD_C_B_CYCLES;
}

cycles LoadAndStore::ld_c_c(CPU* cpu){
    *(cpu->C) = *(cpu->C);
    return LD_C_C_CYCLES;
}

cycles LoadAndStore::ld_c_d(CPU* cpu){
    *(cpu->C) = *(cpu->D);
    return LD_C_D_CYCLES;
}

cycles LoadAndStore::ld_c_e(CPU* cpu){
    *(cpu->C) = *(cpu->E);
    return LD_C_E_CYCLES;
}

cycles LoadAndStore::ld_c_h(CPU* cpu){
    *(cpu->C) = *(cpu->H);
    return LD_C_H_CYCLES;
}

cycles LoadAndStore::ld_c_l(CPU* cpu){
    *(cpu->C) = *(cpu->L);
    return LD_C_A_CYCLES;
}

cycles LoadAndStore::ld_d_a(CPU* cpu){
    *(cpu->D) = *(cpu->A);
    return LD_D_A_CYCLES;
}

cycles LoadAndStore::ld_d_b(CPU* cpu){
    *(cpu->D) = *(cpu->B);
    return LD_D_B_CYCLES;
}

cycles LoadAndStore::ld_d_c(CPU* cpu){
    *(cpu->D) = *(cpu->C);
    return LD_D_C_CYCLES;
}

cycles LoadAndStore::ld_d_d(CPU* cpu){
    *(cpu->D) = *(cpu->D);
    return LD_D_D_CYCLES;
}

cycles LoadAndStore::ld_d_e(CPU* cpu){
    *(cpu->D) = *(cpu->E);
    return LD_D_E_CYCLES;
}

cycles LoadAndStore::ld_d_h(CPU* cpu){
    *(cpu->D) = *(cpu->H);
    return LD_D_H_CYCLES;
}

cycles LoadAndStore::ld_d_l(CPU* cpu){
    *(cpu->D) = *(cpu->L);
    return LD_D_L_CYCLES;
}

cycles LoadAndStore::ld_e_a(CPU* cpu){
    *(cpu->E) = *(cpu->A);
    return LD_E_A_CYCLES;
}

cycles LoadAndStore::ld_e_b(CPU* cpu){
    *(cpu->E) = *(cpu->B);
    return LD_E_B_CYCLES;
}

cycles LoadAndStore::ld_e_c(CPU* cpu){
    *(cpu->E) = *(cpu->C);
    return LD_E_C_CYCLES;
}

cycles LoadAndStore::ld_e_d(CPU* cpu){
    *(cpu->E) = *(cpu->D);
    return LD_E_D_CYCLES;
}

cycles LoadAndStore::ld_e_e(CPU* cpu){
    *(cpu->E) = *(cpu->E);
    return LD_E_E_CYCLES;
}

cycles LoadAndStore::ld_e_h(CPU* cpu){
    *(cpu->E) = *(cpu->H);
    return LD_E_H_CYCLES;
}

cycles LoadAndStore::ld_e_l(CPU* cpu){
    *(cpu->E) = *(cpu->L);
    return LD_E_L_CYCLES;
}

cycles LoadAndStore::ld_h_a(CPU* cpu){
    *(cpu->H) = *(cpu->A);
    return LD_H_A_CYCLES;
}

cycles LoadAndStore::ld_h_b(CPU* cpu){
    *(cpu->H) = *(cpu->B);
    return LD_H_B_CYCLES;
}

cycles LoadAndStore::ld_h_c(CPU* cpu){
    *(cpu->H) = *(cpu->C);
    return LD_H_C_CYCLES;
}

cycles LoadAndStore::ld_h_d(CPU* cpu){
    *(cpu->H) = *(cpu->D);
    return LD_H_D_CYCLES;
}

cycles LoadAndStore::ld_h_e(CPU* cpu){
    *(cpu->H) = *(cpu->E);
    return LD_H_E_CYCLES;
}

cycles LoadAndStore::ld_h_h(CPU* cpu){
    *(cpu->H) = *(cpu->H);
    return LD_H_H_CYCLES;
}

cycles LoadAndStore::ld_h_l(CPU* cpu){
    *(cpu->H) = *(cpu->L);
    return LD_H_L_CYCLES;
}

cycles LoadAndStore::ld_l_a(CPU* cpu){
    *(cpu->L) = *(cpu->A);
    return LD_L_A_CYCLES;
}

cycles LoadAndStore::ld_l_b(CPU* cpu){
    *(cpu->L) = *(cpu->B);
    return LD_L_B_CYCLES;
}

cycles LoadAndStore::ld_l_c(CPU* cpu){
    *(cpu->L) = *(cpu->C);
    return LD_L_C_CYCLES;
}

cycles LoadAndStore::ld_l_d(CPU* cpu){
    *(cpu->L) = *(cpu->D);
    return LD_L_D_CYCLES;
}

cycles LoadAndStore::ld_l_e(CPU* cpu){
    *(cpu->L) = *(cpu->E);
    return LD_L_E_CYCLES;
}

cycles LoadAndStore::ld_l_h(CPU* cpu){
    *(cpu->L) = *(cpu->H);
    return LD_L_H_CYCLES;
}

cycles LoadAndStore::ld_l_l(CPU* cpu){
    *(cpu->L) = *(cpu->L);
    return LD_L_L_CYCLES;
}

cycles LoadAndStore::ld_a_mhl(CPU* cpu){
    *(cpu->A) = cpu->memory->read(cpu->reg_HL.read());
    return LD_A_mHL;
}

cycles LoadAndStore::ld_b_mhl(CPU* cpu){
    *(cpu->B) = cpu->memory->read(cpu->reg_HL.read());
    return LD_B_mHL;
}

cycles LoadAndStore::ld_c_mhl(CPU* cpu){
    *(cpu->C) = cpu->memory->read(cpu->reg_HL.read());
    return LD_C_mHL;
}

cycles LoadAndStore::ld_d_mhl(CPU* cpu){
    *(cpu->D) = cpu->memory->read(cpu->reg_HL.read());
    return LD_D_mHL;
}

cycles LoadAndStore::ld_e_mhl(CPU* cpu){
    *(cpu->E) = cpu->memory->read(cpu->reg_HL.read());
    return LD_E_mHL;
}

cycles LoadAndStore::ld_h_mhl(CPU* cpu){
    *(cpu->H) = cpu->memory->read(cpu->reg_HL.read());
    return LD_H_mHL;
}

cycles LoadAndStore::ld_l_mhl(CPU* cpu){
    *(cpu->L) = cpu->memory->read(cpu->reg_HL.read());
    return LD_L_mHL;
}

cycles LoadAndStore::ld_mhl_a(CPU* cpu){
    cpu->memory->write(cpu->reg_HL.read(), *(cpu->A));
    return LD_mHL_A;
}

cycles LoadAndStore::ld_mhl_b(CPU* cpu){
    cpu->memory->write(cpu->reg_HL.read(), *(cpu->B));
    return LD_mHL_B;
}

cycles LoadAndStore::ld_mhl_c(CPU* cpu){
    cpu->memory->write(cpu->reg_HL.read(), *(cpu->C));
    return LD_mHL_C;
}

cycles LoadAndStore::ld_mhl_d(CPU* cpu){
    cpu->memory->write(cpu->reg_HL.read(), *(cpu->D));
    return LD_mHL_D;
}

cycles LoadAndStore::ld_mhl_e(CPU* cpu){
    cpu->memory->write(cpu->reg_HL.read(), *(cpu->E));
    return LD_mHL_E;
}

cycles LoadAndStore::ld_mhl_h(CPU* cpu){
    cpu->memory->write(cpu->reg_HL.read(), *(cpu->H));
    return LD_mHL_H;
}

cycles LoadAndStore::ld_mhl_l(CPU* cpu){
    cpu->memory->write(cpu->reg_HL.read(), *(cpu->L));
    return LD_mHL_L;
}

cycles LoadAndStore::pop_bc(CPU* cpu){
    popHelper(cpu, cpu->reg_BC);

    return POP_BC_CYCLES;
}

cycles LoadAndStore::pop_de(CPU* cpu){
    popHelper(cpu, cpu->reg_DE);

    return POP_DE_CYCLES;
}

cycles LoadAndStore::pop_hl(CPU* cpu){
    popHelper(cpu, cpu->reg_HL);

    return POP_HL_CYCLES;
}

cycles LoadAndStore::pop_af(CPU* cpu){
    popHelper(cpu, cpu->reg_AF);

    return POP_AF_CYCLES;
}
    
cycles LoadAndStore::push_bc(CPU* cpu){
    pushHelper(cpu, cpu->reg_BC);

    return PUSH_BC_CYCLES;
}

cycles LoadAndStore::push_de(CPU* cpu){
    pushHelper(cpu, cpu->reg_DE);

    return PUSH_DE_CYCLES;
}

cycles LoadAndStore::push_hl(CPU* cpu){
    pushHelper(cpu, cpu->reg_HL);

    return PUSH_HL_CYCLES;
}

cycles LoadAndStore::push_af(CPU* cpu){
    pushHelper(cpu, cpu->reg_AF);

    return PUSH_AF_CYCLES;
}

cycles LoadAndStore::ld_ma16_a(CPU* cpu){
    cpu->memory->write(cpu->parsedData, *(cpu->A));

    return LD_ma16_A_CYCLES;
}

cycles LoadAndStore::ld_a_ma16(CPU* cpu){
    *(cpu->A) = cpu->memory->read(cpu->parsedData);

    return LD_A_ma16_CYCLES;
}

cycles LoadAndStore::ld_hl_sp_pp8(CPU* cpu){

    signedByte imm8 = (signedByte) cpu->parsedData;

    //do arithmetic in a larger buffer so we can check for overflow
    uint32_t result = (uint32_t) cpu->SP.read() + (int32_t) imm8; 

    writeBit(*(cpu->F), FLAG_Z, 0);
    writeBit(*(cpu->F), FLAG_N, 0);
    writeBit(*(cpu->F), FLAG_C, (result > 0xFF)); //carry flag if result is greater than a 16 bit number 
    writeBit(*(cpu->F), FLAG_H, ((((cpu->SP.read() & 0xF) + imm8) > 0xF))); //half carry flag if the 12th bit overflows -> !!! maybe find a better method 

    cpu->reg_HL = (word) (result & 0xFFFF); //take the bottom 16 bits 

    return LD_HL_SPpr8_CYCLES;
}

cycles LoadAndStore::ld_sp_hl(CPU* cpu){

    cpu->SP = cpu->reg_HL.read();

    return LD_SP_HL_CYCLES;
}