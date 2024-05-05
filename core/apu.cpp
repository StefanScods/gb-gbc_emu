/*
The class implementation for the emulator's APU for audio. 
*/
#include "include/apu.h"
#include "include/register.h"
#include <algorithm>
#include <cmath>

// Enables debug cout statements for this file.
#define ENABLE_DEBUG_PRINTS False

APU::APU(){
    audioRegs[0] = &channel1;
    audioRegs[1] = &channel2;
    audioRegs[2] = &channel3;
    audioRegs[3] = &channel4;
    audioRegs[4] = &globalChannel;
}

bool APU::init(){
    wavePatternRAM = new byte[WAVE_RAM_SIZE];
    if (wavePatternRAM == nullptr)
        return false;

    audioData = new AudioChannelData[AUDIO_FRAMES_PER_BUFFER*BUFFER_SCALE];
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
    readLocation = 0;
    writeLocation = 0;
    memset(audioData, 0, AUDIO_BUFFER_SIZE*sizeof(AudioChannelData));

    std::fill(
        wavePatternRAM, 
        wavePatternRAM+(WAVE_RAM_SIZE), 
        0
    );

    for(int i =0; i < NUMBER_OF_CHANNELS + 1; i++){
        audioRegs[i]->reset();
    }
}

void Pulse::reset(){
    ChannelController::reset();
    pace = 0;
    direction = 0;
    invidualStep = 0;
    waveDuty = 0;
    envDir = 0;
    sweepPace = 0;
    envSweepTicks = 0;
    periodSweepTicks = 0;
    initialVolume = 0;
}
void Noise::reset(){
    ChannelController::reset();
    envDir = 0;
    sweepPace = 0;
    initialVolume = 0;
    clockShift = 0;
    LFSRWidth = 0;
    clockDivider = 0;
    envSweepTicks = 0;
    LFSR = 0;
    shiftedOutBit = 0;
}

byte APU::readReg(byte channel, byte reg){
    switch (reg){
        case 0:
            return audioRegs[channel- 1]->readNRx0(); 
        case 1:
            return audioRegs[channel- 1]->readNRx1(); 
        case 2:
            // Special case for channel 5 (global).
            if(channel == 5){
                byte otherRegs = 0;
                writeBit(otherRegs, 0, audioRegs[0]->getOn());
                writeBit(otherRegs, 1, audioRegs[1]->getOn());
                writeBit(otherRegs, 2, audioRegs[2]->getOn());
                writeBit(otherRegs, 3, audioRegs[3]->getOn());
                return ((GlobalAudio*) audioRegs[channel- 1])->readNRx2(otherRegs);
            }
            return audioRegs[channel- 1]->readNRx2(); 
        case 3:
            return audioRegs[channel- 1]->readNRx3();
        case 4:
            return audioRegs[channel- 1]->readNRx4();   
        default:
            return HIGH_IMPEDANCE;
    }
    return HIGH_IMPEDANCE;
}

byte GlobalAudio::readNRx0(){
    return masterVolume;
}
byte PulseSweep::readNRx0(){
    return pace << 4 | ((byte)direction) << 3 | invidualStep;
}
byte Wave::readNRx0(){
    return ((byte) on) << 7;
}


byte GlobalAudio::readNRx1(){
    return panning;
}
byte Pulse::readNRx1(){
    return waveDuty << 6 | initalLenTimer;
}
byte Noise::readNRx1(){
    return initalLenTimer;
}


byte GlobalAudio::readNRx2(byte otherRegs){
    writeBit(otherRegs, 7, masterOn);
    return otherRegs;
}
byte Pulse::readNRx2(){
    return initialVolume << 4 | ((byte)envDir) << 3 | sweepPace;
}
byte Wave::readNRx2(){
    return volume << 5;
}
byte Noise::readNRx2(){
    return initialVolume << 4 | ((byte)envDir) << 3 | sweepPace;
}


byte Noise::readNRx3(){
    return clockShift << 4 |  ((byte)LFSRWidth) << 3 | clockDivider;
}


byte Pulse::readNRx4(){
    return ((byte)lengthEnable) << 6;
}
byte Wave::readNRx4(){
    return ((byte)lengthEnable) << 6;
}
byte Noise::readNRx4(){
    return ((byte)lengthEnable) << 6;
}

