#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QApplication>
#include <QChart>
#include <QChartView>
using namespace QtCharts;

class ChartView : public QChartView
{
    Q_OBJECT
public:
    explicit ChartView(QChart *chart, QWidget *parent = Q_NULLPTR);
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent*event);
    void resizeEvent(QResizeEvent *event);
private:
    QChart * mChart;
    bool mMouseMove; // Whether the window moves under mouse operation
    QPoint mMouseLastPos; // Last mouse position (relative to the window)
    QGraphicsSimpleTextItem *mCoordX;
    QGraphicsSimpleTextItem *mCoordY;
};

#endif // CHARTVIEW_H
