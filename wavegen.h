#ifndef WAVEGEN_H
#define WAVEGEN_H

#include "wave.h"
#include <stdint.h>

void wavegen_init(void);
void wavegen_setWaveform(wavetype type);
uint32_t wavegen_setFrequency(float frequency);

#endif // WAVEGEN_H

