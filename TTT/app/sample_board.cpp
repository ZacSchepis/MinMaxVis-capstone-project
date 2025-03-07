#include "sample_board.h"

SampleBoard::SampleBoard(QWidget *parent) : Board(parent, 5, 5){
    this->map_piece(P1, ":/res/red_checker32px.png");
    this->map_piece(P2, ":/res/black_checker32px.png");
    // Resources are avaible at resources.qrc
    /*
     * See /app/README.md for more paths:
     */
}
