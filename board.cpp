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
            board_state[row][col]=EMPTY;
    occupy_cell(3,3,"WHITE");
    occupy_cell(4,4,"WHITE");
    occupy_cell(3,4,"BLACK");
    occupy_cell(4,3,"BLACK");
}

void Board::piece_added_slot(const qint32 row, const qint32 column)
{
    set_valid_moves(current_color);
    bool moves_present=false;
    bool chance_flipped=false;
    int clicked_index=row*8+column;
    for(int i=0;i<64;i++)
        moves_present=moves_present || move_list[i];
    if(moves_present)
    {
        if(move_list[clicked_index])
        {
            if(current_color==BLACK)
                occupy_cell(row,column,"BLACK");
            else if(current_color==WHITE)
                occupy_cell(row,column,"WHITE");
            capture_pieces(row,column,current_color);
            current_color=1-current_color;
            chance_flipped=true;
            calculate_score();
        }
    }
    else
    {
        current_color=1-current_color;
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
        QString current_color_string;
        if(current_color==WHITE)
            current_color_string="WHITE's turn";
        else if(current_color==BLACK)
            current_color_string="BLACK's turn";
        QMetaObject::invokeMethod(controls_object, "status_update",
                                  Q_ARG(QVariant,current_color_string));
    }
}

