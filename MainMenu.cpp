#include "MainMenu.h"
#include "DualGame.h"
#include "GameLobby.h"
#include "ui_MainMenu.h"
#include <QPainter>
#include <QMessageBox>
#include <QPixmap>
#include <QInputDialog>
#include <QLineEdit>
#include <QTimer>
#include <QFile>

MainMenu::MainMenu(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainMenu)
{
    ui->setupUi(this);
    setFixedSize(800,600);
    setWindowTitle("中国象棋");
    //创建主菜单按钮
    QPushButton* btn_p2p=new QPushButton(this);
    QPushButton* btn_p2c=new QPushButton(this);
    QPushButton* btn_p2p_ol=new QPushButton(this);
    btn_p2p->setStyleSheet("QPushButton{width: 346px; height: 115px; border-image: url(:/img/btn_p2p.png);}"
                           "QPushButton:hover{width: 346px; height: 115px; border-image: url(:/img/btn_p2p_hover.gif);}"
                           "QPushButton:pressed{width: 346px; height: 115px; border-image: url(:/img/btn_p2p_pressed.gif);}");
    btn_p2c->setStyleSheet("QPushButton{width: 346px; height: 115px; border-image: url(:/img/btn_p2c.png);}"
                           "QPushButton:hover{width: 346px; height: 115px; border-image: url(:/img/btn_p2c_hover.gif);}"
                           "QPushButton:pressed{width: 346px; height: 115px; border-image: url(:/img/btn_p2c_pressed.gif);}");
    btn_p2p_ol->setStyleSheet("QPushButton{width: 346px; height: 115px; border-image: url(:/img/btn_p2p_ol.png);}"
                              "QPushButton:hover{width: 346px; height: 115px; border-image: url(:/img/btn_p2p_ol_hover.gif);}"
                              "QPushButton:pressed{width: 346px; height: 115px; border-image: url(:/img/btn_p2p_ol_pressed.gif);}");
    //移动按钮
    btn_p2p->move(230,this->height()*0.15);
    btn_p2c->move(230,this->height()*0.35);
    btn_p2p_ol->move(230,this->height()*0.55);

    connect(btn_p2p,&QPushButton::clicked,[=](){
            this->close();
            //双人模式场景
            DualGame* dualScene=new DualGame("双人对战");
            dualScene->show();
    });
    connect(btn_p2p_ol,&QPushButton::clicked,[=](){
            QString txtLabel="请输入昵称";
            QLineEdit::EchoMode echoMode=QLineEdit::Normal;
            bool ok=false;
            QString text=QInputDialog::getText(this,"",txtLabel,echoMode,"",&ok);
            if(ok && !text.isEmpty()){
                this->hide();
                //加载大厅
                GameLobby* lobby=new GameLobby(text);
                lobby->show();
                connect(lobby,&GameLobby::closeLobby,[=](){
                    this->show();
                });
            }
    });
}
void MainMenu::paintEvent(QPaintEvent *event)
{
    //设置背景
    QPainter painter(this);
    QPixmap bg_main;
    bg_main.load(":/img/bg_main.jpg");
    painter.drawPixmap(0,0,this->width(),this->height(),bg_main);
}
MainMenu::~MainMenu()
{
    delete ui;
}

