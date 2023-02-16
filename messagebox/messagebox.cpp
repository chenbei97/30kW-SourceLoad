#include "messagebox.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

Messagebox::Messagebox(QWidget*parent):QDialog(parent)
{
        mShakeAnimation = Q_NULLPTR;

        setDialogIcon(QIcon(":/images/textIcon.png"));
        setDialogTitle(tr("消息对话框"));
        setDialogFont(QFont("Times New Roman",12));
        setDialogFixedSize(800,280);
        Qt::WindowFlags flags = windowFlags();
        setWindowFlags(flags^Qt::WindowContextHelpButtonHint);

        mIconLabel = new QLabel;
        setPixmap(QPixmap(":/images/textIcon.png"));
        mIconLay = new QHBoxLayout;
        mIconLay->addWidget(mIconLabel);
        mIconBox = new QGroupBox;
        setPixmapBoxStyleSheet("border: 2px solid rgb(18,150,220);border-radius:10px;");
        mIconBox->setLayout(mIconLay);

        mTextEdit = new QTextEdit(tr("尊敬的用户您好,欢迎使用!"));
        mTextEdit->setLineWrapMode(QTextEdit::WidgetWidth);
        mTextEdit->setLineWrapColumnOrWidth(50);
        mTextEdit->setReadOnly(true);
        mTextEdit->setLineWidth(10);
        mTextEdit->setMidLineWidth(20);
        mTextEdit->setFrameShape(QFrame::Panel);
        mTextEdit->setFrameStyle(QFrame::Raised);
        mTextLay = new QHBoxLayout;
        mTextLay->addWidget(mTextEdit);
        mTextBox = new QGroupBox;
        mTextBox->setLayout(mTextLay);
        setTextBoxStyleSheet("border: 2px solid rgb(18,150,220);"); //会覆盖QTextEdit的格式
        setTextEditStyleSheet("color:rgb(18,150,220);font-size:24px;font-weight:bold;border:none;"
                                 "font-family:Times New Roman;text-align: center;");//background-image:url(:/images/textEditBack.jpg);有点丑不放进去了
        setTextWeight(QFont::Bold);
        setTextColor(Qt::black);
        setTextWrapIndent(2,16); // 缩进需要根据文字大概自行调整

        mTopLay = new QHBoxLayout;
        mTopLay->addWidget(mIconBox);
        mTopLay->addWidget(mTextBox);
        mOkBtn = new MessageButton(tr("确定"),0);
        mCancelBtn = new MessageButton(tr("取消"),1);
        mOkBtn->button()->setStyleSheet("color:rgb(18,150,220);border:1px solid rgb(18,150,220);font-size:24px;"
                                          "font-family:Times New Roman;min-width: 120px;min-height: 40px;");
        mCancelBtn->button()->setStyleSheet("color:rgb(18,150,220);border:1px solid rgb(18,150,220);font-size:24px;"
                                          "font-family:Times New Roman;min-width: 120px;min-height: 40px;");
        mBtns << mOkBtn << mCancelBtn; // 方便管理,默认有2个按钮
        mBottomLay = new QHBoxLayout;
        mBottomLay->addStretch();
        mBottomLay->addWidget(mOkBtn->button());
        mBottomLay->addWidget(mCancelBtn->button());

         mLayout = new QVBoxLayout(this);
         mLayout->addLayout(mTopLay);
         mLayout->addLayout(mBottomLay);

         setSlotClassFunction(&Messagebox::accept,0);
         setSlotClassFunction(&Messagebox::reject,1);
}

Messagebox::~Messagebox()
{

}

void Messagebox::setDialogIcon(const QIcon &icon)
{
    setWindowIcon(icon);
}

void Messagebox::setDialogIcon(const QPixmap &pix)
{
    setWindowIcon(QIcon(pix));
}

void Messagebox::setDialogTitle(const QString & title)
{
    setWindowTitle(title);
}

void Messagebox::setDialogFixedSize(const QSize & size)
{
    setFixedSize(size);
}

