#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "ChessMan.h"
#include <QWidget>

struct clkPos{
    int clkRow;
    int clkCol;
    clkPos(){};
    clkPos(int row,int col)
    {
        clkRow=row;
        clkCol=col;
    }
};

class ChessBoard : public QWidget
{
    Q_OBJECT
public:
    ChessBoard(QString);

    int abs(int);
    int getID(QPoint);//根据鼠标所在点获取棋子的id
    QPoint startPos(int,int);//根据棋子的行列值得到drawPixmap的初始点参数
    bool ifLive(int);
    void init();
    void clear();
    void rollback();//悔棋
    int getRandNum();

    //棋盘参数
    ChessMan chessman[32];//创建32个棋子
    double offset_x=5.5;//第一个棋子左上角的x偏移
    double offset_y=4.5;
    int offset_x_corner=34;//棋盘左上角点的x偏移
    int offset_y_corner=33;
    int sqLen=60;//相邻两个棋子中心的距离
    clkPos getClkPos(QPoint);//得到鼠标点击位置的行和列
    int clkID;//定义当前点击到的棋子的id
    int selectedID;//设定当前被选中棋子的id
    int deadID=-1;//被吃掉棋子的id
    QVector<int> vecDead;//被吃的棋子列表
    QVector<clkPos> vecSteps;//走法列表
    QVector<int> vecID;//走过的棋子
    bool curSide=false;//当前阵营

    //走棋规则
    bool canMove(int,int);
    bool canMoveCHE(int,int);
    bool canMoveMA(int,int);
    bool canMoveXIANG(int,int);
    bool canMoveSHI(int,int);
    bool canMoveJIANG(int,int);
    bool canMovePAO(int,int);
    bool canMoveBING(int,int);
signals:

};

#endif // CHESSBOARD_H
