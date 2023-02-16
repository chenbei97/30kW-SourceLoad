#include "associatebaraxis.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

AssociateBarAxis::AssociateBarAxis(QWidget*parent):QGroupBox(parent)
{
    mBaseVal = 2.;

    mValue = new QRadioButton(tr("线性"));
    mLog = new QRadioButton(tr("对数"));
    mBase = new QDoubleSpinBox;
    mBase->setValue(mBaseVal);
    mBase->setRange(0.01,100);
    mBase->setSingleStep(0.01);

    QHBoxLayout * lay1 = new QHBoxLayout;
    lay1->addWidget(new QLabel(tr("水平坐标轴")));
    lay1->addWidget(new QLabel(tr("自动")));

    QHBoxLayout * lay2 = new QHBoxLayout;
    lay2->addWidget(new QLabel(tr("垂直坐标轴")));
    lay2->addWidget(mValue);
    lay2->addWidget(mLog);
    lay2->addWidget(mBase);
    mBase->hide();
    mValue->setChecked(true);

    QVBoxLayout * lay = new QVBoxLayout;
    lay->addLayout(lay1);
    lay->addLayout(lay2);
    setLayout(lay);
    setFont(QFont("Times New Roman",12));
    setTitle(tr("坐标轴"));

    connect(mValue,&QRadioButton::clicked,this,[=]{mBase->hide();});
    connect(mLog,&QRadioButton::clicked,this,[=]{mBase->show();});
    connect(mBase,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
            this,[=](double base){mBaseVal=base;});
}

double AssociateBarAxis::base() const
{
    return mBaseVal;
}

bool AssociateBarAxis::isValueAxis() const
{
    return mValue->isChecked();
}

bool AssociateBarAxis::isLogAxis() const
{
    return  mLog->isChecked();
}
