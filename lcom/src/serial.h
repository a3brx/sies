#pragma once

/*
 * @brief Enum that registers the current state.
 */
enum serial_status {
  MENU, WAITING_FOR_PLAYER, WAITING_FOR_MOVE, READY, RESIGN_SERIAL, EXIT_SERIAL
};

/*
 * @brief Subscribes serial port interrupts
 * @param bit_no Address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 * @return 0 if successful, 1 otherwise
 */
int sp_subscribe_int(uint8_t *bit_no);

/*
 * @brief Unsubscribes serial port interrupts
 * @return 0 if successful, 1 otherwise
 */
int sp_unsubscribe_int();

/*
 * @brief Initializes the serial port.
 * @param base_addr Base address of the serial port.
 * @return Returns 0.
 */
int sp_initialize(unsigned base_addr);

/*
 * @brief Reads a char from the serial port.
 * @return Returns true if successful, false otherwise
 */
bool sp_read_char();

/*
 * @brief Sends a char to the serial port.
 * @oaram to_send Char to send.
 * @return Returns true if successful, false otherwise
 */
bool sp_send_char(char to_send);

/*
 * @brief Sends a move to the serial port.
 * @oaram move Move to send.
 * @return Returns true if successful, false otherwise
 */
bool sp_send_move(char move[5]);

/*
 * @brief handles an int.
 * @return Return 1 if there is a move.
 */
bool sp_int_handler();

/*
 * @brief Stores the last_char read
 * @return Return 1 if there is a move.
 */
bool sp_store_last_char();

/*
 * @brief Gets the last_char_read
 * @return Last char that was read.
 */
char get_last_char_read();

