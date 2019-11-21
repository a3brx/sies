#include "chess.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#import <stdbool.h>

/// ---------------------------------------------------------------------------
///                                   Utils
/// ---------------------------------------------------------------------------

void substr(char *to, const char *from, unsigned char chars) {
    for (unsigned char index = 0; index < chars; ++index)
        *(to + index) = *(from + index);
    *(to + chars) = '\0';
}

/// ---------------------------------------------------------------------------
///                              Piece Definition
/// ---------------------------------------------------------------------------
enum color {
    BLACK, WHITE
};

enum type {
    KING = 'K', QUEEN = 'Q', ROOK = 'R', BISHOP = 'B', KNIGHT = 'N', PAWN = 'P'
};

struct piece {
    enum color color;
    enum type type;
};

static struct piece *construct_piece(enum color color, enum type type) {
    struct piece *new_piece = (struct piece *) malloc(sizeof(struct piece));
    new_piece->type = type;
    new_piece->color = color;
    return new_piece;
}

static void destruct_piece(struct piece *piecePtr) {
    free(piecePtr);
}

static void print_piece(const struct piece *piece) {
    if (piece == NULL)
        printf(" ");
    else
        printf("%c", piece->type);
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

static void construct_board() {
    memset(board, 0, 64);
    initialize_color(WHITE);
    initialize_color(BLACK);
}

static void destruct_board() {
    for (char row = 0; row < 8; row++)
        for (char col = 0; col < 8; col++)
            destruct_piece(board[row][col]);
}

static void remove_piece(const struct position *position) {
    destruct_piece(board[position->row][position->col]);
    board[position->row][position->col] = NULL;
}

static void move_piece(const struct position *from, const struct position *to) {
    remove_piece(to);
    board[to->row][to->col] = board[from->row][from->col];
    board[from->row][from->col] = NULL;
}

struct piece *get_piece(const struct position *position) {
    return board[position->row][position->col];
}

void print_board() {
    for (char row = 7; row >= 0; --row) {
        for (char col = 0; col < 8; ++col) {
            struct position pos;
            pos.col = col;
            pos.row = row;
            print_piece(get_piece(&pos));
            printf(" ");
        }
        printf("\n");
    }
}

/// ---------------------------------------------------------------------------
///                               Game Definition
/// ---------------------------------------------------------------------------

static enum status status = TO_BEGIN;
static enum color now_playing = WHITE;

void construct_game() {
    construct_board();
    status = PLAYING;
}

void destruct_game() {
    destruct_board();
}

enum status get_status() {
    return status;
}

bool valid_move(const struct position *from, const struct position *to) {
    return true;
}

int make_move(const char *notation) {
    if (strcmp(notation, "res\n") == 0) {
        status = RESIGN;
        return 0;
    }
    char aux[3];
    substr(aux, notation + 1, 2);
    struct position initial = construct_position(aux);
    substr(aux, notation + 3, 2);
    struct position final = construct_position(aux);
    if (!valid_move(&initial, &final)) return 1;
    move_piece(&initial, &final);
    return 0;
}

char *get_final_message() {
    if (status == RESIGN) {
        if (now_playing == WHITE) {
            return "White resigns the game";
        } else {
            return "Black resigns the game";
        }
    } else {
        return "No message provided";
    }
}