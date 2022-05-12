#ifndef CPU_H
#define CPU_H
/*
CPU class delcaration for a gameboy color emulator 

date: 2021-11-12 
*/

#include "defines.h"
#include "memory.h"
#include "register.h"
#include "instructionSet.h"

// a generic pop function -> loads data pointed by the stack pointer into the dest reg -> incs the stack pointer by 2
void popHelper(CPU* cpu, reg& dest);

// a generic push function -> loads data saved in the source reg into the bytes  pointed by the stack pointer into -> decs the stack pointer by 2
void pushHelper(CPU* cpu, reg& source);

//takes a word and generates a four character hex output -> does not perform any error checks to make sure output buffer is valid 
void displayHex(word value, char* output);

class CPU {

    //friends all sets of instuctions so they can have access to the cpus private data 
    friend class MiscAndControl;
    friend class JumpAndCall;
    friend class LoadAndStore;
    friend class Bit;
    friend class Arithmetic;
    friend void popHelper(CPU*, reg&);
    friend void pushHelper(CPU*, reg&);

private:

    bool verbose = false;

    //main regs 
    reg reg_AF;
    reg reg_BC;
    reg reg_DE;
    reg reg_HL;

    //8bit addressable versions of the above regs  
    reg8 A = reg_AF.getHighPtr();
    reg8 F = reg_AF.getLowPtr();

    reg8 B = reg_BC.getHighPtr();
    reg8 C = reg_BC.getLowPtr();

    reg8 D = reg_DE.getHighPtr();
    reg8 E = reg_DE.getLowPtr();

    reg8 H = reg_HL.getHighPtr();
    reg8 L = reg_HL.getLowPtr();

    //stack pointer and program counter 
    reg SP;
    reg PC;

    //a word to hold parsed immiditate data from instructions 
    word parsedData;

    //all the instructions executable by the cpu 
    InstructionSet instructionSet;

    //a pointer to memory 
    Memory* memory = nullptr;

    //Interrupt Master Enable Flag
    bool ime; 

    //the clock speed of the cpu -> 4194304Hz or 8400000Hz in CGB mode 
    cycles clockSpeed = (cycles) CLOCKSPEED;

public:

    //run init rountine for the cpu 
    void init();
    
    //toggles certain print statments in the cpu code 
    void toggleVerbose() {verbose = !verbose;}

    //increments the pc by one 
    void inc_pc() {PC++;}

    //binds a memory module to the cpu -> all operations are done on the bound memory module
    void bindMemory(Memory* mem) {memory = mem;}

    /*runs the cpu for the specified number of cycles 

    return: the number of cycles executed*/
    int executeCycles(cycles numCycles);

    /*runs a single instuction

    return: the number of cycles executed*/
    cycles fetchAndExecute();

    //returns the clock speed of the cpu
    cycles getClockSpeed();

    //debug function -> prints the contents of the cpu
    void outputState();

    //fills the buffers with the current state of the cpu -> input buffer must be atleast 6 words / 4 chars wide
    void populateCpuStateBuffer(word* regs, char* flags);

};

#endif