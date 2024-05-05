#ifndef AUDIODEFINES_H
#define AUDIODEFINES_H

#include "defines.h"

/**
 * A header file for audio defines/enums.
 */
typedef struct {
    float left;
    float right;
} AudioChannelData;

const int WAVE_RAM_SIZE = WAVEPATTERNRAME_END - WAVEPATTERNRAME_START + 1;

#define SAMPLE_RATE 44100
#define NUMBER_OF_AUDIO_CHANNELS 2
// App requests 1 audio buffer per application frame.
// Frame here refers to a sample NOT a application frame.
const int AUDIO_FRAMES_PER_BUFFER = SAMPLE_RATE / TARGET_FPS; 

const int CYCLES_PER_AUDIO_FRAME = CYCLES_PER_FRAME / AUDIO_FRAMES_PER_BUFFER;

#define BUFFER_SCALE 10
#define AUDIO_BUFFER_SIZE AUDIO_FRAMES_PER_BUFFER*BUFFER_SCALE

#define CHANNEL_DIVIDER_OVERFLOW 0x800

#define PULSE_SAMPLE_LENGTH 8
#define PULSE_CHANNEL_DIVIDER_SPEED 4

#define WAVE_SAMPLE_LENGTH 32
#define WAVE_CHANNEL_DIVIDER_SPEED 2
#define WAVE_SAMPLES_PER_BYTE 2


// This is hacky but it makes the sound better :/.
const int CYCLES_PER_PULSE_SAMPLE = PULSE_SAMPLE_LENGTH*PULSE_CHANNEL_DIVIDER_SPEED;
const int CYCLES_PER_WAVE_SAMPLE = WAVE_SAMPLE_LENGTH*WAVE_CHANNEL_DIVIDER_SPEED;
const int CYCLES_PER_NOISE_SAMPLE = 8;

#define MAX_CHANNEL_VOLUME 0b1111

const double DUTY_THRESHOLDS[] = {
    0.125,
    0.25,
    0.5,
    0.75
};

#endif