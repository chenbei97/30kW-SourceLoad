#ifndef PIEASSOCIATETABLE_H
#define PIEASSOCIATETABLE_H

#include <QDialog>
#include <QLegendMarker>
#include "associatepieseries.h"
#include "associatepiemode.h"
#include "../chart/chartshowtip.h"
#include "../chart/chartshowlegend.h"

class PieAssociateTable : public QDialog
{
    Q_OBJECT
public:
    explicit PieAssociateTable(QTableView*,QChartView*,QWidget*parent = Q_NULLPTR);
    enum AssociateMode {RowMode,ColMode,RowRegionMode,ColRegionMode};
    typedef  QList<QColor> QColorList;
private:
    void initConnections();
    void onOkBtn();
    void rcMapping();
    void regionMapping();
    void createRowRegionMapping(QPieSeries*);
    void createColRegionMapping(QPieSeries*);
    QList<QPieSlice*> createSlices();
    void setSeriesProperties(QPieSeries*);
    void onSeriesColorChanged(QColor,int);
    AssociateMode mAssociateMode;
    QPair<int,int> mRegionLabelValue;//事先记录区域模式下要映射的标签和数值所在行/列位置
    int mFlagValue;// 事先记录非区域模式下要映射的行/列位置
    QTableView * mTableView;
    TableViewModel * mTableModel;
    QChartView *mChartView;
    QPieSeries * mCurrentSeries;
    QPushButton * mOkBtn;
    ChartShowLegend * mLegend;
    ChartShowTip * mTip;
    AssociatePieMode * mMode;
    AssociatePieSeries * mSeries;
signals:
    void associateCompeleted();
    void seriesColorChanged(QColor,int);
    void modeChanged(int,int = 0);
    void tableChanged();
};

#endif // PIEASSOCIATETABLE_H
