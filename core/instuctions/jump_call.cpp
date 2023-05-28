/*
a implementation file for the jump and call instuctions for the gb/gbc cpu

date: 2021-11-13
*/
#include "include\jump_call.h"
#include "..\include\cpu.h"
#include "..\include\register.h"
#include <iostream>
#include "include\load_store.h"

cycles JumpAndCall::jr_r8(CPU* cpu){

    signedByte imm8 = (signedByte) cpu->parsedData;

    cpu->PC = (word) (cpu->PC.read() + imm8);

    return JR_r8_CYCLES;
}

cycles JumpAndCall::jr_nz_r8(CPU* cpu){
    if(readBit(*(cpu->F), FLAG_Z) == 1) return JR_NZ_r8_CYCLES_MIN;
    
    signedByte imm8 = (signedByte) cpu->parsedData;
    cpu->PC = (word) (cpu->PC.read() + imm8);

    return JR_NZ_r8_CYCLES_MAX;
}

cycles JumpAndCall::jr_z_r8(CPU* cpu){
    if(readBit(*(cpu->F), FLAG_Z) == 0) return JR_Z_r8_CYCLES_MIN;
    
    signedByte imm8 = (signedByte) cpu->parsedData;
    cpu->PC = (word) (cpu->PC.read() + imm8);

    return JR_Z_r8_CYCLES_MAX;
}

cycles JumpAndCall::jr_nc_r8(CPU* cpu){
    if(readBit(*(cpu->F), FLAG_C) == 1) return JR_NC_r8_CYCLES_MIN;
    
    signedByte imm8 = (signedByte) cpu->parsedData;
    cpu->PC = (word) (cpu->PC.read() + imm8);

    return JR_NC_r8_CYCLES_MAX;
}

cycles JumpAndCall::jr_c_r8(CPU* cpu){
    if(readBit(*(cpu->F), FLAG_C) == 0) return JR_C_r8_CYCLES_MIN;
    
    signedByte imm8 = (signedByte) cpu->parsedData;
    cpu->PC = (word) (cpu->PC.read() + imm8);

    return JR_C_r8_CYCLES_MAX;
}

cycles JumpAndCall::ret_nz(CPU* cpu){
    if(readBit(*(cpu->F), FLAG_Z) == 1) return RET_NZ_CYCLES_MIN;

    popHelper(cpu, cpu->PC);

    return RET_NZ_CYCLES_MAX;
}

cycles JumpAndCall::ret_z(CPU* cpu){
    if(readBit(*(cpu->F), FLAG_Z) == 0) return RET_Z_CYCLES_MIN;

    popHelper(cpu, cpu->PC);

    return RET_Z_CYCLES_MAX;
}

cycles JumpAndCall::ret_nc(CPU* cpu){
    if(readBit(*(cpu->F), FLAG_C) == 1) return RET_NC_CYCLES_MIN;

    popHelper(cpu, cpu->PC);

    return RET_NC_CYCLES_MAX;
}

cycles JumpAndCall::ret_c(CPU* cpu){
    if(readBit(*(cpu->F), FLAG_C) == 0) return RET_C_CYCLES_MIN;

    popHelper(cpu, cpu->PC);

    return RET_C_CYCLES_MAX;
}

cycles JumpAndCall::ret(CPU* cpu){
    popHelper(cpu, cpu->PC);

    return RET_CYCLES;
}

cycles JumpAndCall::reti(CPU* cpu){
    cpu->enableInterrupts();
    popHelper(cpu, cpu->PC);

    return RETI_CYCLES;
}


cycles JumpAndCall::jp_nz_a16(CPU* cpu){
    if(readBit(*(cpu->F), FLAG_Z) == 1) return JP_NZ_a16_CYCLES_MIN;

    cpu->PC = cpu->parsedData;

    return JP_NZ_a16_CYCLES_MAX;
}

cycles JumpAndCall::jp_z_a16(CPU* cpu){
    if(readBit(*(cpu->F), FLAG_Z) == 0) return JP_Z_a16_CYCLES_MIN;

    cpu->PC = cpu->parsedData;

    return JP_Z_a16_CYCLES_MAX;
}

