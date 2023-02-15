#include "axistime.h"

AxisTime::AxisTime(QChart * chart,QWidget*parent):
     QGroupBox(parent),mChart(chart),mCurrentAxis(nullptr)
{ // 时间轴支持5类参数的设置
        mDateFormatString = "dd-MM-yyyy";// 默认dd-MM-yyyy
        mTimeFormatString = "h:mm";// 默认h:mm

        mMinTimeEdit = new QDateTimeEdit;
        mMaxTimeEdit = new QDateTimeEdit;
        mTickCount = new QSpinBox;
        mDateFormat = new QComboBox;
        mDateFormat->addItems(QStringList()<<"dd-MM-yyyy"<<"yyyy.MM.dd"<<"yyyy.MM.dd ddd"
                              <<"MMM ddd yy.dd"<<"yyyy"<<"MMM"<<"dd"<<"yyyy.MM"<<"MM.dd"<<"none");
        mTimeFormat = new QComboBox;
        mTimeFormat->addItems(QStringList()<<"h:mm"<<"hh:mm:ss"<<"hh:mm:ss.zzz"
                              <<"hh:mm:ss.zzz ap"<<"none");

        QFormLayout * lay = new QFormLayout;
        lay->setSizeConstraint(QLayout::SetNoConstraint);
        lay->addRow(tr("&轴下限值"),mMinTimeEdit);
        lay->addRow(tr("&轴上限值"),mMaxTimeEdit);
        lay->addRow(tr("&轴主刻度个数"),mTickCount);
        lay->addRow(tr("&日期格式"),mDateFormat);
        lay->addRow(tr("&时间格式"),mTimeFormat);
        setLayout(lay);
        setMinimumWidth(450); // 跟随axisbox.cpp
        setTitle(tr("时间轴"));
}

void AxisTime::setCurrentAxis(QDateTimeAxis * axis)
{
    mCurrentAxis = axis;
    updateState();
}

QDateTimeAxis * AxisTime::currentAxis() const
{
    return mCurrentAxis;
}

void AxisTime::disconnectAllConnections()
{
    disconnect(mMinTimeEdit,SIGNAL(dateTimeChanged(const QDateTime&)),this,SLOT(changeMin(const QDateTime&)));
    disconnect(mMaxTimeEdit,SIGNAL(dateTimeChanged(const QDateTime&)),this,SLOT(changeMax(const QDateTime&)));
    disconnect(mTickCount,SIGNAL(valueChanged(int)),this,SLOT(changeTickCount(int)));
    disconnect(mDateFormat,SIGNAL(currentIndexChanged(const QString&)),this,SLOT(changeDateFormat(const QString&)));
    disconnect(mTimeFormat,SIGNAL(currentIndexChanged(const QString&)),this,SLOT(changeTimeFormat(const QString&)));
}

void AxisTime::updateState()
{
    updateMinState();
    updateMaxState();
    updateTickCountState();
    updateDateFormatState();
    updateTimeFormatState();
}

void AxisTime::updateMinState()
{
        mMinTimeEdit->setDateTime(mCurrentAxis->min());
        disconnect(mMinTimeEdit,SIGNAL(dateTimeChanged(const QDateTime&)),
                this,SLOT(changeMin(const QDateTime&)));
        connect(mMinTimeEdit,SIGNAL(dateTimeChanged(const QDateTime&)),
                this,SLOT(changeMin(const QDateTime&)));
}

void AxisTime::changeMin(const QDateTime& datetime)
{
        mCurrentAxis->setMin(datetime);
}

void AxisTime::updateMaxState()
{
    mMaxTimeEdit->setDateTime(mCurrentAxis->max());
    disconnect(mMaxTimeEdit,SIGNAL(dateTimeChanged(const QDateTime&)),
            this,SLOT(changeMax(const QDateTime&)));
    connect(mMaxTimeEdit,SIGNAL(dateTimeChanged(const QDateTime&)),
            this,SLOT(changeMax(const QDateTime&)));
}

void AxisTime::changeMax(const QDateTime& datetime)
{
    mCurrentAxis->setMax(datetime);
}

void AxisTime::updateTickCountState()
{
    mTickCount->setValue(mCurrentAxis->tickCount());

    disconnect(mTickCount,SIGNAL(valueChanged(int)),this,SLOT(changeTickCount(int)));
    connect(mTickCount,SIGNAL(valueChanged(int)),this,SLOT(changeTickCount(int)));
}

void AxisTime::changeTickCount(int count)
{
       mCurrentAxis->setTickCount(count);
}

void AxisTime::updateDateFormatState()
{
    auto datetimeformat = mCurrentAxis->format().split("\n");
    mDateFormatString = datetimeformat.at(0); // 可能为空

    mDateFormat->setCurrentText(mDateFormatString);
    disconnect(mDateFormat,SIGNAL(currentIndexChanged(const QString&)),
            this,SLOT(changeDateFormat(const QString&)));
    connect(mDateFormat,SIGNAL(currentIndexChanged(const QString&)),
            this,SLOT(changeDateFormat(const QString&)));
}

void AxisTime::changeDateFormat(const QString& dateformat)
{
    mDateFormatString = dateformat;
    if (dateformat == "none" ) mDateFormatString = "";
    else mDateFormatString = dateformat;
    mCurrentAxis->setFormat(mDateFormatString+"\n"+mTimeFormatString);
}

void AxisTime::updateTimeFormatState()
{
    auto datetimeformat = mCurrentAxis->format().split("\n");
    mTimeFormatString = datetimeformat.at(1);// 可能为空
    mTimeFormat->setCurrentText(mTimeFormatString);

    disconnect(mTimeFormat,SIGNAL(currentIndexChanged(const QString&)),
            this,SLOT(changeTimeFormat(const QString&)));
    connect(mTimeFormat,SIGNAL(currentIndexChanged(const QString&)),
            this,SLOT(changeTimeFormat(const QString&)));
}

void AxisTime::changeTimeFormat(const QString& timeformat)
{
    mTimeFormatString = timeformat;
    if (timeformat == "none") mTimeFormatString = "";
    mCurrentAxis->setFormat(mDateFormatString+"\n"+mTimeFormatString);
}
