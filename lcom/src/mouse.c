#include <lcom/lcf.h>
#include "mouse.h"
#include "kbc.h"
#include "macros/error.h"

/// --------------------------------------------------------------------------------
///                                  Interruptions
/// --------------------------------------------------------------------------------

int mouse_hook_id = MOUSE_IRQ_LINE;

int mouse_subscribe_int(uint16_t *bit_no) {
    kbc_write_mouse_byte(SET_STREAM_MODE);
    kbc_write_mouse_byte(ENABLE_DATA_REPORTING);
    *bit_no = BIT(mouse_hook_id);
    error(sys_irqsetpolicy(MOUSE_IRQ_LINE, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id),
          "Error on mouse interruptions subscription");
    return 0;
}

int mouse_unsubscribe_int() {
    error(sys_irqrmpolicy(&mouse_hook_id), "Error on timer interruptions unsubscription");
    kbc_write_mouse_byte(DISABLE_DATA_REPORTING);
    return 0;
}

uint32_t packet_counter = 0;
uint8_t packet_bytes_count = 0;
uint8_t read_bytes[3];

int read_byte() {
    uint16_t packet_byte;
    kbc_read_return(&packet_byte);
    read_bytes[0] = read_bytes[1];
    read_bytes[1] = read_bytes[2];
    read_bytes[2] = (uint8_t) packet_byte;
    return 0;
}

struct mouse_packet last_packet;

struct mouse_packet mouse_get_last_packet() {
    return last_packet;
}

uint32_t mouse_get_packet_counter() {
    return packet_counter;
}

bool valid_packet(uint8_t *first_byte){
    return (*first_byte & SYNCHRONIZATION_BIT);
}

void update_last_packet() {
    for (unsigned i = 0; i < 3; ++i) {
        last_packet.bytes[i] = read_bytes[i];
    }
    last_packet.right_button = (read_bytes[0] & MOUSE_RIGHT_BUTTON);
    last_packet.middle_button = (read_bytes[0] & MOUSE_MIDDLE_BUTTON);
    last_packet.left_button = (read_bytes[0] & MOUSE_LEFT_BUTTON);
    last_packet.x_delta = read_bytes[1];
    if (read_bytes[0] & MOUSE_MSB_X_DELTA)
        last_packet.x_delta -= 256;
    last_packet.y_delta = read_bytes[2];
    if (read_bytes[0] & MOUSE_MSB_Y_DELTA)
        last_packet.y_delta -= 256;
    last_packet.x_overflow = read_bytes[0] & MOUSE_X_OVERFLOW;
    last_packet.y_overflow = read_bytes[0] & MOUSE_Y_OVERFLOW;
}

int16_t mouse_get_delta_x(){
    return last_packet.x_delta;
}

int16_t mouse_get_delta_y(){
    return last_packet.y_delta;
}

bool mouse_get_left_button(){
    return last_packet.left_button;
}

bool packet_ready(){
    return packet_bytes_count == 0;
}

void mouse_int_handler() {
    packet_bytes_count++;
    if (read_byte())
        return;
    if (packet_bytes_count < 3)
        return;
    update_last_packet();
    packet_counter++;
    packet_bytes_count = 0;
}

