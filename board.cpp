#include "board.h"
#include <QDebug>
Board::Board()
{

}

void Board::piece_added_slot(const qint32 row, const qint32 column, const QString &color){
    qDebug() << row << column << color ;
}
