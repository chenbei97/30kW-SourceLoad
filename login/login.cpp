#include "login.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

Login::Login(QWidget*parent ):QDialog(parent)
  , UserNameLabel(new QLabel(tr("用户名称："))), UserNameEdit(new QLineEdit)
  , PassWordLabel(new QLabel(tr("用户密码："))), PassWordEdit(new QLineEdit)
  , ForgetPassWordBtn(new QPushButton(tr("忘记密码")))
  , CreateCountBtn(new QPushButton(tr("注册用户")))
  , OkBtn(new QPushButton(tr("确定"))),CancelBtn(new QPushButton(tr("取消")))
  , IsExist(false),tryCount(0),mPaswordModifier(new RetrievePassword)
{
    setWindowFlags(Qt::WindowMinimizeButtonHint| Qt::WindowCloseButtonHint);
    initUI();
    initConnections();
}

void Login::initUI()
{
      UserNameLabel->setBuddy(UserNameEdit);
      //UserNameLabel->setFrameShape(QFrame::Box);
      UserNameLabel->setFrameShadow(QFrame::Sunken);
      UserNameLabel->setAlignment(Qt::AlignCenter);
      //UserNameLabel->setPalette(palette);
      PassWordLabel->setBuddy(PassWordEdit);
      //PassWordLabel->setFrameShape(QFrame::Box);
      PassWordLabel->setFrameShadow(QFrame::Sunken);
      PassWordLabel->setAlignment(Qt::AlignCenter);

      UserNameEdit->setFocus();
      UserNameEdit->setFrame(false);
      PassWordEdit->setEchoMode(QLineEdit::Password);
      PassWordEdit->setFrame(false);

      ForgetPassWordBtn->setIcon(QIcon(":images/login_forgetpwd.png"));
      ForgetPassWordBtn->setIconSize(QSize(32,32));
      CreateCountBtn->setIcon(QIcon(":images/login_newaccount.png"));
      CreateCountBtn->setIconSize(QSize(32,32));
      OkBtn->setDefault(true);

      setWindowIcon(QIcon(":/images/login_back.png"));
      setFont(QFont("Times New Roman",12));
      setFixedSize(500,270);
      setAttribute(Qt::WA_DeleteOnClose);
      //setWindowFlag(Qt::SplashScreen);
      //setWindowFlag(Qt::FramelessWindowHint);

      QGridLayout * layout0 = new QGridLayout;
      layout0->addWidget(UserNameLabel,0,0,Qt::AlignRight);
      layout0->addWidget(UserNameEdit,0,1,Qt::AlignCenter);
      layout0->addItem(new QSpacerItem(100,30),1,0);
      layout0->addItem(new QSpacerItem(100,30),1,1);
      layout0->addWidget(PassWordLabel,2,0,Qt::AlignRight);
      layout0->addWidget(PassWordEdit,2,1,Qt::AlignCenter);
      layout0->setAlignment(Qt::AlignCenter);
      layout0->setSpacing(5);

      QHBoxLayout * layout1 = new QHBoxLayout;
      layout1->addWidget(ForgetPassWordBtn,1,Qt::AlignRight);
      layout1->addWidget(CreateCountBtn,1,Qt::AlignLeft);

      QHBoxLayout * layout2 = new QHBoxLayout;
      layout2->addStretch();
      layout2->addWidget(OkBtn);
      layout2->addWidget(CancelBtn);

      QVBoxLayout * mainLayout = new QVBoxLayout(this);
      //mainLayout->addItem(new QSpacerItem(100,10));
      mainLayout->addLayout(layout0);
      mainLayout->addItem(new QSpacerItem(100,15));
      mainLayout->addLayout(layout1);
      mainLayout->addItem(new QSpacerItem(100,15));
      mainLayout->addLayout(layout2);
}

