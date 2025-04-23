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
    std::array<int, 2> piece_counter;
    std::list<PiecePos *> pieces;

    minimal_checker();
    /**
     * @brief minimal_checker::is_terminal_node Schepis-4/17
     * Checks if a current game state can't be played further
     * @return
     */
    bool is_terminal_node();

    /**
     * @brief minimal_checker::apply_move  Schepis-4/17
     * @param move apply move to node
     * @param node's stored piece
     * @param captured if passed in, that is removed from the board
     * @param b if not nullptr, also update the board GUI states
     * @return
     */

    MoveBackup apply_move(PiecePos *node, PiecePos *move, PiecePos *captured, Board *b);

    /**
     * @brief minimal_checker::undo_move  Schepis-4/17
     * Undos an applied move using @param backup to restore the state to
     * @param node
     * if @param b isn't nullptr, the board GUI states are updated
     * @return
     */

    void undo_move(PiecePos *node, PiecePos *move, const MoveBackup &backup, Board *b);

    /**
     * @brief minimal_checker::no_legal_moves  Schepis-4/17
     * Checks if there aren't any legal moves remaining
     * for the provided piece type
     * @return
     */

    bool no_legal_moves(PieceType p);



    /**
     * @brief minimal_checker::generate_moves  Schepis-4/17
     * @param p - Generate all moves from PiecePosition p
     * @return
     */
    std::list<std::pair<PiecePos *, PiecePos *>> *generate_moves(PiecePos *p);

    /**
     * @brief minimal_checker::maybe_add Schepis-4/17
     * Maybe add a move to a piece stack
     * @param l - List of move pairs <PieceEnd, CapturePiece> . If a move results in a capture, the capture piece is
     *  stored into the .second field of that elements pair
     * @param r - row index to scan
     * @param c - column index to scan
     * @param rOffset - row offset value, useful for finding capture pieces
     * @param cOffset - column offset value, useful for finding capture pieces
     * @param p - PiecePosition to start looking from
     * @return
     */
    PieceType
    maybe_add(std::list<std::pair<PiecePos *, PiecePos *>> *l, int r, int c, int rOffset, int cOffset, PiecePos *p);

    /**
     * @brief minimal_checker::get_piece_position Schepis-4/17
     * Returns the pointer element from the piece stack to allow for updating
     * Reason: passing around pointers and updating their interal states became
     * easier than using everything by value
     * @param r - row index of piece
     * @param c - column index of piece
     * @return
     */
    PiecePos *get_piece_position(int r, int c);

    /**
     * @brief minimal_checker::evaluate_board Schepis-4/17
     * Scores the current internal state of a board
     * @return
     */
    int evaluate_board();

    /**
     * @brief minimal_checker::clone_pieces Schepis-4/17
     * Copies internal pieces to a source list
     * @param source
     */
    void clone_pieces(std::list<PiecePos*>* source);

    /**
     * @brief minimal_checker::clone_pieces  Schepis-4/17
     * Creates a copy of internal pieces
     * @return
     */
    std::list<PiecePos *> *clone_pieces();

};
static bool should_skip(PiecePos *p, int offset) {
    if(p->is_king) return false;
    if((offset > 0 && p->piece == P1) || (offset < 0 && p->piece == P2)) return true;
    return false;
}

#endif //APP_MINIMAL_CHECKER_H
