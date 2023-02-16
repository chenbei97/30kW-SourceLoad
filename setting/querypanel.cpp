#include "querypanel.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

QueryPanel::QueryPanel(QWidget * parent):QDialog(parent)
{
        mDevice = Q_NULLPTR;

        mQueryPanel = new QListWidget;
        mDisplayPanel = new QTextEdit;

        QHBoxLayout * layout1 = new QHBoxLayout;
        layout1->addWidget(mQueryPanel);
        QGroupBox * box1 = new QGroupBox(tr("命令面板"));
        box1->setLayout(layout1);

        QHBoxLayout * layout2 = new QHBoxLayout;
        layout2->addWidget(mDisplayPanel);
        QGroupBox * box2 = new QGroupBox(tr("消息面板"));
        box2->setLayout(layout2);

        QSplitter * splitter = new QSplitter(Qt::Horizontal,this);
        splitter->setChildrenCollapsible(false); // 禁止小部件被压缩到0
        splitter->setOpaqueResize(false); // 调整大小时分割线是否透明
        splitter->addWidget(box1);
        splitter->addWidget(box2);
        splitter->setStretchFactor(0,3);
        splitter->setStretchFactor(1,5);

        QHBoxLayout * mainLayout = new QHBoxLayout;
        mainLayout->addWidget(splitter);

        initQueryPanel();

        setWindowFlags(Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);//添加最大最小化和关闭按钮
        setWindowFlag(Qt::Window); // 设置window属性就不会总是让界面在前或者在后
        Qt::WindowFlags flags = windowFlags();
        setWindowFlags(flags^Qt::WindowContextHelpButtonHint); // 去除帮助按钮
        resize(QSize(1200,600));
        setFont(QFont("Times New Roman",12));
        setLayout(mainLayout);

        connect(mQueryPanel,SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                this,SLOT(terminalQuery(QListWidgetItem*)));

        connect(this,SIGNAL(terminalQueryData(const QByteArray&)),
                this,SLOT(display(const QByteArray&)));
}

void QueryPanel::terminalQuery(QListWidgetItem * item)
{
    if (mDevice==Q_NULLPTR) {
        Critical * c = new Critical;
        c->setText(tr("请先连接设备!"));
        c->setTextWrapIndent(2,20);
        c->exec();
        delete c;
        return;
    }

    if (!mDevice->isOpen()){
        Critical * c = new Critical;
        c->setText(tr("请保证设备处于正常通讯状态!"));
        c->setTextWrapIndent(2,16);
        c->exec();
        delete c;
        return ;
    }

    QByteArray c = item->text().toLocal8Bit();

    emit setTerminalCommand(c); // 将命令c本身直接作为角色,还可以省去再翻译角色的步骤
    //mDevice->write(c+"\n");
    // 不在这里给设备写入信息,应该在communication的2个usbWorker和tcpWorker对象去写入
    // 这里只是把role和command传递过去,调用他们的terminalQuery方法去更新最终ResponseProcessor内部的role
    // 然后形成闭环,拿到回复的数据再原封不动的返还,TERMINALROLE其实没有任何作用只是为了验证一个闭环逻辑
    // MONITORROLE本质上也没用,唯一的用处是最后Monitor显示时候把角色翻译成文字,说明设置结果的成功与失败

    QString q = QTime::currentTime().toString("[hh:mm:ss:zzz]")+" request=>     {"+c+"}";

    mDisplayPanel->append(QString("<font color = blue>%1</font>").arg(q)); // 方法1使用QTextEdit独有的CSS3设置颜色,
   // 参考 https://blog.csdn.net/chenyijun/article/details/51226133

    //setTextLineColor(Qt::blue); // 方法2 使用QTextCharFormat和QTextCursor实现
    //mDisplayPanel->append(q); // QTextEdit和QPlainTextEdit均可用
}

