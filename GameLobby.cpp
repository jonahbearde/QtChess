#include "GameLobby.h"
#include "MyTextEdit.h"
#include "RemoteGame.h"
#include <QIcon>
#include <QStringList>
#include <QTcpSocket>
#include <QPixmap>
#include <QPainter>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QListWidget>
#include <QListWidgetItem>

GameLobby::GameLobby(QString username){
    setWindowIcon(QIcon(":/img/icon.png"));
    this->setFixedSize(600,400);
    this->username=username;
    this->setWindowTitle(username+" 的游戏大厅");
    clientSocket=new QTcpSocket(this);
    clientSocket->connectToHost("192.168.1.3",9696);
    connect(clientSocket,&QTcpSocket::connected,this,&GameLobby::slotConnected);
    connect(clientSocket,&QTcpSocket::readyRead,this,&GameLobby::slotRecvData);
    connect(this,&GameLobby::invite,this,&GameLobby::slotInvite);
    connect(this,&GameLobby::cancel,this,&GameLobby::slotCancel);
    connect(this,&GameLobby::called,this,&GameLobby::slotCalled);
    connect(this,&GameLobby::accept,this,&GameLobby::slotAccept);
    connect(this,&GameLobby::recvMsg,this,&GameLobby::slotRecvMsg);
    connect(this,&GameLobby::playerInGame,this,&GameLobby::slotPlayerInGame);
    connect(this,&GameLobby::playerExit,this,&GameLobby::slotPlayerExit);



    listWidget=new QListWidget(this);
    listWidget->setFixedSize(400,400);
    listWidget->setStyleSheet("QListWidget::item{background-color: #E8E8E8}"
                              "QListWidget::item::hover{background-color: #D3D3D3}");

    output=new MyTextEdit(this);
    output->setFixedSize(180,200);
    output->setReadOnly(true);
    output->move(410,40);

    input=new MyTextEdit(this);
    input->setFixedSize(180,105);
    input->move(410,260);
    connect(input,&MyTextEdit::sendMsg,[=](){
        QString msg=input->toPlainText();
        output->append(QString("<font size=3 color=gray>%1:</font>").arg(username));
        output->append(QString("<font size=3>%1</font>").arg(msg)) ;
        input->clear();
        QByteArray buf;
        buf.append('a').append('6');
        buf.append(msg);
        clientSocket->write(buf);
    });

    QPushButton* btn_exit=new QPushButton(this);
    btn_exit->setStyleSheet("QPushButton{border-image: url(:/img/leave.gif)}"
                            "QPushButton::hover{border-image: url(:/img/leave_hover.gif)}"
                            "QPushButton::pressed{border-image: url(:/img/leave_pressed.gif)}");
    btn_exit->setFixedSize(100,21);
    btn_exit->move(407,370);

    connect(btn_exit,&QPushButton::clicked,[=](){
        this->close();
    });

    QPushButton* btn_send=new QPushButton(this);
    btn_send->setStyleSheet("QPushButton{border-image: url(:/img/send.gif)}"
                            "QPushButton::hover{border-image: url(:/img/send_hover.gif)}"
                            "QPushButton::pressed{border-image: url(:/img/send_pressed.gif)}");
    btn_send->setFixedSize(50,21);
    btn_send->move(545,370);
    connect(btn_send,&QPushButton::clicked,[=](){
        QString msg=input->toPlainText();
        output->append(QString("<font size=3 color=gray>%1:</font>").arg(username));
        output->append(QString("<font size=3>%1</font>").arg(msg)) ;
        input->clear();
        QByteArray buf;
        buf.append('a').append('6');
        buf.append(msg);
        clientSocket->write(buf);
    });
}