void Board::occupy_cell(int row, int column, QString color)
{
    QMetaObject::invokeMethod(board_obj, "occupy_cell",
                              Q_ARG(QVariant,row),Q_ARG(QVariant,column),Q_ARG(QVariant,color));
    if(color == "WHITE")
    {
        board_state[row][column]=WHITE;
    }
    else if(color == "BLACK")
    {
        board_state[row][column]=BLACK;
    }
}
void Board::print_board()
{
    for(int row=0;row<8;row++)
    {
        for(int col=0;col<8;col++)
        {
            cout<< board_state[row][col]<<" ";
        }
        cout<<endl;
    }
    cout<< "*****************************" <<endl;
}
void Board::set_valid_moves(int color)
{
    for(int i=0;i<64;i++)
        move_list[i]=false;
    for(int row=0;row<8;row++)
    {
        for(int col=0;col<8;col++)
        {
            if (board_state[row][col]==color)
            {
                int flag = 0;
                for(int i=row-1;i>=0;i--)
                {
                    if (board_state[i][col]==1-color)
                        flag=1;
                    else if (board_state[i][col]==EMPTY && flag==1)
                    {
                        move_list[i*8+col]=true;
                        break;
                    }
                    else if (board_state[i][col]==EMPTY && flag==0)
                        break;
                    else if (board_state[i][col]==color)
                        break;
                }
                flag=0;
                for(int i=row+1;i<8;i++)
                {
                    if (board_state[i][col]==1-color)
                        flag=1;
                    else if (board_state[i][col]==EMPTY && flag==1)
                    {
                        move_list[i*8+col]=true;
                        break;
                    }
                    else if (board_state[i][col]==EMPTY && flag==0)
                        break;
                    else if (board_state[i][col]==color)
                        break;

                }
                flag=0;
                for(int i=col-1;i>=0;i--)
                {
                    if (board_state[row][i]==1-color)
                        flag=1;
                    else if (board_state[row][i]==EMPTY && flag==1)
                    {
                        move_list[row*8+i]=true;
                        break;
                    }
                    else if (board_state[row][i]==EMPTY && flag==0)
                        break;
                    else if (board_state[row][i]==color)
                        break;
                }
                flag=0;
                for(int i=col+1;i<8;i++)
                {
                    if (board_state[row][i]==1-color)
                        flag=1;
                    else if (board_state[row][i]==EMPTY && flag==1)
                    {
                        move_list[row*8+i]=true;
                        break;
                    }
                    else if (board_state[row][i]==EMPTY && flag==0)
                        break;
                    else if (board_state[row][i]==color)
                        break;
                }
                flag=0;
                int i=1;
                while(row-i>=0 && col-i>=0)
                {
                    if (board_state[row-i][col-i]==1-color)
                        flag=1;
                    else if (board_state[row-i][col-i]==EMPTY && flag==1)
                    {
                        move_list[(row-i)*8+(col-i)]=true;
                        break;
                    }
                    else if (board_state[row-i][col-i]==EMPTY && flag==0)
                        break;
                    else if (board_state[row-i][col-i]==color)
                        break;
                    i++;
                }
                flag=0;
                i=1;
                while(row+i<8 && col+i<8)
                {
                    if (board_state[row+i][col+i]==1-color)
                        flag=1;
                    else if (board_state[row+i][col+i]==EMPTY && flag==1)
                    {
                        move_list[(row+i)*8+(col+i)]=true;
                        break;
                    }
                    else if (board_state[row+i][col+i]==EMPTY && flag==0)
                        break;
                    else if (board_state[row+i][col+i]==color)
                        break;
                    i++;
                }
                flag=0;
                i=1;
                while(row-i>=0 && col+i<8)
                {
                    if (board_state[row-i][col+i]==1-color)
                        flag=1;
                    else if (board_state[row-i][col+i]==EMPTY && flag==1)
                    {
                        move_list[(row-i)*8+(col+i)]=true;
                        break;
                    }
                    else if (board_state[row-i][col+i]==EMPTY && flag==0)
                        break;
                    else if (board_state[row-i][col+i]==color)
                        break;
                    i++;
                }
                flag=0;
                i=1;
                while(row+i<8 && col-i>=0)
                {
                    if (board_state[row+i][col-i]==1-color)
                        flag=1;
                    else if (board_state[row+i][col-i]==EMPTY && flag==1)
                    {
                        move_list[(row+i)*8+(col-i)]=true;
                        break;
                    }
                    else if (board_state[row+i][col-i]==EMPTY && flag==0)
                        break;
                    else if (board_state[row+i][col-i]==color)
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

void Board::capture_pieces(int row,int col,int color)
{
    QString color_string;
    if(color==BLACK)
        color_string="BLACK";
    else if(color==WHITE)
        color_string="WHITE";
    vector <int> temp;
    if (board_state[row][col]==color)
    {
        temp.erase(temp.begin(),temp.end());
        for(int i=row-1;i>=0;i--)
        {
            if (board_state[i][col]==1-color)
            {
                temp.push_back(i*8+col);
            }
            else if (board_state[i][col]==EMPTY)
                break;
            else if (board_state[i][col]==color)
            {
                while(!temp.empty())
                {
                    int p=temp.back()/8;
                    int q=temp.back()%8;
                    temp.pop_back();
                    board_state[p][q]=1 - board_state[p][q];
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
            if (board_state[i][col]==1-color)
            {
                temp.push_back(i*8+col);
            }
            else if (board_state[i][col]==EMPTY)
                break;
            else if (board_state[i][col]==color)
            {
                while(!temp.empty())
                {
                    int p=temp.back()/8;
                    int q=temp.back()%8;
                    temp.pop_back();
                    board_state[p][q]=1 - board_state[p][q];
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
            if (board_state[row][i]==1-color)
            {
                temp.push_back(row*8+i);
            }
            else if (board_state[row][i]==EMPTY)
                break;
            else if (board_state[row][i]==color)
            {
                while(!temp.empty())
                {
                    int p=temp.back()/8;
                    int q=temp.back()%8;
                    temp.pop_back();
                    board_state[p][q]=1 - board_state[p][q];
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
            if (board_state[row][i]==1-color)
            {
                temp.push_back(row*8+i);
            }
            else if (board_state[row][i]==EMPTY)
                break;
            else if (board_state[row][i]==color)
            {
                while(!temp.empty())
                {
                    int p=temp.back()/8;
                    int q=temp.back()%8;
                    temp.pop_back();
                    board_state[p][q]=1 - board_state[p][q];
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
            if (board_state[row-i][col-i]==1-color)
            {
                temp.push_back((row-i)*8+(col-i));
            }
            else if (board_state[row-i][col-i]==EMPTY)
                break;
            else if (board_state[row-i][col-i]==color)
            {
                while(!temp.empty())
                {
                    int p=temp.back()/8;
                    int q=temp.back()%8;
                    temp.pop_back();
                    board_state[p][q]=1 - board_state[p][q];
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
            if (board_state[row+i][col+i]==1-color)
            {
                temp.push_back((row+i)*8+(col+i));
            }
            else if (board_state[row+i][col+i]==EMPTY)
                break;
            else if (board_state[row+i][col+i]==color)
            {
                while(!temp.empty())
                {
                    int p=temp.back()/8;
                    int q=temp.back()%8;
                    temp.pop_back();
                    board_state[p][q]=1 - board_state[p][q];
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
            if (board_state[row-i][col+i]==1-color)
            {
                temp.push_back((row-i)*8+(col+i));
            }
            else if (board_state[row-i][col+i]==EMPTY)
                break;
            else if (board_state[row-i][col+i]==color)
            {
                while(!temp.empty())
                {
                    int p=temp.back()/8;
                    int q=temp.back()%8;
                    temp.pop_back();
                    board_state[p][q]=1 - board_state[p][q];
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
            if (board_state[row+i][col-i]==1-color)
            {
                temp.push_back((row+i)*8+(col-i));
            }
            else if (board_state[row+i][col-i]==EMPTY)
                break;
            else if (board_state[row+i][col-i]==color)
            {
                while(!temp.empty())
                {
                    int p=temp.back()/8;
                    int q=temp.back()%8;
                    temp.pop_back();
                    board_state[p][q]=1 - board_state[p][q];
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
    set_valid_moves(current_color);
    bool current_color_move=false;
    for(int i=0;i<64;i++)
        current_color_move=current_color_move || move_list[i];
    set_valid_moves(1-current_color);
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
            if(board_state[i][j]==BLACK)
                score_board[BLACK]++;
            else if(board_state[i][j]==WHITE)
                score_board[WHITE]++;
        }
    }
    QMetaObject::invokeMethod(controls_object, "set_score_board",
                              Q_ARG(QVariant,score_board[BLACK]),Q_ARG(QVariant,score_board[WHITE]));

}
