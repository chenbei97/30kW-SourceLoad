#include "communication.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

Communication::Communication(QWidget * parent) :QDialog(parent),
    mWorkMode(WORKMODE::CV),mCommunicationType(Type::None)
{
         /*-------------------------------------主布局-------------------------------------------*/
        mUsbConfigureBtn = new QPushButton(tr("配置"));
        mUsbConnectBtn = new QPushButton(tr("连接"));
        mUsbDisconnectBtn = new QPushButton(tr("断开"));
        mUsbConfigureBtn->setIcon(QIcon(":/images/portsettings.png"));
        mUsbConnectBtn->setIcon(QIcon(":/images/portconnect.png"));
        mUsbDisconnectBtn->setIcon(QIcon(":/images/portdisconnect.png"));
        mUsbConnectBtn->setEnabled(false); // 必须先配置才能连接
        mUsbDisconnectBtn->setEnabled(false); // 必须已连接才能断开

        mTcpConfigureBtn = new QPushButton(tr("配置"));
        mTcpConnectBtn = new QPushButton(tr("连接"));
        mTcpDisconnectBtn = new QPushButton(tr("断开"));
        mTcpConfigureBtn->setIcon(QIcon(":/images/portsettings.png"));
        mTcpConnectBtn->setIcon(QIcon(":/images/tcp_connect.png"));
        mTcpDisconnectBtn->setIcon(QIcon(":/images/tcp_disconnect.png"));
        mTcpConnectBtn->setEnabled(false);
        mTcpDisconnectBtn->setEnabled(false);

        QGroupBox * usbbox = new QGroupBox("USB");
        QHBoxLayout * usblay = new QHBoxLayout;
        usblay->addWidget(mUsbConfigureBtn);
        usblay->addWidget(mUsbConnectBtn);
        usblay->addWidget(mUsbDisconnectBtn);
        usbbox->setLayout(usblay);

        QGroupBox * tcpbox = new QGroupBox("TCP");
        QHBoxLayout * tcplay = new QHBoxLayout;
        tcplay->addWidget(mTcpConfigureBtn);
        tcplay->addWidget(mTcpConnectBtn);
        tcplay->addWidget(mTcpDisconnectBtn);
        tcpbox->setLayout(tcplay);

        QVBoxLayout * mainlay = new QVBoxLayout(this);
        mainlay->addWidget(usbbox);
        mainlay->addWidget(tcpbox);

        setFont(QFont("Times New Roman",12));
        resize(600,200);
        setWindowFlag(Qt::Window); // 设置window属性就不会总是让监控界面在前了
        setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);
        setWindowFlag(Qt::WindowMinimizeButtonHint);
        setWindowModality(Qt::NonModal);
        setWindowIcon(QIcon(":/images/communication.png"));

         /*-------------------------------------USB/TCP-------------------------------------------*/
        mUsbPort = new QSerialPort;
        mUsbQueryThread = new QThread;
        mUsbQueryWorker = new UsbQuery;
        mUsbQueryWorker->setDevice(mUsbPort);
        mUsbQueryTimer = new QTimer(this);
        mUsbQueryTimer->setInterval(mFixedQueryTime);
        mUsbQueryWorker->moveToThread(mUsbQueryThread);

        mTcpIP = "192.168.0.128";
        mTcpPort = 30000;
        mTcpSocket = new QTcpSocket;
        mTcpQueryThread = new QThread; // 固定和设定查询共用1个工作线程
        mTcpQueryWorker = new TcpQuery;
        mTcpQueryWorker->setDevice(mTcpSocket);
        mTcpQueryTimer = new QTimer(this);
        mTcpQueryTimer->setInterval(mFixedQueryTime);
        mTcpQueryWorker->moveToThread(mTcpQueryThread);

        /*-------------------------------------Other--------------------------------------------*/
        mMonitor = new Monitor; // 不要基于this,否则窗口总是靠前显示不方便
        mMonitor->setWorkMode(WORKMODE::CV);
        mLog = new Log;
        mInfo = new Infomation;
        mLogPathInfo = new Infomation;
        initLogPathInfo();
        /*-------------------------------------Connections-------------------------------------------*/
        initConnections();
}

void Communication::paintEvent(QPaintEvent *e)
{
        QPainter p;
        p.begin(this);
        p.setOpacity(0.5);
        p.setRenderHint(QPainter::SmoothPixmapTransform);
        QPixmap pix(":/images/communication.jpg");
        p.drawPixmap(QPoint(0,0),pix);
        p.end();
        Q_UNUSED(e);
}

