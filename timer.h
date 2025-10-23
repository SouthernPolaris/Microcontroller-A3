/*!
 * \brief     Controller for a hardware timer module.
 */
#ifndef TIMER_H
#define TIMER_H
#include <stdint.h>

/*! \brief Initialises the timer with a specified period.
 *  \param period  Period of the timer tick (in cpu \a cycles).
 */
void timer_init(uint32_t period);

/*! \brief Pass a callback to the API, which is executed during the
 *         interrupt handler.
 *  \param callback  Callback function.
 */
void timer_set_callback(void (*callback)(void));

/*! \brief Enables the timer operation. */
void timer_enable(void);

/*! \brief Disables the timer. */
void timer_disable(void);

#endif // TIMER_H