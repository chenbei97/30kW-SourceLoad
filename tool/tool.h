#ifndef TOOL_H
#define TOOL_H

#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QGroupBox>
#include "tableview_tool.h"
#include "chartview_tool.h"

class Tool : public QWidget
{
    Q_OBJECT
public:
    explicit Tool(QWidget *parent =Q_NULLPTR);
    void closeEvent(QCloseEvent *) override;
private:
    QVBoxLayout* mMainLayout;
    QPushButton * mStartTableBtn;
    QPushButton * mStartPlotBtn;
    TableViewWidget * mTableTool;
    ChartViewWidget * mChartTool;
};

#endif // TOOL_H
