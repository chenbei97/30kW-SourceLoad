#include "login_retrieve_password.h"
#if _MSC_VER >=1600
#pragma execution_character_set("utf-8")
#endif

RetrieveThird::RetrieveThird(QWidget * parent):QDialog(parent),TipLabel(new QLabel(tr("第三步")))
 , NewPwdLabel(new QLabel(tr("请输入新密码："))), NewPwdEdit(new QLineEdit)
 , OkBtn(new QPushButton(tr("重置"))), CancelBtn(new QPushButton(tr("取消")))
{
      setFont(QFont("Times New Roman",12));
      setFixedSize(QSize(450,150));
      setWindowIcon(QIcon(":/images/login_forgetpwd.png"));
      initUI();
      initConnections();
}

void RetrieveThird::paintEvent(QPaintEvent *e)
{
    QPainter p;
    p.begin(this);
    p.setOpacity(0.5);
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    QPixmap pix(":/images/forgetpwd_third.jpg");
    pix.scaled(size());
    p.drawPixmap(QPoint(0,0),pix);
    p.end();
    Q_UNUSED(e);
}

void RetrieveThird::getUseName(const QString &username)
{
    mUserName = username;
}

void RetrieveThird::initUI()
{
    QHBoxLayout * lay0 = new QHBoxLayout;
    lay0->addWidget(TipLabel);
    lay0->addSpacerItem(new QSpacerItem(100,30));

    QHBoxLayout * lay1 = new QHBoxLayout;
    lay1->addWidget(NewPwdLabel);
    lay1->addWidget(NewPwdEdit);

    QHBoxLayout * lay2 = new QHBoxLayout;
    lay2->addStretch();
    lay2->addWidget(OkBtn);
    lay2->addWidget(CancelBtn);

    QVBoxLayout * lay = new QVBoxLayout;
    lay->addLayout(lay0);
    lay->addLayout(lay1);
    lay->addLayout(lay2);
    setLayout(lay);

    NewPwdEdit->setFocus();
    QRegExp regName;
    regName.setPattern("^[0-9a-zA-Z_]+$");
    QValidator *validator = new QRegExpValidator(regName);
    NewPwdEdit->setValidator(validator);
    NewPwdEdit->setEchoMode(QLineEdit::Password);
    NewPwdEdit->setToolTip(tr("密码只允许输入数字英文字符和下划线!"));
    NewPwdLabel->setBuddy(NewPwdEdit);
}

void RetrieveThird::initConnections()
{
    connect(CancelBtn,&QPushButton::clicked,this,&RetrieveThird::reject); //关闭当前窗口

    connect(OkBtn,&QPushButton::clicked,this,[=]{
            // 重置密码
            if (NewPwdEdit->text().isEmpty())
            {
                Warning * info = new Warning;
                info->setDialogFixedSize(900,280);
                info->setText(tr("新密码不允许为空!"));
                info->setTextWrapIndent(2,28);
                info->exec();
                delete  info;
                return;
            }

            QDir currentDir = QDir::current();
            currentDir.cdUp();
            QFile file(currentDir.path()+"/config.xml");//能到三级窗口这个肯定是存在的
            if (!file.exists()) return;
            QDomDocument doc;
            doc.setContent(&file);
            QDomElement root = doc.documentElement();
            QDomNode node = root.firstChild();

            while (!node.isNull())
            {
                  QDomElement UserNameNode = node.toElement();
                  if (UserNameNode.text() == mUserName) //找到这个用户名
                  {
                      node = node.nextSibling(); // 下一个节点是旧密码
                      QDomNode oldPwdNode = node.firstChild(); // 拿到旧密码的文本节点
                      node.firstChild().setNodeValue(NewPwdEdit->text());//文本节点更新文字
                      QDomNode newPwdNode = node.firstChild();// 取出新密码节点
                      //qDebug()<<newPwdNode.toText().data(); // 可以打印出新密码的文本节点的文本
                      node.replaceChild(newPwdNode,oldPwdNode); // 替换2个元素节点,因为是替换孩子,所以父节点必须是node(元素节点)
                      // 只更改文本节点,属性id的值没有改变
                  }
                  node = node.nextSibling();
            }
           QString xml = doc.toString(4);
            QTextStream stream(&file);
            file.close(); // setContent已经打开了所以要先关闭
            file.open(QIODevice::Truncate|QIODevice::WriteOnly|QIODevice::Text);// 覆盖
            stream<<xml;
            file.close();

            accept();
    });
}
