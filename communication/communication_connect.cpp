#include "communication.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif


void Communication::initConnections()
{
        /*--------------------------------Communication-------------------------------*/
        connect(this,&Communication::fixedQueryData, mMonitor,&Monitor::showMeasureState);
        connect(this,&Communication::monitorQueryData,mMonitor,&Monitor::showSetState);
        connect(this,static_cast<void (Communication::*)(TIMEOUTCODE)>(&Communication::errorCode),
             mMonitor,&Monitor::setMessageState);
        connect(this,static_cast<void (Communication::*)(USBERROR)>(&Communication::errorCode),
             mMonitor,static_cast<void (Monitor::*)(USBERROR)>(&Monitor::setConnectState));
        connect(this,static_cast<void (Communication::*)(TCPERROR)>(&Communication::errorCode),
             mMonitor,static_cast<void (Monitor::*)(TCPERROR)>(&Monitor::setConnectState));
        connect(mMonitor,&Monitor::setMonitorRole,this,&Communication::getMonitorRole); // Monitor's setMonitorRole=>Communication's getMonitorRole
        connect(this,SIGNAL(getMonitorRole(MONITORROLE,const QByteArray&)),this,SLOT(monitorQuery(MONITORROLE,const QByteArray&)));//=>Communication's setMonitorRole
        connect(this,static_cast<void (Communication::*)(quint32,quint32,quint32,quint32)>(&Communication::selectedPeriod),this,
                [=](quint32 ms,quint32 s,quint32 m,quint32 h){ quint32 total_ms = ms + s * 1000 + m * 60000 + h * 3600000; mLog->setInterval(total_ms);});
        connect(this,static_cast<void (Communication::*)(bool)>(&Communication::logEnabled),this,[=](bool isLogEnabled){mLog->setEnabled(isLogEnabled);});
        connect(this,&Communication::logFileFormat,mLog,&Log::logFileFormat);
        /*--------------------------------USB-------------------------------*/
        connect(this,&Communication::setMonitorRole,mUsbQueryWorker,&UsbQuery::setMonitorRole); // => UsbQuery's setMonitorRole
        connect(mUsbQueryWorker,SIGNAL(fixedQueryData(const QList<QByteArray>&)), this,SLOT(record(const QList<QByteArray>&))); // 固定查询-log/monitor
        connect(mUsbQueryWorker,SIGNAL(monitorQueryData(MONITORROLE,bool)), this,SIGNAL(monitorQueryData(MONITORROLE,bool)));// 设定查询-monitor
        connect(mUsbQueryWorker,SIGNAL(monitorQueryData(MONITORROLE,const QByteArray&,const QByteArray&)),
                this,SLOT(record(MONITORROLE,const QByteArray&,const QByteArray&)));  // 设定查询-log
        connect(mUsbQueryWorker,SIGNAL(errorCode(TIMEOUTCODE)), this,SIGNAL(errorCode(TIMEOUTCODE)));// 超时
        connect(mUsbQueryWorker,SIGNAL(errorCode(USBERROR)),this,SLOT(usbError(USBERROR))); // 串口错误
        connect(mUsbQueryTimer,&QTimer::timeout,this,[=]{ usbCycle();}); // 定时器
        connect(mUsbConfigureBtn,&QPushButton::clicked,this,[=] { usbConfigure();}); // 3个按钮
        connect(mUsbConnectBtn,&QPushButton::clicked,this,[=]{ usbConnect();});
        connect(mUsbDisconnectBtn,&QPushButton::clicked,this,[=]{ usbDisconnect();});
         /*--------------------------------TCP-------------------------------*/
        connect(this,&Communication::setMonitorRole,mTcpQueryWorker,&TcpQuery::setMonitorRole); // => TcpQuery's setMonitorRole
        connect(mTcpQueryWorker,SIGNAL(fixedQueryData(const QList<QByteArray>&)),this,SLOT(record(const QList<QByteArray>&))); // 固定查询-log+monitor
        connect(mTcpQueryWorker,SIGNAL(monitorQueryData(MONITORROLE,bool)), this,SIGNAL(monitorQueryData(MONITORROLE,bool)));// 设定查询-monitor
        connect(mTcpQueryWorker,SIGNAL(monitorQueryData(MONITORROLE,const QByteArray&,const QByteArray&)),
                this,SLOT(record(MONITORROLE,const QByteArray&,const QByteArray&)));  // 设定查询-log
        connect(mTcpQueryWorker,SIGNAL(errorCode(TIMEOUTCODE)),this,SIGNAL(errorCode(TIMEOUTCODE))); // 超时
        connect(mTcpQueryWorker,SIGNAL(errorCode(TCPERROR)),this,SLOT(tcpError(TCPERROR))); // 套接字错误
        connect(mTcpQueryTimer,&QTimer::timeout,this,[=]{ tcpCycle();});// 定时器
        connect(mTcpConnectBtn,&QPushButton::clicked,this,[=]{ tcpConnect();});// 3个按钮
        connect(mTcpDisconnectBtn,&QPushButton::clicked,this,[=]{ tcpDisconnect();});
        connect(mTcpConfigureBtn,&QPushButton::clicked,this,[=] { tcpConfigure();});
        /*--------------------------------Terminal-------------------------------*/
        connect(this,SIGNAL(getTerminalCommand(const QByteArray&)), // QueryPanel's setTerminalRole => this's getTerminalRole
                this,SLOT(terminalQuery(const QByteArray&))); // => updateTerminalRole => emit setTermianlRole
        connect(mUsbQueryWorker,SIGNAL(terminalQueryData(const QByteArray&,const QByteArray&)),
                this,SLOT(record(const QByteArray&,const QByteArray&)));
        connect(mTcpQueryWorker,SIGNAL(terminalQueryData(const QByteArray&,const QByteArray&)),
                this,SLOT(record(const QByteArray&,const QByteArray&)));
}

