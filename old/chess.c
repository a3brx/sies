#include "chess.h"
#include "board.h"
#include <stdbool.h>

struct move{
    struct piece piece;
    char initial[3];
    char final[3];
};

static enum color now_playing = WHITE;

int get_move(struct move * move){
  char *args;
  unsigned long size = 5;
  args = (char *) malloc (size * sizeof(char));
  getline(&args, &size, stdin);
  move->piece.type = args[0];
  move->piece.color = now_playing;
  move->initial[0] = args[1];
  move->initial[1] = args[2];
  move->initial[2] = '\0';
  move->final[0] = args[3];
  move->final[1] = args[4];
  move->final[2] = '\0';
  printf("type: %c initial: %s final: %s\n", move->piece.type, move->initial, move->final);
  return 0;
}

bool game_over(){
    return false;
}

void construct_chess(){
    construct_board();
}

void destruct_chess(){
    destruct_board();
}

void play(){
    print_board();
    while(!game_over()){
        struct move actual_move;
        if(get_move(&actual_move))
            break;
        if(valid_move(&actual_move)){
            make_move(actual_move.initial, actual_move.final);
        } else {
            continue;
        }
        print_board();
    }
}
