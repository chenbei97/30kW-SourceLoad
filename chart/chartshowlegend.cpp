#include "chartshowlegend.h"

ChartShowLegend::ChartShowLegend(QObject*parent):
    QObject(parent),mShowLegend(true)
{

}

void ChartShowLegend::mapping(QChart* chart)
{
    const auto markers = chart->legend()->markers(); // 初始化曲线后再连接
    for (QLegendMarker *marker : markers) { // 利用返回的图例标记的clicked信号实现点击图例变暗并隐藏曲线的效果
        QObject::disconnect(marker, &QLegendMarker::clicked,this, &ChartShowLegend::legendMarkerClicked); // 先断开以防重复连接
        QObject::connect(marker, &QLegendMarker::clicked, this, &ChartShowLegend::legendMarkerClicked);
    }
}

void ChartShowLegend::legendMarkerClicked() // 这个函数不能放在chart.cpp连接因为那个时候还没有曲线,图例尚未创建出来,必须先初始化曲线后再连接
{
    QLegendMarker* marker = qobject_cast<QLegendMarker*> (sender()); // 发送clicked的图例标记
    switch (marker->type())
    {
        case QLegendMarker::LegendMarkerTypeXY: // QXYLegendMarker=直线+样条曲线+散点系列的效果
        {
                //qDebug()<<marker->series()->name();
                //bool isVisible = marker->series()->isVisible();
                //marker->series()->setVisible(isVisible);

                mShowLegend = !mShowLegend; // 2023,1.31,改用这2行代码的原因是 图表添加新的曲线后就会导致原有的曲线点击图例显示隐藏效果失灵
                marker->series()->setVisible(mShowLegend); // 原因每次isVisible=marker->series()->isVisible()都是false,但是setVisible确实起作用,所以不清楚为何返回不变的原因
                // 2023/2.1 经过对mapping的修改不再使用原来的string,rect映射而是改用int,color让单元格和颜色一一对应
                // 发现这个问题得到了解决,上边2行代码又可以使用
                marker->setVisible(true);
                qreal alpha = 1.0;
                if (!marker->series()->isVisible())
                    alpha = 0.5;

                QColor color;
                QBrush brush = marker->labelBrush(); // 图例的文字
                color = brush.color();
                color.setAlphaF(alpha);
                brush.setColor(color);
                marker->setLabelBrush(brush);

                brush = marker->brush(); //图例标记的背景
                color = brush.color();
                color.setAlphaF(alpha);
                brush.setColor(color);
                marker->setBrush(brush);

                QPen pen = marker->pen(); // 图例标记的方框
                color = pen.color();
                color.setAlphaF(alpha);
                pen.setColor(color);
                marker->setPen(pen);
                break;
        }
        case QLegendMarker::LegendMarkerTypeBar:
        {
                    QBarSeries * series = static_cast<QBarSeries*>(marker->series());
                    QList<QBarSet*> barsets = series->barSets();
                    mShowLegend = !mShowLegend;

                    foreach(QBarSet* set,barsets)
                    {
                        if (set->label() == marker->label())
                        {
                            auto color1 = set->labelColor();
                            auto color2 =set->color();
                            auto color3 = set->borderColor();
                            if (mShowLegend) {
                                color1.setAlphaF(1.0);
                                color2.setAlphaF(1.0);
                                color3.setAlphaF(1.0);
                            }
                            else {
                                color1.setAlphaF(0.); // 用透明度来实现显示隐藏的效果
                                color2.setAlphaF(0.);
                                color3.setAlphaF(0.);
                            }
                            set->setLabelColor(color1);
                            set->setColor(color2);
                            set->setBorderColor(color3);
                        }
                    }
                    break;
        }
    case QLegendMarker::LegendMarkerTypePie:
    {
            auto series = static_cast<QPieSeries*>(marker->series());
            auto slices = series->slices();
            mShowLegend = !mShowLegend;
            foreach(QPieSlice* slice,slices)
            {
                if (marker->label() == slice->label())
                {
                    slice->setExploded(!mShowLegend);
                    slice->setLabelVisible(mShowLegend);
                }
            }
    }

    default: break;
    }
}
