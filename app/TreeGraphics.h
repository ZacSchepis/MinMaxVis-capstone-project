#ifndef TREEGRAPHICS_H
#define TREEGRAPHICS_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "tictactoe.h"

class TreeGraphics : public QWidget {
    Q_OBJECT

public:
    /**
    * @brief Constructs the TreeGraphics widget which visualizes the Min-Max game tree.
    *
    * This constructor initializes the QGraphicsScene and QGraphicsView that form the canvas
    * for rendering the recursive game tree of Tic-Tac-Toe boards. If a TicTacToe game instance
    * is provided, it connects the game's `move_Executed` signal to the `update_Tree()` slot,
     * so the visualization updates dynamically after each move.
    *
    * @param parent The parent QWidget.
    * @param game A pointer to the current TicTacToe game instance. If provided,
     *        the visual tree will represent its state and update as the game progresses.
     */
    explicit TreeGraphics(QWidget* parent = nullptr, TicTacToe* game = nullptr);

    public slots:
    /**
     * @brief Updates the Min-Max visualization tree for the current game state.
    *
    * This function clears the current QGraphicsScene and recomputes the best move
    * using the Min-Max algorithm for the current player (`P1` or `P2`). It iterates
    * over all valid empty cells, simulates placing the current player's piece,
    * evaluates the board using `MinMax`, and reverts the move. Based on whether the
    * current player is maximizing (`P2`) or minimizing (`P1`), it selects the move
    * that yields the best score.
    *
    * After determining the best move, it initiates a recursive rendering of the
    * game tree using `Tree_withBoards`, starting from the current board state.
    */
        void update_Tree();

private:
    QGraphicsScene* scene;
    QGraphicsView* view;
    TicTacToe* gameInstance;
    /**
    * @brief Draws a visual connection line between two game boards in the tree.
    *
    * This function adds a line to the scene to represent the parent-child relationship
    * between two board states in the Min-Max tree. The line is color-coded based on the
    * current player:
    *   - Red for Player 1 (P1)
    *   - Green for Player 2 (P2)
    *
    * The line is drawn behind the game boards for clear visibility.
    *
    * @param parent The position of the parent board (center point).
    * @param child The position of the child board (center point).
    * @param currentPlayer Indicates which player's move led to the child node.
    */
    void drawConnectionLine(QPointF parent, QPointF child, PieceType currentPlayer);
    /**
    * @brief Highlights the best move on a specific board in the game tree.
    *
    * This function visually emphasizes the node in the Min-Max tree that corresponds to
    * the best move for the current player. If the current move matches the best move:
    *
     * - A colored border (red for Player, green for Computer) is drawn around the board.
     * - An info button ("ℹ️") is placed at the corner of the board.
     * - Clicking the info button displays a popup with the move coordinates and board score
     *   as evaluated by the Min-Max algorithm.
     *
     * @param proxy The QGraphicsProxyWidget containing the board widget to be highlighted.
     * @param bestMove The best move determined by the Min-Max algorithm.
     * @param currentMove The move represented by the current board being rendered.
     * @param currentPlayer Indicates whether the current board is a result of Player or Computer's turn.
     */
    void highlightBestMove(QGraphicsProxyWidget* proxy,
                                     const std::pair<int, int>& bestMove,
                                     const std::pair<int, int>& currentMove,
                                     PieceType currentPlayer);
    /**
    * @brief Recursively builds and visualizes the Min-Max game tree.
    *
    * This function creates a disabled Tic-Tac-Toe board widget for the given state
    * and adds it to the QGraphicsScene. It draws a tree structure by recursively
    * generating child boards for each possible move. Visual cues include:
    *
    * - Connecting lines between parent and child boards.
    * - Highlighting the best move at the first level (in red or green).
    *
    * Key parameters:
    * - `x`, `y`: Position in the scene for the current board.
    * - `depth`: Remaining recursion depth.
    * - `level`: Current tree level (root is 0).
    * - `state`: Current board state to visualize.
    * - `bestMove`: The optimal move as determined by Min-Max.
    * - `currentMove`: The move that led to this board.
    * - `currentPlayer`: The player whose move created this state.
    * - `parentPos`: Position of the parent board to draw connector lines.
    */
    void Tree_withBoards(int x, int y, int depth, int level,
                         PieceType** state, std::pair<int, int> bestMove,
                         std::pair<int, int> currentMove = {-1, -1},
                         PieceType currentPlayer = P1,
                         QPointF parentPos = QPointF(-1, -1));

    struct TreeNode {
        int x, y, depth, level;
        PieceType** state;
        std::pair<int, int> bestMove;
        std::pair<int, int> currentMove;
        PieceType currentPlayer;
        QPointF parentPos;
    };

    void Tree_withBoards_Iterative(int startX, int startY, int maxDepth);
};

#endif // TREEGRAPHICS_H
