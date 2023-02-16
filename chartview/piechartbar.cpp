#include "piechartbar.h"

PieChartBar::PieChartBar(QTableView*tableview,QChartView* chartview,QWidget*parent):
    ChartBar(tableview,chartview,parent)
{
    mAssociateTable = new PieAssociateTable(tableview,chartview);

    connect(mAssociatetableAct,&QAction::triggered,this,[=]{ mAssociateTable->exec();});
    connect(this,&PieChartBar::tableChanged,mAssociateTable,&PieAssociateTable::tableChanged);
    connect(this,&PieChartBar::seriesColorChanged,mAssociateTable,&PieAssociateTable::seriesColorChanged);
    connect(mAssociateTable,&PieAssociateTable::associateCompeleted,this,&PieChartBar::associateCompeleted);
    connect(mAssociateTable,&PieAssociateTable::modeChanged,this,&PieChartBar::modeChanged);
}
