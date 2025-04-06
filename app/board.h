#ifndef BOARD_H
#define BOARD_H
#include <QLayout>
#include <QWidget>
#include <map>
enum PieceType {
    error_state=-1,
    empty_state,
    P1,
    P2,
};


class Board : public QWidget
{
    Q_OBJECT
private:
    int rows, cols;
    PieceType** board_state;
    int cell_size;
    std::map<PieceType, QString> piece_maps;
//private slots:
//    void handleButton(int row, int col);
public:
    void update_stateofBoard(PieceType** newState);
    QGridLayout *grid;
    PieceType** Retrieve_stateofBoard() const;
    explicit Board(QWidget *parent = nullptr, int r = 3, int c = 3);

    /**
     * @brief Board::get_piece_at Schepis-2/7/25
     * Helper function to get the PieceType
     * at a specific cell
     * @return PieceType
     */
    PieceType get_piece_at(int row, int col);

    /**
     * @brief Board::get_cell_size Helper function to retrieve cell size in px
     * @return int
     */
    int get_cell_size();

    QWidget draw_board();

    /**
     * @brief Board::place Schepis-2/7/25
     * Validates that a given position is in bounds,
     * setting the PieceType at a cell and updates the visual
     * NOTE::This WILL update regardless of collisions at the current being
     * ex: board[r][c] = P1, place(r,c,P2) is called,
     * board[r][c] will now be P2, or empty_state.
     * @return -1 (out of bounds) or 0 (success)
     */
    int place(int r, int c, PieceType piece);
    /**
     * @brief Board::map_piece Schepis-2/7/25
     * useful in derived classes to assign piece icons
     * map_piece([error_state, empty_state, P1, P2], "file_name")
     * see sample_board.cpp for usage
     * @return 0 (no changes) or 1 (updated path)
    */
    int map_piece(PieceType p_type, QString resource_name);

    /**
     * @brief Board::update_cell Schepis-2/7/25
     * This will manage visual cell changes, like re/setting
     * cell icons
     */
    void update_cell(int row, int col);
    ~Board();


    void set_styles(QString const &styles);
    int get_size(int level);
protected:
    void paintEvent(QPaintEvent* event) override;

signals:
};

#endif // BOARD_H