QWidget* GameLobby::createWidget(QString username,QString info){
    QWidget* widget=new QWidget(this);
    QPixmap pix;
    pix.load(":/img/user.png");
    QLabel* userIcon=new QLabel(widget);
    userIcon->setPixmap(pix);
    userIcon->setFixedSize(20,20);

    QLabel* userLabel=new QLabel(widget);
    userLabel->setFixedSize(100,20);
    userLabel->setFont(QFont("Microsoft Yahei",11,75));
    userLabel->setText(username);

    QLabel* infoLabel=new QLabel(widget);
    infoLabel->setFixedSize(60,20);
    infoLabel->setFont(QFont("Microsoft Yahei",11,50));
    infoLabel->setText(info);

    QPushButton* btn1=new QPushButton(widget);
    QPushButton* btn2=new QPushButton(widget);
    if(info=="邀请中"){
        btn1->setStyleSheet("QPushButton{border-image: url(:/img/cancel.gif)}"
                                  "QPushButton::hover{border-image: url(:/img/cancel_hover.gif)}"
                                  "QPushButton::pressed{border-image: url(:/img/cancel_pressed.gif)}");

        btn1->setFixedSize(50,21);
        btn2->setStyleSheet("QPushButton{border-image: url(:/img/blank.gif)}"
                                  "QPushButton::hover{border-image: url(:/img/blank.gif)}"
                                  "QPushButton::pressed{border-image: url(:/img/blank.gif)}");

        btn2->setFixedSize(50,21);
        //发送取消邀请信号
        connect(btn1,&QPushButton::clicked,[=](){
            emit cancel(username);
            QByteArray buf;
            buf.append('a').append('4');
            buf.append(username);
            clientSocket->write(buf);
        });
    }else if(info=="请求对战"){
        btn1->setStyleSheet("QPushButton{border-image: url(:/img/accept.gif)}"
                                  "QPushButton::hover{border-image: url(:/img/accept_hover.gif)}"
                                  "QPushButton::pressed{border-image: url(:/img/accept_pressed.gif)}");

        btn1->setFixedSize(50,21);

        btn2->setStyleSheet("QPushButton{border-image: url(:/img/refuse.gif)}"
                                  "QPushButton::hover{border-image: url(:/img/refuse_hover.gif)}"
                                  "QPushButton::pressed{border-image: url(:/img/refuse_pressed.gif)}");
        btn2->setFixedSize(50,21);

        connect(btn1,&QPushButton::clicked,[=](){
            //接受邀请
            emit accept(username,true);
            QByteArray buf;
            buf.append('a').append('7');
            buf.append(username);
            clientSocket->write(buf);
        });
        connect(btn2,&QPushButton::clicked,[=](){
            //拒绝邀请
            emit cancel(username);
            QByteArray buf;
            buf.append('a').append('5');
            buf.append(username);
            clientSocket->write(buf);
        });

    }else if(info=="空闲中"){
        btn1->setStyleSheet("QPushButton{border-image: url(:/img/invite.gif)}"
                                  "QPushButton::hover{border-image: url(:/img/invite_hover.gif)}"
                                  "QPushButton::pressed{border-image: url(:/img/invite_pressed.gif)}");

        btn1->setFixedSize(50,21);
        connect(btn1,&QPushButton::clicked,[=](){
            emit invite(username);
            QByteArray buf;
            buf.append('a').append('3');
            buf.append(username);
            clientSocket->write(buf);
        });
        btn2->setStyleSheet("QPushButton{border-image: url(:/img/blank.gif)}"
                                  "QPushButton::hover{border-image: url(:/img/blank.gif)}"
                                  "QPushButton::pressed{border-image: url(:/img/blank.gif)}");
        btn2->setFixedSize(50,21);
    }else if(info=="游戏中"){
        btn1->setStyleSheet("QPushButton{border-image: url(:/img/blank.gif)}"
                                  "QPushButton::hover{border-image: url(:/img/blank.gif)}"
                                  "QPushButton::pressed{border-image: url(:/img/blank.gif)}");
        btn2->setStyleSheet("QPushButton{border-image: url(:/img/blank.gif)}"
                                  "QPushButton::hover{border-image: url(:/img/blank.gif)}"
                                  "QPushButton::pressed{border-image: url(:/img/blank.gif)}");
        btn1->setFixedSize(50,21);
        btn2->setFixedSize(50,21);
    }

    QHBoxLayout* hLayout=new QHBoxLayout(widget);
    hLayout->addWidget(userIcon);
    hLayout->addWidget(userLabel);
    hLayout->addWidget(infoLabel);
    hLayout->addWidget(btn1);
    hLayout->addWidget(btn2);
    hLayout->setContentsMargins(0,0,20,0);
    widget->setLayout(hLayout);
    return widget;
}

void GameLobby::closeEvent(QCloseEvent *event){
    emit closeLobby();
    clientSocket->disconnectFromHost();
}

void GameLobby::paintEvent(QPaintEvent *event){
    QPixmap pix;
    pix.load(":/img/chatroom.gif");

    QPainter painter(this);
    painter.drawPixmap(455,5,pix.width(),pix.height(),pix);
    painter.drawLine(QPoint(405,30),QPoint(595,30));
    painter.drawLine(QPoint(405,250),QPoint(595,250));
}

