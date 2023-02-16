#ifndef CHARTCOORDTIP_H
#define CHARTCOORDTIP_H

#include <QGraphicsItem>
#include <QPainter>
#include <QtCharts>
using namespace QtCharts;

class ChartCoordTip : public QObject,public QGraphicsItem
{
    Q_OBJECT
public:
    explicit ChartCoordTip(QChart *chart);
    enum {Type = UserType +1};
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);
    int type() const;
    void setAnchor(QPointF);
    void setText(const QString &text);
    void updateGeometry();
private:
    QChart * mChart;
    QPointF mAnchor;
    QRectF mRect;
    QString mText;
    QRectF mTextRect;
    QFont mTextFont;
};

#endif // CHARTCOORDTIP_H
