#ifndef TICTACTOE_H
#define TICTACTOE_H

#include "board.h"
#include <QPushButton>
#include <QMessageBox>
#include <utility>
#include <vector>

class TicTacToe : public Board {
    Q_OBJECT

public:
    explicit TicTacToe(QWidget *parent = nullptr,  bool enableRightWidget = true);
    void update_stateofBoard(PieceType** newState);
    PieceType** Retrieve_stateofBoard(); // Returns the current board state
    std::vector<std::pair<int, int>> find_possiblemove(); // Returns possible moves
    PieceType** Visualize_Move(PieceType** state, int r, int c,PieceType player); // Simulates a move for visualization
    int MinMax(int recursionLevel, bool isMaximizing, int alpha = INT_MIN, int beta = INT_MAX);
    void PreviewNextMove();


    signals:
        void move_Executed();
    void update_tree_Visualization();

private:
    void Player_move(int r, int c);
    void Computer_move();
    bool Findout_Win(PieceType player);
    bool Findout_draw();
    void Findout_End(const QString& result);
    void Restart_Game();
    std::pair<int, int> move_bestcalculation();
    QLabel* boardScoreLabel;
    QLabel* bestPlayerMoveLabel;
    QLabel* bestComputerMoveLabel;
    int updateBoardScore();
    void updateBestMoves();
    std::vector<PieceType**> previewHistory;
    void ClearPreview();
    PieceType currentTurn = P1;




};

#endif // TICTACTOE_H