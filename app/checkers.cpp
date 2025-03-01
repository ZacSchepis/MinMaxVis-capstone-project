#include "checkers.h"
//#include <QTime.h>
#include <QTimer>
#include <QPushButton>
// Dont worry too much about the board styles
// Im gonna look more into that later to give them
// better themes
#include <iostream>
#include <algorithm>
#include <random>
struct PiecePos DEFAULT_CONFIG = {
       -1, -1, error_state
};

Checkers::Checkers(QWidget *parent) : Board(parent, 8,8) {
    this->map_piece(P2, ":/res/red_checker32px.png");
    this->map_piece(P1, ":/res/black_checker32px.png");
//    this->set_styles("QGridLayout::item.alternate {background-color: red;}");
//    this->setStyle(this->style());
//    this->stack_top = -1;
    PiecePos initialClickStates = {-1, -1, error_state, false, false};
    pieceFromClick = initialClickStates;
    pieceToClick = initialClickStates;
    this->piece_counter[P1-1] = 0;
    this->piece_counter[P2-1] = 0;
    this->p1_turn = true;
//    this->piece_stack
//    this->piece_counter[empty_state] =
}

void Checkers::setClickedPiece(int r, int c){
    PiecePos initialClickStates = {-1, -1, error_state, false, false};

    if(pieceFromClick.piece == error_state && pieceToClick.piece == error_state ){
        pieceFromClick = get_piece_at_pos(r, c);
        std::cout << "Set from: P" <<pieceFromClick.piece <<" (" << pieceFromClick.row << ", " << pieceFromClick.col <<")" << std::endl;
    } else if(pieceFromClick.piece != error_state && pieceToClick.piece == error_state){
        pieceToClick = get_piece_at_pos(r, c);
        std::cout << "Set to: P" <<pieceToClick.piece <<" (" << pieceToClick.row << ", " << pieceToClick.col <<")" << std::endl;
        if(validate_player_move()){
            if(!take_player_move()){
                std::cout << "Invalid move !!!" << std::endl;
            }
            repaint();
        }
        pieceToClick = initialClickStates;
        pieceFromClick = initialClickStates;
        std::cout << "Reset both pieces after making move" << std::endl;

    } else {
        pieceToClick = initialClickStates;
        pieceFromClick = initialClickStates;
        std::cout << "Reset both pieces" << std::endl;

    }
}
void Checkers::toggle_space_connection(bool status){
    for(int r=0; r< get_size(1); r++){
        for(int c=0; c<get_size(1); c++){
            auto* button_at = qobject_cast<QPushButton*>(grid->itemAtPosition(r, c)->widget());
            if(status){
                connect(button_at, &QPushButton::clicked, [this, r, c]() {
                    setClickedPiece(r, c);
                });
            } else {
                disconnect(button_at, &QPushButton::clicked, nullptr, nullptr);
            }
        }
    }
}
int Checkers::populate_board(bool pVsAi) {
    int t=0; int b = this->get_size(1)-1;
    bool alter = true;
    while(t+1 < b-1){

        for(int c=0; c<this->get_size(1)-1; c+=2){
            this->add_place(b, alter ? c : c+1, P1, DEFAULT_CONFIG);
            this->add_place(t, alter ? c+1 : c, P2, DEFAULT_CONFIG);
            this->piece_counter[P1-1]++;
            this->piece_counter[P2-1]++;
        }
        t++;
        b--;
        alter = !alter;
    }
    toggle_space_connection(pVsAi);
    return 1;
}




