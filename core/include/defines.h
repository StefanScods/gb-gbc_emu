#ifndef DEFINES_H
#define DEFINES_H
/**
 * A header file for defines/enums across the entire emulator code base.
 */
#include <cstdint>
#include <iostream>
#include <wx/wxprec.h>

// Debug functions.
namespace debug {
	void scream();
	void loudScream();
	void spinSleep(long long nanosecondsToSleep);
}

// Data types.
typedef int8_t signedByte;
typedef uint8_t byte;
typedef uint16_t word;
typedef uint32_t cycles;

// Used to address the top and bottom halves of the CPU registers.
typedef byte* reg8;  

// Use to store the current state of the CPU.
struct CPU_State{
	// Main GBC registers.
    word reg_AF;
    word reg_BC;
    word reg_DE;
    word reg_HL;
	// Stack pointer and program counter registers.
	word SP;
	word PC;

};

// Emulator execution modes.
enum ExecutionModes {
	CONTINUE, // Only stop for breakpoints or pauses.
	PAUSE, // Stop all emulation but still listen for inputs.
	STEP, // Fetch and execute at keypress for a single frame.
	STEP_CPU // Fetch and execute at keypress for a single instuction.
};

// CPU flags.
#define FLAG_Z 7
#define FLAG_N 6
#define FLAG_H 5
#define FLAG_C 4

// Size of the memory address space.
// GBC has a 16bit address bus -> 65,536 positions of memory
#define MEM_SIZE 65536

// Memory Map.
#define ROMBANK0_START 0x0000
#define ROMBANK0_END 0x3FFF

#define ROMBANKN_START 0x4000
#define ROMBANKN_END 0x7FFF

#define VRAM_START 0x8000
#define VRAM_END 0x9FFF

#define EXTERNALRAM_START 0xA000
#define EXTERNALRAM_END 0xBFFF

#define WRAMBANK0_START 0xC000
#define WRAMBANK0_END 0xCFFF

#define WRAMBANK1_START 0xD000
#define WRAMBANK1_END 0xDFFF

#define ECHO_START 0xE000
#define ECHO_END 0xFDFF   

#define OAM_START 0xFE00
#define OAM_END 0xFE9F

#define NOTUSABLE_START 0xFEA0
#define NOTUSABLE_END 0xFEFF

#define IOPORTS_START 0xFF00
#define IOPORTS_END 0xFF7F

#define HRAM_START 0xFF80
#define HRAM_END 0xFFFE

#define INTERRUPT_ENABLE_REGISTER_ADDR 0xFFFF

// The number of instructions in the instuction set.
#define NUM_INSTRUCTIONS 256

const char toHex[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };

// FPS defines.
#define MILLISECOND_FACTOR 1e3
#define TARGET_FPS 60
#define FRAME_DELAY MILLISECOND_FACTOR/TARGET_FPS

// Clock Speeds.
#define CLOCKSPEED 4194304 //Hz
#define CLOCKSPEED_CGBMODE 8400000 //Hz

// Cartridge Defines.

// Cartridge Header Positions.
#define CARTRIDGE_HEADER_SIZE 335

#define TITLE_START 0x134
#define TITLE_END  0x143

#define MANUFACTURERCODE_START 0x13F
#define MANUFACTURERCODE_END  0x142

#define CGB_FLAG_ADDR 0x0143

#define NEWLICENSECODE_START 0x0144 
#define NEWLICENSECODE_END 0x0145

#define SGB_FLAG_ADDR 0x0146 

#define CARTRIDGE_TYPE_ADDR 0x147

#define ROM_SiZE_ADDR 0x148

#define RAM_SiZE_ADDR 0x149

#define DESTINATION_CODE_ADDR 0x14A

#define OLD_LICENSE_CODE_ADDR 0x14B

#define ROM_VERSION_NUMBER_ADDR 0x14C

// Memory controller types.
#define ROM_ONLY 0x00     
#define MBC1 0x01
#define MBC1_RAM 0x02
#define MBC1_RAM_BATTERY 0x03   
#define MBC2 0x05
#define MBC2_BATTERY 0x06
#define ROM_RAM 0x08
#define ROM_RAM_BATTERY 0x09 
#define MMM01 0x0B
#define MMM01_RAM 0x0C
#define MMM01_RAM_BATTERY 0x0D
#define MBC3_TIMER_BATTERY 0x0F
#define MBC3_TIMER_RAM_BATTERY 0x10
#define MBC3 0x11
#define MBC3_RAM 0x12
#define MBC3_RAM_BATTERY 0x13
#define MBC4 0x15
#define MBC4_RAM 0x16
#define MBC4_RAM_BATTERY 0x17
#define MBC5 0x19
#define MBC5_RAM 0x1A
#define MBC5_RAM_BATTERY 0x1B
#define MBC5_RUMBLE 0x1C
#define MBC5_RUMBLE_RAM 0x1D
#define MBC5_RUMBLE_RAM_BATTERY 0x1E
#define POCKET_CAMERA 0xFC
#define BANDAI_TAMA5 0xFD
#define HuC3 0xFE 
#define HuC1_RAM_BATTERY 0xFF

