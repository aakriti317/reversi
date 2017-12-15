#include "board.h"
#include <QDebug>
#include <iostream>
using namespace std;
Board::Board(QObject *obj)
{
    board_obj=obj;
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
    print_valid_moves();
    int clicked_index=row*8+column;
    if(move_list[clicked_index])
    {
        if(current_color==BLACK)
            occupy_cell(row,column,"BLACK");
        else if(current_color==WHITE)
            occupy_cell(row,column,"WHITE");
        capture_pieces(row,column,current_color);
        current_color=1-current_color;
    }
    print_board();
}

void Board::occupy_cell(int row, int column, QString color){
    QMetaObject::invokeMethod(board_obj, "occupy_cell",
                              Q_ARG(QVariant,row),Q_ARG(QVariant,column),Q_ARG(QVariant,color));
    if(color=="WHITE")
        board_state[row][column]=WHITE;
    else if(color == "BLACK")
        board_state[row][column]=BLACK;
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
    bool list[64]={false};
    if(color==BLACK)
        color_string="BLACK";
    else if(color==WHITE)
        color_string="WHITE";

    if (board_state[row][col]==color)
    {
        for(int i=row-1;i>=0;i--)
        {
            if (board_state[i][col]==1-color)
            {
                //                QMetaObject::invokeMethod(board_obj, "change_color",
                //                                          Q_ARG(QVariant,i),Q_ARG(QVariant,col),Q_ARG(QVariant,color_string));
                //                board_state[i][col]=color;
                list[i*8+col]=true;
            }
            else if (board_state[i][col]==EMPTY)
                break;
            else if (board_state[i][col]==color)
                break;
        }
        for(int i=row+1;i<8;i++)
        {
            if (board_state[i][col]==1-color)
            {
                //                QMetaObject::invokeMethod(board_obj, "change_color",
                //                                          Q_ARG(QVariant,i),Q_ARG(QVariant,col),Q_ARG(QVariant,color_string));
                //                board_state[i][col]=color;
                list[i*8+col]=true;
            }
            else if (board_state[i][col]==EMPTY)
                break;
            else if (board_state[i][col]==color)
                break;

        }
        for(int i=col-1;i>=0;i--)
        {
            if (board_state[row][i]==1-color)
            {
                //                QMetaObject::invokeMethod(board_obj, "change_color",
                //                                          Q_ARG(QVariant,row),Q_ARG(QVariant,i),Q_ARG(QVariant,color_string));
                //                board_state[row][i]=color;
                list[row*8+i]=true;
            }
            else if (board_state[row][i]==EMPTY)
                break;
            else if (board_state[row][i]==color)
                break;
        }
        for(int i=col+1;i<8;i++)
        {
            if (board_state[row][i]==1-color)
            {
                //                QMetaObject::invokeMethod(board_obj, "change_color",
                //                                          Q_ARG(QVariant,row),Q_ARG(QVariant,i),Q_ARG(QVariant,color_string));
                //                board_state[row][i]=color;
                list[row*8+i]=true;
            }

            else if (board_state[row][i]==EMPTY)
                break;
            else if (board_state[row][i]==color)
                break;
        }
        int i=1;
        while(row-i>=0 && col-i>=0)
        {
            if (board_state[row-i][col-i]==1-color)
            {
                //                QMetaObject::invokeMethod(board_obj, "change_color",
                //                                          Q_ARG(QVariant,row-i),Q_ARG(QVariant,col-i),Q_ARG(QVariant,color_string));
                //                board_state[row-i][col-i]=color;
                list[(row-i)*8+(col-i)]=true;
            }
            else if (board_state[row-i][col-i]==EMPTY)
                break;
            else if (board_state[row-i][col-i]==color)
                break;
        }
        i=1;
        while(row+i<8 && col+i<8)
        {
            if (board_state[row+i][col+i]==1-color)
            {
                //                QMetaObject::invokeMethod(board_obj, "change_color",
                //                                          Q_ARG(QVariant,row+i),Q_ARG(QVariant,col+i),Q_ARG(QVariant,color_string));

                //                board_state[row+i][col+i]=color;
                list[(row+i)*8+(col+i)]=true;
            }
            else if (board_state[row+i][col+i]==EMPTY)
                break;
            else if (board_state[row+i][col+i]==color)
                break;
        }
        i=1;
        while(row-i>=0 && col+i<8)
        {
            if (board_state[row-i][col+i]==1-color)
            {
                //                QMetaObject::invokeMethod(board_obj, "change_color",
                //                                          Q_ARG(QVariant,row-i),Q_ARG(QVariant,col+i),Q_ARG(QVariant,color_string));
                //                board_state[row-i][col+i]=color;
                list[(row-i)*8+(col+i)]=true;
            }
            else if (board_state[row-i][col+i]==EMPTY)
                break;
            else if (board_state[row-i][col+i]==color)
                break;
        }
        i=1;
        while(row+i<8 && col-i>=0)
        {
            if (board_state[row+i][col-i]==1-color)
            {
                //                QMetaObject::invokeMethod(board_obj, "change_color",
                //                                          Q_ARG(QVariant,row+i),Q_ARG(QVariant,col-i),Q_ARG(QVariant,color_string));
                //                board_state[row+i][col-i]=color;
                list[(row+i)*8+(col+i)]=true;
            }
            else if (board_state[row+i][col-i]==EMPTY)
                break;
            else if (board_state[row+i][col-i]==color)
                break;
        }

    }
    for(int j=0;j<64;j++)
    {
        if(list[j])
        {
            board_state[j/8][j%8]=color;
            QMetaObject::invokeMethod(board_obj, "change_color",
                                      Q_ARG(QVariant,j/8),Q_ARG(QVariant,j%8),Q_ARG(QVariant,color_string));
            cout<<j/8<<" "<<j%8;
        }

    }
    cout<<endl;
}

