#ifndef PIECHARTTOOL_H
#define PIECHARTTOOL_H

#include "charttool.h"
#include "../seriessetting/pieseriessetting.h"

class PieChartTool : public ChartTool
{
    Q_OBJECT
public:
    explicit PieChartTool(QChart*,QWidget*parent=Q_NULLPTR);
private:
    PieSeriesSetting * mSeriesSetting;
signals:
    void modeChanged(int,int);
    void seriesColorChanged(QColor,int);
};

#endif // PIECHARTTOOL_H
