#ifndef AXISVALUE_H
#define AXISVALUE_H

#include <QGroupBox>
#include <QFormLayout>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QChart>
#include <QLabel>
#include <QValueAxis>
#include <QLineEdit>
#include <QPushButton>
using namespace QtCharts;

class AxisValue: public QGroupBox
{
    Q_OBJECT
public:
    explicit AxisValue(QChart*, QWidget*parent = Q_NULLPTR);
    void setCurrentAxis(QValueAxis*);
    QValueAxis* currentAxis() const;
    void updateState();
    void disconnectAllConnections();
private:
    QChart * mChart;
    QValueAxis * mCurrentAxis;
    QDoubleSpinBox * mMinRange;
    QDoubleSpinBox * mMaxRange;
    QSpinBox * mTickCount;
    QSpinBox * mMinorTickCount;
    QLineEdit * mLabelFormatEdit;
    QPushButton * mLabelFormatBtn;
    void updateMinState();
    void updateMaxState();
    void updateTickCountState();
    void updateMinorTickCountState();
    void updateLabelFormatState();
private slots:
    void changeMin(double);
    void changeMax(double);
    void changeTickCount(int);
    void changeMinorTickCount(int);
    void changeLabelFormat();
};

#endif // AXISVALUE_H
