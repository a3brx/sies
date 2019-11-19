#include <stdint.h>
#include "board.h"

short get_col(char * notation) {
    return notation[0] - 'a';
}

short get_row(char * notation) {
    return notation[1] - '1';
}

struct piece *get_piece(const short *row, const short *col) {
    return board[*row][*col];
}

void initialize_color(enum color color) {
    short row = (color + 1) % 2 * 7;
    board[row][0] = create_piece(color, ROOK);
    board[row][1] = create_piece(color, KNIGHT);
    board[row][2] = create_piece(color, BISHOP);
    board[row][3] = create_piece(color, QUEEN);
    board[row][4] = create_piece(color, KING);
    board[row][5] = create_piece(color, BISHOP);
    board[row][6] = create_piece(color, KNIGHT);
    board[row][7] = create_piece(color, ROOK);
    row = row % 2 * 5 + 1;
    for (short col = 0; col < 8; ++col) {
        board[row][col] = create_piece(color, PAWN);
    }
}

void construct_board() {
    initialize_color(WHITE);
    initialize_color(BLACK);
    for (int row = 2; row < 6; ++row) {
        for (int col = 0; col < 8; ++col) {
            board[row][col] = create_piece(BLACK, NONE);
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
