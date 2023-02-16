#include "monitor.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

void Monitor::initCommonUI() // 布局初始化的公共部分
{
      ConnectState = new QLabel(tr("连接状态：未知")); // 共用的,无论USB还是TCP或其它
      MessageRequestState = new QLabel(tr("消息请求状态：未知")); // 同理
      MessageResponseState = new QLabel(tr("消息回复状态：未知")); // 同理
      MessageSetState = new QLabel(tr("设定状态：暂无设定角色")); // 同理

      ConnectState->setMinimumSize(300,20);
      MessageRequestState->setMinimumSize(220,20);
      MessageResponseState->setMinimumSize(220,20);
      MessageSetState->setMinimumSize(300,20);

      ConnectState->setStyleSheet("color:red;");
      MessageRequestState->setStyleSheet("color:red;");
      MessageResponseState->setStyleSheet("color:red;");
      MessageSetState->setStyleSheet("color:red;");

      statusBar()->addWidget(ConnectState);
      statusBar()->addWidget(MessageRequestState);
      statusBar()->addWidget(MessageResponseState);
      statusBar()->addWidget(MessageSetState);

      mainLayout = new QSplitter(Qt::Vertical,this);
      mainLayout->setOpaqueResize(false); // 可以出现阴影条显示要移动的位置
      mainLayout->setHandleWidth(10);
      mainLayout->setChildrenCollapsible(false);//防止调整分割条把其它子部件挤没了
      setCentralWidget(mainLayout);

      VoltageGroupBox = new QGroupBox(tr("电压面板"),this);
      CurrentGroupBox = new QGroupBox(tr("电流面板"),this);

      // 对MonitorSpinBox这里要提供this,这样此控件可以跟随窗口的变化而变化
      // MonitorLabel不要提供this,否则是固定的宽度,这里希望跟随splitter的效果自由伸缩
      // （1）功率面板设置
      PowerGroupBox = new QGroupBox(tr("功率面板"),this);//功率的容器
      ShowPower = new MonitorLabel(QStringLiteral("00.000 W"));  // 功率的组件
      SetPowerSourceLabel = new QLabel(tr("源功率设置："));
      SetPowerSource = new MonitorSpinBox(this);
      SetPowerLoadLabel = new QLabel(tr("载功率设置："));
      SetPowerLoad = new MonitorSpinBox(this);
      SetPowerSource->setSuffix(QStringLiteral(" W"));
      SetPowerLoad->setSuffix(QStringLiteral(" W"));
      ShowPower->installEventFilter(this);// 使用事件过滤器实现悬停标签字体变大的效果
      connect(SetPowerSource,static_cast<void (QDoubleSpinBox::*)(double) > (&QDoubleSpinBox::valueChanged),
              this,[=](double value){emit setMonitorRole(MONITORROLE::PowerSource,QString::number(value,'f',3).toLocal8Bit());
      });
      connect(SetPowerLoad,static_cast<void (QDoubleSpinBox::*)(double) > (&QDoubleSpinBox::valueChanged),
              this,[=](double value){emit setMonitorRole(MONITORROLE::PowerLoad,QString::number(value,'f',3).toLocal8Bit());
      });

      QGridLayout * PowerLayout = new QGridLayout; // 功率的布局
      PowerLayout->addWidget(ShowPower,0,0,4,1);
      PowerLayout->addWidget(SetPowerSourceLabel,0,1);
      PowerLayout->addWidget(SetPowerSource,1,1);
      PowerLayout->addWidget(SetPowerLoadLabel,2,1);
      PowerLayout->addWidget(SetPowerLoad,3,1);
      PowerGroupBox->setLayout(PowerLayout); // 功率容器设置布局

      // （2） 电阻面板设置
      IsShowResistanceLabel = new QLabel(tr("是否显示内阻面板?"));
      IsShowResistance = new QCheckBox;

      ResistanceGroupBox = new QGroupBox(tr("内阻面板")); // 电阻容器
      ShowResistance = new MonitorLabel(QStringLiteral("00.000 Ω"));  // 功率的组件
      SetResistanceSourceLabel = new QLabel(tr("源内阻设置："));
      SetResistanceSource = new MonitorSpinBox(this);
      SetResistanceLoadLabel = new QLabel(tr("载内阻设置："));
      SetResistanceLoad = new MonitorSpinBox(this);
      SetResistanceSource->setSuffix(QStringLiteral(" Ω"));
      SetResistanceLoad->setSuffix(QStringLiteral(" Ω"));
      ShowResistance->installEventFilter(this);// 使用事件过滤器实现悬停标签字体变大的效果

      connect(IsShowResistance,&QCheckBox::stateChanged,this,
              [=](bool checked){checked?ResistanceGroupBox->show():ResistanceGroupBox->hide();
      });

      connect(SetResistanceSource,static_cast<void (QDoubleSpinBox::*)(double) > (&QDoubleSpinBox::valueChanged),
            this, [=](double value){emit setMonitorRole(MONITORROLE::ResistanceSource,QString::number(value,'f',3).toLocal8Bit());
      });
      connect(SetResistanceLoad,static_cast<void (QDoubleSpinBox::*)(double) > (&QDoubleSpinBox::valueChanged),
            this,[=](double value){emit setMonitorRole(MONITORROLE::ResistanceLoad,QString::number(value,'f',3).toLocal8Bit());
     });

      QGridLayout * ResistanceLayout = new QGridLayout; // 功率的布局
      ResistanceLayout->addWidget(ShowResistance,0,0,4,1);
      ResistanceLayout->addWidget(SetResistanceSourceLabel,0,1);
      ResistanceLayout->addWidget(SetResistanceSource,1,1);
      ResistanceLayout->addWidget(SetResistanceLoadLabel,2,1);
      ResistanceLayout->addWidget(SetResistanceLoad,3,1);
      ResistanceGroupBox->setLayout(ResistanceLayout); // 电阻容器设置布局
      IsShowResistance->setChecked(false);// 默认不显示电阻面板
      ResistanceGroupBox->hide(); // 先执行一次,隐藏电阻面板

      // （3）电压和电流的显示面板设置，不管是CC还是CV，都需要初始化，只是添加进布局的顺序不同
      ShowVoltage = new MonitorLabel(QStringLiteral("00.000 V")); // 电压读取1个
      ShowCurrent = new MonitorLabel(QStringLiteral("00.000 A")); // 电流读取1个
      ShowVoltage->installEventFilter(this); // 使用事件过滤器实现悬停标签字体变大的效果
      ShowCurrent->installEventFilter(this); // 使用事件过滤器实现悬停标签字体变大的效果
}
void Monitor::initCVUI()
{
      this->initCommonUI();
      // （1）面板组件设置
      // （1.1）电压面板组件设置
      QLabel * SetVoltageLabel = new QLabel(tr("电压设置："));
      MonitorSpinBox * SetVoltage = new MonitorSpinBox(this);// 电压设置1个
      SetVoltage->setSuffix(QStringLiteral(" V"));
      SetVoltage->setMaximum(50.0);
      connect(SetVoltage,static_cast<void (QDoubleSpinBox::*)(double) > (&QDoubleSpinBox::valueChanged),
              this,[=](double value){emit setMonitorRole(MONITORROLE::Voltage,QString::number(value,'f',3).toLocal8Bit());
       });
      // （1.2） 电流面板组件设置
      QLabel * SetCurrentSourceLabel = new QLabel(tr("源电流设置：")); // 电流设置2个,源和载
      MonitorSpinBox * SetCurrentSource = new MonitorSpinBox(this);
      QLabel * SetCurrentLoadLabel = new QLabel(tr("载电流设置："));
      MonitorSpinBox * SetCurrentLoad = new MonitorSpinBox(this);
      SetCurrentSource->setSuffix(QStringLiteral(" A"));
      SetCurrentLoad->setSuffix(QStringLiteral(" A"));
      connect(SetCurrentSource,static_cast<void (QDoubleSpinBox::*)(double) > (&QDoubleSpinBox::valueChanged),
              this,[=](double value){emit setMonitorRole(MONITORROLE::CurrentSource,QString::number(value,'f',3).toLocal8Bit());
      });
      connect(SetCurrentLoad,static_cast<void (QDoubleSpinBox::*)(double) > (&QDoubleSpinBox::valueChanged),
              this,[=](double value){emit setMonitorRole(MONITORROLE::CurrentLoad,QString::number(value,'f',3).toLocal8Bit());
      });
      //*********************************************************************************
      // （2）布局设置
      // （2.1）电压布局设置
      QGridLayout * VoltageLayout = new QGridLayout;
      VoltageLayout->addWidget(ShowVoltage,0,0,4,1);
      VoltageLayout->addWidget(SetVoltageLabel,0,1);
      VoltageLayout->addWidget(SetVoltage,1,1);
      VoltageLayout->addWidget(IsShowResistanceLabel,2,1);
      VoltageLayout->addWidget(IsShowResistance,3,1,Qt::AlignRight);
      VoltageGroupBox->setLayout(VoltageLayout);
      // （2.2）电流布局设置
      QGridLayout * CurrentLayout = new QGridLayout; // 电流的布局
      CurrentLayout->addWidget(ShowCurrent,0,0,4,1);
      CurrentLayout->addWidget(SetCurrentSourceLabel,0,1);
      CurrentLayout->addWidget(SetCurrentSource,1,1);
      CurrentLayout->addWidget(SetCurrentLoadLabel,2,1);
      CurrentLayout->addWidget(SetCurrentLoad,3,1);
      CurrentGroupBox->setLayout(CurrentLayout); // 电流容器设置布局

      mainLayout->addWidget(VoltageGroupBox);
      mainLayout->addWidget(CurrentGroupBox);
      mainLayout->addWidget(PowerGroupBox);
      mainLayout->addWidget(ResistanceGroupBox);//默认初始化加电阻面板
}

