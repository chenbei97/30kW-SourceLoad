#include "logperiod.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

LogPeriod::LogPeriod(QWidget*parent):QDialog(parent)
{
        mLogEnabled = new QCheckBox(tr("启用全局日志"));
        mLogEnabled->setCheckState(Qt::Unchecked); // 默认不启用

        mCsvRadioBtn = new QRadioButton("csv");
        mTxtRadioBtn = new QRadioButton("txt");
        mCsvRadioBtn->setChecked(true);// 默认csv
        mRadioBtnGroup = new QButtonGroup;
        mRadioBtnGroup->addButton(mCsvRadioBtn,0); // 默认csv格式
        mRadioBtnGroup->addButton(mTxtRadioBtn,1);
        QLabel * mLogFileType = new QLabel(tr("日志文件格式: "));
        QGroupBox * mRadioBox = new QGroupBox;
        QHBoxLayout * mBoxLay = new QHBoxLayout;
        mBoxLay->addWidget(mLogFileType);
        mBoxLay->addWidget(mCsvRadioBtn);
        mBoxLay->addWidget(mTxtRadioBtn);
        mRadioBox->setLayout(mBoxLay);
        mRadioBox->setStyleSheet("border:none;font-size:24px;font-family:Times New Roman;");
        mRadioBox->setEnabled(false);

        mPeriodList = new QComboBox;
        mPeriodList->setEnabled(false);
        QStringList periods;
        periods << QStringLiteral("300ms")<< QStringLiteral("600ms")<<  QStringLiteral("900ms")
                    <<QStringLiteral("1.5s")<< QStringLiteral("3s") << QStringLiteral("6s")<< QStringLiteral("9s")
                    << QStringLiteral("30s")<< QStringLiteral("1m") << QStringLiteral("10m")
                    << QStringLiteral("30m") << QStringLiteral("1h") << tr("自定义");
        mPeriodList->addItems(periods);
        mPeriodList->setEditable(false);
        mPeriodList->setInsertPolicy(QComboBox::InsertAlphabetically);
        mPeriodList->setCurrentIndex(6); // 默认9s记录1次
        QLabel * info = new QLabel(tr("选择日志周期: "));
        info->setEnabled(false);
        info->setBuddy(mPeriodList);

        mLabel1= new QLabel(tr("自定义周期单位: "));
        mUnitList = new QComboBox;
        QStringList units;
        units<<"ms"<<"s"<<"m"<<"h";
        mUnitList->addItems(units);
        mLabel2 = new QLabel(tr("自定义周期输入: "));
        mCustomPeriodEdit = new QLineEdit;
        QDoubleValidator * validator = new QDoubleValidator(0.0,10000.0,2);
        mCustomPeriodEdit->setValidator(validator);
        isInsertCustomPeriod = new QCheckBox(tr("保存自定义周期"));
        mOkBtn = new QPushButton(tr("完成输入"));
        setEnabled(false);

        mainLayout = new QGridLayout;
        mainLayout->addWidget(mLogEnabled,0,0);
        mainLayout->addWidget(mRadioBox,0,1);
        mainLayout->addWidget(info,1,0);
        mainLayout->addWidget(mPeriodList,1,1);
        mainLayout->addWidget(mLabel1,2,0);
        mainLayout->addWidget(mUnitList,2,1);
        mainLayout->addWidget(mLabel2,3,0);
        mainLayout->addWidget(mCustomPeriodEdit,3,1);
        mainLayout->addWidget(isInsertCustomPeriod,4,0);
        mainLayout->addWidget(mOkBtn,4,1);

        connect(mRadioBtnGroup,static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),
                this,[=](int id){emit logFileFormat(FILEFORMAT(id));});

        connect(mLogEnabled,static_cast<void (QCheckBox::*)(int)>(&QCheckBox::stateChanged),
                this,[=](int state){
                 if (state == Qt::Unchecked){
                     info->setEnabled(false);
                     mPeriodList->setEnabled(false);
                     mRadioBox->setEnabled(false);
                     setEnabled(false);
                     emit logEnabled(false);
                 }
                 else if (state == Qt::Checked) {
                     info->setEnabled(true);
                     mPeriodList->setEnabled(true);
                     mRadioBox->setEnabled(true);
                     emit logEnabled(true);
                     if (mPeriodList->currentText() != tr("自定义"))
                         setEnabled(false);
                     else setEnabled(true);
                 }
        });

        connect(mPeriodList,static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentTextChanged),
                this,[=](const QString&period){
                if (period == tr("自定义")) setEnabled(true); // 是自定义的点击完成输入按钮发出去
                else {
                    setEnabled(false);
                    emitPeriod(period); // 如果不是自定义的用户切换时就发出去
                }
        });
        connect(mOkBtn,&QPushButton::clicked,this,[=]{
              QString period = getCustomPeriod();

              if (period.isEmpty()) return;

              if (isInsertCustomPeriod->isChecked()) {
                  mPeriodList->insertItem(mPeriodList->count()-1,period);
                  mPeriodList->setCurrentText(period);
              }

              emitPeriod(period);

              Infomation * info = new Infomation;
              info->setText(tr("已成功设置日志周期为%1").arg(period));
              info->setTextWrapIndent(2,16);
              info->exec();
              delete info;

        });

        setWindowIcon(QIcon(":/images/tablelog.png"));
        setFont(QFont("Times New Roman",12));
        resize(QSize(400,120));
        setLayout(mainLayout);
}

void LogPeriod::setEnabled(bool checked)
{
    mOkBtn->setEnabled(checked);
    mUnitList->setEnabled(checked);
    mCustomPeriodEdit->setEnabled(checked);
    isInsertCustomPeriod->setEnabled(checked);
    mLabel1->setEnabled(checked);
    mLabel2->setEnabled(checked);
}

