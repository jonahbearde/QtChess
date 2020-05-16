#include "ChessBoard.h"
#include <QPainter>
#include <QTime>

ChessBoard::ChessBoard(QString titleName){
    setWindowTitle(titleName);
    setFixedSize(800,600);
    init();
}

void ChessBoard::init(){
    int n=getRandNum();
    if(n==0){
        curSide=false;
        selectedID=20;
    }else{
        curSide=true;
        selectedID=4;
    }
    for(int id=0;id<32;id++)
    {
        chessman[id].init(id);
    }
}

void ChessBoard::clear(){
    init();
    curSide=false;
    int n=getRandNum();
    if(n==0){
        curSide=false;
        selectedID=20;
    }else{
        curSide=true;
        selectedID=4;
    }
    vecID.clear();
    vecSteps.clear();
    vecDead.clear();
}

void ChessBoard::rollback(){
    if(!vecSteps.isEmpty()){
        selectedID=vecID.last();
        clkPos pos=vecSteps.last();
        chessman[selectedID]._curRow=pos.clkRow;
        chessman[selectedID]._curCol=pos.clkCol;
        vecDead.pop_back();
        vecID.pop_back();
        vecSteps.pop_back();
        curSide=!curSide;
        update();
    }
}
int ChessBoard::getID(QPoint pt)
{
    int row,col;
    int id=-1;
    row=getClkPos(pt).clkRow;
    col=getClkPos(pt).clkCol;
    for(int i=0;i<32;i++)
    {
        if(!vecDead.count(i))
        {
            if(chessman[i]._curRow==row && chessman[i]._curCol==col)
                id=i;
        }
    }
    return id;
}

QPoint ChessBoard::startPos(int row,int col)
{
    double pos_x=offset_x+sqLen*col;
    double pos_y=offset_y+sqLen*row;
    return QPoint(pos_x,pos_y);
}

clkPos ChessBoard::getClkPos(QPoint pt)
{
    int row,col;
    if(pt.x()>offset_x && pt.x()<(542-offset_x) && pt.y()>offset_y && pt.y()<(600-offset_y))
    {
        row=(pt.y()-offset_y_corner)/sqLen;
        if((pt.y()-offset_y_corner)%sqLen>sqLen/2)
            ++row;
        col=(pt.x()-offset_x_corner)/sqLen;
        if((pt.x()-offset_x_corner)%sqLen>sqLen/2)
            ++col;
        return clkPos(row,col);
    }else{
        return clkPos(-1,-1);
    }
}

int ChessBoard::abs(int num)
{
   if(num<0)
       return -num;
   return num;
}

int ChessBoard::getRandNum(){
    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);
    return qrand() % 2;
}

bool ChessBoard::ifLive(int id)
{
    int count=vecDead.count(id);
    return !(bool)count;
}

bool ChessBoard::canMove(int clkRow,int clkCol){
    //调用不同类型棋子的规则
    switch (chessman[selectedID]._type) {
    case ChessMan::CHE: return canMoveCHE(clkRow,clkCol);
        break;
    case ChessMan::MA: return canMoveMA(clkRow,clkCol);
        break;
    case ChessMan::XIANG: return canMoveXIANG(clkRow,clkCol);
        break;
    case ChessMan::SHI: return canMoveSHI(clkRow,clkCol);
        break;
    case ChessMan::JIANG: return canMoveJIANG(clkRow,clkCol);
        break;
    case ChessMan::PAO: return canMovePAO(clkRow,clkCol);
        break;
    case ChessMan::BING: return canMoveBING(clkRow,clkCol);
        break;
    }
}

