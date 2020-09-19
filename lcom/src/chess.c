#include "chess.h"

/// ---------------------------------------------------------------------------
///                              Utils Definition
/// ---------------------------------------------------------------------------
void substr(char *to, const char *from, unsigned char chars) {
    for (unsigned char index = 0; index < chars; ++index)
        *(to + index) = *(from + index);
    *(to + chars) = '\0';
}

bool contains(char **pos, const char *elem) {
    while (*pos != NULL) {
        if (strcmp(*pos, elem) == 0)
            return true;
        pos++;
    }
    return false;
}

/// ---------------------------------------------------------------------------
///                              Piece Definition
/// ---------------------------------------------------------------------------
static struct piece *construct_piece(enum color color, enum type type) {
    struct piece *new_piece = (struct piece *) malloc(sizeof(struct piece));
    new_piece->type = type;
    new_piece->color = color;
    return new_piece;
}

static void destruct_piece(struct piece *piecePtr) {
    free(piecePtr);
}

/// ---------------------------------------------------------------------------
///                             Position Definition
/// ---------------------------------------------------------------------------
static char *construct_notation(const struct position *position) {
    char *result;
    result = (char *) malloc(3 * sizeof(char));
    result[0] = (char) (position->col + 'a');
    result[1] = (char) (position->row + '1');
    result[2] = '\0';
    return result;
}

static struct position construct_position(const char *notation) {
    struct position ret;
    ret.col = (char) (notation[0] - 'a');
    ret.row = (char) (notation[1] - '1');
    return ret;
}

static bool valid_position(const struct position *position) {
    return position->col >= 0 && position->col < 8 && position->row >= 0 && position->row < 8;
}

/// ---------------------------------------------------------------------------
///                              Board Definition
/// ---------------------------------------------------------------------------
struct piece *board[8][8];
enum castling {
    KINGSIDE, QUEENSIDE, NONE
};
static bool possible_castling[2][2] = {{true, true},
                                       {true, true}};

static void initialize_color(enum color color) {
    short row = (short) ((color + 1) % 2 * 7);
    board[row][0] = construct_piece(color, ROOK);
    board[row][1] = construct_piece(color, KNIGHT);
    board[row][2] = construct_piece(color, BISHOP);
    board[row][3] = construct_piece(color, QUEEN);
    board[row][4] = construct_piece(color, KING);
    board[row][5] = construct_piece(color, BISHOP);
    board[row][6] = construct_piece(color, KNIGHT);
    board[row][7] = construct_piece(color, ROOK);
    row = (short) (row % 2 * 5 + 1);
    for (short col = 0; col < 8; ++col) {
        board[row][col] = construct_piece(color, PAWN);
    }
}

static int construct_board() {
    memset(board, 0, 64);
    initialize_color(WHITE);
    initialize_color(BLACK);
    return 0;
}

static int destruct_board() {
    for (unsigned char row = 0; row < 8; row++)
        for (unsigned char col = 0; col < 8; col++)
            destruct_piece(board[row][col]);
    return 0;
}

static void remove_piece(const struct position *position) {
    destruct_piece(board[position->row][position->col]);
    board[position->row][position->col] = NULL;
}

static void move_piece(const struct position *from, const struct position *to) {
    remove_piece(to);
    board[to->row][to->col] = board[from->row][from->col];
    board[from->row][from->col] = NULL;
    if (board[to->row][to->col]->type == PAWN && (to->row == 7 || to->row == 0)) {
        enum color aux = board[to->row][to->col]->color;
        free(board[to->row][to->col]);
        board[to->row][to->col] = construct_piece(aux, QUEEN);
    }
    if (board[to->row][to->col]->type == KING) {
        possible_castling[board[to->row][to->col]->color][KINGSIDE] = false;
        possible_castling[board[to->row][to->col]->color][QUEENSIDE] = false;
    }
    if ((from->row == 0 || from->row == 7)) {
        if (from->col == 0)
            possible_castling[board[to->row][to->col]->color][QUEENSIDE] = false;
        else if (from->col == 7)
            possible_castling[board[to->row][to->col]->color][KINGSIDE] = false;
    }
}