void APU::writeReg(byte channel, byte reg, byte data){
    switch (reg){
        case 0:
            audioRegs[channel- 1]->writeNRx0(data);
            break;
        case 1:
            audioRegs[channel- 1]->writeNRx1(data);
            break;
        case 2:
            // Special case for channel 5 (global).
            if (channel == 5){
                audioRegs[0]->setOn(readBit(data, 0));
                audioRegs[1]->setOn(readBit(data, 1));
                audioRegs[2]->setOn(readBit(data, 2));
                audioRegs[3]->setOn(readBit(data, 3));
            }
            audioRegs[channel- 1]->writeNRx2(data);
            break; 
        case 3:
            audioRegs[channel- 1]->writeNRx3(data);
            break;
        case 4:
            audioRegs[channel- 1]->writeNRx4(data);
            break;  
        default:
            break;
    }
}

void GlobalAudio::writeNRx0(byte data){
    masterVolume = data;
}
void PulseSweep::writeNRx0(byte data){
    pace = (data & 0b1110000) >> 4;
    direction =  readBit(data, 3);
    invidualStep = (data & 0b111); 
}
void Wave::writeNRx0(byte data){
    on = readBit(data, 7);
}

void GlobalAudio::writeNRx1(byte data){
    panning = data;
}
void Pulse::writeNRx1(byte data){
    waveDuty = (data & 0b11000000) >> 6;
    initalLenTimer = (data & 0b00111111);
}
void Wave::writeNRx1(byte data){
    initalLenTimer = data;
}
void Noise::writeNRx1(byte data){
    initalLenTimer = data & 0b00111111;
}

void GlobalAudio::writeNRx2(byte data){
    masterOn = readBit(data, 7);
}
void Pulse::writeNRx2(byte data){
    initialVolume = (data & 0b11110000) >> 4;
    envDir =  readBit(data, 3);
    sweepPace = (data & 0b111); 

    if (envDir == 0 && initialVolume == 0) on = false;
}
void Wave::writeNRx2(byte data){
    volume = (data & 0b1100000)>>5;
}
void Noise::writeNRx2(byte data){
    initialVolume = (data & 0b11110000) >> 4;
    envDir =  readBit(data, 3);
    sweepPace = (data & 0b111); 

    if (envDir ==0 && initialVolume == 0) on = false;
}

void Pulse::writeNRx3(byte data){
    period = (period & 0xFF00) | data;
}
void Wave::writeNRx3(byte data){
    period = (period & 0xFF00) | data;
}
void Noise::writeNRx3(byte data){
    clockShift = (data & 0b11110000) >> 4; 
    LFSRWidth =  readBit(data, 3);
    clockDivider = (data & 0b111); 
}


void Pulse::writeNRx4(byte data){
    lengthEnable = readBit(data, 6);
    period = (period & 0xFF) | (((word) data & 0b111) << 8);

    bool triggerHigh = readBit(data, 7);
    if (triggerHigh){
        trigger();
    } 
}
void Wave::writeNRx4(byte data){
    lengthEnable = readBit(data, 6);
    period = (period & 0xFF) | (((word) data & 0b111) << 8);

    bool triggerHigh = readBit(data, 7);
    if (triggerHigh){
        trigger();
    } 
}
void Noise::writeNRx4(byte data){
    lengthEnable = readBit(data, 6);
    bool triggerHigh = readBit(data, 7);
    if (triggerHigh){
        trigger();
    } 
}


byte APU::readRAM(word address){
    return wavePatternRAM[address-WAVEPATTERNRAME_START];
}
void APU::writeRAM(word address, byte data){
    wavePatternRAM[address-WAVEPATTERNRAME_START] = data;
}

void ChannelController::trigger(){
    // Start playing channel from beginning of wave.
    cyclesCounter = 0;
    initalPeriod = period;
    periodDivider = initalPeriod;
    timer = initalLenTimer;
    on = true;
}
void Pulse::trigger(){
    ChannelController::trigger();
    envSweepTicks = 0;
    periodSweepTicks = 0;
    volume = initialVolume;
}
void Noise::trigger(){
    ChannelController::trigger();
    envSweepTicks = 0;
    volume = initialVolume;
    periodDivider = 0;
}


