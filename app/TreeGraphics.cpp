#include "TreeGraphics.h"
#include <QGraphicsProxyWidget>
#include <QPen>
#include "TicTacToe.h"

TreeGraphics::TreeGraphics(QWidget* parent) : QWidget(parent) {
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);

    view->setGeometry(10, 10, 800, 600);

    int startX = 400;
    int startY = 50;
    int treeDepth = 3;

    drawTreeWithBoards(startX, startY, treeDepth, 0);
}

void TreeGraphics::drawTreeWithBoards(int x, int y, int depth, int level) {
    if (depth == 0)
        return;

    const double scaleFactor = 0.5;
    const int verticalSpacing = 120;
    const int horizontalSpacing = 100 * depth;

    TicTacToe* board = new TicTacToe();
    QGraphicsProxyWidget* proxy = scene->addWidget(board);
    proxy->setScale(scaleFactor);
    proxy->setPos(x, y);

    QRectF boardRect = proxy->boundingRect();
    int boardWidth  = boardRect.width() * scaleFactor;
    int boardHeight = boardRect.height() * scaleFactor;

    int parentBottomCenterX = x + boardWidth / 2;
    int parentBottomCenterY = y + boardHeight;

    int childY = y + verticalSpacing;

    for (int i = 0; i < 2; i++) {
        int childX = (i == 0) ? x - horizontalSpacing : x + horizontalSpacing;

        int childTopCenterX = childX + boardWidth / 2;
        int childTopCenterY = childY;

        scene->addLine(parentBottomCenterX, parentBottomCenterY,
                       childTopCenterX, childTopCenterY,
                       QPen(Qt::black));

        drawTreeWithBoards(childX, childY, depth - 1, level + 1);
    }
}

