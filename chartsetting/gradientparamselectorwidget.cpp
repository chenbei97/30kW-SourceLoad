#include "gradientparamselectorwidget.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

GradientParamSelectorWidget::GradientParamSelectorWidget(QWidget *parent) : QWidget(parent),mBarCount(11)
{
    // 浅蓝,粉,红,橙,黄,淡绿,绿,青,蓝灰,蓝,紫(默认)
    mDefaultColors << QColor("#0776a9") <<QColor("#FFC0CB")<<Qt::red << QColor(255,165,0,255)<<QColor("#FFFF00")
            << QColor("#90EE90")<<Qt::green<<Qt::cyan<<QColor("#6A5ACD")<<Qt::blue << QColor("#800080") ;

    QHBoxLayout * lay = new QHBoxLayout;

    for(int i = 0; i < mBarCount; ++i)
    {
        QString text = i!=10?QString("0.%1").arg(i):QString("1.0");
        mSelectors.append(new GradientParamSelector(text,mDefaultColors.at(i)));
        lay->addWidget(mSelectors.at(i));
    }
    setLayout(lay);
}

QVector<QPair<qreal,QColor>> GradientParamSelectorWidget::selectedColors() const
{
    QVector<QPair<qreal,QColor>> list;
    QPair<qreal,QColor> pair;
    for(int i = 0; i < mBarCount; ++i){// 颜色对话框点击ok之后colorButton内部的mColor已经更新,可以取出来
        pair.first = mSelectors.at(i)->text().toDouble();
        pair.second = mSelectors.at(i)->color();// 内部返回的mButton->mColor,如果用户没有任何操作,返回的其实就是defaultColors
        list << pair;
    }
    return list;
}

QVector<QPair<qreal,QColor>> GradientParamSelectorWidget::defaultColors() const
{
    QVector<QPair<qreal,QColor>> list;
    QList<qreal> steps = {0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0};
    QPair<qreal,QColor> pair;
    for(int i = 0; i < mBarCount; ++i)
    {
        pair.first =steps.at(i);
        pair.second = mDefaultColors.at(i);
        list << pair;
    }
    return list;
}

void GradientParamSelectorWidget::setGrayStyle()
{
    for(int i = 0; i < mBarCount; ++i)
    {
            mSelectors.at(i)->setGrayStyle();
    }
}

void GradientParamSelectorWidget::setDefaultStyle()
{
    for(int i = 0; i < mBarCount; ++i)
    {
            mSelectors.at(i)->setDefaultStyle();
    }
}
