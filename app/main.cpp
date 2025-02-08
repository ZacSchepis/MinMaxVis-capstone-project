#include "mainwindow.h"
#include <QApplication>
#include <QPushButton>

// #include "sample_board.h"
#include "tictactoe.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.setWindowTitle("MinMax Visualizers");
    TicTacToe board_game(&window);
    // SampleBoard board_game(&window);
    // Placing P1 and P2 pieces on a TTT board
    board_game.place(1,0, P1);
    board_game.place(1,2, P2);
    // This will set a space to be empty
    // board_game.place(2,2, empty_state);
    window.setCentralWidget(&board_game);
    window.show();
    return app.exec();
}
