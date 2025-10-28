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

/* Use platform DAC mask for full-scale */
static const uint32_t MAX_DAC = (uint32_t)DAC_MASK;

static uint16_t table_sine[TABLE_SIZE];
static uint16_t table_triangle[TABLE_SIZE];
static uint16_t table_saw[TABLE_SIZE];
static uint16_t table_square[TABLE_SIZE];

static volatile uint32_t sample_idx = 0;           /* advances every tick */
static volatile uint32_t samples_per_period = 1;   /* ticks per full cycle */
static uint32_t interrupt_rate_us = DEFAULT_INTERRUPT_US;

/* Build lookup tables scaled to DAC range */
static void build_tables(void) {
    for (uint32_t i = 0; i < TABLE_SIZE; ++i) {
        /* sine: [-1,1] -> [0, MAX_DAC] */
        float s = sinf((2.0f * (float)M_PI * (float)i) / (float)TABLE_SIZE);
        table_sine[i] = (uint16_t)((s * 0.5f + 0.5f) * (float)MAX_DAC + 0.5f);

        /* triangle: rise then fall */
        if (i < TABLE_SIZE / 2) {
            table_triangle[i] = (uint16_t)(((uint64_t)i * 2ull * MAX_DAC) / TABLE_SIZE);
        } else {
            table_triangle[i] = (uint16_t)((((uint64_t)(TABLE_SIZE - i)) * 2ull * MAX_DAC) / TABLE_SIZE);
        }

        /* sawtooth: linear 0 .. MAX, ensure last element reaches MAX */
        if (TABLE_SIZE > 1) {
            table_saw[i] = (uint16_t)((((uint64_t)i) * MAX_DAC) / (TABLE_SIZE - 1));
        } else {
            table_saw[i] = 0;
        }

        /* square: first half low, second half high */
        table_square[i] = (i < (TABLE_SIZE / 2)) ? 0 : (uint16_t)MAX_DAC;
    }
}

static void wavegen_update(void) {
    uint32_t steps = samples_per_period;
    uint32_t idx = sample_idx;
    uint32_t table_idx;
    uint32_t val = 0;

    if (steps < 1) steps = 1;

    /* Map sample index (0..steps-1) to table index (0..TABLE_SIZE-1) */
    if (steps == 1) {
        table_idx = 0;
    } else {
        table_idx = (uint32_t)((((uint64_t)idx) * TABLE_SIZE) / steps);
        if (table_idx >= TABLE_SIZE) table_idx = TABLE_SIZE - 1;
    }

    switch (currentWaveform) {
        case SQUARE:
            val = table_square[table_idx];
            break;
        case TRIANGLE:
            val = table_triangle[table_idx];
            break;
        case SAWTOOTH:
            val = table_saw[table_idx];
            break;
        default:
            /* IDLE or unknown -> output 0 */
            val = 0;
            break;
    }

    if (val > MAX_DAC) val = MAX_DAC;
    dac_set((int)val);

    /* advance index */
    sample_idx = (sample_idx + 1) % samples_per_period;
}

void wavegen_init(void) {
    build_tables();
    timer_init(interrupt_rate_us);
    timer_set_callback(wavegen_update);
    timer_enable();
}

void wavegen_setWaveform(wavetype type) {
    currentWaveform = type;
    sample_idx = 0;
}

uint32_t wavegen_setFrequency(float frequency) {
    float period_us;
    uint32_t ticks;

    if (frequency <= 0.0f) {
        samples_per_period = 1;
        currentFrequency = 0.0f;
        return samples_per_period;
    }

    currentFrequency = frequency;
    period_us = 1e6f / currentFrequency;
    ticks = (uint32_t)(period_us / (float)interrupt_rate_us + 0.5f);
    if (ticks < 1) ticks = 1;
    samples_per_period = ticks;
    sample_idx = 0;
    return samples_per_period;
}