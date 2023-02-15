#include "piecharttool.h"

PieChartTool::PieChartTool(QChart* chart,QWidget*parent)
    :ChartTool(chart,parent)
{
    mChartWidget->updateScrollArea();

    QHBoxLayout * lay = new QHBoxLayout;
    lay->addWidget(new QLabel(tr("饼图没有坐标轴,无需进行设置~")));
    mAxisWidget->addLayout(lay);
    mAxisWidget->updateScrollArea();

    mSeriesSetting = new PieSeriesSetting(mChart);
    mSeriesWidget->addLayout(mSeriesSetting);
    mSeriesWidget->updateScrollArea();

    connect(this,&PieChartTool::associateCompeleted,mSeriesSetting,&PieSeriesSetting::associateCompeleted);
    connect(this,&PieChartTool::modeChanged,mSeriesSetting,&PieSeriesSetting::modeChanged);
    connect(mSeriesSetting,&PieSeriesSetting::seriesColorChanged,this,&PieChartTool::seriesColorChanged);
}
