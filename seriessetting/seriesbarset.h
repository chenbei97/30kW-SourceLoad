#ifndef SERIESBARSET_H
#define SERIESBARSET_H

#include <QGroupBox>
#include <QColorDialog>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QtCharts>
using namespace QtCharts;

class SeriesBarSet : public QGroupBox
{
    Q_OBJECT
public:
    explicit SeriesBarSet(QChart*,QWidget*parent=Q_NULLPTR);
    enum AssociateMode {RowMode,ColMode,RowRegionMode,ColRegionMode};
    void setCurrentSeries(QBarSeries*);
    QBarSeries* currentSeries() const;
    void updateBarSet();
    void updateAssociateMode(int,int);
private:
    void updateState();
    void disconnectAllConnections();
    QChart * mChart;
    QBarSeries * mCurrentSeries;
    QIcon mIcon;
    AssociateMode mAssociateMode;
    QHash<int,int> mAssociateFlags;
    QColorDialog* colorDialog(const QColor&);
    void updateSumState();
    void updateColorState();

    QComboBox * mCurrentCategory;
    QLabel * mSum;
    QPushButton * mColor;
private slots:
    void changeColor();
    void changeCategory(int);
signals:
    void seriesColorChanged(QBarSeries*,QColor,int);
};

#endif // SERIESBARSET_H