void Communication::monitorQuery(MONITORROLE role,const QByteArray& value)
{// USB/TCP共用同一个getMonitorRole信号获取设定角色和值,才引入了mCommunicationType
      if (Query::controller() != ResponseProcessor::Controller::MonitorQuery)
          Query::setController(ResponseProcessor::Controller::MonitorQuery); // 告知IO设备回复的消息来自monitor不是terminal

     if (mCommunicationType == Type::Serial)
     {
         mUsbQueryWorker->monitorQuery(role,value);// 本质也不需要更新角色,已验证闭环逻辑
         emit setMonitorRole(role,value); // role仅仅是为了在mUsbQueryWorker->monitorQuery内翻译成对应命令
     }
     else if (mCommunicationType == Type::Tcp)
     {
         mTcpQueryWorker->monitorQuery(role,value);
         emit setMonitorRole(role,value);
     }
}

void Communication::record(const QList<QByteArray>&data)
{
        emit fixedQueryData(data); // To Monitor
        if (!mLog->isEnabled()) return; // log not enabled
        if (mLog->isExecute()) { // To log
             mLog->setExecute(false);
             mLog->generateFixedQueryLog(data);//内部已将log数据发送
        }
}

void Communication::record(MONITORROLE role,const QByteArray& preset,const QByteArray&actual)
{
    if (!mLog->isEnabled()) return; // log not enabled
     //  Only To log  角色,  预设值 , 实际值 (设定查询数据同时绑定了信号和record槽函数,转发给Monitor的工作在这里无需再做)
    QString field = Monitor::translateErrorCode(role);
    QStringList data;
    data << field <<preset <<actual;
    mLog->generateMonitorQueryLog(data);
}

void Communication::record(const QByteArray& command, const QByteArray& response)
{
    // To Termial 查询的命令, 命令的回复(设置命令没有回复)
    emit terminalQueryData(response); // Response To QueryPanel

    if (!mLog->isEnabled()) return; // log not enabled

    QByteArray res = response;
    res.chop(1); // 回复带\n,需要去除
    QStringList data;
    data << command << res;
    mLog->generateTerminalQueryLog(data);
}

