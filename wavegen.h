#ifndef WAVEGEN_H
#define WAVEGEN_H

#include "wave.h"
#include <stdint.h>

/// @brief Initialise waveform generator
void wavegen_init(void);

/// @brief Set the current waveform type
/// @param type Waveform type to set
void wavegen_setWaveform(wavetype type);

/// @brief Set the frequency of the waveform
/// @param frequency Frequency to set (in Hz)
/// @return Number of samples per period
uint32_t wavegen_setFrequency(float frequency);

#endif // WAVEGEN_H

