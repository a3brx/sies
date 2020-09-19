#pragma once

#include "macros/kbc.h"

/*
 * @brief Gets status byte of the kbc controler
 * @return Returns 0 if there was no error
 */
int kbc_get_status();

/*
 * @brief Sends a command to the kbc controler
 * @param command The command dto be sent
 * @return Returns 0 if there was no error
 */
int kbc_write_command(uint32_t command);

/*
 * @brief Reads return of kbc controler
 * @param data Variable to be initialized
 * @return Returns 0 if there was no error
 */
int kbc_read_return(uint16_t *data);

/*
 * @brief Writes byte to mouse
 * @param byte Byte to be written
 * @return Returns 0 if there was no error
 */
int kbc_write_mouse_byte(uint8_t byte);
