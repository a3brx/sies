#include <lcom/lcf.h>
#include "serial.h"
#include "macros/serial.h"
#include "macros/error.h"
#include "interrupts.h"
#include "game.h"
#include "draw.h"

unsigned sp_base_addr;
char move_received[5] = "\0\0\0\0\0";
uint8_t last_char_read;
uint8_t received_chars = 0;

int sp_hook_id;

int sp_subscribe_int(uint8_t *bit_no) {
    if (sp_base_addr == COM1_BASE_ADDR) {
        sp_hook_id = COM1_IRQ;
        *bit_no = BIT(sp_hook_id);
        error(sys_irqsetpolicy(COM1_IRQ, IRQ_EXCLUSIVE | IRQ_REENABLE, &sp_hook_id),
              "Error on Serial Port interruptions subscription");
    } else if (sp_base_addr == COM2_BASE_ADDR) {
        sp_hook_id = COM2_IRQ;
        *bit_no = BIT(sp_hook_id);
        error(sys_irqsetpolicy(COM2_IRQ, IRQ_EXCLUSIVE | IRQ_REENABLE, &sp_hook_id),
              "Error on Serial Port interruptions subscription");
    }
    return 0;
}

int sp_unsubscribe_int() {
    error(sys_irqrmpolicy(&sp_hook_id), "Error on Serial Port interruptions unsubscription");
    return 0;
}

int sp_initialize(unsigned base_addr) {
    sp_base_addr = base_addr;

    uint8_t val = DLAB_BIT;
    sys_outb(sp_base_addr + LINE_CTRL_ADDR, val);
    uint16_t divisor = DLAB_BIT_RATE_DIV / 9600;
    uint8_t msb_divisor, lsb_divisor;
    util_get_LSB(divisor, &lsb_divisor);
    util_get_MSB(divisor, &msb_divisor);
    sys_outb(sp_base_addr + DVSR_LATCH_LSB_ADDR, lsb_divisor);
    sys_outb(sp_base_addr + DVSR_LATCH_MSB_ADDR, msb_divisor);
    val = (WORDS_8BITS | STOP_BITS_2) | BIT(3);
    sys_outb(sp_base_addr + LINE_CTRL_ADDR, val);

    util_sys_inb(sp_base_addr + INT_ENABLE_REG_ADDR, &val);
    val &= NO_INTS_MASK;
    val |= ENABLE_DATA_RECEIVED_INT;
    sys_outb(sp_base_addr + INT_ENABLE_REG_ADDR, val);

    val = ENABLE_FIFO | CLEAR_RECEIVE_FIFO | CLEAR_TRANSMIT_FIFO;
    sys_outb(sp_base_addr + FIFO_CTRL_ADDR, val);

    return 0;
}


int sp_check_send() {
    uint8_t val;
    util_sys_inb(sp_base_addr + LINE_STATUS_REG_ADDR, &val);
    return val & TRANSMIT_HOLD_REG_EMPTY;
}

int sp_check_receive() {
    uint8_t val;
    util_sys_inb(sp_base_addr + LINE_STATUS_REG_ADDR, &val);
    return val & RECEIVER_RDY;
}

bool sp_read_char() {
    util_sys_inb(sp_base_addr + RECEIVED_BUF_ADDR, &last_char_read);
    // printf("Read %d\n", last_char_read);
    return true;
}

bool sp_send_char(char to_send) {
    for (int i = 0; i < 20; i++) {
      if (sp_check_send()) break;
      printf("Error sending char\n");
      if (i == 19) return false;
    }
    sys_outb(sp_base_addr + TRANSMIT_BUF_ADDR, to_send);
    // printf("Sent %d\n", to_send);
    return true;
}

bool sp_send_move(char move[5]) {
    for (int i = 0; i < 4; i++) {
        if (!sp_send_char(move[i]))
            return false;
    }
    return true;
}

bool sp_int_handler() {
    uint8_t val;
    bool ret = false;
    do {
        sp_read_char();
        if (last_char_read == RESIGN_SERIAL || last_char_read == EXIT_SERIAL) return true;
        ret = ret || sp_store_last_char();
        util_sys_inb(sp_base_addr + LINE_STATUS_REG_ADDR, &val);
    } while (val & 1);
    return ret;
}

char *sp_get_move() {
    return move_received;
}

extern uint16_t last_move_pos_x, last_move_pos_y;
extern void *video_mem;

bool sp_store_last_char() {
    if (!((last_char_read >= '1' && last_char_read <= '8') || (last_char_read >= 'a' && last_char_read <= 'h'))) return false;

    move_received[received_chars] = last_char_read;
    received_chars++;
    bool ret = false;
    if (received_chars == 4) {
        received_chars = 0;
        ret = !make_move(move_received);
        write_string(move_received, last_move_pos_x, last_move_pos_y, 2, video_mem);
        write_string(" ", last_move_pos_x + 40, last_move_pos_y, 1, video_mem);
        write_string(move_received + 2, last_move_pos_x + 60, last_move_pos_y, 2, video_mem);
    }
    return ret;
}

char get_last_char_read() {
    return last_char_read;
}

uint8_t sp_check_other_player() {
    return (last_char_read == WAITING_FOR_PLAYER || last_char_read == READY);
}
