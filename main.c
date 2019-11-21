#include <stdbool.h>

int main() {
    construct_chess();
    while(true){
        char move[6];
        if(get_move(move) == RESIGN || check_mate(move))
            break;
        if(valid_move(move)){
            make_move(move);
        }
    }
    destruct_chess();
    return 0;
}
