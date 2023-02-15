#ifndef PIECHART_H
#define PIECHART_H

#include <QMainWindow>
#include "chart.h"
#include "chartshowtip.h"
#include "chartshowlegend.h"
#include "chartdatagenerator.h"
#include "../chartview/chartview.h"
#include "../tool/tableviewmodel.h"
#include "../charttool/piecharttool.h"
#include "../chartview/piechartbar.h"

class PieChart : public QMainWindow
{
    Q_OBJECT
public:
    explicit PieChart(QTableView *,QWidget *parent = Q_NULLPTR);
    void clearChart();
    void closeChildrenWindows();
private:
    void initChart();
    QSplitter * mSplitter;
    Chart * mChart;
    QPieSeries * mCurrentSeries;
    TableViewModel * mTableModel;
    ChartView * mChartView;
    QTableView * mTableView;
    ChartDataGenerator * mGenerator;
    ChartShowLegend * mLegend;
    ChartShowTip * mTip;
    PieChartBar * mToolBar;
    PieChartTool * mToolBox;
signals:
    void tableChanged();
    void associateCompeleted();
};

#endif // PIECHART_H
