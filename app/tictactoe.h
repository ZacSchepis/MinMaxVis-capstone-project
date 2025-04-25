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
    /**
    * @brief TicTacToe::TicTacToe
    * Constructor for the TicTacToe class. Initializes a 3x3 game board,
    * assigns visual icons to each player piece (X and O), sets up the right
    * control panel with score and move suggestions (if enabled), and connects
    * all grid buttons to the player's move handler.
    *
    * @param parent QWidget parent (usually the main window or container)
    * @param enableRightWidget Boolean flag indicating whether to show the right-side
    *        UI panel (with board score, best moves, and control buttons)
    */
    explicit TicTacToe(QWidget *parent = nullptr,  bool enableRightWidget = true);

    /**
 * @brief update_stateofBoard
 * Updates the internal and visual representation of the board using a new state.
 * Delegates the state update to the base Board class and then refreshes each
 * individual cell visually to reflect the changes.
 *
 * @param next_stateofBoard A 2D array (3x3) representing the new board state to apply.
 */
    void update_stateofBoard(PieceType** newState);

    /**
    * @brief Retrieve_stateofBoard
    * Returns a pointer to the current internal state of the game board.
    * Delegates the call to the base Board class.
    *
    * @return A 2D array (3x3) representing the board's current state.
    */
    PieceType** Retrieve_stateofBoard();

    /**
    * @brief find_possiblemove
    * Scans the current board state and identifies all legal (empty) moves.
    * Returns a list of available cell coordinates where the player or computer
    * can make a move.
    *
    * @return A vector of (row, column) pairs representing all empty cells.
    */
    std::vector<std::pair<int, int>> find_possiblemove();

    /**
    * @brief Visualize_Move
    * Simulates a move on a given board state without modifying the original.
    * Creates a deep copy of the input state and applies the specified player's move
    * at the given (row, column) coordinates.
    *
    * @param state The current board state to clone and modify.
    * @param r The row index where the move is to be applied.
    * @param c The column index where the move is to be applied.
    * @param player The piece type (P1 or P2) to place on the board.
    *
    * @return A new 3x3 board state reflecting the move, or nullptr if input state is invalid.
    */
    PieceType** Visualize_Move(PieceType** state, int r, int c,PieceType player);

    /**
    * @brief MinMax
    * Implements the MinMax algorithm with Alpha-Beta pruning to evaluate the optimal
    * move score for the current board state. This recursive function simulates all
    * possible future moves for both players and chooses the path that leads to the best
    * outcome, minimizing or maximizing the score accordingly.
    *
    * - A win for the computer (P2) returns +10.
    * - A win for the player (P1) returns -10.
    * - A draw returns 0.
    *
    * The function explores the game tree by placing virtual moves, evaluating recursively,
    * and undoing the move after evaluation. Alpha-Beta pruning is used to skip unnecessary
    * branches and optimize performance.
    *
    * @param recursionLevel Current depth in the recursive tree (primarily for tracking).
    * @param isMaximizing True if it's the maximizing player's (P2) turn, false for minimizing (P1).
    * @param alpha The best already explored option along the path to the maximizer.
    * @param beta The best already explored option along the path to the minimizer.
    *
    * @return The best score found for the current player in this game state.
    */
    int MinMax(int recursionLevel, bool isMaximizing, int alpha = INT_MIN, int beta = INT_MAX);

    /**
    * @brief PreviewNextMove
    * Simulates the next two optimal moves: one for the computer (P2), followed by the best
    * possible response from the player (P1), and updates the board visually to reflect
    * this preview sequence.
    *
    * - Stores the current board state in a preview history stack for reversal.
    * - Applies the computer's best move using MinMax.
    * - Then simulates the player's best counter-move.
    * - Updates the visual board to show the resulting hypothetical state.
    *
    * This function is used for educational or strategic insight to help users
    * understand the MinMax evaluation process.
    */
    void PreviewNextMove();

    PieceType currentTurn = P1;
    /**
    * @brief move_bestcalculation
    * Determines the optimal move for the computer (P2) by simulating all possible moves
    * on the current board and evaluating each using the MinMax algorithm with Alpha-Beta pruning.
    * Tracks the move with the highest score and returns it as the ideal choice.
    *
    * This function is typically called during the computer's turn to decide its next action.
    *
    * @return A pair of integers (row, column) representing the best move for the computer.
    *         Returns {-1, -1} if no valid moves are available.
    */
    std::pair<int, int> move_bestcalculation();

    signals:
        void move_Executed();
    void update_tree_Visualization();

