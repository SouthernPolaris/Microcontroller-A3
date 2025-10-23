#include "finiteStateMachine.h"

#define UP = 0x02
#define DOWN = 0x01
#define LEFT = 0x00
#define RIGHT = 0x04

void fsmInit() {
    currentState = IDLE;
}

wavetype fsmGetCurrentState() {
    return currentState;
}

wavetype fsmUpdate(char joystickValue) {
    switch (currentState) {
        case IDLE:
            if (joystickValue == 0x01) {
							currentState = IDLE;
            }
            break;
        case SQUARE:
            if (joystickValue == 0x04) {
                currentState = SQUARE;
            }
            break;
        case RAMP:
            if (joystickValue == 0x00) {
                currentState = RAMP;
            }
            break;
				case SINE:
						if (joystickValue == 0x02) {
                currentState = SINE;
            }
            break;
    }

    return currentState;
}