#include "GameController.h"  // your custom controller
#include "mainwindow.h"
#include <QApplication>
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.setFixedSize(600,800);
    GameController controller;
    window.setStyleSheet(R"(
    QMainWindow {
        background-color: #1e1e2f;
        border: 8px solid qlineargradient(
            x1:0, y1:0, x2:1, y2:1,
            stop:0 #7f5af0,
            stop:0.5 #5dfdcf,
            stop:1 #7f5af0
        );
        border-radius: 20px;
        padding: 12px;
    }

    QWidget {
        font-family: 'Segoe UI';
        font-size: 14px;
    }

    QPushButton#menuButton {
        background-color: #5dfdcf;
        border: 2px solid #7f5af0;
        border-radius: 10px;
        padding: 6px 12px;
        color: black;
        font-weight: bold;
    }

    QPushButton#menuButton:hover {
        background-color: #7f5af0;
        color: white;
    }

    QPushButton#menuButton:pressed {
        background-color: #372a75;
        color: #ffffff;
    }
)");
    window.setWindowTitle("MinMax Visualizers - TicTacToe + Checkers");
//    Checkers checkers;
//    checkers.populate_board(true);
    window.setCentralWidget(&controller);
    window.show();

    return app.exec();
}

