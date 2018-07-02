#include "board.h"
#include <QDebug>
#include <iostream>
using namespace std;
Board::Board(QObject *root,QObject *bobj,QObject *cobj,QObject *hobj)
{
    board_obj=bobj;
    controls_object=cobj;
    home_obj = hobj;
    root_object = root;
    QObject::connect(board_obj, SIGNAL(piece_added_signal(qint32,qint32)),
                     this, SLOT(piece_added_slot(qint32,qint32)));
    QObject::connect(home_obj, SIGNAL(start_signal(qint32,qint32,qint32,qint32)),
                     this, SLOT(start_slot(qint32,qint32,qint32,qint32)));

}

void Board::start_slot(const qint32 player1_type,const qint32 player2_type,const qint32 player1_intelligence,const qint32 player2_intelligence)
{
    for(int row=0;row<8;row++)
        for(int col=0;col<8;col++)
            state.board_state[row][col]=EMPTY;
    player1.type = player1_type;
    player2.type = player2_type;
    player1.color = BLACK;
    player2.color = WHITE;
    player1.intelligence = player1_intelligence;
    player2.intelligence = player2_intelligence;
    state.current_color=player1.color;
    state.opponent_color=player2.color;

    QMetaObject::invokeMethod(root_object, "initialize");

    state.board_state[3][3]=WHITE;
    QMetaObject::invokeMethod(board_obj, "occupy_cell",
                              Q_ARG(QVariant,3),Q_ARG(QVariant,3),Q_ARG(QVariant,"WHITE"));
    state.board_state[4][4]=WHITE;
    QMetaObject::invokeMethod(board_obj, "occupy_cell",
                              Q_ARG(QVariant,4),Q_ARG(QVariant,4),Q_ARG(QVariant,"WHITE"));
    state.board_state[3][4]=BLACK;
    QMetaObject::invokeMethod(board_obj, "occupy_cell",
                              Q_ARG(QVariant,3),Q_ARG(QVariant,4),Q_ARG(QVariant,"BLACK"));
    state.board_state[4][3]=BLACK;
    QMetaObject::invokeMethod(board_obj, "occupy_cell",
                              Q_ARG(QVariant,4),Q_ARG(QVariant,3),Q_ARG(QVariant,"BLACK"));
    QMetaObject::invokeMethod(controls_object, "status_update",
                              Q_ARG(QVariant,"BLACK's Turn"));
    if(player1.color == state.current_color && player1.type == COMPUTER)
    {
        Tree_Node *root = get_empty_node();
        root->state = duplicate_state(&state);
        create_tree(root,0,player1.intelligence);
        int move = get_move(root,player1.intelligence,player1.color,player1.color);
        cout << move / 8 << ":" << move % 8 << endl;
        piece_added_slot(move / 8, move % 8);
    }
}
void Board::piece_added_slot(const qint32 row, const qint32 column)
{
    bool move_list[64]={false};
    set_valid_moves(state.current_color,&state,move_list);
    bool moves_present=false;
    bool chance_flipped=false;
    int clicked_index=row*8+column;
    for(int i=0;i<64;i++)
        moves_present=moves_present || move_list[i];
    if(moves_present)
    {
        if(move_list[clicked_index])
        {
            occupy_cell(row,column,&state);
            capture_pieces(row,column,&state,true);
            state.current_color=1-state.current_color;
            state.opponent_color=1-state.current_color;
            chance_flipped=true;
            calculate_score(&state);
        }
    }
    else
    {
        state.current_color=1-state.current_color;
        state.opponent_color=1-state.current_color;
        chance_flipped=true;
        QMetaObject::invokeMethod(controls_object, "status_update",
                                  Q_ARG(QVariant,"No more moves left, passing the turn"));
    }
    if(has_game_ended(&state))
    {
        game_ended = true;
        QString message;
        if(score_board[BLACK]>score_board[WHITE])
            message="Game Ended! Black Wins.";
        else if(score_board[BLACK]<score_board[WHITE])
            message="Game Ended! White Wins.";
        else
            message="Game Ended! It's a tie.";
        QMetaObject::invokeMethod(controls_object, "status_update",
                                  Q_ARG(QVariant,message));
    }
    if(chance_flipped && !game_ended)
    {
        QString message;
        if(state.current_color==WHITE)
            message="WHITE's Turn";
        else if(state.current_color==BLACK)
            message="BLACK's Turn";
        QMetaObject::invokeMethod(controls_object, "status_update",
                                  Q_ARG(QVariant,message));
        if(player1.color == state.current_color && player1.type == COMPUTER)
        {
            Tree_Node *root = get_empty_node();
            root->state = duplicate_state(&state);
            create_tree(root,0,player1.intelligence);
            int move = get_move(root,player1.intelligence,player1.color,player1.color);
            cout << move / 8 << ":" << move % 8 << endl;
            piece_added_slot(move / 8, move % 8);
        }
        else if(player2.color == state.current_color && player2.type == COMPUTER)
        {
            Tree_Node *root = get_empty_node();
            root->state = duplicate_state(&state);
            create_tree(root,0,player2.intelligence);
            int move = get_move(root,player2.intelligence,player2.color,player2.color);
            cout << move / 8 << ":" << move % 8 << endl;
            piece_added_slot(move / 8, move % 8);
        }
    }
}

