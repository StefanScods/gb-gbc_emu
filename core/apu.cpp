/*
The class implementation for the emulator's APU for audio. 
*/
#include "include/apu.h"



// Enables debug cout statements for this file.
#define ENABLE_DEBUG_PRINTS True

bool APU::init(){
    wavePatternRAM = new byte[WAVEPATTERNRAME_END - WAVEPATTERNRAME_START + 1];
    if (wavePatternRAM == nullptr)
        return false;

    audioData = new AudioChannelData[AUDIO_FRAMES_PER_BUFFER];
    if (audioData == nullptr)
        return false;

    reset();
    return true;
}

void APU::destroy(){
    // Deallocates the data.
    delete[] wavePatternRAM;
    delete[] audioData;
}

void APU::reset(){
    // Zero all memory.
    cyclesCounter = 0;
    std::fill(
        wavePatternRAM, 
        wavePatternRAM+(WAVEPATTERNRAME_END - WAVEPATTERNRAME_START + 1), 
        0
    );
    memset(audioData, 0, (AUDIO_FRAMES_PER_BUFFER)*sizeof(AudioChannelData));
    for(int i =0; i < NUMBER_OF_CHANNELS + 1; i++){
        audioRegs[i] = {};
    }
}

byte APU::readReg(byte channel, byte reg){
    switch (reg){
        case 0:
            return audioRegs[channel- 1].NRx0; 
        case 1:
            return audioRegs[channel- 1].NRx1; 
        case 2:
            return audioRegs[channel- 1].NRx2; 
        case 3:
            return audioRegs[channel- 1].NRx3;
        case 4:
            return audioRegs[channel- 1].NRx4;   
        default:
            return HIGH_IMPEDANCE;
    }
    return HIGH_IMPEDANCE;
}

void APU::writeReg(byte channel, byte reg, byte data){
    switch (reg){
        case 0:
            audioRegs[channel- 1].NRx0 = data;
            break;
        case 1:
            audioRegs[channel- 1].NRx1 = data;
            break;
        case 2:
            audioRegs[channel- 1].NRx2 = data;
            break; 
        case 3:
            audioRegs[channel- 1].NRx3 = data;
            break;
        case 4:
            audioRegs[channel- 1].NRx4 = data;
            break;  
        default:
            break;
    }
}

byte APU::readRAM(word address){
    return wavePatternRAM[address-WAVEPATTERNRAME_START];
}

void APU::writeRAM(word address, byte data){
    wavePatternRAM[address-WAVEPATTERNRAME_START] = data;
}


void APU::cycle(){
}

AudioChannelData* APU::fetchAudioData(){
    // Generate the sine wave
    for (unsigned int i = 0; i < AUDIO_FRAMES_PER_BUFFER; i++) {
        audioData[i].left = sin(phase * 2.0 * M_PI);
        audioData[i].right = sin(phase * 2.0 * M_PI);
        phase += 500.0 / SAMPLE_RATE;
        if (phase > 1.0) phase -= 1.0;
    }
    return audioData;
}

void APU::saveToState(std::ofstream & stateFile){
}

void APU::loadFromState(std::ifstream & stateFile){
}