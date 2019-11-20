#include "chess.h"

static enum color now_playing = WHITE;

struct move get_move(){
  char args[5];
  scanf("%s", args);
  struct move move;
  move.piece.type = args[0];
  move.piece.color = now_playing;
  move.initial[0] = args[1];
  move.initial[1] = args[2];
  move.final[0] = args[3];
  move.final[1] = args[4];
  printf("%c %s %s\n", move.piece.type, move.initial, move.final);
  return move;
}

void play(){
    construct_board();
    print_board();
    get_move();
    destruct_board();
}
