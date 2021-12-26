#ifndef LOADSTORE_H
#define LOADSTORE_H
/*
a header file for the load and store instuctions for the gb/gbc cpu

date: 2021-11-13
*/

#include "..\..\include\defines.h"
class CPU; //foward declarration
class LoadAndStore{
    
public:
    /*
    LD r16,n16
    Load value n16 into register r16.
    Cycles: 3
    Bytes: 3
    Flags: None affected.*/
    cycles ld_bc_d16(CPU* cpu);
    cycles ld_de_d16(CPU* cpu);
    cycles ld_hl_d16(CPU* cpu);
    cycles ld_sp_d16(CPU* cpu);

    /*
    LD [r16],A
    Store value in register A into byte pointed to by register r16.
    Cycles: 2
    Bytes: 1
    Flags: None affected.*/
    cycles ld_mbc_a(CPU* cpu);
    cycles ld_mde_a(CPU* cpu);
    cycles ld_mhlp_a(CPU* cpu);
    cycles ld_mhls_a(CPU* cpu);
    

    /*
    LD r8,n8
    Load value n8 into register r8.
    Cycles: 2
    Bytes: 2
    Flags: None affected.*/
    cycles ld_a_d8(CPU* cpu);
    cycles ld_b_d8(CPU* cpu);
    cycles ld_c_d8(CPU* cpu);
    cycles ld_d_d8(CPU* cpu);
    cycles ld_e_d8(CPU* cpu);
    cycles ld_h_d8(CPU* cpu);
    cycles ld_l_d8(CPU* cpu);

    /*LD [HL],n8
    Store value n8 into byte pointed to by register HL.
    Cycles: 3
    Bytes: 2
    Flags: None affected.*/
    cycles ld_mhl_d8(CPU* cpu);

    /*
    LD [n16],SP
    Store SP & $FF at address n16 and SP >> 8 at address n16 + 1.
    Cycles: 5
    Bytes: 3
    Flags: None affected.*/
    cycles ld_ma16_sp(CPU* cpu);

    /*LD A,[r16]
    Load value in register A from byte pointed to by register r16.
    Cycles: 2
    Bytes: 1
    Flags: None affected.*/
    cycles ld_a_mbc(CPU* cpu);
    cycles ld_a_mde(CPU* cpu);
    cycles ld_a_mhlp(CPU* cpu);
    cycles ld_a_mhls(CPU* cpu);

    /*LD r8,r8
    Load (copy) value in register on the right into register on the left.
    Cycles: 1
    Bytes: 1
    Flags: None affected.*/
    cycles ld_a_a(CPU* cpu);
    cycles ld_a_b(CPU* cpu);
    cycles ld_a_c(CPU* cpu);
    cycles ld_a_d(CPU* cpu);
    cycles ld_a_e(CPU* cpu);
    cycles ld_a_h(CPU* cpu);
    cycles ld_a_l(CPU* cpu);
    cycles ld_b_a(CPU* cpu);
    cycles ld_b_b(CPU* cpu);
    cycles ld_b_c(CPU* cpu);
    cycles ld_b_d(CPU* cpu);
    cycles ld_b_e(CPU* cpu);
    cycles ld_b_h(CPU* cpu);
    cycles ld_b_l(CPU* cpu);
    cycles ld_c_a(CPU* cpu);
    cycles ld_c_b(CPU* cpu);
    cycles ld_c_c(CPU* cpu);
    cycles ld_c_d(CPU* cpu);
    cycles ld_c_e(CPU* cpu);
    cycles ld_c_h(CPU* cpu);
    cycles ld_c_l(CPU* cpu);
    cycles ld_d_a(CPU* cpu);
    cycles ld_d_b(CPU* cpu);
    cycles ld_d_c(CPU* cpu);
    cycles ld_d_d(CPU* cpu);
    cycles ld_d_e(CPU* cpu);
    cycles ld_d_h(CPU* cpu);
    cycles ld_d_l(CPU* cpu);
    cycles ld_e_a(CPU* cpu);
    cycles ld_e_b(CPU* cpu);
    cycles ld_e_c(CPU* cpu);
    cycles ld_e_d(CPU* cpu);
    cycles ld_e_e(CPU* cpu);
    cycles ld_e_h(CPU* cpu);
    cycles ld_e_l(CPU* cpu);
    cycles ld_h_a(CPU* cpu);
    cycles ld_h_b(CPU* cpu);
    cycles ld_h_c(CPU* cpu);
    cycles ld_h_d(CPU* cpu);
    cycles ld_h_e(CPU* cpu);
    cycles ld_h_h(CPU* cpu);
    cycles ld_h_l(CPU* cpu);
    cycles ld_l_a(CPU* cpu);
    cycles ld_l_b(CPU* cpu);
    cycles ld_l_c(CPU* cpu);
    cycles ld_l_d(CPU* cpu);
    cycles ld_l_e(CPU* cpu);
    cycles ld_l_h(CPU* cpu);
    cycles ld_l_l(CPU* cpu);