QString LogPeriod::getCustomPeriod()
{
        QString unit = "";
        double input = mCustomPeriodEdit->text().toDouble();
        double mul = input / 300.; // 判断浮点数是否为300.0浮点数的倍数
        if (mul != static_cast<quint32>(mul)) {//如果不是取整后不会相等
            Warning * warning = new Warning;
            warning->setDialogFixedSize(900,280);
            warning->setText(tr("推荐300ms的公倍数,否则将取整处理!"));
            warning->setTextWrapIndent(2,16);
            warning->exec();
            delete warning;
        }
        //qDebug()<<"input = "<<input <<" mul = " << mul;

        if (mUnitList->currentIndex() == 0){ // ms
        if (input - 300.0< 0.0) // 不支持300ms以下
        {
            Critical * c = new Critical;
            c->setDialogFixedSize(900,280);
            c->setText(tr("不支持小于300ms的日志周期!"));
            c->setTextWrapIndent(2,20);
            c->exec();
            delete c;
            return unit;
        }
        //qDebug()<<"here "<<QString("%1ms").arg(QString::number(input));
        return QString("%1ms").arg(QString::number(input));
        }
        else if (mUnitList->currentIndex() == 1){ // 0.35s
        if (input - 0.3< 0.0) // 不支持0.3s以下
        {
           Critical * c = new Critical;
           c->setDialogFixedSize(900,280);
           c->setText(tr("不支持小于0.3s的日志周期!"));
           c->setTextWrapIndent(2,20);
           c->exec();
           delete c;
           return unit;
        }
        return QString("%1s").arg(QString::number(input));
        }
        else if (mUnitList->currentIndex() == 2) { // 最小0.01min=0.01*60=0.6s>0.35s是支持的
        return QString("%1m").arg(QString::number(input));
        }
        else if (mUnitList->currentIndex() == 3) {// 0.01h = 0.01 * 3600=36s
        return QString("%1h").arg(QString::number(input));
        }
    return unit;
}

void LogPeriod::emitPeriod(QString period)
{
    QString value;
    if (period.endsWith("ms",Qt::CaseInsensitive)){
        value = period.remove("ms",Qt::CaseInsensitive);
        quint32 ms = value.toUInt();//ms只能输入>350的int或double
        if (ms == 0)
        {
            double val = value.toDouble();
            ms = qRound(val); // 直接取整即可
        }
        //qDebug()<<ms<<"ms";
        mPeriod.ms = ms;mPeriod.s = 0;mPeriod.m = 0;mPeriod.h = 0;
        emit selectedPeriod(ms);
    }
   else if (period.endsWith("s",Qt::CaseInsensitive)) {
           value = period.remove("s",Qt::CaseInsensitive);
           quint32 s = value.toUInt(); // 输入>0.35的int或者double
           if (s == 0) // 说明是浮点数
           {
               double val = value.toDouble();
               // 单位s的浮点数不能直接取整,误差过大
               quint32 ms = qRound(val * 1000); // 转为ms传递
               //qDebug()<<ms<<"ms/s";
               mPeriod.ms = ms;mPeriod.s = 0;mPeriod.m = 0;mPeriod.h = 0;
               emit selectedPeriod(ms);
           }
           else {
               //qDebug()<<s<<"s";
               mPeriod.ms = 0;mPeriod.s = s;mPeriod.m = 0;mPeriod.h = 0;
               emit selectedPeriod(0,s);
           }
   }
   else if (period.endsWith("m",Qt::CaseInsensitive))
   {
       value = period.remove("m",Qt::CaseInsensitive);
       quint32 m = value.toUInt(); // 输入可以是[0.01,+)
       if (m == 0)// 说明是浮点数
       {
           double val = value.toDouble();
           // 如果是0.01m=0.6s取整就是1s误差很大; 0.5m=30s,设定≥0.5m取整导致的损失忽略不计
           if (val < 0.5) //临界值0.5m=30s=30000ms,即<30000ms使用ms传递
           {
               quint32 ms = qRound(val * 60000); // 转为ms传递
               //qDebug()<<ms<<"ms/m";
               mPeriod.ms = ms;mPeriod.s = 0;mPeriod.m = 0;mPeriod.h = 0;
               emit selectedPeriod(ms);
           }
           else {
               quint32 s = qRound(val * 60); // 转为s传递
               //qDebug()<<s<<"s/m";
               mPeriod.ms = 0;mPeriod.s = s;mPeriod.m = 0;mPeriod.h = 0;
               emit selectedPeriod(0,s);
           }
       }
       else {
           //qDebug()<<m<<"m";
           mPeriod.ms = 0;mPeriod.s = 0;mPeriod.m = m;mPeriod.h = 0;
           emit selectedPeriod(0,0,m);
       }
   }
   else if (period.endsWith("h",Qt::CaseInsensitive))
   {
       value = period.remove("h",Qt::CaseInsensitive);
       quint32 h = value.toUInt();
       if (h == 0)
       {
           double val = value.toDouble(); // 0.01h=0.6m=36s,全部使用s来传递,取整损失忽略不计
           quint32 s = qRound(val * 3600);
           //qDebug()<<s<<"s/h";
           mPeriod.ms = 0;mPeriod.s = s;mPeriod.m = 0;mPeriod.h = 0;
           emit selectedPeriod(0,s);
       }
       else {
           //qDebug()<<h<<"h";
           mPeriod.ms = 0;mPeriod.s = 0;mPeriod.m = 0;mPeriod.h = h;
           emit selectedPeriod(0,0,0,h);
       }
   }
}

