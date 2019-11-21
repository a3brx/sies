#include "chess.h"
#include "board.h"
#include <stdio.h>
#include <string.h>

/**
 *  Static Variables
 */
static enum status status = TO_BEGIN;
static enum color now_playing = WHITE;

/**
 *  Private Methods
 */

void construct_chess() {
    construct_board();
    status = PLAYING;
}

void destruct_chess() {
    destruct_board();
}

enum status get_status() {
    return status;
}

void make_move() {

}

char *get_final_message() {
    return "Not a message";
}