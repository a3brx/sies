#include <lcom/lcf.h>
#include "arrow.h"
#include "mouse.h"
#include "draw.h"
#include "chess.h"
#include "game.h"
#include "serial.h"
#include "macros/graphics.h"


static xpm_row_t const circle[] = {
        "22 22 2",
        "  0",
        ". 56",
        "          ..          ",
        "         ....         ",
        "       ........       ",
        "      ..........      ",
        "     ............     ",
        "    ..............    ",
        "   ................   ",
        "  ..................  ",
        " .................... ",
        " .................... ",
        "......................",
        "......................",
        " .................... ",
        " .................... ",
        "  ..................  ",
        "   .................  ",
        "   ................   ",
        "    ..............    ",
        "     ............     ",
        "       ........       ",
        "         ....         ",
        "          ..          "
};

static xpm_row_t const arrow[] = {
        "12 21 3",
        "  0",
        ". 56",
        "X 63",
/* pixels */
        "..          ",
        "...         ",
        ".X..        ",
        ".XX..       ",
        ".XXX..      ",
        ".XXXX..     ",
        ".XXXXX..    ",
        ".XXXXXX..   ",
        ".XXXXXXX..  ",
        ".XXXXXXXX.. ",
        ".XXXXXX XX..",
        ".XXXXXX.....",
        ".XXXXXX.    ",
        ".XXXXXX..   ",
        ".X....X .   ",
        "...  .XX..  ",
        "..   ..XX.  ",
        ".     .XX.. ",
        "      ..X . ",
        "       . X. ",
        "       .... "
};
static uint8_t mouse_line_sizes[21] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 12, 8, 9, 9, 10, 10, 11, 11, 11, 11};

extern uint8_t last_bytes[3];
extern vbe_mode_info_t vbe_info;

int32_t x = 0;
int32_t y = 0;

extern uint8_t *board_video_mem;
extern uint8_t *moves_video_mem;
extern void *video_mem;
extern unsigned char board_xi, board_yi;
extern enum color my_color;
extern bool multiplayer;

void get_position(int32_t x, int32_t y, struct position *res) {
    res->col = (x - board_xi) / 74;
    res->row = 7 - (y - board_yi) / 74;
}

void print_possible_move(char *position) {
    int32_t x = board_xi + (position[0] - 'a') * 74;
    int32_t y = board_yi + (7 - (position[1] - '1')) * 74;
    draw_sprite(circle, x + 74 / 2 - 11, y + 74 / 2 - 11, moves_video_mem);
    memcpy(video_mem, moves_video_mem, BUFFER_SIZE);
    draw_players_time();
}

static bool selected_piece = false;
int print_mouse() {
    uint8_t *ptr = video_mem;
    for (uint8_t i = 0; i < MIN(21, vbe_info.YResolution - y); i++) {
        memcpy(ptr + (y + i) * vbe_info.XResolution + x, moves_video_mem + (y + i) * vbe_info.XResolution + x,
               MIN(mouse_line_sizes[i], vbe_info.XResolution - x));
    }

    x += mouse_get_delta_x();
    if (x > vbe_info.XResolution - 4)
        x = vbe_info.XResolution - 4;
    if (x < 0)
        x = 0;
    y -= mouse_get_delta_y();
    if (y > vbe_info.YResolution - 4)
        y = vbe_info.YResolution - 4;
    if (y < 0)
        y = 0;

    draw_sprite(arrow, x, y, video_mem);

    if(get_now_playing() != my_color && multiplayer)
        return 0;

    static struct position from;
    if (mouse_get_left_button()) {
        if (!selected_piece) {
            struct position pos;
            get_position(x, y, &pos);
            char **moves = get_possible_moves(&pos);
            if(moves[0] == NULL)
                return 0;
            while (*moves != NULL) {
                print_possible_move(*moves);
                free(*moves);
                moves++;
            }
            from = pos;
            selected_piece = true;
        } else {
            struct position pos;
            get_position(x, y, &pos);
            char move[5];
            move[0] = from.col + 'a';
            move[1] = from.row + '1';
            move[2] = pos.col + 'a';
            move[3] = pos.row + '1';
            move[4] = '\0';
            if(!make_move(move) && multiplayer)
                sp_send_move(move);
            draw_board();
            selected_piece = false;
        }
    }
    return 0;
}
