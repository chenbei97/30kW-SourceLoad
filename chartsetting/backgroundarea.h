#ifndef BACKGROUNDAREA_H
#define BACKGROUNDAREA_H

#include <QRadioButton>
#include <QCheckBox>
#include <QSpinBox>
#include <QColorDialog>
#include <QListWidget>
#include "backgroundareagradient.h"

class BackgroundArea : public QWidget
{
    Q_OBJECT
public:
    explicit BackgroundArea(QChart*,const QIcon&,QWidget *parent = Q_NULLPTR);
    void closeChildrenWindows();
    void closeEvent(QCloseEvent *event) override;
private:
    void initGradientBox();
    void initGradientDefaultParams();
    void initMainLayout();
    void initBrushStyleComboBox(QComboBox*);
    QChart * mChart;
    QIcon mIcon;
    BackgroundAreaGradient * mBackgroundAreaGradient;
    QGradient::InterpolationMode mGradientInterpolation;
    QGradient::CoordinateMode mGradientCoordinate;
    QGradient::Spread mGradientSpread;
    QGradient::Type mGradientType;
    QLinearGradient  mLinearGradient;
    QRadialGradient mRadialGradient;
    QConicalGradient mConicalGradient;
private : signals:
    void gradientTypeChanged();
};
#endif // BACKGROUNDAREA_H