void Board::occupy_cell(int row, int column,State *state)
{
    QString color_string;
    if(state->current_color==BLACK)
        color_string="BLACK";
    else if(state->current_color==WHITE)
        color_string="WHITE";
    QMetaObject::invokeMethod(board_obj, "occupy_cell",
                              Q_ARG(QVariant,row),Q_ARG(QVariant,column),Q_ARG(QVariant,color_string));
    state->board_state[row][column]=state->current_color;
}

void Board::print_board(State *state)
{
    for(int row=0;row<8;row++)
    {
        for(int col=0;col<8;col++)
        {
            cout<< state->board_state[row][col]<<" ";
        }
        cout<<endl;
    }
    cout<< "*****************************" <<endl;
}

void Board::set_valid_moves(int color, State *state, bool *move_list)
{
    for(int i=0;i<64;i++)
        move_list[i]=false;
    for(int row=0;row<8;row++)
    {
        for(int col=0;col<8;col++)
        {
            if (state->board_state[row][col]==color)
            {
                int flag = 0;
                for(int i=row-1;i>=0;i--)
                {
                    if (state->board_state[i][col]==1-color)
                        flag=1;
                    else if (state->board_state[i][col]==EMPTY && flag==1)
                    {
                        move_list[i*8+col]=true;
                        break;
                    }
                    else if (state->board_state[i][col]==EMPTY && flag==0)
                        break;
                    else if (state->board_state[i][col]==color)
                        break;
                }
                flag=0;
                for(int i=row+1;i<8;i++)
                {
                    if (state->board_state[i][col]==1-color)
                        flag=1;
                    else if (state->board_state[i][col]==EMPTY && flag==1)
                    {
                        move_list[i*8+col]=true;
                        break;
                    }
                    else if (state->board_state[i][col]==EMPTY && flag==0)
                        break;
                    else if (state->board_state[i][col]==color)
                        break;

                }
                flag=0;
                for(int i=col-1;i>=0;i--)
                {
                    if (state->board_state[row][i]==1-color)
                        flag=1;
                    else if (state->board_state[row][i]==EMPTY && flag==1)
                    {
                        move_list[row*8+i]=true;
                        break;
                    }
                    else if (state->board_state[row][i]==EMPTY && flag==0)
                        break;
                    else if (state->board_state[row][i]==color)
                        break;
                }
                flag=0;
                for(int i=col+1;i<8;i++)
                {
                    if (state->board_state[row][i]==1-color)
                        flag=1;
                    else if (state->board_state[row][i]==EMPTY && flag==1)
                    {
                        move_list[row*8+i]=true;
                        break;
                    }
                    else if (state->board_state[row][i]==EMPTY && flag==0)
                        break;
                    else if (state->board_state[row][i]==color)
                        break;
                }
                flag=0;
                int i=1;
                while(row-i>=0 && col-i>=0)
                {
                    if (state->board_state[row-i][col-i]==1-color)
                        flag=1;
                    else if (state->board_state[row-i][col-i]==EMPTY && flag==1)
                    {
                        move_list[(row-i)*8+(col-i)]=true;
                        break;
                    }
                    else if (state->board_state[row-i][col-i]==EMPTY && flag==0)
                        break;
                    else if (state->board_state[row-i][col-i]==color)
                        break;
                    i++;
                }
                flag=0;
                i=1;
                while(row+i<8 && col+i<8)
                {
                    if (state->board_state[row+i][col+i]==1-color)
                        flag=1;
                    else if (state->board_state[row+i][col+i]==EMPTY && flag==1)
                    {
                        move_list[(row+i)*8+(col+i)]=true;
                        break;
                    }
                    else if (state->board_state[row+i][col+i]==EMPTY && flag==0)
                        break;
                    else if (state->board_state[row+i][col+i]==color)
                        break;
                    i++;
                }
                flag=0;
                i=1;
                while(row-i>=0 && col+i<8)
                {
                    if (state->board_state[row-i][col+i]==1-color)
                        flag=1;
                    else if (state->board_state[row-i][col+i]==EMPTY && flag==1)
                    {
                        move_list[(row-i)*8+(col+i)]=true;
                        break;
                    }
                    else if (state->board_state[row-i][col+i]==EMPTY && flag==0)
                        break;
                    else if (state->board_state[row-i][col+i]==color)
                        break;
                    i++;
                }
                flag=0;
                i=1;
                while(row+i<8 && col-i>=0)
                {
                    if (state->board_state[row+i][col-i]==1-color)
                        flag=1;
                    else if (state->board_state[row+i][col-i]==EMPTY && flag==1)
                    {
                        move_list[(row+i)*8+(col-i)]=true;
                        break;
                    }
                    else if (state->board_state[row+i][col-i]==EMPTY && flag==0)
                        break;
                    else if (state->board_state[row+i][col-i]==color)
                        break;
                    i++;
                }

            }
        }
    }
}