// OP Codes.
#define NOP 0x00
#define NOP_CYCLES 4
#define NOP_LENGTH 1

#define LD_BC_d16 0x01
#define LD_BC_d16_CYCLES 12
#define LD_BC_d16_LENGTH 3

#define LD_mBC_A 0x02
#define LD_mBC_A_CYCLES 8
#define LD_mBC_A_LENGTH 1

#define INC_BC 0x03
#define INC_BC_CYCLES 8
#define INC_BC_LENGTH 1

#define INC_B 0x04
#define INC_B_CYCLES 4
#define INC_B_LENGTH 1

#define DEC_B 0x05
#define DEC_B_CYCLES 4
#define DEC_B_LENGTH 1

#define LD_B_d8 0x06
#define LD_B_d8_CYCLES 8
#define LD_B_d8_LENGTH 2

#define RLCA 0x07
#define RLCA_CYCLES 4
#define RLCA_LENGTH 1

#define LD_mA16_SP 0x08
#define LD_mA16_SP_CYCLES 20
#define LD_mA16_SP_LENGTH 3

#define ADD_HL_BC 0x09
#define ADD_HL_BC_CYCLES 8
#define ADD_HL_BC_LENGTH 1

#define LD_A_mBC 0x0A
#define LD_A_mBC_CYCLES 8
#define LD_A_mBC_LENGTH 1

#define DEC_BC 0x0B
#define DEC_BC_CYCLES 8
#define DEC_BC_LENGTH 1

#define INC_C 0x0C
#define INC_C_CYCLES 4
#define INC_C_LENGTH 1

#define DEC_C 0x0D
#define DEC_C_CYCLES 4
#define DEC_C_LENGTH 1

#define LD_C_d8 0x0E
#define LD_C_d8_CYCLES 8
#define LD_C_d8_LENGTH 2

#define RRCA 0x0F
#define RRCA_CYCLES 4
#define RRCA_LENGTH 1

#define STOP 0x10
#define STOP_CYCLES 4
#define STOP_LENGTH 2

#define LD_DE_d16 0x11
#define LD_DE_d16_CYCLES 12
#define LD_DE_d16_LENGTH 3

#define LD_mDE_A 0x12
#define LD_mDE_A_CYCLES 8
#define LD_mDE_A_LENGTH 1

#define INC_DE 0x13
#define INC_DE_CYCLES 8
#define INC_DE_LENGTH 1

#define INC_D 0x14
#define INC_D_CYCLES 4
#define INC_D_LENGTH 1

#define DEC_D 0x15
#define DEC_D_CYCLES 4
#define DEC_D_LENGTH 1

#define LD_D_d8 0x16
#define LD_D_d8_CYCLES 8
#define LD_D_d8_LENGTH 2

#define RLA 0x17
#define RLA_CYCLES 4
#define RLA_LENGTH 1

#define JR_r8 0x18
#define JR_r8_CYCLES 12
#define JR_r8_LENGTH 2

#define ADD_HL_DE 0x19
#define ADD_HL_DE_CYCLES 8
#define ADD_HL_DE_LENGTH 1

#define LD_A_mDE 0x1A
#define LD_A_mDE_CYCLES 1
#define LD_A_mDE_LENGTH 8

#define DEC_DE 0x1B
#define DEC_DE_CYCLES 8
#define DEC_DE_LENGTH 1

#define INC_E 0x1C
#define INC_E_CYCLES 4
#define INC_E_LENGTH 1

#define DEC_E 0x1D
#define DEC_E_CYCLES 4
#define DEC_E_LENGTH 1

#define LD_E_d8 0x1E
#define LD_E_d8_CYCLES 8
#define LD_E_d8_LENGTH 2

#define RRA 0x1F
#define RRA_CYCLES 4
#define RRA_LENGTH 1

#define JR_NZ_r8 0x20
#define JR_NZ_r8_CYCLES_MAX 12
#define JR_NZ_r8_CYCLES_MIN 8
#define JR_NZ_r8_LENGTH 2

#define LD_HL_d16 0x21
#define LD_HL_d16_CYCLES 12
#define LD_HL_d16_LENGTH 3

#define LD_mHLp_A 0x22
#define LD_mHLp_A_CYCLES 8
#define LD_mHLp_A_LENGTH 1

