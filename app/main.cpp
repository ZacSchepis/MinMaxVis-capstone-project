#include "mainwindow.h"
#include <QApplication>
#include "tictactoe.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.setWindowTitle("MinMax Visualizers");

    TicTacToe board_game(&window);
    window.setCentralWidget(&board_game);
    window.show();

    return app.exec();
}
