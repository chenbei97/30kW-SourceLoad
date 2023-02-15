#include "linechartbar.h"

LineChartBar::LineChartBar(QTableView*tableview,QChartView* chartview,QWidget*parent):
    ChartBar(tableview,chartview,parent)
{
    mAssociateTable = new LineAssociateTable(tableview,chartview);

    connect(mAssociatetableAct,&QAction::triggered,this,[=]{ mAssociateTable->exec();});// 不同基类不同
    connect(this,&LineChartBar::tableChanged,mAssociateTable,&LineAssociateTable::tableChanged);
    connect(this,SIGNAL(seriesColorChanged(QLineSeries*)),mAssociateTable,SIGNAL(seriesColorChanged(QLineSeries*)));
    connect(this,SIGNAL(seriesRemoved(QLineSeries*)),mAssociateTable,SIGNAL(seriesRemoved(QLineSeries*)));
    connect(mAssociateTable,&LineAssociateTable::associateCompeleted,this,&LineChartBar::associateCompeleted);
}
