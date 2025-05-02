//
// Created by ZacSchepis on 4/16/2025.
//

#ifndef APP_CHECKERS2_H
#define APP_CHECKERS2_H
#include <vector>
#include "board.h"
#include "minimal_checker.h"
#include <QMessageBox>

class Checkers : public Board {
private:
    minimal_checker guts;
    PiecePos* pieceToClick;
    PiecePos* pieceFromClick;
    std::array<PieceType, 3> opposing;
    std::list<PiecePos*> pieces;
    /**
     * @brief Checkers::setClickedPiece Schepis-4/17
     * Conditionally sets and applies pieces clicked by the user
     * @param r - row index clicked
     * @param c - column index clicked
     */
    void setClickedPiece(int r, int c);
    int add_place(int r, int c, PieceType p);
    /**
     * @brief Checkers::colour_scale Schepis-4/17
     * Scales a piece type to be 1 or -1
     * @param c - piece type to colour
     * @return
     */
    static int colour_scale(PieceType c);
public:
    explicit Checkers(QWidget *parent=nullptr);
    /**
     * @brief Checkers::next_colour Schepis-4/17
     * Helper function to cycle through piece types without
     * manually switching
     * @param p - piece type to get the next colour of
     * @return
     */
    PieceType next_colour(PieceType p);
    void print_peices();

    /**
     * @brief Checkers::turns Schepis-4/17
     * Runs t turns of AI vs AI checkers
     * @param t - number of turns to do
     * @param pt - piece colour to start on
     */
    void turns(int t, PieceType pt, bool isMaximizing);
    /**
     * @brief Checkers::populate_board Schepis-4/17
     * Populates Checker board with pieces
     * @param pVsAi - True : take player input, False : player input isn't taken
     * @return
     */
    int populate_board(bool pVsAi);
    /**
     * @brief Checkers::toggle_space_connection Schepis-4/17
     * [En/Dis]ables space connections
     * @param status
     */
    void toggle_space_connection(bool status);
    /**
     * @brief Checkers::BestMove Schepis-4/17
     * Find the best move for the provided piece
     * @param pt - piece type to find the best for
     * @return
     */
    MoveDecision* BestMove(PieceType pt, bool isMaximizing);
    /**
     * @brief Checkers::MinMax Schepis-4/17
     * MinMax implementation for Checkers
     * @param recursionLevel - current search depth
     * @param isMaximizing - path if the current player is maximizing
     * @param pt - piece/colour type to look for
     * @param c - Internal data structure to use
     * @param isMaximizing - should this move be maximizing
     * @return
     */
    int MinMax(int recursionLevel, bool isMaximizing, PieceType pt, minimal_checker c, int alpha, int beta);
    void ForceCaptureMoves(std::vector<PieceMoves*> pm);
    void callbackSetup() override;
};
#endif //APP_CHECKERS2_H
