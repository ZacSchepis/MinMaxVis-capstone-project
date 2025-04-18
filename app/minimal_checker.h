//
// Created by XxGam on 4/17/2025.
//

#ifndef APP_MINIMAL_CHECKER_H
#define APP_MINIMAL_CHECKER_H
#include "board.h"
struct PiecePos {
    int row;
    int col;
    PieceType piece;
    bool is_king = false;
    bool directionUp = false;
};

struct MoveDecision {
    PiecePos* pOrigin;
    PiecePos* pEnd;
    PiecePos* pCapture;
    int score;
};
struct MoveBackup {
    int prev_row;
    int prev_col;
    PiecePos* captured;
};
class minimal_checker {
private:
    std::array<PieceType, 3> opposing;
public:
    minimal_checker();

    bool is_terminal_node();

    MoveBackup apply_move(PiecePos *node, PiecePos *move, PiecePos *captured, Board *b);

    void undo_move(PiecePos *node, PiecePos *move, const MoveBackup &backup, Board *b);

    bool no_legal_moves(PieceType p);

    std::array<int, 2> piece_counter;

    std::list<PiecePos *> *clone_pieces();

    std::list<PiecePos *> pieces;

    std::list<std::pair<PiecePos *, PiecePos *>> *generate_moves(PiecePos *p);

    PieceType
    maybe_add(std::list<std::pair<PiecePos *, PiecePos *>> *l, int r, int c, int rOffset, int cOffset, PiecePos *p);

    PiecePos *get_piece_position(int r, int c);

    int evaluate_board();
};
static bool should_skip(PiecePos *p, int offset) {
    if(p->is_king) return false;
    if((offset > 0 && p->piece == P1) || (offset < 0 && p->piece == P2)) return true;
    return false;
}

#endif //APP_MINIMAL_CHECKER_H
