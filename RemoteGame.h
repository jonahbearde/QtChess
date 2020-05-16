#ifndef REMOTEGAME_H
#define REMOTEGAME_H

#include <QObject>
#include "ChessBoard.h"

class RemoteGame:public ChessBoard
{
public:
    RemoteGame(QString titleName,bool isServer);
};

#endif // REMOTEGAME_H