void Board::print_valid_moves(bool *move_list)
{
    for(int i=0;i<64;i++)
        if(move_list[i])
            cout << i / 8 << " " << i % 8 << endl;
    cout<<"***************"<<endl;
}

void Board::capture_pieces(int row,int col,State *state, bool reflect_changes)
{
    QString color_string;
    if(state->current_color==BLACK)
        color_string="BLACK";
    else if(state->current_color==WHITE)
        color_string="WHITE";
    vector <int> temp;
    if (state->board_state[row][col]==state->current_color)
    {
        temp.erase(temp.begin(),temp.end());
        for(int i=row-1;i>=0;i--)
        {
            if (state->board_state[i][col]==state->opponent_color)
            {
                temp.push_back(i*8+col);
            }
            else if (state->board_state[i][col]==EMPTY)
                break;
            else if (state->board_state[i][col]==state->current_color)
            {
                while(!temp.empty())
                {
                    int p=temp.back()/8;
                    int q=temp.back()%8;
                    temp.pop_back();
                    state->board_state[p][q]=1 - state->board_state[p][q];
                    if(reflect_changes)
                        QMetaObject::invokeMethod(board_obj, "change_color",
                                                  Q_ARG(QVariant,p),
                                                  Q_ARG(QVariant,q),
                                                  Q_ARG(QVariant,color_string));
                }
                break;
            }
        }
        temp.erase(temp.begin(),temp.end());
        for(int i=row+1;i<8;i++)
        {
            if (state->board_state[i][col]==state->opponent_color)
            {
                temp.push_back(i*8+col);
            }
            else if (state->board_state[i][col]==EMPTY)
                break;
            else if (state->board_state[i][col]==state->current_color)
            {
                while(!temp.empty())
                {
                    int p=temp.back()/8;
                    int q=temp.back()%8;
                    temp.pop_back();
                    state->board_state[p][q]=1 - state->board_state[p][q];
                    if(reflect_changes)
                        QMetaObject::invokeMethod(board_obj, "change_color",
                                                  Q_ARG(QVariant,p),
                                                  Q_ARG(QVariant,q),
                                                  Q_ARG(QVariant,color_string));
                }
                break;
            }

        }
        temp.erase(temp.begin(),temp.end());
        for(int i=col-1;i>=0;i--)
        {
            if (state->board_state[row][i]==state->opponent_color)
            {
                temp.push_back(row*8+i);
            }
            else if (state->board_state[row][i]==EMPTY)
                break;
            else if (state->board_state[row][i]==state->current_color)
            {
                while(!temp.empty())
                {
                    int p=temp.back()/8;
                    int q=temp.back()%8;
                    temp.pop_back();
                    state->board_state[p][q]=1 - state->board_state[p][q];
                    if(reflect_changes)
                        QMetaObject::invokeMethod(board_obj, "change_color",
                                                  Q_ARG(QVariant,p),
                                                  Q_ARG(QVariant,q),
                                                  Q_ARG(QVariant,color_string));
                }
                break;
            }
        }
        temp.erase(temp.begin(),temp.end());
        for(int i=col+1;i<8;i++)
        {
            if (state->board_state[row][i]==state->opponent_color)
            {
                temp.push_back(row*8+i);
            }
            else if (state->board_state[row][i]==EMPTY)
                break;
            else if (state->board_state[row][i]==state->current_color)
            {
                while(!temp.empty())
                {
                    int p=temp.back()/8;
                    int q=temp.back()%8;
                    temp.pop_back();
                    state->board_state[p][q]=1 - state->board_state[p][q];
                    if(reflect_changes)
                        QMetaObject::invokeMethod(board_obj, "change_color",
                                                  Q_ARG(QVariant,p),
                                                  Q_ARG(QVariant,q),
                                                  Q_ARG(QVariant,color_string));
                }
                break;
            }
        }
        int i=1;
        temp.erase(temp.begin(),temp.end());
        while(row-i>=0 && col-i>=0)
        {
            if (state->board_state[row-i][col-i]==state->opponent_color)
            {
                temp.push_back((row-i)*8+(col-i));
            }
            else if (state->board_state[row-i][col-i]==EMPTY)
                break;
            else if (state->board_state[row-i][col-i]==state->current_color)
            {
                while(!temp.empty())
                {
                    int p=temp.back()/8;
                    int q=temp.back()%8;
                    temp.pop_back();
                    state->board_state[p][q]=1 - state->board_state[p][q];
                    if(reflect_changes)
                        QMetaObject::invokeMethod(board_obj, "change_color",
                                                  Q_ARG(QVariant,p),
                                                  Q_ARG(QVariant,q),
                                                  Q_ARG(QVariant,color_string));
                }
                break;
            }
            i++;
        }
        i=1;
        temp.erase(temp.begin(),temp.end());
        while(row+i<8 && col+i<8)
        {
            if (state->board_state[row+i][col+i]==state->opponent_color)
            {
                temp.push_back((row+i)*8+(col+i));
            }
            else if (state->board_state[row+i][col+i]==EMPTY)
                break;
            else if (state->board_state[row+i][col+i]==state->current_color)
            {
                while(!temp.empty())
                {
                    int p=temp.back()/8;
                    int q=temp.back()%8;
                    temp.pop_back();
                    state->board_state[p][q]=1 - state->board_state[p][q];
                    if(reflect_changes)
                        QMetaObject::invokeMethod(board_obj, "change_color",
                                                  Q_ARG(QVariant,p),
                                                  Q_ARG(QVariant,q),
                                                  Q_ARG(QVariant,color_string));
                }
                break;
            }
            i++;
        }
        i=1;
        temp.erase(temp.begin(),temp.end());
        while(row-i>=0 && col+i<8)
        {
            if (state->board_state[row-i][col+i]==state->opponent_color)
            {
                temp.push_back((row-i)*8+(col+i));
            }
            else if (state->board_state[row-i][col+i]==EMPTY)
                break;
            else if (state->board_state[row-i][col+i]==state->current_color)
            {
                while(!temp.empty())
                {
                    int p=temp.back()/8;
                    int q=temp.back()%8;
                    temp.pop_back();
                    state->board_state[p][q]=1 - state->board_state[p][q];
                    if(reflect_changes)
                        QMetaObject::invokeMethod(board_obj, "change_color",
                                                  Q_ARG(QVariant,p),
                                                  Q_ARG(QVariant,q),
                                                  Q_ARG(QVariant,color_string));
                }
                break;
            }
            i++;
        }
        i=1;
        temp.erase(temp.begin(),temp.end());
        while(row+i<8 && col-i>=0)
        {
            if (state->board_state[row+i][col-i]==state->opponent_color)
            {
                temp.push_back((row+i)*8+(col-i));
            }
            else if (state->board_state[row+i][col-i]==EMPTY)
                break;
            else if (state->board_state[row+i][col-i]==state->current_color)
            {
                while(!temp.empty())
                {
                    int p=temp.back()/8;
                    int q=temp.back()%8;
                    temp.pop_back();
                    state->board_state[p][q]=1 - state->board_state[p][q];
                    if(reflect_changes)
                        QMetaObject::invokeMethod(board_obj, "change_color",
                                                  Q_ARG(QVariant,p),
                                                  Q_ARG(QVariant,q),
                                                  Q_ARG(QVariant,color_string));
                }
                break;
            }
            i++;
        }
    }
}