void PulseSweep::cycle(byte divAPUEvents) {
    if(!on) return;

    // Period sweep is altered at 128 Hz.
    if(readBit(divAPUEvents, 1) && pace > 0){
        periodSweepTicks++;
        if(periodSweepTicks >= pace){
            periodSweepTicks = 0;
            double offset = (initalPeriod / std::pow(2, invidualStep));
            period = direction ? (initalPeriod - offset) : (initalPeriod + offset);
            initalPeriod = period;
            // Prevent overflows / underflows.
            if(period >= CHANNEL_DIVIDER_OVERFLOW){
                on = false;
            }   
        }
    }
    Pulse::cycle(divAPUEvents);
};
void Pulse::cycle(byte divAPUEvents) {
    if(!on) return;

    // Sound length is altered at 256 Hz.
    if(readBit(divAPUEvents, 0) && lengthEnable){
        timer++;
        // Channel is automatically shut off when timer reaches 64.
        if(timer >= 64){
            on = false;
            return;
        }
    }

    // Envelope sweep is altered at 64 Hz.
    if(readBit(divAPUEvents, 2) && sweepPace > 0){
        envSweepTicks++;
        if(envSweepTicks >= sweepPace){
            envSweepTicks = 0;
            int newVolume = envDir ? ((int) volume)+1 : ((int) volume)-1;
            if (newVolume >=  MAX_CHANNEL_VOLUME){
                volume = MAX_CHANNEL_VOLUME;
            } else if (newVolume <=  0){
                volume = 0;
            } else volume = newVolume;
        }
    }

    cyclesCounter++;
    if(cyclesCounter < CYCLES_PER_PULSE_SAMPLE) return;
    cyclesCounter = 0;

    periodDivider++;
    if(periodDivider >= CHANNEL_DIVIDER_OVERFLOW){
        periodDivider = initalPeriod;
    }
};
void Wave::cycle(byte divAPUEvents) {
    if(!on) return;

    // Sound length is altered at 256 Hz.
    if(readBit(divAPUEvents, 0) && lengthEnable){
        timer++;
        // Channel is automatically shut off when timer reaches 64.
        if(timer == 64){
            on = false;
            return;
        }
    }

    cyclesCounter++;
    if(cyclesCounter < CYCLES_PER_WAVE_SAMPLE) return;
    cyclesCounter = 0;

    periodDivider++;
    if(periodDivider >= CHANNEL_DIVIDER_OVERFLOW){
        periodDivider = initalPeriod;
    }
};
void Noise::cycle(byte divAPUEvents) {
    if(!on) return;

    // Sound length is altered at 256 Hz.
    if(readBit(divAPUEvents, 0) && lengthEnable){
        timer++;
        // Channel is automatically shut off when timer reaches 64.
        if(timer >= 64){
            on = false;
            return;
        }
    }

    // Envelope sweep is altered at 64 Hz.
    if(readBit(divAPUEvents, 2) && sweepPace > 0){
        envSweepTicks++;
        if(envSweepTicks >= sweepPace){
            envSweepTicks = 0;
            int newVolume = envDir ? ((int) volume)+1 : ((int) volume)-1;
            if (newVolume >=  MAX_CHANNEL_VOLUME){
                volume = MAX_CHANNEL_VOLUME;
            } else if (newVolume <=  0){
                volume = 0;
            } else volume = newVolume;
        }
    }

    // Determine the number of cycles before we cycle the LFSR.
    double dividerAmount = clockDivider == 0 ? 0.5 : clockDivider;
    double shiftAmount = std::pow(2, clockShift);
    cycles cyclesRequired = CYCLES_PER_NOISE_SAMPLE * dividerAmount * shiftAmount;
    
    cyclesCounter++;
    if(cyclesCounter < cyclesRequired) return;
    cyclesCounter = 0;

    // Perform the linear feedback shift register action.
    byte lowerNibble = LFSR & 0xFF;
    byte upperNibble = (LFSR & 0xFF00) >> 8;
    bool nextBit = readBit(lowerNibble, 0) == readBit(lowerNibble, 1); 
    writeBit(upperNibble, 7, nextBit);
    if(LFSRWidth) writeBit(lowerNibble, 7, nextBit);

    shiftedOutBit = readBit(lowerNibble, 0);
    LFSR = (((word)upperNibble ) << 8 | lowerNibble) >> 1;
};

