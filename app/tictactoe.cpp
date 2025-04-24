#include "tictactoe.h"
#include <cstdlib>
#include <climits>
#include <QTimer>
#include <QLabel>

TicTacToe::TicTacToe(QWidget *parent, bool enableRightWidget)
    : Board(parent, 3, 3, enableRightWidget) {

    this->map_piece(P1, ":/res/x_tictactoe32px.png");
    this->map_piece(P2, ":/res/o_tictactoe32px.png");
    if (getRightLayout()) {
    SetUpRightWidget();

    }
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            QPushButton* button = qobject_cast<QPushButton*>(grid->itemAtPosition(i, j)->widget());
            connect(button, &QPushButton::clicked, [this, i, j]() {
                this->Player_move(i, j);
            });
        }
    }
}

void TicTacToe::SetUpRightWidget() {
    QString labelStyle = "color: black; font-size: 12px;";
    QString buttonStyle = "background-color: black; color: white; font-size: 12px; padding: 4px;";

    boardScoreLabel = new QLabel("Board Score:", this);
    boardScoreLabel->setStyleSheet(labelStyle);
    getRightLayout()->addWidget(boardScoreLabel);

    bestPlayerMoveLabel = new QLabel("Best Move for Player:", this);
    bestPlayerMoveLabel->setStyleSheet(labelStyle);
    getRightLayout()->addWidget(bestPlayerMoveLabel);

    bestComputerMoveLabel = new QLabel("Best Move for Computer:", this);
    bestComputerMoveLabel->setStyleSheet(labelStyle);
    getRightLayout()->addWidget(bestComputerMoveLabel);

    QPushButton* previewNextMoveButton = new QPushButton("Preview Next Move", this);
    previewNextMoveButton->setStyleSheet(buttonStyle);
    previewNextMoveButton->setFixedHeight(28);
    getRightLayout()->addWidget(previewNextMoveButton);
    connect(previewNextMoveButton, &QPushButton::clicked, this, &TicTacToe::PreviewNextMove);

    QPushButton* clearPreviewButton = new QPushButton("Clear Preview", this);
    clearPreviewButton->setStyleSheet(buttonStyle);
    clearPreviewButton->setFixedHeight(28);
    getRightLayout()->addWidget(clearPreviewButton);
    connect(clearPreviewButton, &QPushButton::clicked, this, &TicTacToe::ClearPreview);

    QPushButton* computerMoveButton = new QPushButton("Computer Move", this);
    computerMoveButton->setStyleSheet("background-color: darkblue; color: white; font-size: 12px; padding: 4px;");
    computerMoveButton->setFixedHeight(28);
    getRightLayout()->addWidget(computerMoveButton);
    connect(computerMoveButton, &QPushButton::clicked, this, &TicTacToe::Computer_move);
}


void TicTacToe::Player_move(int row, int column) {
    if (currentTurn != P1) {
        QMessageBox::information(this, "Wait", "It's not your turn!");
        return;
    }
    if (get_piece_at(row, column) == empty_state) {
        place(row, column, P1);
        currentTurn = P2;
        emit move_Executed();
        emit update_tree_Visualization();
        updateBoardScore();
        updateBestMoves();

        if (Findout_Win(P1)) {
            Findout_End("Congrats! You have won the game!");
        } else if (Findout_draw()) {
            Findout_End("It's a draw!");
        } else {
            emit update_tree_Visualization();
        }
    } else {
        QMessageBox::warning(this, "Invalid Move", "This cell is already occupied!");
    }

}

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

PieceType** TicTacToe::Visualize_Move(PieceType** state, int r, int c, PieceType player) {
    if (!state) return nullptr;

    PieceType** next_stateofBoard = new PieceType*[3];
    for (int i = 0; i < 3; i++) {
        next_stateofBoard[i] = new PieceType[3];
        for (int j = 0; j < 3; j++) {
            next_stateofBoard[i][j] = state[i][j];
        }
    }

    next_stateofBoard[r][c] = player;
    return next_stateofBoard;
}

