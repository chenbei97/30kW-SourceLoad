#ifndef SERIESBAR_H
#define SERIESBAR_H

#include <QGroupBox>
#include <QColorDialog>
#include <QIcon>
#include <QComboBox>
#include <QtCharts>
#include <QFormLayout>
#include <QPushButton>
#include "../chartsetting/gradientparamspinbox.h"

using namespace QtCharts;

class SeriesBar : public QGroupBox
{
    Q_OBJECT
public:
    explicit SeriesBar(QChart *,QWidget* = Q_NULLPTR);
    void setCurrentSeries(QBarSeries*);
    QBarSeries* currentSeries() const;
    void updateBar();
private:
    void updateState();
    void disconnectAllConnections();
    QChart * mChart;
    QBarSeries * mCurrentSeries;
    QIcon mIcon;
    QColorDialog* colorDialog(const QColor&);
    QFontDialog* fontDialog(const QFont&);

    void updateWidthState();
    void updateVisibleState();
    void updateAngleState();
    void updatePositionState();
    void updateFontState();
    void updateBorderColorState();

    QDoubleSpinBox * mBarWidth;
    QCheckBox * mLabelVisible;
    GradientParamSpinBox * mLabelAngle;
    QComboBox * mLabelPosition;
    QPushButton * mLabelFont;
    QPushButton * mBorderColor;
private slots:
    void changeWidth(double);
    void changeVisible(int);
    void changeAngle(double);
    void changePosition(int);
    void changeFont();
    void changeBorderColor();
};

#endif // SERIESBAR_H
