#include "calcFrequency.h"
#include <math.h>

float calculateFrequency(float vc, float modulation, float base_freq) {
    float frequency;
    float power;

		int base_actual_freq;

		// TODO: Maybe look into direct mapping to a range of frequencies instead of if-else	
		if (0.0f <= base_freq && base_freq < 1.0f) {
			base_actual_freq = 110;
		} else if (1.0f <= base_freq && base_freq < 2.0f) {
			base_actual_freq = 120;
		} else if (2.0f <= base_freq && base_freq < 3.0f) {
			base_actual_freq = 130;
		} else if (3.0f <= base_freq && base_freq < 3.4f) {
			base_actual_freq = 140;
		} else {
			base_actual_freq = 110; // Default to 110Hz if out of range
		}
	
    power = vc + (2.0f/3.0f * (modulation - 1.5f));

    frequency = base_actual_freq * pow(2.0, power);

    return frequency;
}
