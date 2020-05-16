#include "MyPushButton.h"
#include "MainMenu.h"
#include <QPixmap>
#include <QPropertyAnimation>
#include <QDebug>

MyPushButton::MyPushButton(QWidget *parent) : QPushButton(parent)
{

}
MyPushButton::MyPushButton(QString path,QWidget *parent)
{
    setParent(parent);
    QPixmap pix(path);
    //设置背景
    this->setFixedSize(pix.width(),pix.height());
    this->setStyleSheet("QPushButton{border:0px;}");
    this->setIcon(pix);
    this->setIconSize(QSize(pix.width(),pix.height()));
}
void MyPushButton::bounceBack()
{
    QPropertyAnimation* anim1=new QPropertyAnimation(this,"geometry");
    QPropertyAnimation* anim2=new QPropertyAnimation(this,"geometry");
    anim1->setDuration(200);
    anim1->setStartValue(QRect(this->x(),this->y(),this->width(),this->height()));
    anim1->setEndValue(QRect(this->x(),this->y()+5,this->width(),this->height()));
    anim1->setEasingCurve(QEasingCurve::OutBounce);
    anim2->setDuration(200);
    anim2->setStartValue(QRect(this->x(),this->y()+5,this->width(),this->height()));
    anim2->setEndValue(QRect(this->x(),this->y(),this->width(),this->height()));
    anim2->setEasingCurve(QEasingCurve::OutBounce);
    anim1->start();
    anim2->start();
}