    /*LD r8,[HL]
    Load value into register r8 from byte pointed to by register HL.
    Cycles: 2
    Bytes: 1
    Flags: None affected.*/
    cycles ld_a_mhl(CPU* cpu);
    cycles ld_b_mhl(CPU* cpu);
    cycles ld_c_mhl(CPU* cpu);
    cycles ld_d_mhl(CPU* cpu);
    cycles ld_e_mhl(CPU* cpu);
    cycles ld_h_mhl(CPU* cpu);
    cycles ld_l_mhl(CPU* cpu);

    /*LD [HL],r8
    Store value in register r8 into byte pointed to by register HL.
    Cycles: 2
    Bytes: 1
    Flags: None affected.*/
    cycles ld_mhl_a(CPU* cpu);
    cycles ld_mhl_b(CPU* cpu);
    cycles ld_mhl_c(CPU* cpu);
    cycles ld_mhl_d(CPU* cpu);
    cycles ld_mhl_e(CPU* cpu);
    cycles ld_mhl_h(CPU* cpu);
    cycles ld_mhl_l(CPU* cpu);

    /*POP r16
    Pop register r16 from the stack. This is roughly equivalent to the following imaginary instructions:
    ld LOW(r16), [sp] ; C, E or L
    inc sp
    ld HIGH(r16), [sp] ; B, D or H
    inc sp
    Cycles: 3
    Bytes: 1
    Flags: None affected.*/
    cycles pop_bc(CPU* cpu);
    cycles pop_de(CPU* cpu);
    cycles pop_hl(CPU* cpu);


    /*POP AF
    Pop register AF from the stack. This is roughly equivalent to the following imaginary instructions:
    ld f, [sp] ; See below for individual flags
    inc sp
    ld a, [sp]
    inc sp
    Cycles: 3
    Bytes: 1
    Flags:
    Z Set from bit 7 of the popped low byte.
    N Set from bit 6 of the popped low byte.
    H Set from bit 5 of the popped low byte.
    C Set from bit 4 of the popped low byte.*/
    cycles pop_af(CPU* cpu);

    /*POP r16
    Pop register r16 from the stack. This is roughly equivalent to the following imaginary instructions:
    ld LOW(r16), [sp] ; C, E or L
    inc sp
    ld HIGH(r16), [sp] ; B, D or H
    inc sp
    Cycles: 3
    Bytes: 1
    Flags: None affected.*/
    cycles push_bc(CPU* cpu);
    cycles push_de(CPU* cpu);
    cycles push_hl(CPU* cpu);

    /*PUSH AF
    Push register AF into the stack. This is roughly equivalent to the following imaginary instructions:
    dec sp
    ld [sp], a
    dec sp
    ld [sp], flag_Z << 7 | flag_N << 6 | flag_H << 5 | flag_C << 4
    Cycles: 4
    Bytes: 1
    Flags: None affected.*/
    cycles push_af(CPU* cpu);

    /*LDH [n16],A
    Store value in register A into byte at address n16, provided it is between $FF00 and $FFFF.
    Cycles: 3
    Bytes: 2
    Flags: None affected.*/
    cycles ldh_ma8_a(CPU* cpu);

    /*LDH A,[n16]
    Load value in register A from byte at address n16, provided it is between $FF00 and $FFFF.
    Cycles: 3
    Bytes: 2
    Flags: None affected.*/
    cycles ldh_a_ma8(CPU* cpu);

    /*LDH A,[C]
    Load value in register A from byte at address $FF00+c.
    Cycles: 2
    Bytes: 1
    Flags: None affected.*/
    cycles ld_a_mc(CPU* cpu);

    /*LDH [C],A
    Store value in register A into byte at address $FF00+C.
    Cycles: 2
    Bytes: 1
    Flags: None affected.*/
    cycles ld_mc_a(CPU* cpu);

    /*LD [n16],A
    Store value in register A into byte at address n16.
    Cycles: 4
    Bytes: 3
    Flags: None affected.*/
    cycles ld_ma16_a(CPU* cpu);

    /*LD A,[n16]
    Load value in register A from byte at address n16.
    Cycles: 4
    Bytes: 3
    Flags: None affected.*/
    cycles ld_a_ma16(CPU* cpu);

    /*LD HL,SP+e8
    Add the signed value e8 to SP and store the result in HL.
    Cycles: 3
    Bytes: 2
    Flags:
    Z 0
    N 0
    H Set if overflow from bit 3.
    C Set if overflow from bit 7.*/
    cycles ld_hl_sp_pp8(CPU* cpu);

    /*LD SP,HL
    Load register HL into register SP.
    Cycles: 2
    Bytes: 1
    Flags: None affected.*/
    cycles ld_sp_hl(CPU* cpu);

};



#endif