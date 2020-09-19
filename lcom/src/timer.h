#pragma once

#include "macros/timer.h"

/*
 * @brief Resets the counter to 0
 */
void timer_reset_counter();

/*
 * @return Returns number of timer interruptions
 */
unsigned timer_get_counter();
