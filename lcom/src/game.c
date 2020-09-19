#include "game.h"

#include "art/pieces.h"
#include "macros/graphics.h"
#include "draw.h"
#include "serial.h"
#include "arrow.h"
#include <lcom/lcf.h>

#define SQUARE_SIZE 74
#define SQUARE_COLOR_1 14
#define SQUARE_COLOR_2 55
#define PIECE_SIZE 64
#define BLACK_COLOR (unsigned char) 0
#define WHITE_COLOR (unsigned char) 63

extern vbe_mode_info_t vbe_info;

extern uint8_t *board_video_mem;
extern uint8_t *moves_video_mem;
extern void *video_mem;

unsigned char board_xi, board_yi;

unsigned p1_time = 5 * 60, p2_time = 5 * 60;
uint16_t p1_time_pos_x = 30 + 40, p1_time_pos_y = 200 + 36;
uint16_t p2_time_pos_x = 835 + 40, p2_time_pos_y = 200 + 36;

extern char last_rtc_time[6];

static int draw_piece(xpm_map_t xpm, uint16_t x, uint16_t y, unsigned char *colors) {
    xpm_load(xpm, XPM_INDEXED, &image);
    uint8_t *byte = image.bytes;
    for (uint16_t row = 0; row < image.height; row++) {
        for (uint16_t col = 0; col < image.width; col++) {
            if (*byte != 0)
                draw_pixel(x + col, y + row, colors[*byte - 1], board_video_mem);
            byte++;
        }
    }
    return 0;
}

bool first_board = true;

int draw_board() {
    extern struct piece *board[8][8];
    board_xi = (vbe_info.XResolution - 8 * SQUARE_SIZE) >> 1;
    board_yi = (vbe_info.YResolution - 8 * SQUARE_SIZE) >> 1;
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            draw_rectangle(board_xi + SQUARE_SIZE * i, board_yi + SQUARE_SIZE * j, SQUARE_SIZE, SQUARE_SIZE,
                           (i + j) % 2 ? SQUARE_COLOR_1 : SQUARE_COLOR_2, board_video_mem);
    unsigned char padding = (SQUARE_SIZE - PIECE_SIZE) >> 1;
    struct position position;
    for (unsigned char row = 0; row < 8; row++)
        for (unsigned char col = 0; col < 8; col++) {
            position.col = col;
            position.row = row;
            struct piece *piece = get_piece(&position, board);
            if (piece == NULL)
                continue;
            unsigned char colors[2];
            colors[0] = (piece->color) ? BLACK_COLOR : WHITE_COLOR;
            colors[1] = (piece->color) ? WHITE_COLOR : BLACK_COLOR;
            switch (piece->type) {
                case PAWN:
                    draw_piece(pawn, board_xi + padding + col * SQUARE_SIZE,
                               board_yi + padding + (7 - row) * SQUARE_SIZE, colors);
                    break;
                case KNIGHT:
                    draw_piece(knight, board_xi + padding + col * SQUARE_SIZE,
                               board_yi + padding + (7 - row) * SQUARE_SIZE, colors);
                    break;
                case BISHOP:
                    draw_piece(bishop, board_xi + padding + col * SQUARE_SIZE,
                               board_yi + padding + (7 - row) * SQUARE_SIZE, colors);
                    break;
                case ROOK:
                    draw_piece(rook, board_xi + padding + col * SQUARE_SIZE,
                               board_yi + padding + (7 - row) * SQUARE_SIZE, colors);
                    break;
                case QUEEN:
                    draw_piece(queen, board_xi + padding + col * SQUARE_SIZE,
                               board_yi + padding + (7 - row) * SQUARE_SIZE, colors);
                    break;
                case KING:
                    draw_piece(king, board_xi + padding + col * SQUARE_SIZE,
                               board_yi + padding + (7 - row) * SQUARE_SIZE, colors);
                    break;
            }
        }

    write_string("player 1:", p1_time_pos_x - 40, p1_time_pos_y - 36, 9, board_video_mem);
    write_string("player 2:", p2_time_pos_x - 40, p2_time_pos_y - 36, 9, board_video_mem);
    write_string(last_rtc_time, 512 - 50, 30, 5, board_video_mem);
    memcpy(moves_video_mem, board_video_mem, BUFFER_SIZE);
    memcpy(video_mem, moves_video_mem, BUFFER_SIZE);
    if (first_board) {
        draw_players_time();
        first_board = false;
    }
    write_string("[r] resign", 512 - 5 * 20, 700, 10, moves_video_mem);
    return 0;
}

void update_player_timer() {
    switch (get_now_playing()) {
        case WHITE:
            if (p1_time == 0) {
                return;
            }
            p1_time--;
            break;
        case BLACK:
            if (p2_time == 0) {
                return;
            }
            p2_time--;
            break;
    }

    draw_players_time();
}

