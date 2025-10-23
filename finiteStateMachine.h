#ifndef FINITE_STATE_MACHINE_H
#define FINITE_STATE_MACHINE_H

#include "wave.h"

wavetype currentState = IDLE;

void fsmInit();

wavetype fsmGetCurrentState();

wavetype fsmUpdate(char joystickValue);

#endif // FINITE_STATE_MACHINE_H