void Login::initConnections()
{
    // 如果有此信号传出来,说明需要先创建用户,之前没有用户无法修改密码
    connect(mPaswordModifier,&RetrievePassword::createAccount,CreateCountBtn,&QPushButton::click);

    connect(UserNameEdit,static_cast<void (QLineEdit::*)(const QString&)>(&QLineEdit::textChanged),
            this,[=](const QString&username){
                mUserName = username;
                if (!usernameIsExist())
                {UserNameEdit->setToolTip(tr("已检测到该用户名不存在,请重新输入!"));IsExist=false;}
                else
                {UserNameEdit->setToolTip(tr("该用户名存在."));IsExist=true;}
    });

    connect(ForgetPassWordBtn,&QPushButton::clicked,this,[=]{
              mPaswordModifier->process();
    });

    connect(CreateCountBtn,&QPushButton::clicked,this,[=]{
            Infomation * info = new Infomation;
            info->setDialogFixedSize(900,280);

            CreateAccount * dlg = new CreateAccount;
            int ret = dlg->exec();
            if (ret == QDialog::Accepted)
                info->setText(tr("创建新用户成功!"));
            else info->setText(tr("已取消创建新用户."));

            info->setTextWrapIndent(2,29);
            info->exec();

            delete  dlg;
            delete  info;
    });

    connect(OkBtn,&QPushButton::clicked,this,[=]{
            mUserName = UserNameEdit->text().trimmed();
            mPassWord = PassWordEdit->text().trimmed();
            if (mUserName.isEmpty() || mPassWord.isEmpty())
            {
                Critical * info = new Critical;
                info->setDialogFixedSize(1000,280);
                info->setText(tr("用户名或密码不允许为空!"));
                info->setTextWrapIndent(2,23);
                info->exec();
                delete  info;
                return;
            }
            bool isCorrect = passwordIsCorrect();
            if (!IsExist  || !isCorrect)
            {
                ++tryCount;
                Critical * info = new Critical;
                info->setDialogFixedSize(1000,280);
                info->setText(tr("用户名或密码不正确,请重新输入!"));
                info->setTextWrapIndent(2,23);
                info->exec();
                delete  info;
                UserNameEdit->clear();
                PassWordEdit->clear();
                if (tryCount>4)
                {
                    Critical * info = new Critical;
                    info->setDialogFixedSize(1000,280);
                    info->setText(tr("输入错误次数超过5次,强制退出!"));
                    info->setTextWrapIndent(2,23);
                    info->exec();
                    delete  info;
                    reject();
                }
                return;
            }
            accept();
    });

    connect(CancelBtn,SIGNAL(clicked()),this,SLOT(reject()));
}

void Login::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) //左键按下,说明想要拖动
    {
          mMouseMove = true;
          mMouseLastPos = e->globalPos() - pos(); // 鼠标在电脑屏幕的位置-窗口的位置等于相对窗口的位置
    }
    return QDialog::mousePressEvent(e);//事件转交给父类对话框窗口执行
}

void Login::mouseMoveEvent(QMouseEvent *e)
{
      bool isLeftButton = e->buttons() & Qt::LeftButton; // 是左键按下
      bool isDrag = (e->globalPos() - mMouseLastPos).manhattanLength()  > QApplication::startDragDistance();

      if (mMouseMove && isLeftButton && isDrag)
      {
          move(e->globalPos()-mMouseLastPos);//窗口移动到新的位置
          mMouseLastPos = e->globalPos()-pos();//新的相对位置
      }
      return QDialog::mouseMoveEvent(e);
}

void Login::mouseReleaseEvent(QMouseEvent *e)
{
      mMouseMove = false;
      Q_UNUSED(e);
}

void Login::paintEvent(QPaintEvent *e)
{
    QPainter p;
    p.begin(this);
    p.setOpacity(0.5);
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    QPixmap pix(":/images/login_back.jpg");
    pix.scaled(size());
    p.drawPixmap(QPoint(0,0),pix);
    p.end();
    Q_UNUSED(e);
}

bool Login::usernameIsExist()
{
      QDir currentDir = QDir::current();
      currentDir.cdUp();
      QFile file(currentDir.path()+"/config.xml");
      //qDebug()<<QFileInfo("config.xml").absoluteFilePath();
      if (!file.exists()) return false;
      QXmlStreamReader stream(&file);
      file.open(QIODevice::ReadOnly);
      while (!stream.atEnd())
      {
          if (!file.isOpen()) file.open(QIODevice::ReadOnly);
          if (stream.qualifiedName() == "username")
          {
              if (stream.readElementText() == mUserName)
              {
                  file.close();
                  return true;
              }
          }
          stream.readNext();
      }
      file.close();
      return false;
}

bool Login::passwordIsCorrect()
{
      QDir currentDir = QDir::current();
      currentDir.cdUp();
      QFile file(currentDir.path()+"/config.xml");
      if (!file.exists()) return false;
      QXmlStreamReader stream(&file);
      file.open(QIODevice::ReadOnly);
      while (!stream.atEnd())
      {
          if (stream.qualifiedName() == "username")
          {
              QString username = stream.readElementText();
              if (username == mUserName) // 用户名正确
              {
                  stream.readNext(); // 下一行不是密码,而是换行符
                  stream.readNext();// 再下一行就是密码了
                  QString pwd = stream.readElementText();
                  if (pwd == mPassWord) // 且密码正确
                  {
                          file.close();
                          return true;
                  }
              }
          }
          stream.readNext();
      }
      file.close();
      return false;
}
