#ifndef _CHESS_H_
#define _CHESS_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief A square of the board (has a column and a row)
 */
struct position {
    unsigned char col;
    unsigned char row;
};

/**
 * @brief One of the two colors of pieces in chess
 */
enum color {
    BLACK, WHITE
};

/**
 * @brief One of the siz types of pieces in chess
 */
enum type {
    KING = 'K', QUEEN = 'Q', ROOK = 'R', BISHOP = 'B', KNIGHT = 'N', PAWN = 'P'
};

/**
 * @brief A piece in chess has a type and a color
 */
struct piece {
    enum color color;
    enum type type;
};

/**
 * @brief All the possible states of the gane
 */
enum status {
    PLAYING, CHECKMATE, STALEMATE, REPETITION, RESIGN, TO_BEGIN, OUT_OF_TIME
};

/*
 * @brief Makes a move on the board if possible
 * @return Returns 0 if the move was made and 1 otherwise
 */
int make_move(const char *notation);

/*
 * @brief Constructs a board with allocated pieces in normal initial positions of chess
 * @return Returns 0 if there was no error
 */
int construct_game();

/*
 * @brief Frees all the memory of the pieces
 * @return Returns 0 if there was no error
 */
int destruct_game();

/*
 * @brief Gets a piece in a given position on a given board
 * @param position The selected square
 * @param bd The board to be analyzed
 * @return Returns a pointer to the piece
 */
struct piece *get_piece(const struct position *position, struct piece *bd[8][8]);

/*
 * @return Returns the color of the current player
 */
enum color get_now_playing();

/*
 * @return Returns the status of the game
 */
enum status get_status();

/*
 * @brief Gets the valid moves of a piece in a given position
 * @param from The position of the piece to be analyzed
 * @return Returns an array of chars where each char represents a valid move
 */
char **get_possible_moves(const struct position *from);

/*
 * @brief Changes the status of the game to indicate that the game ended by end of time
 * @return Returns 0 if there was no error
 */
int end_time();

/*
 * @return Returns the size of the final message
 */
int get_final_message_size();

/*
 * @return Returns the position of the final message
 */
int get_final_message_position_x();

/*
 * @return Returns the final message that says how the game ended
 */
char *get_final_message();

#endif


