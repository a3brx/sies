#pragma once
#include "macros/keyboard.h"

/*
 * @return Returns the last scancode bytes read
 */
uint16_t keyboard_get_last_scancode();

/*
 * @brief Reads the last scancode and saves it
 */
void keyboard_int_handler();

/*
 * @brief Subscribes an interruption
 * @param bit_no address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 * @return Returns 0 if there was not an error
 */
int keyboard_subscribe_int(uint8_t *bit_no);

/*
 * @brief Unsubscribes keyboard interruptions
 * @return Returns 0 if there was not an error
 */
int keyboard_unsubscribe_int();

/*
 * @return Returns the last scancode as a char
 */
char last_scancode_to_char();
