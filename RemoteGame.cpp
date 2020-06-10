#include "RemoteGame.h"
#include <QPainter>
#include <QPushButton>
#include <QMessageBox>
#include <QMouseEvent>

RemoteGame::RemoteGame(GameLobby* lobby,QString username,QString opponent,QTcpSocket* socket,QString titleName,bool format):ChessBoard(titleName,format){
    setWindowIcon(QIcon(":/img/icon.png"));
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->lobby=lobby;
    this->username=username;
    this->opponent=opponent;
    this->clientSocket=socket;

    QPushButton* btn_rollback=new QPushButton(this);
    QPushButton* btn_restart=new QPushButton(this);
    QPushButton* btn_exit=new QPushButton(this);
    //设定按钮的样式
    btn_rollback->setStyleSheet("QPushButton{width: 105px; height: 55px; border-image: url(:/img/rollback.png);}"
                                "QPushButton:hover{width: 105px; height: 55px; border-image: url(:/img/rollback_hover.gif);}"
                                "QPushButton:pressed{width: 105px; height: 55px; border-image: url(:/img/rollback_pressed.gif);}");
    btn_restart->setStyleSheet("QPushButton{width: 188px; height: 55px; border-image: url(:/img/restart.png);}"
                               "QPushButton:hover{width: 188px; height: 55px; border-image: url(:/img/restart_hover.gif);}"
                               "QPushButton:pressed{width: 188px; height: 55px; border-image: url(:/img/restart_pressed.gif);}");
    btn_exit->setStyleSheet("QPushButton{width: 188px; height: 55px; border-image: url(:/img/exit.png);}"
                            "QPushButton:hover{width: 188px; height: 55px; border-image: url(:/img/exit_hover.gif);}"
                            "QPushButton:pressed{width: 188px; height: 55px; border-image: url(:/img/exit_pressed.gif);}");
    btn_rollback->move(610,150);
    btn_restart->move(565,210);
    btn_exit->move(565,270);
    //悔棋
    connect(btn_rollback,&QPushButton::clicked,[=](){
             rollback();
             QByteArray buf;
             buf.append('b').append(-1).append(-1).append(-1).append(-1).append(opponent);
             clientSocket->write(buf);

    });
    connect(btn_restart,&QPushButton::clicked,[=](){
        QString message="是否重新开始棋局？";
        //对话框
        if(QMessageBox::Yes==QMessageBox::question(this,"提示",message,QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes)){
            QByteArray buf;
            buf.append('b').append(-2).append(-1).append(-1).append(-1).append(opponent);
            clientSocket->write(buf);
            //重新部署棋子
            clear();
            update();
            finished=false;
        }
    });
    connect(btn_exit,&QPushButton::clicked,[=](){
        close();
    });
    connect(this,&RemoteGame::opponentExit,[=](){
       btn_rollback->setEnabled(false);
       btn_restart->setEnabled(false);
       btn_exit->setEnabled(false);
    });
}

void RemoteGame::closeEvent(QCloseEvent *event){
    qDebug()<<"调用closeEvent";
    if(QMessageBox::Yes==QMessageBox::question(this,"提示","是否返回游戏大厅？",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes)){
        emit closeRemote();
        if(!ifExit){
            QByteArray buf;
            buf.append('b').append(-3).append(-1).append(-1).append(-1).append(opponent);
            clientSocket->write(buf);
            lobby->show();
            event->accept();
        }else{
            lobby->show();
            event->accept();
        }
    }else{
        event->ignore();
    }
}

void RemoteGame::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    QPixmap bg_left,chessBoard,turn;
    bg_left.load(":/img/bg_left.jpg");
    chessBoard.load(":/img/board.png");
    //画棋盘和背景
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

