#ifndef SCATTERASSOCIATETABLE_H
#define SCATTERASSOCIATETABLE_H

#include <QDialog>
#include <QLegendMarker>
#include <QVXYModelMapper>
#include "../chart/chartshowtip.h"
#include "../chart/chartshowlegend.h"
#include "associatexyaxis.h"
#include "associatexymode.h"
#include "associatexyseries.h"

class ScatterAssociateTable : public QDialog
{
    Q_OBJECT
public:
    explicit ScatterAssociateTable(QTableView*,QChartView*,QWidget*parent = Q_NULLPTR);
private:
    QTableView * mTableView;
    TableViewModel * mTableModel;
    QChartView *mChartView;
    QHash<QScatterSeries*,QPair<int,int>> mSeriesXYColumn;
    void initConnections();
    void onOkBtn();
    void singleMapping();
    void doubleMapping();
    void setAxis(QScatterSeries *,Qt::Alignment);
private:
    QPushButton * mOkBtn;
    ChartShowLegend * mLegend;
    ChartShowTip * mTip;
    AssociateXYMode * mMode;
    AssociateXYAxis * mAxis;
    AssociateXYSeries * mSeries;
signals:
    void associateCompeleted();
    void tableChanged();
    void seriesColorChanged(QScatterSeries*);
    void seriesRemoved(QScatterSeries*);
};
#endif // SCATTERASSOCIATETABLE_H
