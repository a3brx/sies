#pragma once

#include "piece.h"

static struct piece *board[8][8];

void construct_board();

void destruct_board();

void print_board();

void make_move(const char *from, const char *to);