#include "usbconfig.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

UsbConfig::UsbConfig(QWidget * parent) :QDialog(parent)
{
        setWindowIcon(QIcon(":/images/portsettings.png"));
        setFont(QFont("Times New Roman",12));
        setWindowTitle(tr("串口配置"));
        resize(QSize(650,300));
        initUI();
        initConnections();
}

void UsbConfig::initUI()
{
      this->initPortInformation();
      this->initPortConfig();
      mainLayout = new QGridLayout(this);
      mainLayout->addWidget(PortInformation,0,0,1,1);
      mainLayout->addWidget(PortConfig,0,1,1,1);
}

void UsbConfig::initPortInformation()
{
        PortInformation = new QGroupBox(tr("串口信息"));
        AvailPortsCombo = new QComboBox; // 有效串口
        PortNameLabel = new QLabel(tr("串口名称："));
        PortLocationLabel = new QLabel(tr("串口位置："));
        PortDescriptionLabel = new QLabel(tr("串口描述："));
        PortManufacturerLabel = new QLabel(tr("制造商："));
        PortVendorLabel = new QLabel(tr("供应商："));
        PortIdentifierLabel = new QLabel(tr("产品编号："));

        QVBoxLayout * PortLabelsLayout = new QVBoxLayout;
        PortLabelsLayout->addWidget(PortNameLabel);
        PortLabelsLayout->addWidget(PortLocationLabel);
        PortLabelsLayout->addWidget(PortDescriptionLabel);
        PortLabelsLayout->addWidget(PortManufacturerLabel);
        PortLabelsLayout->addWidget(PortVendorLabel);
        PortLabelsLayout->addWidget(PortIdentifierLabel);
        QVBoxLayout * PortInfomationLayout = new QVBoxLayout;
        PortInfomationLayout->addWidget(AvailPortsCombo);
        PortInfomationLayout->addLayout(PortLabelsLayout);
        PortInformation->setLayout(PortInfomationLayout);

         // 初始化有效串口及其信息
        QString name; // 串口名称
        QString location; // 串口位置
        QString description; // 描述信息
        QString manufacturer; // 制造商
        quint16 vendor; // 供应商
        quint16 identifier; // 产品编号
        const auto infos = QSerialPortInfo::availablePorts(); // 可用串口

        for (const QSerialPortInfo &info : infos)
        {
              QStringList  informations;
              name = info.portName();
              location = info.systemLocation();
              description = info.description();
              manufacturer = info.manufacturer();
              vendor = info.vendorIdentifier();
              identifier = info.productIdentifier();

              informations << name << location
                   << (!description.isEmpty() ? description : blankString)
                   << (!manufacturer.isEmpty() ? manufacturer : blankString)
                   << (vendor ? QString::number(info.vendorIdentifier(), 16) : blankString)
                   << (identifier ? QString::number(info.productIdentifier(), 16) : blankString);
              AvailPortsCombo->addItem(name,informations); // 把串口名称和对应的所有信息一起存进去便于切换串口时展示信息
        }

      on_PortChanged(0); // 初始化的时候需要先执行1次
}

