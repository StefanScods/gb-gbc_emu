#ifndef JUMPCALL_H
#define JUMPCALL_H
/*
a header file for the jump and call instuctions for the gb/gbc cpu

date: 2021-11-13
*/

#include "..\..\include\defines.h"
class CPU; //foward declarration
class JumpAndCall{
public:

    /*JR e8
    Relative Jump by adding e8 to the address of the instruction following the JR. To clarify, an operand of 0 is equivalent to no jumping.
    Cycles: 3
    Bytes: 2
    Flags: None affected.*/
    cycles jr_r8(CPU* cpu);

    /*JR cc,e8
    Relative Jump by adding e8 to the current address if condition cc is met.
    Cycles: 3 taken / 2 untaken
    Bytes: 2
    Flags: None affected.*/
    cycles jr_nz_r8(CPU* cpu);
    cycles jr_z_r8(CPU* cpu);
    cycles jr_nc_r8(CPU* cpu);
    cycles jr_c_r8(CPU* cpu);

    /*JP n16
    Jump to address n16; effectively, store n16 into PC.
    Cycles: 4
    Bytes: 3
    Flags: None affected.*/
    cycles jp_a16(CPU* cpu);

    /*JP cc,n16
    Jump to address n16 if condition cc is met.
    Cycles: 4 taken / 3 untaken
    Bytes: 3
    Flags: None affected.*/
    cycles jp_nz_a16(CPU* cpu);
    cycles jp_z_a16(CPU* cpu);
    cycles jp_nc_a16(CPU* cpu);
    cycles jp_c_a16(CPU* cpu);

    /*RET cc
    Return from subroutine if condition cc is met.
    Cycles: 5 taken / 2 untaken
    Bytes: 1
    Flags: None affected.*/
    cycles ret_nz(CPU* cpu);
    cycles ret_z(CPU* cpu);
    cycles ret_nc(CPU* cpu);
    cycles ret_c(CPU* cpu);

    /*RET
    Return from subroutine. This is basically a POP PC (if such an instruction existed). See POP r16 for an explanation of how POP works.
    Cycles: 4
    Bytes: 1
    Flags: None affected.*/
    cycles ret(CPU* cpu);

    /*CALL n16
    Call address n16. This pushes the address of the instruction after the CALL on the stack, such that RET can pop it later; then, it executes an implicit JP n16.
    Cycles: 6
    Bytes: 3
    Flags: None affected.*/
    cycles call_a16(CPU* cpu);

    /*CALL cc,n16
    Call address n16 if condition cc is met.
    Cycles: 6 taken / 3 untaken
    Bytes: 3
    Flags: None affected.*/
    cycles call_nz_a16(CPU* cpu);
    cycles call_z_a16(CPU* cpu);
    cycles call_nc_a16(CPU* cpu);
    cycles call_c_a16(CPU* cpu);

    /*RST vec
    Call address vec. This is a shorter and faster equivalent to CALL for suitable values of vec.
    Cycles: 4
    Bytes: 1
    Flags: None affected.*/
    cycles rst_00(CPU* cpu);
    cycles rst_10(CPU* cpu);
    cycles rst_20(CPU* cpu);
    cycles rst_30(CPU* cpu);
    cycles rst_08(CPU* cpu);
    cycles rst_18(CPU* cpu);
    cycles rst_28(CPU* cpu);
    cycles rst_38(CPU* cpu);

    /*RETI
    Return from subroutine and enable interrupts. This is basically equivalent to executing EI then RET, meaning that IME is set right after this instruction.
    Cycles: 4
    Bytes: 1
    Flags: None affected.*/
    cycles reti(CPU* cpu);

    /*JP HL
    Jump to address in HL; effectively, load PC with value in register HL.
    Cycles: 1
    Bytes: 1
    Flags: None affected.*/
    cycles jp_hl(CPU* cpu);


};

#endif