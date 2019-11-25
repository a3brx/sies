#pragma once

enum status {
    PLAYING, CHECKMATE, STALEMATE, REPETITION, RESIGN, TO_BEGIN
};

void print_board();

int make_move(const char *notation);

void construct_game();

void destruct_game();

enum status get_status();

char *get_final_message();