void Monitor::initCCUI()
{
      this->initCommonUI();
        // （1）面板组件设置
      // （1.1）电流面板组件设置
      QLabel * SetCurrentLabel = new QLabel(tr("电流设置："));
      MonitorSpinBox * SetCurrent = new MonitorSpinBox(this);// 电流设置1个
      SetCurrent->setSuffix(QStringLiteral(" A"));
      connect(SetCurrent,static_cast<void (QDoubleSpinBox::*)(double) > (&QDoubleSpinBox::valueChanged),
              this,[=](double value){ emit setMonitorRole(MONITORROLE::Current,QString::number(value,'f',3).toLocal8Bit());
      });
      // （1.2） 电压面板组件设置
      QLabel * SetVoltageSourceLabel = new QLabel(tr("源电压设置：")); // 电压设置2个,源和载
      MonitorSpinBox * SetVoltageSource = new MonitorSpinBox(this);
      QLabel * SetVoltageLoadLabel = new QLabel(tr("载电压设置："));
      MonitorSpinBox * SetVoltageLoad = new MonitorSpinBox(this);
      SetVoltageSource->setSuffix(QStringLiteral(" V"));
      SetVoltageLoad->setSuffix(QStringLiteral(" V"));
      connect(SetVoltageSource,static_cast<void (QDoubleSpinBox::*)(double) > (&QDoubleSpinBox::valueChanged),
              this,[=](double value){emit setMonitorRole(MONITORROLE::VoltageSource,QString::number(value,'f',3).toLocal8Bit());
      });
      connect(SetVoltageLoad,static_cast<void (QDoubleSpinBox::*)(double) > (&QDoubleSpinBox::valueChanged),
              this,[=](double value){emit setMonitorRole(MONITORROLE::VoltageLoad,QString::number(value,'f',3).toLocal8Bit());
       });
      //*********************************************************************************
      // （2）布局设置
      // （2.1）电流布局设置
      QGridLayout * CurrentLayout = new QGridLayout;
      CurrentLayout->addWidget(ShowCurrent,0,0,4,1);
      CurrentLayout->addWidget(SetCurrentLabel,0,1);
      CurrentLayout->addWidget(SetCurrent,1,1);
      CurrentLayout->addWidget(IsShowResistanceLabel,2,1);
      CurrentLayout->addWidget(IsShowResistance,3,1,Qt::AlignRight);
      CurrentGroupBox->setLayout(CurrentLayout);
      // （2.2）电压布局设置
      QGridLayout * VoltageLayout = new QGridLayout; // 电压的布局
      VoltageLayout->addWidget(ShowVoltage,0,0,4,1);
      VoltageLayout->addWidget(SetVoltageSourceLabel,0,1);
      VoltageLayout->addWidget(SetVoltageSource,1,1);
      VoltageLayout->addWidget(SetVoltageLoadLabel,2,1);
      VoltageLayout->addWidget(SetVoltageLoad,3,1);
      VoltageGroupBox->setLayout(VoltageLayout); // 电压容器设置布局

      mainLayout->addWidget(CurrentGroupBox);
      mainLayout->addWidget(VoltageGroupBox);
      mainLayout->addWidget(PowerGroupBox);
      mainLayout->addWidget(ResistanceGroupBox);//默认初始化加电阻面板
}

