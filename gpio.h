#ifndef PINS_H
#define PINS_H

#include "platform.h"

/// @brief Defines mode of GPIO pin
typedef enum {
	Reset,   // Reset pin configuration
	Input,   // Sets pin as an input with no pull-up or pull-down
	Output,  // Sets pin as a low impedance output
	PullUp,  // Enables the internal pull-up resistor and sets pin as input
	PullDown // Enables the internal pull-down resistor and sets pin as input
} PinMode;

/// @brief Defines trigger mode for GPIO interrupts
typedef enum {
	None,   // Disables the interrupt.
	Rising, // Enables an interrupt on the rising edge.
	Falling // Enables an interrupt on the falling edge.
} TriggerMode;

/// @brief Toggle the state of a GPIO pin
/// @param pin Pin to toggle
void gpio_toggle(Pin pin);

/// @brief Set the state of a GPIO pin
/// @param pin Pin to set
/// @param value Value to set the pin to (0 or 1)
void gpio_set(Pin pin, int value);

/// @brief Get the state of a GPIO pin
/// @param pin Pin to read
/// @return State of the pin (0 or 1)
int gpio_get(Pin pin);

/// @brief Set the state of a range of GPIO pins
/// @param pin_base Base pin of the range
/// @param count Number of pins in the range
/// @param value Value to set the pins to (bitmask)
void gpio_set_range(Pin pin_base, int count, int value);

/// @brief Get the state of a range of GPIO pins
/// @param pin_base Base pin of the range
/// @param count Number of pins in the range
/// @return State of the pins
unsigned int gpio_get_range(Pin pin_base, int count);

/// @brief Set the mode of a GPIO pin
/// @param pin Pin to configure
/// @param mode New mode for the pin
void gpio_set_mode(Pin pin, PinMode mode);

/// @brief Configure the interrupt trigger mode for a GPIO pin
/// \param pin Pin to configure
/// \param trig New trigger mode for the pin
void gpio_set_trigger(Pin pin, TriggerMode trig);

/// @brief Set a callback function for GPIO interrupts
/// \param pin Pin to configure
/// \param callback Function to call on interrupt
void gpio_set_callback(Pin pin, void (*callback)(int status));

#endif // PINS_H