bool Board::has_game_ended(State *state)
{
    bool move_list[64]={false};
    set_valid_moves(state->current_color,state,move_list);
    bool current_color_move=false;
    for(int i=0;i<64;i++)
        current_color_move=current_color_move || move_list[i];
    bool move_list1[64]={false};
    set_valid_moves(state->opponent_color,state,move_list1);
    bool opponent_color_move=false;
    for(int i=0;i<64;i++)
        opponent_color_move=opponent_color_move || move_list1[i];
    return !current_color_move && !opponent_color_move;
}

void Board::calculate_score(State *state)
{
    score_board[BLACK]=0;
    score_board[WHITE]=0;
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(state->board_state[i][j]==BLACK)
                score_board[BLACK]++;
            else if(state->board_state[i][j]==WHITE)
                score_board[WHITE]++;
        }
    }
    QMetaObject::invokeMethod(controls_object, "set_score_board",
                              Q_ARG(QVariant,score_board[BLACK]),Q_ARG(QVariant,score_board[WHITE]));

}

double Board::heuristic(State *state)
{
    piece_return pr = piece_count(state);
    int corner_occupancy_value = corner_occupancy(state);
    int corner_closeness_value = corner_closeness(state);
    int mobility_value = mobility(state);
    return (10 * pr.disk_square) + (10 * pr.piece_value) + (74.396 * pr.frontier_value) + (801.724 * corner_occupancy_value) + (382.026 * corner_closeness_value) + (78.922 * mobility_value);
}

