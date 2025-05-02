#ifndef SAMPLE_BOARD_H
#define SAMPLE_BOARD_H
#include "board.h"
#include <QWidget.h>
class SampleBoard : public Board
{
public:
    // explicit Board(QWidget *parent = nullptr, int r = 3, int c = 3);

    explicit SampleBoard(QWidget *parent=nullptr);
    // void map_piece(PieceType p_type, std::string) override;
};

#endif // SAMPLE_BOARD_H
