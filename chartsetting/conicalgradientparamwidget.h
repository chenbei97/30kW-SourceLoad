#ifndef CONICALGRADIENTPARAMWIDGET_H
#define CONICALGRADIENTPARAMWIDGET_H

#include <QCheckBox>
#include <QFormLayout>
#include <QPair>
#include "gradientparamwidget.h"
#include "gradientparamspinbox.h"

class ConicalGradientParamWidget : public GradientParamWidget
{
    Q_OBJECT
public:
    typedef QPair<QPointF,qreal> ConicalParam;
    explicit ConicalGradientParamWidget(QWidget*parent = Q_NULLPTR);
private:
    bool mIsRealTime;
    ConicalParam mConicalParam;
signals:
    void conicalGradientParams(ConicalParam,bool isRealTime=true,bool isBtn=true);
};
#endif // CONICALGRADIENTPARAMWIDGET_H