void GameLobby::msgHandlerOut(QString msg){
    QChar header=msg.at(1);
    if(header=='1'){
        QString s=msg.mid(2);
        QStringList list=s.split("#");
        for(int i=0;i<list.size();i++){
            QString gamer=list.at(i);
            if(!gamer.isEmpty()){
                QWidget* widget=createWidget(gamer,"空闲中");
                QListWidgetItem* item=new QListWidgetItem(listWidget);
                QSize size=item->sizeHint();
                item->setSizeHint(QSize(size.width(),40));
                listWidget->addItem(item);
                listWidget->setItemWidget(item,widget);
                items.insert(gamer,item);
            }
        }
    }else if(header=='2'){
        //有玩家离开
        QString gamer=msg.mid(2);
        for(int i=0;i<listWidget->count();i++){
            QListWidgetItem* item1=items.find(gamer).value();
            QWidget* widget=listWidget->itemWidget(item1);
            QListWidgetItem* item2=listWidget->item(i);
            if(item1==item2){
                items.remove(gamer);
                listWidget->takeItem(i);
                delete item2;
                widget->deleteLater();
                return;
            }
        }
    }else if(header=='3'){
        QString caller=msg.mid(2);
        emit called(caller);
    }else if(header=='4' || header=='5'){
        QString caller=msg.mid(2);
        emit cancel(caller);
    }else if(header=='6'){
        QString s=msg.mid(2);
        QString sender=s.section('#',0,0);
        QString content=s.section('#',1,1);
        emit recvMsg(sender,content);
    }else if(header=='7'){
        //收到对方接受邀请的信号
        QString opponent=msg.mid(2);
        emit accept(opponent,false);
    }else if(header=='8'){
        QString s=msg.mid(2);
        QString player1=s.section('#',0,0);
        QString player2=s.section('#',1,1);
        emit playerInGame(player1,player2);
    }else if(header=='9'){
        QString s=msg.mid(2);
        QString player1=s.section('#',0,0);
        QString player2=s.section('#',1,1);
        emit playerExit(player1,player2);
    }
}

void GameLobby::msgHandlerIn(QString msg){

}

void GameLobby::slotConnected(){
    QByteArray buf;
    buf.append('a').append('1');
    buf.append(username);
    clientSocket->write(buf);
}

void GameLobby::slotRecvData(){
    QByteArray buf=clientSocket->readAll();
    QString msg=(QString)buf;
    QChar header=msg.at(0);
    if(header=='a'){
        msgHandlerOut(msg);
    }else if(header=='b'){
        emit msgInGame(buf);
    }
}

void GameLobby::slotDisconnected(){

}

void GameLobby::slotInvite(QString caller){
    QWidget* widget=createWidget(caller,"邀请中");
    QListWidgetItem* item=items.find(caller).value();
    QWidget* oldWidget=listWidget->itemWidget(item);
    listWidget->setItemWidget(item,widget);
    oldWidget->deleteLater();
}

void GameLobby::slotCancel(QString caller){
    QWidget* widget=createWidget(caller,"空闲中");
    QListWidgetItem* item=items.find(caller).value();
    QWidget* oldWidget=listWidget->itemWidget(item);
    listWidget->setItemWidget(item,widget);
    oldWidget->deleteLater();
}

void GameLobby::slotCalled(QString caller){
    QWidget* widget=createWidget(caller,"请求对战");
    QListWidgetItem* item=items.find(caller).value();
    QWidget* oldWidget=listWidget->itemWidget(item);
    listWidget->setItemWidget(item,widget);
    oldWidget->deleteLater();
}

void GameLobby::slotAccept(QString opponent,bool myTurn){
//    QString title;
//    if(myTurn==false){
//        title=username+"(红)"+" "+opponent+"(黑)";
//    }else{
//        title=username+"(黑)"+" "+opponent+"(红)";
//    }
    //加载游戏界面
    RemoteGame* remote=new RemoteGame(this,username,opponent,clientSocket,"中国象棋",myTurn);
    //将游戏内的消息转发到RemoteGame处理
    connect(this,&GameLobby::msgInGame,remote,&RemoteGame::slotMsgHandler);
    this->hide();
    remote->show();

    connect(remote,&RemoteGame::closeRemote,[=](){
        //更换item
        QListWidgetItem* item=items.find(opponent).value();
        QWidget* widget=createWidget(opponent,"空闲中");
        QWidget* oldWidget=listWidget->itemWidget(item);
        listWidget->setItemWidget(item,widget);
        oldWidget->deleteLater();
    });
}

void GameLobby::slotRecvMsg(QString sender,QString content){
    output->append(QString("<font size=3 color=gray>%1:</font>").arg(sender));
    output->append(QString("<font size=3>%1</font>").arg(content)) ;
}

void GameLobby::slotPlayerInGame(QString player1,QString player2){
    QWidget* widget1=createWidget(player1,"游戏中");
    QWidget* widget2=createWidget(player2,"游戏中");
    QListWidgetItem* item1=items.find(player1).value();
    QListWidgetItem* item2=items.find(player2).value();
    listWidget->setItemWidget(item1,widget1);
    listWidget->setItemWidget(item2,widget2);
}

void GameLobby::slotPlayerExit(QString player1,QString player2){
    QListWidgetItem* item1=items.find(player1).value();
    QWidget* widget1=createWidget(player1,"空闲中");
    QWidget* oldWidget1=listWidget->itemWidget(item1);
    listWidget->setItemWidget(item1,widget1);
    oldWidget1->deleteLater();

    QListWidgetItem* item2=items.find(player2).value();
    QWidget* widget2=createWidget(player2,"空闲中");
    QWidget* oldWidget2=listWidget->itemWidget(item2);
    listWidget->setItemWidget(item2,widget2);
    oldWidget2->deleteLater();
}
