#ifndef CHECKERS_H
#define CHECKERS_H
#include <vector>
#include "board.h"
struct PiecePos {
    int row;
    int col;
    PieceType piece;
    bool is_king = false;
    bool directionUp = false;
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
    int evaluate_board();
//    PiecePos pop_piece();
//    void update_piece_counter
    bool is_terminal_node();
    bool left_right_check(PiecePos p);
    bool no_legal_moves(PieceType p);
    void remove_place(PiecePos p, PieceType replace_with);
    std::vector<PiecePos> generate_moves(PiecePos start);
    PiecePos pieceToClick;
    PiecePos pieceFromClick;
    void setClickedPiece(int r, int c);
    void toggle_space_connection(bool status);
public:
    explicit Checkers(QWidget *parent=nullptr);
    int populate_board(bool pVsAi);
    PiecePos get_piece_at_pos(int row, int col);
    bool is_p1turn();
    void add_place(int row, int col, PieceType p, PiecePos config);
    bool can_place_at(int row, int col);
    int check_move(PieceType p, int row, int col);
    bool validate_player_move();
    bool take_player_move();
    void turns(int t);
//    PiecePos can_capture(PiecePos from, PiecePos to, bool intermediate, int row_offset, int col_offset);
    PiecePos* king_me(PiecePos p);
    bool can_capture(PiecePos from, PiecePos to, int row_offset, int col_offset);
    void moving_unset(PiecePos p_old, PiecePos p_new); 
    std::pair<int, PiecePos> pvs(PiecePos node, int depth, int alpha, int beta, int colour);
    void find_best_move();
    std::vector<PiecePos>::iterator find_piece(int r, int c);
    PieceType wipe_space(PiecePos p);


//private slots:
//    void handleButton() override;
};

#endif // CHECKERS_H
