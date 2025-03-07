#ifndef CHECKERS_H
#define CHECKERS_H
#include <vector>
#include "board.h"
struct PiecePos {
    int row;
    int col;
    PieceType piece;
};

class Checkers : public Board
{

private:
    int populate_row(int r, PieceType p);
    int piece_counter[2];
    bool p1_turn;
    bool can_take();
    std::vector<PiecePos> piece_stack;
//    PiecePos piece_stack[24];
//    int stack_top;
//    void unpop();
//    PiecePos pop_piece();
//    void update_piece_counter
public:
    explicit Checkers(QWidget *parent=nullptr);
    int populate_board();
    int get_piece_count(PieceType p);
    PiecePos get_piece_at_pos(int row, int col);
//    PieceType overtake();
    bool is_p1turn();
    void add_place(int row, int col, PieceType p);
    int take_turn();
    bool can_place_at(int row, int col);
    int check_move(PieceType p, int row, int col);
    void turns(int t);
    bool can_capture(PiecePos from, PiecePos to, int offset);
    void moving_unset(PiecePos p_old, PiecePos p_new);
//private slots:
//    void handleButton() override;
};

#endif // CHECKERS_H
