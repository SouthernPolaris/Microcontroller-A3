#ifndef CALCFREQUENCY_H
#define CALCFREQUENCY_H

/// @brief Calculate frequency based on control voltage, modulation, and base frequency
/// @param vc Control voltage input (in Volts)
/// @param modulation Modulation input (in Volts)
/// @param base_freq Base frequency selector (in Volts)
/// @return Calculated frequency (in Hz)
float calculateFrequency(float vc, float modulation, float base_freq);

#endif // CALCFREQUENCY_H
