#ifndef AXISTIME_H
#define AXISTIME_H

#include <QGroupBox>
#include <QDateTimeAxis>
#include <QDateTimeEdit>
#include <QChart>
#include <QFormLayout>
#include <QSpinBox>
#include <QComboBox>
#include <QDebug>
using namespace QtCharts;

class AxisTime : public QGroupBox
{
    Q_OBJECT
public:
    explicit AxisTime(QChart*, QWidget*parent = Q_NULLPTR);
    void setCurrentAxis(QDateTimeAxis*);
    QDateTimeAxis* currentAxis() const;
    void updateState();
    void disconnectAllConnections();
private:
    QChart * mChart;
    QDateTimeAxis * mCurrentAxis;
    QDateTimeEdit * mMinTimeEdit;
    QDateTimeEdit * mMaxTimeEdit;
    QSpinBox * mTickCount;
    QComboBox * mDateFormat;
    QComboBox * mTimeFormat;
    QString mDateFormatString;
    QString mTimeFormatString;
    void updateMinState();
    void updateMaxState();
    void updateTickCountState();
    void updateDateFormatState();
    void updateTimeFormatState();
private slots:
     void changeMin(const QDateTime&);
     void changeMax(const QDateTime&);
     void changeTickCount(int);
     void changeDateFormat(const QString&);
     void changeTimeFormat(const QString&);
};

#endif // AXISTIME_H
