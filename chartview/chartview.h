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
    bool mMouseMove; // 窗口是否在鼠标操作下移动
    QPoint mMouseLastPos; // 上次的鼠标位置(是相对窗口的位置)
    QGraphicsSimpleTextItem *mCoordX;
    QGraphicsSimpleTextItem *mCoordY;
};

#endif // CHARTVIEW_H
