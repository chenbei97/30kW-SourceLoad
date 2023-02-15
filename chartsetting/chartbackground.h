#ifndef CHARTBACKGROUND_H
#define CHARTBACKGROUND_H

#include "chartbox.h"
#include "backgroundborder.h"
#include "backgroundarea.h"

class ChartBackground : public ChartBox
{
    Q_OBJECT
public:
    explicit ChartBackground(QChart * chart, const QIcon&icon, QWidget*parent = Q_NULLPTR);
    void closeChindrenWindows();
private:
    BackgroundArea * mBackgroundArea;
    BackgroundBorder * mBackgroundBorder;
};
#endif // CHARTBACKGROUND_H
