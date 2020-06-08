#ifndef MYTEXTEDIT_H
#define MYTEXTEDIT_H

#include <QObject>
#include <QTextEdit>

class MyTextEdit:public QTextEdit
{
    Q_OBJECT
public:
    MyTextEdit(QWidget* parent);
    void keyPressEvent(QKeyEvent* event);
signals:
    void sendMsg();
};

#endif // MYTEXTEDIT_H
