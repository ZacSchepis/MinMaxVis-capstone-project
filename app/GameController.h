//
// Created by XxGam on 5/1/2025.
//

#ifndef APP_GAMECONTROLLER_H
#define APP_GAMECONTROLLER_H

#include <QGridLayout>

#include <QWidget>
#include <QLayout>
#include "board.h"
#include "tictactoe.h"
#include "checkers.h"
class GameController : public QWidget {
    Q_OBJECT
    public:
    QGridLayout *grid;

    explicit GameController(QWidget *parent= nullptr, Board *gameRef= nullptr);
//        explicit Board(QWidget *parent = nullptr, int r = 3, int c = 3,bool showRightPanel = false);



    private:
        Board* game;


};





#endif //APP_GAMECONTROLLER_H
