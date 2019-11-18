#pragma once

#include <stdlib.h>
#include <stdio.h>

enum type {
    KING = 'K', QUEEN = 'Q', ROOK = 'R', KNIGHT = 'N', BISHOP = 'B', PAWN = 'P', NONE = 0
};
enum color {
    BLACK, WHITE
};

struct piece {
    enum type type;
    enum color color;
};

struct piece *create_piece(enum color color, enum type type);

void print_piece(struct piece * piece);