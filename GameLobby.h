#ifndef GAMELOBBY_H
#define GAMELOBBY_H

#include <QObject>
#include <QWidget>
#include <QTcpSocket>

class GameLobby : public QWidget
{
    Q_OBJECT
public:
    GameLobby(QString username);
    QTcpSocket* clientSocket=new QTcpSocket(this);
    QString username;
signals:

};

#endif // GAMELOBBY_H