void TicTacToe::Computer_move() {
    if (currentTurn != P2) {
        QMessageBox::information(this, "Wait", "It's not the computer's turn.");
        return;
    }

    std::pair<int, int> IdealMove = move_bestcalculation();
    if (IdealMove.first != -1) {
        place(IdealMove.first, IdealMove.second, P2);
        emit move_Executed();
        updateBoardScore();
        updateBestMoves();

        if (Findout_Win(P2)) {
            Findout_End("The computer has won.");
        } else if (Findout_draw()) {
            Findout_End("It's a draw.");
        } else {
            currentTurn = P1;
            emit update_tree_Visualization();
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
        currentTurn = P1;
    }
}

void TicTacToe::Restart_Game() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            place(i, j, empty_state);
        }
    }
    currentTurn = P1;
    emit move_Executed();
    updateBoardScore();
    updateBestMoves();
    emit update_tree_Visualization();
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

                alpha = std::max(alpha, Score);
                if (beta <= alpha)
                    break;
            }
        }
    }
    return IdealMove;
}


int TicTacToe::updateBoardScore() {
    int score = MinMax(0, true, INT_MIN, INT_MAX);
    return score;
}

void TicTacToe::updateBestMoves() {
    if (!getRightLayout()) return;

    std::pair<int, int> playerMove = {-1, -1};
    std::pair<int, int> computerMove = {-1, -1};
    int bestScore = INT_MAX;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (get_piece_at(i, j) == empty_state) {
                place(i, j, P1);
                int score = MinMax(0, true);
                place(i, j, empty_state);
                if (score < bestScore) {
                    bestScore = score;
                    playerMove = {i, j};
                }
            }
        }
    }
    computerMove = move_bestcalculation();
    if (bestPlayerMoveLabel)
        bestPlayerMoveLabel->setText(QString("Best Move for Player: (%1, %2)").arg(playerMove.first).arg(playerMove.second));

    if (bestComputerMoveLabel)
        bestComputerMoveLabel->setText(QString("Best Move for Computer: (%1, %2)").arg(computerMove.first).arg(computerMove.second));
}

void TicTacToe::PreviewNextMove() {
    PieceType** currentState = Retrieve_stateofBoard();

    // Save current state in previewHistory for ClearPreview
    PieceType** backup = new PieceType*[3];
    for (int i = 0; i < 3; ++i) {
        backup[i] = new PieceType[3];
        for (int j = 0; j < 3; ++j) {
            backup[i][j] = currentState[i][j];
        }
    }
    previewHistory.push_back(backup);

    // Deep copy currentState to previewBoard
    PieceType** previewBoard = new PieceType*[3];
    for (int i = 0; i < 3; i++) {
        previewBoard[i] = new PieceType[3];
        for (int j = 0; j < 3; j++) {
            previewBoard[i][j] = currentState[i][j];
        }
    }

    // Get next move based on whose turn it is
    std::pair<int, int> move = {-1, -1};
    if (currentTurn == P2) {
        move = move_bestcalculation();
    } else {
        int bestScore = INT_MAX;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (previewBoard[i][j] == empty_state) {
                    previewBoard[i][j] = P1;
                    int score = MinMax(0, true);
                    previewBoard[i][j] = empty_state;
                    if (score < bestScore) {
                        bestScore = score;
                        move = {i, j};
                    }
                }
            }
        }
    }

    if (move.first != -1) {
        previewBoard[move.first][move.second] = currentTurn;
    }

    update_stateofBoard(previewBoard);
    // Flip turn after preview
    currentTurn = (currentTurn == P1) ? P2 : P1;


    for (int i = 0; i < 3; ++i) {
        delete[] previewBoard[i];
    }
    delete[] previewBoard;
}


void TicTacToe::ClearPreview() {
    if (previewHistory.empty()) return;

    PieceType** previous = previewHistory.back();
    previewHistory.pop_back();

    update_stateofBoard(previous);

    for (int i = 0; i < 3; ++i) {
        delete[] previous[i];
    }
    delete[] previous;

    emit move_Executed();
    updateBoardScore();
    updateBestMoves();
}