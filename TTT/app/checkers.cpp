#include "checkers.h"
//#include <QTime.h>
#include <QTimer>
// Dont worry too much about the board styles
// Im gonna look more into that later to give them
// better themes
#include <iostream>
#include <algorithm>
#include <random>
Checkers::Checkers(QWidget *parent) : Board(parent, 8,8) {
    this->map_piece(P2, ":/res/red_checker32px.png");
    this->map_piece(P1, ":/res/black_checker32px.png");
//    this->set_styles("QGridLayout::item.alternate {background-color: red;}");
//    this->setStyle(this->style());
//    this->stack_top = -1;
    this->piece_counter[P1-1] = 0;
    this->piece_counter[P2-1] = 0;
    this->p1_turn = true;
//    this->piece_stack
//    this->piece_counter[empty_state] =
}

int Checkers::populate_board() {
    int startIdx = 0;
//    this->stack_top = 0;
    int t=0; int b = this->get_size(1)-1;
    bool alter = true;
    while(t+1 < b-1){

        for(int c=0; c<this->get_size(1)-1; c+=2){
            this->add_place(b, alter ? c : c+1, P1);
            this->add_place(t, alter ? c+1 : c, P2);
            this->piece_counter[P1-1]++;
            this->piece_counter[P2-1]++;
        }
        t++;
        b--;
        alter = !alter;
    }
    return 1;
}
int Checkers::get_piece_count(PieceType p) {
    if(p <= empty_state || p >= P2) return -1;
    return this->piece_counter[p-1];
}
void Checkers::add_place(int row, int col, PieceType p) {
    if(p == error_state){
        return;
    }
    PiecePos pos;
    pos.row = row;
    pos.col = col;
    pos.piece = p;
    int place_res = this->place(row, col, p);
    if(place_res == -1) return;
    if(p == empty_state){
        return;
    }
//    std::cout << "Adding: (" << pos.row << ", "<< pos.col << "). P: " << pos.piece << " to stack." << std::endl;
    this->piece_stack.insert(piece_stack.begin(), pos);
}
//PiecePos Checkers::pop_piece() {
//    if(this->stack_top == -1){
//        PiecePos err;
//        err.piece = error_state;
//        return err;
//    }
//    return this->piece_stack[this->stack_top--];
//}
//void Checkers::unpop(){
//    this->stack_top++;
//}
bool Checkers::is_p1turn() {return this->p1_turn;}
/* Checkers Rules
 * 12 pieces each, on every other dark squared and staggered by rows
 * Black/Red or Red/White
 * Diagonaly on the forward direction (towards opponenent)
 * If opponenent 1 space away (1,1) -> (2,2), and there is an empty space (3,3)
 * You must take the piece
 *
 * */

bool Checkers::can_place_at(int row, int col) {
    return (row >= 0 && row < this->get_size(1)) && (col >= 0 && col < this->get_size(1));
}

int Checkers::check_move(PieceType p, int row, int col) {
    PieceType p_at = this->get_piece_at(row, col);
    if(p_at == error_state) return 0;
    return 1;


}
PiecePos Checkers::get_piece_at_pos(int row, int col) {
//    PiecePos p_at;
    PieceType p = this->get_piece_at(row, col);
    struct PiecePos p_at = {
            row, col, p
    };
    return p_at;
}
void Checkers::turns(int t){
//    int c = t;
    if(t == 0) return;
    QTimer::singleShot(1000, this, [=](){
        repaint();
        if(this->take_turn() == 10) return;
        repaint();
        this->turns(t-1);
    });
}
void Checkers::moving_unset(PiecePos p_old, PiecePos p_new) {
    if(p_new.piece!= error_state && p_old.piece != error_state) {
        this->add_place(p_new.row, p_new.col, p_old.piece);
        this->add_place(p_old.row, p_old.col, empty_state);
//        std::cout << "Moved: " << p_old.row << "," << p_old.col << "," << p_old.piece << std::endl;
//        std::cout << "To: " << p_new.row << "," << p_new.col << std::endl;
    }
}
bool Checkers::can_capture(PiecePos from, PiecePos to, int offset) {
    if(get_piece_at(to.row+offset, to.col+offset) == empty_state && from.piece != to.piece && to.piece != empty_state){
        return true;
    }
    return false;
}
//void Checkers::maybe_capture(PiecePos from, PiecePos to){
//    PiecePos
//}

int Checkers::take_turn() {
    std::random_device rd;
    std::mt19937 g(rd());
    PiecePos p;
    std::cout << "Piece stats: [P1: " << piece_counter[P1-1] << ", P2: " << piece_counter[P2-1] <<"]. Que: ["<< piece_stack.size()<< std::endl;
    for(int i=0; i<this->get_size(1); i++){
        for(int c=0; c<this->get_size(1);c++){
            std::cout<<this->get_piece_at(i,c) << ", ";
        }
    }
    std::cout << "]Stack: " << std::endl;
    for(PiecePos &_p : piece_stack){
        std::cout << _p.piece << ", ";
    }
    std::cout<<std::endl;
//    if(piece_counter[P1] <= 0 || piece_counter[])
    if(piece_stack.empty()) return 5;
    bool ms_cavorkeans_dog = false;
    std::shuffle(this->piece_stack.begin(), this->piece_stack.end(), g);
    p = piece_stack.back();
    piece_stack.pop_back();
    int row_offset = 0;
    if(p.piece == P1)row_offset = -1;
    else if(p.piece == P2) row_offset = 1;
    else{ std::cout << "early return :( " << p.piece<< std::endl; return 1;}
    PiecePos left = get_piece_at_pos(p.row+row_offset, p.col-1);
    PiecePos right = get_piece_at_pos(p.row+row_offset, p.col+1);
    // capture left piece
    if(can_capture(p, left, row_offset)){
        place(left.row, left.col, empty_state);
        PiecePos next = get_piece_at_pos(p.row+(row_offset*2), p.col-2);
        piece_counter[p.piece-1]--;
        moving_unset(left, next);
        std::cout << "Capture on the left!!!!!By: P" << p.piece << std::endl;
        return 1;
    // capture right piece
    } else if(can_capture(p, right, row_offset)) {
        place(right.row, right.col, empty_state);
        PiecePos next = get_piece_at_pos(p.row+(row_offset*2), p.col+2);
        if(next.piece == p.piece) return 1;

        piece_counter[p.piece-1]--;
        moving_unset(left, next);
        std::cout << "Capture on the right!!!!!By: P" << p.piece << std::endl;
        return 1;
    } else {
        if(left.piece == empty_state){
            moving_unset(p,left);
        } else if(right.piece == empty_state){
            moving_unset(p, right);
        } else {
            piece_stack.insert(piece_stack.begin(), p);
            return 2;
        }
    }
    return 0;
}


//void Checkers::paintEvent(QPaintEvent *){
//    QTime time = QTime::currentTime();
//}
//void Checkers::handleButton(int row, int col) {
//
//}