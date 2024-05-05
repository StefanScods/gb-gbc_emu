#ifndef APU_H
#define APU_H
/**
 * Class declaration for a GameBoy Color APU for audio. 
 */
#include "defines.h"
#include "audioDefines.h"

class APU;
/**
 * Base class for the GBs audio channels.
*/
class ChannelController {
protected:
    // Keeps track of the "cycle currency" this channel can spend.
    cycles cyclesCounter = 0;

    word periodDivider = 0;
    bool on = false;
    byte timer = 0;
    byte volume = 0;

    byte initalLenTimer = 0;

    word period = 0;
    word initalPeriod = 0;

    bool lengthEnable = 0;

    // Placeholder for no audio.
    AudioChannelData NO_AUDIO = {};

public:
    /**
     * @brief Reads the NRx0 register.
     * 
     * @returns the data stored in the register.
     */
    virtual byte readNRx0() {return HIGH_IMPEDANCE;}
    /**
     * @brief Reads the NRx1 register.
     * 
     * @returns the data stored in the register.
     */
    virtual byte readNRx1() {return HIGH_IMPEDANCE;}
    /**
     * @brief Reads the NRx2 register.
     * 
     * @returns the data stored in the register.
     */
    virtual byte readNRx2() {return HIGH_IMPEDANCE;}
    /**
     * @brief Reads the NRx3 register.
     * 
     * @returns the data stored in the register.
     */
    virtual byte readNRx3() {return HIGH_IMPEDANCE;}
    /**
     * @brief Reads the NRx4 register.
     * 
     * @returns the data stored in the register.
     */
    virtual byte readNRx4() {return HIGH_IMPEDANCE;}
    /**
     * @brief Writes to the NRx0 register.
     * 
     * @param data The data to store.
     */
    virtual void writeNRx0(byte data){};
        /**
     * @brief Writes to the NRx1 register.
     * 
     * @param data The data to store.
     */
    virtual void writeNRx1(byte data){};
        /**
     * @brief Writes to the NRx2 register.
     * 
     * @param data The data to store.
     */
    virtual void writeNRx2(byte data){};
        /**
     * @brief Writes to the NRx3 register.
     * 
     * @param data The data to store.
     */
    virtual void writeNRx3(byte data){};
    /**
     * @brief Writes to the NRx4 register.
     * 
     * @param data The data to store.
     */
    virtual void writeNRx4(byte data){};

    /**
     * @brief Perform any work need on this clock cycle.
     * 
     * @param divAPUEvents the current state of the “DIV-APU” counter.
     */
    virtual void cycle(byte divAPUEvents) {cyclesCounter++;};

    /**
     * @brief Triggers the channel to start playing.
     */
    virtual void trigger();

    /**
     * @brief Returns any audio data for this clock cycle.
     */
    virtual AudioChannelData sample() {return NO_AUDIO;};

    /**
     * @brief Resets the channel to inital values.
     */
    virtual void reset() {
        cyclesCounter = 0;
        periodDivider = 0;
        on = false;
        timer = 0;
        volume = 0;
          
        initalLenTimer = 0;

        period = 0;
        initalPeriod = 0;

        lengthEnable = 0;
    };

    /**
     * @brief Turn the channel off or on.
     */
    void setOn(bool state){on = state; if(on) trigger();}
    bool getOn(){return on;}

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

/**
 * Class for the global audio controller.
*/
class GlobalAudio : public ChannelController {
    friend class APU;
protected:
    bool masterOn = false;
    byte panning = 0;
    byte masterVolume = 0;
public:
    /**
     * @brief Resets the channel to inital values.
     */
    void reset() {
        masterOn = false;
        panning = 0;
        masterVolume = 0;
    };
    /**
     * @brief Reads the NRx0 register.
     * 
     * @returns the data stored in the register.
     */
    byte readNRx0();
    /**
     * @brief Reads the NRx1 register.
     * 
     * @returns the data stored in the register.
     */
    byte readNRx1();
    /**
     * @brief Writes to the NRx0 register.
     * 
     * @param data The data to store.
     */
    void writeNRx0(byte data);
    /**
     * @brief Reads the NRx2 register.
     * 
     * @returns the data stored in the register.
     */
    byte readNRx2(byte otherRegs);
        /**
     * @brief Writes to the NRx1 register.
     * 
     * @param data The data to store.
     */
    void writeNRx1(byte data);
        /**
     * @brief Writes to the NRx2 register.
     * 
     * @param data The data to store.
     */
    void writeNRx2(byte data);

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


/**
 * Class for channel 2 and base class for channel 1.
*/
class Pulse : public ChannelController {
protected:
    byte pace = 0;
    bool direction = 0;
    byte invidualStep = 0;
    cycles periodSweepTicks = 0;

    byte waveDuty = 0;
    byte initalLenTimer = 0;

    byte initialVolume = 0;
    bool envDir = 0;
    byte sweepPace = 0;

    cycles envSweepTicks = 0;
public:
    /**
     * @brief Reads the NRx1 register.
     * 
     * @returns the data stored in the register.
     */
    byte readNRx1();
    /**
     * @brief Reads the NRx2 register.
     * 
     * @returns the data stored in the register.
     */
    byte readNRx2();
    /**
     * @brief Reads the NRx4 register.
     * 
     * @returns the data stored in the register.
     */
    byte readNRx4();
     /**
     * @brief Writes to the NRx1 register.
     * 
     * @param data The data to store.
     */
    void writeNRx1(byte data);
        /**
     * @brief Writes to the NRx2 register.
     * 
     * @param data The data to store.
     */
    void writeNRx2(byte data);
        /**
     * @brief Writes to the NRx3 register.
     * 
     * @param data The data to store.
     */
    void writeNRx3(byte data);
    /**
     * @brief Writes to the NRx4 register.
     * 
     * @param data The data to store.
     */
    void writeNRx4(byte data);

