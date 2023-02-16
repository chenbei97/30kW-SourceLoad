#include "seriespieslice.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

SeriesPieSlice::SeriesPieSlice(QChart * chart,QWidget* parent):
    QGroupBox(parent),mChart(chart),mCurrentSeries(nullptr)
{
        mCurrentSeries = static_cast<QPieSeries*>(mChart->series().at(0));

        mCurrentSlice = new QComboBox;
        foreach(auto slice,mCurrentSeries->slices()) mCurrentSlice->addItem(slice->label());
        connect(mCurrentSlice,SIGNAL(currentIndexChanged(int)),this,SLOT(changeSlice(int)));

        mSliceValue = new QLabel;
        mSlicePercent = new QLabel;
        mStartAngle = new QLabel;
        mAngleSpan = new QLabel;
        updateShowInfoState();

        mLabelVisible = new QCheckBox;
        updateLabelVisibleState();
        mExploded = new QCheckBox;
        updateExplodedState();

        mBorderWidth = new QSpinBox;
        updateBorderWidthState();
        mDistanceFactor = new QDoubleSpinBox;
        mDistanceFactor->setRange(0.0,1.0);
        mDistanceFactor->setSingleStep(0.1);
        mDistanceFactor->setDecimals(1);
        updateDistanceFactorState();
        mArmLengthFactor = new QDoubleSpinBox;
        mArmLengthFactor->setRange(0.0,1.0);
        mArmLengthFactor->setSingleStep(0.1);
        mArmLengthFactor->setDecimals(1);
        updateArmLengthFactorState();

        mLabelFont = new QPushButton(tr("设置标签格式"));
        updateLabelFontState();
        mLabelColor = new QPushButton(tr("设置标签颜色"));
        updateLabelColorState();
        mFillColor = new QPushButton(tr("设置填充颜色"));
        updateFillColorState();
        mBorderColor = new QPushButton(tr("设置边框颜色"));
        updateBorderColorState();
        mLabelName = new QPushButton(tr("设置标签名称"));
        mLabelNameEdit = new QLineEdit;
        updateLabelNameState();

        QFormLayout * formlay = new QFormLayout;
        formlay->addRow(tr("&当前系列"),mCurrentSlice);
        formlay->addRow(tr("&当前数值"),mSliceValue);
        formlay->addRow(tr("&当前比例"),mSlicePercent);
        formlay->addRow(tr("&当前始角"),mStartAngle);
        formlay->addRow(tr("&当前跨度"),mAngleSpan);
        formlay->addRow(tr("&标签可见"),mLabelVisible);
        formlay->addRow(tr("&标签距离"),mArmLengthFactor);
        formlay->addRow(tr("&标签格式"),mLabelFont);
        formlay->addRow(tr("&标签颜色"),mLabelColor);
        formlay->addRow(tr("&填充颜色"),mFillColor);
        formlay->addRow(tr("&饼块突出"),mExploded);
        formlay->addRow(tr("&饼块距离"),mDistanceFactor);
        formlay->addRow(tr("&边框宽度"),mBorderWidth);
        formlay->addRow(tr("&边框颜色"),mBorderColor);
        QHBoxLayout * nlay = new QHBoxLayout;
        nlay->addWidget(mLabelName);
        nlay->addWidget(mLabelNameEdit);

        QVBoxLayout * lay = new QVBoxLayout;
        lay->addLayout(formlay);
        lay->addLayout(nlay);
        setLayout(lay);
        setTitle(tr("系列"));
}

void SeriesPieSlice::updateAssociateMode(int mode,int flag)
{
    mAssociateMode = AssociateMode(mode); // 枚举值是对应的
    mAssociateFlags[mAssociateMode] = flag;
    // rowMode,flag=0,映射的表格行已保存,flag不使用
    // colMode,flag=0,告知映射的表格列已保存,flag不使用
    // rowRegionMode,flag = firstColumn, 区域的起始列,1列为1个块,那么当前系列对应的表格行就是flag+index
    // colRegionMode,flag = firstRow, 区域的起始行,1行为1个块,那么当前块对应的表格列就是flag+index
}

