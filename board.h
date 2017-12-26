#ifndef BOARD_H
#define BOARD_H
#define BLACK 0
#define WHITE 1
#define EMPTY 2
#include <QObject>

typedef struct
{
   int disk_square;
   int piece_value;
   int frontier_value;

} piece_return;

typedef struct
{
    int board_state[8][8];
    int current_color;
    int opponent_color;
} State;

class Board : public QObject
{
    Q_OBJECT
public slots:
    void piece_added_slot(const qint32 row,const qint32 column);
public:
    Board(QObject *bobj,QObject *cobj);
    void occupy_cell(int row,int column,State &state);
    void print_board();
    void set_valid_moves(int color,State &state);
    void print_valid_moves();
    void capture_pieces(int row,int col,State &state);
    void set_board_state();
    bool has_game_ended();
    void calculate_score();
    int get_move();
    void minimax();
    double heuristic(int **board_state);
    void piece_added(int row,int column,State &state);
    piece_return piece_count(int **board_state);
private:
    QObject *board_obj;
    QObject *controls_object;
    State state;
    bool move_list[64]={false};
    int score_board[2]={0};

};

#endif // BOARD_H
