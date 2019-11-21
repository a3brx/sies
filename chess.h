#pragma once

enum status {
    PLAYING, CHECKMATE, STALEMATE, REPETITION, RESIGN, TO_BEGIN
};

void construct_chess();

void destruct_chess();

void make_move();

enum status get_status();

char *get_final_message();