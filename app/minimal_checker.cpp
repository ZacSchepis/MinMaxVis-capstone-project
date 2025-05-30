//
// Created by XxGam on 4/17/2025.
//

#include "minimal_checker.h"

minimal_checker::minimal_checker(int turns) : pieces(), turn_count(turns) {
    this->opposing[P1] = P2;
    this->opposing[P2] = P1;
    this->opposing[empty_state] = empty_state;
}

bool
minimal_checker::maybe_add(std::list<std::pair<PiecePos *, PiecePos *>> *l, int r, int c, int rOffset, int cOffset,
                           PiecePos *p) {
    PiecePos* piece_at = this->get_piece_position(r, c);
    if(piece_at->piece != empty_state) return false;
    if(should_skip(p, rOffset)) return false;
    if(std::abs(rOffset) == 2 ) {
        int midR = p->row + (rOffset / 2);
        int midC = p->col + (cOffset / 2);
        PiecePos* mid = this->get_piece_position(midR, midC);
        if(this->opposing[p->piece] == mid->piece){
            l->emplace_front(piece_at, mid);
//            print_p(piece_at);
            return true;
        } else {
            return false;
        }
    }

    l->emplace_front(piece_at, nullptr);
    return false;

}

bool minimal_checker::is_terminal_node() {
    return ((this->piece_counter[P1 - 1] == 0) || (this->piece_counter[P2 - 1] == 0)) || ( no_legal_moves(P1) ) || (no_legal_moves(P2));
}
std::list<PiecePos*>* minimal_checker::clone_pieces() {
    auto* new_pieces = new std::list<PiecePos*>();
    for(const auto& p : pieces) {
        auto* clone = new PiecePos{p->row, p->col, p->piece, p->is_king};
        new_pieces->push_back(clone);
    }
    return new_pieces;
}
bool minimal_checker::no_legal_moves(PieceType p) {
    for(const auto pt : this->pieces){
        if(pt->piece != p) continue;
        auto t = this->generate_moves(pt);
        if(!t->moves.empty()) return false;
    }
    return true;
}

PiecePos* minimal_checker::get_piece_position(int r, int c) {
    if((r >= 8 || r < 0) || (c >= 8 || c < 0)) {
        auto* t = new PiecePos{-1, -1, error_state};
        return t;
    }
    for(const auto p : this->pieces){
        if(p->row == r && p->col == c) return p;
    }
//    PieceType temp = this->get_piece_at(r, c);
    auto* res = new PiecePos{r, c, empty_state};
    return res;
}

PieceMoves* minimal_checker::generate_moves(PiecePos *p) {
    auto* result = new PieceMoves();
    result->piece = p;
    result->hasJump = false;
    const std::list<int> tempOffsets = {2, -1, 2, 1};
    const std::list<int> temp = {-1, 1};
    const std::vector<std::pair<int, int>> directions = {
            {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
    };
    for(auto dir : directions) {
        if(should_skip(p, dir.first)) continue;
        if(maybe_add(&result->moves, p->row + 2*dir.first, p->col + 2*dir.second, 2*dir.first, 2*dir.second, p)) {
            result->hasJump = true;
        }
    }
    if(!result->hasJump){
        for(auto dir: directions) {
            if(should_skip(p, dir.first)) continue;
            maybe_add(&result->moves, p->row + dir.first, p->col +dir.second, dir.first, dir.second, p);
        }
    }

    return result;
}
MoveBackup minimal_checker::apply_move(PiecePos* node, PiecePos* move, PiecePos* captured, Board* b){
    MoveBackup backup = {node->row, node->col, captured};
    if(b){
        b->place(move->row, move->col, node->piece);
        b->place(node->row, node->col, empty_state);
    }

    node->row = move->row;
    node->col = move->col;
    if((node->piece == P2 && node->row == 7) || (node->piece == P1 && node->row == 0) ){
        node->is_king = true;
    }
    if(captured){
        auto actual_cap = this->get_piece_position(captured->row, captured->col);
        if(b) b->place(actual_cap->row, actual_cap->col, empty_state);
        pieces.remove(actual_cap);
        piece_counter[actual_cap->piece-1]--;
//        delete actual_cap;
    }
    return backup;
}
void minimal_checker::undo_move(PiecePos* node, PiecePos* move, const MoveBackup& backup, Board* b){
    if(b){
        b->place(backup.prev_row, backup.prev_col, node->piece);
        b->place(move->row, move->col, empty_state);
    }
    node->row = backup.prev_row;
    node->col = backup.prev_col;
    if (backup.captured) {
//        auto* restored_piece = new PiecePos{backup.captured->row, backup.captured->col, backup.captured->piece, backup.captured->is_king};
        pieces.push_front(backup.captured);
        piece_counter[backup.captured->piece - 1]++;
    }
}
int minimal_checker::evaluate_board() {
    int score = 0;
    for(const auto& piece: this->pieces){
        int piece_value = piece->is_king ? 3 : 1;
        auto moves = generate_moves(piece);
        int mobility = (!moves->moves.empty()) ? (int)moves->moves.size() : 0;
        if(!moves->moves.empty()){
            for(auto &m : moves->moves) {
                if(m.second != nullptr) mobility += 2;
            }
        }
        if(moves->hasJump){
            mobility += 2;
        }
//        int position_bonus = (piece->col > 1 && piece->col < (8 -2)) ? 1 : 0;
        int row_bonus = (piece->piece == P1) ? piece->row : (8-piece->row);
        if(piece->is_king) row_bonus = 0;
        if(piece->piece == P1){
            score -= piece_value  + row_bonus + mobility + piece_counter[P1-1];
        } else if(piece->piece == P2){
            score += piece_value  + row_bonus + mobility + piece_counter[P2-1];
        }
    }
    if(no_legal_moves(P1) || piece_counter[P1-1] == 0) return INT_MIN;
    if(no_legal_moves(P2) || piece_counter[P2-1] == 0) return INT_MAX;
    int penalty_per_turn = 1;
    int game_length_penalty = turn_count*penalty_per_turn;
    return score - game_length_penalty;
}

void minimal_checker::clone_pieces(std::list<PiecePos*>* source) {
    for(const auto& p : pieces) {
        auto *clone = new PiecePos{p->row, p->col, p->piece, p->is_king};
        source->push_back(clone);
    }
}