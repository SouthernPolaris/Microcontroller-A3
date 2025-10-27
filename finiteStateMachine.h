#ifndef FINITE_STATE_MACHINE_H
#define FINITE_STATE_MACHINE_H

#include "wave.h"

extern wavetype currentState;

void fsmInit(void);

wavetype fsmGetCurrentState(void);

wavetype fsmUpdate(void);

#endif // FINITE_STATE_MACHINE_H
