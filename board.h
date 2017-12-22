#ifndef BOARD_H
#define BOARD_H
#define BLACK 0
#define WHITE 1
#define EMPTY 2
#include <QObject>
class Board : public QObject
{
    Q_OBJECT
public slots:
    void piece_added_slot(const qint32 row,const qint32 column);
public:
    Board(QObject *bobj,QObject *cobj);
    void occupy_cell(int row,int column,QString color);
    void print_board();
    void set_valid_moves(int color);
    void print_valid_moves();
    void capture_pieces(int row,int col,int color);
    void set_board_state();
    bool has_game_ended();
    void calculate_score();
private:
    QObject *board_obj;
    QObject *controls_object;
    int board_state[8][8];
    bool move_list[64]={false};
    int current_color=BLACK;
    int score_board[2]={0};
};

#endif // BOARD_H
