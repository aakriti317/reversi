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
    void set_valid_moves(int color, State &state, bool *move_list);
    void print_valid_moves(bool *move_list);
    void capture_pieces(int row,int col,State &state);
    bool has_game_ended();
    void calculate_score();
    int get_move();
    void minimax();

    /* Heuristic Funtions */
    double heuristic(State &state);
    piece_return piece_count(State &state);
    int corner_occupancy(State &state);
    int corner_closeness(State &state);
    int mobility(State &state);
private:
    QObject *board_obj;
    QObject *controls_object;
    State state;
    int score_board[2]={0};

};

#endif // BOARD_H