bool Checkers::validate_player_move() {
    if(pieceFromClick.piece < P1){
        std::cout << "Not a player?" << std::endl;
        return false;
    }
    bool shouldGoUp = pieceFromClick.piece == P1;
    for(const auto possible_move : generate_moves(pieceFromClick)){
        if(!can_place_at(possible_move.row, possible_move.col) || possible_move.piece != pieceToClick.piece){
            continue;
        }
        if(possible_move.row == pieceToClick.row && possible_move.col == pieceToClick.col){
            if(shouldGoUp == possible_move.directionUp || pieceFromClick.is_king){
                return true;
            }
        }
    }
    return false;
}
bool Checkers::take_player_move() {
    int row_offset = pieceToClick.row - pieceFromClick.row;
    int col_offset = pieceToClick.col - pieceFromClick.col;
    std::cout << "Offsets: [" << row_offset << ", " << col_offset << "]" << std::endl;
    if(std::abs(row_offset) == 2 && std::abs(col_offset) == 2){
        PiecePos offset_piece = get_piece_at_pos(pieceFromClick.row+(row_offset/2), pieceFromClick.col+(col_offset/2));
        if(can_capture(pieceFromClick, offset_piece, row_offset/2, col_offset/2)) {
            remove_place(offset_piece, empty_state);
//            pieceToClick.piece = pieceFromClick.piece;
            moving_unset(pieceFromClick, pieceToClick);
            return true;
        }
    } else if(std::abs(row_offset) == 1 && std::abs(col_offset) == 1){
        if(pieceToClick.piece == empty_state){
//            pieceToClick.piece = pieceFromClick.piece;
            moving_unset(pieceFromClick, pieceToClick);
            return true;
        }
    }
    return false;
}

//int Checkers::get_piece_count(PieceType p) {
//    if(p <= empty_state || p >= P2) return -1;
//    return this->piece_counter[p-1];
//}
void Checkers::add_place(int row, int col, PieceType p, PiecePos config) {
    if(p == error_state){
        return;
    }
    PiecePos pos;
    pos.row = row;
    pos.col = col;
    pos.piece = p;
    if(config.is_king){
        std::cout << "!!!We got a king bois!!!" << std::endl;
        pos.is_king = true;

    }
//    pos.is_king = config.is_king ? true : ;
//    if()
    int place_res = this->place(row, col, p);
    if(place_res == -1) return;
    if(p == empty_state){
        return;
    }
    this->piece_stack.push_back(pos);
    king_me(pos);

}



//bool Checkers::is_p1turn() {return this->p1_turn;}
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


bool Checkers::left_right_check(PiecePos p) {
    int row_offset;
    if (p.piece == P1) row_offset = -1;
    else if (p.piece == P2) row_offset = 1;
    else { return false; }
    PiecePos l =this->get_piece_at_pos(p.row+row_offset, p.col-1);
    PiecePos r = this->get_piece_at_pos(p.row+row_offset, p.col+1);
    if(l.piece == empty_state || this->can_capture(p, l, row_offset, -1)){
        return true;
    }
    if(r.piece == empty_state || this->can_capture(p, r, row_offset, 1)){
        return true;
    }
    PiecePos rr = this->get_piece_at_pos(p.row+(row_offset*2), p.col+2);
    if(r.piece != empty_state && r.piece != p.piece && rr.piece == empty_state){
        return true;
    }
    PiecePos ll = this->get_piece_at_pos(p.row+(row_offset*2), p.col-2);
    if(l.piece != empty_state && l.piece != p.piece && ll.piece == empty_state){
        return true;
    }
    return false;


}
bool Checkers::no_legal_moves(PieceType p) {
    for(const auto pt : piece_stack){
        if(pt.piece != p) continue;
        if(!left_right_check(pt)) return false;
    }
    return true;
}
bool Checkers::is_terminal_node() {
    return (piece_counter[P1 - 1] == 0 || piece_counter[P2 - 1] == 0) || no_legal_moves(P1) || no_legal_moves(P2);
}

