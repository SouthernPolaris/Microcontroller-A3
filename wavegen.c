#include "wavegen.h"
#include "wave.h"
#include <timer.h>
#include <dac.h>
#include <platform.h>
#include <stdint.h>

/*
 * Wave generator driven by a timer callback. The timer is expected to fire
 * at `interrupt_rate_us` microsecond intervals (default 10 us). For a
 * requested frequency `f`, we compute how many timer ticks make up one
 * waveform period (samples_per_period). On every timer tick we advance the
 * sample index and compute the appropriate DAC value scaled to the full
 * DAC range (DAC_BITS from platform.h).
 */

#define DEFAULT_INTERRUPT_US 10u

/* Use the platform DAC mask to obtain full DAC range (e.g. 10-bit). */
#define MAX_DAC_VALUE (DAC_MASK)

static wavetype currentWaveform = IDLE;
static float currentFrequency = 440.0f;

static uint32_t sample_idx = 0;
static uint32_t samples_per_period = 1; /* number of timer ticks per waveform period */
static uint32_t interrupt_rate_us = DEFAULT_INTERRUPT_US; /* microseconds */

static void wavegen_update(void) {
    uint32_t steps = samples_per_period;
    uint32_t idx = sample_idx;
    uint32_t val = 0;

    if (steps < 2) {
        /* fallback to at least 2 steps to avoid div-by-zero and allow a square */
        steps = 2;
    }

    switch (currentWaveform) {
        case SQUARE:
            val = (idx < (steps / 2)) ? 0 : MAX_DAC_VALUE;
            break;
        case TRIANGLE: {
            /* Use (steps-1) as denominator so endpoints map exactly to 0 to MAX */
            if (steps <= 1) {
                val = 0;
            } else {
                uint32_t denom = steps - 1;
                if (idx <= (denom / 2)) {
                    /* rising edge */
                    val = (uint32_t)((((uint64_t)idx) * 2 * MAX_DAC_VALUE) / denom);
                } else {
                    /* falling edge */
                    val = (uint32_t)((((uint64_t)(denom - idx)) * 2 * MAX_DAC_VALUE) / denom);
                }
            }
        }
            break;
        case SAWTOOTH:
            /* Map 0 to (steps-1) to 0 to MAX_DAC_VALUE so last sample reaches MAX */
            if (steps <= 1) {
                val = 0;
            } else {
                val = (uint32_t)((((uint64_t)idx) * MAX_DAC_VALUE) / (steps - 1));
            }
            break;
        default:
            val = 0;
            break;
    }

    /* clamp to DAC range and output */
    if (val > MAX_DAC_VALUE) val = MAX_DAC_VALUE;
    dac_set((int)val);

    /* advance index */
    sample_idx = (sample_idx + 1) % samples_per_period;
}

void wavegen_init(void) {
    /* initialise timer using configured interrupt period and enable callback */
    timer_init(interrupt_rate_us);
    timer_set_callback(wavegen_update);
    timer_enable();
}

void wavegen_setWaveform(wavetype type) {
    currentWaveform = type;
    sample_idx = 0; /* Reset sample index when changing waveform */
}

void wavegen_setFrequency(float frequency) {
    currentFrequency = frequency;
    if (currentFrequency <= 0.0f) {
        samples_per_period = 1;
        return;
    }

    /* period in microseconds */
    float period_us = 1e6f / currentFrequency;

    /* compute how many timer ticks (interrupts) per waveform period */
    uint32_t ticks = (uint32_t)(period_us / (float)interrupt_rate_us + 0.5f);

    if (ticks < 1) ticks = 1;
    samples_per_period = ticks;
    sample_idx = 0;
}