void Monitor::showEvent(QShowEvent *e)
{
     QRect screenRect = QApplication::desktop()->screenGeometry(); // 屏幕大小
     this->move(screenRect.width() / 8,screenRect.height()/16); // 移动到一个电脑桌面相对居中的位置
     e->accept();
}

bool Monitor:: eventFilter(QObject* watchedObj, QEvent * event)
{
      if (watchedObj == ShowPower)
      {
          if (event->type() == QEvent::HoverEnter )
          {
                // 加了背景图片，色调是蓝色的，所以文字改成红色，背景颜色不要
               //ShowPower->setStyleSheet("QLabel{font:bold 128px;background-color: red;color: blue;}");
               ShowPower->setStyleSheet("QLabel{font:bold 128px;color: red;}");

          }
          if (event->type() == QEvent::HoverLeave)
          {
                ShowPower->setStyleSheet("QLabel{font:bold 64px;color: red;}");
          }
      }
      else if (watchedObj == ShowResistance)
      {
           if (event->type() == QEvent::HoverEnter )
          {
               //ShowResistance->setStyleSheet("QLabel{font:bold 128px;background-color: red;color: blue;}");
               ShowResistance->setStyleSheet("QLabel{font:bold 128px;color: red;}");

          }
          if (event->type() == QEvent::HoverLeave)
          {
                ShowResistance->setStyleSheet("QLabel{font:bold 64px;color: red;}");
          }
      }
      else if (watchedObj == ShowCurrent)
      {
           if (event->type() == QEvent::HoverEnter )
          {
               //ShowCurrent->setStyleSheet("QLabel{font:bold 128px;background-color: red;color: blue;}");
               ShowCurrent->setStyleSheet("QLabel{font:bold 128px;color: red;}");

          }
          if (event->type() == QEvent::HoverLeave)
          {
                ShowCurrent->setStyleSheet("QLabel{font:bold 64px;color: red;}");
          }
      }
      else if (watchedObj == ShowVoltage)
      {
           if (event->type() == QEvent::HoverEnter )
          {
               //ShowVoltage->setStyleSheet("QLabel{font:bold 128px;background-color: red;color: blue;}");
               ShowVoltage->setStyleSheet("QLabel{font:bold 128px;color: red;}");

          }
          if (event->type() == QEvent::HoverLeave)
          {
                ShowVoltage->setStyleSheet("QLabel{font:bold 64px;color: red;}");
          }
      }
      return QMainWindow::eventFilter(watchedObj,event);
}
