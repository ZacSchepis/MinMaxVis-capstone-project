#include "tictactoe.h"
#include <cstdlib>
#include <climits>
#include <QTimer>

PieceType** TicTacToe::Retrieve_stateofBoard() {
    return Board::Retrieve_stateofBoard();
}

std::vector<std::pair<int, int>> TicTacToe::find_possiblemove() {
    std::vector<std::pair<int, int>> moves;
    PieceType** state = this->Retrieve_stateofBoard();

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (state[i][j] == empty_state) {
                moves.push_back({i, j});
            }
        }
    }
    return moves;
}

void TicTacToe::update_stateofBoard(PieceType** next_stateofBoard) {
    this->Board::update_stateofBoard(next_stateofBoard);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            update_cell(i, j);
        }
    }
}

PieceType** TicTacToe::Visualize_Move(PieceType** state, int r, int c) {
    if (!state) {
        return nullptr;
    }

    PieceType** next_stateofBoard = new PieceType*[3];
    for (int i = 0; i < 3; i++) {
        next_stateofBoard[i] = new PieceType[3];
        for (int j = 0; j < 3; j++) {
            next_stateofBoard[i][j] = state[i][j];
        }
    }

    next_stateofBoard[r][c] = P1;
    return next_stateofBoard;
}

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

void TicTacToe::Player_move(int row, int column) {
    if (get_piece_at(row, column) == empty_state) {
        place(row, column, P1);
        emit move_Executed();

        if (Findout_Win(P1)) {
            Findout_End("Congrats! You have won the game!");
        } else if (Findout_draw()) {
            Findout_End("It's a draw!");
        } else {
            emit update_tree_Visualization();
            QTimer::singleShot(500, this, &TicTacToe::Computer_move);
        }
    } else {
        QMessageBox::warning(this, "Invalid Move", "This cell is already occupied!");
    }
}

void TicTacToe::Computer_move() {
    std::pair<int, int> IdealMove = move_bestcalculation();
    if (IdealMove.first != -1) {
        place(IdealMove.first, IdealMove.second, P2);
        emit move_Executed();

        if (Findout_Win(P2)) {
            Findout_End("The computer has won.");
        } else if (Findout_draw()) {
            Findout_End("It's a draw.");
        }
    }
}

bool TicTacToe::Findout_Win(PieceType player) {
    for (int i = 0; i < 3; i++) {
        if (get_piece_at(i, 0) == player && get_piece_at(i, 1) == player && get_piece_at(i, 2) == player) return true;
        if (get_piece_at(0, i) == player && get_piece_at(1, i) == player && get_piece_at(2, i) == player) return true;
    }
    if (get_piece_at(0, 0) == player && get_piece_at(1, 1) == player && get_piece_at(2, 2) == player) return true;
    if (get_piece_at(0, 2) == player && get_piece_at(1, 1) == player && get_piece_at(2, 0) == player) return true;
    return false;
}

bool TicTacToe::Findout_draw() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (get_piece_at(i, j) == empty_state) return false;
        }
    }
    return true;
}

void TicTacToe::Findout_End(const QString& result) {
    QMessageBox msgBox;
    msgBox.setWindowTitle("Game Over");
    msgBox.setText(result);
    msgBox.setInformativeText("Do you want to start a new game?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();
    if (ret == QMessageBox::Yes) {
        Restart_Game();
    }
}

void TicTacToe::Restart_Game() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            place(i, j, empty_state);
        }
    }
    emit move_Executed();
}

int TicTacToe::MinMax(int recursionLevel, bool isMaximizing, int alpha, int beta) {
    if (Findout_Win(P2)) return 10;
    if (Findout_Win(P1)) return -10;
    if (Findout_draw()) return 0;

    if (isMaximizing) {
        int Best_Score = INT_MIN;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (get_piece_at(i, j) == empty_state) {
                    place(i, j, P2);
                    int Score = MinMax(recursionLevel + 1, false, alpha, beta);
                    place(i, j, empty_state);
                    Best_Score = std::max(Score, Best_Score);
                    alpha = std::max(alpha, Score);
                    if (beta <= alpha) break;
                }
            }
        }
        return Best_Score;
    } else {
        int Worst_Score = INT_MAX;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (get_piece_at(i, j) == empty_state) {
                    place(i, j, P1);
                    int Score = MinMax(recursionLevel + 1, true, alpha, beta);
                    place(i, j, empty_state);
                    Worst_Score = std::min(Score, Worst_Score);
                    beta = std::min(beta, Score);
                    if (beta <= alpha) break;
                }
            }
        }
        return Worst_Score;
    }
}

std::pair<int, int> TicTacToe::move_bestcalculation() {
    int Best_Score = INT_MIN;
    std::pair<int, int> IdealMove = {-1, -1};
    int alpha = INT_MIN;
    int beta = INT_MAX;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (get_piece_at(i, j) == empty_state) {
                place(i, j, P2);
                int Score = MinMax(0, false, alpha, beta);
                place(i, j, empty_state);
                if (Score > Best_Score) {
                    Best_Score = Score;
                    IdealMove = {i, j};
                }
            }
        }
    }
    return IdealMove;
}
