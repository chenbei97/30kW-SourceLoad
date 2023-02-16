#include "associatepieseries.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

AssociatePieSeries::AssociatePieSeries(QWidget*parent):QGroupBox(parent)
{
    setFont(QFont("Times New Roman",12));
    setTitle(tr("曲线"));

    mHoleSizeSpin = new QDoubleSpinBox;
    mHoleSizeSpin->setRange(0.0,1.0);
    mHoleSizeSpin->setDecimals(1);
    mHoleSizeSpin->setSingleStep(0.1);

    mLabelPositionCombo = new QComboBox;
    mLabelPositionCombo->addItems(QStringList()<<"Outside"<<"InsideHorizontal"
                             <<"InsideTangential"<<"InsideNormal");

    QFormLayout * lay = new QFormLayout;
    lay->addRow(tr("&饼洞比例"),mHoleSizeSpin);
    lay->addRow(tr("&标签位置"),mLabelPositionCombo);
    setLayout(lay);
}

double AssociatePieSeries::holeSize() const
{
    return mHoleSizeSpin->value();
}

int AssociatePieSeries::labelPosition() const
{
    return mLabelPositionCombo->currentIndex();
}
