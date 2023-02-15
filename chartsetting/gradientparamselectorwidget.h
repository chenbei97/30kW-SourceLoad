#ifndef GRADIENTPARAMSELECTORWIDGET_H
#define GRADIENTPARAMSELECTORWIDGET_H

#include "gradientparamselector.h"

class GradientParamSelectorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GradientParamSelectorWidget(QWidget *parent = Q_NULLPTR);
    QVector<QPair<qreal,QColor>> selectedColors() const;
    QVector<QPair<qreal,QColor>> defaultColors() const;
    void setGrayStyle();
    void setDefaultStyle();
private:
    int mBarCount;
    QList<QColor> mDefaultColors;
    QList<GradientParamSelector*> mSelectors;
};
#endif // GRADIENTPARAMSELECTORWIDGET_H
