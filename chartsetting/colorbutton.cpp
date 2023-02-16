#include "colorbutton.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

ColorButton::ColorButton(QWidget*parent):QPushButton(parent),mColor(Qt::white),mSize(QSize(40,40))
{
    init();
}

ColorButton::ColorButton(const QColor& color,QWidget*parent):QPushButton(parent),mColor(color),mSize(QSize(40,40))
{

    init();
}

void ColorButton::setColor(const QColor & color)
{
    mColor = color;
}

QColor ColorButton::color() const
{
    return  mColor;
}

QSize ColorButton::sizeHint() const
{
    return QSize(40,40); // 推荐直径40
}

QSize ColorButton::minimumSizeHint() const
{
    return QSize(40,40);
}

void ColorButton::init()
{
    setFixedSize(mSize);//要固定,否则会在布局内被拉伸
    QString borderstyle = "border:2px solid black;border-radius:20px;";// 支持rgb,#代码和字符串
    QString statestyle = QString( "background-color:%1;").arg(mColor.name());
    //QString hoverstyle = "unenabled{background-color:white;}"; // 伪状态要在后边写否则会出问题,但是这里没啥用不设置
    setStyleSheet(borderstyle+statestyle);
    connect(this,&QPushButton::clicked,this,[=]{
        QColorDialog * dlg = colorDialog(mColor);
        connect(dlg,static_cast<void (QColorDialog::*) (const QColor&)>(&QColorDialog::colorSelected)
               ,this,[=](const QColor& color){
                mColor=color; // 更新按钮背景颜色
                setStyleSheet(borderstyle+QString( "background-color:%1;").arg(mColor.name()));
        });
        dlg->exec(); delete dlg;
    });
}

void ColorButton::setGrayStyle()
{
    QString style = "border:2px solid black;border-radius:20px;background-color:gray";
    setStyleSheet(style);
}

void ColorButton::setDefaultStyle()
{
    QString style = QString("border:2px solid black;border-radius:20px;background-color:%1").arg(mColor.name());
    setStyleSheet(style);
}

QColorDialog* ColorButton::colorDialog(const QColor&initColor)
{
    QColorDialog * dlg = new QColorDialog(initColor);
    dlg->setFixedSize(800,400);
    dlg->setWindowIcon(QIcon(":/images/toolbox_chart.png"));
    dlg->setOptions(QColorDialog::ShowAlphaChannel|QColorDialog::DontUseNativeDialog);
    dlg->setWindowTitle(tr("设置颜色"));
    return dlg;
}
