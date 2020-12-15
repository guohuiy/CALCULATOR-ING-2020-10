#include "MyPushButton.h"

MyPushButton::MyPushButton(QWidget *parent) : QPushButton(parent)
{

}

void MyPushButton::enterEvent(QEvent *event)
{
    emit hover();
}

void MyPushButton::leaveEvent(QEvent *event)
{
    emit press();
}
