#ifndef TREEGRAPHICS_H
#define TREEGRAPHICS_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "tictactoe.h"

class TreeGraphics : public QWidget {
    Q_OBJECT

public:
    explicit TreeGraphics(QWidget* parent = nullptr, TicTacToe* game = nullptr);

    public slots:
        void update_Tree();  // here we are declaring as a slot

private:
    QGraphicsScene* scene;
    QGraphicsView* view;
    TicTacToe* gameInstance;

    void Tree_withBoards(int x, int y, int depth, int level, PieceType** state, std::pair<int, int> bestMove, std::pair<int, int> currentMove = {-1, -1});

};

#endif // TREEGRAPHICS_H