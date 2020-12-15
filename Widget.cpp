#include "Widget.h"
#include "ui_widget.h"
#include <QFont>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    initMenuLabelTBtn();

    lineEdit=new QLineEdit(this);
    resultlineEdit=new QLineEdit(this);

    initKeyboard();//初始化键盘
    //初始化MC和MR按键不可用
    btnList.at(0)->setDisabled(true);
    btnList.at(1)->setDisabled(true);
    //设置结果框默认值
    resultlineEdit->setDisabled(true);
    resultlineEdit->setText("0");
    resultlineEdit->setAlignment(Qt::AlignRight);
    resultlineEdit->setStyleSheet("QLineEdit { border: none }");
    lineEdit->setDisabled(true);
    lineEdit->setAlignment(Qt::AlignRight);
    lineEdit->setStyleSheet("QLineEdit { border: none }");


}

Widget::~Widget()
{
    delete ui;
}

void Widget::initMenuLabelTBtn()
{
    menu=new QMenu(this);
    QAction* act1 = menu->addAction(QIcon("://imag/aaa.png"),"标准");
    //menu->addSeparator();
    QAction* act2 = menu->addAction(QIcon("://imag/bbb.png"),"科学");
    connect(act1,&QAction::triggered,this,&Widget::keyboard);
    connect(act2,&QAction::triggered,this,&Widget::scKeyboard);
    tBtn=new QToolButton(this);
    tBtn->setText("≡");
    tBtn->setStyleSheet("border:none;");
    tBtn->setMenu(menu);
    tBtn->setPopupMode(QToolButton::InstantPopup);
    tBtn->show();
    lab=new QLabel(this);
    lab->setText("标准");
    lab->show();
}

