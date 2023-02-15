#ifndef BACKGROUNDAREAGRADIENT_H
#define BACKGROUNDAREAGRADIENT_H

#include <QFormLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QtCharts>
#include "lineargradientparamwidget.h"
#include "radialgradientparamwidget.h"
#include "conicalgradientparamwidget.h"
using namespace QtCharts;

class BackgroundAreaGradient : public QGroupBox
{
    Q_OBJECT
public:
    explicit BackgroundAreaGradient(QChart*,const QIcon&,QWidget *parent = Q_NULLPTR);
    void closeChildrenWindows();
    QVector<QPair<qreal,QColor>> defaultColors() const;
    QGradient::InterpolationMode selectedGradientInterpolation() const;
    QGradient::CoordinateMode selectedGradientCoordinate() const;
    QGradient::Spread selectedGradientSpread() const;
    QGradient::Type selectedGradientType() const;
private:
    QChart * mChart;
    QIcon mIcon;
    QGradient::InterpolationMode mGradientInterpolation;
    QGradient::CoordinateMode mGradientCoordinate;
    QGradient::Spread mGradientSpread;
    QGradient::Type mGradientType;
    LinearGradientParamWidget * mLinearGradientParamWidget;
    RadialGradientParamWidget * mRadialGradientParamWidget;
    ConicalGradientParamWidget * mConicalGradientParamWidget;
signals:
    void interpolationChanged(QGradient::InterpolationMode);
    void coordinateChanged(QGradient::CoordinateMode);
    void spreadChanged(QGradient::Spread);
    void gradientTypeChanged(QGradient::Type);
    void gradientParams(QVector<QPair<qreal,QColor>>,QGradient::Type);
    void radialGradientParams(RadialGradientParamWidget::RadialParam,bool=true,bool=true);
    void conicalGradientParams(ConicalGradientParamWidget::ConicalParam,bool=true,bool=true);
};

#endif // BACKGROUNDAREAGRADIENT_H
