
#include "mainwindow.h"
#include <QApplication>
#include "tictactoe.h"
#include "TreeGraphics.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.setWindowTitle("MinMax Visualizer");

    TicTacToe board_game(&window);
    TreeGraphics treeWidget(nullptr, &board_game);

    QObject::connect(&board_game, &TicTacToe::update_tree_Visualization, &treeWidget, &TreeGraphics::update_Tree);
    QObject::connect(&board_game, &TicTacToe::move_Executed, &treeWidget, &TreeGraphics::update_Tree);






    treeWidget.show();
    window.setCentralWidget(&board_game);
    window.show();

    return app.exec();
}