void UsbConfig::initPortConfig()
{
        PortConfig = new QGroupBox(tr("串口配置"));
        PortBaudRateLabel = new QLabel(tr("波特率："));
        PortParityLabel = new QLabel(tr("奇偶位："));
        PortDataBitLabel = new QLabel(tr("数据位："));
        PortStopBitLabel = new QLabel(tr("停止位："));
        PortFlowControlLabel = new QLabel(tr("流控制："));

        PortBaudRateCombo = new QComboBox;
        PortParityCombo = new QComboBox;
        PortDataBitCombo = new QComboBox;
        PortStopBitCombo = new QComboBox;
        PortFlowControlCombo = new QComboBox;

        okBtn = new QPushButton(tr("确定"));

        QGridLayout * layout = new QGridLayout;
        layout->addWidget(PortBaudRateLabel,0,0);
        layout->addWidget(PortBaudRateCombo,0,1);
        layout->addWidget(PortParityLabel,1,0);
        layout->addWidget(PortParityCombo,1,1);
        layout->addWidget(PortDataBitLabel,2,0);
        layout->addWidget(PortDataBitCombo,2,1);
        layout->addWidget(PortStopBitLabel,3,0);
        layout->addWidget(PortStopBitCombo,3,1);
        layout->addWidget(PortFlowControlLabel,4,0);
        layout->addWidget(PortFlowControlCombo,4,1);
        layout->setRowStretch(5,0); // 空列
        layout->addWidget(okBtn,5,1);
        PortConfig->setLayout(layout);

         // 波特率
        QList<qint32> baudrates = QSerialPortInfo::standardBaudRates();
        QStringList stringBaudrates;
        foreach (const quint32& rate, baudrates)
                    stringBaudrates << QString::number(rate,10);
        PortBaudRateCombo->addItems(stringBaudrates);
        PortBaudRateCombo->addItem(tr("自定义")); // 支持自定义波特率
        PortBaudRateCombo->setEditable(false); // 默认不可编辑除非选择了自定义
        PortBaudRateCombo->setInsertPolicy(QComboBox::NoInsert); // 用户自定义的值不会作为新项插入
        PortBaudRateCombo->setCurrentIndex(6); // 对应9600的位置
        PortBaudRateCombo->setToolTip(tr("请注意,自定义波特率只支持[0,4000000]的范围!"));

        // 数据位
        PortDataBitCombo->addItem(QStringLiteral("5"), QSerialPort::Data5); // 存储到指定userData下
        PortDataBitCombo->addItem(QStringLiteral("6"), QSerialPort::Data6);
        PortDataBitCombo->addItem(QStringLiteral("7"), QSerialPort::Data7);
        PortDataBitCombo->addItem(QStringLiteral("8"), QSerialPort::Data8);
        PortDataBitCombo->setCurrentIndex(3); // 默认是Data8

        // 校验位
        PortParityCombo->addItem(QStringLiteral("None"), QSerialPort::NoParity);// 存储到指定userData下
        PortParityCombo->addItem(QStringLiteral("Even"), QSerialPort::EvenParity);
        PortParityCombo->addItem(QStringLiteral("Odd"), QSerialPort::OddParity);
        PortParityCombo->addItem(QStringLiteral("Mark"), QSerialPort::MarkParity);
        PortParityCombo->addItem(QStringLiteral("Space"), QSerialPort::SpaceParity);

        // 停止位
        PortStopBitCombo->addItem(QStringLiteral("1"), QSerialPort::OneStop);// // 存储到指定userData下
        #ifdef Q_OS_WIN // 此类型的停止位只支持window平台
        PortStopBitCombo->addItem(QStringLiteral("1.5"), QSerialPort::OneAndHalfStop);
        #endif
        PortStopBitCombo->addItem(QStringLiteral("2"), QSerialPort::TwoStop);
        PortStopBitCombo->setToolTip(tr("请注意,1.5只支持Windows平台!"));

        // 流控制
        PortFlowControlCombo->addItem(QStringLiteral("None"), QSerialPort::NoFlowControl);
        PortFlowControlCombo->addItem(QStringLiteral("RTS/CTS"), QSerialPort::HardwareControl);
        PortFlowControlCombo->addItem(QStringLiteral("XON/XOFF"), QSerialPort::SoftwareControl);
}

void UsbConfig::initConnections()
{
      // 串口
      connect(AvailPortsCombo,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                                     this,&UsbConfig::on_PortChanged); // 切换串口展示对应的信息,并更新设置
      // 波特率
      connect(PortBaudRateCombo,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                             this,&UsbConfig::on_BaudRateChanged); // 切换波特率更改保存的设置
      // 确定按钮
      connect(okBtn,&QPushButton::clicked,this, &UsbConfig::on_OkBtn);
}

void UsbConfig::paintEvent(QPaintEvent *e)
{
    QPainter p;
    p.begin(this);
    p.setOpacity(0.5);
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    QPixmap pix(":/images/serial.jpg");
    p.drawPixmap(QPoint(0,0),pix);
    p.end();
    Q_UNUSED(e);
}

UsbConfig::Settings UsbConfig::settings() const
{
        return mSettings;
}

void UsbConfig :: setSettings(const UsbConfig::Settings &settings)
{
      mSettings = settings;
      updateSettings();
}

