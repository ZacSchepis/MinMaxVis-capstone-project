#include "mainwindow.h"
#include "QApplication"
#include "QPushButton"
#include "TreeGraphics.h"
#include "tictactoe.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.setWindowTitle("MinMax Visualizers");

    TicTacToe board_game(&window);
    TreeGraphics treeWidget;
    treeWidget.show();

    window.setCentralWidget(&board_game);
    window.show();

    return app.exec();
}
