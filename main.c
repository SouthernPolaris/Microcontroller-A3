#include <platform.h>
#include "adc.h"
#include "dac.h"
#include "joystick.h"
#include "finiteStateMachine.h"
#include "wavegen.h"
#include "calcFrequency.h"

int main(void) {

    // Initialise all functions
		adc_init();
		initPortsForJoystick();
		dac_init();
		wavegen_init();
		fsmInit();
    // Set FSM to idle wave
		
	  // IDLE
		fsmUpdate(0);
	
		while(1) {
	
			// RUN ADC Read on VC
			float vc = adc_read_voltage(1);
		
			// RUN ADC Read on modulation
			float mod = adc_read_voltage(2);

			// RUN ADC Read on potentiometer
			float base = adc_read_voltage(0);

			// Create frequency output from formula
			
			float freq = calculateFrequency(vc, mod, base);

			// READ joystick
			int joystickOut = readJoystick(5);

			// FSM SET WATE STATE --- LED indicator as well
			wavetype state = fsmUpdate(joystickOut);

			// OUTPUT WAVE
			// TODO: Have a timer interrupt until new joystick state is triggered
			// NEED TO Set up an interrupt as this isnt LFO
			// Interrupt is equivalent to step
			// Needed so loop time doesnt add onto wave time
			// When reaches voltage max of 1023 (apparently is actually 4096 as 12 bit ADC but double check), step down
			wavegen_setFrequency(freq);
			wavegen_setWaveform(state);
		}
}