// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <stdint.h>

// Any header files included below this line should have been created by you
#include "interrupts.h"
#include "timer.h"
#include "keyboard.h"
#include "mouse.h"
#include "graphics.h"
#include "arrow.h"
#include "game.h"
#include "rtc.h"
#include "menu.h"
#include "serial.h"

int main(int argc, char *argv[]) {
    // sets the language of LCF messages (can be either EN-US or PT-PT)
    lcf_set_language("EN-US");
    // enables to log function invocations that are being "wrapped" by LCF
    // [comment this out if you don't want/need it]
    // lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");
    // enables to save the output of printf function calls on a file
    // [comment this out if you don't want/need it]
    // lcf_log_output("/home/lcom/labs/lab5/output.txt");
    // handles control over to LCF
    // [LCF handles command line arguments and invokes the right function]
    if (lcf_start(argc, argv))
        return 1;
    // LCF clean up tasks
    // [must be the last statement before return]
    lcf_cleanup();
    return 0;
}

extern enum color my_color;

int (proj_main_loop)(int argc, char *argv[]){
    subscribe_ints(TIMER | KEYBOARD | SERIAL_PORT);
    init_graphics(0x105);
    init_video_mems();
    rtc_read_time();
            
    extern char last_rtc_time[6];
    extern bool passed_a_minute;

    while (true) {
        construct_game();
    switch (draw_menu()) {
        case 2: {
            subscribe_ints(MOUSE);
            draw_board();
            do {
                if (request_message())
                    continue;
                if (received_message(TIMER)) {
                    if (p1_time_finished() || p2_time_finished()) {
                        end_time();
                        break;
                    } 
                    timer_int_handler();
                    if(timer_get_counter() % 60 == 0) {
                    update_player_timer();
                    if (!passed_a_minute)       
                        rtc_passed_a_minute();
                    }
                    if (timer_get_counter() % 3600 == 0) {
                        rtc_int_handler();
                    }
                }
                if (received_message(KEYBOARD)) {
                    keyboard_int_handler();
                    if(keyboard_get_last_scancode() == R_BREAKCODE) {
                        make_move("res\n");
                        sp_send_char(RESIGN_SERIAL);
                        break;
                    }                    
                    if(keyboard_get_last_scancode() == ESC_BREAKCODE) {
                        sp_send_char(EXIT_SERIAL);
                        break;
                    }
                }
                if (received_message(MOUSE)) {
                    mouse_int_handler();
                    if(packet_ready()) {
                        print_mouse();
                    }
                    if(get_status() != PLAYING && get_status() != TO_BEGIN)
                        break;
                }
                if (received_message(SERIAL_PORT)) {
                    if(sp_int_handler()) {
                        if(get_last_char_read() == RESIGN_SERIAL) {
                            make_move("res\n");
                            break;
                        }                        
                        if(get_last_char_read() == EXIT_SERIAL) {
                            break;
                        }
                        draw_board();
                    }
                    if(get_status() != PLAYING && get_status() != TO_BEGIN)
                        break;
                }
            } while (keyboard_get_last_scancode() != ESC_BREAKCODE);
            unsubscribe_ints(MOUSE);
            break;
        }
        case 1: {
            draw_blindfold();
            do {
                if (request_message())
                    continue;
                if (received_message(TIMER)) {
                    timer_int_handler();
                    if (timer_get_counter() % 60 == 0) {
                        if (!passed_a_minute)
                            rtc_passed_a_minute();
                    }
                    if (timer_get_counter() % 3600 == 0) {
                        rtc_int_handler();
                    }
                }
                if (received_message(KEYBOARD)) {
                    keyboard_int_handler();
                    if(keyboard_get_last_scancode() == R_BREAKCODE) {
                        make_move("res\n");
                        sp_send_char(RESIGN_SERIAL);
                        break;
                    }
                    if(keyboard_get_last_scancode() == ESC_BREAKCODE) {
                        sp_send_char(EXIT_SERIAL);
                        break;
                    }
                    if(get_now_playing() != my_color)
                        continue;
                    input_char_to_move(last_scancode_to_char());
                    if(get_status() != PLAYING && get_status() != TO_BEGIN)
                        break;
                }
                if (received_message(SERIAL_PORT)) {
                    sp_int_handler();
                    if(get_last_char_read() == RESIGN_SERIAL) {
                        make_move("res\n");
                        break;
                    }                    
                    if(get_last_char_read() == EXIT_SERIAL) {
                        break;
                    }
                    if(get_status() != PLAYING && get_status() != TO_BEGIN)
                        break;
                }
            } while (keyboard_get_last_scancode() != ESC_BREAKCODE);
            break;
        }
        case 3: {
            subscribe_ints(MOUSE);
            draw_board();
            do {
                if (request_message())
                    continue;
                if (received_message(TIMER)) {
                    if (p1_time_finished() || p2_time_finished()) {
                        end_time();
                        break;
                    } 
                    timer_int_handler();
                    if(timer_get_counter() % 60 == 0) {
                    update_player_timer();
                    if (!passed_a_minute)       
                        rtc_passed_a_minute();
                    }
                    if (timer_get_counter() % 3600 == 0) {
                        rtc_int_handler();
                    }
                }
                if (received_message(KEYBOARD)) {
                    keyboard_int_handler();
                    if(keyboard_get_last_scancode() == R_BREAKCODE) {
                        make_move("res\n");
                        break;
                    }
                }
                if (received_message(MOUSE)) {
                    mouse_int_handler();
                    if(packet_ready()) {
                        print_mouse();
                    }
                    if(get_status() != PLAYING && get_status() != TO_BEGIN)
                        break;
                }
            } while (keyboard_get_last_scancode() != ESC_BREAKCODE);
            unsubscribe_ints(MOUSE);
            break;
        }
        default:
        break;
    }
    destruct_game();
    int next = print_winner();
    if (next == 0) break;
    if (next == 1) continue;
    }
    unsubscribe_ints(TIMER);
    unsubscribe_ints(KEYBOARD);
    vg_exit();
    free_video_mems();
    return 0;
}
