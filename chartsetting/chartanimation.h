#ifndef CHARTANIMATION_H
#define CHARTANIMATION_H

#include <QDoubleSpinBox>
#include <QComboBox>
#include <QLabel>
#include <QEasingCurve>
#include <QGridLayout>
#include "chartbox.h"

class ChartAnimation : public ChartBox
{
    Q_OBJECT
public:
    explicit ChartAnimation(QChart *chart,const QIcon&,QWidget*parent = Q_NULLPTR);
private:
    QComboBox * mAnimationOptions;
    QComboBox * mAnimationCurves;
    QComboBox * mAnimationThemes;
    QDoubleSpinBox * mAnimationDuration;
};
#endif // CHARTANIMATION_H
