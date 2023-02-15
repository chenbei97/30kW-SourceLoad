#ifndef AXISINFO_H
#define AXISINFO_H

#include "axisbox.h"

class AxisInfo : public AxisBox
{
    Q_OBJECT
public:
    AxisInfo(QChart*, QAbstractAxis*,const QIcon&, QWidget*parent = Q_NULLPTR);
    void disconnectAllConnections() override;
private:
    void updateState() override;
    QCheckBox * mAxisVisible;
    QCheckBox * mAxisReverse;
    QLabel * mAxisAlignment;
    QLabel * mAxisOrientation;
    QLabel * mAxisType;
    void updateVisibilityState();
    void updateReversedState();
    void updateAlignmentState();
    void updateOrientationState();
    void updateTypeState();
    void changeVisibility(int);
    void changeReversed(int);
};

#endif // AXISINFO_H
