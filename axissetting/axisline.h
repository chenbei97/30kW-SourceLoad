#ifndef AXISLINE_H
#define AXISLINE_H

#include "axisbox.h"

class AxisLine : public AxisBox
{
    Q_OBJECT
public:
    explicit AxisLine(QChart*, QAbstractAxis*,const QIcon&, QWidget*parent = Q_NULLPTR);
    void disconnectAllConnections() override;
private:
    void updateState() override;
    QCheckBox * mAxisLineVisibility;
    QPushButton * mAxisLineColor;
    QSpinBox * mAxisLineWidth;
    void updateVisibilityState();
    void updaterColorState();
    void updatePenWidthState();
private slots:
    void changeVisibility(int);
    void changeColor();
    void changePenWidth(int);
};

#endif // AXISLINE_H
