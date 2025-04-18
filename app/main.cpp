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
//    TicTacToe board_game(&window);
//    TreeGraphics treeWidget;
//    treeWidget.show();
    Checkers board_game;
    board_game.populate_board(true);
    window.setCentralWidget(&board_game);
    window.show();

    return app.exec();
}