void draw_players_time() {
    char time_str_p1[4], time_str_p2[4];
    time_str_p1[0] = '0' + p1_time / 60;
    time_str_p1[1] = ':';
    time_str_p1[2] = '0' + (p1_time - (p1_time / 60) * 60) / 10;
    time_str_p1[3] = '0' + (p1_time - (p1_time / 60) * 60) % 10;
    write_string(time_str_p1, p1_time_pos_x, p1_time_pos_y, 4, board_video_mem);
    time_str_p2[0] = '0' + p2_time / 60;
    time_str_p2[1] = ':';
    time_str_p2[2] = '0' + (p2_time - (p2_time / 60) * 60) / 10;
    time_str_p2[3] = '0' + (p2_time - (p2_time / 60) * 60) % 10;
    write_string(time_str_p2, p2_time_pos_x, p2_time_pos_y, 4, board_video_mem);
    uint8_t *ptr = moves_video_mem;
    uint32_t displ;
    for (int i = 0; i < 32; i++) {
        displ = (p1_time_pos_x) + (p1_time_pos_y + i) * vbe_info.XResolution;
        memcpy(ptr + displ, board_video_mem + displ, 20 * 4);
        displ = (p2_time_pos_x) + (p2_time_pos_y + i) * vbe_info.XResolution;
        memcpy(ptr + displ, board_video_mem + displ, 20 * 4);
    }
    memcpy(video_mem, moves_video_mem, BUFFER_SIZE);
}

bool p1_time_finished() {
    return (p1_time == 0);
}

bool p2_time_finished() {
    return (p2_time == 0);
}

uint16_t last_move_pos_x = 1024 / 2 - (int) (20 * 2.5), last_move_pos_y = 300;
uint16_t write_move_pos_x = 1024 / 2 - (int) (20 * 2.5), write_move_pos_y = 500;

void draw_blindfold() {
    memset(board_video_mem, 0, BUFFER_SIZE);
    memset(moves_video_mem, 0, BUFFER_SIZE);
    draw_box(last_move_pos_x - 10, last_move_pos_y - 20, 20 * 5 + 20, 36 + 20, 5, 63, board_video_mem);
    draw_box(write_move_pos_x - 10, write_move_pos_y - 20, 20 * 5 + 20, 36 + 20, 5, 63, board_video_mem);
    write_string("their move:", last_move_pos_x - (int) (2.5 * 20), last_move_pos_y - 60, 11, board_video_mem);
    write_string("your move:", write_move_pos_x - 2 * 20, write_move_pos_y - 60, 10, board_video_mem);
    write_string(last_rtc_time, 512 - 50, 30, 5, board_video_mem);
    write_string("[r] resign", 512 - 5 * 20, 700, 10, board_video_mem);
    memcpy(moves_video_mem, board_video_mem, BUFFER_SIZE);
    memcpy(video_mem, board_video_mem, BUFFER_SIZE);
}


char move[5] = "\0\0\0\0\0";
char char_counter = 0;
bool move_completed = false;

bool is_a_to_h(char c) {
    return (c >= 'a' && c <= 'h');
}

bool is_1_to_8(char c) {
    return (c >= '1' && c <= '8');
}

void input_char_to_move(char c) {
    if (c == '\b') {
        if (char_counter > 0)
            char_counter--;
        if (char_counter == 2)
            char_counter--;
        write_string(&c, write_move_pos_x + char_counter * 20, write_move_pos_y, 1, video_mem);
        move_completed = false;
        return;
    }
    if (move_completed) {
        if (c == '\n') {
            if (make_move(move)) {
                write_string("impossible move", 512 - (int) (7.5 * 20), 600, 15, moves_video_mem);
                write_string("     ", write_move_pos_x, write_move_pos_y, 5, moves_video_mem);
                move_completed = false;
                char_counter = 0;
                return;
            }
            sp_send_move(move);
            write_string("sent move", 512 - (int) (4.5 * 20), 600, 9, video_mem);
            move_completed = false;
            char_counter = 0;
            return;
        }
        return;
    }
    if (c == 0 || c == '\n') {
        return;
    }
    switch (char_counter) {
        case 0:
            if (!is_a_to_h(c)) return;
            move[0] = c;
            write_string(&c, write_move_pos_x + char_counter * 20, write_move_pos_y, 1, video_mem);
            char_counter++;
            break;
        case 1:
            if (!is_1_to_8(c)) return;
            move[1] = c;
            write_string(&c, write_move_pos_x + char_counter * 20, write_move_pos_y, 1, video_mem);
            char_counter++;
        case 2:
            write_string(" ", write_move_pos_x + char_counter * 20, write_move_pos_y, 1, video_mem);
            char_counter++;
            break;
        case 3:
            if (!is_a_to_h(c)) return;
            move[2] = c;
            write_string(&c, write_move_pos_x + char_counter * 20, write_move_pos_y, 1, video_mem);
            char_counter++;
            break;
        case 4:
            if (!is_1_to_8(c)) return;
            move[3] = c;
            write_string(&c, write_move_pos_x + char_counter * 20, write_move_pos_y, 1, video_mem);
            move_completed = true;
            char_counter++;
            break;
        default:
            break;
    }
}

