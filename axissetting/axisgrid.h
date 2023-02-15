#ifndef AXISGRID_H
#define AXISGRID_H

#include "axisbox.h"

class AxisGrid : public AxisBox
{
    Q_OBJECT
public:
    explicit AxisGrid(QChart*, QAbstractAxis*,const QIcon&, QWidget*parent = Q_NULLPTR);
    void disconnectAllConnections() override;
private:
    void updateState() override;
    QCheckBox * mAxisGridVisibility;
    QPushButton * mAxisGridColor;
    QSpinBox * mAxisGridWidth;
    QCheckBox * mAxisMinorGridVisibility;
    QPushButton * mAxisMinorGridColor;
    QSpinBox * mAxisMinorGridWidth;
    void updateVisibilityState();
    void updateColorState();
    void updatePenWidthState();
private slots:
    void changeMasterVisibility(int);
    void changeMinorVisibility(int);
    void changeMasterColor();
    void changeMinorColor();
    void changeMasterPenWidth(int);
    void changeMinorPenWidth(int);
};
#endif // AXISGRID_H
