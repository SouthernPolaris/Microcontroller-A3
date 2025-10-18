#include "finiteStateMachine.h"

void fsmInit() {
    currentState = STATE_IDLE;
}

State fsmGetCurrentState() {
    return currentState;
}

State fsmUpdate(int joystickValue) {
    switch (currentState) {
        case STATE_IDLE:
            if (joystickValue > 0) {
                currentState = STATE_SQUARE;
            }
            break;
        case STATE_SQUARE:
            if (joystickValue < 0) {
                currentState = STATE_TRIANGLE;
            }
            break;
        case STATE_TRIANGLE:
            if (joystickValue == 0) {
                currentState = STATE_IDLE;
            }
            break;
    }

    return currentState;
}