    /**
     * @brief Returns any audio data for this clock cycle.
     */
    AudioChannelData sample();

    /**
     * @brief Resets the channel to inital values.
     */
    void reset();

    /**
     * @brief Perform any work need on this clock cycle.
     *      
     * @param divAPUEvents the current state of the “DIV-APU” counter.
     */
    void cycle(byte divAPUEvents);

    /**
     * @brief Triggers the channel to start playing.
     */
    void trigger();

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

/**
 * Class for channel 1
*/
class PulseSweep : public Pulse {
public:
    /**
     * @brief Reads the NRx0 register.
     * 
     * @returns the data stored in the register.
     */
    byte readNRx0();
    /**
     * @brief Writes to the NRx0 register.
     * 
     * @param data The data to store.
     */
    void writeNRx0(byte data);

    /**
     * @brief Perform any work need on this clock cycle.
     *      
     * @param divAPUEvents the current state of the “DIV-APU” counter.
     */
    void cycle(byte divAPUEvents);
};


/**
 * Class for channel 3.
*/
class Wave : public ChannelController {

public:
    /**
     * @brief Reads the NRx0 register.
     * 
     * @returns the data stored in the register.
     */
    byte readNRx0();
    /**
     * @brief Reads the NRx2 register.
     * 
     * @returns the data stored in the register.
     */
    byte readNRx2();
    /**
     * @brief Reads the NRx4 register.
     * 
     * @returns the data stored in the register.
     */
    byte readNRx4();
    /**
     * @brief Writes to the NRx0 register.
     * 
     * @param data The data to store.
     */
    void writeNRx0(byte data);
    /**
     * @brief Writes to the NRx1 register.
     * 
     * @param data The data to store.
     */
    void writeNRx1(byte data);
     /**
     * @brief Writes to the NRx2 register.
     * 
     * @param data The data to store.
     */
    void writeNRx2(byte data);
    /**
     * @brief Writes to the NRx3 register.
     * 
     * @param data The data to store.
     */
    void writeNRx3(byte data);
    /**
     * @brief Writes to the NRx4 register.
     * 
     * @param data The data to store.
     */
    void writeNRx4(byte data);

    /**
     * @brief Returns any audio data for this clock cycle.
     */
    AudioChannelData sample(byte* waveRam);

    /**
     * @brief Perform any work need on this clock cycle.
     */
    void cycle(byte divAPUEvents);
};

/**
 * Class for channel 4.
*/
class Noise : public ChannelController {
private:
    byte initialVolume = 0;
    bool envDir = 0;
    byte sweepPace = 0;

    byte clockShift = 0;
    bool LFSRWidth = 0;
    byte clockDivider = 0;

    cycles envSweepTicks = 0;

    word LFSR = 0;
    bool shiftedOutBit = 0;
public:
    /**
     * @brief Reads the NRx1 register.
     * 
     * @returns the data stored in the register.
     */
    byte readNRx1();
    /**
     * @brief Reads the NRx2 register.
     * 
     * @returns the data stored in the register.
     */
    byte readNRx2();
    /**
     * @brief Reads the NRx3 register.
     * 
     * @returns the data stored in the register.
     */
    byte readNRx3();
    /**
     * @brief Reads the NRx4 register.
     * 
     * @returns the data stored in the register.
     */
    byte readNRx4();
    /**
     * @brief Writes to the NRx1 register.
     * 
     * @param data The data to store.
     */
    void writeNRx1(byte data);
    /**
     * @brief Writes to the NRx2 register.
     * 
     * @param data The data to store.
     */
    void writeNRx2(byte data);
    /**
     * @brief Writes to the NRx3 register.
     * 
     * @param data The data to store.
     */
    void writeNRx3(byte data);
    /**
     * @brief Writes to the NRx4 register.
     * 
     * @param data The data to store.
     */
    void writeNRx4(byte data);

    /**
     * @brief Returns any audio data for this clock cycle.
     */
    AudioChannelData sample();

    /**
     * @brief Resets the channel to inital values.
     */
    void reset();

    /**
     * @brief Perform any work need on this clock cycle.
     */
    void cycle(byte divAPUEvents);

    /**
     * @brief Triggers the channel to start playing.
     */
    void trigger();

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


class APU{
private:
    // A circular buffer for audio data to be played aloud.
    uint32_t readLocation = 0;
    uint32_t writeLocation = 0;
    AudioChannelData* audioData = nullptr;

    // Keeps track of the "cycle currency" the APU can spend.
    cycles cyclesCounter = 0;

    // The audio registers + 1 global control.
    ChannelController* audioRegs[NUMBER_OF_CHANNELS + 1];

    GlobalAudio globalChannel;
    PulseSweep channel1;
    Pulse channel2;
    Wave channel3;
    Noise channel4;

    // Wave pattern RAM for channel 3.
    byte* wavePatternRAM = nullptr;
;
    // Placeholder for no audio.
    AudioChannelData NO_AUDIO = {};

public:
    APU();
    /**
     * @brief Fetches the buffer of audio data used to supply the application 
     * with audio data.
     */
    AudioChannelData* fetchAudioData();
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
     * @brief Perform any work need on this clock cycle.
     * 
     * @param divAPUEvents the current state of the “DIV-APU” counter.
     */
    void cycle(byte divAPUEvents);
    
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