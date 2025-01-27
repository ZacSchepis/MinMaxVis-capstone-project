#include "board.h"

Board::Board(QWidget *parent, int r, int c)
    : QWidget{parent}, rows(r), cols(c)
{
    int board_state[rows][cols];
    for(int i=0; i<r; i++){
        for(int j=0; j<c; j++){
            board_state[i][j] = -1;
        }
    }

}
int Board::place(int r, int c, PieceType piece){
    if((r >= rows || r < 0) || (c >= cols || c < 0)) return -1;
    board_state[r][c] = piece;
    return 0;
}


// This method will override the paint event
// you guys can use this when drawing the board
void Board::paintEvent(QPaintEvent* event){

}
