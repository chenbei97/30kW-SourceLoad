#include  "scattercharttool.h"

ScatterChartTool::ScatterChartTool(QChart* chart,QWidget*parent)
    :ChartTool(chart,parent)
{
    mChartWidget->updateScrollArea();
    mSeriesSetting = new ScatterSeriesSetting(mChart);
    mSeriesWidget->addLayout(mSeriesSetting);
    mSeriesWidget->updateScrollArea();
    mAxisSetting = new AxisSetting(mChart);
    mAxisWidget->addLayout(mAxisSetting);
    mAxisWidget->updateScrollArea();

    connect(this,&ScatterChartTool::associateCompeleted,mAxisSetting,&AxisSetting::associateCompeleted);
    connect(this,&ScatterChartTool::associateCompeleted,mSeriesSetting,&ScatterSeriesSetting::associateCompeleted);
    connect(mSeriesSetting,&ScatterSeriesSetting::seriesColorChanged,this,&ScatterChartTool::seriesColorChanged);
    connect(mSeriesSetting,&ScatterSeriesSetting::seriesRemoved,this,&ScatterChartTool::seriesRemoved);
}
