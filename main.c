#include <stdio.h>
#include <stdbool.h>
#include "chess.h"
#include "board.h"

int main() {
    construct_chess();
    do {
        char * move = "pe2e3";
        make_move(move);
        print_board();
    } while(get_status());
    printf("%s", get_final_message());
    destruct_chess();
    return 0;
}