PiecePos Checkers::get_piece_at_pos(int row, int col) {
    PieceType p = this->get_piece_at(row, col);
//    PiecePos p_at;
    auto it = find_piece(row, col);
    if(it != piece_stack.end()){
        std::cout << " Is king? "<<it->is_king << std::endl;
//        p_at = *it;
        return (*it);
    } else {
        struct PiecePos p_at = {row, col, p};
        return p_at;
//        p_at = {row, col, p};
    }
//    return p_at;
}
void Checkers::turns(int t){
    if(t == 0) return;
    QTimer::singleShot(1000, this, [=](){
        repaint();
//        if(this->take_turn() == 10) return;
        find_best_move();
        repaint();
        this->turns(t-1);
    });
}

// Find a piece in piece_stack at the given position
std::vector<PiecePos>::iterator Checkers::find_piece(int row, int col) {
    return std::find_if(piece_stack.begin(), piece_stack.end(),
                        [row, col](const PiecePos& piece) {
                            return piece.row == row && piece.col == col;
                        });
}
void Checkers::moving_unset(PiecePos p_old, PiecePos p_new) {
    if(p_new.piece!= error_state && p_old.piece != error_state) {
        this->add_place(p_old.row, p_old.col, empty_state, DEFAULT_CONFIG);
//        PiecePos possible_king = *this->king_me(p_new);
        this->wipe_space(p_old);
        p_new.piece = p_old.piece;
        this->add_place(p_new.row, p_new.col, p_old.piece, p_old);
    }
}
PieceType Checkers::wipe_space(PiecePos p){
    auto it = find_piece(p.row, p.col);
    if(it != piece_stack.end()){
        piece_stack.erase(it);
        return it->piece;
    }
    return empty_state;
}
void Checkers::remove_place(PiecePos p, PieceType replace_with){
    bool found = false;
//    auto it = find_piece(p.row, p.col);
//    if(it != piece_stack.end()){
//        if(it->piece > empty_state){
//            piece_counter[it->piece-1]--;
//        }
//        piece_stack.erase(it);
//        place(p.row,p.col, replace_with);
//    }
    PieceType p_ = wipe_space(p);
    if(p_ > empty_state){
        piece_counter[p_-1]--;
        place(p.row, p.col, empty_state);
    }
//    for(auto it = piece_stack.begin(); it != piece_stack.end(); ++it){
//        if(it->row == p.row && it->col == p.col){
//            if(it->piece > empty_state){
//                piece_counter[it->piece-1]--;
//            }
//            piece_stack.erase(it);
//            found = true;
//            break;
//        }
//    }
//    if(found){
//        place(p.row,p.col, replace_with);
//
//    }
}
bool Checkers::can_capture(PiecePos from, PiecePos to, int row_offset, int col_offset) {
    std::cout << "Got offsets: [" << row_offset << ", " << col_offset << "]";
    if(get_piece_at(to.row+row_offset, to.col+col_offset) == empty_state && from.piece != to.piece && to.piece != empty_state){
        return true;
    }
    //this->can_capture(to, get_piece_at_pos(to.row+offset, to.col+offset), offset);
    return false;
}


PiecePos* Checkers::king_me(PiecePos p){
    auto it = std::find_if(piece_stack.begin(), piece_stack.end(), [p](PiecePos p2) {
        return (p.row == p2.row) && (p.col == p2.col);
    });
    if(it != piece_stack.end() && !it->is_king){
        int row = -1;
        switch(it->piece){
            case P2: row = this->get_size(1)-1; break;
            case P1: row = 0; break;
            default: break;
        }
        std::cout << " Check row/matchrow: " << row << "/"<< it->row;
        if(row != -1 && it->row == row){
            it->is_king = true;
            std::cout << "King!" << std::endl;
            return &(*it);
        }
        std::cout << "rows didnt match, or not a player piece. ";
    }
    std::cout << "You are not, a millionare :(";

    return &DEFAULT_CONFIG;
}


