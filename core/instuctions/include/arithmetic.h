#ifndef ARITHMETIC_H
#define ARITHMETIC_H
/*
a header file for the arithmetic instuctions for the gb/gbc cpu

date: 2021-11-13
*/

#include "..\..\include\defines.h"
class CPU; // Forward declaration.

class Arithmetic{
public:

    /*
    INC r16
    Increment value in register r16 by 1.
    Cycles: 2
    Bytes: 1
    Flags: None affected.
    */
    cycles inc_bc(CPU* cpu);
    cycles inc_de(CPU* cpu);
    cycles inc_hl(CPU* cpu);
    cycles inc_sp(CPU* cpu);

    /*DEC r16
    Decrement value in register r16 by 1.
    Cycles: 2
    Bytes: 1
    Flags: None affected.*/
    cycles dec_bc(CPU* cpu);
    cycles dec_de(CPU* cpu);
    cycles dec_hl(CPU* cpu);
    cycles dec_sp(CPU* cpu);

    /*
    INC r8
    Increment value in register r8 by 1.
    Cycles: 1
    Bytes: 1
    Flags:
    Z Set if result is 0.
    N 0
    H Set if overflow from bit 3.*/
    cycles inc_a(CPU* cpu);
    cycles inc_b(CPU* cpu);
    cycles inc_c(CPU* cpu);
    cycles inc_d(CPU* cpu);
    cycles inc_e(CPU* cpu);
    cycles inc_h(CPU* cpu);
    cycles inc_l(CPU* cpu);


    /*DEC r8
    Decrement value in register r8 by 1.
    Cycles: 1
    Bytes: 1
    Flags:
    Z Set if result is 0.
    N 1
    H Set if borrow from bit 4.*/
    cycles dec_a(CPU* cpu);
    cycles dec_b(CPU* cpu);
    cycles dec_c(CPU* cpu);
    cycles dec_d(CPU* cpu);
    cycles dec_e(CPU* cpu);
    cycles dec_h(CPU* cpu);
    cycles dec_l(CPU* cpu);

    /*ADD HL,r16
    Add the value in r16 to HL.
    Cycles: 2
    Bytes: 1
    Flags:
    N 0
    H Set if overflow from bit 11.
    C Set if overflow from bit 15.*/
    cycles add_hl_bc(CPU* cpu);
    cycles add_hl_de(CPU* cpu);
    cycles add_hl_hl(CPU* cpu);
    cycles add_hl_sp(CPU* cpu);

    /*DAA
    Decimal Adjust Accumulator to get a correct BCD representation after an arithmetic instruction.
    Cycles: 1
    Bytes: 1
    Flags:
    Z Set if result is 0.
    H 0
    C Set or reset depending on the operation.*/
    cycles daa(CPU* cpu);


    /*INC [HL]
    Increment the byte pointed to by HL by 1.
    Cycles: 3
    Bytes: 1
    Flags: See INC r8*/
    cycles inc_mhl(CPU* cpu);

    /*
    DEC [HL]
    Decrement the byte pointed to by HL by 1.
    Cycles: 3
    Bytes: 1
    Flags: See DEC r8 */
    cycles dec_mhl(CPU* cpu);

    /*ADD A,r8
    Add the value in r8 to A.
    Cycles: 1
    Bytes: 1
    Flags:
    Z Set if result is 0.
    N 0
    H Set if overflow from bit 3.
    C Set if overflow from bit 7.*/
    cycles add_a_a(CPU* cpu);
    cycles add_a_b(CPU* cpu);
    cycles add_a_c(CPU* cpu);
    cycles add_a_d(CPU* cpu);
    cycles add_a_e(CPU* cpu);
    cycles add_a_h(CPU* cpu);
    cycles add_a_l(CPU* cpu);

    /*ADD A,n8
    Add the value n8 to A.
    Cycles: 2
    Bytes: 2
    Flags: See ADD A,r8*/
    cycles add_a_d8(CPU* cpu);

