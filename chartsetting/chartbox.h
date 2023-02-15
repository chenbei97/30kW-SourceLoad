#ifndef CHARTBOX_H
#define CHARTBOX_H

#include <QGroupBox>
#include <QColorDialog>
#include <QFontDialog>
#include <QIcon>
#include <QLabel>
#include <QChart>
using namespace QtCharts;

class ChartBox : public QGroupBox
{
    Q_OBJECT
public:
    explicit ChartBox(QChart * chart, const QIcon&icon, QWidget*parent = Q_NULLPTR);
    void setChart(QChart*);
    QColorDialog* colorDialog(const QColor&);
    QFontDialog* fontDialog(const QFont&);
protected:
    QChart * mChart;
    QIcon mIcon;
};

#endif // CHARTBOX_H