void QueryPanel::display(const QByteArray&response)
{
    QString res = response;
    res.chop(1); // 数据需要去除"\n"
    QString r = QTime::currentTime().toString("[hh:mm:ss:zzz]")+" response<=     {"+res+"}\n";

    //mDisplayPanel->append(QString("<font color = red>%1</font> <br>").arg(r)); // 方法1使用QTextEdit独有的CSS3设置颜色,多换1行

    setTextLineColor(Qt::red); // 方法2 使用QTextCharFormat和QTextCursor实现
    mDisplayPanel->append(r);
}

void QueryPanel::setTextLineColor(const QColor &color)
{ // 参考 https://blog.csdn.net/iamtoful/article/details/6182445
    QTextCharFormat fmt;
    fmt.setForeground(color);//文字是前景色
    QTextCursor cursor = mDisplayPanel->textCursor();
    cursor.mergeCharFormat(fmt);//光标处合并格式
    mDisplayPanel->mergeCurrentCharFormat(fmt);// 然后文本编辑框合并格式
}

void QueryPanel::setDevice(QIODevice * device)
{
    mDevice = device;
}

QIODevice * QueryPanel::device() const
{
    return mDevice;
}


void QueryPanel::initQueryPanel()
{
      // spic命令来源于core/spic_command.h
    // 只支持查询命令和部分设置命令,这里的设置命令软件不会去验证是否设置成功,用户自行查询即可

    // IEEE488通用命令 16
    mQueryPanel->addItem("*RST");
    mQueryPanel->addItem("*CLS");
    mQueryPanel->addItem("*TRG");
    mQueryPanel->addItem("*WAL");
    mQueryPanel->addItem("*IDN");
    mQueryPanel->addItem("*TST");
    mQueryPanel->addItem("*ESR");
    mQueryPanel->addItem("*STB");
    mQueryPanel->addItem("*SRE?");
    mQueryPanel->addItem("*PSC?");
    mQueryPanel->addItem("*ESE?");
    mQueryPanel->addItem("*OPC");
    mQueryPanel->addItem("*OPC?");
    mQueryPanel->addItem("FORM:ASC");
    mQueryPanel->addItem("FORM:BIN");
    mQueryPanel->addItem("FORM?");
    // System子系统 18
    mQueryPanel->addItem("SYST:POS SAV0");
    mQueryPanel->addItem("SYST:POS RST");
    mQueryPanel->addItem("SYST:POS?");
    mQueryPanel->addItem("SYST:VERS?");
    mQueryPanel->addItem("SYST:ERR?");
    mQueryPanel->addItem("SYST:CLE");
    mQueryPanel->addItem("SYST:REM");
    mQueryPanel->addItem("SYST:LOC");
    mQueryPanel->addItem("SYST:RWL");
    mQueryPanel->addItem("SYST:DATE?");
    mQueryPanel->addItem("SYST:TIME?");
    mQueryPanel->addItem("SYST:BEEP?");
    mQueryPanel->addItem("SYST:COMM:SEL?");
    mQueryPanel->addItem("SYST:COMM:SER:BAUD?");
    mQueryPanel->addItem("SYST:COMM:LAN:IP?");
    mQueryPanel->addItem("SYST:COMM:LAN:IP:MODE?");
    mQueryPanel->addItem("SYST:COMM:LAN:SMAS?");
    mQueryPanel->addItem("SYST:COMM:LAN:DGAT?");
    // SOURce子系统 105
    mQueryPanel->addItem("SOUR:FUNC?"); // cv模式的2A+V
    mQueryPanel->addItem("SOUR:CURR:LIM?");
    mQueryPanel->addItem("SOUR:CURR:LIM? DEF");
    mQueryPanel->addItem("SOUR:CURR:LIM? MIN");
    mQueryPanel->addItem("SOUR:CURR:LIM? MAX");
    mQueryPanel->addItem("SOUR:CURR:LIM:NEG?");
    mQueryPanel->addItem("SOUR:CURR:LIM:NEG? DEF");
    mQueryPanel->addItem("SOUR:CURR:LIM:NEG? MIN");
    mQueryPanel->addItem("SOUR:CURR:LIM:NEG? MAX");
    mQueryPanel->addItem("SOUR:VOLT?");
    mQueryPanel->addItem("SOUR:VOLT? DEF");
    mQueryPanel->addItem("SOUR:VOLT? MIN");
    mQueryPanel->addItem("SOUR:VOLT? MAX");
    mQueryPanel->addItem("SOUR:VOLT:LIM?"); // cc模式的2V+A
    mQueryPanel->addItem("SOUR:VOLT:LIM? DEF");
    mQueryPanel->addItem("SOUR:VOLT:LIM? MIN");
    mQueryPanel->addItem("SOUR:VOLT:LIM? MAX");
    mQueryPanel->addItem("SOUR:VOLT:LIM:NEG?");
    mQueryPanel->addItem("SOUR:VOLT:LIM:NEG? DEF");
    mQueryPanel->addItem("SOUR:VOLT:LIM:NEG? MIN");// 20
    mQueryPanel->addItem("SOUR:VOLT:LIM:NEG? MAX");
    mQueryPanel->addItem("SOUR:CURR?");
    mQueryPanel->addItem("SOUR:CURR? DEF");
    mQueryPanel->addItem("SOUR:CURR? MIN");
    mQueryPanel->addItem("SOUR:CURR? MAX");
    mQueryPanel->addItem("SOUR:POW:LIM?"); // P+R 180row
    mQueryPanel->addItem("SOUR:POW:LIM? DEF");
    mQueryPanel->addItem("SOUR:POW:LIM? MIN");
    mQueryPanel->addItem("SOUR:POW:LIM? MAX");
    mQueryPanel->addItem("SOUR:POW:LIM:NEG?");
    mQueryPanel->addItem("SOUR:POW:LIM:NEG? DEF");
    mQueryPanel->addItem("SOUR:POW:LIM:NEG? MIN");
    mQueryPanel->addItem("SOUR:POW:LIM:NEG? MAX");
    mQueryPanel->addItem("SOUR:RES:LIM?");
    mQueryPanel->addItem("SOUR:RES:LIM? DEF");
    mQueryPanel->addItem("SOUR:RES:LIM? MIN");
    mQueryPanel->addItem("SOUR:RES:LIM? MAX");
    mQueryPanel->addItem("SOUR:RES:LIM:NEG?");
    mQueryPanel->addItem("SOUR:RES:LIM:NEG? DEF");
    mQueryPanel->addItem("SOUR:RES:LIM:NEG? MIN"); // 40
    mQueryPanel->addItem("SOUR:RES:LIM:NEG? MAX");
    mQueryPanel->addItem("SOUR:VOLT:SLEW? DEF");
    mQueryPanel->addItem("SOUR:VOLT:SLEW? MIN");
    mQueryPanel->addItem("SOUR:VOLT:SLEW? MAX");
    mQueryPanel->addItem("SOUR:VOLT:SLEW:POS? DEF");
    mQueryPanel->addItem("SOUR:VOLT:SLEW:POS? MIN");
    mQueryPanel->addItem("SOUR:VOLT:SLEW:POS? MAX");
    mQueryPanel->addItem("SOUR:VOLT:SLEW:NEG? DEF");
    mQueryPanel->addItem("SOUR:VOLT:SLEW:NEG? MIN");
    mQueryPanel->addItem("SOUR:VOLT:SLEW:NEG? MAX");
    mQueryPanel->addItem("SOUR:CURR:SLEW? DEF");
    mQueryPanel->addItem("SOUR:CURR:SLEW? MIN");
    mQueryPanel->addItem("SOUR:CURR:SLEW? MAX");
    mQueryPanel->addItem("SOUR:CURR:SLEW:POS? DEF");
    mQueryPanel->addItem("SOUR:CURR:SLEW:POS? MIN");
    mQueryPanel->addItem("SOUR:CURR:SLEW:POS? MAX");
    mQueryPanel->addItem("SOUR:CURR:SLEW:NEG? DEF");
    mQueryPanel->addItem("SOUR:CURR:SLEW:NEG? MIN");
    mQueryPanel->addItem("SOUR:CURR:SLEW:NEG? MAX");
    mQueryPanel->addItem("SOUR:VOLT:PROT:STAT ON"); // 60
    mQueryPanel->addItem("SOUR:VOLT:PROT:STAT OFF");
    mQueryPanel->addItem("SOUR:VOLT:PROT:STAT?");
    mQueryPanel->addItem("SOUR:VOLT:PROT?");
    mQueryPanel->addItem("SOUR:VOLT:PROT:DEL? DEF");
    mQueryPanel->addItem("SOUR:VOLT:PROT:DEL? MIN");
    mQueryPanel->addItem("SOUR:VOLT:PROT:DEL? MAX");
    mQueryPanel->addItem("SOUR:VOLT:UND:PROT:STAT ON");
    mQueryPanel->addItem("SOUR:VOLT:UND:PROT:STAT OFF");
    mQueryPanel->addItem("SOUR:VOLT:UND:PROT:STAT?");
    mQueryPanel->addItem("SOUR:VOLT:UND:PROT?");
    mQueryPanel->addItem("SOUR:VOLT:UND:PROT:DEL? DEF");
    mQueryPanel->addItem("SOUR:VOLT:UND:PROT:DEL? MIN");
    mQueryPanel->addItem("SOUR:VOLT:UND:PROT:DEL? MAX");
    mQueryPanel->addItem("SOUR:CURR:PROT:STAT ON");
    mQueryPanel->addItem("SOUR:CURR:PROT:STAT OFF");
    mQueryPanel->addItem("SOUR:CURR:PROT:STAT?");
    mQueryPanel->addItem("SOUR:CURR:PROT?");
    mQueryPanel->addItem("SOUR:CURR:PROT:DEL? DEF");
    mQueryPanel->addItem("SOUR:CURR:PROT:DEL? MIN");
    mQueryPanel->addItem("SOUR:CURR:PROT:DEL? MAX"); // 80
    mQueryPanel->addItem("SOUR:CURR:UND:PROT:STAT ON");
    mQueryPanel->addItem("SOUR:CURR:UND:PROT:STAT OFF");
    mQueryPanel->addItem("SOUR:CURR:UND:PROT:STAT?");
    mQueryPanel->addItem("SOUR:CURR:UND:PROT?");
    mQueryPanel->addItem("SOUR:CURR:UND:PROT:DEL? DEF");
    mQueryPanel->addItem("SOUR:CURR:UND:PROT:DEL? MIN");
    mQueryPanel->addItem("SOUR:CURR:UND:PROT:DEL? MAX");
    mQueryPanel->addItem("SOUR:POW:PROT:STAT ON");
    mQueryPanel->addItem("SOUR:POW:PROT:STAT OFF");
    mQueryPanel->addItem("SOUR:POW:PROT:STAT?");
    mQueryPanel->addItem("SOUR:POW:PROT?");
    mQueryPanel->addItem("SOUR:POW:PROT:DEL? DEF");
    mQueryPanel->addItem("SOUR:POW:PROT:DEL? MIN");
    mQueryPanel->addItem("SOUR:POW:PROT:DEL? MAX");
    mQueryPanel->addItem("SOUR:VOLT:UND:PROT:WARM? DEF");
    mQueryPanel->addItem("SOUR:VOLT:UND:PROT:WARM? MIN");
    mQueryPanel->addItem("SOUR:VOLT:UND:PROT:WARM? MAX");
    mQueryPanel->addItem("SOUR:CURR:UND:PROT:WARM? DEF");
    mQueryPanel->addItem("SOUR:CURR:UND:PROT:WARM? MIN");
    mQueryPanel->addItem("SOUR:CURR:UND:PROT:WARM? MAX"); // 100
    mQueryPanel->addItem("SOUR:FUNC:MODE?");
    mQueryPanel->addItem("SOUR:EXT:PROG 1");
    mQueryPanel->addItem("SOUR:EXT:PROG 0");
    mQueryPanel->addItem("SOUR:EXT:PROG?");
    mQueryPanel->addItem("SOUR:FILT:LEV?"); // 105
    // Output子系统 19
    mQueryPanel->addItem("OUTP ON");
    mQueryPanel->addItem("OUTP OFF");
    mQueryPanel->addItem("OUTP:DEL:FALL? DEF");
    mQueryPanel->addItem("OUTP:DEL:FALL? MIN");
    mQueryPanel->addItem("OUTP:DEL:FALL? MAX");
    mQueryPanel->addItem("OUTP:DEL:RISE? DEF");
    mQueryPanel->addItem("OUTP:DEL:RISE? MIN");
    mQueryPanel->addItem("OUTP:DEL:RISE? MAX");
    mQueryPanel->addItem("OUTP:INH:MODE?");
    mQueryPanel->addItem("OUTP:PON:STAT LAST_ON");
    mQueryPanel->addItem("OUTP:PON:STAT LAST_OFF");
    mQueryPanel->addItem("OUTP:PON:STAT?");
    mQueryPanel->addItem("OUTP:PROT:CLE");
    mQueryPanel->addItem("OUTP:PROT:WDOG ON");
    mQueryPanel->addItem("OUTP:PROT:WDOG OFF");
    mQueryPanel->addItem("OUTP:PROT:WDOG?");
    mQueryPanel->addItem("OUTP:PROT:WDOG:DEL? DEF");
    mQueryPanel->addItem("OUTP:PROT:WDOG:DEL? MIN");
    mQueryPanel->addItem("OUTP:PROT:WDOG:DEL? MAX");
    // STATus子系统 9
    mQueryPanel->addItem("STAT:OPER?");
    mQueryPanel->addItem("STAT:OPER:ENAB?");
    mQueryPanel->addItem("STAT:OPER:COND?");
    mQueryPanel->addItem("STAT:OPER:PTR?");
    mQueryPanel->addItem("STAT:OPER:NTR?");
    mQueryPanel->addItem("STAT:QUES:ENAB?");
    mQueryPanel->addItem("STAT:QUES:COND?");
    mQueryPanel->addItem("STAT:QUES:PTR?");
    mQueryPanel->addItem("STAT:QUES:NTR?");
    // MEAS子系统 21
    mQueryPanel->addItem("MEAS:CURR?");
    mQueryPanel->addItem("MEAS:CURR:MAX?");
    mQueryPanel->addItem("MEAS:CURR:MIN?");
    mQueryPanel->addItem("MEAS:CURR:HIGH?");
    mQueryPanel->addItem("MEAS:CURR:LOW?");
    mQueryPanel->addItem("MEAS:ARR:CURR?");
    mQueryPanel->addItem("MEAS:VOLT?");
    mQueryPanel->addItem("MEAS:VOLT:MAX?");
    mQueryPanel->addItem("MEAS:VOLT:MIN?");
    mQueryPanel->addItem("MEAS:VOLT:HIGH?");
    mQueryPanel->addItem("MEAS:VOLT:LOW?");
    mQueryPanel->addItem("MEAS:ARR:VOLT?");
    mQueryPanel->addItem("MEAS:POW?");
    mQueryPanel->addItem("MEAS:POW:MAX?");
    mQueryPanel->addItem("MEAS:POW:MIN?");
    mQueryPanel->addItem("MEAS:ARR:POW?");
    mQueryPanel->addItem("MEAS?");
    mQueryPanel->addItem("MEAS:LOCAI:VOLT?");
    mQueryPanel->addItem("MEAS:REMO:VOLT?");
    mQueryPanel->addItem("MEAS:AHO?");
    mQueryPanel->addItem("MEAS:WHO?");
}