void RemoteGame::mousePressEvent(QMouseEvent *event){
    if(finished) return;
    int row,col;
    QPoint prsPos=event->pos();
    row=getClkPos(prsPos).clkRow;
    col=getClkPos(prsPos).clkCol;
    clkID=getID(prsPos);
    if(curSide==format){
        if(chessman[selectedID]._side!=curSide)
        {
            //当选中本方棋子时，并且为本方阵营，更新selectedID
            if(clkID>-1 && chessman[clkID]._side==curSide)
            {
                selectedID=clkID;
                QByteArray buf;
                buf.append('b').append(selectedID).append(-1).append(-1).append(-1);
                buf.append(opponent);
                clientSocket->write(buf);
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
                    QByteArray buf;
                    //发送选中的棋子信息
                    buf.append('b').append(selectedID).append(-1).append(-1).append(-1);
                    buf.append(opponent);
                    clientSocket->write(buf);
                }
                else{
                    //点击到敌方棋子，判断能否吃棋
                    if(canMove(row,col))
                    {
                        if(format) rotate();
                        deadID=clkID;
                        vecDead.push_back(deadID);//添加到死亡棋子列表中
                        clkPos lastPos(chessman[selectedID]._curRow,chessman[selectedID]._curCol);
                        vecSteps.push_back(lastPos);//添加到走棋列表中
                        vecID.push_back(selectedID);
                        chessman[selectedID]._curRow=row;
                        chessman[selectedID]._curCol=col;
                        curSide=!curSide;//更换阵营
                        QByteArray buf;
                        buf.append('b').append(selectedID).append(9-row).append(col).append(deadID);
                        buf.append(opponent);
                        clientSocket->write(buf);
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
                        if(format) rotate();
                        vecDead.push_back(-1);
                        clkPos lastPos(chessman[selectedID]._curRow,chessman[selectedID]._curCol);
                        vecSteps.push_back(lastPos);
                        vecID.push_back(selectedID);
                        chessman[selectedID]._curRow=row;
                        chessman[selectedID]._curCol=col;
                        curSide=!curSide;//更换阵营
                        QByteArray buf;
                        buf.append('b').append(selectedID).append(9-row).append(col).append(-1);
                        buf.append(opponent);
                        clientSocket->write(buf);
                    }
                }
            }
        }
    }
    update();
}

void RemoteGame::drawChessman(QPainter& painter, int id){
    //如果遍历的id等于selectedID则加载选中的图片
    if(id==selectedID){
        QPoint startPt=startPos(chessman[id]._curRow,chessman[id]._curCol);
        QPixmap imgSl=chessman[id]._imgSl;
        painter.drawPixmap(startPt,imgSl);
    }else{
        int count=vecDead.count(id);//查看该棋子是否在死亡列表中
        if(!count){
            QPoint startPt=startPos(chessman[id]._curRow,chessman[id]._curCol);
            QPixmap img=chessman[id]._img;
            painter.drawPixmap(startPt,img);
        }
    }
}

void RemoteGame::slotMsgHandler(QByteArray buf){
    if(buf.at(1)>-1){
        if(buf.at(2)==-1){
            selectedID=buf.at(1);
            update();
        }else{
            //对手走棋
            selectedID=buf.at(1);
            vecDead.push_back(buf.at(4));
            clkPos lastPos(chessman[selectedID]._curRow,chessman[selectedID]._curCol);
            vecSteps.push_back(lastPos);
            vecID.push_back(selectedID);
            chessman[selectedID]._curRow=buf.at(2);
            chessman[selectedID]._curCol=buf.at(3);
            curSide=!curSide;
            update();
        }
    }else if(buf.at(1)==-1){
        rollback();
    }else if(buf.at(1)==-2){
        QMessageBox::information(this,"提示","对方已重新开始棋局",QMessageBox::Ok,QMessageBox::NoButton);
        clear();
        update();
        finished=false;
    }else if(buf.at(1)==-3){
        qDebug()<<"对方已退出";
        QMessageBox::information(this,"提示","对方已退出游戏",QMessageBox::Ok,QMessageBox::NoButton);
        finished=true;
        emit opponentExit();
        ifExit=true;
    }
}