#define INC_HL 0x23
#define INC_HL_CYCLES 8
#define INC_HL_LENGTH 1

#define INC_H 0x24
#define INC_H_CYCLES 4
#define INC_H_LENGTH 1

#define DEC_H 0x25
#define DEC_H_CYCLES 4
#define DEC_H_LENGTH 1

#define LD_H_d8 0x26
#define LD_H_d8_CYCLES 8
#define LD_H_d8_LENGTH 2

#define DAA 0x27
#define DAA_CYCLES 4
#define DAA_LENGTH 1

#define JR_Z_r8 0x28
#define JR_Z_r8_CYCLES_MAX 12
#define JR_Z_r8_CYCLES_MIN 8
#define JR_Z_r8_LENGTH 2

#define ADD_HL_HL 0x29
#define ADD_HL_HL_CYCLES 8
#define ADD_HL_HL_LENGTH 1

#define LD_A_mHLp 0x2A
#define LD_A_mHLp_CYCLES 8
#define LD_A_mHLp_LENGTH 1

#define DEC_HL 0x2B
#define DEC_HL_CYCLES 8
#define DEC_HL_LENGTH 1

#define INC_L 0x2C
#define INC_L_CYCLES 4
#define INC_L_LENGTH 1

#define DEC_L 0x2D
#define DEC_L_CYCLES 4
#define DEC_L_LENGTH 1

#define LD_L_d8 0x2E
#define LD_L_d8_CYCLES 8
#define LD_L_d8_LENGTH 2

#define CPL 0x2F
#define CPL_CYCLES 4
#define CPL_LENGTH 1

#define JR_NC_r8 0x30
#define JR_NC_r8_CYCLES_MAX 12
#define JR_NC_r8_CYCLES_MIN 8
#define JR_NC_r8_LENGTH 2

#define LD_SP_d16 0x31
#define LD_SP_d16_CYCLES 12
#define LD_SP_d16_LENGTH 3

#define LD_mHLs_A 0x32
#define LD_mHLs_A_CYCLES 8
#define LD_mHLs_A_LENGTH 1

#define INC_SP 0x33
#define INC_SP_CYCLES 8
#define INC_SP_LENGTH 1

#define INC_mHL 0x34
#define INC_mHL_CYCLES 12
#define INC_mHL_LENGTH 1

#define DEC_mHL 0x35
#define DEC_mHL_CYCLES 12
#define DEC_mHL_LENGTH 1

#define LD_mHL_d8 0x36
#define LD_mHL_d8_CYCLES 12
#define LD_mHL_d8_LENGTH 2

#define SCF 0x37
#define SCF_CYCLES 4
#define SCF_LENGTH 1

#define JR_C_r8 0x38
#define JR_C_r8_CYCLES_MAX 12
#define JR_C_r8_CYCLES_MIN 8
#define JR_C_r8_LENGTH 2

#define ADD_HL_SP 0x39
#define ADD_HL_SP_CYCLES 8
#define ADD_HL_SP_LENGTH 1

#define LD_A_mHLs 0x3A
#define LD_A_mHLs_CYCLES 8
#define LD_A_mHLs_LENGTH 1

#define DEC_SP 0x3B
#define DEC_SP_CYCLES 8
#define DEC_SP_LENGTH 1

#define INC_A 0x3C
#define INC_A_CYCLES 4
#define INC_A_LENGTH 1

#define DEC_A 0x3D
#define DEC_A_CYCLES 4
#define DEC_A_LENGTH 1

#define LD_A_d8 0x3E
#define LD_A_d8_CYCLES 8
#define LD_A_d8_LENGTH 2

#define CCF 0x3F
#define CCF_CYCLES 4
#define CCF_LENGTH 1

#define LD_B_B 0x40
#define LD_B_B_CYCLES 4
#define LD_B_B_LENGTH 1

#define LD_B_C 0x41
#define LD_B_C_CYCLES 4
#define LD_B_C_LENGTH 1

#define LD_B_D 0x42
#define LD_B_D_CYCLES 4
#define LD_B_D_LENGTH 1

#define LD_B_E 0x43
#define LD_B_E_CYCLES 4
#define LD_B_E_LENGTH 1

#define LD_B_H 0x44
#define LD_B_H_CYCLES 4
#define LD_B_H_LENGTH 1

#define LD_B_L 0x45
#define LD_B_L_CYCLES 4
#define LD_B_L_LENGTH 1

#define LD_B_mHL 0x46
#define LD_B_mHL_CYCLES 8
#define LD_B_mHL_LENGTH 1

#define LD_B_A 0x47
#define LD_B_A_CYCLES 4
#define LD_B_A_LENGTH 1

