#ifndef CPU_H
#define CPU_H
/**
 * CPU class declaration for a GameBoy Color emulator. 
 */
#include "defines.h"
#include "memory.h"
#include "register.h"
#include "instructionSet.h"

/**
 * @brief  A generic function used to implement the pop instructions. 
 * This function loads the data pointed by the stack pointer into the 
 * register passed as `dest`. The stack pointer is then incremented by 2.
 * 
 * @param cpu - The CPU running this instruction.
 * @param dest - The register to pop from stack into.
 */
void popHelper(CPU* cpu, reg& dest);

/**
 * @brief  A generic function used to implement the push instructions. 
 * This function stores the data from the register passed as `source` into
 * the bytes pointed to by the stack pointer. The stack pointer is then 
 * decremented by 2.
 * 
 * @param cpu - The CPU running this instruction.
 * @param source - The register to push into the stack.
 */
void pushHelper(CPU* cpu, reg& source);

/**
 * @brief A helper function for turning a word of data into a hex notation
 * string. Note: this function expects `output*` to be exactly the size of 
 * 7 chars. No error checking is done to avoid a segmentation fault.
 * 
 * @param value - The word to convert into hex notation.
 * @param output - The output string buffer to populate.
 */
void convertWordToHexNotation(word value, char* output);

class CPU {

    // Friends all sets of instructions so they can have access to the CPUs private data. 
    friend class MiscAndControl;
    friend class JumpAndCall;
    friend class LoadAndStore;
    friend class Bit;
    friend class Arithmetic;
    friend void popHelper(CPU*, reg&);
    friend void pushHelper(CPU*, reg&);

private:
    // A flag used to determine whether debug messages are printed to console.
    bool verbose = false;

    // Main Registers. 
    reg reg_AF;
    reg reg_BC;
    reg reg_DE;
    reg reg_HL;

    // 8bit addressable versions of the above registers. 
    reg8 A = reg_AF.getHighPtr();
    reg8 F = reg_AF.getLowPtr();

    reg8 B = reg_BC.getHighPtr();
    reg8 C = reg_BC.getLowPtr();

    reg8 D = reg_DE.getHighPtr();
    reg8 E = reg_DE.getLowPtr();

    reg8 H = reg_HL.getHighPtr();
    reg8 L = reg_HL.getLowPtr();

    // Stack pointer and program counter registers.
    reg SP;
    reg PC;

    // A word to hold the parsed intermediate data from instructions. 
    word parsedData;

    // All the instructions executable by the CPU; 
    InstructionSet instructionSet;

    // A pointer to memory. 
    Memory* memory = nullptr;

    // Interrupt Master Enable Flag.
    bool ime; 

    // The clock speed of the CPU -> 4194304Hz or 8400000Hz in CGB mode 
    cycles clockSpeed = (cycles) CLOCKSPEED;

public:

    /**
     * @brief Runs the initialize routine for the CPU.
     */
    void init();
    
    /**
     * @brief Toggles the CPU's verbose variable. While verbose is set to true,
     * more std::cout messages will be printed to console displaying the CPUs
     * activity.
     */
    void toggleVerbose() {verbose = !verbose;}

    /**
     * @brief Reads the next instruction byte as pointed to by the PC. Returns
     * the value of memory pointed to by the PC. Afterwards increases the PC
     * by one.
     * 
     * @return byte 
     */
    byte readNextInstructionByte();

    /**
     * @brief Increment the program counter by one.
     */
    void incPC() {PC++;}
    /**
     * @brief Returns the current value of the program counter.
     * 
     * @returns word - The current value of the PC register.
     */
    word getPC(){ return PC.read(); }

    /**
     * @brief Binds a memory module to the CPU. All future CPU operations are
     * performed on bound memory.
     * 
     * @param mem 
     */
    void bindMemory(Memory* mem) { memory = mem; }

    /**
     * @brief Runs the CPU for the specified number of cycles.
     * Returns the number of cycles the CPU actually ran for. 
     * If the returned value is negative, the CPU ran for longer 
     * than the desired cycles.
     * 
     * @param numCycles The number of cycles to execute for.
     * 
     * @return int 
     */
    int executeCycles(cycles numCycles);

    /**
     * @brief Fetches and executes the next instuction pointed to by
     * the CPU's program counter register. Returns the number of 
     * cycles the CPU ran for.
     * 
     * @return cycles 
     */
    cycles fetchAndExecute();

    /**
     * @brief Returns the current clock speed the CPU is running at.
     * 
     * @return cycles 
     */
    cycles getClockSpeed() { return clockSpeed; }

    /**
     * @brief Prints the current state of the CPU to std::cout.
     */
    void printCurrentState();

    /**
     * @brief Populates a CPU_State struct with the current state of 
     * the CPU. This function can be used to pipe the current state 
     * of the CPU to other parts of the emulator application or to
     * serialize the CPU for save stating.
     * 
     * @param CPU_StateBuffer The buffer to populate. 
     */
    void populateCpuStateBuffer(CPU_State* CPU_StateBuffer);
};

#endif