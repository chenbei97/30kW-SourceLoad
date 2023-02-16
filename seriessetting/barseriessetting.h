#ifndef BARSERIESSETTING_H
#define BARSERIESSETTING_H

#include "seriessetting.h"
#include "seriesinfo.h"
#include "seriesbar.h"
#include "seriesbarset.h"

class BarSeriesSetting : public QVBoxLayout
{
    Q_OBJECT
public:
    explicit BarSeriesSetting(QChart*);
private:
    QChart * mChart;
    QBarSeries * mCurrentSeries;
    SeriesInfo * mInfo;
    SeriesBar * mBar;
    SeriesBarSet * mBarSet;
signals:
    void associateCompeleted();
    void modeChanged(int,int);
    void seriesColorChanged(QBarSeries*,QColor,int);
};
#endif // BARSERIESSETTING_H