void Communication::usbError(USBERROR code)
{// 串口错误 需要做一定处理不能直接转发,防止传播错误到监控界面
        emit errorCode(code);
        if (code != USBERROR::UsbNoError)
        {
              mUsbPort->close();
              mUsbQueryTimer->stop();
              mUsbQueryThread->quit();
              mUsbQueryThread->wait();

              mUsbConfigureBtn->setEnabled(true);
              mUsbConnectBtn->setEnabled(false);
              mUsbDisconnectBtn->setEnabled(false);//禁用连接,防止用户再次点击连接造成异常退出

              Critical * dlg = new Critical;
              dlg->setText(tr("串口错误类型: %1").arg(mUsbPort->errorString()));
              dlg->setTextWrapIndent(2,20);
              dlg->exec();
              delete dlg;
        }
}

void Communication::usbCycle()
{
        mUsbQueryWorker->fixedQuery();
        mLog->increaseCounter();
}

void Communication::usbConfigure()
{
        Qt::WindowFlags flags = mUsbSettings.windowFlags();
        mUsbSettings.setWindowFlags(flags | Qt::MSWindowsFixedSizeDialogHint);//设置为固定大小
        int ret = mUsbSettings.exec();
        if (ret == QDialog::Accepted)
        {
            mUsbPort->setPortName(mUsbSettings.settings().Name);
            mUsbPort->setBaudRate(mUsbSettings.settings().BaudRate);
            mUsbPort->setParity(mUsbSettings.settings().Parity);
            mUsbPort->setDataBits(mUsbSettings.settings().DataBit);
            mUsbPort->setStopBits(mUsbSettings.settings().StopBit);
            mUsbPort->setFlowControl(mUsbSettings.settings().FlowControl);
            mUsbConnectBtn->setEnabled(true);
            mUsbDisconnectBtn->setEnabled(false);
        }
}

void Communication::usbConnect()
{
         if (!needUpdateWorkMode()) return;

         if (!mUsbPort->open(QIODevice::ReadWrite)) return;

         showInfo(tr("USB通讯已连接!"));
         mCommunicationType = Type::Serial; // 更新通讯类型

         mUsbQueryThread->start();
         mUsbQueryTimer->start();

         mUsbConnectBtn->setEnabled(false);
         mUsbDisconnectBtn->setEnabled(true);
         mUsbConfigureBtn->setEnabled(false);
         mMonitor->show();
    qDebug()<<4;
         emit currentDevice(mUsbPort);
}

void Communication::usbDisconnect()
{
        mUsbPort->close();
        mUsbQueryTimer->stop();
        mUsbQueryThread->quit();
        mUsbQueryThread->wait();

        mUsbConfigureBtn->setEnabled(true);
        mUsbConnectBtn->setEnabled(true);
        mUsbDisconnectBtn->setEnabled(false);

        mMonitor->close();
        mLog->reset();

        showInfo(tr("USB通讯已断开!"));
        //emit currentDevice(Q_NULLPTR);//不需要,查询面板会弹出错误"请保证设备处于正常状态"

        if (mLog->isEnabled()) // 告知用户日志保存的文件夹路径
            showLogPathInfo(tr("日志默认保存的路径为:\n%1").arg(mLog->logDir()));
}

void Communication::tcpError(TCPERROR code)
{
    emit errorCode(code);
    if (code != TCPERROR::TcpNoError)
    {
          mTcpSocket->close();
          mTcpQueryTimer->stop();
          mTcpQueryThread->quit();
          mTcpQueryThread->wait();

          mTcpConfigureBtn->setEnabled(true);
          mTcpConnectBtn->setEnabled(false);
          mTcpDisconnectBtn->setEnabled(false);//禁用连接,防止用户再次点击连接造成异常退出

          Critical * dlg = new Critical;
          dlg->setText(tr("套接字错误类型: %1").arg(mTcpSocket->errorString()));
          dlg->setTextWrapIndent(2,20);
          dlg->exec();
          delete dlg;
    }
}

void Communication::tcpCycle()
{
        mTcpQueryWorker->fixedQuery();
        mLog->increaseCounter();
}

