#include "tool.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

Tool::Tool(QWidget *parent) : QWidget(parent)
{
    setWindowIcon(QIcon(":/images/advancedtools.png"));
    setWindowFlags(Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
    setWindowFlag(Qt::Window);
    Qt::WindowFlags flags = windowFlags();
    setWindowFlags(flags^Qt::WindowContextHelpButtonHint);
    setMinimumSize(QSize(600,200));
    setFont(QFont("Times New Roman",12));

    mTableTool = new TableViewWidget;
    mChartTool = new ChartViewWidget;

    mStartTableBtn = new QPushButton(tr("启动表格工具"));
    mStartPlotBtn = new QPushButton(tr("启动绘图工具"));
    mMainLayout = new QVBoxLayout(this);

    QPushButton * B = new QPushButton("启动BB工具");
    QPushButton * C = new QPushButton("启动CC工具");
    QPushButton * D = new QPushButton("启动DD工具");

    QGroupBox * box1 = new QGroupBox;
    QHBoxLayout * lay1 = new QHBoxLayout;
    lay1->addWidget(mStartTableBtn);
    lay1->addWidget(mStartPlotBtn);
    box1->setLayout(lay1);

    QGroupBox * box2 = new QGroupBox;
    QHBoxLayout * lay2 = new QHBoxLayout;
    lay2->addWidget(B);
    lay2->addWidget(C);
    lay2->addWidget(D);
    box2->setLayout(lay2);

    mMainLayout->addWidget(box1);
    mMainLayout->setSpacing(10);

    connect(mStartTableBtn,&QPushButton::clicked,this,[=]{mTableTool->show();});
    connect(mStartPlotBtn,&QPushButton::clicked,this,[=]{mChartTool->show();});
}

void Tool::closeEvent(QCloseEvent *e)
{
    mChartTool->close();
    mTableTool->close();
    e->accept();
}
