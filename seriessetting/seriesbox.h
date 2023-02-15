#ifndef SERIESBOX_H
#define SERIESBOX_H

#include <QGroupBox>
#include <QColorDialog>
#include <QFontDialog>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QListWidget>
#include <QtCharts>
#include <QFormLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QSpinBox>
using namespace QtCharts;

class SeriesBox : public QGroupBox
{
    Q_OBJECT
public:
    explicit SeriesBox(QChart *,QWidget* = Q_NULLPTR);
    void setChart(QChart*);
    QChart* chart() const;
    void setCurrentSeries(QAbstractSeries*,int);
    QAbstractSeries * currentSeries() const;
    void setCurrentSeriesId(int);
    int currentSeriesId() const;
protected:
    QChart * mChart;
    QAbstractSeries * mCurrentSeries;
    int mCurrentSeriesId;
    QIcon mIcon;
    virtual void updateState()=0;
};

#endif // SERIESBOX_H
