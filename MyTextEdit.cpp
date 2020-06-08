#include "MyTextEdit.h"
#include <QKeyEvent>
MyTextEdit::MyTextEdit(QWidget* parent):QTextEdit(parent)
{

}
void MyTextEdit::keyPressEvent(QKeyEvent *event){
    QTextEdit::keyPressEvent(event);
    if(event->key()==Qt::Key_Return || event->key()==Qt::Key_Enter){
        emit sendMsg();
    }
}
