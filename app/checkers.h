//
// Created by XxGam on 4/16/2025.
//

#ifndef APP_CHECKERS2_H
#define APP_CHECKERS2_H
#include <vector>
#include "board.h"
#include "minimal_checker.h"

class Checkers : public Board {
private:
    int populate_row(int r, PieceType p);
    int evaluate_board();
    bool is_terminal_node();
    PiecePos* pieceToClick;
    PiecePos* pieceFromClick;
    bool no_legal_moves(PieceType p);
    void setClickedPiece(int r, int c);
    std::array<int, 2> piece_counter;
    int add_place(int r, int c, PieceType p);
    std::list<PiecePos*> pieces;
//    std::pair<int, PiecePos*> pvs(PiecePos *node, int depth, int alpha, int beta, PieceType colour, std::list<MoveDecision*>* moves);
    int colour_scale(PieceType c);
    std::list<std::pair<PiecePos*, PiecePos*>>* generate_moves(PiecePos *p);
    std::array<PieceType, 3> opposing;
    PieceType maybe_add(std::list<std::pair<PiecePos*, PiecePos*>> *l, int r, int c, int rOffset, int cOffset, PiecePos *p);
//    void find_best_move(PieceType pt);
    MoveBackup apply_move(PiecePos* node, PiecePos* move, PiecePos* captured);
    void undo_move(PiecePos* node, PiecePos* move, const MoveBackup& backup);
    std::list<PiecePos*>* clone_pieces();
    void clone_pieces(std::list<PiecePos*>* source);
    PiecePos* pseudo_piece_position(int r, int c, std::list<PiecePos*> *ps);
public:
    explicit Checkers(QWidget *parent=nullptr);
    PiecePos* get_piece_position(int r, int c);
    PieceType next_colour(PieceType p);
    void print_peices();
    void turns(int t, PieceType pt);
    int populate_board(bool pVsAi);
    void toggle_space_connection(bool status);
    MoveDecision* BestMove(PieceType pt);
    int MinMax(int recursionLevel, bool isMaximizing, PieceType pt, minimal_checker c);
};
#endif //APP_CHECKERS2_H
