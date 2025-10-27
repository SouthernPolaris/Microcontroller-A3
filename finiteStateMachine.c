#include "finiteStateMachine.h"
#include "joystick.h"

wavetype currentState = IDLE;

void fsmInit() {
    currentState = IDLE;
}

wavetype fsmGetCurrentState() {
    return currentState;
}

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
