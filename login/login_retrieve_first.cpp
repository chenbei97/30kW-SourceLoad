#include "login_retrieve_password.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

RetrieveFirst::RetrieveFirst(QWidget * parent):QDialog(parent)
  , TipLabel(new QLabel(tr("第一步"))), UserNameLabel(new QLabel(tr("请输入用户名：")))
  , UserNameEdit(new QLineEdit), NextBtn(new QPushButton(tr("下一步")))
  , CancelBtn(new QPushButton(tr("取消")))
{
      setFont(QFont("Times New Roman",12));
      setWindowIcon(QIcon(":/images/login_forgetpwd.png"));
      initUI();
      initConnections();
}

void RetrieveFirst::paintEvent(QPaintEvent *e)
{
    QPainter p;
    p.begin(this);
    p.setOpacity(0.5);
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    QPixmap pix(":/images/forgetpwd_first.jpg");
    pix.scaled(size());
    p.drawPixmap(QPoint(0,0),pix);
    p.end();
    Q_UNUSED(e);
}

void RetrieveFirst::initUI()
{
      UserNameLabel->setBuddy(UserNameEdit);
      UserNameEdit->setFocus();
      NextBtn->setDefault(true);

      QHBoxLayout * lay1 = new QHBoxLayout;
      lay1->addWidget(TipLabel);
      lay1->addSpacerItem(new QSpacerItem(100,30));

      QHBoxLayout * lay2 = new QHBoxLayout;
      lay2->addWidget(UserNameLabel);
      lay2->addWidget(UserNameEdit);
      lay2->setAlignment(UserNameLabel,Qt::AlignRight);
      lay2->setAlignment(UserNameEdit,Qt::AlignLeft);

      QHBoxLayout * lay3 = new QHBoxLayout;
      lay3->addStretch();
      lay3->addWidget(NextBtn);
      lay3->addWidget(CancelBtn);

      QVBoxLayout * mainLayout = new QVBoxLayout(this);
      mainLayout->addLayout(lay1);
      mainLayout->addLayout(lay2);
      mainLayout->addLayout(lay3);
}

void RetrieveFirst::initConnections()
{
      connect(NextBtn,&QPushButton::clicked,this,[=]{
            if (UserNameEdit->text().isEmpty())
            {
                Warning * info = new Warning;
                info->setText(tr("用户名不允许为空!"));
                info->setTextWrapIndent(2,23);
                info->exec();
                delete  info;
                return;
            }
            QDir currentDir = QDir::current();
            currentDir.cdUp();
            QFile file(currentDir.path()+"/config.xml");
            if (!file.exists())
            {
                Warning * info = new Warning;
                info->setText(tr("该用户尚未注册,请先注册!"));
                info->setTextWrapIndent(2,23);
                info->exec();
                delete  info;
                reject();//这句放在emit之前否则会先执行创建窗口了
                emit createAccount();
                return;
            }

            if (!usernameIsExist(UserNameEdit->text()))
            {
                Warning * info = new Warning;
                info->setDialogFixedSize(1100,280);
                info->setText(tr("该用户并不存在,请重新输入用户名!"));
                info->setTextWrapIndent(2,29);
                info->exec();
                delete  info;
                UserNameEdit->clear();
                return;
            }
            QString password = findPassword();
            if (password.isEmpty())
            {
                Critical * info = new Critical;
                info->setText(tr("找不到密码,请重置密码!"));
                info->setTextWrapIndent(2,23);
                info->exec();
                delete  info;
                return;
            }
            accept();
            emit next(UserNameEdit->text(),password); // 用户名和密码传递给下级窗口来显示
      });
      connect(CancelBtn,&QPushButton::clicked,this,[=]{reject();});
}


bool RetrieveFirst::usernameIsExist(const QString& username)
{
      QDir currentDir = QDir::current();
      currentDir.cdUp();
      QFile file(currentDir.path()+"/config.xml");
      if (!file.exists()) return false;
      QXmlStreamReader stream(&file);
      file.open(QIODevice::ReadOnly);

      while (!stream.atEnd())
      {
          if (!file.isOpen()) file.open(QIODevice::ReadOnly);
          if (stream.qualifiedName() == "username")
          {
              if (stream.readElementText() == username)
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

QString RetrieveFirst::findPassword()
{
      QString password ;
      QDir currentDir = QDir::current();
      currentDir.cdUp();
      QFile file(currentDir.path()+"/config.xml");
      if (!file.exists()) return "";
      QXmlStreamReader stream(&file);
      file.open(QIODevice::ReadOnly);
      while (!stream.atEnd())
      {
          if (!file.isOpen()) file.open(QIODevice::ReadOnly);
          if (stream.qualifiedName() == "username")
          {
              QString username = stream.readElementText();
              if (username == UserNameEdit->text()) // 找到了这个用户名
              {
                  stream.readNext(); // 下一行不是密码,而是换行符
                  stream.readNext();// 再下一行就是密码了
                  password = stream.readElementText();
                  file.close();
              }
          }
          stream.readNext();
      }
      file.close();
      return password;
}