void Messagebox::setDialogFixedSize(int w, int h)
{
    setFixedSize(w,h);
}

void Messagebox::setDialogFont(const QFont & font)
{
    setFont(font);
}

void Messagebox::setPixmap(const QIcon &icon,const QSize& size)
{
    mIconLabel->setPixmap(icon.pixmap(size));
}

void Messagebox::setPixmap(const QIcon &icon, int w, int h)
{
    mIconLabel->setPixmap(icon.pixmap(w,h));
}

void Messagebox::setPixmap(const QPixmap &pix)
{
    mIconLabel->setPixmap(pix);
}

void Messagebox::setPixmap(const QPixmap &pix, const QSize &size)
{
    QPixmap p = pix.scaled(size);
    mIconLabel->setPixmap(p);
}

void Messagebox::setPixmap(const QPixmap &pix, int w, int h)
{
    QPixmap p = pix.scaled(w,h);
    mIconLabel->setPixmap(p);
}

void Messagebox::setText(const QString &text)
{
    mTextEdit->setText(tr("%1").arg(text));
    setTextAlignment(Qt::AlignCenter);
}

void Messagebox::setTextAlignment(Qt::Alignment align)
{
    mTextEdit->setAlignment(align);
}

void Messagebox::setTextWeight(int w)
{
    //mTextEdit->setFontWeight(w);//不起作用
    QFont font = mTextEdit->font();
    font.setWeight(w);
    mTextEdit->setFont(font);
}

void Messagebox::setTextWeight(QFont::Weight w)
{
    QFont font = mTextEdit->font();
    font.setWeight(w);
    mTextEdit->setFont(font);
}

void Messagebox::setTextItalic(bool italic)
{
    //mTextEdit->setFontItalic(italic);//不起作用
    QFont font = mTextEdit->font();
    font.setItalic(italic);
    mTextEdit->setFont(font);
}

void Messagebox::setTextColor(const QColor &color)
{
    //mTextEdit->setTextColor(color);//不起作用
    QPalette pal = mTextEdit->palette();
    pal.setColor(QPalette::Text,color);
    mTextEdit->setPalette(pal);
}

void Messagebox::setTextFamily(const QString &family)
{
    mTextEdit->setFontFamily(family);
}

void Messagebox::setTextWrapIndent(int wrap,int indent)
{
    QList<QString> lines = mTextEdit->toPlainText()
            .split("\n",QString::SkipEmptyParts);

    QString indentstr;
    indentstr.fill(' ',indent);

    QString text = QString("").fill('\n',wrap); // wrap行
    text += indentstr; // indent缩进

    for (int row = 0; row < lines.size(); ++row)
    {
        text = text +lines.at(row) + "\n" + indentstr;
    }

    mTextEdit->setText(text);
}

void Messagebox::setTextBoxStyleSheet(const QString & style)
{
    mTextBox->setStyleSheet(style);
}

void Messagebox::setTextEditStyleSheet(const QString &style)
{
    mTextEdit->setStyleSheet(style);
}

void Messagebox::setPixmapBoxStyleSheet(const QString &style)
{
    mIconBox->setStyleSheet(style);
}

MessageButton* Messagebox::button(int id) const
{
    MessageButton * b = Q_NULLPTR;
    for(int i = 0; i < mBtns.size(); ++i)
    {
        if (mBtns.at(i)->id() == id)
        {
            b = mBtns.at(i);
            break;
        }
    }
    if (b == Q_NULLPTR)
    {
        Tip * tip = new Tip;
        tip->setTip(tr("警告![id=%1]的按钮不存在!\n"
                       "请检查操作的按钮编号是否正确~").arg(id));
        tip->exec();
        delete tip;
    }
    return b;
}

QList<MessageButton*> Messagebox::buttons() const
{
    return mBtns;
}

void Messagebox::removeButton(int id)
{
    if (id>=mBtns.size()) return; // 越界错误

    mBottomLay->removeWidget(mBtns.at(id)->button()); // 先移除布局的button
    //delete mBtns.at(id)->button(); //这步析构MessageButton的析构已经做了,而且必须要做
    delete  mBtns.at(id);
    mBtns.removeAt(id); // 再移除列表的button防止异常
}

