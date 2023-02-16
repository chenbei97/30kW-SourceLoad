#include "login_create_account.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

CreateAccount::CreateAccount(QWidget * parent) : QDialog(parent)
 , mainLayout(new QGridLayout(this)),UserNameLabel(new QLabel(tr("用户名：")))
 , UserNameEdit(new QLineEdit),PassWordLabel(new QLabel(tr("密码：")))
 , PassWordEdit(new QLineEdit), OkBtn(new QPushButton(tr("创建"))), CancelBtn(new QPushButton(tr("取消")))
{
        setFont(QFont("Times New Roman",12));
        setWindowIcon(QIcon(":/images/login_newaccount.png"));

        QRegExp regName("^[a-zA-Z][a-zA-Z0-9_]+$");
        QValidator *validator1 = new QRegExpValidator(regName);
        UserNameEdit->setValidator(validator1);
        UserNameEdit->setToolTip(tr("用户名只允许英文字母开头,后面包含英文字符数字下划线!"));
        regName.setPattern("^[0-9a-zA-Z_]+$");
        QValidator *validator2 = new QRegExpValidator(regName);
        PassWordEdit->setValidator(validator2);
        PassWordEdit->setEchoMode(QLineEdit::Password);
        PassWordEdit->setToolTip(tr("密码只允许输入数字英文字符和下划线!"));

        mainLayout->addWidget(UserNameLabel,0,0);
        mainLayout->addWidget(UserNameEdit,0,1);
        mainLayout->addWidget(PassWordLabel,1,0);
        mainLayout->addWidget(PassWordEdit,1,1);

        QHBoxLayout * lay = new QHBoxLayout;
        lay->addStretch();
        lay->addWidget(OkBtn);
        lay->addWidget(CancelBtn);

        mainLayout->addLayout(lay,2,0,1,2);

        connect(OkBtn,&QPushButton::clicked,this,&CreateAccount::createAccount);
        connect(CancelBtn,SIGNAL(clicked()),this,SLOT(reject()));

}

void CreateAccount::paintEvent(QPaintEvent *e)
{
      QPainter p;
      p.begin(this);
      p.setOpacity(0.5);
      p.setRenderHint(QPainter::SmoothPixmapTransform);
      QPixmap pix(":/images/createAccount.jpg");
      pix.scaled(size());
      p.drawPixmap(QPoint(0,0),pix);
      p.end();
      Q_UNUSED(e);
}

void CreateAccount::createAccount()
{
      QString username = UserNameEdit->text().trimmed();
      QString pwd = PassWordEdit->text().trimmed();
      if (username.isEmpty() || pwd.isEmpty())
      {
          Critical * info = new Critical;
          info->setDialogFixedSize(1000,280);
          info->setText(tr("用户名或密码不允许为空!"));
          info->setTextWrapIndent(2,23);
          info->exec();
          delete  info;
          return;
      }

      if (usernameIsExist(username))
      {
          Warning * info = new Warning;
          info->setText(tr("用户名已存在!"));
          info->setTextWrapIndent(2,23);
          info->exec();
          delete  info;
          UserNameEdit->clear();
          PassWordEdit->clear();
          return;
      }

      QDir currentDir = QDir::current();
      currentDir.cdUp();
      QFile file(currentDir.path()+"/config.xml");

      // 有2种类可以用于写xml文档,这里分别展示,如果比较复杂的需要使用QDomDocument
      if (!file.exists()) // 如果是首次写入需要写入版本信息
      {
          QXmlStreamWriter wstream(&file);
          wstream.setAutoFormatting(true);
          wstream.setCodec("UTF-8");
          file.open(QIODevice::WriteOnly|QIODevice::Text);
          wstream.writeStartDocument("1.0"); // 文档的开始
          wstream.writeStartElement("user"); // 根元素的开始

          wstream.writeStartElement("username"); // 用户名元素
          wstream.writeAttribute("id", "1");//元素属性
          wstream.writeCharacters(username);
          wstream.writeEndElement();

          wstream.writeStartElement("password"); // 密码元素
          wstream.writeAttribute("id", "1");//元素属性
          wstream.writeCharacters(pwd);
          wstream.writeEndElement();

          //wstream.writeTextElement("username",username);
          //wstream.writeTextElement("password",pwd);
          wstream.writeEndElement(); // 根元素的结束
          wstream.writeEndDocument(); // 文档的结束
          file.close();
      }
      else{
                QDir currentDir = QDir::current();
                currentDir.cdUp();
                QDomDocument doc("config");
                doc.setContent(&file); // 获取了文件的内容
                QDomElement root = doc.documentElement(); // root.tagName()==user 根元素
                if(root.isNull()) // 有可能文件已存在但是为空,为了那么必须事先创建声明和根元素
                {
                    QDomProcessingInstruction p = doc.createProcessingInstruction("xml version=\"1.0\"","encoding=\"UTF-8\"");
                    QDomElement r =  doc.createElement("user");
                    doc.appendChild(p);
                    doc.appendChild(r);
                    root = doc.documentElement(); // 重新获取根元素
                }

                QDomNode node = root.firstChild();
                int n = 0; // 遍历获取子节点的个数=用户名+密码: 为了得到用户数n/2
                while (!node.isNull())
                {
                      //QDomElement elem = node.toElement();
                      //if (elem.tagName() == "username") qDebug()<<"username : "<<username.text();
                      //if (elem.tagName() == "password") qDebug()<<"password : "<<username.text();
                      node = node.nextSibling();
                      ++n;
                }

                QDomElement usernElemnode = doc.createElement("username"); // 新的用户名节点
                usernElemnode.setAttribute("id",n/2+1); // 用户数+1
                QDomText usernTextnode = doc.createTextNode(username); //新用户名的文本
                usernElemnode.appendChild(usernTextnode);

                QDomElement pwdElemnode = doc.createElement("password");// 新的密码节点
                pwdElemnode.setAttribute("id",n/2+1);
                QDomText pwdTextnode = doc.createTextNode(pwd); // 新密码的文本
                pwdElemnode.appendChild(pwdTextnode);

                root.appendChild(usernElemnode); // user根依次添加username和password节点
                root.appendChild(pwdElemnode);

                QString xml = doc.toString(4); // 缩进4字符, qDebug()<<"xml = "<<xml;
                QTextStream stream(&file);
                file.close(); // setContent已经打开了所以要先关闭
                while (!file.isOpen())
                    file.open(QIODevice::Truncate|QIODevice::WriteOnly|QIODevice::Text);// 覆盖
                stream<<xml;
                file.close();
      }

      accept();
}

bool CreateAccount::usernameIsExist(const QString& username)
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