bool ChessBoard::canMoveCHE(int clkRow,int clkCol){
    //待优化!
    int dist,dist_other;
    //如果选中的棋子在同一行上
    if(clkRow==chessman[selectedID]._curRow)
    {
        dist=clkCol-chessman[selectedID]._curCol;//获取点击的棋子与选中的棋子的距离
        //检验是否有棋子在两个棋子之间
        for(int id=0;id<32;id++)
        {
            if(!(id==selectedID || id==clkID) && ifLive(id))//排除点击和选中的棋子
            {
                if(chessman[id]._curRow==clkRow)
                {
                        dist_other=chessman[id]._curCol-chessman[selectedID]._curCol;
                        //如果中间有棋子在同一方向，且距离小于点击和选中的棋子之间的距离
                        if((dist_other*dist)>0 && abs(dist_other)<abs(dist))
                        {
                            return false;
                        }
                }
            }
        }
        return true;
    }
    if(clkCol==chessman[selectedID]._curCol)
    {
        dist=clkRow-chessman[selectedID]._curRow;
        for(int id=0;id<32;id++)
        {
            if(!(id==selectedID || id==clkID) && ifLive(id))
            {
                if(chessman[id]._curCol==clkCol)
                {
                    dist_other=chessman[id]._curRow-chessman[selectedID]._curRow;
                    if((dist_other*dist)>0 && abs(dist_other)<abs(dist))
                    {
                        return false;
                    }
                }
            }
        }
        return true;
    }
    return false;
}

bool ChessBoard::canMoveMA(int clkRow,int clkCol){
    int dist_row,dist_col;
    //定义鼠标点击的距离
    dist_row=clkRow-chessman[selectedID]._curRow;
    dist_col=clkCol-chessman[selectedID]._curCol;
    if(abs(dist_col)==1)
    {
        if(abs(dist_row)==2)
        {
            //检验每个棋子
            for(int id=0;id<32;id++)
            {
                //检查正前方是否有棋子
                int dist_other_row=chessman[id]._curRow-chessman[selectedID]._curRow;
                if(ifLive(id) && dist_other_row && (dist_row/dist_other_row)==2 && chessman[id]._curCol==chessman[selectedID]._curCol)
                {
                    return false;
                }
            }
            return true;
        }
        return false;
    }
    if(abs(dist_row)==1)
    {
        if(abs(dist_col)==2)
        {
            for(int id=0;id<32;id++)
            {
                int dist_other_col=chessman[id]._curCol-chessman[selectedID]._curCol;
                if(ifLive(id) && dist_other_col && (dist_col/dist_other_col)==2 && chessman[id]._curRow==chessman[selectedID]._curRow)
                {
                    return false;
                }
            }
            return true;
        }
        return false;
    }
    return false;
}

bool ChessBoard::canMoveXIANG(int clkRow,int clkCol){
    int dist_row,dist_col;
    dist_row=clkRow-chessman[selectedID]._curRow;
    dist_col=clkCol-chessman[selectedID]._curCol;
    if(curSide && clkRow>-1 && clkRow<5)
    {
        if(abs(dist_col)==2 && abs(dist_row)==2)
        {
            for(int id=0;id<32;id++)
            {
                int dist_other_row=chessman[id]._curRow-chessman[selectedID]._curRow;
                int dist_other_col=chessman[id]._curCol-chessman[selectedID]._curCol;
                //如果存在棋子挡象，同方向行和列距离为1/2原行和列距离
                if(ifLive(id) && dist_other_row && dist_other_col && (dist_col/dist_other_col)==2 && (dist_row/dist_other_row)==2)
                {
                    return false;
                }
            }
            return true;
        }
        return false;
    }
    if(!curSide && clkRow>4 && clkRow<10)
    {
        if(abs(dist_col)==2 && abs(dist_row)==2)
        {
            for(int id=0;id<32;id++)
            {
                int dist_other_row=chessman[id]._curRow-chessman[selectedID]._curRow;
                int dist_other_col=chessman[id]._curCol-chessman[selectedID]._curCol;
                if(ifLive(id) && dist_other_row && dist_other_col && (dist_col/dist_other_col)==2 && (dist_row/dist_other_row)==2)
                {
                    return false;
                }
            }
            return true;
        }
        return false;
    }
    return false;
}

bool ChessBoard::canMoveSHI(int clkRow,int clkCol){
    int dist_row,dist_col;
    dist_row=clkRow-chessman[selectedID]._curRow;
    dist_col=clkCol-chessman[selectedID]._curCol;
    if(curSide && clkRow>-1 && clkRow<3 && clkCol>2 && clkCol<6)
    {
        if(abs(dist_row)==1 && abs(dist_col)==1)
        {
            return true;
        }
        return false;
    }
    if(!curSide && clkRow>6 && clkRow<10 && clkCol>2 && clkCol<6)
    {
        if(abs(dist_row)==1 && abs(dist_col)==1)
        {
            return true;
        }
        return false;
    }
    return false;
}