void SeriesPieSlice::updatePieSlice()
{// 提供给外部使用,对setCurrentSeries的封装,同时还要更新关于块的combo
       mCurrentSlice->clear();
       if (mChart->series().count() == 0) {disconnectAllConnections();return;}// 先更新当前曲线
       mCurrentSeries = static_cast<QPieSeries*>(mChart->series().at(0));
       foreach(auto slice,mCurrentSeries->slices()) //拿到曲线的系列更新
           mCurrentSlice->addItem(slice->label());
       updateState();//再去更新状态
}

void SeriesPieSlice::disconnectAllConnections()
{
    disconnect(mLabelVisible,SIGNAL(stateChanged(int)),this,SLOT(changeLabelVisible(int)));
    disconnect(mExploded,SIGNAL(stateChanged(int)),this,SLOT(changeExploded(int)));
    disconnect(mBorderWidth,SIGNAL(valueChanged(int)),this,SLOT(changeBorderWidth(int)));
    disconnect(mDistanceFactor,SIGNAL(valueChanged(double)),this,SLOT(changeDistanceFactor(double)));
    disconnect(mArmLengthFactor,SIGNAL(valueChanged(double)),this,SLOT(changeArmLengthFactor(double)));
    disconnect(mLabelFont,&QPushButton::clicked,this,&SeriesPieSlice::changeLabelFont);
    disconnect(mLabelColor,&QPushButton::clicked,this,&SeriesPieSlice::changeLabelColor);
    disconnect(mFillColor,&QPushButton::clicked,this,&SeriesPieSlice::changeFillColor);
    disconnect(mBorderColor,&QPushButton::clicked,this,&SeriesPieSlice::changeBorderColor);
    disconnect(mLabelName,&QPushButton::clicked,this,&SeriesPieSlice::changeLabelName);
}

void SeriesPieSlice::changeSlice(int index)
{
    if (index < 0) return;
    updateState();
}

void SeriesPieSlice::updateShowInfoState()
{
    mSliceValue->setText(QString("%1").arg(mCurrentSeries->slices()
                                           .at(mCurrentSlice->currentIndex())->value()));
    mSlicePercent->setText(QString("%1%").arg(mCurrentSeries->slices()
                                             .at(mCurrentSlice->currentIndex())->percentage()*100.));
    mStartAngle->setText(QString("%1°").arg(mCurrentSeries->slices()
                                           .at(mCurrentSlice->currentIndex())->startAngle()));
    mAngleSpan->setText(QString("%1°").arg(mCurrentSeries->slices()
                                          .at(mCurrentSlice->currentIndex())->angleSpan()));
}

void SeriesPieSlice::updateLabelVisibleState()
{
    mLabelVisible->setCheckState(mCurrentSeries->slices().at(mCurrentSlice->currentIndex())->isLabelVisible()?
                                     Qt::Checked:Qt::Unchecked);
    disconnect(mLabelVisible,SIGNAL(stateChanged(int)),this,SLOT(changeLabelVisible(int)));
    connect(mLabelVisible,SIGNAL(stateChanged(int)),this,SLOT(changeLabelVisible(int)));
}

void SeriesPieSlice::changeLabelVisible(int state)
{
    bool s = state == Qt::Checked;
    mCurrentSeries->slices().at(mCurrentSlice->currentIndex())->setLabelVisible(s);
}

void SeriesPieSlice::updateExplodedState()
{
    mExploded->setCheckState(mCurrentSeries->slices().at(mCurrentSlice->currentIndex())->isExploded()?
                                     Qt::Checked:Qt::Unchecked);
    disconnect(mExploded,SIGNAL(stateChanged(int)),this,SLOT(changeExploded(int)));
    connect(mExploded,SIGNAL(stateChanged(int)),this,SLOT(changeExploded(int)));
}

void SeriesPieSlice::changeExploded(int state)
{
    bool s = state == Qt::Checked;
    mCurrentSeries->slices().at(mCurrentSlice->currentIndex())->setExploded(s);
}

