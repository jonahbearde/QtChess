#include "MainMenu.h"

#include <QApplication>
#include <QProxyStyle>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainMenu w;
    w.show();
    return a.exec();
}
