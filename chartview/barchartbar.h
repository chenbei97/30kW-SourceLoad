#ifndef BARCHARTBAR_H
#define BARCHARTBAR_H

#include "chartbar.h"
#include "barassociatetable.h"

class BarChartBar : public ChartBar
{
    Q_OBJECT
public:
    explicit BarChartBar(QTableView*,QChartView*,QWidget*parent = Q_NULLPTR);
private:
    BarAssociateTable * mAssociateTable;
signals:
    void modeChanged(int,int);
    void seriesColorChanged(QBarSeries*,QColor,int);
};

#endif // BARCHARTBAR_H
