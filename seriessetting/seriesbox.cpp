#include "seriesbox.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

SeriesBox::SeriesBox(QChart *chart,QWidget*parent):
    QGroupBox(parent),mChart(chart),mCurrentSeriesId(0)
{
    mCurrentSeries = chart->series().at(0);
    mIcon.addFile(":/images/toolbox_series.png");
    setFont(QFont("Times New Roman",12));
    setMinimumWidth(450); // 和charttool.cpp保持一致稍小一些
}

void SeriesBox::setChart(QChart *c)
{
    mChart = c;
}

QChart* SeriesBox::chart() const
{
    return  mChart;
}

void SeriesBox::setCurrentSeries(QAbstractSeries *series,int id)
{
    mCurrentSeries = series;
    mCurrentSeriesId = id;
    updateState(); // 切换曲线后更新状态
}

QAbstractSeries* SeriesBox::currentSeries() const
{
    return  mCurrentSeries;
}

int SeriesBox::currentSeriesId() const
{
    return mCurrentSeriesId;
}

void SeriesBox::setCurrentSeriesId(int id)
{
    mCurrentSeriesId = id;
}
