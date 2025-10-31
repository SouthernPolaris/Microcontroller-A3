#ifndef FINITE_STATE_MACHINE_H
#define FINITE_STATE_MACHINE_H

#include "wave.h"

// Current state of the finite state machine
extern wavetype currentState;

/// @brief Initialise the finite state machine
void fsmInit(void);

/// @brief Get the current state of the finite state machine
/// @return Current waveform state
wavetype fsmGetCurrentState(void);

/// @brief Update the finite state machine based on joystick input
/// @return Updated waveform state
wavetype fsmUpdate(void);

#endif // FINITE_STATE_MACHINE_H
