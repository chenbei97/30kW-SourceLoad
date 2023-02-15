#include "gradientparamselector.h"

GradientParamSelector::GradientParamSelector(QWidget *parent)
    : QWidget(parent),mButton(new ColorButton),mLabel(new QLabel(""))
{
    init();
}

GradientParamSelector::GradientParamSelector(const QString&text,QWidget *parent)
    :QWidget(parent),mButton(new ColorButton),mLabel(new QLabel(text))
{
    init();
}

GradientParamSelector::GradientParamSelector(const QColor& color,QWidget *parent)
    :QWidget(parent),mButton(new ColorButton(color)),mLabel(new QLabel(""))
{
    init();
}

GradientParamSelector::GradientParamSelector(const QString&text,const QColor& color,QWidget *parent)
    :QWidget(parent),mButton(new ColorButton(color)),mLabel(new QLabel(text))
{
    init();
}

void GradientParamSelector::init()
{
    setFixedSize(80,100);
    QVBoxLayout * lay = new QVBoxLayout;
    lay->addWidget(mButton,0,Qt::AlignCenter);
    lay->addSpacing(10);
    lay->addWidget(mLabel,0,Qt::AlignCenter);
    setLayout(lay);
}

void GradientParamSelector::setGrayStyle()
{
    mButton->setGrayStyle();
}

void GradientParamSelector::setDefaultStyle()
{
    mButton->setDefaultStyle();
}

QSize GradientParamSelector::sizeHint() const
{
    return QSize(80,100);
}

QSize GradientParamSelector::minimumSizeHint() const
{
    return QSize(80,100);
}

void GradientParamSelector::setColor(const QColor& color)
{
    mButton->setColor(color);
}

QColor GradientParamSelector::color() const
{
    return mButton->color();
}

void GradientParamSelector::setText(const QString &text)
{
    mLabel->setText(text);
}

QString GradientParamSelector::text() const
{
    return mLabel->text();
}
