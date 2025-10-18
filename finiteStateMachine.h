#ifndef FINITE_STATE_MACHINE_H
#define FINITE_STATE_MACHINE_H

typedef enum {
    STATE_IDLE,
    STATE_SQUARE,
    STATE_TRIANGLE,
} State;

State currentState;

void fsmInit();

State fsmGetCurrentState();

State fsmUpdate(int joystickValue);

#endif // FINITE_STATE_MACHINE_H