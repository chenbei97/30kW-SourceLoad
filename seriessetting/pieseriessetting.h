#ifndef PIESERIESSETTING_H
#define PIESERIESSETTING_H

#include "seriessetting.h"
#include "seriesinfo.h"
#include "seriespie.h"
#include "seriespieslice.h"

class PieSeriesSetting : public QVBoxLayout
{
    Q_OBJECT
public:
    explicit PieSeriesSetting(QChart*);
private:
    QChart * mChart;
    QPieSeries * mCurrentSeries;
    SeriesInfo * mInfo;
    SeriesPie * mPie;
    SeriesPieSlice * mPieSlice;
signals:
    void associateCompeleted();
    void modeChanged(int,int);
    void seriesColorChanged(QColor,int);
};

#endif // PIESERIESSETTING_H
