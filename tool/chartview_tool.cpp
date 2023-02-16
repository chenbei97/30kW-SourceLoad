#include "chartview_tool.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

ChartViewWidget::ChartViewWidget(QWidget *parent) : QWidget(parent)
{
    init();
    initConnections();
}

void ChartViewWidget::closeEvent(QCloseEvent *e)
{
    mLineTab->closeChildrenWindows(); // 窗口关闭时把所有子窗口都关掉
    mScatterTab->closeChildrenWindows();
    mBarTab->closeChildrenWindows();
    mPieTab->closeChildrenWindows();
    e->accept();
}

void ChartViewWidget::init()
{
    setMouseTracking(true);
    setWindowIcon(QIcon(":/images/chartview.png"));
    setWindowFlags(Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
    setWindowFlag(Qt::Window);
    Qt::WindowFlags flags = windowFlags();
    setWindowFlags(flags^Qt::WindowContextHelpButtonHint);
    setMinimumSize(QSize(2000,1000));
    setFont(QFont("Times New Roman",12));

    /*-----------------------------------------table-----------------------------------------*/
    mImportFileBtn = new QPushButton(tr("导入文件"));
    mImportHeaderBtn = new QCheckBox(tr("文件带表头?"));
    mImportHeaderBtn->setCheckState(Qt::Checked);
    QGroupBox * tablebtnbox = new QGroupBox;
    QHBoxLayout * tablebtnlay = new QHBoxLayout;
    tablebtnlay->addWidget(mImportFileBtn);
    tablebtnlay->addWidget(mImportHeaderBtn);
    tablebtnbox->setLayout(tablebtnlay);

    mTableView = new QTableView;
    mTableView->setMinimumWidth(400);
    mTableModel = new TableViewModel;
    mTableView->setModel(mTableModel);
    QItemSelectionModel * selection = new QItemSelectionModel(mTableModel);
    mTableView->setSelectionModel(selection);
    mTableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    mTableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    mTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    mTableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    QGroupBox * tablebox = new QGroupBox;
    QVBoxLayout * tablelay = new QVBoxLayout;
    tablelay->addWidget(mTableView);
    tablelay->addWidget(tablebtnbox);
    tablebox->setLayout(tablelay);

    /*-----------------------------------------tab-----------------------------------------*/
    mTab = new QTabWidget;
    mTab->setMinimumWidth(1000);
    mLineTab = new LineChart(mTableView,mTab);
    mScatterTab = new ScatterChart(mTableView,mTab);
    mBarTab = new BarChart(mTableView,mTab);
    mPieTab = new PieChart(mTableView,mTab);
    mTab->addTab(mLineTab,QIcon(":/images/linechart.png"),tr("折线图"));
    mTab->addTab(mScatterTab,QIcon(":/images/scatterchart.png"),tr("散点图"));
    mTab->addTab(mBarTab,QIcon(":/images/barchart.png"),tr("柱状图"));
    mTab->addTab(mPieTab,QIcon(":/images/piechart.png"),tr("饼图"));
    /*-----------------------------------------layout-----------------------------------------*/
    QSplitter * splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(tablebox);
    splitter->addWidget(mTab);
    splitter->setStretchFactor(0,1);
    splitter->setStretchFactor(1,4);
    splitter->setHandleWidth(10);
    splitter->setChildrenCollapsible(false);

    mLayout = new QGridLayout(this);
    mLayout->addWidget(splitter);//改用分割条灵活
//    mLayout->addWidget(tablebox, 1, 0);
//    mLayout->addWidget(mTab, 1, 1);
//    mLayout->setColumnStretch(1, 1);
//    mLayout->setColumnStretch(0, 0);


    connect(this,&ChartViewWidget::tableChanged,this,[=]{
        mTableModel->clearMapping();// 把颜色去掉,不过颜色的去除稍有延迟,update不起作用
        mTableView->update(QRect(0,0,mTableModel->columnCount(),mTableModel->rowCount()));
        mLineTab->clearChart();
        mScatterTab->clearChart();
        mBarTab->clearChart();
        mPieTab->clearChart();
    });
}

void ChartViewWidget::initConnections()
{
    connect(this,&ChartViewWidget::tableChanged,mLineTab,&LineChart::tableChanged);
    connect(this,&ChartViewWidget::tableChanged,mScatterTab,&ScatterChart::tableChanged);
    connect(this,&ChartViewWidget::tableChanged,mBarTab,&BarChart::tableChanged);
    connect(this,&ChartViewWidget::tableChanged,mPieTab,&PieChart::tableChanged);

    connect(mImportFileBtn,&QPushButton::clicked,this,[=]{
        QDir debugDir= QDir::current();
        debugDir.cdUp();
        QString fileName = QFileDialog::getOpenFileName(Q_NULLPTR, tr("Txt/Csv文件"), debugDir.path(),tr("*.txt *.csv"));
        if (fileName.isEmpty()) return; // 弹窗类型语句不可以放在run函数内执行

        bool withHeader = mImportHeaderBtn->isChecked();
        bool isCsv = fileName.contains(".csv");

        QFuture<void> future =QtConcurrent::run([=]{
            mTableModel->clear();

            QFile outFile(fileName);
            outFile.open( QIODevice::ReadOnly );
            QTextStream ts(&outFile);
            ts.setAutoDetectUnicode(true);
            ts.setCodec(QTextCodec::codecForName("UTF-8"));
            if (isCsv) ts.setCodec(QTextCodec::codecForName("gbk"));

            if (withHeader) { // 如果导入文件带有表头
                QString header = ts.readLine(); // 事先读取1行
                if (header.isEmpty()) return; // 至少应该有1行表头

                QStringList headerList;
                headerList = isCsv? header.split(',',QString::SkipEmptyParts):
                                    header.split(QRegExp("\\s+"),QString::SkipEmptyParts);
                mTableModel->setHorizontalHeaderLabels(headerList);//设置表头
            }

            while (!ts.atEnd()) {
                QString line= ts.readLine();
                QStringList lineList =isCsv?line.split(',',QString::SkipEmptyParts):
                                            line.split(QRegExp("\\s+"),QString::SkipEmptyParts);
                mTableModel->appendRow(lineList);
            }
            outFile.close();
            mTableModel->update(); // 必须调用更新内部的模型数据
        });
        while (!future.isFinished()) QApplication::processEvents(QEventLoop::AllEvents, 5);
        emit tableChanged();
    });
}
