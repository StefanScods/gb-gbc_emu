/**
 * CPU class implementation for a GameBoy Color emulator.
 */

#include "include\cpu.h"
#include <iostream>

void popHelper(CPU *cpu, reg &dest)
{
    // Pop the data off the stack.
    cpu->SP++;
    word data = cpu->memory->read(cpu->SP.read());
    cpu->SP++;
    data = data | (cpu->memory->read(cpu->SP.read())) << 8;

    // Load reg with the popped data.
    dest = data;
}

void pushHelper(CPU *cpu, reg &source)
{
    word data = source.read();

    // Push the data onto the stack.
    cpu->memory->write(cpu->SP.read(), data >> 8);
    cpu->SP--;
    cpu->memory->write(cpu->SP.read(), data & 0xFF);
    cpu->SP--;
}

void convertWordToHexNotation(word value, char *output)
{
    output[6] = NULL;
    output[5] = toHex[value % 16];
    value = value >> 4;
    output[4] = toHex[value % 16];
    value = value >> 4;
    output[3] = toHex[value % 16];
    value = value >> 4;
    output[2] = toHex[value % 16];
    output[1] = 'x';
    output[0] = '0';
}

void convertWordToBinaryNotation(word value, char *output)
{
    output[9] = NULL;
    int offset = 1;
    for(int i = 7 ; i >= 0; i--){
        output[ i + offset] = (value % 2) ? '1' : '0';
        value = value >> 1;

        if(i % 4 == 0) offset--;
    }
}

void CPU::init()
{
    // Return early if no memory has been bound.
    if (memory == nullptr)
    {
        std::cerr << "Error: Must Bind Memory to CPU First!" << std::endl;
        return;
    }

    // Init routine according to http://bgb.bircd.org/pandocs.htm#powerupsequence.
    reg_AF = (word)0x01B0;
    reg_BC = (word)0x0013;
    reg_DE = (word)0x00D8;
    reg_HL = (word)0x014D;

    PC = (word)0x0100;
    SP = (word)0xFFFE;

    memory->write(0xFF05, 0x00); // TIMA
    memory->write(0xFF06, 0x00); // TMA
    memory->write(0xFF07, 0x00); // TAC
    memory->write(0xFF10, 0x80); // NR10
    memory->write(0xFF11, 0xBF); // NR11
    memory->write(0xFF12, 0xF3); // NR12
    memory->write(0xFF14, 0xBF); // NR14
    memory->write(0xFF16, 0x3F); // NR21
    memory->write(0xFF17, 0x00); // NR22
    memory->write(0xFF19, 0xBF); // NR24
    memory->write(0xFF1A, 0x7F); // NR30
    memory->write(0xFF1B, 0xFF); // NR31
    memory->write(0xFF1C, 0x9F); // NR32
    memory->write(0xFF1E, 0xBF); // NR33
    memory->write(0xFF20, 0xFF); // NR41
    memory->write(0xFF21, 0x00); // NR42
    memory->write(0xFF22, 0x00); // NR43
    memory->write(0xFF23, 0xBF); // NR30
    memory->write(0xFF24, 0x77); // NR50
    memory->write(0xFF25, 0xF3); // NR51
    memory->write(0xFF26, 0xF1); // NR52
    memory->write(0xFF40, 0x91); // LCDC
    memory->write(0xFF42, 0x00); // SCY
    memory->write(0xFF43, 0x00); // SCX
    memory->write(0xFF45, 0x00); // LYC
    memory->write(0xFF47, 0xFC); // BGP
    memory->write(0xFF48, 0xFF); // OBP0
    memory->write(0xFF49, 0xFF); // OBP1
    memory->write(0xFF4A, 0x00); // WY
    memory->write(0xFF4B, 0x00); // WX
    memory->write(0xFFFF, 0x00); // IE
    
    initializeVRAM();
}

