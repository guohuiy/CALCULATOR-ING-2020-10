#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>

class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    MyPushButton(QWidget *parent = nullptr);

    //鼠标
    virtual void enterEvent(QEvent* event);//鼠标进入
    virtual void leaveEvent(QEvent *event);//鼠标离开
signals:
    void hover();
    void press();
};

#endif // MYPUSHBUTTON_H