void Communication::tcpConfigure()
{
        QDialog * dlg = new QDialog;
        dlg->setWindowIcon(QIcon(":/images/portsettings.png"));
        QPalette pal = dlg->palette();
        pal.setBrush(QPalette::Background,QBrush(QPixmap(":/images/tcp.jpg")));
        dlg->setPalette(pal);
        dlg->setFont(QFont("Times New Roman",12));

        QLabel * IP = new QLabel(tr("IP地址："));
        QLabel * Port = new QLabel(tr("端口号："));
        QLineEdit * IPEdit = new QLineEdit(mTcpIP); // 保留上次的设置
        QLineEdit * PortID = new QLineEdit(QString::number(mTcpPort));
        QIntValidator *val = new QIntValidator(0,400000);
        PortID->setValidator(val); // 设置输入必须是范围内的整数
        QPushButton * okBtn = new QPushButton(tr("确定"));
        QPushButton * cancelBtn = new QPushButton(tr("取消"));
        QGridLayout * layout = new QGridLayout(dlg);

        connect(okBtn,&QPushButton::clicked,this,[=]{
              mTcpIP = IPEdit->text();
              mTcpPort = PortID->text().toUShort();
              QHostAddress addr;
              if (!addr.setAddress(mTcpIP))
              {
                  Critical * info = new Critical;
                  info->setText(tr("IP地址格式不合法!"));
                  info->setTextWrapIndent(2,23);
                  info->exec();
                  delete  info;
              }
              else dlg->accept();
        });
        connect(cancelBtn,SIGNAL(clicked()),dlg,SLOT(reject()));

        layout->addWidget(IP,0,0);
        layout->addWidget(IPEdit,0,1);
        layout->addWidget(Port,1,0);
        layout->addWidget(PortID,1,1);
        layout->addWidget(okBtn,2,0);
        layout->addWidget(cancelBtn,2,1);

        int ret = dlg->exec();

        if (ret == QDialog::Accepted)
        {
              mTcpIP = IPEdit->text();
              mTcpPort = PortID->text().toUShort();
              mTcpConnectBtn->setEnabled(true); //配置过就可用
        }
        delete dlg;
}

void Communication::tcpConnect()
{
        if (!needUpdateWorkMode()) return;
        mTcpSocket->connectToHost(mTcpIP,mTcpPort);
        if (!mTcpSocket->waitForConnected(5000)){
            QApplication::processEvents(QEventLoop::AllEvents, 5);
            Critical * info = new Critical;
            info->setText(tr("TCP通讯连接失败!"));
            info->setTextWrapIndent(2,23);
            info->exec();
            delete  info;
            return;
        }
        //if (!mTcpSocket->isOpen()) return;
        showInfo(tr("TCP通讯已连接!"));

        mCommunicationType = Type::Tcp;

        mTcpQueryThread->start();
        mTcpQueryTimer->start();

        mTcpConfigureBtn->setEnabled(false);
        mTcpConnectBtn->setEnabled(false);
        mTcpDisconnectBtn->setEnabled(true);

        mMonitor->show();

        emit currentDevice(mTcpSocket);
}

void Communication::tcpDisconnect()
{
        mTcpSocket->disconnectFromHost();
        mTcpQueryTimer->stop();
        mTcpQueryThread->quit();
        mTcpQueryThread->wait();

        mTcpConfigureBtn->setEnabled(true);
        mTcpConnectBtn->setEnabled(true);
        mTcpDisconnectBtn->setEnabled(false);

        mMonitor->close();
        mLog->reset();

        showInfo(tr("TCP通讯已断开!"));
        if (mLog->isEnabled()) // 告知用户日志保存的文件夹路径
            showLogPathInfo(tr("日志默认保存的路径为:\n%1").arg(mLog->logDir()));
}

void Communication::terminalQuery(const QByteArray& c)
{
    if (Query::controller() != ResponseProcessor::Controller::TerminalQuery)
        Query::setController(ResponseProcessor::Controller::TerminalQuery); // 告知IO设备回复的消息来自terminal
   if (mCommunicationType == Type::Serial)
       mUsbQueryWorker->terminalQuery(c);
   else if (mCommunicationType == Type::Tcp)
       mTcpQueryWorker->terminalQuery(c); // 无需角色更新,已验证是闭环逻辑
}
