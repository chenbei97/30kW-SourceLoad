#include "axisvalue.h"

AxisValue::AxisValue(QChart * chart,QWidget*parent):
    QGroupBox(parent),mChart(chart),mCurrentAxis(nullptr)
{// 必须是QValueAxis,所以不继承AxisBox,也无需构造函数,而是动态设定
     // 针对QValueAxis使用minortickcount,tickcount,labelformat,range,
    mMinRange = new QDoubleSpinBox;
    mMaxRange = new QDoubleSpinBox;
    mMinRange->setSingleStep(0.01);
    mMaxRange->setSingleStep(0.01);
    mMinRange->setRange(-(double)INT_MAX,(double)INT_MAX);
    mMaxRange->setRange(-(double)INT_MAX,(double)INT_MAX);
    mTickCount = new QSpinBox;
    mMinorTickCount = new QSpinBox;
    mLabelFormatEdit = new QLineEdit("%.1f");
    mLabelFormatBtn = new QPushButton(tr("设置刻度格式"));
    mTickCount->setMinimum(2); // 最小2,默认5

    QGridLayout * lay = new QGridLayout;
    lay->setSizeConstraint(QLayout::SetNoConstraint);
    lay->addWidget(new QLabel(tr("轴下限值")),0,0);
    lay->addWidget(mMinRange,0,1);
    lay->addWidget(new QLabel(tr("轴上限值")),1,0);
    lay->addWidget(mMaxRange,1,1);
    lay->addWidget(new QLabel(tr("轴主刻度个数")),2,0);
    lay->addWidget(mTickCount,2,1);
    lay->addWidget(new QLabel(tr("轴次刻度个数")),3,0);
    lay->addWidget(mMinorTickCount,3,1);
    lay->addWidget(mLabelFormatEdit,4,0);
    lay->addWidget(mLabelFormatBtn,4,1);
    lay->addWidget(new QLabel(tr("支持格式字符串(d,i,o,x,X,f,F,e,E,g,G,c)等")),5,0,1,2);

    setLayout(lay); // 布局不影响先初始化完毕
    setMinimumWidth(450); // 跟随axisbox.cpp
    setTitle(tr("线性轴"));
}

void AxisValue::disconnectAllConnections()
{
    disconnect(mMinRange,SIGNAL(valueChanged(double)),this,SLOT(changeMin(double)));
    disconnect(mMaxRange,SIGNAL(valueChanged(double)),this,SLOT(changeMax(double)));
    disconnect(mTickCount,SIGNAL(valueChanged(int)),this,SLOT(changeTickCount(int)));
    disconnect(mMinorTickCount,SIGNAL(valueChanged(int)),this,SLOT(changeMinorTickCount(int)));
    disconnect(mLabelFormatBtn,SIGNAL(clicked()),this,SLOT(changeLabelFormat()));
}

void AxisValue::setCurrentAxis(QValueAxis * axis)
{
    mCurrentAxis = axis;
    updateState();
}

QValueAxis * AxisValue::currentAxis() const
{
    return mCurrentAxis;
}

void AxisValue::updateState()
{
    updateMinState();
    updateMaxState();
    updateTickCountState();
    updateMinorTickCountState();
    updateLabelFormatState();
}

void AxisValue::updateMinState()
{
        mMinRange->setValue(mCurrentAxis->min());

        disconnect(mMinRange,SIGNAL(valueChanged(double)),this,SLOT(changeMin(double)));
        connect(mMinRange,SIGNAL(valueChanged(double)),this,SLOT(changeMin(double)));
}

void AxisValue::changeMin(double min)
{
        mCurrentAxis->setMin(min);
}

void AxisValue::updateMaxState()
{
        mMaxRange->setValue(mCurrentAxis->max());

        disconnect(mMaxRange,SIGNAL(valueChanged(double)),this,SLOT(changeMax(double)));
        connect(mMaxRange,SIGNAL(valueChanged(double)),this,SLOT(changeMax(double)));
}

void AxisValue::changeMax(double max)
{
        mCurrentAxis->setMax(max);
}

void AxisValue::updateTickCountState()
{
       mTickCount->setValue(mCurrentAxis->tickCount());

       disconnect(mTickCount,SIGNAL(valueChanged(int)),this,SLOT(changeTickCount(int)));
       connect(mTickCount,SIGNAL(valueChanged(int)),this,SLOT(changeTickCount(int)));
}

void AxisValue::changeTickCount(int count)
{
       mCurrentAxis->setTickCount(count);
}

void AxisValue::updateMinorTickCountState()
{
       mMinorTickCount->setValue(mCurrentAxis->minorTickCount());

       disconnect(mMinorTickCount,SIGNAL(valueChanged(int)),this,SLOT(changeMinorTickCount(int)));
       connect(mMinorTickCount,SIGNAL(valueChanged(int)),this,SLOT(changeMinorTickCount(int)));
}

void AxisValue::changeMinorTickCount(int count)
{
       mCurrentAxis->setMinorTickCount(count);
}

void AxisValue::updateLabelFormatState()
{
        mLabelFormatEdit->setText(mCurrentAxis->labelFormat());

        disconnect(mLabelFormatBtn,SIGNAL(clicked()),this,SLOT(changeLabelFormat()));
        connect(mLabelFormatBtn,SIGNAL(clicked()),this,SLOT(changeLabelFormat()));
}

void AxisValue::changeLabelFormat()
{
        mCurrentAxis->setLabelFormat(mLabelFormatEdit->text().simplified());
}

