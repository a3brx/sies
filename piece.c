#include "piece.h"

struct piece *create_piece(enum color color, enum type type) {
    struct piece *to_add = (struct piece *) malloc(sizeof(struct piece));
    to_add->color = color;
    to_add->type = type;
    return to_add;
}

void print_piece(struct piece *piece) {
    if (piece->type) {
        printf("%c ", piece->type);
    } else {
        printf("  ");
    }
}