std::vector<PiecePos> Checkers::generate_moves(PiecePos start){
    std::vector<PiecePos> moves;
    PiecePos err_move;
    err_move.piece = error_state;
    err_move.row = -1;
    err_move.col = -1;
    int row_min = std::min(start.row+2, this->get_size(1)-1);
    std::cout << "Row max/min " << start.row-2 << "/" << row_min << std::endl;
    int count = 2;
    bool isUp = true;
    for(int row_ = start.row-2; row_ <= row_min; row_++){
        if(row_ +1< 0 || row_ >= this->get_size(1)){
            continue;
        }
        if(row_ == start.row){
            count = 1;
            isUp = false;
            continue;
        }
        else {
            PiecePos left = get_piece_at_pos(row_, (start.col+count));
            std::cout << "R: " << left.row << ", " << left.col <<", Count: +"<<count << std::endl;
            left.directionUp = isUp;
            PiecePos right = get_piece_at_pos(row_, std::abs(start.col+(-count)));
            right.directionUp = isUp;
            moves.push_back(left);
            moves.push_back(right);
            count = count-1 == 0 ? 2 : count -1;
        }
    }
    for (const auto& move : moves) {
        std::cout << "Generated move: (" << move.row << ", " << move.col << ") Direction: "
                  << (move.directionUp ? "Up" : "Down") << " Piece: " << move.piece << std::endl;
    }
    return moves;
}

int Checkers::evaluate_board(){
    int score = 0;
    for(const auto& piece: piece_stack){
        int piece_value = piece.is_king ? 3 : 1;
        int position_bonus = (piece.col > 1 && piece.col < get_size(1) -2) ? 1 : 0;
        int row_bonus = (piece.piece == P1) ? piece.row : (get_size(1)-1-piece.row);
        if(piece.piece == P1){
            score += piece_value + position_bonus + row_bonus;
        } else if(piece.piece == P2){
            score -= piece_value + position_bonus + row_bonus;
        }
    }
//    int mobilitiy =
    if(no_legal_moves(P1) || piece_counter[P1-1] == 0) return INT_MIN;
    if(no_legal_moves(P2) || piece_counter[P2-1] == 0) return INT_MAX;
    return score;
}

std::pair<int, PiecePos> Checkers::pvs(PiecePos node, int depth, int alpha, int beta, int colour) {
    if(depth == 0 || is_terminal_node()){
        return std::make_pair(static_cast<int>(colour * evaluate_board()), node);
    }
    int count = 0;
    int score = 0;
    PiecePos best_move = node; // Initialize the best move to the current node

    for(const auto child_move : generate_moves(node)){
        if(child_move.piece == error_state){
            continue;
        }
        if(count == 0){
            score = -pvs(child_move, depth-1, -alpha, -beta, -colour).first;
        }
        else {
            score = -pvs(child_move, depth-1, -alpha-1, -alpha, -colour).first;
            if( alpha < score && score < beta){
                score = -pvs(child_move, depth-1, -beta, -alpha, -colour).first;
            }
            alpha = std::max(alpha, score);
            if(alpha >= beta){
                break;
            }
        }
        if(score > alpha){
            alpha = score;
            best_move = child_move;
        }
        count++;
    }
    return std::make_pair(alpha, best_move);
}

void Checkers::find_best_move(){
    PiecePos best_move;
    int best_score = 0;
    std::pair<int, PiecePos> temp_score;
    int alpha = -10000;
    PiecePos best_piece;
    int beta = 10000;
    for(const auto piece : piece_stack){
        if(piece.piece != P2){ continue;}
         temp_score = pvs(piece, 2, alpha, beta, 1);
        if(temp_score.first > best_score){
            best_score = temp_score.first;
            best_move = temp_score.second;
            best_piece = piece;
        }
    }
    std::cout << "Best score: "<<best_score << ", Best piece: "<<best_piece.piece << "("<<best_piece.row<<", "<<best_piece.col<<") "<<"Best move: (" << best_move.row << ", "<< best_move.col << ")"<< std::endl;
}


/* GraveYard?
 * */
int Checkers::check_move(PieceType p, int row, int col) {
    PieceType p_at = this->get_piece_at(row, col);
    if(p_at == error_state) return 0;
    return 1;
}