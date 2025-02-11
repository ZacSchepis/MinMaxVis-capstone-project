#ifndef TREEGRAPHICS_H
#define TREEGRAPHICS_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "tictactoe.h"  // Include TicTacToe board


class TreeGraphics : public QWidget {
    Q_OBJECT

public:
    explicit TreeGraphics(QWidget* parent = nullptr);

private:
    QGraphicsScene* scene;
    QGraphicsView* view;


    void drawTreeWithBoards(int x, int y, int depth, int level);

};

#endif // TREEGRAPHICS_H
