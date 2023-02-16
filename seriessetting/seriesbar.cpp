#include "seriesbar.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

SeriesBar::SeriesBar(QChart * chart,QWidget* parent):
    QGroupBox(parent),mChart(chart),mCurrentSeries(nullptr)
{ // barwith,labe;visible,labelposition,labelangle, label format不再提供
    // labelfont和bordercolor每个类别的柱状图都可以单独设置,但是这里统一设置
    // 还可以设置填充颜色和标签颜色,每个类别不同,需要分别设置,见SeriesBarSet
    mIcon.addFile(":/images/toolbox_series.png");
    mCurrentSeries = static_cast<QBarSeries*>(mChart->series().at(0));

    mBarWidth = new QDoubleSpinBox;
    updateWidthState();

    mLabelVisible = new QCheckBox;
    updateVisibleState();

    mLabelAngle = new GradientParamSpinBox;
    mLabelAngle->setSuffix("°");
    mLabelAngle->setRange(0.,360.);
    updateAngleState();

    mLabelPosition = new QComboBox;
    mLabelPosition->addItems(QStringList()<<tr("居中")<<tr("顶部靠下")
                             <<tr("底部")<<tr("顶部靠上"));
    updatePositionState();

    mLabelFont = new QPushButton(tr("设置标签格式"));
    updateFontState();

    mBorderColor = new QPushButton(tr("设置边框颜色"));
    updateBorderColorState();

    QFormLayout * lay = new QFormLayout;
    lay->addRow(tr("&柱宽"),mBarWidth);
    lay->addRow(tr("&标签可见"),mLabelVisible);
    lay->addRow(tr("&标签角度"),mLabelAngle);
    lay->addRow(tr("&标签位置"),mLabelPosition);
    lay->addRow(tr("&标签格式"),mLabelFont);
    lay->addRow(tr("&边框颜色"),mBorderColor);

    setTitle(tr("柱状图"));
    setLayout(lay);
}

void SeriesBar::updateBar()
{ // 提供给外部使用,对setCurrentSeries的封装
    if (mChart->series().count() == 0) {disconnectAllConnections();return;} //防止empty后曲线并不存在操作曲线工具栏导致异常
    setCurrentSeries(static_cast<QBarSeries*>(mChart->series().at(0)));
}

void SeriesBar::disconnectAllConnections()
{
    disconnect(mBarWidth,SIGNAL(valueChanged(double)),this,SLOT(changeWidth(double)));
    disconnect(mLabelVisible,SIGNAL(stateChanged(int)),this,SLOT(changeVisible(int)));
    disconnect(mLabelAngle,SIGNAL(valueChanged(double)),this,SLOT(changeAngle(double)));
    disconnect(mLabelPosition,SIGNAL(currentIndexChanged(int)),this,SLOT(changePosition(int)));
    disconnect(mLabelFont,&QPushButton::clicked,this,&SeriesBar::changeFont);
    disconnect(mBorderColor,&QPushButton::clicked,this,&SeriesBar::changeBorderColor);
}

void SeriesBar::setCurrentSeries(QBarSeries *series)
{
    mCurrentSeries = series;
    updateState();
}

QBarSeries* SeriesBar::currentSeries() const
{
    return  mCurrentSeries;
}

void SeriesBar::updateState()
{
    updateWidthState();
    updateVisibleState();
    updateAngleState();
    updatePositionState();
    updateFontState();
    updateBorderColorState();
}

void SeriesBar::updateWidthState()
{
    mBarWidth->setValue(mCurrentSeries->barWidth());
    disconnect(mBarWidth,SIGNAL(valueChanged(double)),this,SLOT(changeWidth(double)));
    connect(mBarWidth,SIGNAL(valueChanged(double)),this,SLOT(changeWidth(double)));
}

void SeriesBar::changeWidth(double width)
{
    mCurrentSeries->setBarWidth(width);
}

