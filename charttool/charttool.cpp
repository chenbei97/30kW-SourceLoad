#include "charttool.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

ChartTool::ChartTool(QChart* chart,QWidget*parent):QToolBox(parent),mChart(chart)
{
    setFont(QFont("Times New Roman",12));
    setMinimumWidth(500);
    setMaximumWidth(800); // 只能在这设置限制ToolBox的宽度否则会挤压chartview
    setFrameShape(QFrame::NoFrame);

    // 图表工具箱有3页: 图表设置,曲线设置和坐标轴设置,图表设置所有曲线都是一样的
    mChartWidget = new ChartToolWidget(this);
    mChartWidget->setObjectName("ChartSettingPage");
    addItem(mChartWidget,QIcon(":/images/toolbox_chart.png"),tr("图表设置"));
    mChartSetting = new ChartSetting(mChart); // 图表工具栏的布局都是相同的,在ChartToolBox(this)就预设好
    mChartWidget->addLayout(mChartSetting);
    //mChartWidget->updateScrollArea(); // 只能调用1次,必须完成所有布局后调用,外部调用

    mSeriesWidget = new ChartToolWidget(this);
    mSeriesWidget->setObjectName("SeriesSettingPage");
    addItem(mSeriesWidget,QIcon(":/images/toolbox_series.png"),tr("曲线设置"));

    mAxisWidget = new ChartToolWidget(this);
    addItem(mAxisWidget,QIcon(":/images/toolbox_axis.png"),tr("坐标轴设置"));
    mAxisWidget ->setObjectName("AxisSettingPage");// 曲线和坐标轴灵活设置,通过addLayout/addGroupBox外部完成
}

void ChartTool::closeChildrenWindows()
{
    mChartSetting->closeChildrenWindows(); // 图表设置的子窗口都关闭
}
