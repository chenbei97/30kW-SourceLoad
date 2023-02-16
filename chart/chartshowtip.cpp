#include "chartshowtip.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

ChartShowTip::ChartShowTip(QObject*parent):QObject(parent)
{
    mCoordRect = Q_NULLPTR;
    mCoordTip = Q_NULLPTR;
}

void ChartShowTip::setChart(QChart *chart)
{
    mCoordTip = new QGraphicsSimpleTextItem(chart);
    mCoordTip->setPen(QPen(QColor("#8B008B")));
    mCoordTip->setFont(QFont("Times New Roman",14));
    mCoordTip->setZValue(10);
    mCoordRect = new QGraphicsRectItem(chart);
    mCoordRect->setZValue(9);
    mCoordRect->setBrush(QColor("#FFFFE0"));
    mCoordRect->setPen(QPen(QColor(0,0,0)));
    mChart = chart;
}

void ChartShowTip::mapping(QLineSeries * series)
{
    connect(series, &QLineSeries::hovered, this, &ChartShowTip::showXYTip);
}

void ChartShowTip::mapping(QScatterSeries*series)
{
    connect(series, &QScatterSeries::hovered, this, &ChartShowTip::showXYTip);
}

void ChartShowTip::mapping(QBarSeries*series)
{
    connect(series, &QBarSeries::hovered, this, &ChartShowTip::showBarTip);
}

void ChartShowTip::mapping(QPieSeries*series)
{
    connect(series, &QPieSeries::hovered, this, &ChartShowTip::showPieTip);
}


void ChartShowTip::showXYTip(QPointF point, bool state)
{
    if (state)
    {
        QPointF pos = mChart->mapToPosition(point);
        mCoordTip->setPos(pos); // 把曲线的坐标值映射为改点在全局的位置
        mCoordTip->setText(QString("[%1, %2] ").arg(point.x()).arg(point.y()));
        mCoordRect->setRect(QRectF(pos,QSizeF(213.,38.))); //长213高38的矩形刚好把文字放进去
        mCoordTip->show();
        mCoordRect->show();
    }
    else {
        mCoordTip->hide();
        mCoordRect->hide();
    }
}

void ChartShowTip::showBarTip(bool status, int index, QBarSet *barset)
{
    Q_UNUSED(index);
    auto font = barset->labelFont();
    if (status)
    {
        font.setBold(true);
        font.setPointSize(20);
        barset->setLabelFont(font);
    }
    else {
        font.setBold(false);
        font.setPointSize(12);
        barset->setLabelFont(font);
    }
}

void ChartShowTip::showPieTip(QPieSlice *slice, bool state)
{
    slice->setExplodeDistanceFactor(0.2);
    slice->setExploded(state);
    if (state) {
        QBrush brush = slice->brush();
        mPieBrush = brush;
        brush.setColor(brush.color().lighter());
        slice->setBrush(brush);
    } else {
        slice->setBrush(mPieBrush);
    }
}
