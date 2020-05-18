#include "GameLobby.h"

GameLobby::GameLobby(QString username){
    setWindowTitle(username+" 的游戏大厅");
    setFixedSize(400,300);
    this->username=username;
}
