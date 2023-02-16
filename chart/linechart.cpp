#include "linechart.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

LineChart::LineChart(QTableView * tableview,QWidget *parent)
    :QMainWindow(parent),mTableView(tableview)
{
    setFont(QFont("Times New Roman",12));
    setWindowIcon(QIcon(":/images/linechart.png"));

    mGenerator = new ChartDataGenerator;
    mChart = new Chart;
    mTableModel = static_cast<TableViewModel*>(mTableView->model());
    mLegend = new ChartShowLegend;
    mTip = new ChartShowTip;

    initChart(); // 注意先初始化,mChart具备标题序列和轴了再被后边所用
    mTip->setChart(mChart);
    mChartView = new ChartView(mChart,this);
    mToolBar = new LineChartBar(mTableView,mChartView,this);
    addToolBar(Qt::TopToolBarArea,mToolBar);
    mToolBox = new LineChartTool(mChart,this);
    mSplitter = new QSplitter(Qt::Horizontal);
    mSplitter->setChildrenCollapsible(false);
    mSplitter->setHandleWidth(10);
    mSplitter->addWidget(mChartView);
    mSplitter->addWidget(mToolBox);
    mSplitter->setStretchFactor(0,4); // 4:1的宽度
    mSplitter->setStretchFactor(1,1);
    setCentralWidget(mSplitter);

    // 工具栏的改变有2个方向可以触发,一个是外部表格导入,触发清空图表,一个是图表工具条的清空动作
    connect(mToolBar,&LineChartBar::associateCompeleted,mToolBox,&LineChartTool::associateCompeleted);
    connect(this,&LineChart::associateCompeleted,mToolBox,&LineChartTool::associateCompeleted);
    connect(this,&LineChart::tableChanged,mToolBar,&LineChartBar::tableChanged);
    connect(mToolBox,&LineChartTool::seriesColorChanged,mToolBar,&LineChartBar::seriesColorChanged);
    connect(mToolBox,&LineChartTool::seriesColorChanged,this,&LineChart::onSeriesColorChanged);
    connect(mToolBox,&LineChartTool::seriesRemoved,mToolBar,&LineChartBar::seriesRemoved);
    connect(mToolBox,&LineChartTool::seriesRemoved,this,&LineChart::onSeriesRemoved);
}

LineChart::~LineChart()
{
    if (mChartView != Q_NULLPTR)
    {
        mChart->removeAllSeries();

        delete mChartView;
        mChartView = Q_NULLPTR;
    }
}

void LineChart::closeChildrenWindows()
{
    mToolBox->closeChildrenWindows(); // 工具箱设置有子窗口,需要关闭
}

void LineChart::clearChart()
{
    if (mChart->axisX()) mChart->removeAxis(mChart->axisX());
    if (mChart->axisY()) mChart->removeAxis(mChart->axisY());
    if (mChart->series().count()) mChart->removeAllSeries();
    mSeriesXYColumn.clear(); // 这里存放的指针和对应的内容也要清空
    emit associateCompeleted(); // 文件导入调用clearChart清空图表还要通知曲线工具栏更改combo
}

void LineChart::initChart()
{
    // 任选1种初始化方式
    //initRandomChart();
    initMappingChart();
    mChart->setTitle("Line Chart Example");
    mLegend->mapping(mChart);
}

void LineChart::initRandomChart()
{
    QLineSeries *series = mGenerator->linechart(5000,100);//幅值5000配合对数坐标轴
    QValueAxis *axisX = mGenerator->axis(series->count()/10); // 默认100个值,间隔10
    QLogValueAxis *axisY = mGenerator->logaxis(4.0,"value"); // 对数底数4

    mChart->addSeries(series); // addSeries要先调用

    mChart->addAxis(axisX, Qt::AlignBottom); // 表添加轴确定好方向
    mChart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX); // 将轴和曲线关联
    series->attachAxis(axisY);// 这步操作切记必须在表先addAxis之后再附着到曲线上

    mTip->mapping(series);
}

void LineChart::initMappingChart()
{

    QLineSeries * series1 = mGenerator->lineseries("line1",Qt::red);
    QLineSeries * series2 = mGenerator->lineseries("line2",Qt::green);
    QLineSeries * series3 = mGenerator->lineseries("line3",Qt::yellow);

    addMapping(mTableModel ,series1,0,1);
    mChart->addSeries(series1);

    addMapping(mTableModel ,series2,2,3);
    mChart->addSeries(series2);

    addMapping(mTableModel ,series3,4,5);
    mChart->addSeries(series3);

    // 用于保存曲线和表格映射列的关系,方便曲线更改颜色后能找到映射列同步颜色
    mSeriesXYColumn[series1] = qMakePair<int,int>(0,1);
    mSeriesXYColumn[series2] = qMakePair<int,int>(2,3);
    mSeriesXYColumn[series3] = qMakePair<int,int>(4,5);

    mChart->createDefaultAxes();
    mChart->axisX()->setTitleText("data point");
    mChart->axisY()->setTitleText("value");
    mChart->axisX()->setTitleFont(QFont("Times New Roman",12));
    mChart->axisY()->setTitleFont(QFont("Times New Roman",12));

    mTip->mapping(series1);
    mTip->mapping(series2);
    mTip->mapping(series3);
}

void  LineChart::addMapping(TableViewModel*model, QXYSeries *series, int col1, int col2)
{
   QVXYModelMapper *mapper = new QVXYModelMapper(this);
   mapper->setXColumn(col1);
   mapper->setYColumn(col2);
   mapper->setSeries(series);
   mapper->setModel(model);

//   for(int y = 0;y < model->rowCount(); ++ y)
//   { // col是水平方向,表示x坐标, y坐标遍历行即可
//       model->addCellMapping(QRect(col1,y,1,1),series->color());
//       model->addCellMapping(QRect(col2,y,1,1),series->color());
//   }
   model->addColMapping(col1,series->color()); // 上方2行代码可用这2行替代
   model->addColMapping(col2,series->color());
}

void LineChart::onSeriesColorChanged(QLineSeries*series)
{
    if (mSeriesXYColumn.keys().contains(series))
    { // 修改颜色的曲线是这里初始化创建出来的曲线(因为可能是associatetable内创建出来的)
        auto xyCol = mSeriesXYColumn[series]; // 创建该曲线所使用的表格2列
        mTableModel->addColMapping(xyCol.first,series->color()); // 让表格对应的曲线两列数据跟随曲线当前颜色
        mTableModel->addColMapping(xyCol.second,series->color());
    }
}

void LineChart::onSeriesRemoved(QLineSeries *series)
{
    if (mSeriesXYColumn.keys().contains(series))
    {
        auto xyCol = mSeriesXYColumn[series];  // 唯一的区别是移除曲线是修改为白色
        mTableModel->addColMapping(xyCol.first,Qt::white);
        mTableModel->addColMapping(xyCol.second,Qt::white);
        mSeriesXYColumn.remove(series); // 已被移除不要再保留映射关系以免以后出现麻烦
    }
}
