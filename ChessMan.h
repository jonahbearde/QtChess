#ifndef CHESSMAN_H
#define CHESSMAN_H
#include <QPixmap>

class ChessMan
{
public:
    ChessMan();
    int _curRow; //当前所在行
    int _curCol;
    int _id; //棋子的id
    bool _dead; //棋子是否被吃
    bool _side;
    enum TYPE{CHE,MA,XIANG,SHI,JIANG,PAO,BING}; //定义棋子的类型
    TYPE _type;
    QPixmap _img,_imgSl; //定义棋子未选中,选中和死亡时加载的图片

    void init(int id); //初始化棋子
};

#endif // CHESSMAN_H
