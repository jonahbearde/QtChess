#ifndef DUALGAME_H
#define DUALGAME_H

#include "ChessBoard.h"

class DualGame:public ChessBoard
{
public:
    DualGame(QString titleName,bool format);
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void drawChessman(QPainter&,int); //画棋子
};

#endif // DUALGAME_H
