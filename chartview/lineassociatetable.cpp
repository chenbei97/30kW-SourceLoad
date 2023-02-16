#include "lineassociatetable.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

LineAssociateTable::LineAssociateTable(QTableView*tableview,QChartView*chartview,QWidget*parent):
    QDialog(parent),mTableView(tableview),mChartView(chartview)
{
    mLegend = new ChartShowLegend;
    mTip = new ChartShowTip;
    mTip->setChart(mChartView->chart());
    mTableModel = static_cast<TableViewModel*>(mTableView->model());
    QVBoxLayout * lay = new QVBoxLayout(this);

    mMode = new AssociateXYMode(mTableModel);
    mSeries = new AssociateXYSeries;
    mSeries->setMarkerSizeVisible(false); // 折线图不能设置标记大小
    mAxis = new AssociateXYAxis;

    mOkBtn = new QPushButton(tr("关联"));
    QHBoxLayout * lay1 = new QHBoxLayout;
    lay1->addStretch();
    lay1->addWidget(mOkBtn);

    lay->addWidget(mMode);
    lay->addWidget(mSeries);
    lay->addWidget(mAxis);
    lay->addLayout(lay1);

    setFont(QFont("Times New Roman",12));
    setWindowIcon(QIcon(":/images/associatetable.png"));
    resize(QSize(800,300));

    initConnections();
}

void LineAssociateTable::initConnections()
{
    connect(mMode,&AssociateXYMode::changeMode,mAxis,&AssociateXYAxis::setHorizontalEnabled);
    connect(mOkBtn,&QPushButton::clicked,this,&LineAssociateTable::onOkBtn);

    connect(this,&LineAssociateTable::tableChanged,this,[=]{ // 导入文件后表格model发生了更新
        disconnect(mOkBtn,&QPushButton::clicked,this,&LineAssociateTable::onOkBtn);
        mTableModel = static_cast<TableViewModel*>(mTableView->model()); //更新model再连
        connect(mOkBtn,&QPushButton::clicked,this,&LineAssociateTable::onOkBtn);

        mMode->adjustRange(); //调整spinbox和表格列数一致
    });

    connect(this,static_cast<void (LineAssociateTable::*)(QLineSeries*)>(&LineAssociateTable::seriesColorChanged),
            this,[=](QLineSeries*series){
            if (mSeriesXYColumn.keys().contains(series))
            { // 说明修改的是这里创建出来的series,有可能是linechart.cpp初始化创建的那3个
                auto cols = mSeriesXYColumn[series];
                mTableModel->addColMapping(cols.first,series->color());
                if (cols.second < 0) return; // 说明是单列映射
                mTableModel->addColMapping(cols.second,series->color());
            }
    });
    connect(this,static_cast<void (LineAssociateTable::*)(QLineSeries*)>(&LineAssociateTable::seriesRemoved),
            this,[=](QLineSeries*series){
            if (mSeriesXYColumn.keys().contains(series))
            {
                auto cols = mSeriesXYColumn[series];
                mTableModel->addColMapping(cols.first,Qt::white);
                if (cols.second < 0) return; // 说明是单列映射
                mTableModel->addColMapping(cols.second,Qt::white);
                mSeriesXYColumn.remove(series);
            }
    });
}

void LineAssociateTable::onOkBtn()
{
    bool isSingle = mMode->isSingle();
    if (isSingle) singleMapping();
    else doubleMapping();
    mLegend->mapping(mChartView->chart());

    emit associateCompeleted(); // 通知曲线工具栏进行界面更新
    accept();
}

void LineAssociateTable::singleMapping()
{
    int col = mMode->singleCol();

    QLineSeries * series = new QLineSeries;
    series->setPointLabelsFormat("(@xPoint, @yPoint)"); //加个()
    QPen pen = series->pen();
    pen.setWidth(mSeries->linewidth()); // 提供给用户选择
    pen.setColor(mSeries->linecolor());
    series->setPen(pen);

    mTableModel->addColMapping(col,mSeries->linecolor());
    mSeriesXYColumn[series] = qMakePair<int,int>(col,-1);

    QString name = mTableModel->horizontalHeaderLabels()[col];
    if (name.toInt()) series->setName("col"+name); // 如果是数字1,2,3..就加上前缀
    else series->setName(name); //否则列名称作为序列名字

    QVector<QVariant> data = mTableModel->colData(col); // 这列的数据
    QPointF point;
    for(int x = 0; x <data.count(); ++x){
        point.setX((qreal)x);point.setY(data[x].toDouble());
        *series  << point;
    }

    mChartView->chart()->addSeries(series);
    QValueAxis * axisX = new QValueAxis; // 单映射是自动生成线性坐标轴,对数坐标轴没有意义
    mChartView->chart()->addAxis(axisX,Qt::AlignBottom);
    series->attachAxis(axisX);
    setAxis(series,Qt::AlignLeft);
    mTip->mapping(series);
}

void LineAssociateTable::doubleMapping()
{
    QPoint point = mMode->doubleCols();
    int xCol = point.x();
    int yCol = point.y();

    QLineSeries * series = new QLineSeries;
    series->setPointLabelsFormat("(@xPoint, @yPoint)"); //加个()
    QPen pen = series->pen();
    pen.setWidth(mSeries->linewidth()); // 提供给用户选择
    pen.setColor(mSeries->linecolor());
    series->setPen(pen);

    mTableModel->addDoubleColMapping(series,xCol,yCol); // 曲线先设置颜色再关联表格否则不体现
    mSeriesXYColumn[series] = qMakePair<int,int>(xCol,yCol);

    QString name1 = mTableModel->horizontalHeaderLabels()[xCol];
    QString name2 = mTableModel->horizontalHeaderLabels()[yCol];
    if (name1.toInt()) name1 = "col"+name1;
    if (name2.toInt()) name2 = "col"+name2;
    series->setName(name1+"-"+name2);

    //chart->removeAllSeries();
    mChartView->chart()->addSeries(series);
    setAxis(series,Qt::AlignBottom);
    setAxis(series,Qt::AlignLeft);
    mTip->mapping(series);
}

void LineAssociateTable::setAxis(QLineSeries *series,Qt::Alignment alignment)
{
    QValueAxis * valueAxis;
    QLogValueAxis * logAxis;
    QAbstractAxis * axis;
    int type;
    if (alignment == Qt::AlignBottom)
    {
        axis = mChartView->chart()->axisX();
        type = mAxis->axisType().x();
    }
    else{
        axis = mChartView->chart()->axisY();
        type = mAxis->axisType().y();
    }

    if (axis) mChartView->chart()->removeAxis(axis); // 首次设置还没有坐标轴

    switch (type) {
        case AssociateXYAxis::Value:
                valueAxis = new QValueAxis;
                mChartView->chart()->addAxis(valueAxis,alignment);
                series->attachAxis(valueAxis);
                //qDebug()<<"hor value";
                break;
          case AssociateXYAxis::Log:
                logAxis = new QLogValueAxis;
                logAxis->setBase(2.); //默认
                auto base = alignment==Qt::AlignBottom?
                            mAxis->axisBase().x():mAxis->axisBase().y();
                if (base != 1.0) logAxis->setBase(base);
                mChartView->chart()->addAxis(logAxis,alignment);
                series->attachAxis(logAxis);
                //qDebug()<<"hor log base = "<<logAxisX->base();
                break;
    }
}

