#include "mainwindow.h"
extern QTranslator * translator;
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

MainWindow::MainWindow(QWidget *parent):QDialog(parent)
{
       setMinimumSize(QSize(600,200));
       setFont(QFont("Times New Roman",12));
       setWindowIcon(QIcon(":/images/dahua.png"));
       setWindowTitle(tr("30kW双向电源上位机"));
       setWindowFlag(Qt::Window);
       setWindowFlag(Qt::WindowStaysOnTopHint);//保持在前, http://t.zoukankan.com/linuxAndMcu-p-11533636.html
       setWindowFlags(Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);//添加最大最小化按钮

       mainSettings = new Setting;//不要基于this,否则总是显示靠前不方便
       mainCommunication = new Communication;
       mainTools = new Tool;

       init();
       initConnections();
}

void MainWindow::init()
{
    /*-----------------------------------------------------Action---------------------------------------------------------*/
    BasicSettingsAct = new QAction(tr("基本设置"));
    AboutHardWareAct = new QAction(tr("硬件"));
    AboutSoftWareAct = new QAction(tr("软件"));
    AboutQtAct = new QAction(tr("Qt"));
    AboutCNAct = new QAction(tr("中文"));
    AboutENAct = new QAction(tr("英文"));

    BasicSettingsAct->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_1));
    AboutHardWareAct->setShortcut(QKeySequence(Qt::CTRL+Qt::SHIFT+Qt::Key_H));
    AboutSoftWareAct->setShortcut(QKeySequence(Qt::CTRL+Qt::SHIFT+Qt::Key_S));
    AboutQtAct->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Q));
    AboutCNAct->setShortcut(QKeySequence(Qt::SHIFT+Qt::Key_C));
    AboutENAct->setShortcut(QKeySequence(Qt::SHIFT+Qt::Key_E));

    QString pathPrefix = QStringLiteral(":/images/");
    QString pathSuffix = QStringLiteral(".png");
    BasicSettingsAct->setIcon(QIcon(pathPrefix+QStringLiteral("basicSettings")+pathSuffix));
    AboutHardWareAct->setIcon(QIcon(pathPrefix+QStringLiteral("aboutHardware")+pathSuffix));
    AboutSoftWareAct->setIcon(QIcon(pathPrefix+QStringLiteral("aboutSoftware")+pathSuffix));
    AboutQtAct->setIcon(QIcon(pathPrefix+QStringLiteral("aboutQt")+pathSuffix));
    //AboutCNAct->setIcon(QIcon(pathPrefix+QStringLiteral("CN")+pathSuffix)); // 加图标的话就看不到互斥动作标记了
    //AboutENAct->setIcon(QIcon(pathPrefix+QStringLiteral("EN")+pathSuffix));

    /*-----------------------------------------------------Menu---------------------------------------------------------*/
    MenuBar = new QMenuBar(this);
    MenuBar->setGeometry(QRect(0,0,width(),30));
    MenuBar->setToolTip(tr("菜单栏"));
    MenuBar->setMouseTracking(true);
    MenuBar->setToolTipDuration(1000);

    MenuSettings = new QMenu(tr("设置"));
    MenuAbout = new QMenu(tr("关于"));
    MenuLanguage = new QMenu(tr("语言"));
    MenuLanguage->setIcon(QIcon(":/images/aboutLanguage.png"));

    MenuSettings->addAction(BasicSettingsAct);
    MenuSettings->addMenu(MenuLanguage);

    QActionGroup * LanguageGroup = new QActionGroup(this);
    AboutCNAct->setCheckable(true); // 默认是false先设为true
    AboutENAct->setCheckable(true);
    // LanguageGroup->addAction(AboutCNAct);
    // LanguageGroup->addAction(AboutENAct);
    // MenuLanguage->addAction(AboutCNAct);
    // MenuLanguage->addAction(AboutENAct);
    MenuLanguage->addAction(LanguageGroup->addAction(AboutCNAct)); //更简洁
    MenuLanguage->addAction(LanguageGroup->addAction(AboutENAct));

    MenuAbout->addAction(AboutHardWareAct);
    MenuAbout->addSeparator();
    MenuAbout->addAction(AboutSoftWareAct);
    MenuAbout->addSeparator();
    MenuAbout->addAction(AboutQtAct);

    MenuBar->addMenu(MenuSettings);
    MenuBar->addSeparator();
    MenuBar->addMenu(MenuAbout);

     /*-----------------------------------------------------ToolButton---------------------------------------------------------*/
    BasicSettingsTbn = new QToolButton;
    CommunicationTbn = new QToolButton;
    AdvanceTbn = new QToolButton;
    AboutTbn = new QToolButton;

    BasicSettingsTbn->setStyleSheet("QToolButton::menu-indicator{image:none;}");// 去除掉右下角的小三角样式
    BasicSettingsTbn->setAutoRaise(true);
    BasicSettingsTbn->setIcon(QIcon(pathPrefix+QStringLiteral("basicSettings")+pathSuffix));
    BasicSettingsTbn->setIconSize(QSize(64,64));

    CommunicationTbn->setAutoRaise(true);
    CommunicationTbn->setStyleSheet("QToolButton::menu-indicator{image:none;}");
    CommunicationTbn->setIcon(QIcon(pathPrefix+QStringLiteral("communication")+pathSuffix));
    CommunicationTbn->setIconSize(QSize(64,64));

    AdvanceTbn->setStyleSheet("QToolButton::menu-indicator{image:none;}");
    AdvanceTbn->setAutoRaise(true);
    AdvanceTbn->setIcon(QIcon(pathPrefix+QStringLiteral("advancedtools")+pathSuffix));
    AdvanceTbn->setIconSize(QSize(64,64));

    AboutTbn->addAction(AboutHardWareAct); // 5个动作同时包含
    AboutTbn->addAction(AboutSoftWareAct);
    AboutTbn->addAction(AboutQtAct);
    AboutTbn->addAction(AboutCNAct);
    AboutTbn->addAction(AboutENAct);
    AboutTbn->setPopupMode(QToolButton::InstantPopup); //只有关于ToolButton设置下拉菜单,无需去除小三角样式
    AboutTbn->setAutoRaise(true);
    AboutTbn->setIcon(QIcon(pathPrefix+QStringLiteral("about")+pathSuffix));
    AboutTbn->setIconSize(QSize(64,64));

     /*-----------------------------------------------------Layout---------------------------------------------------------*/
    mainLayout = new QGridLayout;
    mainLayout->addWidget(BasicSettingsTbn,0,0,1,1,Qt::AlignCenter);
    mainLayout->addWidget(CommunicationTbn,0,1,1,1,Qt::AlignCenter);
    mainLayout->addWidget(AdvanceTbn,0,2,1,1,Qt::AlignCenter);
    mainLayout->addWidget(AboutTbn,0,3,1,1,Qt::AlignCenter);
    mainLayout->setMargin(5);
    mainLayout->setSpacing(5);
    setLayout(mainLayout);
}