/*
 * 计算器运算逻辑
*/
void Widget::OnClicked(BtnType _type,QString _btn)
{
    switch (_type)
    {
        case Num://点击数字按钮
        {
            //结果框值不可追加编辑
            if(!num.isEmpty())
            {
                num.clear();
                if(mOp.isEmpty()) mNum1.clear();
                else mNum2.clear();
            }

            //连续编辑操作数
            if(mOp.isEmpty())//运算符是空
            {
                if(_btn=="00" && mNum1.length() + 2 > BIT_NUM) break;
                if(mNum1.length() <= BIT_NUM)
                {
                    mNum1 += _btn;
                }
            }
            else
            {
                if(mNum2.length() <= BIT_NUM) mNum2+=_btn;
            }
            break;
        }
        case Op://点击运算符按钮
        {
            if(!num.isEmpty()) mNum1=num;//刚刚计算当前运算结果，此时点击运算符，则使用该暂存的运算结果

            //运算符连续运算
            if(!mNum1.isEmpty() && !mNum2.isEmpty())
            {
                double num1 = mNum1.toDouble();
                double num2 = mNum2.toDouble();
                double result = 0.0;
                if(mOp=="+") result=num1+num2;
                else if(mOp=="-") result=num1-num2;
                else if(mOp=="×")
                    result=num1*num2;
                else if(mOp=="÷")
                {
                    if(num2 != 0.0) result=num1/num2;
                    else
                    {
                        lineEdit->setText("除数不能为0");
                        return ;
                    }
                }

                //显示运算结果
                mNum1 = QString::number(result);
                resultlineEdit->setText(mNum1);//数字转字符串
                mNum2.clear();
                mOp.clear();
            }

            if(mNum1.isEmpty()) mNum1="0";//如果操作数1不存在，就转换为0与操作数2计算
            if(mNum1.endsWith(".")) mNum1+="0";

            mOp=_btn;
            break;
        }
        case Clear://点击清除按钮
        {
            if(!mNum1.isEmpty()) mNum1.clear();
            if(!mNum2.isEmpty()) mNum2.clear();
            if(!mOp.isEmpty()) mOp.clear();
            if(!num.isEmpty()) num.clear();
            //设置结果框为默认值0
            lineEdit->setText(Q_NULLPTR);
            resultlineEdit->setText("0");

            return;
        }
        case Dot://小数点按钮
        {
            if(!num.isEmpty()) num.clear();

            if(mOp.isEmpty())//运算符是空
            {
                //操作数一的小数点
                if(mNum1.isEmpty()) mNum1="0" + _btn;
                else if(!mNum1.isEmpty() && !mNum1.contains("."))
                {
                    mNum1 += _btn;
                }

            }
            else
            {
                //操作数二的小数点
                if(!mNum2.isEmpty() && !mNum2.contains("."))
                {
                    mNum2 += _btn;
                }
            }
            break;
        }
        case Equal: //等于
        {
            if(!num.isEmpty() && mOp.isEmpty()) mNum1=num;
            if(!num.isEmpty() && !mOp.isEmpty()) mNum2=num;

            if(mNum1.isEmpty()) return;
            else if(mOp.isEmpty())
            {
                //完成a=运算，输入一个操作数点击等号
                if(mNum1.endsWith(".")) mNum1+="0";
                if(mNum1.contains(".") && mNum1.toDouble()==0) mNum1="0.0";
                else if(mNum1.toDouble()==0) mNum1="0";
                lineEdit->setText(mNum1+"=");
                resultlineEdit->setText(mNum1);
                num=mNum1;//暂存当前运算结果
                if(!mNum1.isEmpty()) mNum1.clear();

                return;
            }
            else if(mNum2.isEmpty())
            {
                //计算a×类似的运算转化为a×a
                mNum2.append(mNum1);
            }

            if(mNum2.endsWith(".")) mNum2+="0";

            //完成两个操作数的运算
            double num1 = mNum1.toDouble();
            double num2 = mNum2.toDouble();
            double result = 0.0;
            if(mOp=="+")result=num1+num2;
            else if(mOp=="-")result=num1-num2;
            else if(mOp=="×")result=num1*num2;
            else if(mOp=="÷")
            {
                if(num2 != 0.0) result=num1/num2;
                else
                {
                    lineEdit->setText("除数不能为0");
                    return ;
                }
            }

            //显示运算结果
            lineEdit->setText(mNum1+mOp+mNum2+"=");
            mNum1 = QString::number(result);
            resultlineEdit->setText(mNum1);//数字转字符串

            num=mNum1;//暂存当前运算结果
            if(!mNum1.isEmpty()) mNum1.clear();
            if(!mNum2.isEmpty()) mNum2.clear();
            if(!mOp.isEmpty()) mOp.clear();

            return;
        }
        case Back:
        {
           if(!mNum2.isEmpty() && !mOp.isEmpty())//!mNum1.isEmpty() &&
           {
                mNum2.chop(1);//尾部删除字符串，指定个数
                if(mNum2.isEmpty())//所有数据都被回退掉显示默认值0
                {
                    resultlineEdit->setText("0");
                    return ;
                }
                break;
           }

           if(!mNum1.isEmpty() && mOp.isEmpty())
           {
               mNum1.chop(1);
               if(mNum1.isEmpty())//所有数据都被回退掉显示默认值0
               {
                   resultlineEdit->setText("0");
                   return ;
               }
               break;
           }

           if(!num.isEmpty()) lineEdit->setText(Q_NULLPTR);
           return;
        }
    }

    //显示运算式
    if(mOp.isEmpty()) resultlineEdit->setText(mNum1);
    else if(!mNum2.isEmpty())
    {
        resultlineEdit->setText(mNum2);
    }
    else
    {
        resultlineEdit->setText(mNum1);
        lineEdit->setText(mNum1+mOp+mNum2);
    }
}

