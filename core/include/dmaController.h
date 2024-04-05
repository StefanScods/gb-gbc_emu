/*
The header declaration for the GB's DMA controller.
*/
#ifndef DMACONTROLLER_H
#define DMACONTROLLER_H

#include "defines.h"
#include "memory.h"

class DMAController {

private:
    bool transferActive = false;
    cycles cyclesPassed = 0;
    word targetAddress = 0x0000;

    Memory* memory = nullptr;

public:

    /**
     * @brief Set up the DMA Controller to begin a DMA transfer.
     */
    bool startTransfer(word startingAddress);

    /**
     * @brief Binds a memory module to the DMA Controller.
     * 
     * @param mem 
     */
    void bindMemory(Memory* mem) { memory = mem; }

    /**
     * @brief Perform any work need on this clock cycle. This will increase 
     * the DMA Controller's internal cycle counter.
     * 
     * @param cpuDoubleSpeed - A boolean indicating to cycle twice.
     */
    void cycle(bool cpuDoubleSpeed);

    /**
     * @brief Reset the DMA Controller to its starting state.
     */
    void reset();

    /**
     * @brief Writes the current state to a state file.
     *
     * @param writeBuffer The buffer to write to.
    */
    void saveToState(byte*& writeBuffer);
    /**
     * @brief Load the current state from a state file.
     *
     * @param readBuffer The buffer to read from.
    */
    void loadFromState(byte*& readBuffer);
};


#endif