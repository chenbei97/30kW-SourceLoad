#include "scatterassociatetable.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

ScatterAssociateTable::ScatterAssociateTable(QTableView*tableview,QChartView*chartview,QWidget*parent):
    QDialog(parent),mTableView(tableview),mChartView(chartview)
{
    mLegend = new ChartShowLegend;
    mTip = new ChartShowTip;
    mTip->setChart(mChartView->chart());
    mTableModel = static_cast<TableViewModel*>(mTableView->model());
    QVBoxLayout * lay = new QVBoxLayout(this);

    mMode = new AssociateXYMode(mTableModel);
    mSeries = new AssociateXYSeries;
    mSeries->setLineWidthVisible(false);
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

void ScatterAssociateTable::initConnections()
{
    connect(mMode,&AssociateXYMode::changeMode,mAxis,&AssociateXYAxis::setHorizontalEnabled);
    connect(mOkBtn,&QPushButton::clicked,this,&ScatterAssociateTable::onOkBtn);

    connect(this,&ScatterAssociateTable::tableChanged,this,[=]{
        disconnect(mOkBtn,&QPushButton::clicked,this,&ScatterAssociateTable::onOkBtn);
        mTableModel = static_cast<TableViewModel*>(mTableView->model());
        connect(mOkBtn,&QPushButton::clicked,this,&ScatterAssociateTable::onOkBtn);
        mMode->adjustRange();
    });

    connect(this,static_cast<void (ScatterAssociateTable::*)(QScatterSeries*)>(&ScatterAssociateTable::seriesColorChanged),
            this,[=](QScatterSeries*series){
            if (mSeriesXYColumn.keys().contains(series))
            {
                auto cols = mSeriesXYColumn[series];
                mTableModel->addColMapping(cols.first,series->color());
                if (cols.second < 0) return;
                mTableModel->addColMapping(cols.second,series->color());
            }
    });
    connect(this,static_cast<void (ScatterAssociateTable::*)(QScatterSeries*)>(&ScatterAssociateTable::seriesRemoved),
            this,[=](QScatterSeries*series){
            if (mSeriesXYColumn.keys().contains(series))
            {
                auto cols = mSeriesXYColumn[series];
                mTableModel->addColMapping(cols.first,Qt::white);
                if (cols.second < 0) return;
                mTableModel->addColMapping(cols.second,Qt::white);
                mSeriesXYColumn.remove(series);
            }
    });
}

void ScatterAssociateTable::onOkBtn()
{
    bool isSingle = mMode->isSingle();
    if (isSingle) singleMapping();
    else doubleMapping();
    mLegend->mapping(mChartView->chart());

    emit associateCompeleted();
    accept();
}

void ScatterAssociateTable::singleMapping()
{
    int col = mMode->singleCol();

    QScatterSeries * series = new QScatterSeries;
    series->setPointLabelsFormat("(@xPoint, @yPoint)");
    series->setBorderColor(Qt::white);
    series->setColor(mSeries->linecolor());
    series->setMarkerSize(mSeries->markersize());

    mTableModel->addColMapping(col,mSeries->linecolor());
    mSeriesXYColumn[series] = qMakePair<int,int>(col,-1);

    QString name = mTableModel->horizontalHeaderLabels()[col];
    if (name.toInt()) series->setName("col"+name);
    else series->setName(name);

    QVector<QVariant> data = mTableModel->colData(col);
    QPointF point;
    for(int x = 0; x <data.count(); ++x){
        point.setX((qreal)x);point.setY(data[x].toDouble());
        *series  << point;
    }

    mChartView->chart()->addSeries(series);
    QValueAxis * axisX = new QValueAxis;
    mChartView->chart()->addAxis(axisX,Qt::AlignBottom);
    series->attachAxis(axisX);
    setAxis(series,Qt::AlignLeft);
    mTip->mapping(series);
}

void ScatterAssociateTable::doubleMapping()
{
    QPoint point = mMode->doubleCols();
    int xCol = point.x();
    int yCol = point.y();

    QScatterSeries * series = new QScatterSeries;
    series->setPointLabelsFormat("(@xPoint, @yPoint)");
    series->setBorderColor(Qt::white);
    series->setColor(mSeries->linecolor());
    series->setMarkerSize(mSeries->markersize());

    mTableModel->addDoubleColMapping(series,xCol,yCol);
    mSeriesXYColumn[series] = qMakePair<int,int>(xCol,yCol);

    QString name1 = mTableModel->horizontalHeaderLabels()[xCol];
    QString name2 = mTableModel->horizontalHeaderLabels()[yCol];
    if (name1.toInt()) name1 = "col"+name1;
    if (name2.toInt()) name2 = "col"+name2;
    series->setName(name1+"-"+name2);

    mChartView->chart()->addSeries(series);
    setAxis(series,Qt::AlignBottom);
    setAxis(series,Qt::AlignLeft);
    mTip->mapping(series);
}

void ScatterAssociateTable::setAxis(QScatterSeries *series,Qt::Alignment alignment)
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

    if (axis) mChartView->chart()->removeAxis(axis);

    switch (type) {
        case AssociateXYAxis::Value:
                valueAxis = new QValueAxis;
                mChartView->chart()->addAxis(valueAxis,alignment);
                series->attachAxis(valueAxis);
                break;
          case AssociateXYAxis::Log:
                logAxis = new QLogValueAxis;
                logAxis->setBase(2.);
                auto base = alignment==Qt::AlignBottom?
                            mAxis->axisBase().x():mAxis->axisBase().y();
                if (base != 1.0) logAxis->setBase(base);
                mChartView->chart()->addAxis(logAxis,alignment);
                series->attachAxis(logAxis);
                break;
    }
}
