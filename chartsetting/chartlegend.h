#ifndef CHARTLEGEND_H
#define CHARTLEGEND_H

#include <QCheckBox>
#include <QComboBox>
#include <QGridLayout>
#include <QRadioButton>
#include <QButtonGroup>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include "chartbox.h"

class ChartLegend : public ChartBox
{
    Q_OBJECT
public:
    explicit ChartLegend(QChart * chart, const QIcon&icon, QWidget*parent = Q_NULLPTR);
private:
    QCheckBox * mLegendVisibleCheckBox;
    QCheckBox * mLegendBackVisibleCheckBox;
    QPushButton * mLegendTextColorBtn;
    QPushButton * mLegendBorderColorBtn;
    QPushButton * mLegendTextFontBtn;
    QComboBox * mLegendMarkers;
    QDoubleSpinBox * mLegendX;
    QDoubleSpinBox * mLegendY;
    QDoubleSpinBox * mLegendW;
    QDoubleSpinBox * mLegendH;
    QRectF mLegendGeometry;
    void updateLegendLayout();
};

#endif // CHARTLEGEND_H
