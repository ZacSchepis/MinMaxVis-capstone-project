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
    void moveoftheplayer(int r, int c);
    void Moveofthecomputer();
    bool Detectingthewin(PieceType player);
    bool Detectingthedraw();
    void DetectingtheEnd(const QString& result);
    void restartTheGame();
    int MinMax(int recursionLevel, bool isMaximizing);
    std::pair<int, int> BestMoveCalculation();
};

#endif // TICTACTOE_H

