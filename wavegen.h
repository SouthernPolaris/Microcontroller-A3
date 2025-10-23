#ifndef WAVEGEN_H
#define WAVEGEN_H

#include "wave.h"

void wavegen_init(void);
void wavegen_setWaveform(wavetype type);
void wavegen_setFrequency(float frequency);

#endif // WAVEGEN_H