#include "scatterchart.h"

ScatterChart::ScatterChart(QTableView * tableview,QWidget *parent)
    :QMainWindow(parent),mTableView(tableview)
{
    setFont(QFont("Times New Roman",12));
    setWindowIcon(QIcon(":/images/scatterchart.png"));

    mGenerator = new ChartDataGenerator;
    mChart = new Chart;
    mTableModel = static_cast<TableViewModel*>(mTableView->model());
    mLegend = new ChartShowLegend;
    mTip = new ChartShowTip;

    initChart();
    mTip->setChart(mChart);
    mChartView = new ChartView(mChart,this);
    mToolBar = new ScatterChartBar(mTableView,mChartView,this);
    addToolBar(Qt::TopToolBarArea,mToolBar);

    mToolBox = new ScatterChartTool(mChart,this);
    mSplitter = new QSplitter(Qt::Horizontal);
    mSplitter->setChildrenCollapsible(false);
    mSplitter->setHandleWidth(10);
    mSplitter->addWidget(mChartView);
    mSplitter->addWidget(mToolBox);
    mSplitter->setStretchFactor(0,4); // 4:1的宽度
    mSplitter->setStretchFactor(1,1);
    setCentralWidget(mSplitter);

    connect(mToolBar,&ScatterChartBar::associateCompeleted,mToolBox,&ScatterChartTool::associateCompeleted);
    connect(this,&ScatterChart::associateCompeleted,mToolBox,&ScatterChartTool::associateCompeleted);
    connect(this,&ScatterChart::tableChanged,mToolBar,&ScatterChartBar::tableChanged);
    connect(mToolBox,&ScatterChartTool::seriesColorChanged,mToolBar,&ScatterChartBar::seriesColorChanged);
    connect(mToolBox,&ScatterChartTool::seriesColorChanged,this,&ScatterChart::onSeriesColorChanged);
    connect(mToolBox,&ScatterChartTool::seriesRemoved,mToolBar,&ScatterChartBar::seriesRemoved);
    connect(mToolBox,&ScatterChartTool::seriesRemoved,this,&ScatterChart::onSeriesRemoved);
}

void ScatterChart::initChart()
{
    QScatterSeries * series1 = mGenerator->scatterseries("scatter1",Qt::red);
    QScatterSeries * series2 = mGenerator->scatterseries("scatter2",Qt::green);
    QScatterSeries * series3 = mGenerator->scatterseries("scatter3",Qt::yellow);

    addMapping(mTableModel ,series1,0,1);
    mChart->addSeries(series1);

    addMapping(mTableModel ,series2,2,3);
    mChart->addSeries(series2);

    addMapping(mTableModel ,series3,4,5);
    mChart->addSeries(series3);

    mSeriesXYColumn[series1] = qMakePair<int,int>(0,1);
    mSeriesXYColumn[series2] = qMakePair<int,int>(2,3);
    mSeriesXYColumn[series3] = qMakePair<int,int>(4,5);

    mChart->createDefaultAxes();

    mTip->mapping(series1);
    mTip->mapping(series2);
    mTip->mapping(series3);

    mChart->setTitle("Scatter Chart Example");
    mLegend->mapping(mChart);
}

void  ScatterChart::addMapping(TableViewModel*model, QXYSeries *series, int col1, int col2)
{
   QVXYModelMapper *mapper = new QVXYModelMapper(this);
   mapper->setXColumn(col1);
   mapper->setYColumn(col2);
   mapper->setSeries(series);
   mapper->setModel(model);

   model->addColMapping(col1,series->color());
   model->addColMapping(col2,series->color());
}

void ScatterChart::closeChildrenWindows()
{
    mToolBox->closeChildrenWindows();
}

ScatterChart::~ScatterChart()
{
    if (mChartView != Q_NULLPTR)
    {
        mChart->removeAllSeries();

        delete mChartView;
        mChartView = Q_NULLPTR;
    }
}

void ScatterChart::clearChart()
{
    if (mChart->axisX()) mChart->removeAxis(mChart->axisX());
    if (mChart->axisY()) mChart->removeAxis(mChart->axisY());
    if (mChart->series().count()) mChart->removeAllSeries();
    mSeriesXYColumn.clear(); // 这里存放的指针和对应的内容也要清空
    emit associateCompeleted(); // 文件导入调用clearChart清空图表还要通知曲线工具栏更改combo
}

void ScatterChart::onSeriesColorChanged(QScatterSeries*series)
{
    if (mSeriesXYColumn.keys().contains(series))
    { // 修改颜色的曲线是这里初始化创建出来的曲线(因为可能是associatetable内创建出来的)
        auto xyCol = mSeriesXYColumn[series]; // 创建该曲线所使用的表格2列
        mTableModel->addColMapping(xyCol.first,series->color()); // 让表格对应的曲线两列数据跟随曲线当前颜色
        mTableModel->addColMapping(xyCol.second,series->color());
    }
}

void ScatterChart::onSeriesRemoved(QScatterSeries *series)
{
    if (mSeriesXYColumn.keys().contains(series))
    {
        auto xyCol = mSeriesXYColumn[series];  // 唯一的区别是移除曲线是修改为白色
        mTableModel->addColMapping(xyCol.first,Qt::white);
        mTableModel->addColMapping(xyCol.second,Qt::white);
        mSeriesXYColumn.remove(series); // 已被移除不要再保留映射关系以免以后出现麻烦
    }
}
