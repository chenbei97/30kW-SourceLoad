#ifndef CHARTMARGIN_H
#define CHARTMARGIN_H

#include <QVBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include "chartbox.h"

class ChartMargin : public ChartBox
{
    Q_OBJECT
public:
    explicit ChartMargin(QChart *chart,const QIcon&,QWidget*parent = Q_NULLPTR);
private:
    QPushButton * mMarginResetBtn;
};
#endif // CHARTMARGIN_H
