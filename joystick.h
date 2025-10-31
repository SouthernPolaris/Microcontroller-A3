#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "platform.h"

/// @brief Initialise GPIO pins for joystick buttons
void initPortsForJoystick(void);

/// @brief Read the state of a joystick button
/// @param pin Pin corresponding to the joystick button
/// @return 1 if the button is pressed, 0 otherwise
int readJoystick(Pin pin);

#endif // JOYSTICK_H
