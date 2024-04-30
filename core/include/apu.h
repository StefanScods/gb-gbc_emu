#ifndef APU_H
#define APU_H
/**
 * Class declaration for a GameBoy Color APU for audio. 
 */
#include "defines.h"

struct ChannelControl {
    // Some channel-specific feature (if present).
    byte NRx0;
    // Controls the length timer.
    byte NRx1;
    // Controls the volume and envelope,
    byte NRx2;
    // Controls the period (maybe only partially),
    byte NRx3;
    // Has the channel’s trigger and length timer enable bits, as well as any leftover bits of period;
    byte NRx4;
};


class APU{
private:
    // Keeps track of the "cycle currency" the APU can spend.
    cycles cyclesCounter = 0;

    // The audio registers + 1 global control.
    ChannelControl audioRegs[NUMBER_OF_CHANNELS + 1];
    // Wave pattern RAM for channel 3.
    byte* wavePatternRAM = nullptr;


public:
    /**
     * @brief Dynamically allocates all blocks
     * of memory. Returns a bool indicating success.
     */
    bool init();
    /**
     * @brief Deallocates all dynamic memory.
     */
    void destroy();
        /**
     * @brief Resets the APU to inital values.
     */
    void reset();
    /**
     * @brief Perform any work need on this clock cycle. This will increase 
     * the APU's internal cycle counter.
     */
    void cycle();

    /**
     * @brief Read one of the APU's registers. Note for speed the 
     * arguments are not bound checked so ensure they are within the 
     * specified range.
     * 
     * @param channel Access audio channels 1-4 or global using 5.
     * @param reg Access the specific register (0-4).
     * 
     * @returns the data stored in the register.
     */
    byte readReg(byte channel, byte reg);
    /**
     * @brief Write to one of the APU's registers. Note for speed the 
     * arguments are not bound checked so ensure they are within the 
     * specified range.
     * 
     * @param channel Access audio channels 1-4 or global using 5.
     * @param reg Access the specific register (0-4).
     * @param data The data to store.
     */
    void writeReg(byte channel, byte reg, byte data);

    /**
     * @brief Read the APU's wave pattern RAM. Note for speed the 
     * arguments are not bound checked so ensure they are within the 
     * specified range.
     * 
     * @param address An address within the space 0xFF30–0xFF3F.
     * 
     * @returns the data stored.
     */
    byte readRAM(word address);
    /**
     * @brief Write to the APU's wave pattern RAM. Note for speed the 
     * arguments are not bound checked so ensure they are within the 
     * specified range.
     * 
     * @param address An address within the space 0xFF30–0xFF3F.
     * @param data The data to store.
     */
    void writeRAM(word address, byte data);

    /**
     * @brief Writes the current state to a state file.
     *
     * @param stateFile The file to write to.
    */
    void saveToState(std::ofstream & stateFile);
    /**
     * @brief Load the current state from a state file.
     *
     * @param stateFile The file to load from.
    */
    void loadFromState(std::ifstream & stateFile);
};

#endif 