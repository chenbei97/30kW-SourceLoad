#include "chartbackground.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

ChartBackground::ChartBackground(QChart *chart,const QIcon& icon,QWidget*parent):ChartBox(chart,icon,parent)
{
    QCheckBox * v = new QCheckBox;
    v->setCheckState(Qt::Checked); // chart.cpp默认可见
    connect(v,static_cast<void (QCheckBox::*)(int)>(&QCheckBox::stateChanged),this,[=](int state){
            bool r = state == Qt::Unchecked;
            mChart->setBackgroundVisible(!r);
    });

    QCheckBox * shadow = new QCheckBox;
    shadow->setCheckState(Qt::Checked); // chart.cpp 默认启用阴影效果
    connect(shadow,static_cast<void (QCheckBox::*)(int)>(&QCheckBox::stateChanged),
            this,[=](int state){
            bool r = state == Qt::Checked;
            mChart->setDropShadowEnabled(r);
    });

    QPushButton * c = new QPushButton(tr("设置背景"));
    mBackgroundArea = new BackgroundArea(chart,icon);
    connect(c,&QPushButton::clicked,this,[=]{mBackgroundArea->show();});

    QPushButton * b = new QPushButton(tr("设置边框"));
    mBackgroundBorder = new BackgroundBorder(chart,icon);
    connect(b,&QPushButton::clicked,this,[=]{mBackgroundBorder->show();});

    QFormLayout * lay = new QFormLayout;
    lay->addRow(tr("&背景可见"),v);
    lay->addRow(tr("&阴影效果"),shadow);
    lay->addRow(tr("&背景画刷"),c);
    lay->addRow(tr("&背景边框"),b);
    setTitle(tr("背景"));
    setLayout(lay);
}

void ChartBackground::closeChindrenWindows()
{
    mBackgroundArea->closeChildrenWindows();
    mBackgroundArea->close();
    mBackgroundBorder->close();
}
