#ifndef LINESERIESSETTING_H
#define LINESERIESSETTING_H

#include "seriessetting.h"
#include "seriesxy.h"

class LineSeriesSetting : public QVBoxLayout
{
    Q_OBJECT
public:
    explicit LineSeriesSetting(QChart*);
private:
    QChart * mChart;
    QLineSeries * mCurrentSeries;
    SeriesSetting * mGenericSetting; // 使用组合
    SeriesXY * mXYSetting; // 折线图使用XY即可
signals:
    void associateCompeleted();
    void seriesColorChanged(QLineSeries*);
    void seriesRemoved(QLineSeries*);
};

#endif // LINESERIESSETTING_H