struct piece *get_piece(const struct position *position, struct piece *bd[8][8]) {
    return bd[position->row][position->col];
}

/// ---------------------------------------------------------------------------
///                               Game Definition
/// ---------------------------------------------------------------------------
static enum status status = TO_BEGIN;
static enum color now_playing = WHITE;

int construct_game() {
    construct_board();
    status = PLAYING;
    return 0;
}

int destruct_game() {
    destruct_board();
    return 0;
}

enum color get_now_playing() {
    return now_playing;
}

enum status get_status() {
    return status;
}

static bool threat_on_position(const struct position *pos, struct piece *bd[8][8]) {
    bool ret = false;
    struct position test_pos;
    // Right
    test_pos.row = pos->row;
    signed char col, row;
    for (col = pos->col + 1; col < 8; col++) {
        test_pos.col = col;
        struct piece *p = get_piece(&test_pos, bd);
        if (p != NULL) {
            if (p->color != now_playing &&
                (p->type == ROOK || p->type == QUEEN || (col == pos->col + 1 && p->type == KING)))
                ret = true;
            break;
        }
    }
    // Left
    for (col = pos->col - 1; col >= 0; col--) {
        test_pos.col = col;
        struct piece *p = get_piece(&test_pos, bd);
        if (p != NULL) {
            if (p->color != now_playing &&
                (p->type == ROOK || p->type == QUEEN || (col == pos->col - 1 && p->type == KING))) {
                ret = true;
            }
            break;
        }
    }
    // Top
    test_pos.col = pos->col;
    for (row = pos->row + 1; row < 8; row++) {
        test_pos.row = row;
        struct piece *p = get_piece(&test_pos, bd);
        if (p != NULL) {
            if (p->color != now_playing &&
                (p->type == ROOK || p->type == QUEEN || (row == pos->row + 1 && p->type == KING)))
                ret = true;
            break;
        }
    }
    // Down
    for (row = pos->row - 1; row >= 0; row--) {
        test_pos.row = row;
        struct piece *p = get_piece(&test_pos, bd);
        if (p != NULL) {
            if (p->color != now_playing &&
                (p->type == ROOK || p->type == QUEEN || (row == pos->row - 1 && p->type == KING)))
                ret = true;
            break;
        }
    }
    // Top Right Diagonal
    for (col = pos->col + 1, row = pos->row + 1; col < 8 && row < 8; col++, row++) {
        test_pos.row = row;
        test_pos.col = col;
        struct piece *p = get_piece(&test_pos, bd);
        if (p != NULL) {
            if (p->color != now_playing && (p->type == BISHOP || p->type == QUEEN || (row == pos->row + 1 &&
                                                                                      (p->type == KING ||
                                                                                       (now_playing == WHITE &&
                                                                                        p->type == PAWN)))))
                ret = true;
            break;
        }
    }
    // Down Right Diagonal
    for (col = pos->col + 1, row = pos->row - 1; col < 8 && row >= 0; col++, row--) {
        test_pos.row = row;
        test_pos.col = col;
        struct piece *p = get_piece(&test_pos, bd);
        if (p != NULL) {
            if (p->color != now_playing && (p->type == BISHOP || p->type == QUEEN || (row == pos->row - 1 &&
                                                                                      (p->type == KING ||
                                                                                       (now_playing == BLACK &&
                                                                                        p->type == PAWN)))))
                ret = true;
            break;
        }
    }
    // Down Left Diagonal
    for (col = pos->col - 1, row = pos->row - 1; col >= 0 && row >= 0; col--, row--) {
        test_pos.row = row;
        test_pos.col = col;
        struct piece *p = get_piece(&test_pos, bd);
        if (p != NULL) {
            if (p->color != now_playing && (p->type == BISHOP || p->type == QUEEN || (row == pos->row - 1 &&
                                                                                      (p->type == KING ||
                                                                                       (now_playing == BLACK &&
                                                                                        p->type == PAWN)))))
                ret = true;
            break;
        }
    }
    // Top Left Diagonal
    for (col = pos->col - 1, row = pos->row + 1; col >= 0 && row < 8; col--, row++) {
        test_pos.row = row;
        test_pos.col = col;
        struct piece *p = get_piece(&test_pos, bd);
        if (p != NULL) {
            if (p->color != now_playing && (p->type == BISHOP || p->type == QUEEN || (row == pos->row + 1 &&
                                                                                      (p->type == KING ||
                                                                                       (now_playing == WHITE &&
                                                                                        p->type == PAWN)))))
                ret = true;
            break;
        }
    }
    // Horse Squares
    for (signed char i = -2; i <= 2; i++) {
        if (i == 0)
            continue;
        for (signed char j = -1; j <= 1; j += 2) {
            test_pos.row = pos->row + i;
            test_pos.col = pos->col + j * (3 - abs(i));
            if (!valid_position(&test_pos))
                continue;
            struct piece *p = get_piece(&test_pos, bd);
            if (p != NULL) {
                if (p->color != now_playing && (p->type == KNIGHT))
                    ret = true;
            }
        }
    }
    return ret;
}

