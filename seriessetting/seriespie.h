#ifndef SERIESPIE_H
#define SERIESPIE_H

#include <QGroupBox>
#include <QIcon>
#include <QComboBox>
#include <QtCharts>
#include <QFormLayout>
#include <QCheckBox>
#include <QLabel>
#include "../chartsetting/gradientparamspinbox.h"

class SeriesPie : public QGroupBox
{
    Q_OBJECT
public:
    explicit SeriesPie(QChart *,QWidget* = Q_NULLPTR);
    void setCurrentSeries(QPieSeries*);
    QPieSeries* currentSeries() const;
    void updatePie();
private:
    void updateState();
    void disconnectAllConnections();
    QChart * mChart;
    QPieSeries * mCurrentSeries;
    QIcon mIcon;

    void updateSumState();
    void updateLabelPositionState();
    void updateStartAngleState();
    void updateEndAngleState();
    void updateHoleSizeState();
    void updatePieSizeState();
    void updateHPositionState();
    void updateVPositionState();

    QLabel * mSum;
    QComboBox * mLabelPosition;
    GradientParamSpinBox * mStartAngle;
    GradientParamSpinBox * mEndAngle;
    QDoubleSpinBox * mHoleSize;
    QDoubleSpinBox * mPieSize;
    QDoubleSpinBox * mHPosition;
    QDoubleSpinBox * mVPosition;
private slots:
    void changeLabelPosition(int);
    void changeStartAngle(double);
    void changeEndAngle(double);
    void changeHoleSize(double);
    void changePieSize(double);
    void changeHPosition(double);
    void changeVPosition(double);
};

#endif // SERIESPIE_H
