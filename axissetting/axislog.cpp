#include "axislog.h"

AxisLog::AxisLog(QChart * chart,QWidget*parent):
    QGroupBox(parent),mChart(chart),mCurrentAxis(nullptr)
{// 必须是QLogValueAxis,所以不继承AxisBox,也无需构造函数,而是动态设定
    // 针对QLogValueAxis使用minortickcount,labelformat,range,base,tickcount随base变化不能设置
   mBase = new QDoubleSpinBox;
   mMinRange = new QDoubleSpinBox;
   mMaxRange = new QDoubleSpinBox;
   mBase->setSingleStep(0.01);
   mMinRange->setSingleStep(0.01);
   mMaxRange->setSingleStep(0.01);
   mBase->setRange(0.01,100.); // 必须大于0且不能等于1
   mMinRange->setRange(0.01,(double)INT_MAX); // 必须大于0
   mMaxRange->setRange(0.01,(double)INT_MAX);
   mTickCount = new QLabel;
   mMinorTickCount = new QSpinBox;
   mLabelFormatEdit = new QLineEdit("%.1f");
   mLabelFormatBtn = new QPushButton(tr("设置刻度格式"));
   QLabel * tip = new QLabel(tr("支持格式字符串(d,i,o,x,X,f,F,e,E,g,G,c)等"));
   //tip->setMinimumHeight(30);

   QFormLayout * lay = new QFormLayout;
   lay->setSizeConstraint(QLayout::SetNoConstraint);
   lay->addRow(tr("&轴主刻度个数"),mTickCount);
   lay->addRow(tr("&轴次刻度个数"),mMinorTickCount);
   lay->addRow(tr("&轴底数"),mBase);
   lay->addRow(tr("&轴下限值"),mMinRange);
   lay->addRow(tr("&轴上限值"),mMaxRange);
   QHBoxLayout * llay = new QHBoxLayout;
   llay->setSizeConstraint(QLayout::SetNoConstraint);
   llay->addWidget(mLabelFormatEdit);
   llay->addWidget(mLabelFormatBtn);

   QVBoxLayout * vlay = new QVBoxLayout;
   vlay->setSizeConstraint(QLayout::SetNoConstraint);
   vlay->addLayout(lay);
   vlay->addLayout(llay);
   vlay->addWidget(tip);

   setLayout(vlay); // 布局不影响先初始化完毕
   setMinimumWidth(450); // 跟随axisbox.cpp
   setTitle(tr("对数轴"));
}

void AxisLog::disconnectAllConnections()
{
    disconnect(mBase,SIGNAL(valueChanged(double)),this,SLOT(changeBase(double)));
    disconnect(mMinRange,SIGNAL(valueChanged(double)),this,SLOT(changeMin(double)));
    disconnect(mMaxRange,SIGNAL(valueChanged(double)),this,SLOT(changeMax(double)));
    disconnect(mMinorTickCount,SIGNAL(valueChanged(int)),this,SLOT(changeMinorTickCount(int)));
    disconnect(mLabelFormatBtn,SIGNAL(clicked()),this,SLOT(changeLabelFormat()));
}

void AxisLog::setCurrentAxis(QLogValueAxis*axis)
{
    mCurrentAxis = axis;
    updateState();
}

QLogValueAxis* AxisLog::currentAxis() const
{
    return mCurrentAxis;
}

void AxisLog::updateState()
{
    updateBaseState();
    updateMinState();
    updateMaxState();
    updateTickCountState();
    updateMinorTickCountState();
    updateLabelFormatState();
}

void AxisLog::updateBaseState()
{
        mBase->setValue(mCurrentAxis->base());

        disconnect(mBase,SIGNAL(valueChanged(double)),this,SLOT(changeBase(double)));
        connect(mBase,SIGNAL(valueChanged(double)),this,SLOT(changeBase(double)));
}

void AxisLog::changeBase(double base)
{
        if (base == 1.00) return;
        mCurrentAxis->setBase(base);
        updateTickCountState(); // base的改变可能会让tickcount改变
}

void AxisLog::updateMinState()
{
        mMinRange->setValue(mCurrentAxis->min());

        disconnect(mMinRange,SIGNAL(valueChanged(double)),this,SLOT(changeMin(double)));
        connect(mMinRange,SIGNAL(valueChanged(double)),this,SLOT(changeMin(double)));
}

void AxisLog::changeMin(double min)
{
        mCurrentAxis->setMin(min);
}

void AxisLog::updateMaxState()
{
        mMaxRange->setValue(mCurrentAxis->max());

        disconnect(mMaxRange,SIGNAL(valueChanged(double)),this,SLOT(changeMax(double)));
        connect(mMaxRange,SIGNAL(valueChanged(double)),this,SLOT(changeMax(double)));
}

void AxisLog::changeMax(double max)
{
        mCurrentAxis->setMax(max);
}

void AxisLog::updateTickCountState()
{
        mTickCount->setText(tr("%1").arg(mCurrentAxis->tickCount()));
}

void AxisLog::updateMinorTickCountState()
{
       mMinorTickCount->setValue(mCurrentAxis->minorTickCount());

       disconnect(mMinorTickCount,SIGNAL(valueChanged(int)),this,SLOT(changeMinorTickCount(int)));
       connect(mMinorTickCount,SIGNAL(valueChanged(int)),this,SLOT(changeMinorTickCount(int)));
}

void AxisLog::changeMinorTickCount(int count)
{
       mCurrentAxis->setMinorTickCount(count);
       //qDebug()<<mCurrentAxis->minorTickCount(); // 返回确实跟随了说明设置成功
       // 但是界面没反应可能是底层原因
}

void AxisLog::updateLabelFormatState()
{
        mLabelFormatEdit->setText(mCurrentAxis->labelFormat());

        disconnect(mLabelFormatBtn,SIGNAL(clicked()),this,SLOT(changeLabelFormat()));
        connect(mLabelFormatBtn,SIGNAL(clicked()),this,SLOT(changeLabelFormat()));
}

void AxisLog::changeLabelFormat()
{
        mCurrentAxis->setLabelFormat(mLabelFormatEdit->text().simplified());
}