cycles JumpAndCall::jp_nc_a16(CPU* cpu){
    if(readBit(*(cpu->F), FLAG_C) == 1) return JP_NC_a16_CYCLES_MIN;

    cpu->PC = cpu->parsedData;

    return JP_NC_a16_CYCLES_MAX;
}

cycles JumpAndCall::jp_c_a16(CPU* cpu){
    if(readBit(*(cpu->F), FLAG_C) == 0) return JP_C_a16_CYCLES_MIN;

    cpu->PC = cpu->parsedData;

    return JP_C_a16_CYCLES_MAX;
}

cycles JumpAndCall::jp_a16(CPU* cpu){

    cpu->PC = cpu->parsedData;

    return JP_a16_CYCLES;
}
    
cycles JumpAndCall::call_a16(CPU* cpu){

    pushHelper(cpu, cpu->PC);
    cpu->PC = cpu->parsedData;

    return CALL_a16_CYCLES;
}

cycles JumpAndCall::call_nz_a16(CPU* cpu){
    if(readBit(*(cpu->F), FLAG_Z) == 1) return CALL_NZ_a16_CYCLES_MIN;

    pushHelper(cpu, cpu->PC);
    cpu->PC = cpu->parsedData;

    return CALL_NZ_a16_CYCLES_MAX;
}

cycles JumpAndCall::call_z_a16(CPU* cpu){
if(readBit(*(cpu->F), FLAG_Z) == 0) return CALL_Z_a16_CYCLES_MIN;

    pushHelper(cpu, cpu->PC);
    cpu->PC = cpu->parsedData;

    return CALL_Z_a16_CYCLES_MAX;
}

cycles JumpAndCall::call_nc_a16(CPU* cpu){
    if(readBit(*(cpu->F), FLAG_C) == 1) return CALL_NC_a16_CYCLES_MIN;

    pushHelper(cpu, cpu->PC);
    cpu->PC = cpu->parsedData;

    return CALL_NC_a16_CYCLES_MAX;
}

cycles JumpAndCall::call_c_a16(CPU* cpu){
if(readBit(*(cpu->F), FLAG_C) == 0) return CALL_C_a16_CYCLES_MIN;

    pushHelper(cpu, cpu->PC);
    cpu->PC = cpu->parsedData;

    return CALL_C_a16_CYCLES_MAX;
}

cycles JumpAndCall::rst_00(CPU* cpu){
    pushHelper(cpu, cpu->PC);
    cpu->PC = (word) 0x0000;

    return RST_00H_CYCLES;
}
cycles JumpAndCall::rst_10(CPU* cpu){
    pushHelper(cpu, cpu->PC);
    cpu->PC = (word) 0x0010;

    return RST_10H_CYCLES;
}

cycles JumpAndCall::rst_20(CPU* cpu){
    pushHelper(cpu, cpu->PC);
    cpu->PC = (word) 0x0020;

    return RST_20H_CYCLES;
}

cycles JumpAndCall::rst_30(CPU* cpu){
    pushHelper(cpu, cpu->PC);
    cpu->PC = (word) 0x0030;

    return RST_30H_CYCLES;
}

cycles JumpAndCall::rst_08(CPU* cpu){
    pushHelper(cpu, cpu->PC);
    cpu->PC = (word) 0x0000;

    return RST_08H_CYCLES;
}

cycles JumpAndCall::rst_18(CPU* cpu){
    pushHelper(cpu, cpu->PC);
    cpu->PC = (word) 0x0018;

    return RST_18H_CYCLES;
}

cycles JumpAndCall::rst_28(CPU* cpu){
    pushHelper(cpu, cpu->PC);
    cpu->PC = (word) 0x0028;

    return RST_28H_CYCLES;
}

cycles JumpAndCall::rst_38(CPU* cpu){
    pushHelper(cpu, cpu->PC);
    cpu->PC = (word) 0x0038;

    return RST_38H_CYCLES;
}

cycles JumpAndCall::jp_hl(CPU* cpu){

    cpu->PC =cpu->reg_HL.read();

    return JP_HL_CYCLES;
}