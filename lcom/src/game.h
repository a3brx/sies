#pragma once

#include "chess.h"

/*
 * @brief Draws the game board.
 * @return 0 if success, 1 otherwise.
 */
int draw_board();

/*
 * @brief Updates both players' time decrementing them by one depending on whose turn it is.
 */
void update_player_timer();

/*
 * @brief Writes both players' time on screen.
 */
void draw_players_time();

/*
 * @brief Checks if player 1 time has run out.
 * @return Returns true if the time has run out, false otherwise.
 */
bool p1_time_finished();

/*
 * @brief Checks if player 2 time has run out.
 * @return Returns true if the time has run out, false otherwise.
 */
bool p2_time_finished();

/*
 * @brief Draws blindfold base, the two boxes and "your move:" and "their move:".
 */
void draw_blindfold();

/*
 * @brief Processes writing input in "blindfold" mode.
 * @param c Char to process.
 */
void input_char_to_move(char c);