void SeriesBar::updateVisibleState()
{
    auto state = mCurrentSeries->isLabelsVisible()?Qt::Checked:Qt::Unchecked;
    mLabelVisible->setCheckState(state);

    disconnect(mLabelVisible,SIGNAL(stateChanged(int)),this,SLOT(changeVisible(int)));
    connect(mLabelVisible,SIGNAL(stateChanged(int)),this,SLOT(changeVisible(int)));
}

void SeriesBar::changeVisible(int state)
{
    bool s = state ==Qt::Checked;
    mCurrentSeries->setLabelsVisible(s);
}

void SeriesBar::updateAngleState()
{
    mLabelAngle->setValue(mCurrentSeries->labelsAngle());
    disconnect(mLabelAngle,SIGNAL(valueChanged(double)),this,SLOT(changeAngle(double)));
    connect(mLabelAngle,SIGNAL(valueChanged(double)),this,SLOT(changeAngle(double)));
}

void SeriesBar::changeAngle(double angle)
{
    mCurrentSeries->setLabelsAngle(angle);
}

void SeriesBar::updatePositionState()
{
    mLabelPosition->setCurrentIndex(mCurrentSeries->labelsPosition());
    disconnect(mLabelPosition,SIGNAL(currentIndexChanged(int)),this,SLOT(changePosition(int)));
    connect(mLabelPosition,SIGNAL(currentIndexChanged(int)),this,SLOT(changePosition(int)));
}

void SeriesBar::changePosition(int index)
{
    mCurrentSeries->setLabelsPosition(QAbstractBarSeries::LabelsPosition(index));
}

void SeriesBar::updateFontState()
{
    disconnect(mLabelFont,&QPushButton::clicked,this,&SeriesBar::changeFont);
    connect(mLabelFont,&QPushButton::clicked,this,&SeriesBar::changeFont);
}

void SeriesBar::changeFont()
{
    auto barsets = mCurrentSeries->barSets();
    QFont oldfont = barsets.at(0)->labelFont(); //外部可以保证至少有1个,0个也不会调用到这里
    QFontDialog * dlg = fontDialog(oldfont);
    connect(dlg,static_cast<void (QFontDialog::*) (const QFont&)>(&QFontDialog::fontSelected)
           ,this,[=](const QFont& font){
        foreach(auto set,barsets){
            set->setLabelFont(font); // 所有统一设置成一样格式
        }
     });
    dlg->exec(); delete dlg;
}

void SeriesBar::updateBorderColorState()
{
    disconnect(mBorderColor,&QPushButton::clicked,this,&SeriesBar::changeBorderColor);
    connect(mBorderColor,&QPushButton::clicked,this,&SeriesBar::changeBorderColor);
}

void SeriesBar::changeBorderColor()
{
    auto barsets = mCurrentSeries->barSets();
    QColor oldcolor = barsets.at(0)->labelColor();
    QColorDialog * dlg = colorDialog(oldcolor);
    connect(dlg,static_cast<void (QColorDialog::*) (const QColor&)>(&QColorDialog::colorSelected)
           ,this,[=](const QColor& color){
        foreach(auto set,barsets){
            set->setBorderColor(color); // 所有统一设置成一样颜色
        }
    });
    dlg->exec(); delete dlg;
}

QColorDialog* SeriesBar::colorDialog(const QColor&initColor)
{
    QColorDialog * dlg = new QColorDialog(initColor);
    dlg->setFixedSize(800,400);
    dlg->setWindowIcon(mIcon);
    dlg->setOptions(QColorDialog::ShowAlphaChannel|QColorDialog::DontUseNativeDialog);//QColorDialog::NoButtons|
    dlg->setWindowTitle(tr("设置颜色"));
    return dlg;
}

QFontDialog* SeriesBar::fontDialog(const QFont &initFont)
{
    QFontDialog * dlg = new QFontDialog(initFont);
    dlg->setFixedSize(800,400);
    dlg->setWindowIcon(mIcon);
    dlg->setWindowTitle(tr("设置格式"));
    dlg->setOptions(QFontDialog::DontUseNativeDialog);
    return dlg;
}
