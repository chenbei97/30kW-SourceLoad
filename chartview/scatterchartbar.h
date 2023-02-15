#ifndef SCATTERCHARTBAR_H
#define SCATTERCHARTBAR_H

#include "chartbar.h"
#include "scatterassociatetable.h"

class ScatterChartBar : public ChartBar
{
    Q_OBJECT
public:
    explicit ScatterChartBar(QTableView*,QChartView*,QWidget*parent = Q_NULLPTR);
private:
    ScatterAssociateTable * mAssociateTable;
signals:
    void seriesColorChanged(QScatterSeries*);
    void seriesRemoved(QScatterSeries*);
};
#endif // SCATTERCHARTBAR_H
