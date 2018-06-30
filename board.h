#ifndef BOARD_H
#define BOARD_H
#define BLACK 0
#define WHITE 1
#define EMPTY 2
#define INF 1e10
#define HUMAN 0
#define COMPUTER 1

#include <QObject>
#include <vector>

struct piece_return
{
    int disk_square;
    double piece_value;
    double frontier_value;

};

struct State
{
    int board_state[8][8];
    int current_color;
    int opponent_color;
};

struct Tree_Node
{
    Tree_Node *best_child;
    int move[2];
    State *state;
    std::vector<Tree_Node*> children;
};

struct Player
{
    int type;
    int color;
};

class Board : public QObject
{
    Q_OBJECT
public slots:
    void piece_added_slot(const qint32 row,const qint32 column);

public:
    Board(QObject *bobj,QObject *cobj);
    void occupy_cell(int row,int column,State &state);
    void print_board(State &state);
    void set_valid_moves(int color, State &state, bool *move_list);
    void print_valid_moves(bool *move_list);
    void capture_pieces(int row, int col, State &state, bool reflect_changes);
    bool has_game_ended(State &state);
    void calculate_score(State &state);


    /* AI Funtions */
    double heuristic(State &state);
    piece_return piece_count(State &state);
    int corner_occupancy(State &state);
    int corner_closeness(State &state);
    int mobility(State &state);
    Tree_Node *get_empty_node();
    void create_tree(Tree_Node *root,int current_level,int depth);
    State *duplicate_state(State &state);
    double mini_max(Tree_Node *root,int depth,int player,int current_color,double alpha,double beta);
    int get_move(Tree_Node *root,int depth,int player,int current_color);

private:
    QObject *board_obj;
    QObject *controls_object;
    State state;
    int score_board[2]={0};
    Player player1;
    Player player2;
    int depth = 2;

};

#endif // BOARD_H
