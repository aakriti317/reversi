#include "board.h"
#include <QDebug>
Board::Board(QObject *obj)
{
    board_obj=obj;
    QObject::connect(board_obj, SIGNAL(piece_added_signal(qint32,qint32,QString)),
                     this, SLOT(piece_added_slot(qint32,qint32,QString)));
}

void Board::piece_added_slot(const qint32 row, const qint32 column, const QString &color){
    qDebug() << row << column << color ;
}

void Board::occupy_cell(int row, int column, QString color){
    QMetaObject::invokeMethod(board_obj, "occupy_cell",
                              Q_ARG(QVariant,row),Q_ARG(QVariant,column),Q_ARG(QVariant,color));
}
