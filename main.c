#include <platform.h>
#include "adc.h"
#include "joystick.h"
#include "comparator.h"
#include "finiteStateMachine.h"
#include "calcFrequency.h"

int main(void) {

    // Initialise all functions
		adc_init();
		initPortsForJoystick();
		comparator_init();
		tone_init();
		fsmInit();
    // Set FSM to idle wave
		
	  // IDLE
		fsmUpdate(0);
	
		while(1) {
	
			// RUN ADC Read on VC
			float vc = adc_read(1);
		
			// RUN ADC Read on modulation
			float mod = adc_read(2);
		
			// RUN ADC Read on potentiometer
			float base = adc_read(0);

			// Create frequency output from formula
			
			float freq = calculateFrequency(vc, mod, base);

			// READ joystick
			int joystickOut = readJoystick(5);

			// FSM SET WATE STATE --- LED indicator as well
			wavetype state = fsmUpdate(joystickOut);

			// OUTPUT WAVE
			// TODO: MODIFY THIS to not do set cycles but have a timer interrupt until new joystick state is triggered
			// NEED TO Set up an interrupt as this isnt LFO
			// Interrupt is equivalent to step
			// Needed so loop time doesnt add onto wave time
			// When reaches voltage max of 1023, step down
			// Dont do tone play
			wavegen_setFrequency(freq);
			wavegen_setWaveform(state);
		}
}