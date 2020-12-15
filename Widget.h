#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QList>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QToolButton>
#include <QMenu>
#include <QAction>
#include "MyPushButton.h"

const int BIT_NUM = 32;//操作数位数

enum BtnType//按钮类型
{
    Num,
    Op,
    Dot,
    Equal,
    Clear,
    Back
};


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:

    QList<QString> btnContentList{"MC","MR","M+","M-",
                                 "C","+","-","←",
                                 "7","8","9","×",
                                 "4","5","6","÷",
                                 "1","2","3","=",
                                 "0",".","00"};
    QList<QString> btnScContentList{"DEG","F-E",
                                    "MC","MR","M+","M-",
                                    "⊿三角学","函数",
                                    "2nd","Π","e","C","←",
                                    "x²","¹/x","|x|","exp","mod",
                                    "√x","(",")","n!","÷",
                                     "x^y","7","8","9","×",
                                     "10^x","4","5","6","-",
                                     "㏒","1","2","3","+",
                                     "㏑","0",".","00","="};
    Ui::Widget *ui;
    QMenu* menu;//切换科学计算器和普通计算器
    QToolButton* tBtn;//进入菜单按钮
    QLabel* lab;//显示计算器类型
    void initMenuLabelTBtn();

    QList<MyPushButton*> btnList;//普通计算器按钮管理
    QList<MyPushButton*> btnScList;//科学计算器按钮管理

    QLineEdit *lineEdit, *resultlineEdit;
    QString mNum1;//操作数1
    QString mNum2;//操作数2
    QString mOp;//运算符
    QString num;//寄存当前结果框显示数据
    QString memNum;//内存值

    void OnClicked(BtnType _type,QString _btn);//计算器逻辑处理
    void initKeyboard();//初始化键盘
    void initScientificKeyboard();//初始化科学计算器表盘
    void keyboard();
    void scKeyboard();
    void on_McButton_clicked();
    void on_MrButton_clicked();
    void on_MSumButton_clicked();
    void on_MSubButtoon_clicked();

protected:
    virtual void resizeEvent(QResizeEvent *event);

};
#endif // WIDGET_H