AudioChannelData Pulse::sample() {
    if(!on) return NO_AUDIO;

    double position = (double) (periodDivider - initalPeriod) / (CHANNEL_DIVIDER_OVERFLOW - initalPeriod);

    AudioChannelData data;
    data.left = position > DUTY_THRESHOLDS[waveDuty] ? 1 : 0;
    data.right = position > DUTY_THRESHOLDS[waveDuty] ? 1 : 0;

    // Apply volume.
    data.left = data.left * ((double) volume) / MAX_CHANNEL_VOLUME;
    data.right = data.right * ((double) volume) / MAX_CHANNEL_VOLUME;

    return data;
};
AudioChannelData Wave::sample(byte* waveRam) {
    if(!on) return NO_AUDIO;

    // Generates a value between 0 and 32.
    double phase = (double) (periodDivider - initalPeriod) / (CHANNEL_DIVIDER_OVERFLOW - initalPeriod) * WAVE_SAMPLES_PER_BYTE * WAVE_RAM_SIZE;
    // Convert that into a memory access starting at 1 then looping to 0 after reaching 31.
    int position = ((int) std::round(phase) + 1) % (WAVE_SAMPLES_PER_BYTE * WAVE_RAM_SIZE);
    bool high = position % 2 == 0;
    byte valueInRam = waveRam[position / 2];

    // Turn the data into a audio sample.
    double audioLevel = 0;
    if (high){
        audioLevel = ((double)((valueInRam & 0xF0) >> 4)) / MAX_CHANNEL_VOLUME; 
    } else {
        audioLevel = ((double)(valueInRam & 0xF)) / MAX_CHANNEL_VOLUME; 
    }
    switch(volume){
        // Mute (No sound).
        case 0:
            audioLevel = 0;
            break;
        // 100% volume.
        case 1:
            break;
        // 50% volume.
        case 2:
            audioLevel = audioLevel * 0.5;
            break;
        // 25% volume.     
        case 3:
            audioLevel = audioLevel * 0.25;
            break;
        default:
            break;
    }

    // Package the sample.
    AudioChannelData data;
    data.left = audioLevel;
    data.right = audioLevel;

    return data;
};
AudioChannelData Noise::sample() {
    if(!on) return NO_AUDIO;

    AudioChannelData data;
    data.left = shiftedOutBit ? 1 : 0;
    data.right = shiftedOutBit ? 1 : 0;

    // Apply volume. 
    data.left = data.left * ((double) volume) / MAX_CHANNEL_VOLUME;
    data.right = data.right * ((double) volume) / MAX_CHANNEL_VOLUME;

    return data;
};

void APU::cycle(byte divAPUEvents){
    if(!globalChannel.masterOn) return;

    // Try and stop the writes from going to fast.
    int bufferReadDist = (writeLocation - readLocation);
    if(bufferReadDist > AUDIO_FRAMES_PER_BUFFER*2) return;
    if(bufferReadDist < 0 && bufferReadDist + AUDIO_BUFFER_SIZE > AUDIO_FRAMES_PER_BUFFER*2) return;

    // Cycle the APU.
    channel1.cycle(divAPUEvents); 
    channel2.cycle(divAPUEvents);    
    channel3.cycle(divAPUEvents);      
    channel4.cycle(divAPUEvents);    
    cyclesCounter++;
    if(cyclesCounter < CYCLES_PER_AUDIO_FRAME) return;
    // Enough time has elapsed. Do some audio processing.
    cyclesCounter = 0;

    // Sample each of the channels.
    AudioChannelData channel1Sample = channel1.sample();
    AudioChannelData channel2Sample = channel2.sample();
    AudioChannelData channel3Sample = channel3.sample(wavePatternRAM);
    AudioChannelData channel4Sample = channel4.sample();

    // Mix samples.
    audioData[writeLocation].right = 0;
    audioData[writeLocation].right += readBit(globalChannel.panning, 0) ?  channel1Sample.right : 0;
    audioData[writeLocation].right += readBit(globalChannel.panning, 1) ?  channel2Sample.right : 0;
    audioData[writeLocation].right += readBit(globalChannel.panning, 2) ?  channel3Sample.right : 0;
    audioData[writeLocation].right += readBit(globalChannel.panning, 3) ?  channel4Sample.right : 0;
    audioData[writeLocation].right = audioData[writeLocation].right / NUMBER_OF_CHANNELS;
    audioData[writeLocation].left = 0;
    audioData[writeLocation].left += readBit(globalChannel.panning, 4) ?  channel1Sample.left : 0;
    audioData[writeLocation].left += readBit(globalChannel.panning, 5) ?  channel2Sample.left : 0;
    audioData[writeLocation].left += readBit(globalChannel.panning, 6) ?  channel3Sample.left : 0;
    audioData[writeLocation].left += readBit(globalChannel.panning, 7) ?  channel4Sample.left : 0;
    audioData[writeLocation].left = audioData[writeLocation].left / NUMBER_OF_CHANNELS;
    
    // Clip the audio data.
    if(audioData[writeLocation].left > 1.0) audioData[writeLocation].left = 1.0;
    else if(audioData[writeLocation].left < 0) audioData[writeLocation].left = 0;
    if(audioData[writeLocation].right > 1.0) audioData[writeLocation].right = 1.0;
    else if(audioData[writeLocation].right < 0) audioData[writeLocation].right = 0;

    // Apply master volume.
    byte masterVolLeft = (((globalChannel.masterVolume & 0b1110000) >> 4) + 1) / 8;
    byte masterVolRight = ((globalChannel.masterVolume & 0b111) + 1) / 8;

    audioData[writeLocation].left = audioData[writeLocation].left * masterVolLeft;
    audioData[writeLocation].right = audioData[writeLocation].right * masterVolRight;
    
    // Increment the write position in the audio buffer.
    writeLocation++;
    if(writeLocation == AUDIO_BUFFER_SIZE) writeLocation = 0;
}

