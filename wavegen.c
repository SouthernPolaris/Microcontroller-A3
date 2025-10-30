#include "wavegen.h"
#include "wave.h"
#include "timer.h"
#include "dac.h"
#include "platform.h"
#include "delay.h"
#include <stdint.h>

#define DEFAULT_INTERRUPT_US 10u

/* Use the platform DAC mask to obtain full DAC range (e.g. 10-bit). */
#define MAX_DAC_VALUE (1023)

volatile wavetype currentWaveform = IDLE;
float currentFrequency = 440.0f;
static int val;

uint32_t sample_idx = 0;
volatile uint32_t samples_per_period = 1; /* number of timer ticks per waveform period */
uint32_t interrupt_rate_us = DEFAULT_INTERRUPT_US; /* microseconds */

static void wavegen_update(void) {
    volatile uint32_t steps;
    volatile uint32_t idx;

    volatile uint32_t half;
		volatile int delay_val;

    steps = samples_per_period;
    idx = sample_idx;
    val = 0;
	
		if (steps < 2) steps = 2;
		half = steps / 2;


//		for(idx = sample_idx; idx < samples_per_period; idx++) {
//			switch (currentWaveform) {
//        case SQUARE:
//            if (idx < half) {
//							val = MAX_DAC_VALUE;
//						} else {
//							val = 0;
//						}
//            break;
//        case TRIANGLE:
//            /* Use (steps-1) as denominator so endpoints map exactly to 0 to MAX */
//            if (idx < half) {
//								val = (uint32_t)((uint64_t)idx * MAX_DAC_VALUE) / (half);
//						} else {
//								val = (uint32_t)(((uint64_t) steps - idx) * MAX_DAC_VALUE) / (half);
//						}
//						
//            break;
//        case SAWTOOTH:
//            /* Map 0 to (steps-1) to 0 to MAX_DAC_VALUE so last sample reaches MAX */
//            val = (idx * MAX_DAC_VALUE) / (steps - 1);

//						break;
//				default:
//            val = 0;
//            break;
//				
//				
//			}
//			
//			if (val > MAX_DAC_VALUE) val = MAX_DAC_VALUE;
//						dac_set((int)val);
//			
//			
//			delay_val = steps * (1/currentFrequency);
//			delay_us(delay_val);
//		}

	
	
			for(idx = sample_idx; idx < 50; idx++) {
				switch (currentWaveform) {
					case SQUARE:
							if (idx < 25) {
								val = MAX_DAC_VALUE;
							} else {
								val = 0;
							}
							break;
					case TRIANGLE:
							/* Use (steps-1) as denominator so endpoints map exactly to 0 to MAX */
							if (idx < 25) {
									
									val += (MAX_DAC_VALUE / 25);
								//val = (uint32_t)((uint64_t)idx * MAX_DAC_VALUE) / (25);
							} else {
									val -= (MAX_DAC_VALUE / 25);
									//val = (uint32_t)(((uint64_t) steps - idx) * MAX_DAC_VALUE) / (25);
							}
							
							break;
					case SAWTOOTH:
							/* Map 0 to (steps-1) to 0 to MAX_DAC_VALUE so last sample reaches MAX */
							val += ((MAX_DAC_VALUE) / (50));

							break;
					default:
							val = 0;
							break;
					
					
				}
				
				if (val > MAX_DAC_VALUE) val = MAX_DAC_VALUE;
							dac_set((int)val);
				
				
				delay_val = (steps * interrupt_rate_us) / 50;
				delay_us(delay_val);
			}
	
//    switch (currentWaveform) {
//        case SQUARE:
//            if (idx < half) {
//							val = MAX_DAC_VALUE;
//						} else {
//							val = 0;
//						}
//            break;
//        case TRIANGLE:
//            /* Use (steps-1) as denominator so endpoints map exactly to 0 to MAX */
//            if (idx < half) {
//								val = (uint32_t)((uint64_t)idx * MAX_DAC_VALUE) / (half);
//						} else {
//								val = (uint32_t)(((uint64_t) steps - idx) * MAX_DAC_VALUE) / (half);
//						}
//						
//            break;
//        case SAWTOOTH:
//            /* Map 0 to (steps-1) to 0 to MAX_DAC_VALUE so last sample reaches MAX */
//            val = (idx * MAX_DAC_VALUE) / (steps - 1);

//						break;
//				default:
//            val = 0;
//            break;
//		}
		
    /* clamp to DAC range and output */
//    if (val > MAX_DAC_VALUE) val = MAX_DAC_VALUE;
//    dac_set((int)val);

    /* advance index */
    //sample_idx = (sample_idx + 1) % samples_per_period;
}

void wavegen_init(void) {
    /* initialise timer using configured interrupt period and enable callback */
    timer_init(interrupt_rate_us);
    timer_set_callback(wavegen_update);
    timer_enable();
}

void wavegen_setWaveform(wavetype type) {
		if (currentWaveform != type) {
			val = 0;
		}
    currentWaveform = type;
    sample_idx = 0; /* Reset sample index when changing waveform */
}

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
