#include "Checkers.h"
#include <QTimer>
#include <QPushButton>
#include <iostream>
#include <algorithm>
#include <random>
Checkers::Checkers(QWidget *parent) : Board(parent, 8, 8), pieces(), piece_counter(), opposing() {
    this->map_piece(P2, ":/res/red_checker32px.png");
    this->map_piece(P1, ":/res/black_checker32px.png");
//    PiecePos initialClickStates = {-1, -1, error_state, false};
    this->piece_counter[P1-1] = 0;
    this->piece_counter[P2-1] = 0;
    this->opposing[0] = empty_state;
    this->opposing[P1] = P2;
    this->opposing[P2] = P1;
    pieceFromClick = nullptr;
    pieceToClick = nullptr;
}



void print_p(PiecePos* p) {
    std::cout << "Piece{"<<p->piece << ", " << p->row << ", " << p->col <<"}" << std::endl;
}

void Checkers::setClickedPiece(int r, int c) {

    if(pieceFromClick== nullptr && pieceToClick == nullptr ){
        pieceFromClick = guts.get_piece_position(r, c);
        std::cout << "Set from: P" <<pieceFromClick->piece <<" (" << pieceFromClick->row << ", " << pieceFromClick->col <<")" << std::endl;
    } else if(pieceFromClick != nullptr && pieceToClick  == nullptr){
        pieceToClick = guts.get_piece_position(r, c);
        std::cout << "Set to: P" <<pieceToClick->piece <<" (" << pieceToClick->row << ", " << pieceToClick->col <<")" << std::endl;
        std::cout << "And then the piece to generate moves for? ";
        std::cout << "Did it mutate the piece count?" << this->piece_counter[P1-1] << ", " << this->piece_counter[P2-1] << std::endl;
        print_p(pieceFromClick);
        auto* possible = guts.generate_moves(pieceFromClick);
        bool isValid = false;
        auto validMove = std::pair<PiecePos*, PiecePos*>();
        if(!possible->empty()){
            for(const auto& m : *possible){
                print_p(m.first);
                if((m.first->row == pieceToClick->row) && (m.first->col == pieceToClick->col)){
                    isValid = true;
                    validMove = m;
                    break;
                }
            }
        }
        if(!isValid) {
            pieceToClick = nullptr;
            pieceFromClick = nullptr;
            return;
        }
        guts.apply_move(pieceFromClick, validMove.first, validMove.second, this);
        repaint();
        pieceToClick = nullptr;
        pieceFromClick = nullptr;
//        this->p1_turn = false;
//        find_best_move();
//        this->p1_turn = true;

        std::cout << "Reset both pieces after making move" << std::endl;
        auto move = this->BestMove(P2);
        auto pieceActual = guts.get_piece_position(move->pOrigin->row, move->pOrigin->col);
        guts.apply_move(pieceActual, move->pEnd, move->pCapture, this);
        repaint();

    } else {
        pieceToClick = nullptr;
        pieceFromClick = nullptr;
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
int Checkers::add_place(int r, int c, PieceType p){
    if(this->place(r, c, p) == -1) return -1;
    auto* temp = new PiecePos{r, c, p};
    guts.pieces.push_front(temp);
    return 0;
}
int Checkers::populate_board(bool pVsAi) {
    int t=0; int b = this->get_size(1)-1;
    bool alter = true;
    std::vector<PiecePos> temp;

    while(t+1 < b-1){

        for(int c=0; c<this->get_size(1)-1; c+=2){
            this->add_place(b, alter ? c : c+1, P1);
            this->add_place(t, alter ? c+1 : c, P2);
            guts.piece_counter[P1-1]++;
            guts.piece_counter[P2-1]++;
        }
        t++;
        b--;
        alter = !alter;

    }
    std::cout << "Pieces in theory: " << guts.pieces.size() << std::endl;
//    std::move(this->pieces, )
    toggle_space_connection(pVsAi);
    return 1;
}

int Checkers::colour_scale(PieceType c) {
    if(c > empty_state) {
        return (c == P2) ? 1 : -1;
    }
    return 0;
}


void Checkers::turns(int t, PieceType pt){
    if(t == 0) return;
    QTimer::singleShot(500, this, [=](){
        repaint();
        std::cout << "Start turn " << t << std::endl;
        auto b = BestMove(pt);
        auto actual = guts.get_piece_position(b->pOrigin->row, b->pOrigin->col);
        guts.apply_move(actual, b->pEnd, b->pCapture, this);
        repaint();
        this->turns(t-1, next_colour(pt));
    });
}

PieceType Checkers::next_colour(PieceType p) {
    return this->opposing[p];
}

[[maybe_unused]]
void Checkers::print_peices() {
    for(auto s : this->pieces) {
        print_p(s);
    }
}

int Checkers::MinMax(int recursionLevel, bool isMaximizing, PieceType pt, minimal_checker c) {
    if(recursionLevel == 0 || c.is_terminal_node()) {
        return c.evaluate_board();
    }
    int max_score = INT_MIN;
    auto* pieces_ = c.clone_pieces();
    if(!isMaximizing) max_score = INT_MAX;
    for(const auto& p : *pieces_) {
        if(p->piece != pt) continue;
        for(const auto m : *c.generate_moves(p)){
            auto b = c.apply_move(p, m.first, m.second, nullptr);
            int evaluationRes = MinMax(recursionLevel-1, !isMaximizing, next_colour(pt), c);
            c.undo_move(p, m.first, b, nullptr);
            if(isMaximizing) {
                max_score = std::max(evaluationRes, max_score);
            } else {
                max_score = std::min(evaluationRes, max_score);
            }
        }
    }
    return max_score;
}

MoveDecision* Checkers::BestMove(PieceType pt) {
    std::cout << "Begin best move find!" << std::endl;
    std::cout << "Pieces in theory: " << guts.pieces.size() << std::endl;

    int max_score = INT_MIN;
    auto best_move = new MoveDecision{nullptr, nullptr, nullptr, INT_MIN};
    minimal_checker temp_;

    auto* pieces_ = guts.clone_pieces();
    guts.clone_pieces(&temp_.pieces);
    auto start = std::chrono::high_resolution_clock::now();
    for(const auto& p : *pieces_) {
        if(p->piece != pt) continue;
        for(const auto m : *temp_.generate_moves(p)) {
            auto b = temp_.apply_move(p, m.first, m.second, nullptr);
            int eval_res = MinMax(3, true, P2, temp_);
            temp_.undo_move(p, m.first, b, nullptr);
            if(eval_res > max_score){
                best_move->pOrigin = p;
                best_move->pEnd = m.first;
                best_move->pCapture = m.second;
                best_move->score = eval_res;
                max_score = eval_res;
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
    std::cout << "Thinking time: " << duration.count() << " ms " << std::endl;
    std::cout << "End best move find!" << std::endl;
    return best_move;
}