#ifndef CHARTBAR_H
#define CHARTBAR_H

#include <QToolBar>
#include <QTableView>
#include "../tool/tableviewmodel.h"
#include "screenshoot.h"

class ChartBar : public QToolBar
{
    Q_OBJECT
public:
    explicit ChartBar(QTableView*,QChartView*,QWidget*parent = Q_NULLPTR);
protected:
    QAction * mAssociatetableAct;
    QTableView * mTableView;
    QChartView * mChartView;
    QChart * mChart;
    ScreenShoot * mScreenShot;
signals:
    void associateCompeleted();
    void tableChanged();
};

#endif // CHARTBAR_H
