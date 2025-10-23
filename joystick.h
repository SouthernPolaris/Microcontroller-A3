#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "platform.h"

void initPortsForJoystick(void);
int readJoystick(Pin pin);

#endif // JOYSTICK_H
