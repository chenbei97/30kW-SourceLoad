#include "linecharttool.h"

LineChartTool::LineChartTool(QChart* chart,QWidget*parent)
    :ChartTool(chart,parent)
{
    mChartWidget->updateScrollArea(); // 图表设置都相同,只需调用一次更新滑动区域

    mSeriesSetting = new LineSeriesSetting(mChart); // 通用设置
    mSeriesWidget->addLayout(mSeriesSetting);
    mSeriesWidget->updateScrollArea(); // 曲线和坐标轴设置不同,在这里调用

    // 针对折线图常用的XY轴有4套组合使用(根据情况可以在chartbar的associatetable内增加类型)
    // 1.AxisSettingValue和AxisSettingValue 2.AxisSettingValue和AxisSettingLog
    // 3.AxisSettingLog和AxisSettingValue 4.AxisSettingLog和AxisSettingLog
    mAxisSetting = new AxisSetting(mChart); // 根据传来的轴组合确定内部的4种组合之一
    mAxisWidget->addLayout(mAxisSetting);
    mAxisWidget->updateScrollArea();

    connect(this,&LineChartTool::associateCompeleted,mAxisSetting,&AxisSetting::associateCompeleted);
    connect(this,&LineChartTool::associateCompeleted,mSeriesSetting,&LineSeriesSetting::associateCompeleted);
    connect(mSeriesSetting,&LineSeriesSetting::seriesColorChanged,this,&LineChartTool::seriesColorChanged);
    connect(mSeriesSetting,&LineSeriesSetting::seriesRemoved,this,&LineChartTool::seriesRemoved);
}
