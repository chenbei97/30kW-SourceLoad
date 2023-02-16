#include "seriesinfo.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

SeriesInfo::SeriesInfo(QChart * chart,QWidget*parent):
    SeriesBox(chart,parent)
{
        mSeriesType = new QLabel;
        updateTypeState();

        mSeriseVisible = new QCheckBox;
        updateVisibilityState();

        mSeriesOpacity = new QDoubleSpinBox;
        mSeriesOpacity->setRange(0.0,1.0);
        mSeriesOpacity->setSingleStep(0.1);
        mSeriesOpacity->setDecimals(1);
        updateOpacityState();

        mSeriesNameEdit = new QLineEdit;
        mSeriesNameBtn = new QPushButton(tr("设置曲线名称"));
        QHBoxLayout * nlay = new QHBoxLayout;
        nlay->addWidget(mSeriesNameEdit);
        nlay->addWidget(mSeriesNameBtn);
        updateNameState();

        QFormLayout * flay = new QFormLayout;
        flay->addRow(tr("&曲线类型"),mSeriesType);
        flay->addRow(tr("&曲线可见"),mSeriseVisible);
        flay->addRow(tr("&曲线透明"),mSeriesOpacity);

        QVBoxLayout * lay = new QVBoxLayout;
        lay->addLayout(flay);
        lay->addLayout(nlay);
        setLayout(lay);
        setTitle(tr("曲线"));
}

void SeriesInfo::updateInfo()
{ // 用于给外部使用,对setCurrentSeries的一层封装
    if (!mChart->series().count())  {
        disconnectAllConnections(); //
        return; // empty之后可能还没关联,这个时候没有新的series就不要进行
    }
     setCurrentSeries(mChart->series().at(0),0);// 内部调用updateState()更新
}

void SeriesInfo::updateState()
{
    updateTypeState();
    updateVisibilityState();
    updateOpacityState();
    updateNameState();
}

void SeriesInfo::disconnectAllConnections()
{
    disconnect(mSeriseVisible,&QCheckBox::stateChanged,this,&SeriesInfo::changeVisibility);
    disconnect(mSeriesOpacity,SIGNAL(valueChanged(double)),this,SLOT(changeOpacity(double)));
    disconnect(mSeriesNameBtn,&QPushButton::clicked,this,&SeriesInfo::changeName);
}

void SeriesInfo::updateTypeState()
{
    switch (mCurrentSeries->type()) {
            case 0: mSeriesType->setText("line chart");break;
            case 1: mSeriesType->setText("area chart");break;
            case 2: mSeriesType->setText("vertical bar chart");break;
            case 3: mSeriesType->setText("vertical stacked bar chart");break;
            case 4: mSeriesType->setText("vertical percent bar chart");break;
            case 5: mSeriesType->setText("pie chart");break;
            case 6: mSeriesType->setText("scatter chart");break;
            case 7: mSeriesType->setText("spline chart");break;
            case 8: mSeriesType->setText("horizonal bar chart");break;
            case 9: mSeriesType->setText("horizonal stacked bar chart");break;
            case 10: mSeriesType->setText("horizonal percent bar chart");break;
            case 11: mSeriesType->setText("box plot chart");break;
            case 12: mSeriesType->setText("candlestick chart");break;
            default:break;
    }
}

void SeriesInfo::updateVisibilityState()
{
    Qt::CheckState state = mCurrentSeries->isVisible()?Qt::Checked:Qt::Unchecked;
    mSeriseVisible->setCheckState(state); // 当前曲线会切换,都要连接,又要避免重复连接
    disconnect(mSeriseVisible,&QCheckBox::stateChanged,this,&SeriesInfo::changeVisibility);
    connect(mSeriseVisible,&QCheckBox::stateChanged,this,&SeriesInfo::changeVisibility);
}

void SeriesInfo::changeVisibility(int state)
{
    bool s = state == Qt::Checked;
    mCurrentSeries->setVisible(s);
}

void SeriesInfo::updateOpacityState()
{
    mSeriesOpacity->setValue(mCurrentSeries->opacity());
    disconnect(mSeriesOpacity,SIGNAL(valueChanged(double)),this,SLOT(changeOpacity(double)));
    connect(mSeriesOpacity,SIGNAL(valueChanged(double)),this,SLOT(changeOpacity(double)));
}

void SeriesInfo::changeOpacity(double opacity)
{
    mCurrentSeries->setOpacity(opacity);
}

void SeriesInfo::updateNameState()
{
    mSeriesNameEdit->setText(mCurrentSeries->name());
    disconnect(mSeriesNameBtn,&QPushButton::clicked,this,&SeriesInfo::changeName);
    connect(mSeriesNameBtn,&QPushButton::clicked,this,&SeriesInfo::changeName);
}

void SeriesInfo::changeName()
{
    mCurrentSeries->setName(mSeriesNameEdit->text()); // 一旦改名要让曲线选择的Combo也要同步更改
    emit nameChanged(mSeriesNameEdit->text(),mCurrentSeriesId);
}
