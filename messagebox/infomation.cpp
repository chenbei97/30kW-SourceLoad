#include "infomation.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

Infomation::Infomation()
{
    setDialogTitle(tr("提示对话框"));
    setDialogIcon(QPixmap(":/images/infomationIcon.png"));
    setDialogFixedSize(800,280);
    setText(tr("这是一个提示对话框!"));
    setPixmap(QPixmap(":/images/infomationIcon.png"));
    setPixmapBoxStyleSheet("border:2px solid #1afa29;border-radius:10px;");
    setTextBoxStyleSheet("border: 2px solid #1afa29;");
    setTextEditStyleSheet("color:#1afa29;font-size:24px;font-weight: bold;"
                          "font-family:Times New Roman;text-align: center;");
    setTextWrapIndent(2,21);
    setTextColor(Qt::green);
    removeButton(1); // cancel's id =1
    MessageButton* okbutton = button(0);
    okbutton->button()->setStyleSheet("color:#1afa29;border:1px solid #1afa29;font-size:24px;"
                                      "font-family:Times New Roman;min-width: 120px;min-height: 40px;");
    // 后4种用法满足语法
    //Messagebox::setSlotClassFunction(&Messagebox::accept,0); //私有不合法
    //Messagebox::setSlotFunction(func,0); // 或者使用父类继承而来的函数合法,但不能用于类成员函数
    //Infomation::setSlotClassFunction(&Messagebox::accept,0);// 支持继承的成员函数
    Infomation::setSlotClassFunction(&Infomation::accept,0); // 支持继承的成员函数
    //Infomation::setSlotClassFunction(&Infomation::onOkBtnSlot,0); // 支持自己的成员函数
}

void Infomation::test()
{
    button(0)->setButtonName("好的");// 测试改名是否可以
    MessageButton * button0 = new MessageButton;button0->setId(0);addButton(button0);//测试编号重复的情况
    MessageButton * button1 = new MessageButton;button1->setId(1);button1->setButtonName("A");addButton(button1);
    addButton(button1);//测试按钮重复的情况
    MessageButton * button2 = new MessageButton;button2->setId(2);button2->setButtonName("B");addButton(button2);
    MessageButton * button3 = new MessageButton;button3->setId(3);button3->setButtonName("C");addButton(button3);
    MessageButton * button4 = new MessageButton;button4->setId(4);button4->setButtonName("D");addButton(button4);
    //qDebug()<<"id=5 button is exist? "<<!(button(5)==Q_NULLPTR); // 测试id=5是否存在,不存在j会返回空指针
    //qDebug()<<"id=2 button is exist? "<<!(button(2)==Q_NULLPTR) <<" name is "<<button(2)->buttonName();
    connect(button4->button(),&QPushButton::clicked,this,[=]{
        Tip*tip=new Tip;tip->setTip("谁创建了按钮谁就有义务绑定好对应的槽函数!");
        tip->exec();
        delete tip;
    });
    connect(button3->button(),&QPushButton::clicked,this,[=]{
        MessageButton * b;
        QString text = "当前已有的按钮id列表为: ";
        foreach(b,buttons())
        {
            text = text + QString::number(b->id()) + ",";
        }
        text.chop(1);
        Tip*tip=new Tip;tip->setTip(text);
        tip->exec();
        delete tip;
    });
}

void Infomation::setSlotClassFunction(ButtonSlotClassFunc p, int id)
{
    MessageButton * b = button(id);
    if (b == Q_NULLPTR)  return;
    if (p == 0) return;
    // 如果有自定义的按钮(id!=0,1),有义务断开这个按钮和之前槽函数的连接
    // 否则这个自定义按钮不仅连接之前的槽函数,还会连接新的p函数
    connect(b,&MessageButton::clicked,this,p);
}
