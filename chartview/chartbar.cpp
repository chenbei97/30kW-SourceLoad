#include "chartbar.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

ChartBar::ChartBar(QTableView*tableview,QChartView* chartview,QWidget*parent):
    QToolBar(parent),mTableView(tableview),mChartView(chartview),mChart(chartview->chart())
{
    mScreenShot = new ScreenShoot(chartview);
    QAction * zoombig = new QAction(QIcon(":/images/zoombig.bmp"),tr("放大"));
    QAction * zoomsmall = new QAction(QIcon(":/images/zoomsmall.bmp"),tr("缩小"));
    QAction * zoom = new QAction(QIcon(":/images/zoom.bmp"),tr("还原"));
    QAction * screenshot = new QAction(QIcon(":/images/screenshot.png"),tr("截图"));
    mAssociatetableAct = new QAction(QIcon(":/images/associatetable.png"),tr("关联表格"));
    QAction * clearChart = new QAction(QIcon(":/images/clearChart.png"),tr("清空"));

    addAction(zoombig);
    addAction(zoomsmall);
    addAction(zoom);
    addAction(screenshot);
    addAction(clearChart);
    addAction(mAssociatetableAct);

    setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    connect(zoombig,&QAction::triggered,this,[=]{mChart->zoom(1.2);});
    connect(zoomsmall,&QAction::triggered,this,[=]{mChart->zoom(0.8);});
    connect(zoom,&QAction::triggered,this,[=]{mChart->zoomReset();});
    connect(clearChart,&QAction::triggered,this,[=]{
        if (mChart->axisX()) mChart->removeAxis(mChart->axisX());
        if (mChart->axisY()) mChart->removeAxis(mChart->axisY());
        if (mChart->series().count()) mChart->removeAllSeries();
        auto model = static_cast<TableViewModel*>(mTableView->model());
        model->clearMapping();
        mTableView->update(QRect(0,0,model->columnCount(),model->rowCount()));
        emit associateCompeleted(); // 借助此信号来更新工具栏
    });
    connect(screenshot,&QAction::triggered,this,[=]{mScreenShot->exec();});
}
