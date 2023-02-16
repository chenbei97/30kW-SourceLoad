#include "pieseriessetting.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

PieSeriesSetting::PieSeriesSetting(QChart*chart):mChart(chart)
{
    mCurrentSeries = static_cast<QPieSeries*>(mChart->series().at(0));
    mInfo = new SeriesInfo(chart);
    mPie = new SeriesPie(chart);
    mPieSlice = new SeriesPieSlice(chart);
    addWidget(mInfo);
    addWidget(mPie);
    addWidget(mPieSlice);

    // 导入文件，清空图表和关联表格都会发出该信号，其中导入和清空需要调用disconnectAllConnections
    connect(this,&PieSeriesSetting::associateCompeleted,mInfo,&SeriesInfo::updateInfo);
    connect(this,&PieSeriesSetting::associateCompeleted,mPie,&SeriesPie::updatePie);
    connect(this,&PieSeriesSetting::associateCompeleted,mPieSlice,&SeriesPieSlice::updatePieSlice);
    connect(this,&PieSeriesSetting::modeChanged,mPieSlice,&SeriesPieSlice::updateAssociateMode);//告知当前曲线是什么模式生成的
    // 饼图系列的颜色改变后去改变对应的表格颜色
    connect(mPieSlice,&SeriesPieSlice::seriesColorChanged,this,&PieSeriesSetting::seriesColorChanged);
}