piece_return Board::piece_count(State *state)
{
    int V[8][8] =
    {
        {20,-3,11,8,8,11,-3,20},
        {-3,-7,-4,1,1,-4,-7,-3},
        {11,-4,2,2,2,2,-4,11},
        {8,1,2,-3,-3,2,1,8},
        {8,1,2,-3,-3,2,1,8},
        {11,-4,2,2,2,2,-4,11},
        {-3,-7,-4,1,1,-4,-7,-3},
        {20,-3,11,8,8,11,-3,20}
    };

    int neighbour[8][2]=
    {
        {-1,-1},
        { 0,-1},
        { 1,-1},
        {-1, 0},
        { 1, 0},
        {-1, 1},
        { 0, 1},
        { 1, 1}
    };
    int my_pieces=0,opponent_pieces=0,disk_square=0;
    int myFrontierPieces = 0;
    int opponentFrontierPieces = 0;
    double Frontier_value=0.0,piece_value=0.0;
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(state->board_state[i][j]==state->current_color)
            {
                disk_square += V[i][j];
                my_pieces++;
            }
            else if(state->board_state[i][j]==state->opponent_color)
            {
                disk_square -= V[i][j];
                opponent_pieces++;
            }
            if(state->board_state[i][j] != EMPTY)
            {
                for(int k=0;k<8;k++)
                {
                    int x = i + neighbour[k][0];
                    int y = j + neighbour[k][1];
                    if(x>=0 && x<8 && y>=0 && y<8 && state->board_state[x][y] == EMPTY) {
                        if (state->board_state[i][j] == state->current_color)
                            myFrontierPieces++;
                        else
                            opponentFrontierPieces++;
                    }
                    break;
                }
            }
        }
    }

    if (myFrontierPieces > opponentFrontierPieces)
        Frontier_value = (-100.0 * myFrontierPieces) / (myFrontierPieces + opponentFrontierPieces);
    else if(myFrontierPieces < opponentFrontierPieces)
        Frontier_value = (100.0 * opponentFrontierPieces) / (myFrontierPieces + opponentFrontierPieces);
    else
        Frontier_value = 0.0;

    if(my_pieces > opponent_pieces)
        piece_value = (100.0 * my_pieces) / (my_pieces + opponent_pieces);
    else if(my_pieces < opponent_pieces)
        piece_value = (-100.0 * opponent_pieces) / (my_pieces + opponent_pieces);
    else
        piece_value = 0.0;

    piece_return pr;
    pr.disk_square = disk_square;
    pr.piece_value = piece_value;
    pr.frontier_value = Frontier_value;
    return pr;
}

