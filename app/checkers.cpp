#include "Checkers.h"
#include <QTimer>
#include <QPushButton>
#include <iostream>
#include <algorithm>
#include <random>
Checkers::Checkers(QWidget *parent) : Board(parent, 8, 8), pieces() {
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
    PiecePos initialClickStates = {-1, -1, error_state, false, false};
//    if(!this->is_p1turn()) {
//        std::cout << "It isn't your turn :(" << std::endl;
//    }
    if(pieceFromClick== nullptr && pieceToClick == nullptr ){
        pieceFromClick = this->get_piece_position(r, c);
        std::cout << "Set from: P" <<pieceFromClick->piece <<" (" << pieceFromClick->row << ", " << pieceFromClick->col <<")" << std::endl;
    } else if(pieceFromClick != nullptr && pieceToClick  == nullptr){
        pieceToClick = this->get_piece_position(r, c);
        std::cout << "Set to: P" <<pieceToClick->piece <<" (" << pieceToClick->row << ", " << pieceToClick->col <<")" << std::endl;
        std::cout << "And then the piece to generate moves for? ";
        std::cout << "Did it mutate the piece count?" << this->piece_counter[P1-1] << ", " << this->piece_counter[P2-1] << std::endl;
        print_p(pieceFromClick);
        auto* possible = generate_moves(pieceFromClick);
        bool isValid = false;
//        print_p(pieceF)
        auto validMove = std::pair<PiecePos*, PiecePos*>();
//        PiecePos* p;
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
        auto b = apply_move(pieceFromClick, validMove.first, validMove.second);
        repaint();
        pieceToClick = nullptr;
        pieceFromClick = nullptr;
//        this->p1_turn = false;
//        find_best_move();
//        this->p1_turn = true;

        std::cout << "Reset both pieces after making move" << std::endl;
        auto move = this->BestMove(P2);
        auto pieceActual = get_piece_position(move->pOrigin->row, move->pOrigin->col);
        auto backup = apply_move(pieceActual, move->pEnd, move->pCapture);
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
    this->pieces.push_front(temp);
    return 0;
}
int Checkers::populate_board(bool pVsAi) {
    int t=0; int b = this->get_size(1)-1;
    bool alter = true;
    std::vector<PiecePos> temp;

    while(t+1 < b-1){

        for(int c=0; c<this->get_size(1)-1; c+=2){
            auto* p = new PiecePos{b, alter ? c : c+1, P1};
//            this->piecespush_back({.row=b, .col=(alter ? c : c+1), .piece=P1});
//            this->pieces.push_front(p);
            auto *p2 = new PiecePos{t, alter ? c+1 : c, P2};
//            this->pieces.push_front(p2);

            this->add_place(b, alter ? c : c+1, P1);
            this->add_place(t, alter ? c+1 : c, P2);
//            this->add_place(b, alter ? c : c+1, P1, DEFAULT_CONFIG);
//            this->add_place(t, alter ? c+1 : c, P2, DEFAULT_CONFIG);
            this->piece_counter[P1-1]++;
            this->piece_counter[P2-1]++;
        }
        t++;
        b--;
        alter = !alter;

    }
    std::cout << "Pieces in theory: " << pieces.size() << std::endl;
//    std::move(this->pieces, )
    toggle_space_connection(pVsAi);
    return 1;
}

int Checkers::populate_row(int r, PieceType p) {
    return 0;
}
int Checkers::colour_scale(PieceType c) {
    if(c > empty_state) {
        return (c == P2) ? 1 : -1;
    }
    return 0;
}

PiecePos* Checkers::pseudo_piece_position(int r, int c, std::list<PiecePos*> *ps) {
    if((r >= 8 || r < 0) || (c >= 8 || c < 0)) return new PiecePos{-1, -1, error_state};
    for(const auto p : *ps){
        if(p->row == r && p->col == c) return p;
    }
//    PieceType temp = this->get_piece_at(r, c);
    auto* res = new PiecePos{r, c, empty_state};
    return res;

}

PiecePos* Checkers::get_piece_position(int r, int c) {
    if((r >= 8 || r < 0) || (c >= 8 || c < 0)) return new PiecePos{-1, -1, error_state};
    for(const auto p : this->pieces){
        if(p->row == r && p->col == c) return p;
    }
//    PieceType temp = this->get_piece_at(r, c);
    auto* res = new PiecePos{r, c, empty_state};
    return res;
}
int sign(int n){
    if(n < 0) return -1;
    else if(n > 0) return 1;
    else return 0;
}


PieceType Checkers::maybe_add(std::list<std::pair<PiecePos*, PiecePos*>> *l, int r, int c, int rOffset, int cOffset, PiecePos *p){
    PiecePos* piece_at = this->get_piece_position(r, c);
    if(piece_at->piece != empty_state) return error_state;
    if(should_skip(p, rOffset)) return error_state;
    if(std::abs(rOffset) == 2 ) {
        int midR = p->row + (rOffset / 2);
        int midC = p->col + (cOffset / 2);
        PiecePos* mid = this->get_piece_position(midR, midC);
        if(this->opposing[p->piece] == mid->piece){
            l->emplace_front(piece_at, mid);
//            print_p(piece_at);
            return piece_at->piece;
        } else {
            return error_state;
        }
    }

    l->emplace_front(piece_at, nullptr);
    return piece_at->piece;
}

std::list<std::pair<PiecePos*, PiecePos*>>* Checkers::generate_moves(PiecePos *p) {
    auto* moves = new std::list<std::pair<PiecePos*, PiecePos*>>();
    const std::list<int> tempOffsets = {2, -1, 2, 1};
    bool forceCapture = false;
    const std::list<int> temp = {-1, 1};
    const std::vector<std::pair<int, int>> directions = {
            {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
    };
    for(auto dir : directions) {
        if(should_skip(p, dir.first)) continue;
        maybe_add(moves, p->row + dir.first, p->col +dir.second, dir.first, dir.second, p);
        maybe_add(moves, p->row + 2*dir.first, p->col + 2*dir.second, 2*dir.first, 2*dir.second, p);

    }
    return moves;
}
void Checkers::turns(int t, PieceType pt){
    if(t == 0) return;
    QTimer::singleShot(500, this, [=](){
        repaint();
        std::cout << "Start turn " << t << std::endl;
//        print_peices();
//        this->find_best_move(pt);
        auto b = BestMove(pt);
        auto actual = get_piece_position(b->pOrigin->row, b->pOrigin->col);
        apply_move(actual, b->pEnd, b->pCapture);
        repaint();
        this->turns(t-1, next_colour(pt));
    });
}

PieceType Checkers::next_colour(PieceType p) {
    return this->opposing[p];
}


void Checkers::print_peices() {
    for(auto s : this->pieces) {
        print_p(s);
    }
}
bool Checkers::no_legal_moves(PieceType p) {
    for(const auto pt : this->pieces){
        if(pt->piece != p) continue;
        auto t = this->generate_moves(pt);
        if(!t->empty()) return false;
    }
    return true;
}
int Checkers::evaluate_board() {
    int score = 0;
    for(const auto& piece: this->pieces){
        int piece_value = piece->is_king ? 3 : 1;
        int position_bonus = (piece->col > 1 && piece->col < get_size(1) -2) ? 1 : 0;
        int row_bonus = (piece->piece == P1) ? piece->row : (get_size(1)-1-piece->row);
        if(piece->piece == P1){
            score += piece_value + position_bonus + row_bonus;
        } else if(piece->piece == P2){
            score -= piece_value + position_bonus + row_bonus;
        }
    }
//    int mobilitiy =
    if(no_legal_moves(P1) || piece_counter[P1-1] == 0) return INT_MIN;
    if(no_legal_moves(P2) || piece_counter[P2-1] == 0) return INT_MAX;
    return score;
}

bool Checkers::is_terminal_node() {
    return ((this->piece_counter[P1 - 1] == 0) || (this->piece_counter[P2 - 1] == 0)) || ( no_legal_moves(P1) ) || (no_legal_moves(P2));
}
MoveBackup Checkers::apply_move(PiecePos* node, PiecePos* move, PiecePos* captured){
    MoveBackup backup = {node->row, node->col, captured};
    place(move->row, move->col, node->piece);
    place(node->row, node->col, empty_state);
    node->row = move->row;
    node->col = move->col;
    if((node->piece == P1 && node->row == 0) || (node->piece == P2 && node->row == 7)){
        node->is_king = true;
    }
    if(captured){
        std::cout << "Got a capture outside? ";
        auto actual_cap = this->get_piece_position(captured->row, captured->col);
        print_p(actual_cap);
        place(actual_cap->row, actual_cap->col, empty_state);
        pieces.remove(actual_cap);
        piece_counter[captured->piece-1]--;
        delete actual_cap;
    }
    return backup;
}
void Checkers::clone_pieces(std::list<PiecePos*>* source) {
    for(const auto& p : pieces) {
        auto *clone = new PiecePos{p->row, p->col, p->piece, p->is_king};
        source->push_back(clone);
    }
}
std::list<PiecePos*>* Checkers::clone_pieces() {
    auto* new_pieces = new std::list<PiecePos*>();
    for(const auto& p : pieces) {
        auto* clone = new PiecePos{p->row, p->col, p->piece, p->is_king};
        new_pieces->push_back(clone);
    }
    return new_pieces;
}
void Checkers::undo_move(PiecePos* node, PiecePos* move, const MoveBackup& backup){
    place(backup.prev_row, backup.prev_col, node->piece);
    place(move->row, move->col, empty_state);
    node->row = backup.prev_row;
    node->col = backup.prev_col;
    if (backup.captured) {
        auto* restored_piece = new PiecePos{backup.captured->row, backup.captured->col, backup.captured->piece, backup.captured->is_king};
        pieces.push_front(restored_piece);
        piece_counter[backup.captured->piece - 1]++;
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
    std::cout << "Pieces in theory: " << pieces.size() << std::endl;

    int max_score = INT_MIN;
    auto best_move = new MoveDecision{nullptr, nullptr, nullptr, INT_MIN};
    minimal_checker temp_;

    auto* pieces_ = clone_pieces();
    this->clone_pieces(&temp_.pieces);
    auto start = std::chrono::high_resolution_clock::now();
    for(const auto& p : *pieces_) {
        if(p->piece != pt) continue;
        for(const auto m : *temp_.generate_moves(p)) {
            auto b = temp_.apply_move(p, m.first, m.second, nullptr);
            int evalres = MinMax(3, true, P2, temp_);
            temp_.undo_move(p, m.first, b, nullptr);
            if(evalres > max_score){
//                std::cout << "Eval: " << evalres << ", Found Piece? " << (m.second ? "Yes!" : "nah no capture") << std::endl;
//                best_move = new MoveDecision{p, m.first, m.second, evalres};
                best_move->pOrigin = p;
                best_move->pEnd = m.first;
                best_move->pCapture = m.second;
//                print_p()
                best_move->score = evalres;
                max_score = evalres;
            }
        }
    }
    // return best move

//    if((best_move->pOrigin != nullptr) && (best_move->pEnd != nullptr)) {
//        std::cout << "Applying: ";
//        print_p(best_move->pOrigin);
//        print_p(best_move->pEnd);
//        if(best_move->pCapture){
//            print_p(best_move->pCapture);
//        }
//
//        apply_move(best_move->pOrigin, best_move->pEnd, best_move->pCapture);
//    }
//    print_peices();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
    std::cout << "Thinking time: " << duration.count() << " ms " << std::endl;
    std::cout << "End best move find!" << std::endl;
    return best_move;
}