#ifndef BOARD_H
#define BOARD_H
#define BLACK 0
#define WHITE 1
#include <QObject>
class Board : public QObject
{
    Q_OBJECT
public slots:
    void piece_added_slot(const qint32 row,const qint32 column,const QString &color);
public:
    Board(QObject *obj);
    void occupy_cell(int row,int column,QString color);
private:
    QObject *board_obj;

};

#endif // BOARD_H