#define LD_C_B 0x48
#define LD_C_B_CYCLES 4
#define LD_C_B_LENGTH 1

#define LD_C_C 0x49
#define LD_C_C_CYCLES 4
#define LD_C_C_LENGTH 1

#define LD_C_D 0x4A
#define LD_C_D_CYCLES 4
#define LD_C_D_LENGTH 1

#define LD_C_E 0x4B
#define LD_C_E_CYCLES 4
#define LD_C_E_LENGTH 1

#define LD_C_H 0x4C
#define LD_C_H_CYCLES 4
#define LD_C_H_LENGTH 1

#define LD_C_L 0x4D
#define LD_C_L_CYCLES 4
#define LD_C_L_LENGTH 1

#define LD_C_mHL 0x4E
#define LD_C_mHL_CYCLES 8
#define LD_C_mHL_LENGTH 1

#define LD_C_A 0x4F
#define LD_C_A_CYCLES 4
#define LD_C_A_LENGTH 1

#define LD_D_B 0x50
#define LD_D_B_CYCLES 4
#define LD_D_B_LENGTH 1

#define LD_D_C 0x51
#define LD_D_C_CYCLES 4
#define LD_D_C_LENGTH 1

#define LD_D_D 0x52
#define LD_D_D_CYCLES 4
#define LD_D_D_LENGTH 1

#define LD_D_E 0x53
#define LD_D_E_CYCLES 4
#define LD_D_E_LENGTH 1

#define LD_D_H 0x54
#define LD_D_H_CYCLES 4
#define LD_D_H_LENGTH 1

#define LD_D_L 0x55
#define LD_D_L_CYCLES 4
#define LD_D_L_LENGTH 1

#define LD_D_mHL 0x56
#define LD_D_mHL_CYCLES 8
#define LD_D_mHL_LENGTH 1

#define LD_D_A 0x57
#define LD_D_A_CYCLES 4
#define LD_D_A_LENGTH 1

#define LD_E_B 0x58
#define LD_E_B_CYCLES 4
#define LD_E_B_LENGTH 1

#define LD_E_C 0x59
#define LD_E_C_CYCLES 4
#define LD_E_C_LENGTH 1

#define LD_E_D 0x5A
#define LD_E_D_CYCLES 4
#define LD_E_D_LENGTH 1

#define LD_E_E 0x5B
#define LD_E_E_CYCLES 4
#define LD_E_E_LENGTH 1

#define LD_E_H 0x5C
#define LD_E_H_CYCLES 4
#define LD_E_H_LENGTH 1

#define LD_E_L 0x5D
#define LD_E_L_CYCLES 4
#define LD_E_L_LENGTH 1

#define LD_E_mHL 0x5E
#define LD_E_mHL_CYCLES 8
#define LD_E_mHL_LENGTH 1

#define LD_E_A 0x5F
#define LD_E_A_CYCLES 4
#define LD_E_A_LENGTH 1

#define LD_H_B 0x60
#define LD_H_B_CYCLES 4
#define LD_H_B_LENGTH 1

#define LD_H_C 0x61
#define LD_H_C_CYCLES 4
#define LD_H_C_LENGTH 1

#define LD_H_D 0x62
#define LD_H_D_CYCLES 4
#define LD_H_D_LENGTH 1

#define LD_H_E 0x63
#define LD_H_E_CYCLES 4
#define LD_H_E_LENGTH 1

#define LD_H_H 0x64
#define LD_H_H_CYCLES 4
#define LD_H_H_LENGTH 1

#define LD_H_L 0x65
#define LD_H_L_CYCLES 4
#define LD_H_L_LENGTH 1

#define LD_H_mHL 0x66
#define LD_H_mHL_CYCLES 8
#define LD_H_mHL_LENGTH 1

#define LD_H_A 0x67
#define LD_H_A_CYCLES 4
#define LD_H_A_LENGTH 1

#define LD_L_B 0x68
#define LD_L_B_CYCLES 4
#define LD_L_B_LENGTH 1

#define LD_L_C 0x69
#define LD_L_C_CYCLES 4
#define LD_L_C_LENGTH 1

#define LD_L_D 0x6A
#define LD_L_D_CYCLES 4
#define LD_L_D_LENGTH 1

#define LD_L_E 0x6B
#define LD_L_E_CYCLES 4
#define LD_L_E_LENGTH 1

#define LD_L_H 0x6C
#define LD_L_H_CYCLES 4
#define LD_L_H_LENGTH 1

#define LD_L_L 0x6D
#define LD_L_L_CYCLES 4
#define LD_L_L_LENGTH 1

#define LD_L_mHL 0x6E
#define LD_L_mHL_CYCLES 8
#define LD_L_mHL_LENGTH 1

