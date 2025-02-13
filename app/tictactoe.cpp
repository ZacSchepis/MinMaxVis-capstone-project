#include "tictactoe.h"
#include <cstdlib>
#include <climits>

TicTacToe::TicTacToe(QWidget *parent) : Board(parent, 3, 3) {
    this->map_piece(P1, ":/res/x_tictactoe32px.png");
    this->map_piece(P2, ":/res/o_tictactoe32px.png");

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            QPushButton* button = qobject_cast<QPushButton*>(grid->itemAtPosition(i, j)->widget());
            connect(button, &QPushButton::clicked, [this, i, j]() {
                this->Player_move(i, j);
            });
        }
    }
}

// this function manages a player's move.
void TicTacToe::Player_move(int row, int column) {
    if (get_piece_at(row, column) == empty_state) {
        place(row, column, P1);
        if (Findout_Win(P1)) {
            Findout_End("Congrats. You have won the game!!!!!");
        } else if (Findout_draw()) {
            Findout_End("It's a draw!!!!");
        } else {
            Computer_move();
        }
    } else {
        QMessageBox::warning(this, "Please make a valid move", "This cell is already occupied!");
    }
}


// this function manages the computer's move.
void TicTacToe::Computer_move() {
    std::pair<int, int> IdealMove = BestMoveCalculation();
    if (IdealMove.first != -1) { // This will dtect the possible move in the game
        place(IdealMove.first, IdealMove.second, P2);
        if (Findout_Win(P2)) {
            Findout_End("The computer has won the game.");
        } else if (Findout_draw()) {
            Findout_End("It's a draw.");
        }
    }
}

// Sansar 02/08/2025
// This function helps in the detection of the win by detecting if the player have achieved three pieces in the diagonal, row or column
bool TicTacToe::Findout_Win(PieceType player) {
    for (int i = 0; i < 3; i++) {
        if (get_piece_at(i, 0) == player && get_piece_at(i, 1) == player && get_piece_at(i, 2) == player) return true;
        if (get_piece_at(0, i) == player && get_piece_at(1, i) == player && get_piece_at(2, i) == player) return true;
    }
    if (get_piece_at(0, 0) == player && get_piece_at(1, 1) == player && get_piece_at(2, 2) == player) return true;
    if (get_piece_at(0, 2) == player && get_piece_at(1, 1) == player && get_piece_at(2, 0) == player) return true;
    return false;
}


// This function detects the draw in the game
bool TicTacToe::Findout_draw() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (get_piece_at(i, j) == empty_state) return false;
        }
    }
    return true;
}


// This function asks the player when they wish to begin a new game and shows a message box with the round's result.
void TicTacToe::Findout_End(const QString& result) {
    QMessageBox msgBox;
    msgBox.setWindowTitle("!!!Game Over!!!");
    msgBox.setText(result);
    msgBox.setInformativeText("Do you want to start a new game?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();
    if (ret == QMessageBox::Yes) {
        Restart_Game();
    }
}


// This function will help in restarting of the game if the user wants to restart it will empty all the cells in the board
void TicTacToe::Restart_Game() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            place(i, j, empty_state);
        }
    }
}


// Here we are implementing the mini-max algorithm recursively and calculating the best move by checking the future possible best move
int TicTacToe::MinMax(int recursionLevel, bool isMaximizing) {
    if (Findout_Win(P2)) return 10;
    if (Findout_Win(P1)) return -10;
    if (Findout_draw()) return 0;

    if (isMaximizing) {
        int MaxScore = INT_MIN;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (get_piece_at(i, j) == empty_state) {
                    place(i, j, P2);
                    int Evaluation = MinMax(recursionLevel + 1, false);
                    place(i, j, empty_state);
                    MaxScore = std::max(Evaluation, MaxScore);
                }
            }
        }
        return MaxScore;
    } else {
        int MaxScore = INT_MAX;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (get_piece_at(i, j) == empty_state) {
                    place(i, j, P1);
                    int Evaluation = MinMax(recursionLevel + 1, true);
                    place(i, j, empty_state);
                    MaxScore = std::min(Evaluation, MaxScore);
                }
            }
        }
        return MaxScore;
    }
}

// This function doesthe evaluation all the possible move and give us the best move using the min-max algorithm
std::pair<int, int> TicTacToe::BestMoveCalculation() {
    int MaxScore = INT_MIN;
    std::pair<int, int> IdealMove = {-1, -1};
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (get_piece_at(i, j) == empty_state) {
                place(i, j, P2);
                int Evaluation = MinMax(0, false);
                place(i, j, empty_state);
                if (Evaluation > MaxScore) {
                    MaxScore = Evaluation;
                    IdealMove = {i, j};
                }
            }
        }
    }
    return IdealMove;
}