void SeriesPieSlice::updateBorderWidthState()
{
    mBorderWidth->setValue(mCurrentSeries->slices().at(mCurrentSlice->currentIndex())->borderWidth());
    disconnect(mBorderWidth,SIGNAL(valueChanged(int)),this,SLOT(changeBorderWidth(int)));
    connect(mBorderWidth,SIGNAL(valueChanged(int)),this,SLOT(changeBorderWidth(int)));
}

void SeriesPieSlice::changeBorderWidth(int width)
{
    mCurrentSeries->slices().at(mCurrentSlice->currentIndex())->setBorderWidth(width);
}

void SeriesPieSlice::updateDistanceFactorState()
{
    mDistanceFactor->setValue(mCurrentSeries->slices().at(mCurrentSlice->currentIndex())->explodeDistanceFactor());
    disconnect(mDistanceFactor,SIGNAL(valueChanged(double)),this,SLOT(changeDistanceFactor(double)));
    connect(mDistanceFactor,SIGNAL(valueChanged(double)),this,SLOT(changeDistanceFactor(double)));
}

void SeriesPieSlice::changeDistanceFactor(double factor)
{
    mCurrentSeries->slices().at(mCurrentSlice->currentIndex())->setExplodeDistanceFactor(factor);
}

void SeriesPieSlice::updateArmLengthFactorState()
{
    mArmLengthFactor->setValue(mCurrentSeries->slices().at(mCurrentSlice->currentIndex())->labelArmLengthFactor());
    disconnect(mArmLengthFactor,SIGNAL(valueChanged(double)),this,SLOT(changeArmLengthFactor(double)));
    connect(mArmLengthFactor,SIGNAL(valueChanged(double)),this,SLOT(changeArmLengthFactor(double)));
}

void SeriesPieSlice::changeArmLengthFactor(double factor)
{
    mCurrentSeries->slices().at(mCurrentSlice->currentIndex())->setLabelArmLengthFactor(factor);
}

void SeriesPieSlice::updateLabelFontState()
{
    disconnect(mLabelFont,&QPushButton::clicked,this,&SeriesPieSlice::changeLabelFont);
    connect(mLabelFont,&QPushButton::clicked,this,&SeriesPieSlice::changeLabelFont);
}

void SeriesPieSlice::changeLabelFont()
{
    auto slices= mCurrentSeries->slices();
    QFont oldfont = slices.at(mCurrentSlice->currentIndex())->labelFont();
    QFontDialog * dlg = fontDialog(oldfont);
    connect(dlg,static_cast<void (QFontDialog::*) (const QFont&)>(&QFontDialog::fontSelected)
           ,this,[=](const QFont& font){
            slices.at(mCurrentSlice->currentIndex())->setLabelFont(font);
     });
    dlg->exec(); delete dlg;
}

void SeriesPieSlice::updateLabelColorState()
{
    disconnect(mLabelColor,&QPushButton::clicked,this,&SeriesPieSlice::changeLabelColor);
    connect(mLabelColor,&QPushButton::clicked,this,&SeriesPieSlice::changeLabelColor);
}

void SeriesPieSlice::changeLabelColor()
{
    auto slices= mCurrentSeries->slices();
    QColor oldcolor = slices.at(mCurrentSlice->currentIndex())->labelColor();
    QColorDialog * dlg = colorDialog(oldcolor);
    connect(dlg,static_cast<void (QColorDialog::*) (const QColor&)>(&QColorDialog::colorSelected)
           ,this,[=](const QColor& color){
        slices.at(mCurrentSlice->currentIndex())->setLabelColor(color);
    });
    dlg->exec(); delete dlg;
}

void SeriesPieSlice::updateFillColorState()
{
    disconnect(mFillColor,&QPushButton::clicked,this,&SeriesPieSlice::changeFillColor);
    connect(mFillColor,&QPushButton::clicked,this,&SeriesPieSlice::changeFillColor);
}

