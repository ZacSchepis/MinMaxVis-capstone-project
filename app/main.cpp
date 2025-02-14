#include "mainwindow.h"
<<<<<<< HEAD
#include <QApplication>
#include <QPushButton>
#include <QTimer>
// #include "sample_board.h"
//#include "tictactoe.h"
#include "checkers.h"
#include <windows.h>
=======
#include "QApplication"
#include "QPushButton"
#include "TreeGraphics.h"
#include "tictactoe.h"

>>>>>>> origin/main
int main(int argc, char *argv[]) {
//#ifdef _WIN32
//    if (AttachConsole(ATTACH_PARENT_PROCESS)) {
//        freopen("CONOUT$", "w", stdout);
//        freopen("CONOUT$", "w", stderr);
//    }
//#endif
    QApplication app(argc, argv);
    MainWindow window;
    window.setWindowTitle("MinMax Visualizers");
<<<<<<< HEAD
    Checkers board_game(&window);
    // SampleBoard board_game(&window);
    // Placing P1 and P2 pieces on a TTT board
//    board_game.place(1,0, P1);
//    board_game.place(1,2, P2);
//     This will set a space to be empty
    // board_game.place(2,2, empty_state);
    board_game.populate_board();
=======

    TicTacToe board_game(&window);
    TreeGraphics treeWidget;
    treeWidget.show();
>>>>>>> origin/main

    window.setCentralWidget(&board_game);
    int c_ = 5;
//    board_game.take
//    QTimer::singleShot(5000, &board_game, [=]{
//        board_game.
//    });
    board_game.turns(1000);

//    time
//    while(true){
//        board_game.take_turn();
//    }
    window.show();

    return app.exec();
}