int Board::corner_occupancy(State *state)
{
    int my_pieces=0,opponent_pieces=0;
    if(state->board_state[0][0] == state->current_color)
        my_pieces ++;
    else if(state->board_state[0][0] == state->opponent_color)
        opponent_pieces ++;
    if(state->board_state[0][7] == state->current_color)
        my_pieces ++;
    else if(state->board_state[0][7] == state->opponent_color)
        opponent_pieces ++;
    if(state->board_state[7][0] == state->current_color)
        my_pieces ++;
    else if(state->board_state[7][0] == state->opponent_color)
        opponent_pieces ++;
    if(state->board_state[7][7] == state->current_color)
        my_pieces ++;
    else if(state->board_state[7][7] == state->opponent_color)
        opponent_pieces ++;
    return 25 * (my_pieces - opponent_pieces);
}

int Board::corner_closeness(State *state)
{
    int my_pieces=0,opponent_pieces=0;
    if(state->board_state[0][0] == EMPTY)
    {
        if(state->board_state[0][1] == state->current_color)
            my_pieces ++;
        else if(state->board_state[0][1] == state->opponent_color)
            opponent_pieces ++;
        if(state->board_state[1][0] == state->current_color)
            my_pieces ++;
        else if(state->board_state[1][0] == state->current_color)
            opponent_pieces ++;
        if(state->board_state[1][1] == state->current_color)
            my_pieces ++;
        else if(state->board_state[1][1] == state->opponent_color)
            opponent_pieces ++;
    }
    if(state->board_state[0][7] == EMPTY)
    {
        if(state->board_state[0][6] == state->current_color)
            my_pieces ++;
        else if(state->board_state[0][6] == state->opponent_color)
            opponent_pieces ++;
        if(state->board_state[1][7] == state->current_color)
            my_pieces ++;
        else if(state->board_state[1][7] == state->opponent_color)
            opponent_pieces ++;
        if(state->board_state[1][6] == state->current_color)
            my_pieces ++;
        else if(state->board_state[1][6] == state->opponent_color)
            opponent_pieces ++;
    }
    if(state->board_state [7][0] == EMPTY)
    {
        if(state->board_state[1][7] == state->current_color)
            my_pieces ++;
        else if(state->board_state[1][7] == state->opponent_color)
            opponent_pieces ++;
        if(state->board_state[6][0] == state->current_color)
            my_pieces ++;
        else if(state->board_state[6][0] == state->opponent_color)
            opponent_pieces ++;
        if(state->board_state[6][1] == state->current_color)
            my_pieces ++;
        else if(state->board_state[6][1] == state->opponent_color)
            opponent_pieces ++;
    }
    if(state->board_state[7][7] == EMPTY)
    {
        if(state->board_state[6][7] == state->current_color)
            my_pieces ++;
        else if(state->board_state[6][7] == state->opponent_color)
            opponent_pieces ++;
        if(state->board_state[7][6] == state->current_color)
            my_pieces ++;
        else if(state->board_state[7][6] == state->opponent_color)
            opponent_pieces ++;
        if(state->board_state[6][6] == state->current_color)
            my_pieces ++;
        else if(state->board_state[6][6] == state->opponent_color)
            opponent_pieces ++;
    }
    return -12.5 * (my_pieces - opponent_pieces);
}

