#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chess/chess.h"

int main() {
    construct_game();
    print_board();
    do {
        char move[7];
        fgets(move, sizeof(move), stdin);
        make_move(move);
        print_board();
    } while (!get_status());
    printf("%s\n", get_final_message());
    destruct_game();
    return 0;
}
