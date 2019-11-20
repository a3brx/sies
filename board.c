#include "board.h"
#include <stdlib.h>
#include <stdio.h>

static struct piece *board[8][8];

short get_col(const char *notation) {
    return notation[0] - 'a';
}

short get_row(const char *notation) {
    return notation[1] - '1';
}

struct piece *get_piece(const short *row, const short *col) {
    return board[*row][*col];
}

void initialize_color(enum color color) {
    short row = (color + 1) % 2 * 7;
    board[row][0] = construct_piece(color, ROOK);
    board[row][1] = construct_piece(color, KNIGHT);
    board[row][2] = construct_piece(color, BISHOP);
    board[row][3] = construct_piece(color, QUEEN);
    board[row][4] = construct_piece(color, KING);
    board[row][5] = construct_piece(color, BISHOP);
    board[row][6] = construct_piece(color, KNIGHT);
    board[row][7] = construct_piece(color, ROOK);
    row = row % 2 * 5 + 1;
    for (short col = 0; col < 8; ++col) {
        board[row][col] = construct_piece(color, PAWN);
    }
}

void construct_board() {
    initialize_color(WHITE);
    initialize_color(BLACK);
    for (int row = 2; row < 6; ++row) {
        for (int col = 0; col < 8; ++col) {
            board[row][col] = construct_piece(BLACK, NONE);
        }
    }
}

void destruct_board() {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            free(board[row][col]);
        }
    }
}

void print_board() {
    for (short row = 7; row >= 0; --row) {
        for (short col = 0; col < 8; ++col) {
            print_piece(get_piece(&row, &col));
        }
        printf("\n");
    }
}

void make_move(const char *from, const char *to) {
    struct piece **initial = &board[get_row(from)][get_col(from)];
    struct piece **final = &board[get_row(to)][get_col(to)];
    free(*final);
    *final = *initial;
    *initial = construct_piece(BLACK, NONE);

}