void MainWindow::initConnections()
{
    // 关于/设置按钮使用addAction,不使用clicked
    connect(BasicSettingsTbn,&QToolButton::clicked,this,[=]{BasicSettingsAct->trigger();});
    connect(CommunicationTbn,&QToolButton::clicked,this,[=]{mainCommunication->show();});
    connect(AdvanceTbn,&QToolButton::clicked,this,[=]{mainTools->show();});
    connect(BasicSettingsAct,&QAction::triggered,this,[=]{mainSettings->show();});
    connect(AboutQtAct,&QAction::triggered,this,[=]{QMessageBox::aboutQt(this,tr("关于Qt"));});
    connect(AboutHardWareAct,&QAction::triggered,this,[=]{

    });
    connect(AboutSoftWareAct,&QAction::triggered,this,[=]{

    });
    connect(AboutENAct,&QAction::triggered,this,[=]{changeLanguage(LANUAGE ::EN);});
    connect(AboutCNAct,&QAction::triggered,this,[=]{changeLanguage(LANUAGE ::CN);});

   connect(mainSettings,static_cast<void (Setting::*)(WORKMODE)>(&Setting::workModeChanged),
           this,[=](WORKMODE mode){mainCommunication->setWorkMode(mode);});
   connect(mainSettings,&Setting::selectedPeriod,mainCommunication,&Communication::selectedPeriod);
   connect(mainSettings,&Setting::logEnabled,mainCommunication,&Communication::logEnabled);
   connect(mainSettings,&Setting::logFileFormat,mainCommunication,&Communication::logFileFormat);
   connect(mainSettings,&Setting::setTerminalCommand,mainCommunication,&Communication::getTerminalCommand);
   connect(mainCommunication,&Communication::terminalQueryData,mainSettings,&Setting::terminalQueryData);
   connect(mainCommunication,static_cast<void (Communication::*)(QIODevice*)>(&Communication::currentDevice)
           ,this,[=](QIODevice* device){ // usb/tcp connect clicked trigger this signal,set device for querypanel
            QueryPanel * querypanel = static_cast<QueryPanel*>(mainSettings->statckWidget(2));
            querypanel->setDevice(device);
   });
}

