/*
The header declaration for the GB's DMA and HDMA controllers.
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


class HDMAController {
private:
    bool transferActive = false;

    word targetAddress = 0x0000;
    word sourceAddress = 0x0000;

    bool generalPurpose = false;
    byte transferSize = 0;

    Memory* memory = nullptr;

public:
    /**
     * @brief Binds a memory module to the DMA Controller.
     * 
     * @param mem 
     */
    void bindMemory(Memory* mem) { memory = mem; }
    /**
     * @brief Sets the source address of the HDMA transfer.
     * 
     * @param data The data to set.
     * @param high Determines whether to update the high or low byte.
     */
    void setSourceAddr(byte data, bool high);
    /**
     * @brief Sets the target address of the HDMA transfer.
     * 
     * @param data The data to set.
     * @param high Determines whether to update the high or low byte.
     */
    void setTargetAddr(byte data, bool high);

    /**
     * @brief Sets the status register of the HDMA transfer.
     * 
     * @param data The data to set.
     */
    void setStatus(byte data);

    /**
     * @brief Returns the value of the HDMA transfer's status register.
     */
    byte readStatus();

    /**
     * @brief Performs a chunk of transfer work (0x10 bytes). Does nothing
     * if transferActive is false.
     */
    void transferChunk();

    /**
     * @brief Reset the HDMA Controller to its starting state.
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