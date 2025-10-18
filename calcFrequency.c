#include "calcFrequency.h"
#include <math.h>

float calculateFrequency(float vc, float modulation, float base_freq) {
    float frequency;
    float power;

    power = vc + (2.0/3.0 * (modulation - 1.5));

    frequency = base_freq * pow(2.0, power);

    return frequency;
}