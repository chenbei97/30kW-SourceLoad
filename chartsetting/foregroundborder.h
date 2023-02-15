#ifndef FOREGROUNDBORDER_H
#define FOREGROUNDBORDER_H

#include <QGroupBox>
#include <QListWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QFormLayout>
#include <QChart>
using namespace QtCharts;

class ForegroundBorder : public QWidget
{
    Q_OBJECT
public:
    explicit ForegroundBorder(QChart*,const QIcon&,QWidget *parent = Q_NULLPTR);
private:
    QChart * mChart;
    QIcon mIcon;
    void initBrushStyleComboBox(QComboBox *);
    void initJoinStyleComboBox(QComboBox *);
    void initCapStyleComboBox(QComboBox *);
};

#endif // FOREGROUNDBORDER_H