static bool lets_king_in_check(const struct position *from, const struct position *to) {
    if (!valid_position(from) || !valid_position(to))
        return true;
    struct piece *test[8][8];
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++) {
            if (board[i][j] != NULL)
                test[i][j] = construct_piece(board[i][j]->color, board[i][j]->type);
            else
                test[i][j] = NULL;
        }
    free(test[to->row][to->col]);
    test[to->row][to->col] = NULL;
    test[to->row][to->col] = test[from->row][from->col];
    test[from->row][from->col] = NULL;
    struct position pos;
    bool found = false;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            pos.row = i;
            pos.col = j;
            struct piece *p = get_piece(&pos, test);
            if (p != NULL && p->type == KING && p->color == now_playing) {
                found = true;
                break;
            }
        }
        if (found)
            break;
    }
    bool ret = threat_on_position(&pos, test);
    for (unsigned char row = 0; row < 8; row++)
        for (unsigned char col = 0; col < 8; col++) {
            free(test[row][col]);
        }
    return ret;
}

char **get_possible_moves(const struct position *from) {
    static char *result[28];
    memset(result, 0, 28 * sizeof(char *));
    valid_position(from);
    construct_notation(from);
    struct position position;
    unsigned char index = 0;
    unsigned char actual = 0;
    if (!valid_position(from) || get_piece(from, board) == NULL || get_piece(from, board)->color != now_playing)
        return result;
    switch (get_piece(from, board)->type) {
        case KING: {
            struct position to_verify[8];
            for (signed char i = -1; i <= 1; i++)
                for (signed char j = -1; j <= 1; j++) {
                    if (i == 0 && j == 0)
                        continue;
                    position.row = from->row + i;
                    position.col = from->col + j;
                    if (!valid_position(&position))
                        continue;
                    to_verify[index++] = position;
                }
            bool threat = false;
            if (possible_castling[now_playing][KINGSIDE]) {
                position.row = from->row;
                for (position.col = from->col; position.col < from->col + 2; position.col++)
                    if (threat_on_position(&position, board)){
                        threat = true;
                        break;
                    }
                if (!threat)
                    to_verify[index++] = position;
            }
            threat = false;
            if (possible_castling[now_playing][QUEENSIDE]) {
                position.row = from->row;
                for (position.col = from->col; position.col > from->col - 2; position.col--)
                    if (threat_on_position(&position, board)){
                        threat = true;
                        break;
                    }
                if (!threat)
                    to_verify[index++] = position;
            }

            for (unsigned char i = 0; i < index; i++) {
                if (get_piece(&to_verify[i], board) != NULL && get_piece(&to_verify[i], board)->color == now_playing)
                    continue;
                if (lets_king_in_check(from, &to_verify[i]))
                    continue;
                result[actual++] = construct_notation(&to_verify[i]);
            }
            return result;
        }
        case QUEEN: {
            struct position to_verify[27];
            for (signed char k = -1; k <= 1; k += 2)
                for (signed char j = 0; j < 2; j++)
                    for (signed char i = 1; i < 8; i++) {
                        position.row = from->row + (k * i) * (j);
                        position.col = from->col + (k * i) * ((j + 1) % 2);
                        if (!valid_position(&position))
                            break;
                        to_verify[index++] = position;
                        if (get_piece(&position, board) != NULL)
                            break;
                    }
            for (signed char j = -1; j <= 1; j += 2)
                for (signed char k = -1; k <= 1; k += 2)
                    for (signed char i = 1; i < 8; i++) {
                        position.row = from->row + j * i;
                        position.col = from->col + k * j * i;
                        if (!valid_position(&position))
                            break;
                        to_verify[index++] = position;
                        if (get_piece(&position, board) != NULL)
                            break;
                    }
            for (unsigned char i = 0; i < index; i++) {
                if (get_piece(&to_verify[i], board) != NULL && get_piece(&to_verify[i], board)->color == now_playing)
                    continue;
                if (lets_king_in_check(from, &to_verify[i]))
                    continue;
                result[actual++] = construct_notation(&to_verify[i]);
            }
            return result;
        }
        case ROOK: {
            struct position to_verify[14];
            for (signed char k = -1; k <= 1; k += 2)
                for (signed char j = 0; j < 2; j++)
                    for (signed char i = 1; i < 8; i++) {
                        position.row = from->row + (k * i) * (j);
                        position.col = from->col + (k * i) * ((j + 1) % 2);
                        if (!valid_position(&position))
                            break;
                        to_verify[index++] = position;
                        if (get_piece(&position, board) != NULL)
                            break;
                    }
            for (unsigned char i = 0; i < index; i++) {
                if (get_piece(&to_verify[i], board) != NULL && get_piece(&to_verify[i], board)->color == now_playing)
                    continue;
                if (lets_king_in_check(from, &to_verify[i]))
                    continue;
                result[actual++] = construct_notation(&to_verify[i]);
            }
            return result;
        }
        case BISHOP: {
            struct position to_verify[13];
            for (signed char j = -1; j <= 1; j += 2)
                for (signed char k = -1; k <= 1; k += 2)
                    for (signed char i = 1; i < 8; i++) {
                        position.row = from->row + j * i;
                        position.col = from->col + k * j * i;
                        if (!valid_position(&position))
                            break;
                        to_verify[index++] = position;
                        if (get_piece(&position, board) != NULL)
                            break;
                    }
            for (unsigned char i = 0; i < index; i++) {
                if (get_piece(&to_verify[i], board) != NULL && get_piece(&to_verify[i], board)->color == now_playing)
                    continue;
                if (lets_king_in_check(from, &to_verify[i]))
                    continue;
                result[actual++] = construct_notation(&to_verify[i]);
            }
            return result;
        }
        case KNIGHT: {
            struct position to_verify[8];
            for (signed char i = -2; i <= 2; i++) {
                if (i == 0)
                    continue;
                for (signed char j = -1; j <= 1; j += 2) {
                    position.row = from->row + i;
                    position.col = from->col + j * (3 - abs(i));
                    if (valid_position(&position))
                        to_verify[index++] = position;
                }
            }
            for (unsigned char i = 0; i < index; i++) {
                if (get_piece(&to_verify[i], board) != NULL && get_piece(&to_verify[i], board)->color == now_playing)
                    continue;
                if (lets_king_in_check(from, &to_verify[i]))
                    continue;
                result[actual++] = construct_notation(&to_verify[i]);
            }
            return result;
        }
        case PAWN: {
            struct position to_verify[4];
            for (signed char i = -1; i <= 1; i += 2) {
                position.col = from->col + i;
                position.row = from->row + ((now_playing == WHITE) ? 1 : -1);
                if (valid_position(&position))
                    to_verify[index++] = position;
                if (get_piece(&position, board) != NULL)
                    break;
            }
            for (unsigned char i = 1; i <= 2; i++) {
                if (i == 2 && from->row != ((now_playing == WHITE) ? 1 : 6))
                    break;
                position.col = from->col;
                position.row = from->row + ((now_playing == WHITE) ? i : -i);
                if (valid_position(&position))
                    to_verify[index++] = position;
                if (get_piece(&position, board) != NULL)
                    break;
            }
            for (unsigned char i = 0; i < index; i++) {
                if (to_verify[i].col == from->col && get_piece(&to_verify[i], board) != NULL)
                    continue;
                if (to_verify[i].col != from->col &&
                    (get_piece(&to_verify[i], board) == NULL || get_piece(&to_verify[i], board)->color == now_playing))
                    continue;
                if (lets_king_in_check(from, &to_verify[i]))
                    continue;
                result[actual++] = construct_notation(&to_verify[i]);
            }
            return result;
        }
    }
    return result;
}

