#ifndef CHART_H
#define CHART_H

#include <QtCharts/QChart>
#include <QEasingCurve>
#include <QLegendMarker>
using namespace QtCharts;

class Chart : public QChart
{
    Q_OBJECT
public:
    explicit Chart(QGraphicsItem *parent = Q_NULLPTR, Qt::WindowFlags = Qt::Widget);
};

#endif // CHART_H