void UsbConfig::updateSettings()
{
          AvailPortsCombo->setCurrentText(mSettings.Name);
          PortParityCombo->setCurrentText(mSettings.ParityString);
          PortDataBitCombo->setCurrentText(mSettings.DataBitString);
          PortStopBitCombo->setCurrentText(mSettings.StopBitString);
          PortFlowControlCombo->setCurrentText(mSettings.FlowControlString);


          QList<qint32> baudrates = QSerialPortInfo::standardBaudRates();
          if (!baudrates.contains(mSettings.BaudRate)) // 如果不是常见的值,说明是自定义的值,此时必须设为可编辑的才能成功赋值
              PortBaudRateCombo->setEditable(true);
          else PortBaudRateCombo->setEditable(false);
          PortBaudRateCombo->setCurrentText(mSettings.BaudRateString);

}
void UsbConfig::on_PortChanged(int index)
{
      if (index < -1) return;
      QStringList list = AvailPortsCombo->itemData(index).toStringList(); // 获取当前串口项的存储数据转为stringList
      // list的信息依次是串口名(0),串口位置(1),描述信息(2),制造商(3),供应商(4),和产品编号(5)
      // 使用三元运算符判断,没有这种信息的话就不打印.也就是空白字符串N/A

      PortNameLabel->setText(tr("串口名称：%1").arg(list.at(0)));
      PortLocationLabel->setText(tr("串口位置：%1").arg(list.count() > 1 ? list.at(1) : tr(blankString)));
      PortDescriptionLabel->setText(tr("串口描述：%1").arg(list.count() > 2 ? list.at(2) : tr(blankString)));
      PortManufacturerLabel->setText(tr("制造商：%1").arg(list.count() > 3 ? list.at(3) : tr(blankString)));
      PortVendorLabel->setText(tr("供应商：%1").arg(list.count() > 4 ? list.at(4) : tr(blankString)));
      PortIdentifierLabel->setText(tr("产品编号：%1").arg(list.count() > 5 ? list.at(5) : tr(blankString)));
}

void UsbConfig::on_BaudRateChanged(int index)
{

    if (index+1 == PortBaudRateCombo->count()) // 最后1个选项(自定义)
    {
        QStandardItemModel * model = qobject_cast<QStandardItemModel*>(PortBaudRateCombo->model());
        QStandardItem * item = model->item(index);
        item->setToolTip(tr("请注意,自定义波特率只支持[0,4000000]的范围!"));
        PortBaudRateCombo->setEditable(true); //可编辑,让用户自己输入
        PortBaudRateCombo->clearEditText();
        QLineEdit *lineEdit = PortBaudRateCombo->lineEdit();// 获取编辑框(QComboBox默认使用的代理是QLineEdit)
        lineEdit->setValidator(new QIntValidator(0, 4000000)); // 设置约束使之只能输入指定范围的整数
    }
    else PortBaudRateCombo->setEditable(false); //改回来不可编辑

}

void UsbConfig::on_OkBtn()
{
        // 串口名
        mSettings.Name = AvailPortsCombo->currentText();
        // 波特率
        mSettings.BaudRate = PortBaudRateCombo->currentText().toInt();
        mSettings.BaudRateString = QString::number(mSettings.BaudRate);
        // 数据位
        mSettings.DataBit = static_cast<QSerialPort::DataBits>(PortDataBitCombo->itemData(PortDataBitCombo->currentIndex()).toInt()); // 数据在初始化项时已经给了5,6,7,8
        mSettings.DataBitString = PortDataBitCombo->currentText();
        // 校验位
        mSettings.Parity = static_cast<QSerialPort::Parity>(PortParityCombo->itemData(PortParityCombo->currentIndex()).toInt());
        mSettings.ParityString = PortParityCombo->currentText();
        // 停止位
        mSettings.StopBit = static_cast<QSerialPort::StopBits>(PortStopBitCombo->itemData(PortStopBitCombo->currentIndex()).toInt());
        mSettings.StopBitString = PortStopBitCombo->currentText();
        // 流控制
        mSettings.FlowControl = static_cast<QSerialPort::FlowControl>(PortFlowControlCombo->itemData(PortFlowControlCombo->currentIndex()).toInt());
        mSettings.FlowControlString = PortFlowControlCombo->currentText();

         accept();
}

