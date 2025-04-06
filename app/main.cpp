#include "mainwindow.h"
#include <QApplication>
#include <QPushButton>
#include <QTimer>
// #include "sample_board.h"
// #include "tictactoe.h"
 #include "checkers.h"
#include <windows.h>

#include "QApplication"
#include "QPushButton"
#include "TreeGraphics.h"
#include "tictactoe.h"

int main(int argc, char *argv[]) {
#ifdef _WIN32
    if (AttachConsole(ATTACH_PARENT_PROCESS)) {
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
    }
#endif
    QApplication app(argc, argv);
    MainWindow window;
    //    window.setWindowTitle("MinMax Visualizers");

    /*Tic Tac Toe Game Code*/
    // TicTacToe board_game(&window);
    // TreeGraphics treeWidget(nullptr, &board_game);
    // QObject::connect(&board_game, &TicTacToe::update_tree_Visualization, &treeWidget, &TreeGraphics::update_Tree);
    // QObject::connect(&board_game, &TicTacToe::move_Executed, &treeWidget, &TreeGraphics::update_Tree);
    // treeWidget.show();
    /*----------------------------------*/

    /*Checkers Game Run*/
    Checkers board_game;
    board_game.populate_board(true);
    /*------------------------------------*/

    //    board_game.turns(20);
    //    board_game.turns(200);
    window.setCentralWidget(&board_game);
    window.show();
    //    board_game.find_best_move();

    return app.exec();
}