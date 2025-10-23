#include "joystick.h"
#include <platform.h>
#include <gpio.h>

void initPortsForJoystick() {
	gpio_set_mode(P_SW_UP, PullUp); //Set up as input with pullup resistor
	gpio_set_mode(P_SW_DN, PullUp);
	gpio_set_mode(P_SW_LT, PullUp);
	gpio_set_mode(P_SW_RT, PullUp);
	gpio_set_mode(P_SW_CR, PullUp);
}

int readJoystick(Pin pin) {
   	return !gpio_get(pin);
}
