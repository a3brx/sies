#include <stdbool.h>
#include <stdint.h>

int main() {
    construct_chess();
    while(true){
        char move[6];
        uint8_t result = make_move(move);
        if(result == STALEMATE || result == REPETITION){
            printf("Draw\n");
            break;
        }
        if(result == RESIGN || CHECKMATE){
            printf("%s wins!", get_winner());
            break;
        }
    }
    destruct_chess();
    return 0;
}
