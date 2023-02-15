#include "barchartbar.h"

BarChartBar::BarChartBar(QTableView*tableview,QChartView* chartview,QWidget*parent):
    ChartBar(tableview,chartview,parent)
{
    mAssociateTable = new BarAssociateTable(tableview,chartview);

    connect(mAssociatetableAct,&QAction::triggered,this,[=]{ mAssociateTable->exec();});
    connect(this,&BarChartBar::tableChanged,mAssociateTable,&BarAssociateTable::tableChanged);
    connect(this,&BarChartBar::seriesColorChanged,mAssociateTable,&BarAssociateTable::seriesColorChanged);
    connect(mAssociateTable,&BarAssociateTable::associateCompeleted,this,&BarChartBar::associateCompeleted);
    connect(mAssociateTable,&BarAssociateTable::modeChanged,this,&BarChartBar::modeChanged);//关联模式改变通知工具栏
}
