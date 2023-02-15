#ifndef CHARTSETTING_H
#define CHARTSETTING_H

#include "charttitle.h"
#include "chartmargin.h"
#include "chartanimation.h"
#include "chartlegend.h"
#include "chartbackground.h"
#include "chartforeground.h"

// chartsetting没有子类,所有曲线类型均可用
class ChartSetting : public QVBoxLayout
{
    Q_OBJECT
public:
    explicit ChartSetting(QChart* chart);
    void closeChildrenWindows();
private:
    QChart * mChart;
    ChartTitle * mTitle;
    ChartMargin * mMargin;
    ChartAnimation * mAnimation;
    ChartLegend * mLegend;
    ChartBackground * mBackground;
    ChartForeground * mForeground;
};
#endif // CHARTSETTING_H
