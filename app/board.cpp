#include "board.h"
#include <QPushButton.h>
#include <QIcon.h>
#include <QCoreApplication.h>
Board::Board(QWidget *parent, int r, int c)
    : QWidget{parent}, rows(r), cols(c), cell_size(50)
{
    grid = new QGridLayout(this);
    // Initializing piece_maps to default values
    this->map_piece(error_state, "");
    this->map_piece(empty_state, "");
    this->map_piece(P1, "");
    this->map_piece(P2, "");
    this->setFixedSize(c*this->cell_size, r*this->cell_size);
    board_state = new PieceType*[rows];
    for(int i=0; i<r; i++){
        board_state[i] = new PieceType[cols];
        for(int j=0; j<c; j++){
            board_state[i][j] = empty_state;
            QPushButton *button = new QPushButton(this);
            button->setFixedSize(48,48);
            bool is_black = (i + j) % 2 == 1;

            button->setStyleSheet(QString("background-color: %1;").arg(is_black ? "black":"white"));
//            c_++;
            grid->addWidget(button, i, j);
        }
    }
//    this->set_styles("background-color: white; border:2px solid black;");
    setLayout(grid);

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
//void Board::handleButton(int row, int col) {
//
//}