//布局普通计算器表盘
void Widget::initKeyboard()
{
    float width=this->width();
    float height=this->height();

    tBtn->move(3,3);
    tBtn->resize((width-3-3)/10,(height*2/5-3-1-1)/2/2);
    tBtn->setFont(QFont("宋体",tBtn->height()/5,QFont::Bold));
    lab->move(3+3+(width-3-3)/10,3);
    lab->resize((width-3-3)/10,(height*2/5-3-1-1)/2/2);
    lab->setFont(QFont("宋体",lab->height()/5,QFont::Bold));

    lineEdit->move(3,3+(height*2/5-3-1-1)/2/2);
    lineEdit->resize(width-3-3,(height*2/5-3-1-1)/2/2);
    lineEdit->setFont(QFont("宋体",lineEdit->height()/3,QFont::Bold));

    resultlineEdit->resize(width-3-3,(height*2/5-3-1-1)/2);
    resultlineEdit->setFont(QFont("宋体",resultlineEdit->height()/3,QFont::Bold));
    resultlineEdit->move(3,3+1+(height*2/5-3-1-1)/2);

    float ax=(width-3-1-1-1-3)/4;//按钮宽度
    float ay=(height*3/5-3-1-1-1-1-1-1)/6;//按钮高度

    float x=0.0, y=height*2/5;//键盘布局起始点
    int len=btnContentList.length();
    for(int i=0;i<len;++i)
    {
        if(btnList.length()<len)
        {
            MyPushButton * btn=new MyPushButton(this);
            if(i<4)
            {
                btn->setFlat(true);//去掉边框;//btn->setStyleSheet("border:none;");
                connect(btn,&MyPushButton::hover,btn,[btn](){
                    btn->setFlat(false);//加边框;
                    btn->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);}\
                                        QPushButton:hover{background-color: rgb(200, 200, 200);}\
                                        QPushButton:pressed{ background-color: rgb(150, 150, 150); }");
                });
                connect(btn,&MyPushButton::press,btn,[btn](){btn->setFlat(true);});//去边框;
            }
            else if(i<=7 || (i%4==3 && 4!=i/4)) btn->setStyleSheet("QPushButton{background-color: rgb(230, 230, 230);}\
                                                                             QPushButton:hover{background-color: rgb(200, 200, 200);}\
                                                                             QPushButton:pressed{ background-color: rgb(150, 150, 150); }");
            else if(4==i/4 && 3==i%4) btn->setStyleSheet("QPushButton{background-color: rgb(100, 200, 100); }\
                                QPushButton:hover{background-color: rgb(83, 176, 76);}\
                                QPushButton:pressed{ background-color: rgb(56, 131, 54); }");
            else btn->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);}\
                                     QPushButton:hover{background-color: rgb(200, 200, 200);}\
                                     QPushButton:pressed{ background-color: rgb(150, 150, 150); }");
            btnList.append(btn);

             connect(btn,&QPushButton::clicked,this,[this,btn](){
                 if(btn->text()=="MC") on_McButton_clicked();
                 else if(btn->text()=="MR") on_MrButton_clicked();
                 else if(btn->text()=="M+") on_MSumButton_clicked();
                 else if(btn->text()=="M-") on_MSubButtoon_clicked();
                 else if(btn->text()=="=") OnClicked(Equal,btn->text());
                 else if(btn->text()=="←") OnClicked(Back,btn->text());
                 else if(btn->text()=="C") OnClicked(Clear,btn->text());
                 else if(btn->text()==".") OnClicked(Dot,btn->text());
                 else if(btn->text()=="+" || btn->text()=="-" || btn->text()=="×" || btn->text()=="÷") OnClicked(Op,btn->text());
                 else OnClicked(Num,btn->text());
             });
        }
        if(4==i/4 && 3==i%4) btnList.at(i)->resize(ax,2*ay+1);
        else if(i<4) btnList.at(i)->resize(100,ay*0.9);//新加设计
        else btnList.at(i)->resize(ax,ay);

        if(0==i%4) //第一列
        {
            if(i<4) btnList.at(i)->move(x+3,y+0.1*ay+1);//新加设计
            else btnList.at(i)->move(x+3,y+1*(i/4+1)+ay*(i/4));
        }
        else //后三列
        {
            if(i<4) btnList.at(i)->move(x+3+(1+100)*(i%4),y+0.1*ay+1);//新加设计
            else btnList.at(i)->move(x+3+(1+ax)*(i%4),y+1*(i/4+1)+ay*(i/4));
        }
        btnList.at(i)->setFont(QFont("宋体",(this->height()*3/5-3-1-1-1-1-1-1)/30));
        btnList.at(i)->setText(btnContentList.at(i));
        btnList.at(i)->show();


    }
}

