#include "finiteStateMachine.h"
#include "joystick.h"

// Current state of the finite state machine
wavetype currentState = IDLE;

/**
 * Initialize the finite state machine
 */
void fsmInit() {
    currentState = IDLE;
}

/**
 * Get the current state of the finite state machine
 * \return Current waveform state
 */
wavetype fsmGetCurrentState() {
    return currentState;
}

/**
 * Update the finite state machine based on joystick input
 * \return Updated waveform state
 */
wavetype fsmUpdate(void) {
	int up;
	int down;
	int left;
	int right;

	up = readJoystick(P_SW_UP);
	down = readJoystick(P_SW_DN);
	left = readJoystick(P_SW_LT);
	right = readJoystick(P_SW_RT);

	if (up) {
		currentState = SAWTOOTH;
	} else if (down) {
		currentState = IDLE;
	} else if (left) {
		currentState = TRIANGLE;
	} else if (right) {
		currentState = SQUARE;
	}

    return currentState;
}
