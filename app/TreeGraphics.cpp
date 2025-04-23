#include "TreeGraphics.h"
#include <QGraphicsProxyWidget>
#include <QPen>

TreeGraphics::TreeGraphics(QWidget* parent, TicTacToe* game) : QWidget(parent), gameInstance(game) {
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);
    view->setGeometry(10, 10, 1200, 800); // Adjust size to fit expanding tree

    if (gameInstance) {
        connect(gameInstance, &TicTacToe::move_Executed, this, &TreeGraphics::update_Tree);
    }

    update_Tree();
}

void TreeGraphics::update_Tree() {
    scene->clear();

    std::pair<int, int> bestMove = {-1, -1};

    if (gameInstance) {
        PieceType currentPlayer = gameInstance->currentTurn;
        int bestScore = (currentPlayer == P2) ? INT_MIN : INT_MAX;

        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (gameInstance->get_piece_at(i, j) == empty_state) {
                    gameInstance->place(i, j, currentPlayer);
                    int score = gameInstance->MinMax(0, currentPlayer == P2);
                    gameInstance->place(i, j, empty_state);

                    if ((currentPlayer == P2 && score > bestScore) ||
                        (currentPlayer == P1 && score < bestScore)) {
                        bestScore = score;
                        bestMove = {i, j};
                        }
                }
            }
        }
        Tree_withBoards(600, 50, 3, 0, gameInstance->Retrieve_stateofBoard(),
                        bestMove, {-1, -1}, currentPlayer, QPointF(-1, -1));
    }
}

void TreeGraphics::drawConnectionLine(QPointF parent, QPointF child, PieceType currentPlayer) {
    // Choose line color based on current player
    QPen linePen = (currentPlayer == P1) ? QPen(Qt::red, 2) : QPen(Qt::green, 2);

    // Create the line item between parent and child centers
    QGraphicsLineItem* connection = scene->addLine(QLineF(parent, child), linePen);
    connection->setZValue(-2); // Make sure it stays behind the boards
}

void TreeGraphics::highlightBestMove(QGraphicsProxyWidget* proxy,
                                     const std::pair<int, int>& bestMove,
                                     const std::pair<int, int>& currentMove,
                                     PieceType currentPlayer) {
    if (currentMove != bestMove) return;

    QRectF widgetRect = proxy->widget()->geometry();
    QRectF borderRect(0, 0, widgetRect.width() * proxy->scale(), widgetRect.height() * proxy->scale());

    QPen highlightPen = (currentPlayer == P1) ? QPen(Qt::red, 4) : QPen(Qt::green, 4);
    QString playerLabel = (currentPlayer == P1) ? "Player" : "Computer";

    QGraphicsRectItem* border = scene->addRect(borderRect, highlightPen);
    border->setPos(proxy->pos());
    border->setZValue(proxy->zValue() - 1);

    QPushButton* infoButton = new QPushButton("ℹ️");
    infoButton->setStyleSheet("background-color: transparent; font-weight: bold;");
    QGraphicsProxyWidget* infoProxy = scene->addWidget(infoButton);
    infoProxy->setPos(proxy->pos().x() + borderRect.width() - 10, proxy->pos().y() - 10);
    infoProxy->setZValue(proxy->zValue() + 1);

    connect(infoButton, &QPushButton::clicked, [=]() {
        QString details = QString("Best %1 Move: (%2, %3)\nBoard Score: %4")
                          .arg(playerLabel)
                          .arg(bestMove.first)
                          .arg(bestMove.second)
                          .arg(gameInstance->MinMax(0, currentPlayer == P2));
        QMessageBox::information(nullptr, "Move Info", details);
    });
}

void TreeGraphics::Tree_withBoards(int x, int y, int depth, int level,
                                   PieceType** state, std::pair<int, int> bestMove,
                                   std::pair<int, int> currentMove, PieceType currentPlayer,
                                   QPointF parentPos) {

    if (depth == 0 || state == nullptr) return;

    TicTacToe* board = new TicTacToe(nullptr, false);
    board->update_stateofBoard(state);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            QLayoutItem *item = board->grid->itemAtPosition(i, j);
            if (item) {
                QWidget *widget = item->widget();
                if (QPushButton *button = qobject_cast<QPushButton*>(widget)) {
                    button->setDisabled(true);
                }
            }
        }
    }

    QGraphicsProxyWidget* proxy = scene->addWidget(board);
    proxy->setScale(0.5);
    proxy->setPos(x, y);

    QPointF currentCenter(x + board->width() * proxy->scale() / 2,
                          y + board->height() * proxy->scale() / 2);

    // Draw line from parent to this node
    if (level == 1 && parentPos != QPointF(-1, -1)) {
        drawConnectionLine(parentPos, currentCenter, currentPlayer);
    }

    // Highlight best move at level 1
    if (level == 1 && currentMove == bestMove) {
        highlightBestMove(proxy, bestMove, currentMove, currentPlayer);

    }

    std::vector<std::pair<int, int>> possibleMoves = gameInstance->find_possiblemove();
    PieceType nextPlayer = (currentPlayer == P1) ? P2 : P1;

    for (size_t i = 0; i < possibleMoves.size(); i++) {
        int childX = x + (i - static_cast<int>(possibleMoves.size()) / 2) * 100;
        int childY = y + 120;

        PieceType** newState = gameInstance->Visualize_Move(
            state, possibleMoves[i].first, possibleMoves[i].second, currentPlayer);

        if (newState) {
            Tree_withBoards(childX, childY, depth - 1, level + 1,
                            newState, bestMove, possibleMoves[i], nextPlayer, currentCenter);
        }
    }
}

