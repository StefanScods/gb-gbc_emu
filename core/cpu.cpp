/**
 * CPU class implementation for a GameBoy Color emulator.
 */

#include "include\cpu.h"
#include <iostream>

void popHelper(CPU *cpu, reg &dest)
{
    // Pop the data off the stack.
    word data = cpu->memory->read(cpu->SP.read());
    cpu->SP++;
    data = data | (cpu->memory->read(cpu->SP.read())) << 8;
    cpu->SP++;

    // Load reg with the popped data.
    dest = data;
}

void pushHelper(CPU *cpu, reg &source)
{
    word data = source.read();

    // Push the data onto the stack.
    cpu->SP--;
    cpu->memory->write(cpu->SP.read(), data >> 8);
    cpu->SP--;
    cpu->memory->write(cpu->SP.read(), data & 0xFF);
}

CPU::CPU(){
    reg_AF.setAsFlagReg();
}

void CPU::setInitalValues()
{
    // Init routine according to http://bgb.bircd.org/pandocs.htm#powerupsequence.
    reg_AF = (word)0x01B0;
    reg_BC = (word)0x0013;
    reg_DE = (word)0x00D8;
    reg_HL = (word)0x014D;

    PC = (word)0x0100;
    SP = (word)0xFFFE;

    masterInterruptEnableFlag = true;
    activeInterruptVector = 0x0000;
}

byte CPU::readNextInstructionByte()
{
    // Fetch the next byte of the instruction.
    byte nextOpcode = memory->read(PC.read());

    // If the CPU is currently facing the double read bug, don't increase PC this time.
    if(doubleReadBug) doubleReadBug = false;
    // Else increase the PC.
    else incPC();
    
    return nextOpcode;
}

cycles CPU::fetchAndExecute()
{
    // Read the next instuction and increment PC.
    byte nextOpcode = readNextInstructionByte();

    // Translate the OP code into a instruction.
    Instruction nextInstruction = instructionSet.set[nextOpcode];

    // If the instuction has immediate data, additional memory needs are necessary to populate parsed data.
    switch (nextInstruction.length)
    {
    case 2:
        // Constructs a word from the two bytes of parsed data.
        // PC+1 is placed in the lower 8 bits.
        // Upper 8 bits are left unused.
        parsedData = readNextInstructionByte();
        break;
    case 3:
        // Constructs a word from the two bytes of parsed data.
        // PC+1 is placed in the lower 8 bits.
        // PC+2 is placed in the upper 8 bits.
        parsedData = readNextInstructionByte() | (readNextInstructionByte() << 8);
        break;

    default:
        break;
    }

    // Print OP code data.
    if (verbose)
    {
        std::cout << nextInstruction.opcode << std::endl;
    }

    // Clear the cashed cycles.
    cyclesSinceLastInstuction = 0;
    // Execute the instuction and return number of cycles.
    cycles executedCycles =  nextInstruction.execute(this);
    return executedCycles;
}

cycles CPU::cycle(){
    // If we are in low power mode do nothing.
    if(lowPowerMode){
        // If theres at least one cycle in halt, we are not effected by the double read bug.
        setDoubleReadBug(false);
        return 0;
    }

    cyclesSinceLastInstuction++;
    // Run for two cycles if running in double speed mode.
    if(doubleSpeedMode) cyclesSinceLastInstuction++;

    // Handle interrupts if there is an active handler.
    if(activeInterruptVector != 0x0000){
        // It takes 5 cycles to swap to the interrupt handler.
        if(5 > cyclesSinceLastInstuction) return 0;
        cyclesSinceLastInstuction-=5;

        // Push the current value of PC to the stack.
        pushHelper(this, PC);
        // Update PC to point to the interrupt vector.
        PC = activeInterruptVector;
        // Clear the active interrupt vector.
        activeInterruptVector = 0x0000;
        // Return the 5 cycles it took to do the above work.
        return 5;
    }

    // Check the next instuction.
    Instruction nextInstruction = instructionSet.set[memory->read(PC.read())];
    if(nextInstruction.cycleCount <= cyclesSinceLastInstuction){
       return fetchAndExecute();
    }

    // CPU does "nothing" this cycle.
    return 0;
}

void CPU::printCurrentState()
{

    // Switch to hex notation display.
    std::cout << std::hex;

    std::cout << "========================" << std::endl;
    std::cout << "AF: 0x" << (int)reg_AF.read() << std::endl;
    std::cout << "BC: 0x" << (int)reg_BC.read() << std::endl;
    std::cout << "DE: 0x" << (int)reg_DE.read() << std::endl;
    std::cout << "HL: 0x" << (int)reg_HL.read() << std::endl;
    std::cout << "\nSP: 0x" << (int)SP.read() << std::endl;
    std::cout << "PC: 0x" << (int)PC.read() << std::endl;

    std::cout << "FLAGS: Z: " << readBit(*(F), FLAG_Z)
              << " N: " << readBit(*(F), FLAG_N)
              << " H: " << readBit(*(F), FLAG_H)
              << " C: " << readBit(*(F), FLAG_C) << std::endl;

    std::cout << "========================" << std::endl;

    // Return back to a decimal notation display.
    std::cout << std::dec;
}

void CPU::populateCpuStateBuffer(CPU_State *CPU_StateBuffer)
{
    // Return early if received a nullptr.
    if (CPU_StateBuffer == nullptr)
    {
        std::cerr << "Error: Null pointer passed into `CPU::populateCpuStateBuffer()`" << std::endl;
        return;
    }

    // Populate the buffer with the CPUs current state.
    CPU_StateBuffer->reg_AF = reg_AF.read();
    CPU_StateBuffer->reg_BC = reg_BC.read();
    CPU_StateBuffer->reg_DE = reg_DE.read();
    CPU_StateBuffer->reg_HL = reg_HL.read();
    CPU_StateBuffer->SP = SP.read();
    CPU_StateBuffer->PC = PC.read();
}

void CPU::disableInterrupts()
{
    // Disable all interrupts.
    activeInterruptVector = 0x0000;
    masterInterruptEnableFlag = false;
}
void CPU::enableInterrupts()
{
    // Enable the interrupts defined in IE register.
    masterInterruptEnableFlag = true;
}

void CPU::setActiveInterruptHandler(word interruptVectorAddress) {
    // Turn off interrupts while the handler is active.
    disableInterrupts();
    // If we run an interrupt handler we are not effected by the double read bug.
    setDoubleReadBug(false);
    // Set the interrupt handler address.
    activeInterruptVector = interruptVectorAddress;
    // Clear the cashed cycles.
    cyclesSinceLastInstuction = 0;
}