bool ChessBoard::canMoveJIANG(int clkRow,int clkCol){
    int dist_row,dist_col;
    dist_row=clkRow-chessman[selectedID]._curRow;
    dist_col=clkCol-chessman[selectedID]._curCol;
    if(curSide && clkRow>-1 && clkRow<3 && clkCol>2 && clkCol<6)
    {
        if((abs(dist_row)==1 && abs(dist_col)==0) || (abs(dist_col)==1 && abs(dist_row)==0))
        {
            return true;
        }
        return false;
    }
    if(!curSide && clkRow>6 && clkRow<10 && clkCol>2 && clkCol<6)
    {
        if((abs(dist_row)==1 && abs(dist_col)==0) || (abs(dist_col)==1 && abs(dist_row)==0))
        {
            return true;
        }
        return false;
    }
    return false;
}

bool ChessBoard::canMovePAO(int clkRow,int clkCol){
    int dist_row=clkRow-chessman[selectedID]._curRow;
    int dist_col=clkCol-chessman[selectedID]._curCol;
    if(clkID==-1)
    {
        if(!dist_col)
        {
            for(int id=0;id<32;id++)
            {
                int dist_other_row=chessman[id]._curRow-chessman[selectedID]._curRow;
                int dist_other_col=chessman[id]._curCol-chessman[selectedID]._curCol;
                if((id!=selectedID) && ifLive(id) && (!dist_other_col) && (dist_row*dist_other_row)>0 && abs(dist_other_row)<abs(dist_row))
                {
                    return false;
                }
            }
            return true;
        }
        if(!dist_row)
        {
            for(int id=0;id<32;id++)
            {
                int dist_other_row=chessman[id]._curRow-chessman[selectedID]._curRow;
                int dist_other_col=chessman[id]._curCol-chessman[selectedID]._curCol;
                if((id!=selectedID) && ifLive(id) && (!dist_other_row) && (dist_col*dist_other_col)>0 && abs(dist_other_col)<abs(dist_col))
                {
                    return false;
                }
            }
            return true;
        }
        return false;
    }
    else
    {
        if(!dist_col)
        {
            int count=0;
            for(int id=0;id<32;id++)
            {
                int dist_other_row=chessman[id]._curRow-chessman[selectedID]._curRow;
                int dist_other_col=chessman[id]._curCol-chessman[selectedID]._curCol;
                if((id!=selectedID) && ifLive(id) && (!dist_other_col) && (dist_row*dist_other_row)>0 && abs(dist_other_row)<abs(dist_row))
                {
                    count++;
                    if(count>1) return false;
                }
            }
            if(count==1) return true;
            return false;
        }
        if(!dist_row)
        {
            int count=0;
            for(int id=0;id<32;id++)
            {
                int dist_other_row=chessman[id]._curRow-chessman[selectedID]._curRow;
                int dist_other_col=chessman[id]._curCol-chessman[selectedID]._curCol;
                if((id!=selectedID) && ifLive(id) && (!dist_other_row) && (dist_col*dist_other_col)>0 && abs(dist_other_col)<abs(dist_col))
                {
                    count++;
                    if(count>1) return false;
                }
            }
            if(count==1) return true;
            return false;
        }
        return false;
    }
    return false;
}

bool ChessBoard::canMoveBING(int clkRow,int clkCol){
    int dist_row,dist_col;
    dist_row=clkRow-chessman[selectedID]._curRow;
    dist_col=clkCol-chessman[selectedID]._curCol;
    int curRow=chessman[selectedID]._curRow;
    if(curSide)
    {
        if(curRow<5)
        {
            if(!dist_col && dist_row==1) return true;
            return false;
        }else
        {
            if((abs(dist_col)==1 && dist_row==0) || (dist_col==0 && dist_row==1)) return true;
            return false;
        }
    }
    if(!curSide)
    {
        if(curRow>4)
        {
            if(!dist_col && dist_row==-1) return true;
            return false;
        }else
        {
            if((abs(dist_col)==1 && dist_row==0) || (dist_col==0 && dist_row==-1)) return true;
            return false;
        }
    }
    return false;
}
