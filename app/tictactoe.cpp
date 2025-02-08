#include "tictactoe.h"

// Dont worry too much about the board styles
// Im gonna look more into that later to give them
// better themes
TicTacToe::TicTacToe(QWidget *parent) : Board(parent, 3,3) {
    this->map_piece(P1, ":/res/x_tictactoe32px.png");
    this->map_piece(P2, ":/res/o_tictactoe32px.png");
}
