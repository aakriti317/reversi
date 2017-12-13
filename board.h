#ifndef BOARD_H
#define BOARD_H
#define BLACK 0
#define WHITE 1

class Board
{
public:
    Board();
    void add_piece(int row,int column,int color);
};

#endif // BOARD_H