void Messagebox::addButton(MessageButton * button)
{
    for(int i = 0; i < mBtns.size(); ++ i){
        if (button == mBtns.at(i))
        {
            Tip * tip = new Tip;
            tip->setTip(tr("该按钮[id=%1]已被添加,请不要重复添加!").arg(button->id()));
            tip->exec();
            delete tip;
            return;
        }
        if (button->id() == mBtns.at(i)->id())
        {
           Tip * tip = new Tip;
           QString text = tr("按钮编号[id=%1]已被使用,请选择新编号!\n"
                             "您可通过遍历按钮列表查看已有的编号~").arg(button->id());
           tip->setTip(text);
           tip->exec();
           delete tip;
           return;
        }
    }
    setButtonStyle(button,"color:rgb(18,150,220);border:1px solid rgb(18,150,220);font-size:24px;"
                           "font-family:Times New Roman;min-width: 120px;min-height: 40px;"); //默认保持一致(子类调用addButton需要自行调用setButtonStyle)
    mBottomLay->addWidget(button->button());
    mBtns << button;
}

void Messagebox::setButtonStyle(MessageButton * button, const QString &style)
{
     button->button()->setStyleSheet(style);
}

void Messagebox::shake(int duration,int range,int count,QEasingCurve::Type type)
{
       mShakeAnimation = new QPropertyAnimation(this,"geometry"); // 每次都需要重新创建,因为上次已被销毁
       QPoint initPos = pos();
       mShakeAnimation->setEasingCurve(type);
       mShakeAnimation->setDuration(duration);
       mShakeAnimation->setStartValue(QRect(initPos,size()));

       int nShakeCount = count; //抖动次数
       double nStep = 1.0/nShakeCount; // 每1次抖动是1步,要移动到新位置,0.05,0.1,...,0.95(1.0是最后1步)
       double r;

       for(int i = 1; i < nShakeCount; ++i){
           r = i&1 ? -range : range; // ,i&1只看最后1位,非0即1交替
           mShakeAnimation->setKeyValueAt(nStep*i,QRect(QPoint(initPos.x() + r,initPos.y()),size()));
           //qDebug()<<"i = "<<i<<" i&1 = "<<(i&1)<<" r = "<<r<<"  rect = "<<mShakeAnimation->keyValueAt(nStep*i);
       }
       mShakeAnimation->setEndValue(QRect(initPos,size())); // 结束的时候回到原位置(但是不代表pos回到了原来的位置)
       //qDebug()<<mShakeAnimation->keyValues();
       mShakeAnimation->start(QAbstractAnimation::DeleteWhenStopped); // 完成后动画指针删除
       //move(initPos);//在这里移动不起作用,要在使用shake函数的地方调用
       //qDebug()<<"after animation pos = "<<pos();
}

void Messagebox::setSlotClassFunction(ButtonSlotClassFunc p, int id)
{
    MessageButton * b = button(id);
    if ( b==Q_NULLPTR)  return;
    if (p == 0) return;
    connect(b,&MessageButton::clicked,this,p); // QPushButton=>MessageButton=>p
}

void Messagebox::setSlotFunction(ButtonSlotFunc p, int id)
{
    MessageButton * b = button(id);
    if (b == Q_NULLPTR)  return;
    if (p == 0) return;
//    if (id == 0)
//        disconnect(b,&MessageButton::clicked,this,&Messagebox::accept);
//    else if (id == 1)
//        disconnect(b,&MessageButton::clicked,this,&Messagebox::reject);
    // 按钮的使用者有义务自己解除绑定,否则调用本函数会导致1个clicked连接了多个函数
    // 不过这里只是连接accept/reject函数,这是一般都要连接的,所以这里注释掉上述2行代码不解除
    // 如果是比较复杂的函数有义务解除然后至少绑定1个accept或reject函数,再用另一个函数完成其它功能
    connect(b,&MessageButton::clicked,this,p);
}
