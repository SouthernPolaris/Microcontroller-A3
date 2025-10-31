#include "joystick.h"
#include "platform.h"
#include "gpio.h"

/**
 * Initialise GPIO pins for joystick buttons
 */
void initPortsForJoystick() {
	// Initialise GPIO pins for joystick buttons as inputs with pull-up resistors
	gpio_set_mode(P_SW_UP, PullUp);
	gpio_set_mode(P_SW_DN, PullUp);
	gpio_set_mode(P_SW_LT, PullUp);
	gpio_set_mode(P_SW_RT, PullUp);
	gpio_set_mode(P_SW_CR, PullUp);
}

/**
 * Read the state of a joystick button
 * \param pin Pin corresponding to the joystick button
 * \return 1 if the button is pressed, 0 otherwise
 */
int readJoystick(Pin pin) {
   	return !gpio_get(pin);
}