#define LD_L_A 0x6F
#define LD_L_A_CYCLES 4
#define LD_L_A_LENGTH 1

#define LD_mHL_B 0x70
#define LD_mHL_B_CYCLES 8
#define LD_mHL_B_LENGTH 1

#define LD_mHL_C 0x71
#define LD_mHL_C_CYCLES 8
#define LD_mHL_C_LENGTH 1

#define LD_mHL_D 0x72
#define LD_mHL_D_CYCLES 8
#define LD_mHL_D_LENGTH 1

#define LD_mHL_E 0x73
#define LD_mHL_E_CYCLES 8
#define LD_mHL_E_LENGTH 1

#define LD_mHL_H 0x74
#define LD_mHL_H_CYCLES 8
#define LD_mHL_H_LENGTH 1

#define LD_mHL_L 0x75
#define LD_mHL_L_CYCLES 8
#define LD_mHL_L_LENGTH 1

#define HALT 0x76
#define HALT_CYCLES 4
#define HALT_LENGTH 1

#define LD_mHL_A 0x77
#define LD_mHL_A_CYCLES 8
#define LD_mHL_A_LENGTH 1

#define LD_A_B 0x78
#define LD_A_B_CYCLES 4
#define LD_A_B_LENGTH 1

#define LD_A_C 0x79
#define LD_A_C_CYCLES 4
#define LD_A_C_LENGTH 1

#define LD_A_D 0x7A
#define LD_A_D_CYCLES 4
#define LD_A_D_LENGTH 1

#define LD_A_E 0x7B
#define LD_A_E_CYCLES 4
#define LD_A_E_LENGTH 1

#define LD_A_H 0x7C
#define LD_A_H_CYCLES 4
#define LD_A_H_LENGTH 1

#define LD_A_L 0x7D
#define LD_A_L_CYCLES 4
#define LD_A_L_LENGTH 1

#define LD_A_mHL 0x7E
#define LD_A_mHL_CYCLES 8
#define LD_A_mHL_LENGTH 1

#define LD_A_A 0x7F
#define LD_A_A_CYCLES 4
#define LD_A_A_LENGTH 1

#define ADD_A_B 0x80
#define ADD_A_B_CYCLES 4
#define ADD_A_B_LENGTH 1

#define ADD_A_C 0x81
#define ADD_A_C_CYCLES 4
#define ADD_A_C_LENGTH 1

#define ADD_A_D 0x82
#define ADD_A_D_CYCLES 4
#define ADD_A_D_LENGTH 1

#define ADD_A_E 0x83
#define ADD_A_E_CYCLES 4
#define ADD_A_E_LENGTH 1

#define ADD_A_H 0x84
#define ADD_A_H_CYCLES 4
#define ADD_A_H_LENGTH 1

#define ADD_A_L 0x85
#define ADD_A_L_CYCLES 8
#define ADD_A_L_LENGTH 1

#define ADD_A_mHL 0x86
#define ADD_A_mHL_CYCLES 8
#define ADD_A_mHL_LENGTH 1

#define ADD_A_A 0x87
#define ADD_A_A_CYCLES 4
#define ADD_A_A_LENGTH 1

#define ADC_A_B 0x88
#define ADC_A_B_CYCLES 4
#define ADC_A_B_LENGTH 1

#define ADC_A_C 0x89
#define ADC_A_C_CYCLES 4
#define ADC_A_C_LENGTH 1

#define ADC_A_D 0x8A
#define ADC_A_D_CYCLES 4
#define ADC_A_D_LENGTH 1

#define ADC_A_E 0x8B
#define ADC_A_E_CYCLES 4
#define ADC_A_E_LENGTH 1

#define ADC_A_H 0x8C
#define ADC_A_H_CYCLES 4
#define ADC_A_H_LENGTH 1

#define ADC_A_L 0x8D
#define ADC_A_L_CYCLES 4
#define ADC_A_L_LENGTH 1

#define ADC_A_mHL 0x8E
#define ADC_A_mHL_CYCLES 8
#define ADC_A_mHL_LENGTH 1

#define ADC_A_A 0x8F
#define ADC_A_A_CYCLES 4
#define ADC_A_A_LENGTH 1

#define SUB_B 0x90
#define SUB_B_CYCLES 4
#define SUB_B_LENGTH 1

#define SUB_C 0x91
#define SUB_C_CYCLES 4
#define SUB_C_LENGTH 1

#define SUB_D 0x92
#define SUB_D_CYCLES 4
#define SUB_D_LENGTH 1

#define SUB_E 0x93
#define SUB_E_CYCLES 4
#define SUB_E_LENGTH 1

