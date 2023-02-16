#include "messagebutton.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

MessageButton::MessageButton()
{
     _button = new QPushButton;
     _id = 0;
     connect(_button,&QPushButton::clicked,this,&MessageButton::clicked);
}

MessageButton::MessageButton(const QString&name)
{
    _button = new QPushButton;//这里不能用MessageButton()替代构造,会异常!
    _id = 0;
    setButtonName(name);
    connect(_button,&QPushButton::clicked,this,&MessageButton::clicked);//MessageButton就当成QPushButton那样用
}

MessageButton::MessageButton(int id)
{
    _button = new QPushButton;
    _id = id;
    connect(_button,&QPushButton::clicked,this,&MessageButton::clicked);
}

MessageButton::MessageButton(const QString&name,int id)
{
    _button = new QPushButton;
    _id = id;
    setButtonName(name);
    connect(_button,&QPushButton::clicked,this,&MessageButton::clicked);
}

MessageButton::~MessageButton()
{
    if (_button != Q_NULLPTR)
    {
        delete _button; // 这步很重要,Messagebox.removeButton时这个_button还在就会显示在窗口上
        _button = Q_NULLPTR;
    }
}

void MessageButton::setButton(QPushButton * button)
{
    _button = button;
}

QPushButton* MessageButton::button() const
{
    return _button;
}


void MessageButton::setButtonName(const QString &name)
{
    _button->setText(name);
}

QString MessageButton::buttonName() const
{
    return _button->text();
}

void MessageButton::setId(int id)
{
    _id = id;
}

int MessageButton::id() const
{
    return _id;
}
