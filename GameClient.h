#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include <QObject>
#include <QWidget>

class GameClient : public QWidget
{
    Q_OBJECT
public:
    explicit GameClient(QWidget *parent = nullptr);

signals:

};

#endif // GAMECLIENT_H
