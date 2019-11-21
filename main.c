#include <stdio.h>
#include <stdbool.h>
#include "chess.h"

int main() {
    construct_chess();
    while(true){
        char * move = "pe2e3";
        enum status result = make_move(move);
        if(result == STALEMATE || result == REPETITION){
            printf("Draw\n");
            break;
        }
        if(result == RESIGN || result == CHECKMATE){
            printf("%s wins!\n", get_winner());
            break;
        }
    }
    destruct_chess();
    return 0;
}
