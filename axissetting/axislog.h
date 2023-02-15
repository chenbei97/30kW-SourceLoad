#ifndef AXISLOG_H
#define AXISLOG_H

#include <QGroupBox>
#include <QFormLayout>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QChart>
#include <QLabel>
#include <QValueAxis>
#include <QLineEdit>
#include <QPushButton>
#include <QLogValueAxis>
#include <QSpacerItem>
using namespace QtCharts;

class AxisLog : public QGroupBox
{
    Q_OBJECT
public:
    explicit AxisLog(QChart*, QWidget*parent = Q_NULLPTR);
    void setCurrentAxis(QLogValueAxis*);
    QLogValueAxis* currentAxis() const;
    void disconnectAllConnections();
private:
    void updateState();
    QChart * mChart;
    QLogValueAxis * mCurrentAxis;
    QDoubleSpinBox * mBase;
    QDoubleSpinBox * mMinRange;
    QDoubleSpinBox * mMaxRange;
    QLabel * mTickCount;
    QSpinBox * mMinorTickCount;
    QLineEdit * mLabelFormatEdit;
    QPushButton * mLabelFormatBtn;
    void updateBaseState();
    void updateMinState();
    void updateMaxState();
    void updateTickCountState();
    void updateMinorTickCountState();
    void updateLabelFormatState();
private slots:
    void changeBase(double);
    void changeMin(double);
    void changeMax(double);
    void changeMinorTickCount(int);
    void changeLabelFormat();

};

#endif // AXISLOG_H
