#include "Widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setWindowTitle("计算器");
    w.setStyleSheet("background-color: rgb(255, 255, 255);");
    w.setWindowIcon(QIcon("://imag/ccc.png"));
    //w.setWindowFlag(Qt::FramelessWindowHint,true);
    w.setMinimumWidth(400);
    w.setMinimumHeight(620);
    w.show();

    return a.exec();
}