//布局科学计算器表盘
void Widget::initScientificKeyboard()
{
    float width=this->width();
    float height=this->height();

    tBtn->move(3,3);
    tBtn->resize((width-3-3)/10,(height*2/5-3-1-1)/2/2);
    tBtn->setFont(QFont("宋体",tBtn->height()/5,QFont::Bold));
    lab->move(3+3+(width-3-3)/10,3);
    lab->resize((width-3-3)/10,(height*2/5-3-1-1)/2/2);
    lab->setFont(QFont("宋体",lab->height()/5,QFont::Bold));

    lineEdit->move(3,3+(height*2/5-3-1-1)/2/2);
    lineEdit->resize(width-3-3,(height*2/5-3-1-1)/2/2);
    lineEdit->setFont(QFont("宋体",lineEdit->height()/3,QFont::Bold));

    resultlineEdit->resize(width-3-3,(height*2/5-3-1-1)/2);
    resultlineEdit->setFont(QFont("宋体",resultlineEdit->height()/3,QFont::Bold));
    resultlineEdit->move(3,3+1+(height*2/5-3-1-1)/2);

    float ax=(width-3-1-1-1-3)/5;//按钮宽度
    float ay=(height*3/5-3-1-1-1-1-1-1-1-1-1)/10;//按钮高度

    QFont qf=QFont("宋体",(this->height()*3/5-3-1-1-1-1-1-1)/30);//设置字体大小
    float x=0.0, y=height*2/5;//键盘布局起始点
    int len=btnScContentList.length();
    for(int i=0;i<len;++i)
    {
        if(btnScList.length()<len)
        {
            MyPushButton * btn=new MyPushButton(this);
            if(i<8)
            {
                btn->setFlat(true);//去掉边框;//btn->setStyleSheet("border:none;");
                connect(btn,&MyPushButton::hover,btn,[btn](){
                    btn->setFlat(false);//加边框;
                    btn->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);}\
                                        QPushButton:hover{background-color: rgb(200, 200, 200);}\
                                        QPushButton:pressed{ background-color: rgb(150, 150, 150); }");
                });
                connect(btn,&MyPushButton::press,btn,[btn](){btn->setFlat(true);});//去边框;
            }
            else if(42==i) btn->setStyleSheet("QPushButton{background-color: rgb(100, 200, 100); }\
                                QPushButton:hover{background-color: rgb(83, 176, 76);}\
                                QPushButton:pressed{ background-color: rgb(56, 131, 54); }");
            else if((i-8)/5>2 && (i-8)%5>0 && (i-8)%5<4) btn->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);}\
                                                                            QPushButton:hover{background-color: rgb(200, 200, 200);}\
                                                                            QPushButton:pressed{ background-color: rgb(150, 150, 150); }");
            else if(42!=i) btn->setStyleSheet("QPushButton{background-color: rgb(230, 230, 230);}\
                                     QPushButton:hover{background-color: rgb(200, 200, 200);}\
                                     QPushButton:pressed{ background-color: rgb(150, 150, 150); }");
            btnScList.append(btn);
            if(6==i)//设计函数按钮
            {
                QMenu *menu=new QMenu(this);
                QAction* act1 = menu->addAction("sin");
                QAction* act2 = menu->addAction("cos");
                QAction* act3 = menu->addAction("tan");
                QAction* act4 = menu->addAction("sec");
                QAction* act5 = menu->addAction("csc");
                QAction* act6 = menu->addAction("cot");
                btn->setMenu(menu);
            }
            if(7==i)
            {
                QMenu *menu=new QMenu(this);
                QAction* act1 = menu->addAction("|x|");
                QAction* act2 = menu->addAction("⌈x⌉");
                QAction* act3 = menu->addAction("⌊x⌋");
                QAction* act4 = menu->addAction("rand");
                btn->setMenu(menu);
            }
        }
        if(42==i) btnScList.at(i)->resize(ax,ay);
        else if(i<6) btnScList.at(i)->resize(ax*0.8,ay);//新加设计
        else if(i<8) btnScList.at(i)->resize(160,ay);//新加设计
        else if(42!=i) btnScList.at(i)->resize(ax,ay);

        if(i<2)
        {
            if(0==i) btnScList.at(i)->move(x+3,y+1);
            else btnScList.at(i)->move(x+3+ax*0.8+1,y+1);
        }
        else if(i<6)
        {
            if(0==(i-2)%4) btnScList.at(i)->move(x+3,y+(1+ay)+1);
            else btnScList.at(i)->move(x+3+(1+ax*0.8)*((i-2)%4),y+(1+ay)+1);
        }
        else if(i<8)
        {
            if(6==i) btnScList.at(i)->move(x+3,y+(1+ay)*2+1);
            else btnScList.at(i)->move(x+3+160+1,y+(1+ay)*2+1);
        }
        else
        {
            if(0==(i-8)%5) //第一列
            {
                btnScList.at(i)->move(x+3,y+1+(1+ay)*(3+(i-8)/5));
            }
            else //后四列
            {
                btnScList.at(i)->move(x+3+(1+ax)*((i-8)%5),y+1+(1+ay)*(3+(i-8)/5));
            }
        }

        btnScList.at(i)->setText(btnScContentList.at(i));
        if((i-8)/5>2 && (i-8)%5>0 && (i-8)%5<4) qf.setBold(true);
        else qf.setBold(false);
        btnScList.at(i)->setFont(qf);
        btnScList.at(i)->show();

    }
}

