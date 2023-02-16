#ifndef CHARTSHOWTIP_H
#define CHARTSHOWTIP_H

#include <QGraphicsSimpleTextItem>
#include <QGraphicsRectItem>
#include <QtCharts>
using namespace  QtCharts;

class ChartShowTip : public QObject //Inherit or report an error
{
    Q_OBJECT
public:
    explicit ChartShowTip(QObject*parent=Q_NULLPTR);
    void setChart(QChart*);
    void mapping(QLineSeries*);
    void mapping(QScatterSeries*);
    void mapping(QBarSeries*);
    void mapping(QPieSeries*);
private:
    void showXYTip(QPointF point, bool state);
    void showBarTip(bool status, int index, QBarSet *barset);
    void showPieTip(QPieSlice *slice, bool state);
    QChart* mChart;
    QBrush mPieBrush;
    QGraphicsSimpleTextItem * mCoordTip;
    QGraphicsRectItem * mCoordRect;
};

#endif // CHARTSHOWTIP_H
