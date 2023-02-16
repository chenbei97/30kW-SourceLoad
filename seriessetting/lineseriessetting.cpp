#include "lineseriessetting.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

LineSeriesSetting::LineSeriesSetting(QChart*chart):mChart(chart)
{
    mGenericSetting = new SeriesSetting(chart);
    mXYSetting = new SeriesXY(chart);

    addLayout(mGenericSetting);
    addWidget(mXYSetting);

    // 导入文件，清空图表和关联表格都会发出该信号，其中导入和清空需要调用disconnectAllConnections
    connect(this,&LineSeriesSetting::associateCompeleted,mGenericSetting,&SeriesSetting::updateGenericSetting);
    connect(this,&LineSeriesSetting::associateCompeleted,mXYSetting,&SeriesXY::updateXY);

    connect(mXYSetting,static_cast<void (SeriesXY::*)(QXYSeries*)>(&SeriesXY::seriesColorChanged),
            this,[=](QXYSeries*series){emit seriesColorChanged(static_cast<QLineSeries*>(series));});

    connect(mGenericSetting,static_cast<void (SeriesSetting::*)(QAbstractSeries*)>(&SeriesSetting::seriesRemoved),
            this,[=](QAbstractSeries *series){ emit seriesRemoved(static_cast<QLineSeries*>(series));});
    connect(mGenericSetting,static_cast<void (SeriesSetting::*)(int)>(&SeriesSetting::seriesToggled), // 通用设置中切换曲线后去调整特有设置
            this,[=](int index){ // 折线图只允许QLineSeries一种类型
            mCurrentSeries = static_cast<QLineSeries*>(mChart->series().at(index)); // 更新当前曲线
            mXYSetting->setCurrentSeries(mCurrentSeries,index);//切换曲线进行更新
    });
    connect(mGenericSetting,static_cast<void (SeriesSetting::*)(int)>(&SeriesSetting::seriesRemoved),
            this,[=](int index){ // 同理更新,index能够确保是有效的,如果没有曲线了不会再连接到这里,最小为0,最大count-1
            mCurrentSeries = static_cast<QLineSeries*>(mChart->series().at(index)); //移除最后1个曲线的话会自动前移
            mXYSetting->setCurrentSeries(mCurrentSeries,index);
    });
}
