#include "chartsetting.h"

ChartSetting::ChartSetting(QChart* chart):mChart(chart)
{
    setObjectName("ChartSettingsLayout");
    QIcon icon(":/images/toolbox_chart.png");
    mTitle = new ChartTitle(mChart,icon);
    mMargin = new ChartMargin(mChart,icon);
    mAnimation = new ChartAnimation(mChart,icon);
    mLegend = new ChartLegend(mChart,icon);
    mBackground = new ChartBackground(mChart,icon);
    mForeground = new ChartForeground(mChart,icon);
    addWidget(mTitle);
    addWidget(mMargin);
    addWidget(mAnimation);
    addWidget(mLegend);
    addWidget(mBackground);
    addWidget(mForeground);
    addWidget(new QGroupBox);
}

void ChartSetting::closeChildrenWindows()
{
    mBackground->closeChindrenWindows();
    mForeground->closeChindrenWindows();
}







