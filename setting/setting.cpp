#include "setting.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

Setting::Setting(QWidget * parent):QDialog(parent)
{
    initUI();
    initConnections();
}

void Setting::initUI()
{
    // (1) 顶部水平布局
    topLayout = new QHBoxLayout;
    listwidget = new QListWidget;
    QStringList  settings;
    settings <<tr("工作模式")<<tr("日志周期")<<tr("查询面板");
    //settings <<tr("工作模式")<<tr("日志周期")<<tr("表格功能")<<tr("查询面板");
    listwidget->addItems(settings);

    stackwidget = new QStackedWidget; // 堆叠界面
    mWorkMode = new WorkModeDialog(this);
    mLogPeriod = new LogPeriod(this);
    mTerminal = new QueryPanel(this);

    stackwidget->addWidget(mWorkMode);
    stackwidget->addWidget(mLogPeriod);
    stackwidget->addWidget(mTerminal);

    topLayout->addWidget(listwidget,Qt::AlignCenter);
    topLayout->addWidget(stackwidget,Qt::AlignCenter);
    topLayout->setMargin(10);
    topLayout->setSpacing(10);
    topLayout->setStretch(0,1);
    topLayout->setStretch(1,4);// 1:4的比例

    // (2) 底部水平布局
    bottomLayout = new QHBoxLayout;
    okBtn = new QPushButton(tr("确定"));
    cancelBtn = new QPushButton(tr("取消"));
    bottomLayout->addStretch();
    bottomLayout->addWidget(okBtn);
    bottomLayout->addWidget(cancelBtn);

    // (3) 全局布局
    mainLayout = new QGridLayout;
    mainLayout->setMargin(10);
    mainLayout->setSpacing(10);
    mainLayout->addLayout(topLayout,0,0);
    mainLayout->addLayout(bottomLayout,1,0);

    setLayout(mainLayout);
    resize(1150,400);
    setFont(QFont("Times New Roman",12));
    setWindowTitle(tr("基本设置"));
    setWindowIcon(QIcon(":/images/basicSettings.png"));
    setWindowFlags(Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);//添加最大最小化和关闭按钮
    setWindowFlag(Qt::Window); // 设置window属性就不会总是让界面在前或者在后
    Qt::WindowFlags flags = windowFlags();
    setWindowFlags(flags^Qt::WindowContextHelpButtonHint); // 去除帮助按钮
}

void Setting::initConnections()
{
    connect(mTerminal,&QueryPanel::setTerminalCommand,this,&Setting::setTerminalCommand);//通知communication要查询的面板命令
    connect(this,&Setting::terminalQueryData,mTerminal,&QueryPanel::terminalQueryData);

    connect(mWorkMode,&WorkModeDialog::workModeChanged,this,&Setting::workModeChanged);// 通知monitor
    connect(mLogPeriod,&LogPeriod::selectedPeriod,this,&Setting::selectedPeriod); // 通知 communication
    connect(mLogPeriod,&LogPeriod::logEnabled,this,&Setting::logEnabled); // 通知 communication
    connect(mLogPeriod,&LogPeriod::logFileFormat,this,&Setting::logFileFormat); // 通知 communication

    connect(listwidget,static_cast<void (QListWidget::*)(int)>(&QListWidget::currentRowChanged),
            this,[=](int row){stackwidget->setCurrentIndex(row);});
    connect(okBtn,&QPushButton::clicked,this,[=]{accept();});
    connect(cancelBtn,SIGNAL(clicked()),this,SLOT(reject()));
}

QWidget* Setting::statckWidget(int index) const
{
    return stackwidget->widget(index);
}
