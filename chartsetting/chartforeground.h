#ifndef CHARTFOREGROUND_H
#define CHARTFOREGROUND_H

#include <QCheckBox>
#include "chartbox.h"
#include "foregroundarea.h"
#include "foregroundborder.h"

class ChartForeground : public ChartBox
{
    Q_OBJECT
public:
    explicit ChartForeground(QChart * chart, const QIcon&icon, QWidget*parent = Q_NULLPTR);
    void closeChindrenWindows();
private:
    ForegroundArea * mForegroundArea;
    ForegroundBorder * mForegroundBorder;
};

#endif // CHARTFOREGROUND_H
