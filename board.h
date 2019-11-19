#pragma once

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "piece.h"

static struct piece *board[8][8];

void construct_board();
void destruct_board();
void print_board();