AudioChannelData* APU::fetchAudioData(){
    uint32_t offset = readLocation;
    // Increment the read postion for next time.
    readLocation++;
    if(readLocation == AUDIO_BUFFER_SIZE) readLocation = 0;

    // Return the currently selected audio frame.
    return audioData+offset;
}

void ChannelController::saveToState(byte*& writeBuffer){
    std::memcpy(writeBuffer, &cyclesCounter, sizeof(cycles)); writeBuffer+=sizeof(cycles);

    std::memcpy(writeBuffer, &on, sizeof(bool)); writeBuffer+=sizeof(bool);
    std::memcpy(writeBuffer, &lengthEnable, sizeof(bool)); writeBuffer+=sizeof(bool);

    std::memcpy(writeBuffer, &timer, sizeof(byte)); writeBuffer+=sizeof(byte);
    std::memcpy(writeBuffer, &volume, sizeof(byte)); writeBuffer+=sizeof(byte);
    std::memcpy(writeBuffer, &initalLenTimer, sizeof(byte)); writeBuffer+=sizeof(byte);

    std::memcpy(writeBuffer, &periodDivider, sizeof(word)); writeBuffer+=sizeof(word);
    std::memcpy(writeBuffer, &period, sizeof(word)); writeBuffer+=sizeof(word);
    std::memcpy(writeBuffer, &initalPeriod, sizeof(word)); writeBuffer+=sizeof(word);
}
void GlobalAudio::saveToState(byte*& writeBuffer){
    std::memcpy(writeBuffer, &masterOn, sizeof(bool)); writeBuffer+=sizeof(bool);
    std::memcpy(writeBuffer, &panning, sizeof(byte)); writeBuffer+=sizeof(byte);
    std::memcpy(writeBuffer, &masterVolume, sizeof(byte)); writeBuffer+=sizeof(byte);
}
void Pulse::saveToState(byte*& writeBuffer){
    ChannelController::saveToState(writeBuffer);

    std::memcpy(writeBuffer, &periodSweepTicks, sizeof(cycles)); writeBuffer+=sizeof(cycles);
    std::memcpy(writeBuffer, &envSweepTicks, sizeof(cycles)); writeBuffer+=sizeof(cycles);

    std::memcpy(writeBuffer, &direction, sizeof(bool)); writeBuffer+=sizeof(bool);
    std::memcpy(writeBuffer, &envDir, sizeof(bool)); writeBuffer+=sizeof(bool);

    std::memcpy(writeBuffer, &pace, sizeof(byte)); writeBuffer+=sizeof(byte);
    std::memcpy(writeBuffer, &sweepPace, sizeof(byte)); writeBuffer+=sizeof(byte);
    std::memcpy(writeBuffer, &envDir, sizeof(byte)); writeBuffer+=sizeof(byte);
    std::memcpy(writeBuffer, &waveDuty, sizeof(byte)); writeBuffer+=sizeof(byte);
    std::memcpy(writeBuffer, &initalLenTimer, sizeof(byte)); writeBuffer+=sizeof(byte);
    std::memcpy(writeBuffer, &initialVolume, sizeof(byte)); writeBuffer+=sizeof(byte);
}
void Noise::saveToState(byte*& writeBuffer){
    ChannelController::saveToState(writeBuffer);

    std::memcpy(writeBuffer, &envSweepTicks, sizeof(cycles)); writeBuffer+=sizeof(cycles);

    std::memcpy(writeBuffer, &LFSRWidth, sizeof(bool)); writeBuffer+=sizeof(bool);
    std::memcpy(writeBuffer, &envDir, sizeof(bool)); writeBuffer+=sizeof(bool);
    std::memcpy(writeBuffer, &shiftedOutBit, sizeof(bool)); writeBuffer+=sizeof(bool);

    std::memcpy(writeBuffer, &clockShift, sizeof(byte)); writeBuffer+=sizeof(byte);
    std::memcpy(writeBuffer, &sweepPace, sizeof(byte)); writeBuffer+=sizeof(byte);
    std::memcpy(writeBuffer, &envDir, sizeof(byte)); writeBuffer+=sizeof(byte);
    std::memcpy(writeBuffer, &clockDivider, sizeof(byte)); writeBuffer+=sizeof(byte);
    std::memcpy(writeBuffer, &initalLenTimer, sizeof(byte)); writeBuffer+=sizeof(byte);
    std::memcpy(writeBuffer, &initialVolume, sizeof(byte)); writeBuffer+=sizeof(byte);

    std::memcpy(writeBuffer, &LFSR, sizeof(word)); writeBuffer+=sizeof(word);
}
void APU::saveToState(std::ofstream & stateFile){
    int bytesToWrite = sizeof(cycles) + sizeof(byte)*WAVE_RAM_SIZE; // APU Size.
    bytesToWrite += (sizeof(cycles) + sizeof(bool)*2 + sizeof(byte)*3 + sizeof(word)*3)*NUMBER_OF_CHANNELS; // Base audio reg size.
    bytesToWrite += sizeof(bool) + sizeof(byte)*2; // Global reg size.
    bytesToWrite += (sizeof(bool)*2 + sizeof(byte)*6 + sizeof(cycles)*2)*2; // Channel 1 + 2.
    bytesToWrite += (sizeof(bool)*3 + sizeof(byte)*6 + sizeof(cycles) + sizeof(word)); // Channel 4.

    byte* writeBuffer = new byte[
        bytesToWrite
    ];
    byte* writeBufferStart = writeBuffer;

    std::memcpy(writeBuffer, &cyclesCounter, sizeof(cycles)); writeBuffer+=sizeof(cycles);
    std::memcpy(writeBuffer, wavePatternRAM, sizeof(byte)*WAVE_RAM_SIZE); writeBuffer+=sizeof(byte)*WAVE_RAM_SIZE;

    for (int i = 0; i<NUMBER_OF_CHANNELS+1; i++) audioRegs[i]->saveToState(writeBuffer);

    // Write out the data.
    stateFile.write((char*)writeBufferStart, bytesToWrite);
    delete[] writeBufferStart;
}