void CPU::initializeVRAM(){   
    // No error checking since the `init` function provides safety net code.

    // Overwrite the first 416 bytes of VRAM with the Nintendo logo. 
    const int VALUES_TO_EDIT = 416;
    const byte INITAL_VRAM_STATE[VALUES_TO_EDIT] =
        { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
          0xF0, 0x00, 0xF0, 0x00, 0xFC, 0x00, 0xFC, 0x00, 0xFC, 0x00, 0xFC, 0x00, 0xF3, 0x00, 0xF3, 0x00, 
          0x3C, 0x00, 0x3C, 0x00, 0x3C, 0x00, 0x3C, 0x00, 0x3C, 0x00, 0x3C, 0x00, 0x3C, 0x00, 0x3C, 0x00, 
          0xF0, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF3, 0x00, 0xF3, 0x00, 
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCF, 0x00, 0xCF, 0x00, 
          0x00, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x0F, 0x00, 0x3F, 0x00, 0x3F, 0x00, 0x0F, 0x00, 0x0F, 0x00, 
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0x0F, 0x00, 0x0F, 0x00, 
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x00, 0xF0, 0x00, 
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF3, 0x00, 0xF3, 0x00, 
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0xC0, 0x00, 
          0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x00, 0xFF, 0x00, 0xFF, 0x00, 
          0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC0, 0x00, 0xC3, 0x00, 0xC3, 0x00, 
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x00, 0xFC, 0x00, 
          0xF3, 0x00, 0xF3, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0xF0, 0x00, 
          0x3C, 0x00, 0x3C, 0x00, 0xFC, 0x00, 0xFC, 0x00, 0xFC, 0x00, 0xFC, 0x00, 0x3C, 0x00, 0x3C, 0x00, 
          0xF3, 0x00, 0xF3, 0x00, 0xF3, 0x00, 0xF3, 0x00, 0xF3, 0x00, 0xF3, 0x00, 0xF3, 0x00, 0xF3, 0x00,
          0xF3, 0x00, 0xF3, 0x00, 0xC3, 0x00, 0xC3, 0x00, 0xC3, 0x00, 0xC3, 0x00, 0xC3, 0x00, 0xC3, 0x00,
          0xCF, 0x00, 0xCF, 0x00, 0xCF, 0x00, 0xCF, 0x00, 0xCF, 0x00, 0xCF, 0x00, 0xCF, 0x00, 0xCF, 0x00,
          0x3C, 0x00, 0x3C, 0x00, 0x3F, 0x00, 0x3F, 0x00, 0x3C, 0x00, 0x3C, 0x00, 0x0F, 0x00, 0x0F, 0x00, 
          0x3C, 0x00, 0x3C, 0x00, 0xFC, 0x00, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x00, 0xFC, 0x00, 
          0xFC, 0x00, 0xFC, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0xF0, 0x00, 0xF0, 0x00, 
          0xF3, 0x00, 0xF3, 0x00, 0xF3, 0x00, 0xF3, 0x00, 0xF3, 0x00, 0xF3, 0x00, 0xF0, 0x00, 0xF0, 0x00, 
          0xC3, 0x00, 0xC3, 0x00, 0xC3, 0x00, 0xC3, 0x00, 0xC3, 0x00, 0xC3, 0x00, 0xFF, 0x00, 0xFF, 0x00, 
          0xCF, 0x00, 0xCF, 0x00, 0xCF, 0x00, 0xCF, 0x00, 0xCF, 0x00, 0xCF, 0x00, 0xC3, 0x00, 0xC3, 0x00, 
          0x0F, 0x00, 0x0F, 0x00, 0x0F, 0x00, 0x0F, 0x00, 0x0F, 0x00, 0x0F, 0x00, 0xFC, 0x00, 0xFC, 0x00,
          0x3C, 0x00, 0x42, 0x00, 0xB9, 0x00, 0xA5, 0x00, 0xB9, 0x00, 0xA5, 0x00, 0x42, 0x00, 0x3C, 0x00 };
    for(word i = 0; i<VALUES_TO_EDIT; i++){
        memory->write(VRAM_START+i, INITAL_VRAM_STATE[i]);
    }
}

byte CPU::readNextInstructionByte()
{
    byte nextOpcode = memory->read(PC.read());
    incPC();

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

    // Execute the instuction and return number of cycles.
    return nextInstruction.execute(this);
}

int CPU::executeCycles(cycles numCycles)
{

    cycles cyclesRemaining = numCycles;

    // Runs the cpu in a loop for the desired amount of cycles.
    while (cyclesRemaining > 0)
    {

        // Read the next instuction to check its cycle timing.
        byte nextOpcode = memory->read(PC.read());
        Instruction nextInstruction = instructionSet.set[nextOpcode];
        // Return early if the next instruction will exceed the cyclesRemaining.
        if (cyclesRemaining < nextInstruction.cycleCount)
            break;

        // Execute the next instruction.
        cycles cyclesUsed = fetchAndExecute();
        // Reduce the remaining cycles.
        cyclesRemaining -= cyclesUsed;
    }

    // Return the deficit of desired cycles vs actual cycles.
    return (int)numCycles - (int)cyclesRemaining;
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
    ime = false;
    memory->write(INTERRUPT_ENABLE_REGISTER_ADDR, 0x00);
}
void CPU::enableInterrupts()
{
    // todo!!! delay this such that the code "ei; di;" would prevent any interrupts.
    ime = true;
    memory->write(INTERRUPT_ENABLE_REGISTER_ADDR, 0x01);
}