#define SUB_H 0x94
#define SUB_H_CYCLES 4
#define SUB_H_LENGTH 1

#define SUB_L 0x95
#define SUB_L_CYCLES 4
#define SUB_L_LENGTH 1

#define SUB_mHL 0x96
#define SUB_mHL_CYCLES 8
#define SUB_mHL_LENGTH 1

#define SUB_A 0x97
#define SUB_A_CYCLES 4
#define SUB_A_LENGTH 1

#define SBC_A_B 0x98
#define SBC_A_B_CYCLES 4
#define SBC_A_B_LENGTH 1

#define SBC_A_C 0x99
#define SBC_A_C_CYCLES 4
#define SBC_A_C_LENGTH 1

#define SBC_A_D 0x9A
#define SBC_A_D_CYCLES 4
#define SBC_A_D_LENGTH 1

#define SBC_A_E 0x9B
#define SBC_A_E_CYCLES 4
#define SBC_A_E_LENGTH 1

#define SBC_A_H 0x9C
#define SBC_A_H_CYCLES 4
#define SBC_A_H_LENGTH 1

#define SBC_A_L 0x9D
#define SBC_A_L_CYCLES 4
#define SBC_A_L_LENGTH 1

#define SBC_A_mHL 0x9E
#define SBC_A_mHL_CYCLES 8
#define SBC_A_mHL_LENGTH 1

#define SBC_A_A 0x9F
#define SBC_A_A_CYCLES 4
#define SBC_A_A_LENGTH 1

#define AND_B 0xA0
#define AND_B_CYCLES 4
#define AND_B_LENGTH 1

#define AND_C 0xA1
#define AND_C_CYCLES 4
#define AND_C_LENGTH 1

#define AND_D 0xA2
#define AND_D_CYCLES 4
#define AND_D_LENGTH 1

#define AND_E 0xA3
#define AND_E_CYCLES 4
#define AND_E_LENGTH 1

#define AND_H 0xA4
#define AND_H_CYCLES 4
#define AND_H_LENGTH 1

#define AND_L 0xA5
#define AND_L_CYCLES 4
#define AND_L_LENGTH 1

#define AND_mHL 0xA6
#define AND_mHL_CYCLES 8
#define AND_mHL_LENGTH 1

#define AND_A 0xA7
#define AND_A_CYCLES 4
#define AND_A_LENGTH 1

#define XOR_B 0xA8
#define XOR_B_CYCLES 4
#define XOR_B_LENGTH 1

#define XOR_C 0xA9
#define XOR_C_CYCLES 4
#define XOR_C_LENGTH 1

#define XOR_D 0xAA
#define XOR_D_CYCLES 4
#define XOR_D_LENGTH 1

#define XOR_E 0xAB
#define XOR_E_CYCLES 4
#define XOR_E_LENGTH 1

#define XOR_H 0xAC
#define XOR_H_CYCLES 4
#define XOR_H_LENGTH 1

#define XOR_L 0xAD
#define XOR_L_CYCLES 4
#define XOR_L_LENGTH 1

#define XOR_mHL 0xAE
#define XOR_mHL_CYCLES 8
#define XOR_mHL_LENGTH 1

#define XOR_A 0xAF
#define XOR_A_CYCLES 4
#define XOR_A_LENGTH 1

#define OR_B 0xB0
#define OR_B_CYCLES 4
#define OR_B_LENGTH 1

#define OR_C 0xB1
#define OR_C_CYCLES 4
#define OR_C_LENGTH 1

#define OR_D 0xB2
#define OR_D_CYCLES 4
#define OR_D_LENGTH 1

#define OR_E 0xB3
#define OR_E_CYCLES 4
#define OR_E_LENGTH 1

#define OR_H 0xB4
#define OR_H_CYCLES 4
#define OR_H_LENGTH 1

#define OR_L 0xB5
#define OR_L_CYCLES 4
#define OR_L_LENGTH 1

#define OR_mHL 0xB6
#define OR_mHL_CYCLES 8
#define OR_mHL_LENGTH 1

#define OR_A 0xB7
#define OR_A_CYCLES 4
#define OR_A_LENGTH 1

#define CP_B 0xB8
#define CP_B_CYCLES 4
#define CP_B_LENGTH 1

#define CP_C 0xB9
#define CP_C_CYCLES 4
#define CP_C_LENGTH 1

#define CP_D 0xBA
#define CP_D_CYCLES 4
#define CP_D_LENGTH 1

#define CP_E 0xBB
#define CP_E_CYCLES 4
#define CP_E_LENGTH 1

#define CP_H 0xBC
#define CP_H_CYCLES 4
#define CP_H_LENGTH 1

#define CP_L 0xBD
#define CP_L_CYCLES 4
#define CP_L_LENGTH 1

