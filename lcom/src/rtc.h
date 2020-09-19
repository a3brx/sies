#pragma once

#include "macros/rtc.h"

/*
 * @brief Reads the current time.
 * @return Return 0 if success, 1 otherwise.
 */
int rtc_read_time();

/*
 * @brief Subscribes rtc interrupts
 * @param bit_no Address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 * @return 0 if successful, 1 otherwise
 */
int rtc_subscribe_int(uint16_t *bit_no);

/*
 * @brief Unsubscribes rtc interrupts
 * @return 0 if successful, 1 otherwise
 */
int rtc_unsubscribe_int();

/*
 * @brief Handles an rtc interruption.
 */
void rtc_int_handler();

/*
 * @brief If it has passed a minute since the program was started, this function changes a global variable.
 */
void rtc_passed_a_minute();

