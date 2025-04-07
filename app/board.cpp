#include "board.h"
#include <QPushButton.h>
#include <QIcon.h>
#include <QCoreApplication.h>
#include <QLabel>
Board::Board(QWidget *parent, int r, int c, bool showRightPanel)
    : QWidget{parent}, rows(r), cols(c), cell_size(50), right_panel(nullptr), right_layout(nullptr)
{
    QHBoxLayout *outer_layout = new QHBoxLayout(this);

    // Create the game board grid
    grid = new QGridLayout();
    grid->setSpacing(0);

    this->map_piece(error_state, "");
    this->map_piece(empty_state, "");
    this->map_piece(P1, "");
    this->map_piece(P2, "");

    // Adjust the width only if right panel is shown
    int extra_width = showRightPanel ? 150 : 0;
    this->setFixedSize((c * this->cell_size) + extra_width, r * this->cell_size);

    // Initialize board state and add buttons
    board_state = new PieceType*[rows];
    for (int i = 0; i < r; i++) {
        board_state[i] = new PieceType[cols];
        for (int j = 0; j < c; j++) {
            board_state[i][j] = empty_state;
            QPushButton *button = new QPushButton(this);
            button->setFixedSize(48, 48);
            bool is_black = (i + j) % 2 == 1;
            button->setStyleSheet(QString("background-color: %1;").arg(is_black ? "black" : "white"));
            grid->addWidget(button, i, j);
        }
    }

    // Wrap the grid in a QWidget
    QWidget *board_widget = new QWidget(this);
    board_widget->setLayout(grid);
    outer_layout->addWidget(board_widget);

    // Conditionally create and add the right panel
    if (showRightPanel) {
        right_panel = new QWidget(this);
        right_panel->setFixedWidth(150);
        right_panel->setStyleSheet("background-color: #ddd;");
        right_layout = new QVBoxLayout(right_panel);
        right_panel->setLayout(right_layout);
        outer_layout->addWidget(right_panel);
    }

    setLayout(outer_layout);
}





int Board::map_piece(PieceType p_type, QString resource_name) {
    if(this->piece_maps.count(p_type) == 0) {
        this->piece_maps[p_type] = resource_name;
        // this->piece_maps.emplace(p_type, resource_name);
        return 1;
    }
    if(this->piece_maps.at(p_type) == resource_name) return 0;
    this->piece_maps.at(p_type) = resource_name;
    return 1;
}

void Board::update_cell(int row, int col){
    QLayoutItem *item = grid->itemAtPosition(row, col);
    PieceType p_type = this->get_piece_at(row, col);
    if(item && (p_type != error_state)) {
        QWidget *wid = item->widget();
        QPushButton *button = qobject_cast<QPushButton*>(wid);
        QString icon_path = this->piece_maps.at(p_type);
        button->setIcon(icon_path != "" ? QIcon(icon_path) : QIcon());
    }
}

void Board::update_stateofBoard(PieceType** newState) {
    if (!newState) {
        qDebug() << "ERROR: Attempted to setBoardState with a nullptr!";
        return;
    }

    qDebug() << "Setting board state...";

    // Free old board state memory
    if (board_state) {
        for (int i = 0; i < rows; i++) {
            if (board_state[i]) {
                delete[] board_state[i];
            }
        }
        delete[] board_state;
    }

    // Allocate new board state
    board_state = new PieceType*[rows];
    for (int i = 0; i < rows; i++) {
        board_state[i] = new PieceType[cols];
        for (int j = 0; j < cols; j++) {
            board_state[i][j] = newState[i][j];  // Copy new state
        }
    }
}



int Board::place(int r, int c, PieceType piece){
    if((r >= rows || r < 0) || (c >= cols || c < 0)) return -1;
    board_state[r][c] = piece;
    this->update_cell(r, c);
    return 0;
}

PieceType Board::get_piece_at(int row, int col) {
    if((row >= this->rows || row < 0) || (col >= this->cols || col <0)) return error_state;
    return this->board_state[row][col];
}

PieceType** Board::Retrieve_stateofBoard() const {
    return board_state;
}

int Board::get_cell_size() {return this->cell_size;}

void Board::paintEvent(QPaintEvent* event){
    QWidget::paintEvent(event);
}

Board::~Board() {
    for(int i=0; i<rows; i++){
        delete[] board_state[i];
    }
    delete[] board_state;
}

void Board::set_styles(QString const &styles) {
    this->setStyleSheet(styles);
}
int Board::get_size(int level) {
    return level == 1 ? this->rows : this->cols*this->cols;
}

QVBoxLayout* Board::getRightLayout() {
    return right_layout;
}
