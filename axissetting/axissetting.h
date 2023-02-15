#ifndef AXISSETTING_H
#define AXISSETTING_H

#include "axisinfo.h"
#include "axistitle.h"
#include "axislabel.h"
#include "axisshade.h"
#include "axisline.h"
#include "axisgrid.h"
#include "axisvalue.h"
#include "axislog.h"
#include "axistime.h"
#include "axisbarcategory.h"

class AxisSetting : public QVBoxLayout
{
    Q_OBJECT
public:
    explicit AxisSetting(QChart*);
private:
    void disconnectAllConnections();
    QChart * mChart;
    QIcon mIcon;
    QAbstractAxis * mCurrentAxis;
    QRadioButton * mAxisX;
    QRadioButton * mAxisY;
    QGroupBox * mWhichAxis;
    void initWhichAxis();
    AxisInfo * mAxisInfo;
    AxisTitle * mAxisTitle;
    AxisLabel * mAxisLabel;
    AxisShade * mAxisShade;
    AxisLine * mAxisLine;
    AxisGrid * mAxisGrid;
    AxisValue * mAxisValue;
    AxisLog * mAxisLog;
    AxisTime * mAxisTime;
    AxisBarCategory * mAxisBar;
signals:
    void associateCompeleted();
};
#endif // AXISSETTING_H
