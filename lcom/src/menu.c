#include <lcom/lcf.h>
#include "menu.h"
#include "game.h"
#include "draw.h"
#include "interrupts.h"
#include "macros/keyboard.h"
#include "macros/graphics.h"
#include "keyboard.h"
#include "mouse.h"
#include "graphics.h"
#include "arrow.h"
#include "rtc.h"
#include "serial.h"
#include "timer.h"

extern uint8_t *moves_video_mem;
extern void *video_mem;
extern bool passed_a_minute;
extern char last_rtc_time[6];

uint16_t title_x = 512 - (int)(5.5 * 20), title_y = 300;
uint16_t options_x = 512 - (int)(9 * 20), options_y = 500;

enum color my_color = BLACK;

bool multiplayer = false;

int draw_menu() {
  reset_video_mems();

  write_string("minix chess", title_x, title_y, 12, moves_video_mem);
  write_string("[ n ] normal mode\n[ b ] blindfolded\n[ s ] hot seat\n[esc] exit", options_x, options_y, 61, moves_video_mem);
  write_string(last_rtc_time, 512 - 50, 30, 5, moves_video_mem);

  enum serial_status actual_status = MENU;
  int ret = -1;
    do {
        if (request_message())
            continue;
        if (received_message(TIMER)) {
            timer_int_handler();
            if (timer_get_counter() % 60 == 0)
                if (!passed_a_minute)
                    rtc_passed_a_minute();
            if (timer_get_counter() % 3600 == 0)
                rtc_int_handler();
        }
        if (received_message(KEYBOARD)) {
            keyboard_int_handler();
            if (keyboard_get_last_scancode() == S_BREAKCODE) return 3;
            if (keyboard_get_last_scancode() == B_BREAKCODE || keyboard_get_last_scancode() == N_BREAKCODE) {
                actual_status = WAITING_FOR_PLAYER;
                sp_send_char(WAITING_FOR_PLAYER);
                ret = keyboard_get_last_scancode() == B_BREAKCODE ? 1 : 2;
                draw_init_multi(ret - 1);
                multiplayer = true;
            };
        }
        if (received_message(SERIAL_PORT)) {
            sp_read_char();
            if (get_last_char_read() == READY) {
                actual_status = READY;
                sp_send_char(READY);
                return ret;
            } else if (get_last_char_read() == WAITING_FOR_PLAYER) {
                if (actual_status == READY) {
                    sp_send_char(READY);
                } else if (actual_status == WAITING_FOR_PLAYER) {
                    actual_status = READY;
                    sp_send_char(READY);
                    my_color = WHITE;
                }
            }
        }
    } while (keyboard_get_last_scancode() != ESC_BREAKCODE);
    return 0;
}

void draw_init_multi(bool mode) {
    memset(moves_video_mem, 0, BUFFER_SIZE);
    if (!mode) 
      write_string("blindfold mode", title_x - 20, title_y, 14, moves_video_mem);
    else 
      write_string("board mode", title_x, title_y, 10, moves_video_mem);
    write_string("waiting for other player", 512 - 12 * 20, options_y, 24, moves_video_mem);
    write_string(last_rtc_time, 512 - 50, 30, 5, moves_video_mem);

    memcpy(video_mem, moves_video_mem, BUFFER_SIZE);
}

int print_winner() {
    memset(video_mem, 0, BUFFER_SIZE);
    uint16_t title_y = 300;
    uint16_t message_x = 512 - (int)(5 * 20), message_y = 500;
    write_string(get_final_message(), get_final_message_position_x(), title_y, get_final_message_size(), video_mem);
    write_string("[esc] exit", message_x, message_y, 10, video_mem);
    rtc_read_time();
    write_string(last_rtc_time, 512 - 50, 30, 5, moves_video_mem);

    do {
        if (request_message())
            continue;
        if (received_message(KEYBOARD)) {
            keyboard_int_handler();
            if (keyboard_get_last_scancode() == ESC_BREAKCODE) return 0;
        }
    } while (true);
}