void MainWindow::resizeEvent(QResizeEvent *e)
{
      MenuBar->setGeometry(QRect(0,0,e->size().width(),30));
}

void MainWindow::closeEvent(QCloseEvent * e)
{
     mainCommunication->monitor()->close();
     mainCommunication->close(); // 主窗口关闭让通讯也关闭,因为通讯不基于this创建
     mainTools->close();
     if (mainCommunication!=Q_NULLPTR)
     {
         delete mainCommunication;
         mainCommunication = Q_NULLPTR;
     }
     e->accept();
}

void MainWindow::paintEvent(QPaintEvent *e)
{
      // 绘图开始的左上角顶点的X坐标是0，Y坐标应该是菜单栏的高度30
      int x = 0;
      int y = MenuBar->height();
      int height = this->height()- y; //要绘制的高度
      int width = this->width(); // 要绘制的宽度
      QRect rect(x,y,width,height);
      QPixmap pix;
      pix.load(":/images/DH.png");
      QPainter painter; // 不要使用指针的方式,new出来的无法销毁
      painter.begin(this);
      painter.setOpacity(0.2);
      painter.setRenderHint(QPainter::SmoothPixmapTransform);
      painter.drawPixmap(rect,pix);
      painter.end();
      Q_UNUSED(e);
}

void MainWindow::contextMenuEvent(QContextMenuEvent *e)
{
      Q_UNUSED(e);
      QMenu * menulist = new QMenu(this);
      menulist->addAction(BasicSettingsAct);
      menulist->addSeparator();
      menulist->addAction(AboutCNAct);
      menulist->addAction(AboutENAct);
      menulist->exec(QCursor::pos());
}

void MainWindow::setLanuage(LANUAGE lan)
{
    mLanuage = lan;
    AboutCNAct->setChecked(mLanuage==LANUAGE ::CN);
    AboutENAct->setChecked(mLanuage==LANUAGE ::EN);
}

void MainWindow::changeLanguage(LANUAGE  lan) // CN/EN的act调用
{
     mLanuage = lan;

     QString a = (lan == LANUAGE ::CN ? "cn":"en");
     QString path = QStringLiteral(":/%1.qm").arg(a);

     qApp->removeTranslator(translator);
     delete translator;
     translator=new QTranslator;
     translator->load(path);
     qApp->installTranslator(translator);
     QSettings settings("WWB-Qt",QCoreApplication::applicationName()); //注册组织和应用程序
     settings.setValue("language",a);

     qApp->exit(773); // 重启程序,773是约定的退出指令,main.cpp会收到他并重启
}
