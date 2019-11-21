#include "board.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/// ---------------------------------------------------------------------------
///                              Piece Definition
/// ---------------------------------------------------------------------------
struct piece {
    enum color color;
    enum type type;
};

struct piece *construct_piece(enum color color, enum type type) {
    struct piece *new_piece = (struct piece *) malloc(sizeof(struct piece));
    new_piece->type = type;
    new_piece->color = color;
    return new_piece;
}

void destruct_piece(struct piece *piecePtr) {
    free(piecePtr);
}

/// ---------------------------------------------------------------------------
///                             Position Definition
/// ---------------------------------------------------------------------------

struct position {
    char col;
    char row;
};

static struct position construct_position(const char *notation) {
    struct position ret;
    ret.col = (char) (notation[0] - 'a');
    ret.row = (char) (notation[1] - '1');
    return ret;
}

/// ---------------------------------------------------------------------------
///                              Board Definition
/// ---------------------------------------------------------------------------
static struct piece *board[8][8];

struct piece *get_piece(const char *notation) {
    struct position position = construct_position(notation);
    return board[position.row][position.col];
}

static void print_piece(const struct piece *piece) {
    if (piece == NULL)
        printf(" ");
    else
        printf("%c", piece->type);
}

static void initialize_color(enum color color) {
    short row = (short) ((color + 1) % 2 * 7);
    board[row][0] = construct_piece(color, ROOK);
    board[row][1] = construct_piece(color, KNIGHT);
    board[row][2] = construct_piece(color, BISHOP);
    board[row][3] = construct_piece(color, QUEEN);
    board[row][4] = construct_piece(color, KING);
    board[row][5] = construct_piece(color, BISHOP);
    board[row][6] = construct_piece(color, KNIGHT);
    board[row][7] = construct_piece(color, ROOK);
    row = (short) (row % 2 * 5 + 1);
    for (short col = 0; col < 8; ++col) {
        board[row][col] = construct_piece(color, PAWN);
    }
}

void construct_board() {
    memset(board, 0, 64);
    initialize_color(WHITE);
    initialize_color(BLACK);
}

void destruct_board() {
    for (char row = 0; row < 8; row++)
        for (char col = 0; col < 8; col++)
            destruct_piece(board[row][col]);
}

void print_board() {
    for (char row = 7; row >= 0; --row) {
        for (char col = 0; col < 8; ++col) {
            char pos[2];
            pos[0] = (char) (col + 'a');
            pos[1] = (char) (row + '1');
            print_piece(get_piece(pos));
            printf(" ");
        }
        printf("\n");
    }
}