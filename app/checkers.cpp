#include "Checkers.h"
#include <QTimer>
#include <QPushButton>
#include <iostream>
#include <algorithm>
#include <random>
Checkers::Checkers(QWidget *parent) : Board(parent, 8, 8, false, true), pieces(), opposing(), guts(0) {
    this->map_piece(P2, ":/res/red_checker32px");
    this->map_piece(P1, ":res/checkers_p1_32px.png");
//    PiecePos initialClickStates = {-1, -1, error_state, false};
    this->guts.piece_counter[P1-1] = 0;
    this->guts.piece_counter[P2-1] = 0;
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
        std::cout << "Did it mutate the piece count?" << this->guts.piece_counter[P1-1] << ", " << this->guts.piece_counter[P2-1] << std::endl;
        print_p(pieceFromClick);
        auto* possible = guts.generate_moves(pieceFromClick);
        bool isValid = false;
        auto validMove = std::pair<PiecePos*, PiecePos*>();
        if(!possible->moves.empty()){
            for(const auto& m : possible->moves){
                print_p(m.first);
                if((m.first->row == pieceToClick->row) && (m.first->col == pieceToClick->col)){
                    isValid = true;
                    validMove = m;
                    break;
                }
            }
        }
        if(!isValid) {
            QMessageBox::information(this, "Invalid Move", "Your move isn't valid. Please select a different move.");
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
        auto move = this->BestMove(P2, true);
        auto pieceActual = guts.get_piece_position(move->pOrigin->row, move->pOrigin->col);
        guts.apply_move(pieceActual, move->pEnd, move->pCapture, this);
        repaint();
        PieceType winner = error_state;
        if(guts.no_legal_moves(P1) ) winner = P2;
        else if(guts.no_legal_moves(P2)) winner = P1;
        if(winner != error_state){
            QMessageBox::information(this, "Winner!!!", QString("Player %1 has won!").arg(winner));
        }
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
//    this->add_place(0, 1, P2);
//    guts.piece_counter[P2-1]++;
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


void Checkers::turns(int t, PieceType pt, bool isMaximizing){
    if(t == 0) return;
    QTimer::singleShot(500, this, [=](){
        repaint();
        std::cout << "Start turn " << t << std::endl;
        auto b = BestMove(pt, isMaximizing);
        auto actual = guts.get_piece_position(b->pOrigin->row, b->pOrigin->col);
        guts.apply_move(actual, b->pEnd, b->pCapture, this);
        guts.incr_turn_count();
        repaint();
        this->turns(t-1, next_colour(pt), !isMaximizing);
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

int Checkers::MinMax(int recursionLevel, bool isMaximizing, PieceType pt, minimal_checker c, int alpha, int beta) {
    if(recursionLevel == 0 || c.is_terminal_node()) {
        return c.evaluate_board();
    }
    int best_score =( pt == P1) ? INT_MIN : INT_MAX;

//    int max_score = pt == P1 ? INT_MIN : INT_MAX;
    auto* pieces_ = c.clone_pieces();
    for(const auto& p : *pieces_) {
        if(p->piece != pt) continue;
        auto moveStat = c.generate_moves(p);
        for(const auto m : moveStat->moves){
            auto b = c.apply_move(p, m.first, m.second, nullptr);
            minimal_checker c2(c.get_turn_count());
            c.clone_pieces(&c2.pieces);
            int evaluationRes = MinMax(recursionLevel-1, !isMaximizing, next_colour(pt), c2, alpha, beta);
            c.undo_move(p, m.first, b, nullptr);
            if(isMaximizing) {

                best_score = std::max(evaluationRes, best_score);
                alpha = std::max(alpha, evaluationRes);
                if(beta <= alpha) {
                    return best_score; // beta cut off
                }

            } else {
                best_score = std::min(evaluationRes, best_score);
                beta = std::min(beta, evaluationRes);
                if(beta <= alpha) return best_score; // alpha cut off
            }
        }
    }
    return best_score;
}
void
Checkers::ForceCaptureMoves(std::vector<PieceMoves*> pm){
//    bool hasJumps = std::any_of(pm.begin(), pm.end(),
//                                [](PieceMoves* m) { return  m->hasJump;});
//    if(hasJumps){
//        pm.erase(std::remove_if(pm.begin(), pm.end(),
//                                [](PieceMoves* m) { return !m->hasJump;}), pm.end());
//    }
}
void Checkers::callbackSetup() {
    populate_board(true);
}
MoveDecision* Checkers::BestMove(PieceType pt, bool isMaximizing) {
    std::cout << "Begin best move find!" << std::endl;
    std::cout << "Pieces in theory: " << guts.pieces.size() << std::endl;

    int max_score = INT_MIN;
    auto best_move = new MoveDecision{nullptr, nullptr, nullptr, INT_MIN};
    minimal_checker temp_(guts.get_turn_count());

    auto* pieces_ = guts.clone_pieces();
    guts.clone_pieces(&temp_.pieces);
    auto start = std::chrono::high_resolution_clock::now();
    int lookAhead = 4;
    if(pt == P2 && guts.piece_counter[P2-1] < 5) {
        lookAhead = 5;
    }
    for(const auto& p : *pieces_) {
        if(p->piece != pt) continue;
        auto moveStat = temp_.generate_moves(p);
        for(const auto m : moveStat->moves) {
            auto b = temp_.apply_move(p, m.first, m.second, nullptr);
            int eval_res = MinMax(lookAhead, isMaximizing, pt, temp_, INT_MIN, INT_MAX);
            temp_.undo_move(p, m.first, b, nullptr);
            if(eval_res >= max_score){
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