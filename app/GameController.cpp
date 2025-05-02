//
// Created by XxGam on 5/1/2025.
//

#include "GameController.h"
#include "TreeGraphics.h"
GameController::GameController(QWidget* parent, Board* gameRef) : QWidget{parent}, game(gameRef) {
    auto *ly = new QGridLayout();
//    ly->setRowStretch(1,1);
//    ly->setSpacing(20);
//    ly->setHorizontalSpacing(20);
//    ly->setVerticalSpacing(40);
//    ly->setContentsMargins(0, 0, 0, 10);
    if (!gameRef) {
        auto *tttButton = new QPushButton("TicTacToe");
        tttButton->setObjectName("menuButton");
        auto *cheButton = new QPushButton("Checkers");
        cheButton->setObjectName("menuButton");
        ly->addWidget(tttButton, 1, 0);
        ly->addWidget(cheButton, 2, 0);
        connect(tttButton, &QPushButton::clicked, this, [=]() {
            if (game) {
                ly->removeWidget(game);
                delete game;
                game = nullptr;
            }
            TicTacToe* ttt = new TicTacToe(this);
            game = ttt;
            ttt->setStyleSheet(QString("background-color: #00a4c9;"));
            TreeGraphics* treeWidget = new TreeGraphics(nullptr,ttt);
            treeWidget->setAttribute(Qt::WA_DeleteOnClose);
            connect(ttt, &TicTacToe::update_tree_Visualization, treeWidget, &TreeGraphics::update_Tree);
            connect(ttt, &TicTacToe::move_Executed, treeWidget, &TreeGraphics::update_Tree);
            treeWidget->show();
            game->callbackSetup();
            ly->addWidget(game, 0, 0, 1, 1);
            game->show();
        });

        connect(cheButton, &QPushButton::clicked, this, [=]() {
            if (game) {
                ly->removeWidget(game);
                delete game;
            }
            game = new Checkers(this);
            game->callbackSetup();
            ly->addWidget(game, 0, 0, 1, 1 );
//            game->grid->widget()->setStyleSheet("align-items: center;");
            game->show();
        });

    }

    // 🔥 THIS is what you're missing
    this->setLayout(ly);
}


