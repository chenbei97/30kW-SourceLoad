#include "barchart.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

BarChart::BarChart(QTableView * tableview,QWidget *parent)
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
    mToolBar = new BarChartBar(tableview,mChartView,this);
    addToolBar(Qt::TopToolBarArea,mToolBar);
    mToolBox = new BarChartTool(mChart,this);
    mSplitter = new QSplitter(Qt::Horizontal);
    mSplitter->setChildrenCollapsible(false);
    mSplitter->setHandleWidth(10);
    mSplitter->addWidget(mChartView);
    mSplitter->addWidget(mToolBox);
    setCentralWidget(mSplitter);

    connect(mToolBar,&BarChartBar::associateCompeleted,mToolBox,&BarChartTool::associateCompeleted);
    connect(mToolBar,&BarChartBar::modeChanged,mToolBox,&BarChartTool::modeChanged);//来自关联表格方向
    connect(this,&BarChart::modeChanged,mToolBox,&BarChartTool::modeChanged);// 来自初始化方向
    connect(this,&BarChart::associateCompeleted,mToolBox,&BarChartTool::associateCompeleted);
    connect(this,&BarChart::tableChanged,mToolBar,&BarChartBar::tableChanged);

    connect(mToolBox,&BarChartTool::seriesColorChanged,mToolBar,&BarChartBar::seriesColorChanged);// 通知关联表格方向
    connect(mToolBox,&BarChartTool::seriesColorChanged,this,&BarChart::onSeriesColorChanged); // 工具栏颜色通知初始化方向
}

void BarChart::initChart()
{
    QList <QBarSet*> * barsets = new QList<QBarSet*>;
    QList<QVector<QVariant>> colDatas;
    for(int col = 0; col <mTableModel->columnCount(); ++col)
    {
        barsets->append(new QBarSet("col "+mTableModel->horizontalHeaderLabel(col)));
        barsets->at(col)->setColor(mTableModel->cellColor(0,col));
        barsets->at(col)->setBorderColor(mTableModel->cellColor(0,col));
        barsets->at(col)->setLabelColor(mTableModel->cellColor(0,col));
        colDatas.append(mTableModel->colData(col));
    }

    for (int c = 0 ; c < barsets->count(); ++c)
    {
        for (int r = 0; r < colDatas.at(0).count(); ++r)
            barsets->at(c)->append(colDatas.at(c).at(r).toDouble());
    }

    QBarSeries * series = new QBarSeries;
    series->setBarWidth(2.);
    series->setLabelsVisible(true); // 要显示,否则barset的label不显示
    series->setLabelsPosition(QAbstractBarSeries::LabelsOutsideEnd);
    series->append(*barsets);

    mTip->mapping(series);
    mChart->addSeries(series);
    mChart->createDefaultAxes();//默认1,2,3..作为横轴
    mChart->setTitle("Bar Chart Example");
    mLegend->mapping(mChart);
    mCurrentSeries = series;

    emit modeChanged(3,0); //起始列0,整个区域
}

void BarChart::closeChildrenWindows()
{
    mToolBox->closeChildrenWindows();
}

void BarChart::clearChart()
{
    if (mChart->axisX()) mChart->removeAxis(mChart->axisX());
    if (mChart->axisY()) mChart->removeAxis(mChart->axisY());
    if (mChart->series().count()) mChart->removeAllSeries();
    emit associateCompeleted(); // 文件导入调用clearChart清空图表还要通知曲线工具栏更改combo
}

void BarChart::onSeriesColorChanged(QBarSeries*series,QColor color, int col)
{ // 用户在修改初始化生成的表格,colRegion模式,返回的就是该系列对应的表格列数
    if (series == mCurrentSeries) // 说明是初始化方向的设置
        mTableModel->addColMapping(col,color);
}
