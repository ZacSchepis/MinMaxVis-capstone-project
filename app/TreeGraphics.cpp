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
    scene->clear();  // here we are clearing the tree
    Tree_withBoards(600, 50, 3, 0, gameInstance->Retrieve_stateofBoard());
}

void TreeGraphics::Tree_withBoards(int x, int y, int depth, int level, PieceType** state) {
    if (depth == 0 || state == nullptr) {
        return;
    }

    // here we are creating a new TTT Board deppending on the cureent move
    TicTacToe* board = new TicTacToe();
    board->update_stateofBoard(state);  // here the board state is being applied
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            QLayoutItem *item = board->grid->itemAtPosition(i, j);
            if (item) {
                QWidget *widget = item->widget();
                if (QPushButton *button = qobject_cast<QPushButton*>(widget)) {
                    button->setDisabled(true);  // we are disbling so that we can't make moves in the board in the tree
                }
            }
        }
    }

    QGraphicsProxyWidget* proxy = scene->addWidget(board);
    proxy->setScale(0.5);
    proxy->setPos(x, y);

    std::vector<std::pair<int, int>> possibleMoves = gameInstance->find_possiblemove();

    for (size_t i = 0; i < possibleMoves.size(); i++) {
        int childX = x + (i - possibleMoves.size() / 2) * 100;
        PieceType** newState = gameInstance->Visualize_Move(state, possibleMoves[i].first, possibleMoves[i].second);

        if (newState) {
            Tree_withBoards(childX, y + 120, depth - 1, level + 1, newState);
        }
    }
}