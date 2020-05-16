#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include "MainMenu.h"
#include <QPixmap>
#include <QPushButton>

class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit MyPushButton(QWidget *parent = nullptr);
    MyPushButton(QString,QWidget *);
    void bounceBack();// 跳跃效果函数
signals:


};

#endif // MYPUSHBUTTON_H
