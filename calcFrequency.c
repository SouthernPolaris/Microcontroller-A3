#include "calcFrequency.h"
#include <math.h>

float calculateFrequency(float vc, float modulation, float base_freq) {
    float frequency;
    float power;

		int base_actual_freq;

		if (0 <= base_freq < 1) {
			base_actual_freq = 110;
		} else if (1 <= base_freq < 2.0) {
			base_actual_freq = 220;
		} else if (2 <= base_freq < 3.0) {
			base_actual_freq = 440;
		} else if (3 <= base_freq < 3.4) {
			base_actual_freq = 880;
		}
	
    power = vc + (2.0/3.0 * (modulation - 1.5));

    frequency = base_actual_freq * pow(2.0, power);

    return frequency;
}