void ChannelController::loadFromState(byte*& readBuffer){
    std::memcpy(&cyclesCounter, readBuffer, sizeof(cycles)); readBuffer+=sizeof(cycles);

    std::memcpy(&on, readBuffer, sizeof(bool)); readBuffer+=sizeof(bool);
    std::memcpy(&lengthEnable, readBuffer, sizeof(bool)); readBuffer+=sizeof(bool);

    std::memcpy(&timer, readBuffer, sizeof(byte)); readBuffer+=sizeof(byte);
    std::memcpy(&volume, readBuffer, sizeof(byte)); readBuffer+=sizeof(byte);
    std::memcpy(&initalLenTimer, readBuffer, sizeof(byte)); readBuffer+=sizeof(byte);

    std::memcpy(&periodDivider, readBuffer, sizeof(word)); readBuffer+=sizeof(word);
    std::memcpy(&period, readBuffer, sizeof(word)); readBuffer+=sizeof(word);
    std::memcpy(&initalPeriod, readBuffer, sizeof(word)); readBuffer+=sizeof(word);
}
void GlobalAudio::loadFromState(byte*& readBuffer){
    std::memcpy(&masterOn, readBuffer, sizeof(bool)); readBuffer+=sizeof(bool);
    std::memcpy(&panning, readBuffer, sizeof(byte)); readBuffer+=sizeof(byte);
    std::memcpy(&masterVolume, readBuffer, sizeof(byte)); readBuffer+=sizeof(byte);
}
void Pulse::loadFromState(byte*& readBuffer){
    ChannelController::loadFromState(readBuffer);

    std::memcpy(&periodSweepTicks, readBuffer, sizeof(cycles)); readBuffer+=sizeof(cycles);
    std::memcpy(&envSweepTicks, readBuffer, sizeof(cycles)); readBuffer+=sizeof(cycles);

    std::memcpy(&direction, readBuffer, sizeof(bool)); readBuffer+=sizeof(bool);
    std::memcpy(&envDir, readBuffer, sizeof(bool)); readBuffer+=sizeof(bool);

    std::memcpy(&pace, readBuffer, sizeof(byte)); readBuffer+=sizeof(byte);
    std::memcpy(&sweepPace, readBuffer, sizeof(byte)); readBuffer+=sizeof(byte);
    std::memcpy(&envDir, readBuffer, sizeof(byte)); readBuffer+=sizeof(byte);
    std::memcpy(&waveDuty, readBuffer, sizeof(byte)); readBuffer+=sizeof(byte);
    std::memcpy(&initalLenTimer, readBuffer, sizeof(byte)); readBuffer+=sizeof(byte);
    std::memcpy(&initialVolume, readBuffer, sizeof(byte)); readBuffer+=sizeof(byte);
}
void Noise::loadFromState(byte*& readBuffer){
    ChannelController::loadFromState(readBuffer);

    std::memcpy(&envSweepTicks, readBuffer, sizeof(cycles)); readBuffer+=sizeof(cycles);

    std::memcpy(&LFSRWidth, readBuffer, sizeof(bool)); readBuffer+=sizeof(bool);
    std::memcpy(&envDir, readBuffer, sizeof(bool)); readBuffer+=sizeof(bool);
    std::memcpy(&shiftedOutBit, readBuffer, sizeof(bool)); readBuffer+=sizeof(bool);

    std::memcpy(&clockShift, readBuffer, sizeof(byte)); readBuffer+=sizeof(byte);
    std::memcpy(&sweepPace, readBuffer, sizeof(byte)); readBuffer+=sizeof(byte);
    std::memcpy(&envDir, readBuffer, sizeof(byte)); readBuffer+=sizeof(byte);
    std::memcpy(&clockDivider, readBuffer, sizeof(byte)); readBuffer+=sizeof(byte);
    std::memcpy(&initalLenTimer, readBuffer, sizeof(byte)); readBuffer+=sizeof(byte);
    std::memcpy(&initialVolume, readBuffer, sizeof(byte)); readBuffer+=sizeof(byte);

    std::memcpy(&LFSR, readBuffer, sizeof(word)); readBuffer+=sizeof(word);
}
void APU::loadFromState(std::ifstream & stateFile){
    int bytesToRead = sizeof(cycles) + sizeof(byte)*WAVE_RAM_SIZE; // APU Size.
    bytesToRead += (sizeof(cycles) + sizeof(bool)*2 + sizeof(byte)*3 + sizeof(word)*3)*NUMBER_OF_CHANNELS; // Base audio reg size.
    bytesToRead += sizeof(bool) + sizeof(byte)*2; // Global reg size.
    bytesToRead += (sizeof(bool)*2 + sizeof(byte)*6 + sizeof(cycles)*2)*2; // Channel 1 + 2.
    bytesToRead += (sizeof(bool)*3 + sizeof(byte)*6 + sizeof(cycles) + sizeof(word)); // Channel 4.

    byte* readBuffer = new byte[
        bytesToRead
    ];
    byte* readBufferStart = readBuffer;
    stateFile.read((char*)readBufferStart, bytesToRead);

    std::memcpy(&cyclesCounter, readBuffer, sizeof(cycles)); readBuffer+=sizeof(cycles);
    std::memcpy(wavePatternRAM, readBuffer, sizeof(byte)*WAVE_RAM_SIZE); readBuffer+=sizeof(byte)*WAVE_RAM_SIZE;

    for (int i = 0; i<NUMBER_OF_CHANNELS+1; i++) audioRegs[i]->loadFromState(readBuffer);

    delete[] readBufferStart;
}