#pragma once

#include "macros/mouse.h"

/*
 * @brief A struct that has information of a mouse packet
 */
struct mouse_packet {
    bool y_overflow;
    bool x_overflow;
    bool middle_button;
    bool right_button;
    bool left_button;
    int16_t x_delta;
    int16_t y_delta;
    uint8_t bytes[3];
};

/*
 * @brief Subscribes and enables Timer 0 interrupts
 * @param bit_no address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 * @return Returns 0 if there was no error
 */
int mouse_subscribe_int(uint16_t *bit_no);

/*
 * @brief Unsubscribes Timer 0 interrupts
 * @return Returns 0 if there was no error
 */
int mouse_unsubscribe_int();

/*
 * @return Returns the delta x of the last packet read
 */
int16_t mouse_get_delta_x();

/*
 * @return Returns the delta y of the last packet read
 */
int16_t mouse_get_delta_y();

/*
 * @return Returns true if the left button bit is enabled on the last packet read
 */
bool mouse_get_left_button();

/*
 * @return Returns true if there is a valid packet that has all the three bytes read
 */
bool packet_ready();

/*
 * @brief Increments counter
 */
void mouse_int_handler();

