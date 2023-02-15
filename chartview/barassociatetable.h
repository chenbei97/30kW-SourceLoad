#ifndef BARASSOCIATETABLE_H
#define BARASSOCIATETABLE_H

#include <QDialog>
#include <QLegendMarker>
#include "../chart/chartshowtip.h"
#include "../chart/chartshowlegend.h"
#include "associatebarmode.h"
#include "associatebarseries.h"
#include "associatebaraxis.h"

class BarAssociateTable : public QDialog
{
    Q_OBJECT
public:
    explicit BarAssociateTable(QTableView*,QChartView*,QWidget*parent = Q_NULLPTR);
    typedef  QList<QColor> QColorList;
    enum AssociateMode {RowMode,ColMode,RowRegionMode,ColRegionMode};
    struct CategoryColor{
        QStringList categories;
        QColorList colors;
    };
private:
    void initConnections();
    void onOkBtn();
    void rcMapping();
    QStringList rcCategories() const;
    void regionMapping();
    QBarSet * createSet();
    QBarSeries * createSeries();
    void setAxisX(QBarSeries*,const QStringList&);
    void setAxisY(QBarSeries*);
    void createColRegionMapping(QBarSeries*);
    void createRowRegionMapping(QBarSeries*);
    CategoryColor getBarSetParams(QBarSeries*series) const;
    void onSeriesColorChanged(QBarSeries*,QColor,int);
    QTableView * mTableView;
    TableViewModel * mTableModel;
    QChartView *mChartView;
    QBarSeries * mCurrentSeries;
    QPair<int,int> mRegionFlagCount;
    AssociateMode mAssociateMode;
    QPushButton * mOkBtn;
    ChartShowLegend * mLegend;
    ChartShowTip * mTip;
    AssociateBarMode * mMode;
    AssociateBarSeries * mSeries;
    AssociateBarAxis * mAxis;
signals:
    void associateCompeleted();
    void seriesColorChanged(QBarSeries*,QColor,int);
    void modeChanged(int,int);
    void tableChanged();
};

#endif // BARASSOCIATETABLE_H