    /*ADC A,n8
    Add the value n8 plus the carry flag to A.
    Cycles: 2
    Bytes: 2
    Flags: See ADC A,r8*/
    cycles adc_a_d8(CPU* cpu);

    /*AND A,n8
    Bitwise AND between the value in n8 and A.
    Cycles: 2
    Bytes: 2
    Flags: See AND A,r8*/
    cycles and_a_d8(CPU* cpu);

    /*XOR A,n8
    Bitwise XOR between the value in n8 and A.
    Cycles: 2
    Bytes: 2
    Flags: See XOR A,r8*/
    cycles xor_a_d8(CPU* cpu);

    /*SBC A,n8
    Subtract the value n8 and the carry flag from A.
    Cycles: 2
    Bytes: 2
    Flags: See SBC A,r8*/
    cycles sbc_a_d8(CPU* cpu);

    /*SUB A,n8
    Subtract the value n8 from A.
    Cycles: 2
    Bytes: 2
    Flags: See SUB A,r8 */
    cycles sub_a_d8(CPU* cpu);

    /*OR A,n8
    Store into A the bitwise OR of n8 and A.
    Cycles: 2
    Bytes: 2
    Flags: See OR A,r8*/
    cycles or_a_d8(CPU* cpu);

    /*CP A,n8
    Subtract the value n8 from A and set flags accordingly, but don't store the result.
    Cycles: 2
    Bytes: 2
    Flags: See CP A,r8*/
    cycles cp_a_d8(CPU* cpu);

    /*ADD A,[HL]
    Add the byte pointed to by HL to A.
    Cycles: 2
    Bytes: 1
    Flags: See ADD A,r8*/
    cycles add_a_mhl(CPU* cpu);

    /*ADC A,r8
    Add the value in r8 plus the carry flag to A.
    Cycles: 1
    Bytes: 1
    Flags:
    Z Set if result is 0. 
    N 0
    H Set if overflow from bit 3.
    C Set if overflow from bit 7.*/
    cycles adc_a_a(CPU* cpu);
    cycles adc_a_b(CPU* cpu);
    cycles adc_a_c(CPU* cpu);
    cycles adc_a_d(CPU* cpu);
    cycles adc_a_e(CPU* cpu);
    cycles adc_a_h(CPU* cpu);
    cycles adc_a_l(CPU* cpu);

    /*
    ADC A,[HL]
    Add the byte pointed to by HL plus the carry flag to A.
    Cycles: 2
    Bytes: 1
    Flags: See ADC A,r8*/
    cycles adc_a_mhl(CPU* cpu);

    /*SUB A,r8
    Subtract the value in r8 from A.
    Cycles: 1
    Bytes: 1
    Flags:
    Z Set if result is 0.
    N 1
    H Set if borrow from bit 4.
    C Set if borrow (set if r8 > A).*/
    cycles sub_a_a(CPU* cpu);
    cycles sub_a_b(CPU* cpu);
    cycles sub_a_c(CPU* cpu);
    cycles sub_a_d(CPU* cpu);
    cycles sub_a_e(CPU* cpu);
    cycles sub_a_h(CPU* cpu);
    cycles sub_a_l(CPU* cpu);


    /*SUB A,[HL]
    Subtract the byte pointed to by HL from A.
    Cycles: 2
    Bytes: 1
    Flags: See SUB A,r8*/
    cycles sub_a_mhl(CPU* cpu);

    /*SBC A,r8
    Subtract the value in r8 and the carry flag from A.
    Cycles: 1
    Bytes: 1
    Flags:
    Z Set if result is 0.
    N 1
    H Set if borrow from bit 4.
    C  Set if borrow (i.e. if (r8 + carry) > A).*/
    cycles sbc_a_a(CPU* cpu);
    cycles sbc_a_b(CPU* cpu);
    cycles sbc_a_c(CPU* cpu);
    cycles sbc_a_d(CPU* cpu);
    cycles sbc_a_e(CPU* cpu);
    cycles sbc_a_h(CPU* cpu);
    cycles sbc_a_l(CPU* cpu);