void Widget::keyboard()
{
    lab->setText("标准");
    foreach(MyPushButton* btn,btnScList) delete btn;
    btnScList.clear();
    initKeyboard();
}

void Widget::scKeyboard()
{
    lab->setText("科学");
    foreach(MyPushButton* btn,btnList) delete btn;
    btnList.clear();
    initScientificKeyboard();
}

//窗口大小改变事件
void Widget::resizeEvent(QResizeEvent *event)
{
    if(event->oldSize()!=event->size())
    {
        if(lab->text()=="科学")
        {
            initScientificKeyboard();
        }
        else
        {
            initKeyboard();
        }
    }

}

/*
 * 清空寄存值
*/
void Widget::on_McButton_clicked()
{
    if(!memNum.isEmpty())
    {
        memNum.clear();
    }

    //设置MC和MR按键不可用
    btnList.at(0)->setDisabled(true);
    btnList.at(1)->setDisabled(true);
}

/*
 * 重新调用寄存值，显示在屏幕上
*/
void Widget::on_MrButton_clicked()
{
    resultlineEdit->setText(memNum);

    num=memNum;
}

//内存已有数据，激活MC和MR按钮
void Widget::on_MSumButton_clicked()
{
    if(memNum.isEmpty())
    {
        memNum=resultlineEdit->text();
        btnList.at(0)->setDisabled(false);
        btnList.at(1)->setDisabled(false);
    }
    else
    {
        memNum = QString::number(memNum.toDouble() + resultlineEdit->text().toDouble());
    }
    num=memNum;
}

//内存已有数据，激活MC和MR按钮
void Widget::on_MSubButtoon_clicked()
{
    if(memNum.isEmpty())
    {
        memNum="-"+resultlineEdit->text();
        btnList.at(0)->setDisabled(false);
        btnList.at(1)->setDisabled(false);
    }
    else
    {
        memNum = QString::number(memNum.toDouble() - resultlineEdit->text().toDouble());
    }
    num=memNum;
}
