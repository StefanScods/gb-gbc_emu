#ifndef INSTUCTION_H
#define INSTUCTION_H
/*
a header file for the instuctions and opcodes for the gb/gbc cpu

date: 2021-11-13
*/
#include "defines.h"
#include <string>
#include <functional>
#include "../instuctions/include/misc_control.h"
#include "../instuctions/include/load_store.h"
#include "../instuctions/include/bit.h"
#include "../instuctions/include/arithmetic.h"
#include "../instuctions/include/jump_call.h"

class CPU; //foward declarration
typedef std::function<cycles (CPU*)> operation;


//basic struct for parsing data 
struct Instruction{
    std::string opcode;
    uint8_t length;
    cycles cycleCount;
    operation execute;
};

class InstructionSet{

public:
    //instruction sets 
    Instruction set[NUM_INSTRUCTIONS]; 
    
    //instruction set groups 
    MiscAndControl miscAndControl;
    JumpAndCall jumpAndCall;
    LoadAndStore loadAndStore;
    Bit bit;
    Arithmetic arithmetic;
    
    InstructionSet(){
        set[NOP] = {"NOP", NOP_LENGTH, NOP_CYCLES, std::bind(&MiscAndControl::nop, &miscAndControl, std::placeholders::_1)};
        set[LD_BC_d16] = {"LD_BC_d16", LD_BC_d16_LENGTH, LD_BC_d16_CYCLES, std::bind(&LoadAndStore::ld_bc_d16, &loadAndStore, std::placeholders::_1)};
        set[LD_mBC_A] = {"LD_mBC_A", LD_mBC_A_LENGTH, LD_mBC_A_CYCLES, std::bind(&LoadAndStore::ld_mbc_a, &loadAndStore, std::placeholders::_1)};
        set[INC_BC] = {"INC_BC", INC_BC_LENGTH, INC_BC_CYCLES, std::bind(&Arithmetic::inc_bc, &arithmetic, std::placeholders::_1)};
        set[INC_B] = {"INC_B", INC_B_LENGTH, INC_B_CYCLES, std::bind(&Arithmetic::inc_b, &arithmetic, std::placeholders::_1)};
        set[DEC_B] = {"DEC_B", DEC_B_LENGTH, DEC_B_CYCLES, std::bind(&Arithmetic::dec_b, &arithmetic, std::placeholders::_1)};
        set[LD_B_d8] = {"LD_B_d8", LD_B_d8_LENGTH, LD_B_d8_CYCLES, std::bind(&LoadAndStore::ld_b_d8, &loadAndStore, std::placeholders::_1)};
        set[RLCA] = {"RLCA", RLCA_LENGTH, RLCA_CYCLES, std::bind(&Bit::rlca, &bit, std::placeholders::_1)};
        set[LD_mA16_SP] = {"LD_mA16_SP", LD_mA16_SP_LENGTH, LD_mA16_SP_CYCLES, std::bind(&LoadAndStore::ld_ma16_sp, &loadAndStore, std::placeholders::_1)};
        set[ADD_HL_BC] = {"ADD_HL_BC", ADD_HL_BC_LENGTH, ADD_HL_BC_CYCLES, std::bind(&Arithmetic::add_hl_bc, &arithmetic, std::placeholders::_1)};
        set[LD_A_mBC] = {"LD_A_mBC", LD_A_mBC_LENGTH, LD_A_mBC_CYCLES, std::bind(&LoadAndStore::ld_a_mbc, &loadAndStore, std::placeholders::_1)};
        set[DEC_BC] = {"DEC_BC", DEC_BC_LENGTH, DEC_BC_CYCLES, std::bind(&Arithmetic::dec_bc, &arithmetic, std::placeholders::_1)};
        set[INC_C] = {"INC_C", INC_C_LENGTH, INC_C_CYCLES, std::bind(&Arithmetic::inc_c, &arithmetic, std::placeholders::_1)};
        set[DEC_C] = {"DEC_C", DEC_C_LENGTH, DEC_C_CYCLES, std::bind(&Arithmetic::dec_c, &arithmetic, std::placeholders::_1)};
        set[LD_C_d8] = {"LD_C_d8", LD_C_d8_LENGTH, LD_C_d8_CYCLES, std::bind(&LoadAndStore::ld_c_d8, &loadAndStore, std::placeholders::_1)};
        set[RRCA] = {"RRCA", RRCA_LENGTH, RRCA_CYCLES, std::bind(&Bit::rrca, &bit, std::placeholders::_1)};
        set[STOP] = {"STOP", STOP_LENGTH, STOP_CYCLES, std::bind(&MiscAndControl::stop, &miscAndControl, std::placeholders::_1)};
        set[LD_DE_d16] = {"LD_DE_d16", LD_DE_d16_LENGTH, LD_DE_d16_CYCLES, std::bind(&LoadAndStore::ld_de_d16, &loadAndStore, std::placeholders::_1)};
        set[LD_mDE_A] = {"LD_mDE_A", LD_mDE_A_LENGTH, LD_mDE_A_CYCLES, std::bind(&LoadAndStore::ld_mde_a, &loadAndStore, std::placeholders::_1)};
        set[INC_DE] = {"INC_DE", INC_DE_LENGTH, INC_DE_CYCLES, std::bind(&Arithmetic::inc_de, &arithmetic, std::placeholders::_1)};
        set[INC_D] = {"INC_D", INC_D_LENGTH, INC_D_CYCLES, std::bind(&Arithmetic::inc_d, &arithmetic, std::placeholders::_1)};
        set[DEC_D] = {"DEC_D", DEC_D_LENGTH, DEC_D_CYCLES, std::bind(&Arithmetic::dec_d, &arithmetic, std::placeholders::_1)};
        set[LD_D_d8] = {"LD_D_d8", LD_D_d8_LENGTH, LD_D_d8_CYCLES, std::bind(&LoadAndStore::ld_d_d8, &loadAndStore, std::placeholders::_1)};
        set[RLA] = {"RLA", RLA_LENGTH, RLA_CYCLES, std::bind(&Bit::rla, &bit, std::placeholders::_1)};
        set[JR_r8] = {"JR_r8", JR_r8_LENGTH, JR_r8_CYCLES, std::bind(&JumpAndCall::jr_r8, &jumpAndCall, std::placeholders::_1)};
        set[ADD_HL_DE] = {"ADD_HL_DE", ADD_HL_DE_LENGTH, ADD_HL_DE_CYCLES, std::bind(&Arithmetic::add_hl_de, &arithmetic, std::placeholders::_1)};
        set[LD_A_mDE] = {"LD_A_mDE", LD_A_mDE_LENGTH, LD_A_mDE_CYCLES, std::bind(&LoadAndStore::ld_a_mde, &loadAndStore, std::placeholders::_1)};
        set[DEC_DE] = {"DEC_DE", DEC_DE_LENGTH, DEC_DE_CYCLES, std::bind(&Arithmetic::dec_de, &arithmetic, std::placeholders::_1)};
        set[INC_E] = {"INC_E", INC_E_LENGTH, INC_E_CYCLES, std::bind(&Arithmetic::inc_e, &arithmetic, std::placeholders::_1)};
        set[DEC_E] = {"DEC_E", DEC_E_LENGTH, DEC_E_CYCLES, std::bind(&Arithmetic::dec_e, &arithmetic, std::placeholders::_1)}; 
        set[LD_E_d8] = {"LD_E_d8", LD_E_d8_LENGTH, LD_E_d8_CYCLES, std::bind(&LoadAndStore::ld_e_d8, &loadAndStore, std::placeholders::_1)};
        set[RRA] = {"RRA", RRA_LENGTH, RRA_CYCLES, std::bind(&Bit::rra, &bit, std::placeholders::_1)};
        set[JR_NZ_r8] = {"JR_NZ_r8", JR_NZ_r8_LENGTH, JR_NZ_r8_CYCLES_MAX, std::bind(&JumpAndCall::jr_nz_r8, &jumpAndCall, std::placeholders::_1)};
        set[LD_HL_d16] = {"LD_HL_d16", LD_HL_d16_LENGTH, LD_HL_d16_CYCLES, std::bind(&LoadAndStore::ld_hl_d16, &loadAndStore, std::placeholders::_1)};
        set[LD_mHLp_A] = {"LD_mHLp_A", LD_mHLp_A_LENGTH, LD_mHLp_A_CYCLES, std::bind(&LoadAndStore::ld_mhlp_a, &loadAndStore, std::placeholders::_1)};
        set[INC_HL] = {"INC_HL", INC_HL_LENGTH, INC_HL_CYCLES, std::bind(&Arithmetic::inc_hl, &arithmetic, std::placeholders::_1)};
        set[INC_H] = {"INC_H", INC_H_LENGTH, INC_H_CYCLES, std::bind(&Arithmetic::inc_h, &arithmetic, std::placeholders::_1)};
        set[DEC_H] = {"DEC_H", DEC_H_LENGTH, DEC_H_CYCLES, std::bind(&Arithmetic::dec_h, &arithmetic, std::placeholders::_1)};
        set[LD_H_d8] = {"LD_H_d8", LD_H_d8_LENGTH, LD_H_d8_CYCLES, std::bind(&LoadAndStore::ld_h_d8, &loadAndStore, std::placeholders::_1)};
        set[DAA] = {"DAA", DAA_LENGTH, DAA_CYCLES, std::bind(&Arithmetic::daa, &arithmetic, std::placeholders::_1)};
        set[JR_Z_r8] = {"JR_Z_r8", JR_Z_r8_LENGTH, JR_Z_r8_CYCLES_MAX, std::bind(&JumpAndCall::jr_z_r8, &jumpAndCall, std::placeholders::_1)};
        set[ADD_HL_HL] = {"ADD_HL_HL", ADD_HL_HL_LENGTH, ADD_HL_HL_CYCLES, std::bind(&Arithmetic::add_hl_hl, &arithmetic, std::placeholders::_1)};
        set[LD_A_mHLp] = {"LD_A_mHLp",LD_A_mHLp_LENGTH, LD_A_mHLp_CYCLES, std::bind(&LoadAndStore::ld_a_mhlp, &loadAndStore, std::placeholders::_1)};
        set[DEC_HL] = {"DEC_HL", DEC_HL_LENGTH, DEC_HL_CYCLES, std::bind(&Arithmetic::dec_hl, &arithmetic, std::placeholders::_1)};
        set[INC_L] = {"INC_L", INC_L_LENGTH, INC_L_CYCLES, std::bind(&Arithmetic::inc_l, &arithmetic, std::placeholders::_1)};
        set[DEC_L] = {"DEC_L", DEC_L_LENGTH, DEC_L_CYCLES, std::bind(&Arithmetic::dec_l, &arithmetic, std::placeholders::_1)}; 
        set[LD_L_d8] = {"LD_L_d8",LD_L_d8_LENGTH, LD_L_d8_CYCLES, std::bind(&LoadAndStore::ld_l_d8, &loadAndStore, std::placeholders::_1)};
        set[CPL] = {"CPL", CPL_LENGTH, CPL_CYCLES, std::bind(&Bit::cpl, &bit, std::placeholders::_1)};
        set[JR_NC_r8] = {"JR_NC_r8", JR_NC_r8_LENGTH, JR_NC_r8_CYCLES_MAX, std::bind(&JumpAndCall::jr_nc_r8, &jumpAndCall, std::placeholders::_1)};
        set[LD_SP_d16] = {"LD_SP_d16",LD_SP_d16_LENGTH, LD_SP_d16_CYCLES, std::bind(&LoadAndStore::ld_sp_d16, &loadAndStore, std::placeholders::_1)};
        set[LD_mHLs_A] = {"LD_mHLs_A",LD_mHLs_A_LENGTH, LD_mHLs_A_CYCLES, std::bind(&LoadAndStore::ld_mhls_a, &loadAndStore, std::placeholders::_1)};
        set[INC_SP] = {"INC_SP", INC_SP_LENGTH, INC_SP_CYCLES, std::bind(&Arithmetic::inc_sp, &arithmetic, std::placeholders::_1)};
        set[INC_mHL] = {"INC_mHL", INC_mHL_LENGTH, INC_mHL_CYCLES, std::bind(&Arithmetic::inc_mhl, &arithmetic, std::placeholders::_1)};
        set[DEC_mHL] = {"DEC_mHL", DEC_mHL_LENGTH, DEC_mHL_CYCLES, std::bind(&Arithmetic::dec_mhl, &arithmetic, std::placeholders::_1)};
        set[LD_mHL_d8] = {"LD_mHL_d8",LD_mHL_d8_LENGTH, LD_mHL_d8_CYCLES, std::bind(&LoadAndStore::ld_mhl_d8, &loadAndStore, std::placeholders::_1)};
        set[SCF] = {"SCF", SCF_LENGTH, SCF_CYCLES, std::bind(&Bit::scf, &bit, std::placeholders::_1)};
        set[JR_C_r8] = {"JR_C_r8", JR_C_r8_LENGTH, JR_C_r8_CYCLES_MAX, std::bind(&JumpAndCall::jr_c_r8, &jumpAndCall, std::placeholders::_1)};
        set[ADD_HL_SP] = {"ADD_HL_SP", ADD_HL_SP_LENGTH, ADD_HL_SP_CYCLES, std::bind(&Arithmetic::add_hl_sp, &arithmetic, std::placeholders::_1)};
        set[LD_A_mHLs] = {"LD_A_mHLs",LD_A_mHLs_LENGTH, LD_A_mHLs_CYCLES, std::bind(&LoadAndStore::ld_a_mhls, &loadAndStore, std::placeholders::_1)};
        set[DEC_SP] = {"DEC_SP", DEC_SP_LENGTH, DEC_SP_CYCLES, std::bind(&Arithmetic::dec_sp, &arithmetic, std::placeholders::_1)};
        set[INC_A] = {"INC_A", INC_A_LENGTH, INC_A_CYCLES, std::bind(&Arithmetic::inc_a, &arithmetic, std::placeholders::_1)};
        set[DEC_A] = {"DEC_A", DEC_A_LENGTH, DEC_A_CYCLES, std::bind(&Arithmetic::dec_a, &arithmetic, std::placeholders::_1)}; 
        set[LD_A_d8] = {"LD_A_d8",LD_A_d8_LENGTH, LD_A_d8_CYCLES, std::bind(&LoadAndStore::ld_a_d8, &loadAndStore, std::placeholders::_1)};
        set[CCF] = {"CCF", CCF_LENGTH, CCF_CYCLES, std::bind(&Bit::ccf, &bit, std::placeholders::_1)};
        set[LD_B_B] = {"LD_B_B",LD_B_B_LENGTH, LD_B_B_CYCLES, std::bind(&LoadAndStore::ld_b_b, &loadAndStore, std::placeholders::_1)};
        set[LD_B_C] = {"LD_B_C",LD_B_C_LENGTH, LD_B_C_CYCLES, std::bind(&LoadAndStore::ld_b_c, &loadAndStore, std::placeholders::_1)};
        set[LD_B_D] = {"LD_B_D",LD_B_D_LENGTH, LD_B_D_CYCLES, std::bind(&LoadAndStore::ld_b_d, &loadAndStore, std::placeholders::_1)};
        set[LD_B_E] = {"LD_B_E",LD_B_E_LENGTH, LD_B_E_CYCLES, std::bind(&LoadAndStore::ld_b_e, &loadAndStore, std::placeholders::_1)};
        set[LD_B_H] = {"LD_B_H",LD_B_H_LENGTH, LD_B_H_CYCLES, std::bind(&LoadAndStore::ld_b_h, &loadAndStore, std::placeholders::_1)};
        set[LD_B_L] = {"LD_B_L",LD_B_L_LENGTH, LD_B_L_CYCLES, std::bind(&LoadAndStore::ld_b_l, &loadAndStore, std::placeholders::_1)};
        set[LD_B_mHL] = {"LD_B_mHL",LD_B_mHL_LENGTH, LD_B_mHL_CYCLES, std::bind(&LoadAndStore::ld_b_mhl, &loadAndStore, std::placeholders::_1)};
        set[LD_B_A] = {"LD_B_A",LD_B_A_LENGTH, LD_B_A_CYCLES, std::bind(&LoadAndStore::ld_b_a, &loadAndStore, std::placeholders::_1)};   
        set[LD_C_B] = {"LD_C_B",LD_C_B_LENGTH, LD_C_B_CYCLES, std::bind(&LoadAndStore::ld_c_b, &loadAndStore, std::placeholders::_1)};
        set[LD_C_C] = {"LD_C_C",LD_C_C_LENGTH, LD_C_C_CYCLES, std::bind(&LoadAndStore::ld_c_c, &loadAndStore, std::placeholders::_1)};
        set[LD_C_D] = {"LD_C_D",LD_C_D_LENGTH, LD_C_D_CYCLES, std::bind(&LoadAndStore::ld_c_d, &loadAndStore, std::placeholders::_1)};
        set[LD_C_E] = {"LD_C_E",LD_C_E_LENGTH, LD_C_E_CYCLES, std::bind(&LoadAndStore::ld_c_e, &loadAndStore, std::placeholders::_1)};
        set[LD_C_H] = {"LD_C_H",LD_C_H_LENGTH, LD_C_H_CYCLES, std::bind(&LoadAndStore::ld_c_h, &loadAndStore, std::placeholders::_1)};
        set[LD_C_L] = {"LD_C_L",LD_C_L_LENGTH, LD_C_L_CYCLES, std::bind(&LoadAndStore::ld_c_l, &loadAndStore, std::placeholders::_1)};
        set[LD_C_mHL] = {"LD_C_mHL",LD_C_mHL_LENGTH, LD_C_mHL_CYCLES, std::bind(&LoadAndStore::ld_c_mhl, &loadAndStore, std::placeholders::_1)};
        set[LD_C_A] = {"LD_C_A",LD_C_A_LENGTH, LD_C_A_CYCLES, std::bind(&LoadAndStore::ld_c_a, &loadAndStore, std::placeholders::_1)};
        set[LD_D_B] = {"LD_D_B",LD_D_B_LENGTH, LD_D_B_CYCLES, std::bind(&LoadAndStore::ld_d_b, &loadAndStore, std::placeholders::_1)};
        set[LD_D_C] = {"LD_D_C",LD_D_C_LENGTH, LD_D_C_CYCLES, std::bind(&LoadAndStore::ld_d_c, &loadAndStore, std::placeholders::_1)};
        set[LD_D_D] = {"LD_D_D",LD_D_D_LENGTH, LD_D_D_CYCLES, std::bind(&LoadAndStore::ld_d_d, &loadAndStore, std::placeholders::_1)};
        set[LD_D_E] = {"LD_D_E",LD_D_E_LENGTH, LD_D_E_CYCLES, std::bind(&LoadAndStore::ld_d_e, &loadAndStore, std::placeholders::_1)};
        set[LD_D_H] = {"LD_D_H",LD_D_H_LENGTH, LD_D_H_CYCLES, std::bind(&LoadAndStore::ld_d_h, &loadAndStore, std::placeholders::_1)};
        set[LD_D_L] = {"LD_D_L",LD_D_L_LENGTH, LD_D_L_CYCLES, std::bind(&LoadAndStore::ld_d_l, &loadAndStore, std::placeholders::_1)};
        set[LD_D_mHL] = {"LD_D_mHL",LD_D_mHL_LENGTH, LD_D_mHL_CYCLES, std::bind(&LoadAndStore::ld_d_mhl, &loadAndStore, std::placeholders::_1)};
        set[LD_D_A] = {"LD_D_A",LD_D_A_LENGTH, LD_D_A_CYCLES, std::bind(&LoadAndStore::ld_d_a, &loadAndStore, std::placeholders::_1)};
        set[LD_E_B] = {"LD_E_B",LD_E_B_LENGTH, LD_E_B_CYCLES, std::bind(&LoadAndStore::ld_e_b, &loadAndStore, std::placeholders::_1)};
        set[LD_E_C] = {"LD_E_C",LD_E_C_LENGTH, LD_E_C_CYCLES, std::bind(&LoadAndStore::ld_e_c, &loadAndStore, std::placeholders::_1)};
        set[LD_E_D] = {"LD_E_D",LD_E_D_LENGTH, LD_E_D_CYCLES, std::bind(&LoadAndStore::ld_e_d, &loadAndStore, std::placeholders::_1)};
        set[LD_E_E] = {"LD_E_E",LD_E_E_LENGTH, LD_E_E_CYCLES, std::bind(&LoadAndStore::ld_e_e, &loadAndStore, std::placeholders::_1)};
        set[LD_E_H] = {"LD_E_H",LD_E_H_LENGTH, LD_E_H_CYCLES, std::bind(&LoadAndStore::ld_e_h, &loadAndStore, std::placeholders::_1)};
        set[LD_E_L] = {"LD_E_L",LD_E_L_LENGTH, LD_E_L_CYCLES, std::bind(&LoadAndStore::ld_e_l, &loadAndStore, std::placeholders::_1)};
        set[LD_E_mHL] = {"LD_E_mHL",LD_E_mHL_LENGTH, LD_E_mHL_CYCLES, std::bind(&LoadAndStore::ld_e_mhl, &loadAndStore, std::placeholders::_1)};
        set[LD_E_A] = {"LD_E_A",LD_E_A_LENGTH, LD_E_A_CYCLES, std::bind(&LoadAndStore::ld_e_a, &loadAndStore, std::placeholders::_1)};
        set[LD_H_B] = {"LD_H_B",LD_H_B_LENGTH, LD_H_B_CYCLES, std::bind(&LoadAndStore::ld_h_b, &loadAndStore, std::placeholders::_1)};
        set[LD_H_C] = {"LD_H_C",LD_H_C_LENGTH, LD_H_C_CYCLES, std::bind(&LoadAndStore::ld_h_c, &loadAndStore, std::placeholders::_1)};
        set[LD_H_D] = {"LD_H_D",LD_H_D_LENGTH, LD_H_D_CYCLES, std::bind(&LoadAndStore::ld_h_d, &loadAndStore, std::placeholders::_1)};
        set[LD_H_E] = {"LD_H_E",LD_H_E_LENGTH, LD_H_E_CYCLES, std::bind(&LoadAndStore::ld_h_e, &loadAndStore, std::placeholders::_1)};
        set[LD_H_H] = {"LD_H_H",LD_H_H_LENGTH, LD_H_H_CYCLES, std::bind(&LoadAndStore::ld_h_h, &loadAndStore, std::placeholders::_1)};
        set[LD_H_L] = {"LD_H_L",LD_H_L_LENGTH, LD_H_L_CYCLES, std::bind(&LoadAndStore::ld_h_l, &loadAndStore, std::placeholders::_1)};
        set[LD_H_mHL] = {"LD_H_mHL",LD_H_mHL_LENGTH, LD_H_mHL_CYCLES, std::bind(&LoadAndStore::ld_h_mhl, &loadAndStore, std::placeholders::_1)};
        set[LD_H_A] = {"LD_H_A",LD_H_A_LENGTH, LD_H_A_CYCLES, std::bind(&LoadAndStore::ld_h_a, &loadAndStore, std::placeholders::_1)};
        set[LD_L_B] = {"LD_L_B",LD_L_B_LENGTH, LD_L_B_CYCLES, std::bind(&LoadAndStore::ld_l_b, &loadAndStore, std::placeholders::_1)};
        set[LD_L_C] = {"LD_L_C",LD_L_C_LENGTH, LD_L_C_CYCLES, std::bind(&LoadAndStore::ld_l_c, &loadAndStore, std::placeholders::_1)};
        set[LD_L_D] = {"LD_L_D",LD_L_D_LENGTH, LD_L_D_CYCLES, std::bind(&LoadAndStore::ld_l_d, &loadAndStore, std::placeholders::_1)};
        set[LD_L_E] = {"LD_L_E",LD_L_E_LENGTH, LD_L_E_CYCLES, std::bind(&LoadAndStore::ld_l_e, &loadAndStore, std::placeholders::_1)};
        set[LD_L_H] = {"LD_L_H",LD_L_H_LENGTH, LD_L_H_CYCLES, std::bind(&LoadAndStore::ld_l_h, &loadAndStore, std::placeholders::_1)};
        set[LD_L_L] = {"LD_L_L",LD_L_L_LENGTH, LD_L_L_CYCLES, std::bind(&LoadAndStore::ld_l_l, &loadAndStore, std::placeholders::_1)};
        set[LD_L_mHL] = {"LD_L_mHL",LD_L_mHL_LENGTH, LD_L_mHL_CYCLES, std::bind(&LoadAndStore::ld_l_mhl, &loadAndStore, std::placeholders::_1)};
        set[LD_L_A] = {"LD_L_A",LD_L_A_LENGTH, LD_L_A_CYCLES, std::bind(&LoadAndStore::ld_l_a, &loadAndStore, std::placeholders::_1)};
        set[LD_mHL_B] = {"LD_mHL_B",LD_mHL_B_LENGTH, LD_mHL_B_CYCLES, std::bind(&LoadAndStore::ld_mhl_b, &loadAndStore, std::placeholders::_1)};
        set[LD_mHL_C] = {"LD_mHL_C",LD_mHL_C_LENGTH, LD_mHL_C_CYCLES, std::bind(&LoadAndStore::ld_mhl_c, &loadAndStore, std::placeholders::_1)};
        set[LD_mHL_D] = {"LD_mHL_D",LD_mHL_D_LENGTH, LD_mHL_D_CYCLES, std::bind(&LoadAndStore::ld_mhl_d, &loadAndStore, std::placeholders::_1)};
        set[LD_mHL_E] = {"LD_mHL_E",LD_mHL_E_LENGTH, LD_mHL_E_CYCLES, std::bind(&LoadAndStore::ld_mhl_e, &loadAndStore, std::placeholders::_1)};
        set[LD_mHL_H] = {"LD_mHL_H",LD_mHL_H_LENGTH, LD_mHL_H_CYCLES, std::bind(&LoadAndStore::ld_mhl_h, &loadAndStore, std::placeholders::_1)};
        set[LD_mHL_L] = {"LD_mHL_L",LD_mHL_L_LENGTH, LD_mHL_L_CYCLES, std::bind(&LoadAndStore::ld_mhl_l, &loadAndStore, std::placeholders::_1)};
        set[HALT] = {"HALT",HALT_LENGTH, HALT_CYCLES, std::bind(&MiscAndControl::notSupported, &miscAndControl, std::placeholders::_1)};
        set[LD_mHL_A] = {"LD_mHL_A_LENGTH",LD_mHL_A, LD_mHL_A_CYCLES, std::bind(&LoadAndStore::ld_mhl_a, &loadAndStore, std::placeholders::_1)};
        set[LD_A_B] = {"LD_A_B",LD_A_B_LENGTH, LD_A_B_CYCLES, std::bind(&LoadAndStore::ld_a_b, &loadAndStore, std::placeholders::_1)};
        set[LD_A_C] = {"LD_A_C",LD_A_C_LENGTH, LD_A_C_CYCLES, std::bind(&LoadAndStore::ld_a_c, &loadAndStore, std::placeholders::_1)};
        set[LD_A_D] = {"LD_A_D",LD_A_D_LENGTH, LD_A_D_CYCLES, std::bind(&LoadAndStore::ld_a_d, &loadAndStore, std::placeholders::_1)};
        set[LD_A_E] = {"LD_A_E",LD_A_E_LENGTH, LD_A_E_CYCLES, std::bind(&LoadAndStore::ld_a_e, &loadAndStore, std::placeholders::_1)};
        set[LD_A_H] = {"LD_A_H",LD_A_H_LENGTH, LD_A_H_CYCLES, std::bind(&LoadAndStore::ld_a_h, &loadAndStore, std::placeholders::_1)};
        set[LD_A_L] = {"LD_A_L",LD_A_L_LENGTH, LD_A_L_CYCLES, std::bind(&LoadAndStore::ld_a_l, &loadAndStore, std::placeholders::_1)};
        set[LD_A_mHL] = {"LD_A_mHL",LD_A_mHL_LENGTH, LD_A_mHL_CYCLES, std::bind(&LoadAndStore::ld_a_mhl, &loadAndStore, std::placeholders::_1)};
        set[LD_A_A] = {"LD_A_A",LD_A_A_LENGTH, LD_A_A_CYCLES, std::bind(&LoadAndStore::ld_a_a, &loadAndStore, std::placeholders::_1)};
        set[ADD_A_B] = {"ADD_A_B",ADD_A_B_LENGTH, ADD_A_B_CYCLES, std::bind(&Arithmetic::add_a_b, &arithmetic, std::placeholders::_1)};
        set[ADD_A_C] = {"ADD_A_C",ADD_A_C_LENGTH, ADD_A_C_CYCLES, std::bind(&Arithmetic::add_a_c, &arithmetic, std::placeholders::_1)};
        set[ADD_A_D] = {"ADD_A_D",ADD_A_D_LENGTH, ADD_A_D_CYCLES, std::bind(&Arithmetic::add_a_d, &arithmetic, std::placeholders::_1)};
        set[ADD_A_E] = {"ADD_A_E",ADD_A_E_LENGTH, ADD_A_E_CYCLES, std::bind(&Arithmetic::add_a_e, &arithmetic, std::placeholders::_1)};
        set[ADD_A_H] = {"ADD_A_H",ADD_A_H_LENGTH, ADD_A_H_CYCLES, std::bind(&Arithmetic::add_a_h, &arithmetic, std::placeholders::_1)};
        set[ADD_A_L] = {"ADD_A_L",ADD_A_L_LENGTH, ADD_A_L_CYCLES, std::bind(&Arithmetic::add_a_l, &arithmetic, std::placeholders::_1)};
        set[ADD_A_mHL] = {"ADD_A_mHL",ADD_A_mHL_LENGTH, ADD_A_mHL_CYCLES, std::bind(&Arithmetic::add_a_mhl, &arithmetic, std::placeholders::_1)};
        set[ADD_A_A] = {"ADD_A_A",ADD_A_A_LENGTH, ADD_A_A_CYCLES, std::bind(&Arithmetic::add_a_a, &arithmetic, std::placeholders::_1)}; 
        set[ADC_A_B] = {"ADC_A_B",ADC_A_B_LENGTH, ADC_A_B_CYCLES, std::bind(&Arithmetic::adc_a_b, &arithmetic, std::placeholders::_1)};
        set[ADC_A_C] = {"ADC_A_C",ADC_A_C_LENGTH, ADC_A_C_CYCLES, std::bind(&Arithmetic::adc_a_c, &arithmetic, std::placeholders::_1)};
        set[ADC_A_D] = {"ADC_A_D",ADC_A_D_LENGTH, ADC_A_D_CYCLES, std::bind(&Arithmetic::adc_a_d, &arithmetic, std::placeholders::_1)};
        set[ADC_A_E] = {"ADC_A_E",ADC_A_E_LENGTH, ADC_A_E_CYCLES, std::bind(&Arithmetic::adc_a_e, &arithmetic, std::placeholders::_1)};
        set[ADC_A_H] = {"ADC_A_H",ADC_A_H_LENGTH, ADC_A_H_CYCLES, std::bind(&Arithmetic::adc_a_h, &arithmetic, std::placeholders::_1)};
        set[ADC_A_L] = {"ADC_A_L",ADC_A_L_LENGTH, ADC_A_L_CYCLES, std::bind(&Arithmetic::adc_a_l, &arithmetic, std::placeholders::_1)};
        set[ADC_A_mHL] = {"ADC_A_mHL",ADC_A_mHL_LENGTH, ADC_A_mHL_CYCLES, std::bind(&Arithmetic::adc_a_mhl, &arithmetic, std::placeholders::_1)};
        set[ADC_A_A] = {"ADC_A_A",ADC_A_A_LENGTH, ADC_A_A_CYCLES, std::bind(&Arithmetic::adc_a_a, &arithmetic, std::placeholders::_1)};          
        set[SUB_B] = {"SUB_B",SUB_B_LENGTH, SUB_B_CYCLES, std::bind(&Arithmetic::sub_a_b, &arithmetic, std::placeholders::_1)};
        set[SUB_C] = {"SUB_C",SUB_C_LENGTH, SUB_C_CYCLES, std::bind(&Arithmetic::sub_a_c, &arithmetic, std::placeholders::_1)};
        set[SUB_D] = {"SUB_D",SUB_D_LENGTH, SUB_D_CYCLES, std::bind(&Arithmetic::sub_a_d, &arithmetic, std::placeholders::_1)};
        set[SUB_E] = {"SUB_E",SUB_E_LENGTH, SUB_E_CYCLES, std::bind(&Arithmetic::sub_a_e, &arithmetic, std::placeholders::_1)};
        set[SUB_H] = {"SUB_H",SUB_H_LENGTH, SUB_H_CYCLES, std::bind(&Arithmetic::sub_a_h, &arithmetic, std::placeholders::_1)};
        set[SUB_L] = {"SUB_L",SUB_L_LENGTH, SUB_L_CYCLES, std::bind(&Arithmetic::sub_a_l, &arithmetic, std::placeholders::_1)};
        set[SUB_mHL] = {"SUB_mHL",SUB_mHL_LENGTH, SUB_mHL_CYCLES, std::bind(&Arithmetic::sub_a_mhl, &arithmetic, std::placeholders::_1)};
        set[SUB_A] = {"SUB_A",SUB_A_LENGTH, SUB_A_CYCLES, std::bind(&Arithmetic::sub_a_a, &arithmetic, std::placeholders::_1)};       
        set[SBC_A_B] = {"SBC_A_B",SBC_A_B_LENGTH, SBC_A_B_CYCLES, std::bind(&Arithmetic::sbc_a_b, &arithmetic, std::placeholders::_1)};
        set[SBC_A_C] = {"SBC_A_C",SBC_A_C_LENGTH, SBC_A_C_CYCLES, std::bind(&Arithmetic::sbc_a_c, &arithmetic, std::placeholders::_1)};
        set[SBC_A_D] = {"SBC_A_D",SBC_A_D_LENGTH, SBC_A_D_CYCLES, std::bind(&Arithmetic::sbc_a_d, &arithmetic, std::placeholders::_1)};
        set[SBC_A_E] = {"SBC_A_E",SBC_A_E_LENGTH, SBC_A_E_CYCLES, std::bind(&Arithmetic::sbc_a_e, &arithmetic, std::placeholders::_1)};
        set[SBC_A_H] = {"SBC_A_H",SBC_A_H_LENGTH, SBC_A_H_CYCLES, std::bind(&Arithmetic::sbc_a_h, &arithmetic, std::placeholders::_1)};
        set[SBC_A_L] = {"SBC_A_L",SBC_A_L_LENGTH, SBC_A_L_CYCLES, std::bind(&Arithmetic::sbc_a_l, &arithmetic, std::placeholders::_1)};
        set[SBC_A_mHL] = {"SBC_A_mHL",SBC_A_mHL_LENGTH, SBC_A_mHL_CYCLES, std::bind(&Arithmetic::sbc_a_mhl, &arithmetic, std::placeholders::_1)};
        set[SBC_A_A] = {"SBC_A_A",SBC_A_A_LENGTH, SBC_A_A_CYCLES, std::bind(&Arithmetic::sbc_a_a, &arithmetic, std::placeholders::_1)}; 
        set[AND_B] = {"AND_B",AND_B_LENGTH, AND_B_CYCLES, std::bind(&Arithmetic::and_a_b, &arithmetic, std::placeholders::_1)};
        set[AND_C] = {"AND_C",AND_C_LENGTH, AND_C_CYCLES, std::bind(&Arithmetic::and_a_c, &arithmetic, std::placeholders::_1)};
        set[AND_D] = {"AND_D",AND_D_LENGTH, AND_D_CYCLES, std::bind(&Arithmetic::and_a_d, &arithmetic, std::placeholders::_1)};
        set[AND_E] = {"AND_E",AND_E_LENGTH, AND_E_CYCLES, std::bind(&Arithmetic::and_a_e, &arithmetic, std::placeholders::_1)};
        set[AND_H] = {"AND_H",AND_H_LENGTH, AND_H_CYCLES, std::bind(&Arithmetic::and_a_h, &arithmetic, std::placeholders::_1)};
        set[AND_L] = {"AND_L",AND_L_LENGTH, AND_L_CYCLES, std::bind(&Arithmetic::and_a_l, &arithmetic, std::placeholders::_1)};
        set[AND_mHL] = {"AND_mHL",AND_mHL_LENGTH, AND_mHL_CYCLES, std::bind(&Arithmetic::and_a_mhl, &arithmetic, std::placeholders::_1)};
        set[AND_A] = {"AND_A",AND_A_LENGTH, AND_A_CYCLES, std::bind(&Arithmetic::and_a_a, &arithmetic, std::placeholders::_1)};      
        set[XOR_B] = {"XOR_B",XOR_B_LENGTH, XOR_B_CYCLES, std::bind(&Arithmetic::xor_a_b, &arithmetic, std::placeholders::_1)};
        set[XOR_C] = {"XOR_C",XOR_C_LENGTH, XOR_C_CYCLES, std::bind(&Arithmetic::xor_a_c, &arithmetic, std::placeholders::_1)};
        set[XOR_D] = {"XOR_D",XOR_D_LENGTH, XOR_D_CYCLES, std::bind(&Arithmetic::xor_a_d, &arithmetic, std::placeholders::_1)};
        set[XOR_E] = {"XOR_E",XOR_E_LENGTH, XOR_E_CYCLES, std::bind(&Arithmetic::xor_a_e, &arithmetic, std::placeholders::_1)};
        set[XOR_H] = {"XOR_H",XOR_H_LENGTH, XOR_H_CYCLES, std::bind(&Arithmetic::xor_a_h, &arithmetic, std::placeholders::_1)};
        set[XOR_L] = {"XOR_L",XOR_L_LENGTH, XOR_L_CYCLES, std::bind(&Arithmetic::xor_a_l, &arithmetic, std::placeholders::_1)};
        set[XOR_mHL] = {"XOR_mHL",XOR_mHL_LENGTH, XOR_mHL_CYCLES, std::bind(&Arithmetic::xor_a_mhl, &arithmetic, std::placeholders::_1)};
        set[XOR_A] = {"XOR_A",XOR_A_LENGTH, XOR_A_CYCLES, std::bind(&Arithmetic::xor_a_a, &arithmetic, std::placeholders::_1)};  
        set[OR_B] = {"OR_B",OR_B_LENGTH, OR_B_CYCLES, std::bind(&Arithmetic::or_a_b, &arithmetic, std::placeholders::_1)};
        set[OR_C] = {"OR_C",OR_C_LENGTH, OR_C_CYCLES, std::bind(&Arithmetic::or_a_c, &arithmetic, std::placeholders::_1)};
        set[OR_D] = {"OR_D",OR_D_LENGTH, OR_D_CYCLES, std::bind(&Arithmetic::or_a_d, &arithmetic, std::placeholders::_1)};
        set[OR_E] = {"OR_E",OR_E_LENGTH, OR_E_CYCLES, std::bind(&Arithmetic::or_a_e, &arithmetic, std::placeholders::_1)};
        set[OR_H] = {"OR_H",OR_H_LENGTH, OR_H_CYCLES, std::bind(&Arithmetic::or_a_h, &arithmetic, std::placeholders::_1)};
        set[OR_L] = {"OR_L",OR_L_LENGTH, OR_L_CYCLES, std::bind(&Arithmetic::or_a_l, &arithmetic, std::placeholders::_1)};
        set[OR_mHL] = {"OR_mHL",OR_mHL_LENGTH, OR_mHL_CYCLES, std::bind(&Arithmetic::or_a_mhl, &arithmetic, std::placeholders::_1)};
        set[OR_A] = {"OR_A",OR_A_LENGTH, OR_A_CYCLES, std::bind(&Arithmetic::or_a_a, &arithmetic, std::placeholders::_1)};
        set[CP_B] = {"CP_B",CP_B_LENGTH, CP_B_CYCLES, std::bind(&Arithmetic::cp_a_b, &arithmetic, std::placeholders::_1)};
        set[CP_C] = {"CP_C",CP_C_LENGTH, CP_C_CYCLES, std::bind(&Arithmetic::cp_a_c, &arithmetic, std::placeholders::_1)};
        set[CP_D] = {"CP_D",CP_D_LENGTH, CP_D_CYCLES, std::bind(&Arithmetic::cp_a_d, &arithmetic, std::placeholders::_1)};
        set[CP_E] = {"CP_E",CP_E_LENGTH, CP_E_CYCLES, std::bind(&Arithmetic::cp_a_e, &arithmetic, std::placeholders::_1)};
        set[CP_H] = {"CP_H",CP_H_LENGTH, CP_H_CYCLES, std::bind(&Arithmetic::cp_a_h, &arithmetic, std::placeholders::_1)};
        set[CP_L] = {"CP_L",CP_L_LENGTH, CP_L_CYCLES, std::bind(&Arithmetic::cp_a_l, &arithmetic, std::placeholders::_1)};
        set[CP_mHL] = {"CP_mHL",CP_mHL_LENGTH, CP_mHL_CYCLES, std::bind(&Arithmetic::cp_a_mhl, &arithmetic, std::placeholders::_1)};
        set[CP_A] = {"CP_A",CP_A_LENGTH, CP_A_CYCLES, std::bind(&Arithmetic::cp_a_a, &arithmetic, std::placeholders::_1)};   
        set[RET_NZ] = {"RET_NZ",RET_NZ_LENGTH, RET_NZ_CYCLES_MAX, std::bind(&JumpAndCall::ret_nz, &jumpAndCall, std::placeholders::_1)};
        set[POP_BC] = {"POP_BC",POP_BC_LENGTH, POP_BC_CYCLES, std::bind(&LoadAndStore::pop_bc, &loadAndStore, std::placeholders::_1)};
        set[JP_NZ_a16] = {"JP_NZ_a16",JP_NZ_a16_LENGTH, JP_NZ_a16_CYCLES_MAX, std::bind(&JumpAndCall::jp_nz_a16, &jumpAndCall, std::placeholders::_1)};
        set[JP_a16] = {"JP_a16",JP_a16_LENGTH, JP_a16_CYCLES, std::bind(&JumpAndCall::jp_a16, &jumpAndCall, std::placeholders::_1)};
        set[CALL_NZ_a16] = {"CALL_NZ_a16",CALL_NZ_a16_LENGTH, CALL_NZ_a16_CYCLES_MAX, std::bind(&JumpAndCall::call_nz_a16, &jumpAndCall, std::placeholders::_1)};
        set[PUSH_BC] = {"PUSH_BC",PUSH_BC_LENGTH, PUSH_BC_CYCLES, std::bind(&LoadAndStore::push_bc, &loadAndStore, std::placeholders::_1)};
        set[ADD_A_d8] = {"ADD_A_d8",ADD_A_d8_LENGTH, ADD_A_d8_CYCLES, std::bind(&Arithmetic::add_a_d8, &arithmetic, std::placeholders::_1)};
        set[RST_00H] = {"RST_00H",RST_00H_LENGTH, RST_00H_CYCLES, std::bind(&JumpAndCall::rst_00, &jumpAndCall, std::placeholders::_1)};
        set[RET_Z] = {"RET_Z",RET_Z_LENGTH, RET_Z_CYCLES_MAX, std::bind(&JumpAndCall::ret_z, &jumpAndCall, std::placeholders::_1)};
        set[RET] = {"RET",RET_LENGTH, RET_CYCLES, std::bind(&JumpAndCall::ret, &jumpAndCall, std::placeholders::_1)};
        set[JP_Z_a16] = {"JP_Z_a16",JP_Z_a16_LENGTH, JP_Z_a16_CYCLES_MAX, std::bind(&JumpAndCall::jp_z_a16, &jumpAndCall, std::placeholders::_1)};
        set[PREFIX_CB] = {"PREFIX_CB",PREFIX_CB_LENGTH, PREFIX_CB_CYCLES, std::bind(&Bit::prefix_cb, &bit, std::placeholders::_1)};
        set[CALL_Z_a16] = {"CALL_Z_a16",CALL_Z_a16_LENGTH, CALL_Z_a16_CYCLES_MAX, std::bind(&JumpAndCall::call_z_a16, &jumpAndCall, std::placeholders::_1)};
        set[CALL_a16] = {"CALL_a16",CALL_a16_LENGTH, CALL_a16_CYCLES, std::bind(&JumpAndCall::call_a16, &jumpAndCall, std::placeholders::_1)};
        set[ADC_A_d8] = {"ADC_A_d8",ADC_A_d8_LENGTH, ADC_A_d8_CYCLES, std::bind(&Arithmetic::adc_a_d8, &arithmetic, std::placeholders::_1)};
        set[RST_08H] = {"RST_08H",RST_08H_LENGTH, RST_08H_CYCLES, std::bind(&JumpAndCall::rst_08, &jumpAndCall, std::placeholders::_1)};
        set[RET_NC] = {"RET_NC",RET_NC_LENGTH, RET_NC_CYCLES_MAX, std::bind(&JumpAndCall::ret_nc, &jumpAndCall, std::placeholders::_1)};
        set[POP_DE] = {"POP_DE",POP_DE_LENGTH, POP_DE_CYCLES, std::bind(&LoadAndStore::pop_de, &loadAndStore, std::placeholders::_1)};
        set[JP_NC_a16] = {"JP_NC_a16",JP_NC_a16_LENGTH, JP_NC_a16_CYCLES_MAX, std::bind(&JumpAndCall::jp_nc_a16, &jumpAndCall, std::placeholders::_1)};
        set[UNDEF_D3] = {"UNDEF_D3", UNDEF_D3_LENGTH, UNDEF_D3_CYCLES, std::bind(&MiscAndControl::notSupported, &miscAndControl, std::placeholders::_1)};
        set[CALL_NC_a16] = {"CALL_NC_a16",CALL_NC_a16_LENGTH, CALL_NC_a16_CYCLES_MAX, std::bind(&JumpAndCall::call_nc_a16, &jumpAndCall, std::placeholders::_1)};
        set[PUSH_DE] = {"PUSH_DE",PUSH_DE_LENGTH, PUSH_DE_CYCLES, std::bind(&LoadAndStore::push_de, &loadAndStore, std::placeholders::_1)};
        set[SUB_d8] = {"SUB_d8",SUB_d8_LENGTH, SUB_d8_CYCLES, std::bind(&Arithmetic::sub_a_d8, &arithmetic, std::placeholders::_1)};
        set[RST_10H] = {"RST_10H",RST_10H_LENGTH, RST_10H_CYCLES, std::bind(&JumpAndCall::rst_10, &jumpAndCall, std::placeholders::_1)};
        set[RET_C] = {"RET_C",RET_C_LENGTH, RET_C_CYCLES_MAX, std::bind(&JumpAndCall::ret_c, &jumpAndCall, std::placeholders::_1)};
        set[RETI] = {"RETI",RETI_LENGTH, RETI_CYCLES, std::bind(&JumpAndCall::reti, &jumpAndCall, std::placeholders::_1)};
        set[JP_C_a16] = {"JP_C_a16",JP_C_a16_LENGTH, JP_C_a16_CYCLES_MAX, std::bind(&JumpAndCall::jp_c_a16, &jumpAndCall, std::placeholders::_1)};
        set[UNDEF_DB] = {"UNDEF_DB", UNDEF_DB_LENGTH, UNDEF_DB_CYCLES, std::bind(&MiscAndControl::notSupported, &miscAndControl, std::placeholders::_1)};
        set[CALL_C_a16] = {"CALL_C_a16",CALL_C_a16_LENGTH, CALL_C_a16_CYCLES_MAX, std::bind(&JumpAndCall::call_c_a16, &jumpAndCall, std::placeholders::_1)};
        set[UNDEF_DD] = {"UNDEF_DD", UNDEF_DD_LENGTH, UNDEF_DD_CYCLES, std::bind(&MiscAndControl::notSupported, &miscAndControl, std::placeholders::_1)};
        set[SBC_A_d8] = {"SBC_A_d8",SBC_A_d8_LENGTH, SBC_A_d8_CYCLES, std::bind(&Arithmetic::sbc_a_d8, &arithmetic, std::placeholders::_1)};
        set[RST_18H] = {"RST_18H",RST_18H_LENGTH, RST_18H_CYCLES, std::bind(&JumpAndCall::rst_18, &jumpAndCall, std::placeholders::_1)};
        set[LDH_ma8_A] = {"LDH_ma8_A",LDH_ma8_A_LENGTH, LDH_ma8_A_CYCLES, std::bind(&LoadAndStore::ldh_ma8_a, &loadAndStore, std::placeholders::_1)};
        set[POP_HL] = {"POP_HL",POP_HL_LENGTH, POP_HL_CYCLES, std::bind(&LoadAndStore::pop_hl, &loadAndStore, std::placeholders::_1)};
        set[LD_mC_A] = {"LD_mC_A",LD_mC_A_LENGTH, LD_mC_A_CYCLES, std::bind(&LoadAndStore::ld_mc_a, &loadAndStore, std::placeholders::_1)};
        set[UNDEF_E3] = {"UNDEF_E3", UNDEF_E3_LENGTH, UNDEF_E3_CYCLES, std::bind(&MiscAndControl::notSupported, &miscAndControl, std::placeholders::_1)};
        set[UNDEF_E4] = {"UNDEF_E4", UNDEF_E4_LENGTH, UNDEF_E4_CYCLES, std::bind(&MiscAndControl::notSupported, &miscAndControl, std::placeholders::_1)};
        set[PUSH_HL] = {"PUSH_HL",PUSH_HL_LENGTH, PUSH_HL_CYCLES, std::bind(&LoadAndStore::push_hl, &loadAndStore, std::placeholders::_1)};
        set[AND_d8] = {"AND_d8",AND_d8_LENGTH, AND_d8_CYCLES, std::bind(&Arithmetic::and_a_d8, &arithmetic, std::placeholders::_1)};
        set[RST_20H] = {"RST_20H",RST_20H_LENGTH, RST_20H_CYCLES, std::bind(&JumpAndCall::rst_20, &jumpAndCall, std::placeholders::_1)};
        set[ADD_SP_r8] = {"ADD_SP_r8",ADD_SP_r8_LENGTH, ADD_SP_r8_CYCLES, std::bind(&Arithmetic::add_sp_r8, &arithmetic, std::placeholders::_1)};
        set[JP_HL] = {"JP_HL",JP_HL_LENGTH, JP_HL_CYCLES, std::bind(&JumpAndCall::jp_hl, &jumpAndCall, std::placeholders::_1)};
        set[LD_ma16_A] = {"LD_ma16_A",LD_ma16_A_LENGTH, LD_ma16_A_CYCLES, std::bind(&LoadAndStore::ld_ma16_a, &loadAndStore, std::placeholders::_1)};
        set[UNDEF_EB] = {"UNDEF_EB", UNDEF_EB_LENGTH, UNDEF_EB_CYCLES, std::bind(&MiscAndControl::notSupported, &miscAndControl, std::placeholders::_1)};
        set[UNDEF_EC] = {"UNDEF_EC", UNDEF_EC_LENGTH, UNDEF_EC_CYCLES, std::bind(&MiscAndControl::notSupported, &miscAndControl, std::placeholders::_1)};
        set[UNDEF_ED] = {"UNDEF_ED", UNDEF_ED_LENGTH, UNDEF_ED_CYCLES, std::bind(&MiscAndControl::notSupported, &miscAndControl, std::placeholders::_1)};
        set[XOR_d8] = {"XOR_d8",XOR_d8_LENGTH, XOR_d8_CYCLES, std::bind(&Arithmetic::xor_a_d8, &arithmetic, std::placeholders::_1)};
        set[RST_28H] = {"RST_28H",RST_28H_LENGTH, RST_28H_CYCLES, std::bind(&JumpAndCall::rst_28, &jumpAndCall, std::placeholders::_1)}; 
        set[LDH_A_ma8] = {"LDH_A_ma8",LDH_A_ma8_LENGTH, LDH_A_ma8_CYCLES, std::bind(&LoadAndStore::ldh_a_ma8, &loadAndStore, std::placeholders::_1)};
        set[POP_AF] = {"POP_AF",POP_AF_LENGTH, POP_AF_CYCLES, std::bind(&LoadAndStore::pop_af, &loadAndStore, std::placeholders::_1)};
        set[LD_A_mC] = {"LD_A_mC",LD_A_mC_LENGTH, LD_A_mC_CYCLES, std::bind(&LoadAndStore::ld_a_mc, &loadAndStore, std::placeholders::_1)};
        set[DI] = {"DI", DI_LENGTH, DI_CYCLES, std::bind(&MiscAndControl::di, &miscAndControl, std::placeholders::_1)};
        set[UNDEF_F4] = {"UNDEF_F4", UNDEF_F4_LENGTH, UNDEF_F4_CYCLES, std::bind(&MiscAndControl::notSupported, &miscAndControl, std::placeholders::_1)};
        set[PUSH_AF] = {"PUSH_AF",PUSH_AF_LENGTH, PUSH_AF_CYCLES, std::bind(&LoadAndStore::push_af, &loadAndStore, std::placeholders::_1)};
        set[OR_d8] = {"OR_d8",OR_d8_LENGTH, OR_d8_CYCLES, std::bind(&Arithmetic::or_a_d8, &arithmetic, std::placeholders::_1)};
        set[RST_30H] = {"RST_30H",RST_30H_LENGTH, RST_30H_CYCLES, std::bind(&JumpAndCall::rst_30, &jumpAndCall, std::placeholders::_1)};
        set[LD_HL_SPpr8] = {"LD_HL_SPpr8",LD_HL_SPpr8_LENGTH, LD_HL_SPpr8_CYCLES, std::bind(&LoadAndStore::ld_hl_sp_pp8, &loadAndStore, std::placeholders::_1)};
        set[LD_SP_HL] = {"LD_SP_HL",LD_SP_HL_LENGTH, LD_SP_HL_CYCLES, std::bind(&LoadAndStore::ld_sp_hl, &loadAndStore, std::placeholders::_1)};
        set[LD_A_ma16] = {"LD_A_ma16",LD_A_ma16_LENGTH, LD_A_ma16_CYCLES, std::bind(&LoadAndStore::ld_a_ma16, &loadAndStore, std::placeholders::_1)};
        set[EI] = {"EI", EI_LENGTH, EI_CYCLES, std::bind(&MiscAndControl::ei, &miscAndControl, std::placeholders::_1)};
        set[UNDEF_FC] = {"UNDEF_FC", UNDEF_FC_LENGTH, UNDEF_FC_CYCLES, std::bind(&MiscAndControl::notSupported, &miscAndControl, std::placeholders::_1)};
        set[UNDEF_FD] = {"UNDEF_FD", UNDEF_FD_LENGTH, UNDEF_FD_CYCLES, std::bind(&MiscAndControl::notSupported, &miscAndControl, std::placeholders::_1)};
        set[CP_d8] = {"CP_d8",CP_d8_LENGTH, CP_d8_CYCLES, std::bind(&Arithmetic::cp_a_d8, &arithmetic, std::placeholders::_1)};
        set[RST_38H] = {"RST_38H",RST_38H_LENGTH, RST_38H_CYCLES, std::bind(&JumpAndCall::rst_38, &jumpAndCall, std::placeholders::_1)};
    }
};





#endif