    /*SBC A,[HL]
    Subtract the byte pointed to by HL and the carry flag from A.
    Cycles: 2
    Bytes: 1
    Flags: See SBC A,r8*/
    cycles sbc_a_mhl(CPU* cpu);

    /*AND A,r8
    Bitwise AND between the value in r8 and A.
    Cycles: 1
    Bytes: 1
    Flags:
    Z Set if result is 0.
    N 0
    H 1
    C 0*/
    cycles and_a_a(CPU* cpu);
    cycles and_a_b(CPU* cpu);
    cycles and_a_c(CPU* cpu);
    cycles and_a_d(CPU* cpu);
    cycles and_a_e(CPU* cpu);
    cycles and_a_h(CPU* cpu);
    cycles and_a_l(CPU* cpu);

    /*AND A,[HL]
    Bitwise AND between the byte pointed to by HL and A.
    Cycles: 2
    Bytes: 1
    Flags: See AND A,r8*/
    cycles and_a_mhl(CPU* cpu);

    /*XOR A,r8
    Bitwise XOR between the value in r8 and A.
    Cycles: 1
    Bytes: 1
    Flags:
    Z Set if result is 0.
    N 0
    H 0
    C 0*/
    cycles xor_a_a(CPU* cpu);
    cycles xor_a_b(CPU* cpu);
    cycles xor_a_c(CPU* cpu);
    cycles xor_a_d(CPU* cpu);
    cycles xor_a_e(CPU* cpu);
    cycles xor_a_h(CPU* cpu);
    cycles xor_a_l(CPU* cpu);


    /*XOR A,[HL]
    Bitwise XOR between the byte pointed to by HL and A.
    Cycles: 2
    Bytes: 1
    Flags: See XOR A,r8*/
    cycles xor_a_mhl(CPU* cpu);

    /*OR A,r8
    Store into A the bitwise OR of the value in r8 and A.
    Cycles: 1
    Bytes: 1
    Flags:
    Z Set if result is 0.
    N 0
    H 0
    C 0*/
    cycles or_a_a(CPU* cpu);
    cycles or_a_b(CPU* cpu);
    cycles or_a_c(CPU* cpu);
    cycles or_a_d(CPU* cpu);
    cycles or_a_e(CPU* cpu);
    cycles or_a_h(CPU* cpu);
    cycles or_a_l(CPU* cpu);

    /*OR A,[HL]
    Store into A the bitwise OR of the byte pointed to by HL and A.
    Cycles: 2
    Bytes: 1
    Flags: See OR A,r8 */
    cycles or_a_mhl(CPU* cpu);

    /*CP A,r8
    Subtract the value in r8 from A and set flags accordingly, but don't store the result. This is useful for ComParing values.
    Cycles: 1
    Bytes: 1
    Flags:
    Z Set if result is 0.
    N 1
    H Set if borrow from bit 4.
    C Set if borrow (i.e. if r8 > A).*/
    cycles cp_a_a(CPU* cpu);
    cycles cp_a_b(CPU* cpu);
    cycles cp_a_c(CPU* cpu);
    cycles cp_a_d(CPU* cpu);
    cycles cp_a_e(CPU* cpu);
    cycles cp_a_h(CPU* cpu);
    cycles cp_a_l(CPU* cpu);

    /*CP A,[HL]
    Subtract the byte pointed to by HL from A and set flags accordingly, but don't store the result.
    Cycles: 2
    Bytes: 1
    Flags: See CP A,r8*/
    cycles cp_a_mhl(CPU* cpu);

    /*ADD SP,e8
    Add the signed value e8 to SP.
    Cycles: 2
    Bytes: 2
    Flags:
    Z 0
    N 0
    H Set if overflow from bit 3.
    C Set if overflow from bit 7.*/
    cycles add_sp_r8(CPU* cpu);



    };

#endif