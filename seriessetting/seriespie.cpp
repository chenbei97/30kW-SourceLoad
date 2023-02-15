#include "seriespie.h"

SeriesPie::SeriesPie(QChart*chart,QWidget*parent):QGroupBox(parent),mChart(chart)
{
    mIcon.addFile(":/images/toolbox_series.png");
    mCurrentSeries = static_cast<QPieSeries*>(mChart->series().at(0));

    mSum = new QLabel;
    updateSumState();

    mLabelPosition = new QComboBox;
    mLabelPosition->addItems(QStringList()<<"Outside"<<"InsideHorizontal"
                             <<"InsideTangential"<<"InsideNormal");
    updateLabelPositionState(); // 所有块统一设置

    mHoleSize = new QDoubleSpinBox;
    mHoleSize->setRange(0.0,1.0);
    mHoleSize->setSingleStep(0.1);
    updateHoleSizeState();

    mPieSize = new QDoubleSpinBox;
    mPieSize->setRange(0.0,1.0);
    mPieSize->setSingleStep(0.1);
    updatePieSizeState();

    mHPosition = new QDoubleSpinBox;
    mHPosition->setRange(0.0,1.0);
    mHPosition->setSingleStep(0.1);
    updateHPositionState();

    mVPosition = new QDoubleSpinBox;
    mVPosition->setRange(0.0,1.0);
    mVPosition->setSingleStep(0.1);
    updateVPositionState();

    mStartAngle = new GradientParamSpinBox;
    mStartAngle->setRange(0.0,360.);
    mStartAngle->setSuffix("°");
    updateStartAngleState();

    mEndAngle = new GradientParamSpinBox;
    mEndAngle->setRange(0.0,360.);
    mEndAngle->setSuffix("°");
    updateEndAngleState();

    QFormLayout * lay = new QFormLayout;
    lay->addRow(tr("&系列和数"),mSum);
    lay->addRow(tr("&标签位置"),mLabelPosition);
    lay->addRow(tr("&饼洞比例"),mHoleSize);
    lay->addRow(tr("&饼块比例"),mPieSize);
    lay->addRow(tr("&水平位置"),mHPosition);
    lay->addRow(tr("&垂直位置"),mVPosition);
    lay->addRow(tr("&起始角度"),mStartAngle);
    lay->addRow(tr("&终止角度"),mEndAngle);
    setLayout(lay);
    setTitle(tr("饼图"));
}

void SeriesPie::updatePie()
{ // 提供给外部使用,对setCurrentSeries的封装
    if (mChart->series().count() == 0) {disconnectAllConnections();return;}
    setCurrentSeries(static_cast<QPieSeries*>(mChart->series().at(0)));
}

void SeriesPie::disconnectAllConnections()
{
    disconnect(mEndAngle,SIGNAL(valueChanged(double)),this,SLOT(changeEndAngle(double)));
    disconnect(mStartAngle,SIGNAL(valueChanged(double)),this,SLOT(changeStartAngle(double)));
    disconnect(mHPosition,SIGNAL(valueChanged(double)),this,SLOT(changeHPosition(double)));
    disconnect(mVPosition,SIGNAL(valueChanged(double)),this,SLOT(changeVPosition(double)));
    disconnect(mPieSize,SIGNAL(valueChanged(double)),this,SLOT(changePieSize(double)));
    disconnect(mHoleSize,SIGNAL(valueChanged(double)),this,SLOT(changeHoleSize(double)));
    disconnect(mLabelPosition,SIGNAL(currentIndexChanged(int)),this,SLOT(changeLabelPosition(int)));
}

void SeriesPie::updateEndAngleState()
{
    mEndAngle->setValue(mCurrentSeries->pieEndAngle());
    disconnect(mEndAngle,SIGNAL(valueChanged(double)),this,SLOT(changeEndAngle(double)));
    connect(mEndAngle,SIGNAL(valueChanged(double)),this,SLOT(changeEndAngle(double)));
}

void SeriesPie::changeEndAngle(double angle)
{
    mCurrentSeries->setPieEndAngle(angle);
}

void SeriesPie::updateStartAngleState()
{
    mStartAngle->setValue(mCurrentSeries->pieStartAngle());
    disconnect(mStartAngle,SIGNAL(valueChanged(double)),this,SLOT(changeStartAngle(double)));
    connect(mStartAngle,SIGNAL(valueChanged(double)),this,SLOT(changeStartAngle(double)));
}

void SeriesPie::changeStartAngle(double angle)
{
    mCurrentSeries->setPieStartAngle(angle);
}

void SeriesPie::updateHPositionState()
{
    mHPosition->setValue(mCurrentSeries->horizontalPosition());
    disconnect(mHPosition,SIGNAL(valueChanged(double)),this,SLOT(changeHPosition(double)));
    connect(mHPosition,SIGNAL(valueChanged(double)),this,SLOT(changeHPosition(double)));
}

void SeriesPie::changeHPosition(double pos)
{
    mCurrentSeries->setHorizontalPosition(pos);
}

void SeriesPie::updateVPositionState()
{
    mVPosition->setValue(mCurrentSeries->verticalPosition());
    disconnect(mVPosition,SIGNAL(valueChanged(double)),this,SLOT(changeVPosition(double)));
    connect(mVPosition,SIGNAL(valueChanged(double)),this,SLOT(changeVPosition(double)));
}

void SeriesPie::changeVPosition(double pos)
{
    mCurrentSeries->setVerticalPosition(pos);
}

void SeriesPie::updatePieSizeState()
{
    mPieSize->setValue(mCurrentSeries->pieSize());
    disconnect(mPieSize,SIGNAL(valueChanged(double)),this,SLOT(changePieSize(double)));
    connect(mPieSize,SIGNAL(valueChanged(double)),this,SLOT(changePieSize(double)));
}

void SeriesPie::changePieSize(double size)
{
    mCurrentSeries->setPieSize(size);
}

void SeriesPie::updateHoleSizeState()
{
    mHoleSize->setValue(mCurrentSeries->holeSize());
    disconnect(mHoleSize,SIGNAL(valueChanged(double)),this,SLOT(changeHoleSize(double)));
    connect(mHoleSize,SIGNAL(valueChanged(double)),this,SLOT(changeHoleSize(double)));
}

void SeriesPie::changeHoleSize(double size)
{
    mCurrentSeries->setHoleSize(size);
}

void SeriesPie::updateLabelPositionState()
{
    mLabelPosition->setCurrentIndex(mCurrentSeries->slices().at(0)->labelPosition());
    disconnect(mLabelPosition,SIGNAL(currentIndexChanged(int)),
               this,SLOT(changeLabelPosition(int)));
    connect(mLabelPosition,SIGNAL(currentIndexChanged(int)),
                   this,SLOT(changeLabelPosition(int)));
}

void SeriesPie::changeLabelPosition(int index)
{
    auto slices = mCurrentSeries->slices();
    for(auto slice:  slices) // 统一设置
        slice->setLabelPosition(QPieSlice::LabelPosition(index));
}

void SeriesPie::updateSumState()
{
    mSum->setText(QString("%1").arg(mCurrentSeries->sum()));
}

void SeriesPie::setCurrentSeries(QPieSeries *series)
{
    mCurrentSeries = series;
    updateState();
}

QPieSeries* SeriesPie::currentSeries() const
{
    return  mCurrentSeries;
}

void SeriesPie::updateState()
{
    updateLabelPositionState();
    updateStartAngleState();
    updateEndAngleState();
    updateHoleSizeState();
    updatePieSizeState();
    updateHPositionState();
    updateVPositionState();
}
