#ifndef BIT_H
#define BIT_H
/*
a header file for the bit instuctions for the gb/gbc cpu

date: 2021-11-13
*/

#include "..\..\include\defines.h"
class CPU; //foward declarration

class Bit{
public:

    /*RLCA
    Rotate register A left.
    C <- [7 <- 0] <- [7]
    Cycles: 1
    Bytes: 1
    Flags:
    Z 0
    N 0
    H 0
    C Set according to result.*/
    cycles rlca(CPU* cpu);

    /*RRCA
    Rotate register A right.
    [0] -> [7 -> 0] -> C
    Cycles: 1
    Bytes: 1
    Flags:
    Z 0
    N 0
    H 0
    C Set according to result.*/
    cycles rrca(CPU* cpu);

    /*RLA
    Rotate register A left through carry.
    C <- [7 <- 0] <- C
    Cycles: 1
    Bytes: 1
    Flags:
    Z 0
    N 0
    H 0
    C Set according to result.*/
    cycles rla(CPU* cpu);

    /*RRA
    Rotate register A right through carry.
    C -> [7 -> 0] -> C
    Cycles: 1
    Bytes: 1
    Flags:
    Z 0
    N 0
    H 0
    C Set according to result.*/
    cycles rra(CPU* cpu);

    /*
    CPL
    ComPLement accumulator (A = ~A).
    Cycles: 1
    Bytes: 1
    Flags:
    N 1
    H 1*/
    cycles cpl(CPU* cpu);


    /*SCF
    Set Carry Flag.
    Cycles: 1
    Bytes: 1
    Flags:
    N 0
    H 0
    C 1*/
    cycles scf(CPU* cpu);

    /*CCF
    Complement Carry Flag.
    Cycles: 1
    Bytes: 1
    Flags:
    N 0
    H 0
    C Inverted. */
    cycles ccf(CPU* cpu);

    /*RLC r8
    Rotate register r8 left.
    C <- [7 <- 0] <- [7]
    Cycles: 2
    Bytes: 2
    Flags:
    Z Set if result is 0.
    N 0
    H 0
    C Set according to result.

    RLC [HL]
    Rotate byte pointed to by HL left.
    C <- [7 <- 0] <- [7]
    Cycles: 4
    Bytes: 2
    Flags: See RLC r8
    
    RRC r8
    Rotate register r8 right.
    [0] -> [7 -> 0] -> C
    Cycles: 2
    Bytes: 2
    Flags:
    Z Set if result is 0.
    N 0
    H 0
    C Set according to result.

    RRC [HL]
    Rotate byte pointed to by HL right.
    [0] -> [7 -> 0] -> C
    Cycles: 4
    Bytes: 2
    Flags: See RRC r8
    
    RL r8
    Rotate bits in register r8 left through carry.
    C <- [7 <- 0] <- C
    Cycles: 2
    Bytes: 2
    Flags:
    Z Set if result is 0.
    N 0
    H 0 
    C Set according to result.

    RL [HL]
    Rotate byte pointed to by HL left through carry.
    C <- [7 <- 0] <- C
    Cycles: 4
    Bytes: 2
    Flags: See RL r8
    
    RR r8
    Rotate register r8 right through carry.
    C -> [7 -> 0] -> C
    Cycles: 2
    Bytes: 2
    Flags:
    Z Set if result is 0.
    N 0
    H 0
    C Set according to result.

    RR [HL]
    Rotate byte pointed to by HL right through carry.
    C -> [7 -> 0] -> C
    Cycles: 4
    Bytes: 2
    Flags: See RR r8
    
    SLA r8
    Shift Left Arithmetic register r8.
    C <- [7 <- 0] <- 0
    Cycles: 2
    Bytes: 2
    Flags:
    Z Set if result is 0.
    N 0
    H 0
    C Set according to result.

    SLA [HL]
    Shift Left Arithmetic byte pointed to by HL.
    C <- [7 <- 0] <- 0
    Cycles: 4
    Bytes: 2
    Flags: See SLA r8

    SRA r8
    Shift Right Arithmetic register r8.
    [7] -> [7 -> 0] -> C
    Cycles: 2
    Bytes: 2
    Flags:
    Z Set if result is 0.
    N 0
    H 0
    C Set according to result.

    SRA [HL]
    Shift Right Arithmetic byte pointed to by HL.
    [7] -> [7 -> 0] -> C
    Cycles: 4
    Bytes: 2
    Flags: See SRA r8
    
    SWAP r8
    Swap upper 4 bits in register r8 and the lower 4 ones.
    Cycles: 2
    Bytes: 2
    Flags:
    Z Set if result is 0.
    N 0
    H 0
    C 0

    SWAP [HL]
    Swap upper 4 bits in the byte pointed by HL and the lower 4 ones.
    Cycles: 4
    Bytes: 2
    Flags: See SWAP r8
    
    SRL r8
    Shift Right Logic register r8.
    0 -> [7 -> 0] -> C
    Cycles: 2
    Bytes: 2
    Flags:
    Z Set if result is 0.
    N 0
    H 0
    C Set according to result.

    SRL [HL]
    Shift Right Logic byte pointed to by HL.
    0 -> [7 -> 0] -> C
    Cycles: 4
    Bytes: 2
    Flags: See SRA r8
    
    BIT u3,r8
    Test bit u3 in register r8, set the zero flag if bit not set.
    Cycles: 2
    Bytes: 2
    Flags:
    Z Set if the selected bit is 0.
    N 0
    H 1

    BIT u3,[HL]
    Test bit u3 in the byte pointed by HL, set the zero flag if bit not set.
    Cycles: 3
    Bytes: 2
    Flags: See BIT u3,r8

    RES u3,r8
    Set bit u3 in register r8 to 0. Bit 0 is the rightmost one, bit 7 the leftmost one.
    Cycles: 2
    Bytes: 2
    Flags: None affected.

    RES u3,[HL]
    Set bit u3 in the byte pointed by HL to 0. Bit 0 is the rightmost one, bit 7 the leftmost one.
    Cycles: 4
    Bytes: 2
    Flags: None affected.

    SET u3,r8
    Set bit u3 in register r8 to 1. Bit 0 is the rightmost one, bit 7 the leftmost one.
    Cycles: 2
    Bytes: 2
    Flags: None affected.

    SET u3,[HL]
    Set bit u3 in the byte pointed by HL to 1. Bit 0 is the rightmost one, bit 7 the leftmost one.
    Cycles: 4
    Bytes: 2
    Flags: None affected.*/
    cycles prefix_cb(CPU* cpu);

};

#endif