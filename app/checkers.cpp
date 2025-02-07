#include "checkers.h"

// Dont worry too much about the board styles
// Im gonna look more into that later to give them
// better themes
Checkers::Checkers(QWidget *parent) : Board(parent, 8,8) {
    this->map_piece(P1, ":/res/red_checker32px.png");
    this->map_piece(P2, ":/res/black_checker32px.png");
}
