#ifndef TICTACTOE_H
#define TICTACTOE_H
#include "board.h"
class TicTacToe : public Board
{
public:
    explicit TicTacToe(QWidget *parent=nullptr);
};

#endif // TICTACTOE_H