private:
    /**
    * @brief Player_move
    * Handles the player's move on the Tic-Tac-Toe board.
    * Validates that it is the player's turn and the selected cell is empty.
    * If valid, places the player's piece (P1), updates game state, UI, and emits
    * signals to refresh the MinMax visualization.
    *
    * Also checks for game-ending conditions (win or draw) after the move and
    * triggers end-game handling if necessary.
    *
    * @param row The row index (0–2) of the cell clicked.
    * @param column The column index (0–2) of the cell clicked.
    */
    void Player_move(int r, int c);

    /**
     * @brief Computer_move
     * Executes the computer's turn using the MinMax algorithm to determine the optimal move.
     * Validates turn order, places the computer's piece (P2), updates the board, and evaluates
     * the game state for a win or draw. If the game continues, control is returned to the player.
     *
     * Emits signals to update the board display and tree visualization after the move.
     */
    void Computer_move();

    /**
    * @brief Findout_Win
    * Checks whether the specified player has won the game.
    * Evaluates all possible winning conditions: horizontal, vertical, and diagonal lines.
    *
    * @param player The player (P1 or P2) to check for a winning condition.
    * @return true if the player has achieved a winning condition; false otherwise.
    */
    bool Findout_Win(PieceType player);

    /**
    * @brief Findout_draw
    * Determines whether the game has ended in a draw.
    * A draw is declared if all cells on the board are filled and no player has won.
    *
    * @return true if the board is full and no winning condition is met; false otherwise.
    */
    bool Findout_draw();

    /**
    * @brief Findout_End
    * Handles the end-of-game scenario by displaying a message box with the result
    * (e.g., win or draw). Prompts the user to start a new game and restarts the board
    * if the user confirms.
    *
    * @param result A string message indicating the outcome of the game.
    */
    void Findout_End(const QString& result);

    /**
    * @brief Restart_Game
    * Resets the game board by clearing all cells to the empty state.
    * Resets the current turn to the player (P1), updates board score and move suggestions,
    * and emits signals to refresh the game UI and visualization tree.
    */
    void Restart_Game();



    /**
    * @brief updateBoardScore
    * Calculates the current board score from the perspective of the computer (P2)
    * using the MinMax algorithm with Alpha-Beta pruning.
    * This score represents how favorable the current game state is for the AI.
    *
    * @return An integer score: higher values favor the computer, lower values favor the player.
    */
    int updateBoardScore();

    /**
    * @brief updateBestMoves
    * Analyzes the current board to determine and display the optimal next move
    * for both the player (P1) and the computer (P2) based on MinMax evaluation.
    *
    * - Simulates all possible moves for the player to find the one with the lowest score.
    * - Uses `move_bestcalculation()` to find the best move for the computer.
    * - Updates the right-side UI labels to show the recommended moves.
    *
    * This function is typically called after each move to assist the player with hints
    * and to show how the AI evaluates the game state.
    */
    void updateBestMoves();

    std::vector<PieceType**> previewHistory;
    /**
    * @brief ClearPreview
    * Reverts the board to its previous state before the last preview move was shown.
    * Retrieves the last saved board state from the preview history stack, restores it,
    * and updates the game UI accordingly.
    *
    * - Frees the memory associated with the preview state.
    * - Emits signals to refresh the board display and best move suggestions.
    *
    * This function is typically used in conjunction with PreviewNextMove().
    */
    void ClearPreview();

    /**
    * @brief SetUpRightWidget
    * Initializes and configures the right-side UI panel for the Tic-Tac-Toe board.
    * Adds labels to display the current board score and suggested moves for both
    * the player and computer. Also creates and connects control buttons for previewing
    * the next move, clearing the preview, and executing the computer's move.
    *
    * This method assumes that the right-side layout (`getRightLayout()`) is already
    * initialized and available. It should only be called if `enableRightWidget` is true.
    */
    void SetUpRightWidget();

    QLabel* boardScoreLabel;
    QLabel* bestPlayerMoveLabel;
    QLabel* bestComputerMoveLabel;
};

#endif // TICTACTOE_H