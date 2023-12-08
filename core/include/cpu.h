#ifndef CPU_H
#define CPU_H
/**
 * CPU class declaration for a GameBoy Color emulator. 
 */
#include "defines.h"
#include "memory.h"
#include "register.h"
#include "instructionSet.h"
#include <vector>

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

    // A flag which determines if the CPU is currently in low power mode. This flag is turned on while the CPU is halted or stoped.
    bool lowPowerMode = false;

    // A flag to handle the double read coming out of halt.
    bool doubleReadBug = false;
    
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
    
    // A bool indicating the CPU is operating using the double speed clock,
    bool doubleSpeedMode = false;

    // Keeps track of the "cycle currency" the CPU can spend.
    cycles cyclesSinceLastInstuction = 0;

    // Used to disable all interrupts, overriding any enabled bits in the IE Register.
    bool masterInterruptEnableFlag = false;
    word activeInterruptVector = 0x0000;

public:
    CPU();

    /**
     * @brief Runs the initialize routine for the CPU.
     */
    void setInitalValues();
    
    /**
     * @brief Toggles the CPU's verbose variable. While verbose is set to true,
     * more std::cout messages will be printed to console displaying the CPUs
     * activity.
     */
    void toggleVerbose() {verbose = !verbose;}

    /**
     * @brief Set's the CPU's low power mode flag.
     */
    void setLowPowerMode(bool state){lowPowerMode = state;}

    /**
     * @brief Set's the CPU's double read bug flag to simulate a hardware issue.
     */
    void setDoubleReadBug(bool state){doubleReadBug = state;}
    /**
     * @brief Get's the current state of the CPU's double read bug flag.
     */
    bool getDoubleReadBug(){return doubleReadBug;}

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
     * @brief Attempt to run the CPU. This will increase the CPU's internal 
     * cycle counter. Once that cycle counter is high enough to run the desired
     * instuction, that instuction is actually executed.
     * 
     * This function will return the number of cycles the CPU ran for or 
     * 0 if the cycle counter in not yet high enough.
     * 
     * @return int 
     */
    cycles cycle();

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
    cycles getClockSpeed() { return doubleSpeedMode ? CLOCKSPEED_CGBMODE : CLOCKSPEED; }

    /**
     * @brief Returns the whether the CPU is running at double speed.
     * @return bool 
     */
    bool getDoubleSpeedMode(){return doubleSpeedMode;}
    
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

    /**
     * Returns the CPU's master interrupt enable flag.
    */
    bool getMasterInterruptEnabledFlag(){ return masterInterruptEnableFlag; }
    /**
     * @brief Disables all interrupts using the interrupt master 
     * enable flag.
     */
    void disableInterrupts();
    /**
     * @brief Enables all interrupts using the interrupt master 
     * enable flag.
     */
    void enableInterrupts();
    /**
     * @brief Sets the address of the active interrupt handler. This triggers the CPU 
     * to jump to this address during cycle().
    */
    void setActiveInterruptHandler(word interruptVectorAddress);
};

#endif