#ifndef BACKGROUNDBORDER_H
#define BACKGROUNDBORDER_H

#include <QWidget>
#include <QSpinBox>
#include <QComboBox>
#include <QFormLayout>
#include <QListWidget>
#include <QChart>
using namespace QtCharts;

class BackgroundBorder : public QWidget
{
    Q_OBJECT
public:
    explicit BackgroundBorder(QChart*,const QIcon&,QWidget *parent = Q_NULLPTR);
private:
    void initBrushStyleComboBox(QComboBox *);
    void initJoinStyleComboBox(QComboBox *);
    void initCapStyleComboBox(QComboBox *);
    QChart * mChart;
    QIcon mIcon;
};

#endif // BACKGROUNDBORDER_H
