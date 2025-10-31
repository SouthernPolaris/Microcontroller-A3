#include "platform.h"
#include "adc.h"
#include "dac.h"
#include "joystick.h"
#include "finiteStateMachine.h"
#include "wavegen.h"
#include "calcFrequency.h"

int main(void) {
	volatile float vc;
	volatile float mod;
	volatile float base;
	volatile float freq;
	volatile wavetype state;

	volatile uint32_t debugFreq;
	
    // Initialise all functions
	adc_init();
	initPortsForJoystick();
	dac_init();
	wavegen_init();
	fsmInit();
	
	while(1) {

		// RUN ADC Read on VC
		vc = adc_read_voltage(0);
	
		// RUN ADC Read on modulation
		mod = 1.5; //adc_read_voltage(2);

		// RUN ADC Read on potentiometer
		base = adc_read_voltage(1);

		// Create frequency output from formula
		
		freq = calculateFrequency(vc, mod, base);

		// FSM SET WAVE STATE --- LED indicator as well
		state = fsmUpdate();

		// OUTPUT WAVE
		debugFreq = wavegen_setFrequency(freq);
		wavegen_setWaveform(state);
	}
}
