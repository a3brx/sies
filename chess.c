#include "chess.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#import <stdbool.h>

/// ---------------------------------------------------------------------------
///                                   Utils
/// ---------------------------------------------------------------------------
static void substr(char *to, const char *from, unsigned char chars) {
    for (unsigned char index = 0; index < chars; ++index)
        *(to + index) = *(from + index);
    *(to + chars) = '\0';
}

static bool contains(char **pos, const char *elem) {
    while (*pos != NULL) {
        if (strcmp(*pos, elem) == 0)
            return true;
        pos++;
    }
    return false;
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
    unsigned char col;
    unsigned char row;
};

static char *construct_notation(const struct position *position) {
    static char result[3];
    result[0] = (char) (position->col + 'a');
    result[1] = (char) (position->row + '1');
    result[2] = '\0';
    return result;
}

static struct position construct_position(const char *notation) {
    struct position ret;
    ret.col = (char) (notation[0] - 'a');
    ret.row = (char) (notation[1] - '1');
    return ret;
}

static bool valid_position(const struct position *position) {
    return position->col >= 0 && position->col < 8 && position->row >= 0 && position->row < 8;
};

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

static bool lets_king_in_check(const struct position *from, const struct position *to) {
    return false;
}

static char **get_possible_moves(const struct position *from) {
    static char *result[28];
    char actual = 0;
    memset(result, 0, 28);
    if (!valid_position(from) || get_piece(from) == NULL || get_piece(from)->color != now_playing)
        return result;
    switch (get_piece(from)->type) {
        case KING:
            break;
        case QUEEN:
            break;
        case ROOK:
            break;
        case BISHOP:
            break;
        case KNIGHT:
            break;
        case PAWN: {
            struct position position;
            struct position to_verify[4];
            char index = 0;
            for (char i = -1; i < 2; i++) {
                position.col = from->col + i;
                position.row = from->row + ((now_playing == WHITE) ? 1 : -1);
                if (valid_position(&position))
                    to_verify[index++] = position;
            }
            for (char i = 0; i < index; i++) {
                if (to_verify[i].col == from->col && get_piece(&to_verify[i]) != NULL)
                    continue;
                if (to_verify[i].col != from->col &&
                    (get_piece(&to_verify[i]) == NULL || get_piece(&to_verify[i])->color == now_playing))
                    continue;
                if (lets_king_in_check(from, &position))
                    continue;
                result[actual++] = construct_notation(&to_verify[i]);
            }
            return result;
        }
    }
}

static bool threat_on_position(const struct position *pos, const enum color color) {
    struct position aux;
    char **poss_moves;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            aux.row = i;
            aux.col = j;
            if (get_piece(&aux) == NULL)
                continue;
            else if (get_piece(&aux)->color != color) {
                poss_moves = get_possible_moves(&aux);
                if (contains(poss_moves, pos))
                    return true;
            }
        }
    }
    return false;
}

static bool check_check(enum color color) {
//    struct position king_pos, aux;
//    struct position * pos = board;
//    char step = 1;
//    if (!color) {
//        step = -1;
//        pos += 63;
//    }
//    for (int i = 0; i < 64; pos--, i++) {
//        pos += step;
//        aux.col = pos->col;
//        aux.row = pos->row;
//        if (get_piece(&aux) != NULL && get_piece(&aux)->type == KING) {
//            king_pos.col = pos->col;
//            king_pos.row = pos->row;
//        }
//    }
//    char **poss_moves;
//    for (int i = 0; i < 8; i++) {
//        for (int j = 0; j < 8; j++) {
//            aux.row = i;
//            aux.col = j;
//            poss_moves = get_possible_moves(&aux);
//            if (contains(poss_moves, ))
//        }
//    }
    return false;
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
    if (!contains(get_possible_moves(&initial), aux)) {
        printf("Invalid move!\n");
        return 1;
    }
    struct position final = construct_position(aux);
    move_piece(&initial, &final);
    now_playing = !now_playing;
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