int Board::mobility(State *state)
{
    bool move_list[64]={false};
    set_valid_moves(state->current_color,state,move_list);
    int my_moves=0;
    for(int i=0;i<64;i++)
    {
        if(move_list[i]==true)
            my_moves++;
    }
    bool move_list1[64]={false};
    set_valid_moves(state->opponent_color,state,move_list1);
    int opponent_moves=0;
    for(int i=0;i<64;i++)
    {
        if(move_list1[i]==true)
            opponent_moves++;
    }
    if(my_moves > opponent_moves)
        return (100 * my_moves ) / (my_moves + opponent_moves);
    else if(my_moves < opponent_moves)
        return (-100 * opponent_moves) / (my_moves + opponent_moves);
    else
        return 0;
}

Tree_Node *Board::get_empty_node()
{
    Tree_Node *tn = (Tree_Node*)malloc(sizeof(Tree_Node));
    tn->move[0] = -1;
    tn->move[1] = -1;
    tn->best_child = NULL;
    return tn;
}

State *Board::duplicate_state(State *state)
{
    State *temp = (State*)malloc(sizeof(State));
    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
            temp->board_state[i][j] = state->board_state[i][j];
    temp->current_color = state->current_color;
    temp->opponent_color = state->opponent_color;
    return temp;
}

void Board::create_tree(Tree_Node *root,int current_level,int depth)
{
    if(!root || current_level > depth)
        return;
    bool move_list[64]={false};
    set_valid_moves((int)root->state->current_color,(root->state),move_list);
    for(int i=0;i<64;i++)
    {
        if(move_list[i] == true)
        {
            Tree_Node *temp = get_empty_node();
            temp->move[0] = i/8;
            temp->move[1] = i%8;
            temp->state = duplicate_state((root->state));
            temp->state->board_state[i/8][i%8] = temp->state->current_color;
            capture_pieces(i/8,i%8,(temp->state),false);
            temp->state->current_color = 1 - temp->state->current_color;
            temp->state->opponent_color = 1 - temp->state->current_color;
            root->children.push_back(temp);
            create_tree(temp,1+current_level,depth);
        }
    }
}

double Board::mini_max(Tree_Node *root,int depth,int player,int current_color,double alpha,double beta)
{
    if(depth==0 || root->children.size() == 0)
        return heuristic(root->state);
    double current_value;
    if(current_color == player )
    {
        current_value = -1 * INF;
        for(auto &child : root->children)
        {
            if(current_value < beta)
            {
                double child_value = mini_max(child,depth-1,player,1-current_color,current_value,beta);
                if(current_value < child_value)
                {
                    current_value = child_value;
                    root->best_child = child;
                }
            }
        }
    }
    else
    {
        current_value = INF;
        for(auto &child : root->children)
        {
            if(current_value < alpha)
            {
                double child_value = mini_max(child,depth-1,player,1-current_color,alpha,current_value);
                if(current_value > child_value)
                {
                    current_value = child_value;
                    root->best_child = child;
                }
            }
        }
    }
    return current_value;
}

int Board::get_move(Tree_Node *root,int depth,int player,int current_color)
{
    mini_max(root,depth,player,current_color,-1*INF,INF);
    root = root->best_child;
    return root->move[0] * 8 + root->move[1];
}

