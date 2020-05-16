#include "DualGame.h"
#include "MainMenu.h"
#include <QPushButton>
#include <QPainter>
#include <QMouseEvent>
#include <QTimer>
#include <QMessageBox>

DualGame::DualGame(QString titleName):ChessBoard(titleName)
{
    QPushButton *btn_rollback=new QPushButton(this);
    QPushButton *btn_restart=new QPushButton(this);
    QPushButton *btn_exit=new QPushButton(this);
    btn_rollback->setStyleSheet("QPushButton{"
                                "width: 105px;"
                                "height: 55px;"
                                "border-image: url(:/img/rollback.png);"
                            "}");
    btn_restart->setStyleSheet("QPushButton{"
                                "width: 188px;"
                                "height: 55px;"
                                "border-image: url(:/img/restart.png);"
                                "}");
    btn_exit->setStyleSheet("QPushButton{"
                                "width: 188px;"
                                "height: 55px;"
                                "border-image: url(:/img/exit.png);"
                                "}");
    btn_rollback->move(610,150);
    btn_restart->move(565,210);
    btn_exit->move(565,270);
    connect(btn_rollback,&QPushButton::clicked,[=](){
             rollback();
    });
    connect(btn_restart,&QPushButton::clicked,[=](){
        QString message="是否重新开始棋局？";
        if(QMessageBox::Yes==QMessageBox::question(this,"再来一局",message,QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes)){
            //重新部署棋子
            clear();
            update();
        }
    });
    connect(btn_exit,&QPushButton::clicked,[=](){
        QString message="是否返回菜单？";
        if(QMessageBox::Yes==QMessageBox::question(this,"返回菜单",message,QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes)){
            this->close();
            MainMenu* mainMenu=new MainMenu();
            mainMenu->show();
        }
    });
}

void DualGame::paintEvent(QPaintEvent *){
    QPainter painter(this);
    QPixmap bg_left,chessBoard,turn;
    bg_left.load(":/img/bg_left.jpg");
    chessBoard.load(":/img/board.png");
    painter.drawPixmap(chessBoard.width(),0,this->width()-chessBoard.width(),this->height(),bg_left);
    painter.drawPixmap(0,0,chessBoard.width(),chessBoard.height(),chessBoard);
    //显示当前阵营
    if(curSide==true){
        turn.load(":/img/blackturn.png");
        painter.drawPixmap(560,45,turn.width(),turn.height(),turn);
    }else{
        turn.load(":/img/redturn.png");
        painter.drawPixmap(560,45,turn.width(),turn.height(),turn);
    }
    //画棋子
    for(int id=0;id<32;id++)
    {
        drawChessman(painter,id);
    }
}

void DualGame::mousePressEvent(QMouseEvent *event){
    int row,col;
    QPoint prsPos=event->pos();
    row=getClkPos(prsPos).clkRow;
    col=getClkPos(prsPos).clkCol;
    clkID=getID(prsPos);
    //如果场上选中的棋子不属于当前阵营
    if(chessman[selectedID]._side!=curSide)
    {
        //当选中本方棋子时，更新selectedID
        if(clkID>-1 && chessman[clkID]._side==curSide)
        {
            selectedID=clkID;
        }
    }
    //此时已经选中我方棋子
    else
    {
        if(clkID>-1)
        {
            //如果点击到本方棋子，更新selectedID
            if(chessman[clkID]._side==curSide)
            {
                selectedID=clkID;
            }
            else{
                //点击到敌方棋子，判断能否吃棋
                if(canMove(row,col))
                {
                    deadID=clkID;
                    vecDead.push_back(deadID);//添加到死亡棋子列表中
                    clkPos lastPos(chessman[selectedID]._curRow,chessman[selectedID]._curCol);
                    vecSteps.push_back(lastPos);//添加到走棋列表中
                    vecID.push_back(selectedID);
                    chessman[selectedID]._curRow=row;
                    chessman[selectedID]._curCol=col;
                    curSide=!curSide;//更换阵营
                }
            }
        }
        else
        {
            //如果点击棋盘的空位
            if(row>-1)
            {
                //判断能否走棋
                if(canMove(row,col))
                {
                    vecDead.push_back(-1);
                    clkPos lastPos(chessman[selectedID]._curRow,chessman[selectedID]._curCol);
                    vecSteps.push_back(lastPos);
                    vecID.push_back(selectedID);
                    chessman[selectedID]._curRow=row;
                    chessman[selectedID]._curCol=col;
                    curSide=!curSide;//更换阵营
                }
            }
        }
    }
    update();
}

void DualGame::drawChessman(QPainter& painter, int id){
    //如果遍历的id等于selectedID则加载选中的图片
    if(id==selectedID){
        QPoint startPt=startPos(chessman[id]._curRow,chessman[id]._curCol);
        QPixmap imgSl=chessman[id]._imgSl;
        painter.drawPixmap(startPt,imgSl);
    }else
    {
        int count=vecDead.count(id);//查看该棋子是否在死亡列表中
        if(!count){
            QPoint startPt=startPos(chessman[id]._curRow,chessman[id]._curCol);
            QPixmap img=chessman[id]._img;
            painter.drawPixmap(startPt,img);
        }
    }
}

