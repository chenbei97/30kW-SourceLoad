#ifndef SCATTERSERIESSETTING_H
#define SCATTERSERIESSETTING_H

#include "seriessetting.h"
#include "seriesxy.h"
#include "seriesscatter.h"

class ScatterSeriesSetting : public QVBoxLayout
{
    Q_OBJECT
public:
    explicit ScatterSeriesSetting(QChart*);
private:
    QChart * mChart;
    QScatterSeries * mCurrentSeries;
    SeriesSetting * mGenericSetting;
    SeriesXY * mXYSetting;
    SeriesScatter * mScatterSetting;
signals:
    void associateCompeleted();
    void seriesColorChanged(QScatterSeries*);
    void seriesRemoved(QScatterSeries*);
};

#endif // SCATTERSERIESSETTING_H
