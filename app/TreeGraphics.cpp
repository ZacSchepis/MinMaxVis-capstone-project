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
        // Recalculate best move for Player
        int bestScore = INT_MAX;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (gameInstance->get_piece_at(i, j) == empty_state) {
                    gameInstance->place(i, j, P1);
                    int score = gameInstance->MinMax(0, true);
                    gameInstance->place(i, j, empty_state);
                    if (score < bestScore) {
                        bestScore = score;
                        bestMove = {i, j};
                    }
                }
            }
        }
    }

    Tree_withBoards(600, 50, 3, 0, gameInstance->Retrieve_stateofBoard(), bestMove);
}

void TreeGraphics::Tree_withBoards(int x, int y, int depth, int level, PieceType** state, std::pair<int, int> bestMove, std::pair<int, int> currentMove) {
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


    if (level == 2 && bestMove != std::pair<int, int>{-1, -1}) {
    } else if (level == 1 && currentMove == bestMove) {
        QRectF widgetRect = proxy->widget()->geometry();
        QRectF borderRect(0, 0, widgetRect.width() * proxy->scale(), widgetRect.height() * proxy->scale());

        QGraphicsRectItem* border = scene->addRect(borderRect, QPen(Qt::red, 4));
        border->setPos(proxy->pos());
        border->setZValue(proxy->zValue() - 1);

        // INFO ICON
        QPushButton* infoButton = new QPushButton("ℹ️");
        infoButton->setStyleSheet("background-color: transparent; font-weight: bold;");

        QGraphicsProxyWidget* infoProxy = scene->addWidget(infoButton);
        infoProxy->setPos(proxy->pos().x() + borderRect.width() - 10, proxy->pos().y() - 10);
        infoProxy->setZValue(proxy->zValue() + 1);

        connect(infoButton, &QPushButton::clicked, [=]() {
            QString details = QString("Best Computer Move: (%1, %2)\nBoard Score: %3")
                              .arg(bestMove.first)
                              .arg(bestMove.second)
                              .arg(gameInstance->MinMax(0, true));
            QMessageBox::information(nullptr, "Move Info", details);
        });
    }

        if (level == 2 && currentMove == bestMove) {
            QRectF widgetRect = proxy->widget()->geometry();
            QRectF borderRect(0, 0, widgetRect.width() * proxy->scale(), widgetRect.height() * proxy->scale());

            QGraphicsRectItem* border = scene->addRect(borderRect, QPen(Qt::green, 4));
            border->setPos(proxy->pos());
            border->setZValue(proxy->zValue() - 1);

            // INFO ICON
            QPushButton* infoButton = new QPushButton("ℹ️");
            infoButton->setStyleSheet("background-color: transparent; font-weight: bold;");

            QGraphicsProxyWidget* infoProxy = scene->addWidget(infoButton);
            infoProxy->setPos(proxy->pos().x() + borderRect.width() - 10, proxy->pos().y() - 10);
            infoProxy->setZValue(proxy->zValue() + 1);

            connect(infoButton, &QPushButton::clicked, [=]() {
                QString details = QString("Best Move: (%1, %2)\nBoard Score: %3")
                                  .arg(bestMove.first)
                                  .arg(bestMove.second)
                                  .arg(gameInstance->MinMax(0, true));
                QMessageBox::information(nullptr, "Move Info", details);
            });
        }





    std::vector<std::pair<int, int>> possibleMoves = gameInstance->find_possiblemove();
    for (size_t i = 0; i < possibleMoves.size(); i++) {
        int childX = x + (i - static_cast<int>(possibleMoves.size()) / 2) * 100;
        PieceType currentPlayer = (level % 2 == 0) ? P2 : P1;  // Even = Computer (P2), Odd = Player (P1)






        PieceType** newState = gameInstance->Visualize_Move(
            state, possibleMoves[i].first, possibleMoves[i].second, currentPlayer);

        if (newState) {
            Tree_withBoards(childX, y + 120, depth - 1, level + 1, newState, bestMove, possibleMoves[i]);
        }
    }
}