void SeriesPieSlice::changeFillColor()
{
    auto slices= mCurrentSeries->slices();
    QColor oldcolor = slices.at(mCurrentSlice->currentIndex())->color();
    QColorDialog * dlg = colorDialog(oldcolor);
    connect(dlg,static_cast<void (QColorDialog::*) (const QColor&)>(&QColorDialog::colorSelected)
           ,this,[=](const QColor& color){
        slices.at(mCurrentSlice->currentIndex())->setColor(color);
        switch (mAssociateMode) {
            case 0:
                    emit seriesColorChanged(color,mCurrentSlice->currentIndex());
                    break; // row事先已保存,col=index
            case 1:
                     emit seriesColorChanged(color,mCurrentSlice->currentIndex());
                    break; // col事先已保存,row=index
            case 2:
                    emit seriesColorChanged(color,mAssociateFlags[2]+mCurrentSlice->currentIndex());
                    break; // valueRow,labelRow事先已保存,firstColumn+index=col
            case 3:
                    emit seriesColorChanged(color,mAssociateFlags[3]+mCurrentSlice->currentIndex());
                    break;// valueCol,labelCol事先已保存,firstRow+index=row
        }
    });
    dlg->exec(); delete dlg;
}

void SeriesPieSlice::updateBorderColorState()
{
    disconnect(mBorderColor,&QPushButton::clicked,this,&SeriesPieSlice::changeBorderColor);
    connect(mBorderColor,&QPushButton::clicked,this,&SeriesPieSlice::changeBorderColor);
}

void SeriesPieSlice::changeBorderColor()
{
    auto slices= mCurrentSeries->slices();
    QColor oldcolor = slices.at(mCurrentSlice->currentIndex())->borderColor();
    QColorDialog * dlg = colorDialog(oldcolor);
    connect(dlg,static_cast<void (QColorDialog::*) (const QColor&)>(&QColorDialog::colorSelected)
           ,this,[=](const QColor& color){
        slices.at(mCurrentSlice->currentIndex())->setBorderColor(color);
    });
    dlg->exec(); delete dlg;
}

void SeriesPieSlice::updateLabelNameState()
{
    auto label= mCurrentSeries->slices().at(mCurrentSlice->currentIndex())->label();
    auto index = label.indexOf('(');
    mLabelNameEdit->setText(label.left(index));
    disconnect(mLabelName,&QPushButton::clicked,this,&SeriesPieSlice::changeLabelName);
    connect(mLabelName,&QPushButton::clicked,this,&SeriesPieSlice::changeLabelName);
}

void SeriesPieSlice::changeLabelName()
{
    auto slice = mCurrentSeries->slices().at(mCurrentSlice->currentIndex());
    auto name = QString("%1(%2%)").arg(mLabelNameEdit->text()).arg(slice->value()*100.);
    slice->setLabel(name);
    mCurrentSlice->setItemText(mCurrentSlice->currentIndex(),name); // combo跟随改名
}

void SeriesPieSlice::updateState()
{
    updateShowInfoState();
    updateLabelVisibleState();
    updateExplodedState();
    updateBorderWidthState();
    updateDistanceFactorState();
    updateArmLengthFactorState();
    updateLabelFontState();
    updateLabelColorState();
    updateFillColorState();
    updateBorderColorState();
    updateLabelNameState();
}

void SeriesPieSlice::setCurrentSeries(QPieSeries * series)
{
    mCurrentSeries = series;
    updateState();
}

QPieSeries* SeriesPieSlice::currentSeries() const
{
    return mCurrentSeries;
}

QColorDialog* SeriesPieSlice::colorDialog(const QColor&initColor)
{
    QColorDialog * dlg = new QColorDialog(initColor);
    dlg->setFixedSize(800,400);
    dlg->setWindowIcon(mIcon);
    dlg->setOptions(QColorDialog::ShowAlphaChannel|QColorDialog::DontUseNativeDialog);//QColorDialog::NoButtons|
    dlg->setWindowTitle(tr("设置颜色"));
    return dlg;
}

QFontDialog* SeriesPieSlice::fontDialog(const QFont &initFont)
{
    QFontDialog * dlg = new QFontDialog(initFont);
    dlg->setFixedSize(800,400);
    dlg->setWindowIcon(mIcon);
    dlg->setWindowTitle(tr("设置格式"));
    dlg->setOptions(QFontDialog::DontUseNativeDialog);
    return dlg;
}
