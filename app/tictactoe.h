#ifndef TICTACTOE_H
#define TICTACTOE_H

#include "board.h"
#include <QPushButton>
#include <QMessageBox>
#include <utility>

class TicTacToe : public Board {
    Q_OBJECT

public:
    explicit TicTacToe(QWidget *parent = nullptr);

private:
    void Player_move(int r, int c);
    void Computer_move();
    bool Findout_Win(PieceType player);
    bool Findout_draw();
    void Findout_End(const QString& result);
    void Restart_Game();
    int MinMax(int recursionLevel, bool isMaximizing);
    std::pair<int, int> BestMoveCalculation();
};

#endif // TICTACTOE_H