#define CP_mHL 0xBE
#define CP_mHL_CYCLES 8
#define CP_mHL_LENGTH 1

#define CP_A 0xBF
#define CP_A_CYCLES 4
#define CP_A_LENGTH 1

#define RET_NZ 0xC0
#define RET_NZ_CYCLES_MAX 20
#define RET_NZ_CYCLES_MIN 8
#define RET_NZ_LENGTH 1

#define POP_BC 0xC1
#define POP_BC_CYCLES 12
#define POP_BC_LENGTH 1

#define JP_NZ_a16 0xC2
#define JP_NZ_a16_CYCLES_MAX 16
#define JP_NZ_a16_CYCLES_MIN 12
#define JP_NZ_a16_LENGTH 3

#define JP_a16 0xC3
#define JP_a16_CYCLES 16
#define JP_a16_LENGTH 3

#define CALL_NZ_a16 0xC4
#define CALL_NZ_a16_CYCLES_MAX 24
#define CALL_NZ_a16_CYCLES_MIN 12
#define CALL_NZ_a16_LENGTH 3

#define PUSH_BC 0xC5
#define PUSH_BC_CYCLES 16
#define PUSH_BC_LENGTH 1

#define ADD_A_d8  0xC6
#define ADD_A_d8_CYCLES 8
#define ADD_A_d8_LENGTH 2

#define RST_00H 0xC7
#define RST_00H_CYCLES 16
#define RST_00H_LENGTH 1

#define RET_Z 0xC8
#define RET_Z_CYCLES_MAX 20
#define RET_Z_CYCLES_MIN 8
#define RET_Z_LENGTH 1

#define RET 0xC9
#define RET_CYCLES 16
#define RET_LENGTH 1

#define JP_Z_a16 0xCA
#define JP_Z_a16_CYCLES_MAX 16
#define JP_Z_a16_CYCLES_MIN 12
#define JP_Z_a16_LENGTH 3

#define PREFIX_CB 0xCB
#define PREFIX_CB_CYCLES 16
#define PREFIX_CB_LENGTH 1

#define CALL_Z_a16 0xCC
#define CALL_Z_a16_CYCLES_MAX 24
#define CALL_Z_a16_CYCLES_MIN 12
#define CALL_Z_a16_LENGTH 3

#define CALL_a16 0xCD
#define CALL_a16_CYCLES 24
#define CALL_a16_LENGTH 3

#define ADC_A_d8 0xCE
#define ADC_A_d8_CYCLES 8
#define ADC_A_d8_LENGTH 2

#define RST_08H 0xCF
#define RST_08H_CYCLES 16
#define RST_08H_LENGTH 1

#define RET_NC 0xD0
#define RET_NC_CYCLES_MAX 20
#define RET_NC_CYCLES_MIN 8
#define RET_NC_LENGTH 1

#define POP_DE 0xD1
#define POP_DE_CYCLES 12
#define POP_DE_LENGTH 1

#define JP_NC_a16 0xD2
#define JP_NC_a16_CYCLES_MAX 16
#define JP_NC_a16_CYCLES_MIN 12
#define JP_NC_a16_LENGTH 3

#define UNDEF_D3 0xD3
#define UNDEF_D3_CYCLES 4
#define UNDEF_D3_LENGTH 1

#define CALL_NC_a16 0xD4
#define CALL_NC_a16_CYCLES_MAX 24
#define CALL_NC_a16_CYCLES_MIN 12
#define CALL_NC_a16_LENGTH 3

#define PUSH_DE 0xD5
#define PUSH_DE_CYCLES 16
#define PUSH_DE_LENGTH 1

#define SUB_d8 0xD6
#define SUB_d8_CYCLES 8
#define SUB_d8_LENGTH 2

#define RST_10H 0xD7
#define RST_10H_CYCLES 16
#define RST_10H_LENGTH 1

#define RET_C 0xD8
#define RET_C_CYCLES_MAX 20
#define RET_C_CYCLES_MIN 8
#define RET_C_LENGTH 1

#define RETI 0xD9
#define RETI_CYCLES 16
#define RETI_LENGTH 1

#define JP_C_a16 0xDA
#define JP_C_a16_CYCLES_MAX 16
#define JP_C_a16_CYCLES_MIN 12
#define JP_C_a16_LENGTH 3

#define UNDEF_DB 0xDB
#define UNDEF_DB_CYCLES 4
#define UNDEF_DB_LENGTH 1

#define CALL_C_a16 0xDC
#define CALL_C_a16_CYCLES_MAX 24
#define CALL_C_a16_CYCLES_MIN 12
#define CALL_C_a16_LENGTH 3

