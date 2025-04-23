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
    void drawConnectionLine(QPointF parent, QPointF child, PieceType currentPlayer);
    void highlightBestMove(QGraphicsProxyWidget* proxy,
                                     const std::pair<int, int>& bestMove,
                                     const std::pair<int, int>& currentMove,
                                     PieceType currentPlayer);
    void Tree_withBoards(int x, int y, int depth, int level,
                         PieceType** state, std::pair<int, int> bestMove,
                         std::pair<int, int> currentMove = {-1, -1},
                         PieceType currentPlayer = P1,
                         QPointF parentPos = QPointF(-1, -1));

};

#endif // TREEGRAPHICS_H