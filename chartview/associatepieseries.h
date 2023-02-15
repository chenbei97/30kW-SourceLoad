#ifndef ASSOCIATEPIESERIES_H
#define ASSOCIATEPIESERIES_H

#include <QGroupBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QFormLayout>

class AssociatePieSeries : public QGroupBox
{
    Q_OBJECT
public:
    explicit AssociatePieSeries(QWidget*parent = Q_NULLPTR);
    int labelPosition() const;
    double holeSize() const;
private:
    QDoubleSpinBox * mHoleSizeSpin;
    QComboBox * mLabelPositionCombo;
};

#endif // ASSOCIATEPIESERIES_H