#define UNDEF_DD 0xDD
#define UNDEF_DD_CYCLES 4
#define UNDEF_DD_LENGTH 1

#define SBC_A_d8 0xDE
#define SBC_A_d8_CYCLES 8
#define SBC_A_d8_LENGTH 2

#define RST_18H 0xDF
#define RST_18H_CYCLES 16
#define RST_18H_LENGTH 1

#define LDH_ma8_A 0xE0
#define LDH_ma8_A_CYCLES 12
#define LDH_ma8_A_LENGTH 2

#define POP_HL 0xE1
#define POP_HL_CYCLES 12
#define POP_HL_LENGTH 1

#define LD_mC_A 0xE2
#define LD_mC_A_CYCLES 8
#define LD_mC_A_LENGTH 1

#define UNDEF_E3 0xE3
#define UNDEF_E3_CYCLES 4
#define UNDEF_E3_LENGTH 1

#define UNDEF_E4 0xE4
#define UNDEF_E4_CYCLES 4
#define UNDEF_E4_LENGTH 1

#define PUSH_HL 0xE5
#define PUSH_HL_CYCLES 16
#define PUSH_HL_LENGTH 1

#define AND_d8 0xE6
#define AND_d8_CYCLES 8
#define AND_d8_LENGTH 2

#define RST_20H 0xE7
#define RST_20H_CYCLES 16
#define RST_20H_LENGTH 1

#define ADD_SP_r8 0xE8
#define ADD_SP_r8_CYCLES 16
#define ADD_SP_r8_LENGTH 2

#define JP_HL 0xE9
#define JP_HL_CYCLES 4
#define JP_HL_LENGTH 1

#define LD_ma16_A 0xEA
#define LD_ma16_A_CYCLES 16
#define LD_ma16_A_LENGTH 3

#define UNDEF_EB 0xEB
#define UNDEF_EB_CYCLES 4
#define UNDEF_EB_LENGTH 1

#define UNDEF_EC 0xEC
#define UNDEF_EC_CYCLES 4
#define UNDEF_EC_LENGTH 1

#define UNDEF_ED 0xED
#define UNDEF_ED_CYCLES 4
#define UNDEF_ED_LENGTH 1

#define XOR_d8 0xEE
#define XOR_d8_CYCLES 8
#define XOR_d8_LENGTH 2

#define RST_28H 0xEF
#define RST_28H_CYCLES 16
#define RST_28H_LENGTH 1

#define LDH_A_ma8 0xF0
#define LDH_A_ma8_CYCLES 12
#define LDH_A_ma8_LENGTH 2

#define POP_AF 0xF1
#define POP_AF_CYCLES 12
#define POP_AF_LENGTH 1

#define LD_A_mC 0xF2
#define LD_A_mC_CYCLES 8
#define LD_A_mC_LENGTH 1

#define DI 0xF3
#define DI_CYCLES 4
#define DI_LENGTH 1

#define UNDEF_F4 0xF4
#define UNDEF_F4_CYCLES 4
#define UNDEF_F4_LENGTH 1

#define PUSH_AF 0xF5
#define PUSH_AF_CYCLES 16
#define PUSH_AF_LENGTH 1

#define OR_d8 0xF6
#define OR_d8_CYCLES 8
#define OR_d8_LENGTH 2

#define RST_30H 0xF7
#define RST_30H_CYCLES 16
#define RST_30H_LENGTH 1

#define LD_HL_SPpr8 0xF8
#define LD_HL_SPpr8_CYCLES 12
#define LD_HL_SPpr8_LENGTH 2

#define LD_SP_HL 0xF9
#define LD_SP_HL_CYCLES 8
#define LD_SP_HL_LENGTH 1

#define LD_A_ma16 0xFA
#define LD_A_ma16_CYCLES 16
#define LD_A_ma16_LENGTH 3

#define EI 0xFB
#define EI_CYCLES 4
#define EI_LENGTH 1

#define UNDEF_FC 0xFC
#define UNDEF_FC_CYCLES 4
#define UNDEF_FC_LENGTH 1

#define UNDEF_FD 0xFD
#define UNDEF_FD_CYCLES 4
#define UNDEF_FD_LENGTH 1

#define CP_d8 0xFE
#define CP_d8_CYCLES 8
#define CP_d8_LENGTH 2

#define RST_38H 0xFF
#define RST_38H_CYCLES 16
#define RST_38H_LENGTH 1

//CB prefixed defines 
#define mHL_CYCLES 8 
#define NORMAL_CYCLES 8 

#define RLC 0
#define RRC 1
#define RL 2
#define RR 3
#define SLA 4
#define SRA 5
#define SWAP 6
#define SRL 7
#define BIT 8
#define RES 16
#define SET 24


#endif