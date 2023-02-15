#include "barseriessetting.h"

BarSeriesSetting::BarSeriesSetting(QChart*chart):mChart(chart)
{
    mCurrentSeries = static_cast<QBarSeries*>(mChart->series().at(0));
    mInfo = new SeriesInfo(chart);
    mBar = new SeriesBar(chart);
    mBarSet = new SeriesBarSet(chart);
    addWidget(mInfo);
    addWidget(mBar);
    addWidget(mBarSet);

    // 导入文件，清空图表和关联表格都会发出该信号，其中导入和清空需要调用disconnectAllConnections
    connect(this,&BarSeriesSetting::associateCompeleted,mInfo,&SeriesInfo::updateInfo);
    connect(this,&BarSeriesSetting::associateCompeleted,mBar,&SeriesBar::updateBar);
    connect(this,&BarSeriesSetting::associateCompeleted,mBarSet,&SeriesBarSet::updateBarSet);
    connect(this,&BarSeriesSetting::modeChanged,mBarSet,&SeriesBarSet::updateAssociateMode);//告知当前曲线是什么模式生成的
    // 柱状图系列的颜色改变后去改变对应的表格颜色
    connect(mBarSet,&SeriesBarSet::seriesColorChanged,this,&BarSeriesSetting::seriesColorChanged);
}
