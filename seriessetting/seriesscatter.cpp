#include "seriesscatter.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

SeriesScatter::SeriesScatter(QChart * chart,QWidget* parent):
    QGroupBox(parent),mChart(chart),mCurrentSeries(nullptr),mCurrentSeriesId(0)
{
    mIcon.addFile(":/images/toolbox_series.png");
    mCurrentSeries = static_cast<QScatterSeries*>(mChart->series().at(0));

    mMarkerShape = new QComboBox;
    mMarkerShape->addItems(QStringList()<<"Circle"<<"Rectangle");
    updateMarkerShape();

    mMarkerSize = new QDoubleSpinBox;
    updateMarkerSize();

    mBorderColor = new QPushButton(tr("设置边框颜色"));
    updateBorderColor();

    mFillColor = new QPushButton(tr("设置填充颜色"));
    updateFillColor();

    QFormLayout * lay = new QFormLayout;
    lay->addRow(tr("&标记形状"),mMarkerShape);
    lay->addRow(tr("&标记大小"),mMarkerSize);
    lay->addRow(tr("&标记边框"),mBorderColor);
    lay->addRow(tr("&标记填充"),mFillColor);
    setTitle(tr("标记"));
    setLayout(lay);
}

void SeriesScatter::updateScatter()
{
    if (!mChart->series().count())
    {
        disconnectAllConnections();
        return;
    }
    setCurrentSeries(static_cast<QScatterSeries*>(mChart->series().at(0)),0);
}

void SeriesScatter::disconnectAllConnections()
{
    disconnect(mMarkerShape,SIGNAL(currentIndexChanged(int)),this,SLOT(changeShape(int)));
    disconnect(mMarkerSize,SIGNAL(valueChanged(double)),this,SLOT(changeSize(double)));
    disconnect(mBorderColor,&QPushButton::clicked,this,&SeriesScatter::changeBorderColor);
    disconnect(mFillColor,&QPushButton::clicked,this,&SeriesScatter::changeFillColor);
}


void SeriesScatter::setCurrentSeries(QScatterSeries *series, int id)
{
    mCurrentSeries = series;
    mCurrentSeriesId = id;
    updateState();
}

QScatterSeries * SeriesScatter::currentSeries() const
{
    return mCurrentSeries;
}

void SeriesScatter::setCurrentSeriesId(int id)
{
    mCurrentSeriesId = id;
}

int SeriesScatter::currentSeriesId() const
{
    return mCurrentSeriesId;
}

void SeriesScatter::updateState()
{
    updateMarkerShape();
    updateMarkerSize();
    updateBorderColor();
    updateFillColor();
}

void SeriesScatter::updateMarkerShape()
{
    auto shape = mCurrentSeries->markerShape();
    mMarkerShape->setCurrentIndex(shape); // shape对应可以作为index

    disconnect(mMarkerShape,SIGNAL(currentIndexChanged(int)),this,SLOT(changeShape(int)));
    connect(mMarkerShape,SIGNAL(currentIndexChanged(int)),this,SLOT(changeShape(int)));
}

void SeriesScatter::changeShape(int index)
{
    mCurrentSeries->setMarkerShape(QScatterSeries::MarkerShape(index));
}

void SeriesScatter::updateMarkerSize()
{
    mMarkerSize->setValue(mCurrentSeries->markerSize());

    disconnect(mMarkerSize,SIGNAL(valueChanged(double)),this,SLOT(changeSize(double)));
    connect(mMarkerSize,SIGNAL(valueChanged(double)),this,SLOT(changeSize(double)));
}

void SeriesScatter::changeSize(double size)
{
    mCurrentSeries->setMarkerSize(size);
}

void SeriesScatter::updateBorderColor()
{
    disconnect(mBorderColor,&QPushButton::clicked,this,&SeriesScatter::changeBorderColor);
    connect(mBorderColor,&QPushButton::clicked,this,&SeriesScatter::changeBorderColor);
}

void SeriesScatter::changeBorderColor()
{
    QColor oldcolor = mCurrentSeries->borderColor();
    QColorDialog * dlg = colorDialog(oldcolor);
    connect(dlg,static_cast<void (QColorDialog::*) (const QColor&)>(&QColorDialog::colorSelected)
           ,this,[=](const QColor& color){ mCurrentSeries->setBorderColor(color);
            //emit seriesColorChanged(mCurrentSeries);
    });
    dlg->exec(); delete dlg;
}

void SeriesScatter::updateFillColor()
{
    disconnect(mFillColor,&QPushButton::clicked,this,&SeriesScatter::changeFillColor);
    connect(mFillColor,&QPushButton::clicked,this,&SeriesScatter::changeFillColor);
}

void SeriesScatter::changeFillColor()
{
    QColor oldcolor = mCurrentSeries->color();
    QColorDialog * dlg = colorDialog(oldcolor);
    connect(dlg,static_cast<void (QColorDialog::*) (const QColor&)>(&QColorDialog::colorSelected)
           ,this,[=](const QColor& color){ mCurrentSeries->setColor(color);
            emit seriesColorChanged(mCurrentSeries);
    });
    dlg->exec(); delete dlg;
}

QColorDialog* SeriesScatter::colorDialog(const QColor&initColor)
{
    QColorDialog * dlg = new QColorDialog(initColor);
    dlg->setFixedSize(800,400);
    dlg->setWindowIcon(mIcon);
    dlg->setOptions(QColorDialog::ShowAlphaChannel|QColorDialog::DontUseNativeDialog);//QColorDialog::NoButtons|
    dlg->setWindowTitle(tr("设置颜色"));
    return dlg;
}
