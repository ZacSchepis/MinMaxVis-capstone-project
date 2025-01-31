#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
// We could override this and map it over
// to something else to display the piece
// TTT - X/O paintings
// Che - red/black circle paintings
enum PieceType {
    empty_space,
    P1,
    P2
};


class Board : public QWidget
{
    Q_OBJECT
private:
    int rows, cols;
    int** board_state;
public:
    explicit Board(QWidget *parent = nullptr, int r = 3, int c = 3);
    QWidget draw_board();
    int place(int r, int c, PieceType piece);
    // virtual void map_piece(PieceType p_type);
    // int place(int p, int piece);
protected:
    void paintEvent(QPaintEvent* event) override;

signals:
};

#endif // BOARD_H
