#pragma once

/*
 * @brief Enum to help with interrupts subscription, unsubsctiption and handling.
 */
enum dispositive {
    TIMER = BIT(0), KEYBOARD = BIT(1), MOUSE = BIT(2), RTC = BIT(3), SERIAL_PORT = BIT(4)
};

/*
 * @brief Verifies if any message from the driver has been received.
 * @return Returns 0 if received, 1 if error occurred.
 */
int request_message();

/*
 * @brief Checks if a message from the dispositive was received.
 * @param dispositive Dispositive to check.
 * @return True if received, False otherwise
 */
bool received_message(enum dispositive dispositive);

/*
 * @brief Subscribes dispositives interrupts.
 * @param dispositives Which dispositives to subbscribe.
 * @return Return 0 if successful, 1 otherwise.
 */
int subscribe_ints(uint8_t dispositives);

/*
 * @brief Unsubscribes dispositives interrupts.
 * @param dispositives Which dispositives to unsubbscribe.
 * @return Return 0 if successful, 1 otherwise.
 */
int unsubscribe_ints(uint8_t dispositives);
