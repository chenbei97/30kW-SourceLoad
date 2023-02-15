#include "piechart.h"

PieChart::PieChart(QTableView * tableview,QWidget *parent)
    :QMainWindow(parent),mTableView(tableview)
{
    mCurrentSeries = nullptr;
    setFont(QFont("Times New Roman",12));
    setWindowIcon(QIcon(":/images/barchart.png"));
    mGenerator = new ChartDataGenerator;
    mChart = new Chart;
    mTableModel = static_cast<TableViewModel*>(mTableView->model());
    mLegend = new ChartShowLegend;
    mTip = new ChartShowTip;

    initChart();
    mChartView = new ChartView(mChart,this);
    mToolBar = new PieChartBar(tableview,mChartView,this);
    addToolBar(Qt::TopToolBarArea,mToolBar);
    mToolBox = new PieChartTool(mChart,this);
    mSplitter = new QSplitter(Qt::Horizontal);
    mSplitter->setChildrenCollapsible(false);
    mSplitter->setHandleWidth(10);
    mSplitter->addWidget(mChartView);
    mSplitter->addWidget(mToolBox);
    setCentralWidget(mSplitter);

    connect(mToolBar,&PieChartBar::associateCompeleted,mToolBox,&PieChartTool::associateCompeleted);
    connect(mToolBar,&PieChartBar::modeChanged,mToolBox,&PieChartTool::modeChanged);
    connect(this,&PieChart::associateCompeleted,mToolBox,&PieChartTool::associateCompeleted);
    connect(this,&PieChart::tableChanged,mToolBar,&PieChartBar::tableChanged);
    connect(mToolBox,&PieChartTool::seriesColorChanged,mToolBar,&PieChartBar::seriesColorChanged);

}

void PieChart::initChart()
{ // 这里不映射到表格了,没有太大意义
    QPieSeries * series = mGenerator->pie();
    mTip->mapping(series);
    mChart->addSeries(series);
    mChart->createDefaultAxes();//饼图没有X和Y轴,创建也无用
    mChart->setTitle("Pie Chart Example");
    mLegend->mapping(mChart);
    mCurrentSeries = series;
}

void PieChart::clearChart()
{
    if (mChart->axisX()) mChart->removeAxis(mChart->axisX());
    if (mChart->axisY()) mChart->removeAxis(mChart->axisY());
    if (mChart->series().count()) mChart->removeAllSeries();
    emit associateCompeleted(); // 文件导入调用clearChart清空图表还要通知曲线工具栏更改combo
}

void PieChart::closeChildrenWindows()
{
    mToolBox->closeChildrenWindows();
}



















