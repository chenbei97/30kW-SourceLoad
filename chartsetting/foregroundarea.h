#ifndef FOREGROUNDAREA_H
#define FOREGROUNDAREA_H

#include <QFormLayout>
#include <QPushButton>
#include <QComboBox>
#include <QListWidget>
#include <QSpinBox>
#include <QColorDialog>
#include <QGroupBox>
#include <QChart>
using namespace QtCharts;

class ForegroundArea: public QGroupBox
{
    Q_OBJECT
public:
    explicit ForegroundArea(QChart*,const QIcon&,QWidget *parent = Q_NULLPTR);
    QPushButton * colorSetButton() const ;
    QComboBox * brushStyleComboBox() const;
private:
    QChart * mChart;
    QIcon mIcon;
    QPushButton * mColorButton;
    QComboBox * mBrushStyle;
    void initBrushStyleComboBox();
};

#endif // FOREGROUNDAREA_H