void Communication::closeEvent(QCloseEvent *e)
{
      if (mUsbDisconnectBtn->isEnabled() | mTcpDisconnectBtn->isEnabled())
      {//如果没有关闭运行直接退出可能导致线程还在运行却无法停止会出现异常错误
          Warning * warning = new Warning;
          warning->setText(tr("请先断开USB/TCP连接再关闭!"));
          warning->setTextWrapIndent(2,16);
          warning->exec();
          delete warning;
          e->ignore();
          return;
      }
      e->accept();
}

bool Communication::needUpdateWorkMode()
{
    Messagebox * box = new Messagebox;
    box->setText(tr("是否已确认工作模式?(默认CV模式)"));
    box->setDialogFixedSize(900,280);
    box->setTextWrapIndent(2,16);
    box->button(0)->setButtonName(tr("是"));
    box->button(1)->setButtonName(tr("否"));
    int ret = box->exec();
    if (ret == QDialog::Rejected) return false;

     if (mMonitor->workMode() != mWorkMode) // 说明切换了工作模式
     {
          delete mMonitor;
          mMonitor = new Monitor;//新的工作界面,信号连接要重新绑定
          mMonitor->setWorkMode(mWorkMode);
          connect(this,&Communication::fixedQueryData, mMonitor,&Monitor::showMeasureState);
          connect(this,&Communication::monitorQueryData,mMonitor,&Monitor::showSetState);
          connect(this,static_cast<void (Communication::*)(TIMEOUTCODE)>(&Communication::errorCode),
                       mMonitor,&Monitor::setMessageState);
          connect(this,static_cast<void (Communication::*)(USBERROR)>(&Communication::errorCode),
                       mMonitor,static_cast<void (Monitor::*)(USBERROR)>(&Monitor::setConnectState));
          connect(this,static_cast<void (Communication::*)(TCPERROR)>(&Communication::errorCode),
                       mMonitor,static_cast<void (Monitor::*)(TCPERROR)>(&Monitor::setConnectState));
          connect(mMonitor,&Monitor::setMonitorRole,this,&Communication::getMonitorRole);
     }
    return true;
}

void Communication::showInfo(const QString & text)
{
    mInfo->setText(text);
    mInfo->setTextWrapIndent(2,23);
    mInfo->exec();
}

void Communication::initLogPathInfo()
{
    mLogPathInfoIsShow = true;
    mLogPathInfo->setText(""); // 文字在 showLogPath()添加这里只做初始化的工作
    mLogPathInfo->button(0)->setButtonName(tr("我已知晓"));
    MessageButton * button = new MessageButton(tr("不再提示"),1); // button id=1
    connect(button,&MessageButton::clicked,this,[=]{
         mLogPathInfo->accept();
         mLogPathInfoIsShow = false; // 全局不再提示
    });
    mLogPathInfo->addButton(button);
    QString style = "color:#1afa29;border:1px solid #1afa29;font-size:24px;"
                    "font-family:Times New Roman;min-width:160px;min-height:40px;"; // 按钮宽度不同,适应英文
    mLogPathInfo->setButtonStyle(mLogPathInfo->button(0),style);
    mLogPathInfo->setButtonStyle(button,style);//style需要保持一致
    mLogPathInfo->setTextAlignment(Qt::AlignLeft);
}

void Communication::showLogPathInfo(const QString& path)
{
    if (mLogPathInfoIsShow == false) return;// 不再提示
    mLogPathInfo->setText(path);
    mLogPathInfo->setTextWrapIndent(1,0);
    mLogPathInfo->exec();
}

WORKMODE Communication::workmode() const
{
    return mWorkMode;
}

void Communication::setWorkMode(WORKMODE mode)
{
    mWorkMode = mode;
}

UsbConfig::Settings Communication::usbSettings() const
{
    return mUsbSettings.settings();
}

void Communication::setUsbSettings(UsbConfig::Settings settings)
{
    mUsbSettings.setSettings(settings);
}

QIODevice* Communication::device(Type type) const
{
    QIODevice * dev = Q_NULLPTR;

    if (type == Type::Serial) return mUsbPort;
    if (type == Type::Tcp) return mTcpSocket;

    return dev;
}

Monitor* Communication::monitor() const
{
    return mMonitor;
}
