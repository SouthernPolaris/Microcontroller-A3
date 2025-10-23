#include "wavegen.h"
#include "wave.h"
#include <timer.h>
#include <dac.h>
#include <stdint.h>

#define NUM_STEPS 64
#define MAX_DAC_VALUE 0xFF

static wavetype currentWaveform = IDLE;
static float currentFrequency = 440.0f;

static uint32_t sample_idx = 0;
static uint32_t samples_per_period = NUM_STEPS;
static uint32_t interrupt_rate_us = 10; // 10 microseconds

static void wavegen_update(void) {
    uint16_t val = 0;

    switch (currentWaveform) {
        case SQUARE:
            val = (sample_idx < NUM_STEPS / 2) ? 0 : MAX_DAC_VALUE;
            break;
        case TRIANGLE:
            val = (sample_idx < NUM_STEPS / 2)
                    ? (sample_idx * 2 * MAX_DAC_VALUE / NUM_STEPS)
                    : (2 * MAX_DAC_VALUE - sample_idx * 2 * MAX_DAC_VALUE / NUM_STEPS);
            break;
        case SAWTOOTH:
            val = (sample_idx * MAX_DAC_VALUE) / NUM_STEPS;
            break;
        default:
            val = 0;
            break;
    }

    dac_set(val);

    sample_idx = (sample_idx + 1) % samples_per_period;
}

void wavegen_init(void) {
    timer_init(interrupt_rate_us);
    timer_set_callback(wavegen_update);
    timer_enable();
}

void wavegen_setWaveform(wavetype type) {
    currentWaveform = type;
    sample_idx = 0; // Reset sample index when changing waveform
}

void wavegen_setFrequency(float frequency) {
    currentFrequency = frequency;
    float period = 1e6f / currentFrequency;
    samples_per_period = (uint32_t)(period / interrupt_rate_us);

    if (samples_per_period < 1) {
        samples_per_period = 1; // Ensure at least one sample per period
    }
}