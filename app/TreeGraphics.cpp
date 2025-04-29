#include "TreeGraphics.h"
#include <QGraphicsProxyWidget>
#include <QPen>
#include <stack>


TreeGraphics::TreeGraphics(QWidget* parent, TicTacToe* game) : QWidget(parent), gameInstance(game) {
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);
    view->setGeometry(10, 10, 1200, 800); // Adjust size to fit expanding tree

    if (gameInstance) {
        connect(gameInstance, &TicTacToe::move_Executed, this, &TreeGraphics::update_Tree);
    }

    update_Tree();
}

void TreeGraphics::update_Tree() {
    scene->clear();

    std::pair<int, int> bestMove = {-1, -1};

    if (gameInstance) {
        PieceType currentPlayer = gameInstance->currentTurn;
        int bestScore = (currentPlayer == P2) ? INT_MIN : INT_MAX;

        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (gameInstance->get_piece_at(i, j) == empty_state) {
                    gameInstance->place(i, j, currentPlayer);
                    int score = gameInstance->MinMax(0, currentPlayer == P2);
                    gameInstance->place(i, j, empty_state);

                    if ((currentPlayer == P2 && score > bestScore) ||
                        (currentPlayer == P1 && score < bestScore)) {
                        bestScore = score;
                        bestMove = {i, j};
                        }
                }
            }
        }
        Tree_withBoards_Iterative(600, 50, 3);
    }
}

void TreeGraphics::drawConnectionLine(QPointF parent, QPointF child, PieceType currentPlayer) {
    // Choose line color based on current player
    QPen linePen = (currentPlayer == P1) ? QPen(Qt::red, 2) : QPen(Qt::green, 2);

    // Create the line item between parent and child centers
    QGraphicsLineItem* connection = scene->addLine(QLineF(parent, child), linePen);
    connection->setZValue(-2); // Make sure it stays behind the boards
}

void TreeGraphics::highlightBestMove(QGraphicsProxyWidget* proxy,
                                     const std::pair<int, int>& bestMove,
                                     const std::pair<int, int>& currentMove,
                                     PieceType currentPlayer) {
    if (currentMove != bestMove) return;

    QRectF widgetRect = proxy->widget()->geometry();
    QRectF borderRect(0, 0, widgetRect.width() * proxy->scale(), widgetRect.height() * proxy->scale());

    QPen highlightPen = (currentPlayer == P1) ? QPen(Qt::red, 4) : QPen(Qt::green, 4);
    QString playerLabel = (currentPlayer == P1) ? "Player" : "Computer";

    QGraphicsRectItem* border = scene->addRect(borderRect, highlightPen);
    border->setPos(proxy->pos());
    border->setZValue(proxy->zValue() - 1);

    QPushButton* infoButton = new QPushButton("ℹ️");
    infoButton->setStyleSheet("background-color: transparent; font-weight: bold;");
    QGraphicsProxyWidget* infoProxy = scene->addWidget(infoButton);
    infoProxy->setPos(proxy->pos().x() + borderRect.width() - 10, proxy->pos().y() - 10);
    infoProxy->setZValue(proxy->zValue() + 1);

    connect(infoButton, &QPushButton::clicked, [=]() {
        QString perspective = (gameInstance->currentTurn == P2) ? "Computer" : "Player";

        MinMaxStatistics stats;
        gameInstance->MinMax(0, gameInstance->currentTurn == P2, INT_MIN, INT_MAX, &stats);

        QString explanation;
        explanation += QString("<b>The best move that the MinMax Alogorithm calculated is:  %1:</b> (%2, %3)<br><br>")
                           .arg(perspective)
                           .arg(bestMove.first)
                           .arg(bestMove.second);

        explanation += "<u><b>Basis of decision being made:</b></u><br>";
        QString goal;
        if (gameInstance->currentTurn == P2) {
            goal = "maximize";
        } else {
            goal = "minimize";
        }

        explanation += QString("- %1 is trying to %2 the score.<br>").arg(perspective).arg(goal);

        explanation += "- Possible moves were simulated recursively.<br>";
        explanation += "- We predicted the future move of the opponent to calculate the best move.<br>";
        explanation += "- We used Alpha-Beta pruning to skip unnecessary paths.<br><br>";

        explanation += "<u><b>All the available moves evaluated with their score:</b></u><br>";
        for (auto& moveScore : stats.moveScores) {
            explanation += QString("• Move (%1, %2) ➔ Score: %3<br>")
                               .arg(moveScore.first.first)
                               .arg(moveScore.first.second)
                               .arg(moveScore.second);
        }

        explanation += "<br><u><b>The actual Summary of Alpha-beta Pruning:</b></u><br>";
        explanation += QString("• Total number of Nodes Explored: %1<br>").arg(stats.nodesExplored);
        explanation += QString("• Total number of Nodes Pruned: %1<br><br>").arg(stats.nodesPruned);

        if (stats.nodesExplored > 0) {
            double pruningEfficiency = (static_cast<double>(stats.nodesPruned) / stats.nodesExplored) * 100.0;
            explanation += QString("<b>Actual efficiency of Alpha-Beta Pruning is:</b> ~%1% of the search space was pruned.<br><br>")
                               .arg(QString::number(pruningEfficiency, 'f', 2));
        }

        // we are finding the best move here
        int bestEvalScore = -9999;
        std::pair<int, int> bestEvalMove = {-1, -1};
        if (!stats.moveScores.empty()) {
            bestEvalScore = (gameInstance->currentTurn == P2) ? INT_MIN : INT_MAX;
            for (auto& move : stats.moveScores) {
                if ((gameInstance->currentTurn == P2 && move.second > bestEvalScore) ||
                    (gameInstance->currentTurn == P1 && move.second < bestEvalScore)) {
                    bestEvalScore = move.second;
                    bestEvalMove = move.first;
                }
            }
        }

        int sameScoreCount = 0;
        for (auto& move : stats.moveScores) {
            if (move.second == bestEvalScore) {
                sameScoreCount++;
            }
        }

        bool isBadMove = false;

        TicTacToe tempGame(nullptr, false);
        PieceType** clonedState = gameInstance->Retrieve_stateofBoard();

        // here we are deep cloning the code
        PieceType** newState = new PieceType*[3];
        for (int i = 0; i < 3; i++) {
            newState[i] = new PieceType[3];
            for (int j = 0; j < 3; j++) {
                newState[i][j] = clonedState[i][j];
            }
        }

        tempGame.update_stateofBoard(newState);

        // applying the best evaluation  move here
        tempGame.place(bestEvalMove.first, bestEvalMove.second, (gameInstance->currentTurn == P2) ? P2 : P1);

        // this is for the opponent's turn
        PieceType opponent = (gameInstance->currentTurn == P2) ? P1 : P2;
        std::vector<std::pair<int, int>> opponentMoves = tempGame.find_possiblemove();
        for (auto& move : opponentMoves) {
            tempGame.place(move.first, move.second, opponent);
            if (tempGame.Findout_Win(opponent)) {
                isBadMove = true;
            }
            tempGame.place(move.first, move.second, empty_state); // undo move
        }

        // here we are freeing the memory
        for (int i = 0; i < 3; i++) delete[] newState[i];
        delete[] newState;

        // various explanation based on the various moves that we make
        explanation += QString("<b>Reason for choosing (%1, %2):</b><br>")
                           .arg(bestEvalMove.first)
                           .arg(bestEvalMove.second);

        if (isBadMove) {
            explanation += "- Although the score was optimal, this move exposed an immediate win opportunity for the opponent.<br>";
            explanation += "- This highlights the limitation of purely score-based selection without deep strategic analysis.<br>";
        } else if (sameScoreCount > 1) {
            explanation += QString("- Multiple moves had the same evaluation score of %1.<br>").arg(bestEvalScore);
            explanation += "- We selected this move based on the move order we had.<br>";
        } else {
            explanation += QString("- This move had the best evaluation socre of  %1 among all the move-options available.<br>").arg(bestEvalScore);
        }

        QMessageBox msgBox;
        msgBox.setWindowTitle("Move Explanation (Detailed)");
        msgBox.setTextFormat(Qt::RichText);
        msgBox.setText(explanation);
        msgBox.exec();
    });



}

