#include "wavegen.h"
#include "wave.h"
#include "timer.h"
#include "dac.h"
#include "platform.h"
#include <stdint.h>
#include <math.h>

#define DEFAULT_INTERRUPT_US 10u
#define TABLE_SIZE 256u

static volatile wavetype currentWaveform = IDLE;
static volatile float currentFrequency = 440.0f;

/* Full-scale DAC range */
static const uint32_t MAX_DAC = (uint32_t)DAC_MASK;

/* Waveform lookup tables */
static uint16_t table_sine[TABLE_SIZE];
static uint16_t table_triangle[TABLE_SIZE];
static uint16_t table_saw[TABLE_SIZE];
static uint16_t table_square[TABLE_SIZE];

/* DDS variables */
static volatile float phase = 0.0f;
static volatile float step_size = 0.0f;
static uint32_t interrupt_rate_us = DEFAULT_INTERRUPT_US;

static void build_tables(void) {
    for (uint32_t i = 0; i < TABLE_SIZE; ++i) {
        /* sine: [-1,1] → [0, MAX_DAC] */
        float s = sinf((2.0f * (float)M_PI * (float)i) / (float)TABLE_SIZE);
        table_sine[i] = (uint16_t)((s * 0.5f + 0.5f) * (float)MAX_DAC + 0.5f);

        /* triangle: rise then fall */
        if (i < TABLE_SIZE / 2) {
            table_triangle[i] = (uint16_t)(((uint64_t)i * 2ull * MAX_DAC) / TABLE_SIZE);
        } else {
            table_triangle[i] = (uint16_t)((((uint64_t)(TABLE_SIZE - i)) * 2ull * MAX_DAC) / TABLE_SIZE);
        }

        /* sawtooth: 0 → MAX linearly */
        if (TABLE_SIZE > 1) {
            table_saw[i] = (uint16_t)((((uint64_t)i) * MAX_DAC) / (TABLE_SIZE - 1));
        } else {
            table_saw[i] = 0;
        }

        /* square: first half low, second half high */
        table_square[i] = (i < (TABLE_SIZE / 2)) ? 0 : (uint16_t)MAX_DAC;
    }
}

/* -------------------- Timer callback -------------------- */
static void wavegen_update(void) {
    uint32_t idx = (uint32_t)phase;
    uint32_t val = 0;

    /* Wrap index safely */
    if (idx >= TABLE_SIZE)
        idx = TABLE_SIZE - 1;

    switch (currentWaveform) {
        case SQUARE:
            val = table_square[idx];
            break;
        case TRIANGLE:
            val = table_triangle[idx];
            break;
        case SAWTOOTH:
            val = table_saw[idx];
            break;
        case SINE:
            val = table_sine[idx];
            break;
        default:
            val = 0;
            break;
    }

    if (val > MAX_DAC) val = MAX_DAC;
    dac_set((int)val);

    /* Advance phase by fractional step size */
    phase += step_size;
    if (phase >= (float)TABLE_SIZE)
        phase -= (float)TABLE_SIZE;
}

void wavegen_init(void) {
    build_tables();
    timer_init(interrupt_rate_us);
    timer_set_callback(wavegen_update);
    timer_enable();
}

void wavegen_setWaveform(wavetype type) {
    currentWaveform = type;
    phase = 0.0f;
}


uint32_t wavegen_setFrequency(float frequency) {
    if (frequency <= 0.0f) {
        step_size = 0.0f;
        currentFrequency = 0.0f;
        return 0;
    }

    currentFrequency = frequency;

    /* Compute step size for DDS */
    float update_rate = 1e6f / (float)interrupt_rate_us;
    step_size = ((float)TABLE_SIZE * frequency) / update_rate;

    /* Prevent overflow or NaN */
    if (step_size < 0.0001f) step_size = 0.0001f;
    if (step_size > (float)TABLE_SIZE) step_size = (float)TABLE_SIZE;

    phase = 0.0f;
    return (uint32_t)(step_size * 1000.0f);
}
