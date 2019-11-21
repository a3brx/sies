#pragma once

enum status {
    PLAYING, CHECKMATE, STALEMATE, REPETITION, RESIGN
};

void construct_chess();

void destruct_chess();

enum status make_move();

char * get_winner();