static enum castling castling(struct position *from, struct position *to) {
    if (get_piece(to, board)->type == KING && abs(from->col - to->col) > 1) {
        return to->col == 6 ? KINGSIDE : QUEENSIDE;
    }
    return NONE;
}

int make_move(const char *notation) {
    if (strcmp(notation, "res\n") == 0) {
        status = RESIGN;
        return 0;
    }
    char aux[3];
    substr(aux, notation, 2);
    struct position initial = construct_position(aux);
    substr(aux, notation + 2, 2);
    char **pm = get_possible_moves(&initial);
    if (!contains(pm, aux)) {
        puts("\nInvalid move!");
        while (*pm != NULL) {
            free(*pm);
            pm++;
        }
        return 1;
    }
    struct position final = construct_position(aux);
    move_piece(&initial, &final);
    enum castling cast = castling(&initial, &final);
    if (cast != NONE) {
        board[now_playing ? 0 : 7][(cast == KINGSIDE) ? 5 : 3] = board[now_playing ? 0 : 7][(cast == KINGSIDE) ? 7 : 0];
        board[now_playing ? 0 : 7][(cast == KINGSIDE) ? 7 : 0] = NULL;
    }
    now_playing = !now_playing;
    struct position king_pos;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++) {
            if (board[i][j] == NULL || board[i][j]->color != now_playing)
                continue;
            struct position pos;
            pos.row = i;
            pos.col = j;
            if (board[i][j]->type == KING) {
                king_pos.row = i;
                king_pos.col = j;
            }
            if (get_possible_moves(&pos)[0] != NULL)
                return 0;
        }
    status = threat_on_position(&king_pos, board) ? CHECKMATE : STALEMATE;
    return 0;
}

