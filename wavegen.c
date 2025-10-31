#include "wavegen.h"
#include "wave.h"
#include "timer.h"
#include "dac.h"
#include "platform.h"
#include <stdint.h>

#define DEFAULT_INTERRUPT_US 10u

/* Use the platform DAC mask to obtain full DAC range (e.g. 10-bit). */
#define MAX_DAC_VALUE (1023)

// Variables to hold current waveform and frequency
volatile wavetype currentWaveform = IDLE;
float currentFrequency = 440.0f;
static int val;

// Sample index and samples per period
uint32_t sample_idx = 0;
volatile uint32_t samples_per_period = 1; /* number of timer ticks per waveform period */

uint32_t STEPS_PER_WAVEFORM = 50;
uint32_t interrupt_rate_us = DEFAULT_INTERRUPT_US; /* microseconds */

// Update waveform output based on current settings
static void wavegen_update(void) {
    volatile uint32_t steps;

	volatile int delay_val;

    steps = samples_per_period;

	// Avoid division by zero
	if (steps < 2) steps = 2;

	// Generate waveform samples
	wavegen_step();
	delay_val = (steps * interrupt_rate_us) / STEPS_PER_WAVEFORM;
	
	// Avoid too small delay values
	if (delay_val < 2) {
		delay_val = 2;
	}

	timer_set_match_us(delay_val);
}

/**
 * Set timer match register based on microsecond delay
 * \param us Delay in microseconds
 */
static inline void timer_set_match_us(uint32_t us) {
	uint32_t pclk = SystemCoreClock / 4;
	uint32_t ticks = (pclk / 1000000) * us;
	LPC_TIM0 -> MR0 = ticks;
	
	// Unsure if resetting TC is necessary here
	LPC_TIM0->TC = 0;
}

/**
 * Generate the next sample of the waveform
 */
static void wavegen_step(void) {
	volatile uint32_t idx;

	idx = sample_idx;

	switch (currentWaveform) {
		case SQUARE:
			if (idx < (STEPS_PER_WAVEFORM / 2)) {
				val = MAX_DAC_VALUE;
			} else {
				val = 0;
			}
			break;
		case TRIANGLE:
			if (idx < (STEPS_PER_WAVEFORM / 2)) {
				val += (MAX_DAC_VALUE / (STEPS_PER_WAVEFORM / 2));
			} else {
				val -= (MAX_DAC_VALUE / (STEPS_PER_WAVEFORM / 2));
			}
			break;
		case SAWTOOTH:
			val += ((MAX_DAC_VALUE) / (STEPS_PER_WAVEFORM));
			break;
		default:
			val = 0;
			break;
	}

	if (val > MAX_DAC_VALUE) val = MAX_DAC_VALUE;
	dac_set((int)val);

	sample_idx++;
	if (sample_idx >= samples_per_period) {
		sample_idx = 0;
		val = 0;
	}

}

/**
 * Initialise waveform generator
 */
void wavegen_init(void) {
    /* initialise timer using configured interrupt period and enable callback */
    timer_init(interrupt_rate_us);
    timer_set_callback(wavegen_update);
    timer_enable();
}

/**
 * Set the current waveform type
 * \param type Waveform type to set
 */
void wavegen_setWaveform(wavetype type) {
	if (currentWaveform != type) {
		val = 0;
	}

    currentWaveform = type;
    sample_idx = 0; /* Reset sample index when changing waveform */
}

/**
 * Set the frequency of the waveform
 * \param frequency Frequency to set (in Hz)
 * \return Number of samples per period
 */
uint32_t wavegen_setFrequency(float frequency) {
	float period_us;
    uint32_t ticks;
    
    currentFrequency = frequency;


    /* period in microseconds */
    period_us = 1e6f / currentFrequency;

    /* compute how many timer ticks (interrupts) per waveform period */
    ticks = (uint32_t)(period_us / (float)interrupt_rate_us);

    if (ticks < 1) ticks = 1;
    samples_per_period = ticks;
    sample_idx = 0;
	
	return samples_per_period;
}
