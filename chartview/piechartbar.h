#ifndef PIECHARTBAR_H
#define PIECHARTBAR_H

#include "chartbar.h"
#include "pieassociatetable.h"

class PieChartBar : public ChartBar
{
    Q_OBJECT
public:
    explicit PieChartBar(QTableView*,QChartView*,QWidget*parent = Q_NULLPTR);
private:
    PieAssociateTable * mAssociateTable;
signals:
    void modeChanged(int,int);
    void seriesColorChanged(QColor,int);
};

#endif // PIECHARTBAR_H