int end_time() {
    status = OUT_OF_TIME;
    return 0;
}

int get_final_message_position_x() {
        if (status == RESIGN) {
        if (now_playing == WHITE) {
            return 512 - 11 * 20;
        } else {
            return 512 - 11 * 20;
        }
    } else if (status == CHECKMATE) {
        if (now_playing == WHITE) {
            return 512 - (int)(11.5 * 20);
        } else {
            return 512 - (int)(11.5 * 20);
        }
    } else if (status == OUT_OF_TIME) {
        if (now_playing == WHITE) {
            return 512 - 9 * 20;
        } else {
            return 512 - 9 * 20;
        }
    } else if (status == STALEMATE) {
        return 512 - (int)(7.5 * 20);
    } else {
        return 512 - (int)(11.5 * 20);
    }
}

int get_final_message_size() {
        if (status == RESIGN) {
        if (now_playing == WHITE) {
            return 22;
        } else {
            return 22;
        }
    } else if (status == CHECKMATE) {
        if (now_playing == WHITE) {
            return 23;
        } else {
            return 23;
        }
    } else if (status == OUT_OF_TIME) {
        if (now_playing == WHITE) {
            return 18;
        } else {
            return 18;
        }
    } else if (status == STALEMATE) {
        return 17;
    } else {
        return 23;
    }
}


char *get_final_message() {
    if (status == RESIGN) {
        if (now_playing == WHITE) {
            return "white resigns the game";
        } else {
            return "black resigns the game";
        }
    } else if (status == CHECKMATE) {
        if (now_playing == WHITE) {
            return "black wins by checkmate";
        } else {
            return "white wins by checkmate";
        }
    } else if (status == OUT_OF_TIME) {
        if (now_playing == WHITE) {
            return "black wins by time";
        } else {
            return "white wins by time";
        }
    } else if (status == STALEMATE) {
        return "draw by stalemate";
    } else {
        return "someone exited the game";
    }
}
