#include <lcom/lcf.h>
#include "keyboard.h"
#include "kbc.h"
#include "macros/error.h"

/// --------------------------------------------------------------------------------
///                                  Interruptions
/// --------------------------------------------------------------------------------

int keyboard_hook_id = KEYBOARD_IRQ_LINE;

int keyboard_subscribe_int(uint8_t *bit_no) {
    *bit_no = BIT(keyboard_hook_id);
    error(sys_irqsetpolicy(KEYBOARD_IRQ_LINE, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hook_id),
          "Error on keyboard interruptions subscription");
    return 0;
}

int keyboard_unsubscribe_int() {
    error(sys_irqrmpolicy(&keyboard_hook_id), "Error on timer interruptions unsubscription");
    return 0;
}

struct scancode {
    bool mode;
    uint8_t size;
    uint8_t bytes[2];
};

struct scancode last_scancode;

uint16_t keyboard_get_last_scancode() {
    return (last_scancode.bytes[1] << 8) | last_scancode.bytes[0];
}

void keyboard_int_handler() {
    uint16_t scancode;
    if (kbc_read_return(&scancode))
        return;
    last_scancode.mode = (scancode & BREAKCODE) ? false : true;
    last_scancode.bytes[0] = scancode;
    last_scancode.bytes[1] = scancode >> 8;
    last_scancode.size = (last_scancode.bytes[1] == LONG_SCANCODE) ? 2 : 1;
}

void keyboard_reset_scancode() {
    last_scancode.bytes[0] = 0;
    last_scancode.bytes[1] = 0;
}


char last_scancode_to_char() {
    if (last_scancode.size != 1) {
        return 0;
    }
    switch (last_scancode.bytes[0]) {
        case A_BREAKCODE: 
            return 'a';
        case B_BREAKCODE: 
            return 'b';
        case C_BREAKCODE: 
            return 'c';
        case D_BREAKCODE: 
            return 'd';
        case E_BREAKCODE: 
            return 'e';
        case F_BREAKCODE: 
            return 'f';
        case G_BREAKCODE: 
            return 'g';
        case H_BREAKCODE: 
            return 'h';
        case NUM1_BREAKCODE: 
            return '1';
        case NUM2_BREAKCODE: 
            return '2';
        case NUM3_BREAKCODE: 
            return '3';
        case NUM4_BREAKCODE: 
            return '4';
        case NUM5_BREAKCODE: 
            return '5';
        case NUM6_BREAKCODE: 
            return '6';
        case NUM7_BREAKCODE: 
            return '7';
        case NUM8_BREAKCODE: 
            return '8';
        case BACKSPACE_BREAKCODE: 
            return '\b';        
        case ENTER_BREAKCODE: 
            return '\n';
        default:
            return 0;
    }
}

/// --------------------------------------------------------------------------------
///                            Functions only for the lab
/// --------------------------------------------------------------------------------

void keyboard_print_last_scancode() {
    kbd_print_scancode(last_scancode.mode, last_scancode.size, last_scancode.bytes);
}
