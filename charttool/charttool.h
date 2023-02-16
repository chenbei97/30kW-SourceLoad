#ifndef CHARTTOOL_H
#define CHARTTOOL_H

#include <QToolBox>
#include "charttoolwidget.h"
#include "../chartsetting/chartsetting.h"

class ChartTool : public QToolBox
{
    Q_OBJECT
public:
    explicit ChartTool(QChart* chart,QWidget*parent=Q_NULLPTR);
    void closeChildrenWindows();
    void updateChart();
protected:
    ChartToolWidget * mChartWidget;
    ChartToolWidget * mSeriesWidget;
    ChartToolWidget * mAxisWidget;
    QChart * mChart;
private:
    ChartSetting * mChartSetting;
signals:
    void associateCompeleted();
};
#endif // CHARTTOOL_H
