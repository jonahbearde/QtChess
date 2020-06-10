#ifndef REMOTEGAME_H
#define REMOTEGAME_H

#include "ChessBoard.h"
#include "GameLobby.h"
#include <QObject>
#include <QTcpSocket>
#include <QPushButton>

class RemoteGame:public ChessBoard
{
    Q_OBJECT
public:
    RemoteGame(GameLobby*,QString,QString,QTcpSocket*,QString,bool);
    GameLobby* lobby;
    QTcpSocket* clientSocket;
    QString username,opponent;
//    QPushButton *btn_rollback,*btn_restart,*btn_exit;
    bool finished=false;
    bool ifExit=false;
    void paintEvent(QPaintEvent*);
    void closeEvent(QCloseEvent*);
    void mousePressEvent(QMouseEvent* event);
    void drawChessman(QPainter&,int); //画棋子
signals:
    void closeRemote();
    void opponentExit();
public slots:
    void slotMsgHandler(QByteArray);
};

#endif // REMOTEGAME_H
