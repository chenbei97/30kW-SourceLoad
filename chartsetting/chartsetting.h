#ifndef CHARTSETTING_H
#define CHARTSETTING_H

#include "charttitle.h"
#include "chartmargin.h"
#include "chartanimation.h"
#include "chartlegend.h"
#include "chartbackground.h"
#include "chartforeground.h"

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
