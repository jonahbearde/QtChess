#ifndef GAMELOBBY_H
#define GAMELOBBY_H

#include "MyTextEdit.h"
#include <QObject>
#include <QWidget>
#include <QTcpSocket>
#include <QMap>
#include <QListWidgetItem>

class GameLobby : public QWidget
{
    Q_OBJECT
public:
    GameLobby(QString username);
    void msgHandlerOut(QString);
    void msgHandlerIn(QString);
    QWidget* createWidget(QString,QString);
    void closeEvent(QCloseEvent *event);
    void paintEvent(QPaintEvent *event);
    QTcpSocket* clientSocket;
    QListWidget* listWidget;
    MyTextEdit *input,*output;
    QString username;
    int status=0;
    QMap<QString,QListWidgetItem*> items;
signals:
    void closeLobby();
    void invite(QString);
    void cancel(QString);
    void accept(QString,bool);
    void called(QString);
    void recvMsg(QString,QString);
    void playerInGame(QString,QString);
    void playerExit(QString,QString);
    void msgInGame(QByteArray);
private slots:
    void slotConnected();
    void slotRecvData();
    void slotDisconnected();
    void slotInvite(QString);
    void slotCancel(QString);
    void slotAccept(QString,bool);
    void slotCalled(QString);
    void slotRecvMsg(QString,QString);
    void slotPlayerInGame(QString,QString);
    void slotPlayerExit(QString,QString);
};

#endif // GAMELOBBY_H
