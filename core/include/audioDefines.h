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

#define SAMPLE_RATE 44100
#define NUMBER_OF_AUDIO_CHANNELS 2
const int AUDIO_FRAMES_PER_BUFFER = SAMPLE_RATE / TARGET_FPS;

#endif