void TreeGraphics::Tree_withBoards_Iterative(int startX, int startY, int maxDepth) {
    if (!gameInstance) return;

    std::stack<TreeNode> stack;

    // this will help in determining the best move as per the current move
    std::pair<int, int> bestMove;
    if (gameInstance->currentTurn == P2) { // for computer's turn
        bestMove = gameInstance->move_bestcalculation();
    } else { // this is for the scenario of Player's turn
        int bestScore = INT_MAX;
        bestMove = {-1, -1};
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (gameInstance->get_piece_at(i, j) == empty_state) {
                    gameInstance->place(i, j, P1);
                    int score = gameInstance->MinMax(0, true);
                    gameInstance->place(i, j, empty_state);

                    if (score < bestScore) {
                        bestScore = score;
                        bestMove = {i, j};
                    }
                }
            }
        }
    }

    TreeNode root = { startX, startY, maxDepth, 0,
                     gameInstance->Retrieve_stateofBoard(),
                     bestMove,
                     {-1, -1},
                     gameInstance->currentTurn,
                     QPointF(-1, -1) };

    stack.push(root);

    while (!stack.empty()) {
        TreeNode node = stack.top();
        stack.pop();

        if (node.depth == 0 || node.state == nullptr)
            continue;

        TicTacToe* board = new TicTacToe(nullptr, false);
        board->update_stateofBoard(node.state);

        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j) {
                QLayoutItem* item = board->grid->itemAtPosition(i, j);
                if (item) {
                    if (QPushButton* button = qobject_cast<QPushButton*>(item->widget()))
                        button->setDisabled(true);
                }
            }

        QGraphicsProxyWidget* proxy = scene->addWidget(board);
        proxy->setScale(0.5);
        proxy->setPos(node.x, node.y);

        QPointF currentCenter(node.x + board->width() * proxy->scale() / 2,
                              node.y + board->height() * proxy->scale() / 2);

        if (node.level == 1 && node.parentPos != QPointF(-1, -1))
            drawConnectionLine(node.parentPos, currentCenter, node.currentPlayer);

        if (node.level == 1) {
            if (node.currentMove == node.bestMove) { //  correctly comparing for also getting the best move for the player properly
                highlightBestMove(proxy, node.bestMove, node.currentMove, node.currentPlayer);
            }
        }

        std::vector<std::pair<int, int>> moves = gameInstance->find_possiblemove();
        PieceType nextPlayer = (node.currentPlayer == P1) ? P2 : P1;

        for (size_t i = 0; i < moves.size(); ++i) {
            int childX = node.x + (i - static_cast<int>(moves.size()) / 2) * 100;
            int childY = node.y + 120;

            PieceType** newState = gameInstance->Visualize_Move(node.state, moves[i].first, moves[i].second, node.currentPlayer);
            if (newState) {
                TreeNode childNode = {
                    childX, childY, node.depth - 1, node.level + 1,
                    newState, node.bestMove, moves[i],
                    nextPlayer, currentCenter
                };
                stack.push(childNode);
            }
        }
    }
}



