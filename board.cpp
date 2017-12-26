#include "board.h"
#include <QDebug>
#include <iostream>
using namespace std;
Board::Board(QObject *bobj,QObject *cobj)
{
    board_obj=bobj;
    controls_object=cobj;
    QObject::connect(board_obj, SIGNAL(piece_added_signal(qint32,qint32)),
                     this, SLOT(piece_added_slot(qint32,qint32)));
    for(int row=0;row<8;row++)
        for(int col=0;col<8;col++)
            state.board_state[row][col]=EMPTY;
    state.current_color=BLACK;
    state.opponent_color=WHITE;
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
}

void Board::piece_added_slot(const qint32 row, const qint32 column)
{
    set_valid_moves(state.current_color,state);
    bool moves_present=false;
    bool chance_flipped=false;
    int clicked_index=row*8+column;
    for(int i=0;i<64;i++)
        moves_present=moves_present || move_list[i];
    if(moves_present)
    {
        if(move_list[clicked_index])
        {
            occupy_cell(row,column,state);
            capture_pieces(row,column,state);
            state.current_color=1-state.current_color;
            state.opponent_color=1-state.current_color;
            chance_flipped=true;
            calculate_score();
        }
    }
    else
    {
        state.current_color=1-state.current_color;
        chance_flipped=true;
        QMetaObject::invokeMethod(controls_object, "status_update",
                                  Q_ARG(QVariant,"No more moves left, passing the turn"));
    }
    if(has_game_ended())
    {
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
    if(chance_flipped)
    {
        QString message;
        if(state.current_color==WHITE)
            message="WHITE's turn";
        else if(state.current_color==BLACK)
            message="BLACK's turn";
        QMetaObject::invokeMethod(controls_object, "status_update",
                                  Q_ARG(QVariant,message));
    }
}

void Board::occupy_cell(int row, int column,State &state)
{
    QString color_string;
    if(state.current_color==BLACK)
        color_string="BLACK";
    else if(state.current_color==WHITE)
        color_string="WHITE";
    QMetaObject::invokeMethod(board_obj, "occupy_cell",
                              Q_ARG(QVariant,row),Q_ARG(QVariant,column),Q_ARG(QVariant,color_string));
    state.board_state[row][column]=state.current_color;
    print_board();
}
void Board::print_board()
{
    for(int row=0;row<8;row++)
    {
        for(int col=0;col<8;col++)
        {
            cout<< state.board_state[row][col]<<" ";
        }
        cout<<endl;
    }
    cout<< "*****************************" <<endl;
}
void Board::set_valid_moves(int color,State &state)
{
    for(int i=0;i<64;i++)
        move_list[i]=false;
    for(int row=0;row<8;row++)
    {
        for(int col=0;col<8;col++)
        {
            if (state.board_state[row][col]==color)
            {
                int flag = 0;
                for(int i=row-1;i>=0;i--)
                {
                    if (state.board_state[i][col]==1-color)
                        flag=1;
                    else if (state.board_state[i][col]==EMPTY && flag==1)
                    {
                        move_list[i*8+col]=true;
                        break;
                    }
                    else if (state.board_state[i][col]==EMPTY && flag==0)
                        break;
                    else if (state.board_state[i][col]==color)
                        break;
                }
                flag=0;
                for(int i=row+1;i<8;i++)
                {
                    if (state.board_state[i][col]==1-color)
                        flag=1;
                    else if (state.board_state[i][col]==EMPTY && flag==1)
                    {
                        move_list[i*8+col]=true;
                        break;
                    }
                    else if (state.board_state[i][col]==EMPTY && flag==0)
                        break;
                    else if (state.board_state[i][col]==color)
                        break;

                }
                flag=0;
                for(int i=col-1;i>=0;i--)
                {
                    if (state.board_state[row][i]==1-color)
                        flag=1;
                    else if (state.board_state[row][i]==EMPTY && flag==1)
                    {
                        move_list[row*8+i]=true;
                        break;
                    }
                    else if (state.board_state[row][i]==EMPTY && flag==0)
                        break;
                    else if (state.board_state[row][i]==color)
                        break;
                }
                flag=0;
                for(int i=col+1;i<8;i++)
                {
                    if (state.board_state[row][i]==1-color)
                        flag=1;
                    else if (state.board_state[row][i]==EMPTY && flag==1)
                    {
                        move_list[row*8+i]=true;
                        break;
                    }
                    else if (state.board_state[row][i]==EMPTY && flag==0)
                        break;
                    else if (state.board_state[row][i]==color)
                        break;
                }
                flag=0;
                int i=1;
                while(row-i>=0 && col-i>=0)
                {
                    if (state.board_state[row-i][col-i]==1-color)
                        flag=1;
                    else if (state.board_state[row-i][col-i]==EMPTY && flag==1)
                    {
                        move_list[(row-i)*8+(col-i)]=true;
                        break;
                    }
                    else if (state.board_state[row-i][col-i]==EMPTY && flag==0)
                        break;
                    else if (state.board_state[row-i][col-i]==color)
                        break;
                    i++;
                }
                flag=0;
                i=1;
                while(row+i<8 && col+i<8)
                {
                    if (state.board_state[row+i][col+i]==1-color)
                        flag=1;
                    else if (state.board_state[row+i][col+i]==EMPTY && flag==1)
                    {
                        move_list[(row+i)*8+(col+i)]=true;
                        break;
                    }
                    else if (state.board_state[row+i][col+i]==EMPTY && flag==0)
                        break;
                    else if (state.board_state[row+i][col+i]==color)
                        break;
                    i++;
                }
                flag=0;
                i=1;
                while(row-i>=0 && col+i<8)
                {
                    if (state.board_state[row-i][col+i]==1-color)
                        flag=1;
                    else if (state.board_state[row-i][col+i]==EMPTY && flag==1)
                    {
                        move_list[(row-i)*8+(col+i)]=true;
                        break;
                    }
                    else if (state.board_state[row-i][col+i]==EMPTY && flag==0)
                        break;
                    else if (state.board_state[row-i][col+i]==color)
                        break;
                    i++;
                }
                flag=0;
                i=1;
                while(row+i<8 && col-i>=0)
                {
                    if (state.board_state[row+i][col-i]==1-color)
                        flag=1;
                    else if (state.board_state[row+i][col-i]==EMPTY && flag==1)
                    {
                        move_list[(row+i)*8+(col-i)]=true;
                        break;
                    }
                    else if (state.board_state[row+i][col-i]==EMPTY && flag==0)
                        break;
                    else if (state.board_state[row+i][col-i]==color)
                        break;
                    i++;
                }

            }
        }
    }
}
void Board::print_valid_moves()
{
    for(int i=0;i<64;i++)
        if(move_list[i])
            cout << i / 8 << " " << i % 8 << endl;
    cout<<"***************"<<endl;
}
void Board::capture_pieces(int row,int col,State &state)
{
    QString color_string;
    if(state.current_color==BLACK)
        color_string="BLACK";
    else if(state.current_color==WHITE)
        color_string="WHITE";
    vector <int> temp;
    cout<< state.board_state[row][col]<<" "<<state.current_color<<endl;
    if (state.board_state[row][col]==state.current_color)
    {
        temp.erase(temp.begin(),temp.end());
        for(int i=row-1;i>=0;i--)
        {
            if (state.board_state[i][col]==state.opponent_color)
            {
                temp.push_back(i*8+col);
            }
            else if (state.board_state[i][col]==EMPTY)
                break;
            else if (state.board_state[i][col]==state.current_color)
            {
                while(!temp.empty())
                {
                    int p=temp.back()/8;
                    int q=temp.back()%8;
                    temp.pop_back();
                    state.board_state[p][q]=1 - state.board_state[p][q];
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
            if (state.board_state[i][col]==state.opponent_color)
            {
                temp.push_back(i*8+col);
            }
            else if (state.board_state[i][col]==EMPTY)
                break;
            else if (state.board_state[i][col]==state.current_color)
            {
                while(!temp.empty())
                {
                    int p=temp.back()/8;
                    int q=temp.back()%8;
                    temp.pop_back();
                    state.board_state[p][q]=1 - state.board_state[p][q];
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
            if (state.board_state[row][i]==state.opponent_color)
            {
                temp.push_back(row*8+i);
            }
            else if (state.board_state[row][i]==EMPTY)
                break;
            else if (state.board_state[row][i]==state.current_color)
            {
                while(!temp.empty())
                {
                    int p=temp.back()/8;
                    int q=temp.back()%8;
                    temp.pop_back();
                    state.board_state[p][q]=1 - state.board_state[p][q];
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
            if (state.board_state[row][i]==state.opponent_color)
            {
                temp.push_back(row*8+i);
            }
            else if (state.board_state[row][i]==EMPTY)
                break;
            else if (state.board_state[row][i]==state.current_color)
            {
                while(!temp.empty())
                {
                    int p=temp.back()/8;
                    int q=temp.back()%8;
                    temp.pop_back();
                    state.board_state[p][q]=1 - state.board_state[p][q];
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
            if (state.board_state[row-i][col-i]==state.opponent_color)
            {
                temp.push_back((row-i)*8+(col-i));
            }
            else if (state.board_state[row-i][col-i]==EMPTY)
                break;
            else if (state.board_state[row-i][col-i]==state.current_color)
            {
                while(!temp.empty())
                {
                    int p=temp.back()/8;
                    int q=temp.back()%8;
                    temp.pop_back();
                    state.board_state[p][q]=1 - state.board_state[p][q];
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
            if (state.board_state[row+i][col+i]==state.opponent_color)
            {
                temp.push_back((row+i)*8+(col+i));
            }
            else if (state.board_state[row+i][col+i]==EMPTY)
                break;
            else if (state.board_state[row+i][col+i]==state.current_color)
            {
                while(!temp.empty())
                {
                    int p=temp.back()/8;
                    int q=temp.back()%8;
                    temp.pop_back();
                    state.board_state[p][q]=1 - state.board_state[p][q];
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
            if (state.board_state[row-i][col+i]==state.opponent_color)
            {
                temp.push_back((row-i)*8+(col+i));
            }
            else if (state.board_state[row-i][col+i]==EMPTY)
                break;
            else if (state.board_state[row-i][col+i]==state.current_color)
            {
                while(!temp.empty())
                {
                    int p=temp.back()/8;
                    int q=temp.back()%8;
                    temp.pop_back();
                    state.board_state[p][q]=1 - state.board_state[p][q];
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
            if (state.board_state[row+i][col-i]==state.opponent_color)
            {
                temp.push_back((row+i)*8+(col-i));
            }
            else if (state.board_state[row+i][col-i]==EMPTY)
                break;
            else if (state.board_state[row+i][col-i]==state.current_color)
            {
                while(!temp.empty())
                {
                    int p=temp.back()/8;
                    int q=temp.back()%8;
                    temp.pop_back();
                    state.board_state[p][q]=1 - state.board_state[p][q];
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

bool Board::has_game_ended()
{
    set_valid_moves(state.current_color,state);
    bool current_color_move=false;
    for(int i=0;i<64;i++)
        current_color_move=current_color_move || move_list[i];
    set_valid_moves(state.opponent_color,state);
    bool opponent_color_move=false;
    for(int i=0;i<64;i++)
        opponent_color_move=opponent_color_move || move_list[i];
    return !current_color_move && !opponent_color_move;
}

void Board::calculate_score()
{
    score_board[BLACK]=0;
    score_board[WHITE]=0;
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(state.board_state[i][j]==BLACK)
                score_board[BLACK]++;
            else if(state.board_state[i][j]==WHITE)
                score_board[WHITE]++;
        }
    }
    QMetaObject::invokeMethod(controls_object, "set_score_board",
                              Q_ARG(QVariant,score_board[BLACK]),Q_ARG(QVariant,score_board[WHITE]));

}

double Board::heuristic(int **board_state)
{

}

piece_return Board::piece_count(int **board_state)
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
        {0,-1},
        {1,-1},
        {-1,0},
        {1,0},
        {-1,1},
        {0,1},
        {1,1}
    };
    int my_pices=0,opponent_pieces=0,disk_square=0;
    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
        {

        }



}
